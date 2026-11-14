"""Recovery farm: keep many headless model workers translating, gate-test, accept.

A deterministic scheduler, not an LLM. Workflow and policy: docs/agent/FARM.md.

    py -3.13 scripts/cmre_farm.py run RUN [--workers 12] [--audit-workers 4] [--hours 10]
    py -3.13 scripts/cmre_farm.py status RUN
    py -3.13 scripts/cmre_farm.py stop RUN

Workers (``opencode run``) may only edit their scratch source and their own hint
file, and may only run read-only cmre commands plus ``cmre test --source``. The
farm re-tests every candidate itself, copies passes into recovery/c/, runs the
canonical test, then ``cmre accept`` serially. Failures restore the exact prior
bytes and escalate up the model ladder. Accepted functions are audited
asynchronously; a defect gets one fix attempt, otherwise it is reported open.
"""
from __future__ import annotations

import argparse
from dataclasses import dataclass, field
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))

# 2026-11-13 head-to-head, 300-1500 B, opencode 2.x: MiniMax M3.1 Flash 6/11 passes (free plan),
# MiniMax M3 3/13 (dropped), DeepSeek v4.1 Flash #low 4/12 then default effort 2/4; DeepSeek
# solved functions MiniMax failed. All arms ~1 KB/h per worker, bounded by reasoning tokens.
# DeepSeek runs use v4.1 Flash (Adam's choice); fix jobs use the top rung.
DEFAULT_LADDER = ('minimax-coding-plan/MiniMax-M3.1-Flash', 'opencode-go/deepseek-v4.1-flash#low',
                  'opencode-go/deepseek-v4.1-flash')
DEFAULT_AUDIT_MODEL = 'opencode-go/deepseek-v4.1-flash'
# A defect needs a second auditor before any fix: the MiniMax M3 auditor flagged 8 of 8 functions
# with false positives, DeepSeek #low passed real defects (cross-audits, 2026-11-13). The second
# opinion is a separate Flash run that must prove each finding from the listing.
DEFAULT_CONFIRM_MODEL = 'opencode-go/deepseek-v4.1-flash'
# The coordinator must outlive an OpenCode Go usage limit, so it runs on the MiniMax plan.
COORDINATOR_MODEL = 'minimax-coding-plan/MiniMax-M3.1-Flash'
# Runtime-library x87 helpers: 7 of 10 production audit defects (control word, ST0, caller
# slots the gate cannot see), 99 functions / 18 KB of supply (2026-11-13).
DEFAULT_SKIP_RANGES = ((0x00940000, 0x00953000),)
# Events that wake a waiting coordinator (`wait`).
NOTABLE_EVENTS = ('farm-exit', 'open-defect', 'model-unavailable', 'model-limited', 'supply-empty',
                  'request-failed')
# OpenCode Go has a 5-hour usage limit per account (HTTP 429, hit 2026-11-13 evening); it pauses
# every model of that provider until it resets. Without a reset time, retry after this long.
DEFAULT_LIMIT_SECONDS = 900
STALE_SECONDS = 600   # state.json is saved every tick; older than this means the farm is gone
HARD_HOLDS = ('harness-blocker', 'evidence-needed')
MAX_FIX_ROUNDS = 1
MAX_AUDIT_ROUNDS = 2
DEFAULT_STALL_MINUTES = 15   # opencode 2.x is silent while a step thinks; 8 min killed real work
FRONTIER_LIMIT = 5000   # cmre returns smallest-first; ask for the whole queue
PY = 'py -3.13'

WORKER_COMMANDS = ('brief', 'test', 'explain', 'show', 'disasm', 'family', 'work show', 'trace-check')
# rg honours .gitignore, so it skips the multi-GB CM0102/ and .worktrees/ trees; a recursive
# grep over them hung a worker for 23 minutes (2026-11-13 smoke). No grep/ls/find.
DEFAULT_EFFORT = 'low'
READ_ONLY_SHELL = ('rg *', 'cat *', 'head *', 'tail *', 'sed -n *', 'wc *', 'Get-Content *')


@dataclass
class Item:
    fn: str
    bytes: int
    ready: bool
    waits: int
    module: str


@dataclass
class FarmConfig:
    workers: int
    ladder: list
    audit_workers: int
    audit_model: str | None
    attempt_minutes: float
    hours: float | None   # None: no deadline (status view)
    min_bytes: int
    max_bytes: int | None
    include_held: bool
    owner: str
    stall_minutes: float = 15   # DEFAULT_STALL_MINUTES
    confirm_model: str | None = None
    skip_ranges: tuple = ()


def parse_ranges(text):
    """'0xLO-0xHI,...' -> [(lo, hi)], half-open VA ranges."""
    ranges = []
    for part in (p.strip() for p in (text or '').split(',')):
        if part:
            lo, hi = part.split('-')
            ranges.append((int(lo, 16), int(hi, 16)))
    return ranges


def _skipped(fn, ranges):
    va = int(fn[3:], 16)
    return any(lo <= va < hi for lo, hi in ranges)


def parse_queue(text):
    """Parse `cmre next --c` lines: `fn_X  NNNB  ready|waits:N  module`."""
    items = []
    for line in text.splitlines():
        m = re.match(r'\s*(fn_[0-9a-f]{8})\s+(\d+)B\s+(ready|waits:(\d+))\s*(.*)$', line)
        if m:
            items.append(Item(m.group(1), int(m.group(2)), m.group(3) == 'ready',
                              int(m.group(4) or 0), m.group(5).strip()))
    return items


# --- mechanical source checks ------------------------------------------------

_NONCODE = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|/\*.*?\*/|//[^\n]*', re.S)
_BANNED = (
    (re.compile(r'\b(?:asm|__asm|__asm__)\b'), 'inline assembly (plain C only)'),
    (re.compile(r'__attribute__\s*\(\(\s*(?:__)?optimize'), 'optimize attribute steers codegen, not evidence'),
    (re.compile(r'\bregister\b'), 'register storage class steers codegen, not evidence'),
    (re.compile(r'\bCM_ORIGINAL_CALL\b'), 'CM_ORIGINAL_CALL belongs to legacy abi records only'),
    (re.compile(r'\bnaked\b'), 'naked function'),
)


def mechanical_problems(fn, text):
    """Cheap rejections that never need the gate. Comments and strings are ignored."""
    code = _NONCODE.sub(lambda m: ' ' * len(m.group()), text.replace('\\\r\n', '').replace('\\\n', ''))
    problems = [message for pattern, message in _BANNED if pattern.search(code)]
    if re.search(r'/\*\s*WIP', text):
        problems.append('WIP marker present: the source is not finished')
    if not re.search(r'\bcm_%s\s*\(' % re.escape(fn), code):
        problems.append(f'missing entry definition cm_{fn}(...)')
    return problems


# --- worker configuration and prompts -----------------------------------------

def _rel(root, path):
    try:
        return Path(path).resolve().relative_to(Path(root).resolve()).as_posix()
    except ValueError:
        return Path(path).as_posix()


def worker_config(root, job, effort=DEFAULT_EFFORT):
    """OpenCode config: read anything, edit only the job's own files, run an allowlist.

    effort sets the job model's reasoning effort. MiniMax at its default effort spent the
    whole 32k output cap on one reasoning step and wrote nothing (2026-11-13 smoke).
    """
    fn = job['fn']
    own = job['audit_path'] if job['kind'] == 'audit' else job['source']
    edit = {'*': 'deny'}
    for name in {_rel(root, own), Path(own).as_posix(), str(Path(own))}:
        edit[name] = 'allow'
    if job['kind'] != 'audit':
        edit[f'recovery/hints/{fn}.json'] = 'allow'
    bash = {'*': 'deny'}
    for command in WORKER_COMMANDS:
        bash[f'{PY} -m cmre {command} *'] = 'allow'
    bash[f'{PY} scripts/cmre_hint_check.py *'] = 'allow'
    for command in READ_ONLY_SHELL:
        bash[command] = 'allow'
    config = {
        'permission': {'edit': edit, 'bash': bash, 'read': 'allow', 'webfetch': 'deny'},
        'plugin': [], 'snapshot': False, 'autoupdate': False, 'share': 'disabled',
        'tools': {'task': False, 'skill': False, 'webfetch': False, 'question': False},
    }
    if effort and '/' in job.get('model', ''):
        provider, model = job['model'].split('/', 1)
        config['provider'] = {provider: {'models': {model: {
            'options': {'effort': effort, 'reasoningEffort': effort}}}}}
    return config


def v2_agent(root, job):
    """OpenCode 2.x agent definition with the same per-job limits as worker_config().

    2.x reads permissions from agent files, not OPENCODE_CONFIG_CONTENT; exact relative edit
    paths and shell globs were verified to be enforced on 2026-11-13.
    """
    fn = job['fn']
    own = job['audit_path'] if job['kind'] == 'audit' else job['source']
    rules = [('*', '*', 'deny'), ('read', '*', 'allow'), ('grep', '*', 'allow'), ('glob', '*', 'allow'),
             ('edit', _rel(root, own), 'allow')]
    if job['kind'] != 'audit':
        rules.append(('edit', f'recovery/hints/{fn}.json', 'allow'))
    shell = [f'{PY} -m cmre {command} *' for command in WORKER_COMMANDS]
    shell += [f'{PY} scripts/cmre_hint_check.py *', *READ_ONLY_SHELL]
    rules += [('shell', command, 'allow') for command in shell]
    lines = ['---', f"description: CM0102 farm {job['kind']} {fn}", 'mode: primary', 'permissions:']
    lines += [f'  - {{ action: "{a}", resource: "{r}", effect: {e} }}' for a, r, e in rules]
    lines += ['---', 'You are a CM0102 farm worker. Follow the user message exactly.', '']
    return '\n'.join(lines)


def _hold_text(hold):
    if not hold:
        return ''
    return (f"\nThis function is HELD ({hold.get('state')}): {hold.get('reason', '')}"
            + (f" Retry when: {hold['retry_when']}" if hold.get('retry_when') else '')
            + '\nYour source must resolve that concern explicitly; say how in your final line.\n')


def _file(root, relative):
    try:
        return (Path(root) / relative).read_text(encoding='utf-8', errors='replace')
    except OSError:
        return ''


def _section(text, start, stop):
    """The first line matching `start` and the lines after it, up to the next line matching `stop`."""
    lines = text.splitlines()
    for i, line in enumerate(lines):
        if re.match(start, line):
            out = [line]
            for following in lines[i + 1:]:
                if re.match(stop, following):
                    break
                out.append(following)
            return '\n'.join(out)
    return ''


def _fenced(title, body, lang=''):
    return ['', f'## {title}', f'```{lang}', body.rstrip(), '```'] if body.strip() else []


def bundle_text(root, job, brief):
    """One file with everything a job reads: workers spent 5-8 of ~20 steps re-reading the
    brief, draft, header and rules (2026-11-13), and every step costs reasoning tokens."""
    fn, kind = job['fn'], job['kind']
    farm_md = _file(root, 'docs/agent/FARM.md')
    checklist = _section(farm_md, r'## Audit checklist', r'## ')
    parts = [f'# {fn} {kind} bundle',
             'Generated by the farm for this job. It replaces running cmre brief and reading the '
             'rules, header, draft or source separately.']
    parts += _fenced('cmre brief (evidence, callers, callees, globals, listing)', brief)
    if kind == 'audit':
        parts += ['', checklist]
        parts += _fenced(f'Accepted source recovery/c/{fn}.c', _file(root, f'recovery/c/{fn}.c'), 'c')
    else:
        parts += ['', '## Rules (docs/agent/C.md, Loop step 2)',
                  _section(_file(root, 'docs/agent/C.md'), r'2\. Write ', r'3\. '),
                  '', _section(farm_md, r'## Worker rules', r'## ')]
        if kind == 'fix':
            parts += ['', checklist]
        draft = re.search(r'^draft: (\S+)', brief, re.M)
        if draft:
            parts += _fenced(f'Draft {draft.group(1)}', _file(root, draft.group(1)), 'c')
    parts += _fenced('Header recovery/include/cmre.h', _file(root, 'recovery/include/cmre.h'), 'c')
    return '\n'.join(parts) + '\n'


def _bundle_steps(job, first):
    """Prompt lines that point at the bundle, or the classic brief step without one."""
    if job.get('bundle'):
        return [f"1. Read {job['bundle']} completely, once. It holds the brief with the full listing, "
                f"{first}, and recovery/include/cmre.h. Do not rerun the brief or reread those files."]
    return [f"1. {PY} -m cmre brief {job['fn']}   (evidence, callers, callees, globals, draft path, listing)"]


def translate_prompt(root, job):
    fn, src = job['fn'], _rel(root, job['source'])
    lines = [
        f'CM0102 farm worker. Translate {fn} ({job["bytes"]} bytes, {job.get("module") or "unknown module"}) '
        'to plain C that passes the semantic gate. Do the work now; do not reply with a plan.',
        '',
        f'Write your C ONLY to {src}. You may also edit recovery/hints/{fn}.json for c_hints fixtures. '
        'Every other edit and command is denied.',
    ]
    if not job.get('bundle'):
        lines += ['Read docs/agent/FARM.md section "Worker rules" once before writing.']
    lines += [
        'Everything you need is the brief, the draft it names, the C.md Rules, and the header '
        'recovery/include/cmre.h (types, CM_GLOBAL, CM_THISCALL, CM_FS). Do not search the repository, '
        'list directories or read cmre/ Python sources.',
        'Write a first complete version within your first few steps and test it early; iterate from test output.',
        '',
        *_bundle_steps(job, 'the draft, the C.md rules, the worker rules'),
        f'2. Write {src} from the draft or the starting source below; check every statement against the listing.',
        f'3. {PY} -m cmre test {fn} --c --source {src}',
        f'4. On a mismatch: {PY} -m cmre explain {fn} --c --source {src} --case N   then fix the first difference.',
        '   Stop after 5 failed tests. Build failures count.',
        '',
        'Finish with exactly one line: FARM-RESULT: pass | FARM-RESULT: fail <first remaining mismatch>',
    ]
    if job.get('prior'):
        p = job['prior']
        lines += ['', f'Previous attempt ({p["model"]}) failed: {p["detail"][:1500]}',
                  f'Its source is already in {src}; start from it.']
    elif job.get('seeded'):
        lines += ['', f'{src} already holds an earlier unaccepted source; start from it.']
    return '\n'.join(lines) + _hold_text(job.get('hold')) + '\n'


def fix_prompt(root, job):
    fn, src = job['fn'], _rel(root, job['source'])
    findings = json.dumps(job.get('findings', []), indent=1)[:4000]
    return '\n'.join([
        f'CM0102 farm fixer. {fn} passed the gate and was accepted, but an audit found fidelity defects '
        'the gate cannot see. Do the work now.',
        '',
        f'The accepted source is already in {src}. Edit ONLY that file (and recovery/hints/{fn}.json).',
        *([] if job.get('bundle') else ['Read docs/agent/FARM.md sections "Worker rules" and "Audit checklist".']),
        *_bundle_steps(job, 'the draft, the C.md rules, the worker rules, the audit checklist'),
        '   Verify each finding against the listing. A finding without listing evidence may be a false '
        'positive: keep the original code for it.',
        f'2. Fix confirmed defects, then {PY} -m cmre test {fn} --c --source {src} until it passes (5 failures max).',
        '',
        'Audit findings:', findings, '',
        'Finish with exactly one line: FARM-RESULT: pass <what changed> | FARM-RESULT: fail <why>',
    ]) + '\n'


def audit_prompt(root, job):
    fn, out = job['fn'], _rel(root, job['audit_path'])
    held = _hold_text(job.get('hold')).replace('Your source must resolve that concern explicitly; say how in your final line.',
                                               'Check specifically that the accepted source resolves it.')
    return '\n'.join([
        f'CM0102 farm auditor. recovery/c/{fn}.c passed the differential gate and was accepted. '
        'Find fidelity defects the gate cannot see. Do the work now; you cannot edit sources.',
        '',
        'Use at most 20 tool calls, then write the verdict file. A finding you cannot prove from the '
        'listing within that budget is not a finding.',
        *(_bundle_steps(job, 'the audit checklist, the accepted source') if job.get('bundle') else [
            'Read docs/agent/FARM.md section "Audit checklist", then:',
            f'1. {PY} -m cmre brief {fn}   (it includes the listing) and read recovery/c/{fn}.c']),
        '2. Walk every checklist item against the listing. Callee sources in recovery/c/ are fair to read '
        'for their signatures. ' + ('Apart from the bundle, do' if job.get('bundle') else 'Do')
        + ' not read farm run files, the ledger, transcripts or directories.',
        f'3. Write {out} as JSON: {{"verdict": "ok"|"defect", "findings": '
        '[{"line": N, "detail": "...", "evidence": "listing VA and instruction"}]}',
        '   Report a defect only with concrete listing evidence. Style, naming and comments are not defects.',
        '',
        'Finish with exactly one line: FARM-AUDIT: ok | FARM-AUDIT: defect <count>',
    ]) + held + '\n'


def confirm_prompt(root, job):
    """Second-opinion audit: verify another auditor's findings, keep only the proven ones."""
    fn, out = job['fn'], _rel(root, job['audit_path'])
    findings = json.dumps(job.get('findings', []), indent=1)[:6000]
    return '\n'.join([
        f'CM0102 farm second auditor. recovery/c/{fn}.c passed the differential gate and was accepted. '
        f"Another auditor ({job.get('first_model') or 'unknown model'}) reported the findings below. "
        'Do the work now; you cannot edit sources.',
        'Auditors produce false positives: for example reading `cmp si, cx` as a 32-bit compare, or calling '
        'a truncation before a 16-bit compare a behaviour change. Verify every finding independently '
        'against the listing instructions; do not trust its wording.',
        '',
        'Use at most 20 tool calls, then write the verdict file.',
        *(_bundle_steps(job, 'the audit checklist, the accepted source') if job.get('bundle') else [
            f'1. {PY} -m cmre brief {fn}   (it includes the listing) and read recovery/c/{fn}.c']),
        '2. For each finding, confirm it with the listing VA and instruction, or reject it.',
        f'3. Write {out} as JSON: {{"verdict": "defect"|"ok", "findings": [only the confirmed findings, '
        'each with "line", "detail" and "evidence"]}. "ok" when none is confirmed.',
        '',
        'Findings to verify:', findings, '',
        'Finish with exactly one line: FARM-AUDIT: ok | FARM-AUDIT: defect <count>',
    ]) + '\n'


# --- the scheduler ---------------------------------------------------------------

def _read(path):
    path = Path(path)
    return path.read_bytes() if path.exists() else None


def _restore(path, data):
    path = Path(path)
    if data is None:
        path.unlink(missing_ok=True)
    else:
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(data)


def _validate_worker_hints(before, after):
    """Only c_hints may differ from the dispatch snapshot; never repair worker input."""
    if before == after:
        return
    if after is None:
        raise ValueError('worker deleted the hints file')
    old = json.loads(before) if before is not None else {}
    new = json.loads(after)
    if not isinstance(old, dict) or not isinstance(new, dict):
        raise ValueError('hints must be objects')
    shared = lambda hints: json.dumps({k: v for k, v in hints.items() if k != 'c_hints'},
                                     sort_keys=True, allow_nan=False)
    if shared(old) != shared(new):
        raise ValueError('worker changed shared hints outside c_hints')
    from cmre import difftest
    # Validate only the authorized proposal under worker rules. Existing shared
    # policy/bindings are frozen, not worker proposals; preserve inherited input
    # dimensions so fixture validation uses the actual C argument/arena sizes.
    dimensions = {k: new[k] for k in ('arg_count', 'arena_size') if k in new}
    difftest.validate_hints({**dimensions, 'c_hints': new.get('c_hints', {})}, worker=True)


def report_detail(report):
    parts = [str(report.get('status'))]
    parts += [str(p) for p in report.get('problems') or []]
    for opt, build in (report.get('builds') or {}).items():
        if isinstance(build, dict) and build.get('status') not in (None, 'pass'):
            parts.append(f"{opt} {build.get('status')}: {build.get('detail', '')}")
            if build.get('stderr'):
                parts.append(str(build['stderr'])[:800])
    return ' | '.join(parts)[:2500]


class InlineExecutor:
    """Runs submitted work immediately: the default, and what the tests rely on."""

    def submit(self, fn, *args):
        from concurrent.futures import Future
        future = Future()
        try:
            future.set_result(fn(*args))
        except Exception as exc:   # surfaced by Farm._collect as a failed verification
            future.set_exception(exc)
        return future


class Farm:
    def __init__(self, run_dir, root, cmre, launcher, config, clock=time.monotonic, verifier=None):
        self.run_dir, self.root, self.cmre, self.launcher, self.config = Path(run_dir), Path(root), cmre, launcher, config
        self.clock = clock
        # Gate re-tests (median 48 s per job) run here so they never block dispatch.
        self.verifier = verifier or InlineExecutor()
        self.verifying = []        # (job, future)
        self.supply_empty = False
        # cmre lets only the claim owner run --source diagnostics on a claimed function;
        # the farm's re-tests and its workers (inherited env) test as this owner.
        os.environ['CMRE_CLAIM_OWNER'] = config.owner
        self.run_dir.mkdir(parents=True, exist_ok=True)
        self.state_path = self.run_dir / 'state.json'
        if self.state_path.exists():
            self.state = json.loads(self.state_path.read_text(encoding='utf-8'))
        else:
            self.state = {'schema': 'cmre-farm/1', 'started_at': time.time(), 'owner': config.owner,
                          'attempts': {}, 'accepted': {}, 'parked': {}, 'skipped': {}, 'audits': {},
                          'defects': {}, 'fix_rounds': {}, 'unavailable_models': {}}
        self.state.setdefault('unavailable_models', {})
        self.state.setdefault('limited_providers', {})
        self.deadline = None if config.hours is None else clock() + config.hours * 3600
        self.running = []          # (job, handle, started)
        self.pending = []          # requeued translate/fix jobs, highest priority first
        self.accept_queue = []
        # Audits are not persisted as a queue: on (re)start, every accepted function without a
        # verdict that still has audit rounds left is audited again.
        self.audit_queue = [fn for fn in self.state['accepted']
                            if config.audit_workers and self._needs_audit(self.state['audits'].get(fn, {}))]
        self.frontier = None
        self.seen = set(self.state['accepted']) | set(self.state['parked']) | set(self.state['skipped'])
        self.counter = sum(len(v) for v in self.state['attempts'].values())

    @staticmethod
    def _needs_audit(audit):
        if audit.get('verdict') in ('ok', 'defect'):
            return False
        if audit.get('verdict') == 'unconfirmed':
            return audit.get('confirm_rounds', 0) < MAX_AUDIT_ROUNDS
        return audit.get('rounds', 0) < MAX_AUDIT_ROUNDS

    # -- bookkeeping
    def _event(self, event_name, **data):
        with (self.run_dir / 'events.jsonl').open('a', encoding='utf-8') as out:
            out.write(json.dumps({'at': time.time(), 'event': event_name, **data}) + '\n')

    def save(self):
        tmp = self.state_path.with_suffix('.tmp')
        tmp.write_text(json.dumps(self.state, indent=1), encoding='utf-8')
        tmp.replace(self.state_path)

    def stopping(self):
        return (self.run_dir / 'STOP').exists() or (self.deadline is not None and self.clock() >= self.deadline)

    def finished(self):
        return self.stopping() and not self.running and not self.accept_queue and not self.verifying

    def summary(self):
        accepted = self.state['accepted']
        by_model = {}
        for rows in self.state['attempts'].values():
            for row in rows:
                if row.get('kind') == 'audit':   # verdicts are not translation attempts
                    continue
                m = by_model.setdefault(row['model'], {'attempts': 0, 'passes': 0})
                m['attempts'] += 1
                m['passes'] += row['outcome'] == 'pass'
        hours = max((time.time() - self.state['started_at']) / 3600, 1e-9)
        total = sum(a['bytes'] for a in accepted.values())
        audits = [a.get('verdict') for a in self.state['audits'].values()]
        return {'accepted_functions': len(accepted), 'accepted_bytes': total,
                'bytes_per_hour': round(total / hours), 'hours': round(hours, 2), 'by_model': by_model,
                'running': [f"{j['fn']}:{j['kind']}:{j['model']}" for j, _, _ in self.running],
                'pending': len(self.pending), 'parked': len(self.state['parked']),
                'skipped': len(self.state['skipped']),
                'audits_ok': audits.count('ok'),
                'audits_defect': audits.count('defect'),
                'audit_errors': audits.count('error'),
                'audits_unconfirmed': audits.count('unconfirmed'),
                'audits_disputed': sum(a.get('verdict') == 'ok' and bool(a.get('disputed') or a.get('dismissed'))
                                       for a in self.state['audits'].values()),
                'open_defects': sorted(self.state['defects']),
                'unavailable_models': dict(self.state['unavailable_models']),
                'limited': {p: v['until'] for p, v in self.state.get('limited_providers', {}).items()
                            if v['until'] > time.time()},
                'stopping': self.stopping()}

    # -- supply
    def _next_item(self):
        if self.frontier is None or not self.frontier:
            items = self.cmre.frontier(FRONTIER_LIMIT, self.config.min_bytes, self.config.max_bytes,
                                       self.config.include_held, self.config.owner)
            items = [i for i in items if i.fn not in self.seen and not _skipped(i.fn, self.config.skip_ranges)]
            held = self.cmre.held([i.fn for i in items]) if self.config.include_held else set()
            # Bytes are the goal: unheld first, then callee-ready, then largest.
            self.frontier = sorted(items, key=lambda i: (i.fn in held, not i.ready, -i.bytes, i.fn))
        while self.frontier:
            item = self.frontier.pop(0)
            if item.fn in self.seen:
                continue
            self.seen.add(item.fn)
            try:
                self.cmre.claim(item.fn, self.config.owner, max(self.config.hours or 1, 1))
            except ValueError as exc:
                self.state['skipped'][item.fn] = str(exc)[:300]
                self._event('skipped', fn=item.fn, reason=str(exc)[:300])
                continue
            return item
        return None

    def _limited_until(self, model):
        """Wall-clock end of the model's provider usage limit, or None when it is usable."""
        limit = self.state['limited_providers'].get(model.split('/', 1)[0])
        return limit['until'] if limit and limit['until'] > time.time() else None

    def _available(self, model):
        return model not in self.state['unavailable_models'] and self._limited_until(model) is None

    def _rung_model(self, rung):
        """First available ladder model at or above rung, or (None, rung) when none remain."""
        while rung < len(self.config.ladder):
            if self._available(self.config.ladder[rung]):
                return self.config.ladder[rung], rung
            rung += 1
        return None, rung

    def _next_pending(self):
        """The first requeued job some rung at or above its own can run now. A job whose
        remaining rungs are only paused by a usage limit waits; one with none left is parked."""
        i = 0
        while i < len(self.pending):
            job = self.pending[i]
            model, rung = self._rung_model(job['rung'])
            if model is not None:
                job.update(model=model, rung=rung)
                return self.pending.pop(i)
            if not any(self._limited_until(m) for m in self.config.ladder[job['rung']:]
                       if m not in self.state['unavailable_models']):
                self.pending.pop(i)
                self._park(job['fn'], 'no available model on the ladder')
                continue
            i += 1
        return None

    def _new_job(self, kind, fn, model, rung=0, **extra):
        self.counter += 1
        job = {'kind': kind, 'fn': fn, 'model': model, 'rung': rung, 'n': self.counter, **extra}
        if kind == 'audit':
            name = f'{fn}.confirm.json' if extra.get('confirm') else f'{fn}.json'
            job['audit_path'] = str(self.run_dir / 'audit' / name)
        else:
            job['source'] = str(self.run_dir / 'src' / f'{fn}.c')
        return job

    def _launch(self, job):
        c_path = self.root / 'recovery/c' / f"{job['fn']}.c"
        hint_path = self.root / 'recovery/hints' / f"{job['fn']}.json"
        job['_snap'] = {'c': _read(c_path), 'hint': _read(hint_path)}
        job.pop('bundle', None)
        bundle = self._bundle(job)
        if bundle is not None:
            job['bundle'] = _rel(self.root, bundle)
        if job['kind'] == 'audit':
            Path(job['audit_path']).parent.mkdir(parents=True, exist_ok=True)
            Path(job['audit_path']).unlink(missing_ok=True)
            prompt = confirm_prompt(self.root, job) if job.get('confirm') else audit_prompt(self.root, job)
        else:
            source = Path(job['source'])
            source.parent.mkdir(parents=True, exist_ok=True)
            seed = job.pop('_seed', None)
            if seed is None and job['kind'] == 'translate' and not job.get('prior') and job['_snap']['c'] is not None:
                seed = job['_snap']['c']
                job['seeded'] = True
            _restore(source, seed)
            job['_seed_bytes'] = seed
            prompt = fix_prompt(self.root, job) if job['kind'] == 'fix' else translate_prompt(self.root, job)
        handle = self.launcher.start(job, prompt, worker_config(self.root, job))
        self.running.append((job, handle, self.clock()))
        self._event('dispatch', fn=job['fn'], kind=job['kind'], model=job['model'], n=job['n'])

    def _bundle(self, job):
        """Write the job's bundle file; None (classic prompt) when the brief is unavailable."""
        brief = getattr(self.cmre, 'brief', None)
        if brief is None:
            return None
        try:
            text = brief(job['fn'])
        except Exception as exc:   # a broken brief must not stop the farm; the worker runs it itself
            self._event('bundle-failed', fn=job['fn'], error=str(exc)[:300])
            return None
        if not text:
            return None
        path = self.run_dir / 'bundle' / f"{job['fn']}.md"
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(bundle_text(self.root, job, text), encoding='utf-8')
        return path

    def _dispatch(self):
        if self.stopping():
            return
        translators = [j for j, _, _ in self.running if j['kind'] != 'audit']
        while len(translators) < self.config.workers:
            job = self._next_pending()
            if job is None:
                if self._rung_model(0)[0] is None:
                    break
                item = self._next_item()
                if item is None:
                    if not self.supply_empty and not self.pending:
                        self.supply_empty = True
                        self._event('supply-empty', running=len(translators))
                    break
                hold = self.cmre.hold(item.fn) if self.config.include_held else None
                model, rung = self._rung_model(0)
                job = self._new_job('translate', item.fn, model, rung, bytes=item.bytes,
                                    module=item.module, hold=hold)
            self._launch(job)
            translators.append(job)
        audits = [j for j, _, _ in self.running if j['kind'] == 'audit']
        while len(audits) < self.config.audit_workers and self.audit_queue and self.config.audit_model:
            fn = self.audit_queue[0]
            prior = self.state['audits'].get(fn, {})
            confirm = prior.get('verdict') == 'unconfirmed' and self.config.confirm_model
            if not self._available(self.config.confirm_model if confirm else self.config.audit_model):
                break   # paused by a usage limit (or retired); the queue waits
            self.audit_queue.pop(0)
            accepted = self.state['accepted'][fn]
            if confirm:
                job = self._new_job('audit', fn, self.config.confirm_model, bytes=accepted['bytes'],
                                    hold=accepted.get('hold'), confirm=True,
                                    findings=prior.get('findings', []), first_model=prior.get('model'))
            else:
                job = self._new_job('audit', fn, self.config.audit_model, bytes=accepted['bytes'],
                                    hold=accepted.get('hold'))
            self._launch(job)
            audits.append(job)

    # -- results
    def _record(self, job, outcome, detail):
        self.state['attempts'].setdefault(job['fn'], []).append(
            {'model': job['model'], 'kind': job['kind'], 'outcome': outcome, 'detail': detail[:600], 'at': time.time()})
        self._event('result', fn=job['fn'], kind=job['kind'], model=job['model'], n=job['n'],
                    outcome=outcome, detail=detail[:300])

    def _fail(self, job, detail):
        snap = job['_snap']
        _restore(self.root / 'recovery/c' / f"{job['fn']}.c", snap['c'])
        _restore(self.root / 'recovery/hints' / f"{job['fn']}.json", snap['hint'])
        self._record(job, 'fail', detail)
        fn = job['fn']
        if job['kind'] == 'fix':
            self.state['defects'][fn] = self.state['audits'].get(fn, {}).get('findings', [])
            self._event('open-defect', fn=fn, detail=detail[:300])
            return
        if job['kind'] == 'adopt':   # a coordinator hand fix: the accepted source stays
            self._event('request-failed', fn=fn, kind='adopt', detail=detail[:300])
            return
        model, rung = self._rung_model(job['rung'] + 1)
        if model is not None and not self.stopping():
            source = Path(job['source'])
            prior_source = source.read_bytes() if source.exists() else None
            nxt = self._new_job('translate', fn, model, rung, bytes=job['bytes'],
                                module=job.get('module'), hold=job.get('hold'),
                                prior={'model': job['model'], 'detail': detail})
            nxt['_seed'] = prior_source
            self.pending.append(nxt)
        else:
            self._park(fn, detail)

    def _park(self, fn, detail):
        self.state['parked'][fn] = {'detail': detail[:600],
                                    'models': [r['model'] for r in self.state['attempts'].get(fn, [])]}
        self.cmre.release(self.config.owner, [fn])

    def _limited(self, job, message, seconds):
        """Provider usage limit: pause the provider, requeue the job, spend no attempt."""
        _restore(self.root / 'recovery/c' / f"{job['fn']}.c", job['_snap']['c'])
        _restore(self.root / 'recovery/hints' / f"{job['fn']}.json", job['_snap']['hint'])
        provider = job['model'].split('/', 1)[0]
        until = time.time() + max(60, seconds)
        self.state['limited_providers'][provider] = {'until': until, 'message': message[:300]}
        self._event('model-limited', provider=provider, model=job['model'], until=until,
                    message=message[:300], fn=job['fn'], n=job['n'])
        if job['kind'] == 'audit':
            self.audit_queue.insert(0, job['fn'])
            return
        again = {k: v for k, v in job.items() if k not in ('_snap', '_seed_bytes')}
        again['_seed'] = job.get('_seed_bytes')
        self.pending.insert(0, again)

    def _infra(self, job, error):
        """Provider/auth failure: not the model's attempt. Retire the model, requeue the job."""
        _restore(self.root / 'recovery/c' / f"{job['fn']}.c", job['_snap']['c'])
        _restore(self.root / 'recovery/hints' / f"{job['fn']}.json", job['_snap']['hint'])
        self.state['unavailable_models'][job['model']] = error[:300]
        self._event('model-unavailable', model=job['model'], error=error[:300], fn=job['fn'], n=job['n'])
        if job['kind'] == 'audit':
            if self.config.audit_model == job['model']:
                self.config.audit_model = next((m for m in reversed(self.config.ladder)
                                                if m not in self.state['unavailable_models']), None)
            if self.config.audit_model:
                self.audit_queue.insert(0, job['fn'])
            return
        again = {k: v for k, v in job.items() if k not in ('_snap', '_seed_bytes')}
        again['_seed'] = job.get('_seed_bytes')
        self.pending.insert(0, again)

    def _verify(self, job, timed_out):
        self.verifying.append((job, self.verifier.submit(self._check, job, timed_out)))

    def _check(self, job, timed_out):
        """None when the candidate passed both gate runs, else the failure detail.

        Runs on a verifier thread: it reads the job and calls the gate; only the canonical copy
        into recovery/c/ is written here, and the main loop restores it on failure."""
        fn, source = job['fn'], Path(job['source'])
        if job.get('_stalled'):
            return f'stalled: no worker output for {self.config.stall_minutes} min'
        if timed_out:
            return f'timeout after {self.config.attempt_minutes} min'
        text_bytes = _read(source)
        if text_bytes is None:
            if job.get('_finish') == 'length':
                return 'worker hit the output cap before writing source'
            return 'worker wrote no source'
        if text_bytes == job.get('_seed_bytes'):
            return 'worker left the starting source unchanged'
        text = text_bytes.decode('utf-8', errors='replace')
        problems = mechanical_problems(fn, text)
        if problems:
            return 'mechanical: ' + '; '.join(problems)
        try:
            _validate_worker_hints(job['_snap']['hint'], _read(self.root / 'recovery/hints' / f'{fn}.json'))
        except (ValueError, TypeError, UnicodeError) as exc:
            # _collect routes this to _fail, which restores the original bytes
            # (or absence). Do not replace the dispatch snapshot with the edits.
            return f'worker hints rejected: {exc}'
        report = self.cmre.test(fn, str(source))
        if report.get('status') != 'pass':
            return report_detail(report)
        target = self.root / 'recovery/c' / f'{fn}.c'
        target.write_bytes(text_bytes)
        report = self.cmre.test(fn, None)
        if report.get('status') != 'pass':
            return 'canonical: ' + report_detail(report)
        return None

    def _collect(self):
        waiting = []
        for job, future in self.verifying:
            if not future.done():
                waiting.append((job, future))
                continue
            try:
                detail = future.result()
            except Exception as exc:
                detail = f'verification crashed: {exc}'
            if detail is None:
                self.accept_queue.append(job)
            else:
                self._fail(job, detail)
        self.verifying = waiting

    def _audit_result(self, job, timed_out):
        fn = job['fn']
        try:
            data = json.loads(Path(job['audit_path']).read_text(encoding='utf-8'))
            verdict = data.get('verdict')
            findings = data.get('findings') or []
        except (OSError, ValueError):
            verdict, findings = 'error', ['timeout' if timed_out else 'no audit file']
        if verdict not in ('ok', 'defect'):
            verdict = 'error'
        audit = self.state['audits'].setdefault(fn, {'rounds': 0})
        if job.get('confirm'):
            return self._confirm_result(job, audit, verdict, findings)
        audit.update(verdict=verdict, findings=findings, model=job['model'])
        audit['rounds'] += 1
        self._record(job, verdict, json.dumps(findings)[:600])
        if verdict == 'error':
            if audit['rounds'] < MAX_AUDIT_ROUNDS and not self.stopping():
                self.audit_queue.append(fn)
        elif verdict == 'ok':
            self.state['defects'].pop(fn, None)
        elif self.config.confirm_model:
            audit.update(verdict='unconfirmed', confirm_rounds=0)
            if not self.stopping():
                self.audit_queue.append(fn)
        else:
            self._defect(fn, findings)

    def _confirm_result(self, job, audit, verdict, findings):
        fn = job['fn']
        audit['confirm_rounds'] = audit.get('confirm_rounds', 0) + 1
        self._record(job, f'confirm-{verdict}', json.dumps(findings)[:600])
        if verdict == 'error':   # stays unconfirmed; retried once
            if audit['confirm_rounds'] < MAX_AUDIT_ROUNDS and not self.stopping():
                self.audit_queue.append(fn)
        elif verdict == 'ok':
            audit.update(verdict='ok', disputed=audit.get('findings', []), disputed_by=job['model'], findings=[])
            self.state['defects'].pop(fn, None)
        else:
            audit.update(verdict='defect', findings=findings, confirmed_by=job['model'])
            self._defect(fn, findings)

    def _defect(self, fn, findings):
        """A defect that stands: one fix attempt with the strongest rung, then an open defect."""
        self.state['defects'][fn] = findings
        rounds = self.state['fix_rounds'].get(fn, 0)
        if rounds < MAX_FIX_ROUNDS and not self.stopping():
            self.state['fix_rounds'][fn] = rounds + 1
            fix = self._new_job('fix', fn, self.config.ladder[-1], len(self.config.ladder) - 1,
                                bytes=self.state['accepted'][fn]['bytes'], findings=findings)
            fix['_seed'] = _read(self.root / 'recovery/c' / f'{fn}.c')
            self.pending.insert(0, fix)
        else:
            self._event('open-defect', fn=fn, findings=len(findings))

    def _reap(self):
        still = []
        for job, handle, started in self.running:
            code = handle.poll()
            now = self.clock()
            timed_out = code is None and now - started > self.config.attempt_minutes * 60
            activity = getattr(handle, 'last_activity', lambda: None)()
            stalled = (code is None and not timed_out and activity is not None
                       and now - max(activity, started) > self.config.stall_minutes * 60)
            if code is None and not timed_out and not stalled:
                still.append((job, handle, started))
                continue
            if timed_out or stalled:
                handle.kill()
            limit = getattr(handle, 'limit_error', lambda: None)()
            if limit:
                self._limited(job, *limit)
                continue
            infra = getattr(handle, 'infra_error', lambda: None)()
            if infra:
                self._infra(job, infra)
                continue
            if stalled:
                job['_stalled'] = True
            job['_finish'] = getattr(handle, 'finish_reason', lambda: None)()
            if job['kind'] == 'audit':
                self._audit_result(job, timed_out)
            else:
                self._verify(job, timed_out)
        self.running = still

    def _accept(self):
        waiting = []
        for job in self.accept_queue:
            fn = job['fn']
            hold = self.cmre.hold(fn)
            for _ in range(5):
                if not hold or hold.get('state') not in HARD_HOLDS:
                    break
                self.cmre.retry(fn, f"farm {self.config.owner}: gate pass under current harness; "
                                    f"async audit owns the held concern ({hold.get('reason', '')[:120]})")
                hold = self.cmre.hold(fn)
            code, output = self.cmre.accept(fn)
            if code == 2:
                waiting.append(job)   # another writer holds the lease; try next tick
                continue
            if code != 0:
                self._fail(job, f'accept refused: {output[-600:]}')
                continue
            self._record(job, 'pass', 'accepted')
            was_held = bool(job.get('hold')) or bool(self.state['accepted'].get(fn, {}).get('was_held'))
            self.state['accepted'][fn] = {'bytes': job['bytes'], 'model': job['model'], 'kind': job['kind'],
                                          'at': time.time(), 'was_held': was_held, 'hold': job.get('hold')}
            self._event('accepted', fn=fn, bytes=job['bytes'], model=job['model'], kind=job['kind'])
            if job['kind'] == 'adopt':   # a coordinator hand fix: audit it afresh
                self.state['defects'].pop(fn, None)
                self.state['audits'][fn] = {'rounds': 0, 'adopted': job.get('note', '')}
            rounds = self.state['audits'].get(fn, {}).get('rounds', 0)
            if self.config.audit_workers and rounds < MAX_AUDIT_ROUNDS:
                self.audit_queue.append(fn)
        self.accept_queue = waiting

    def _requests(self):
        """Apply coordinator requests (`adopt`, `dismiss`) written into RUN/requests/."""
        for path in sorted((self.run_dir / 'requests').glob('*.json')):
            try:
                request = json.loads(path.read_text(encoding='utf-8'))
            except (OSError, ValueError):
                continue   # still being written; next tick
            path.unlink(missing_ok=True)
            fn = request.get('fn', '')
            if request.get('kind') == 'dismiss':
                audit = self.state['audits'].setdefault(fn, {'rounds': 0})
                audit.update(verdict='ok', dismissed=request.get('reason', ''),
                             dismissed_findings=audit.get('findings', []), findings=[])
                self.state['defects'].pop(fn, None)
                self._event('dismissed', fn=fn, reason=request.get('reason', '')[:300])
            elif request.get('kind') == 'adopt':
                job = self._new_job('adopt', fn, 'hand', bytes=self.state['accepted'].get(fn, {}).get('bytes', 0),
                                    note=request.get('note', ''))
                job['source'] = request['source']
                job['_snap'] = {'c': _read(self.root / 'recovery/c' / f'{fn}.c'),
                                'hint': _read(self.root / 'recovery/hints' / f'{fn}.json')}
                self._event('adopt', fn=fn, source=request['source'])
                self._verify(job, False)

    def tick(self):
        self._reap()
        self._requests()
        self._collect()
        self._accept()
        self._dispatch()
        self.save()

    def run(self, poll=10.0):
        self._event('farm-start', pid=os.getpid(), workers=self.config.workers,
                    audit_workers=self.config.audit_workers, ladder=self.config.ladder)
        try:
            while True:
                self.tick()
                supply_done = self.frontier == [] or self.config.workers == 0
                if self.finished() or (not self.running and not self.pending and not self.accept_queue
                                       and not self.verifying and not self.audit_queue and supply_done):
                    break
                time.sleep(poll)
        finally:
            self.cmre.release(self.config.owner, None)
            self.save()
            self._event('farm-exit', status=status_line(self.summary()))


def running_from_events(path):
    """Dispatches with no terminal event yet: fn:kind:model."""
    live = {}
    try:
        lines = Path(path).read_text(encoding='utf-8').splitlines()
    except OSError:
        return []
    for line in lines:
        event = json.loads(line)
        if event['event'] == 'dispatch':
            live[event['n']] = f"{event['fn']}:{event['kind']}:{event['model']}"
        elif event['event'] in ('result', 'model-unavailable') and 'n' in event:
            live.pop(event['n'], None)
    return list(live.values())


def _read_events(path, offset):
    """Complete JSON lines appended after `offset`: (events, new offset)."""
    try:
        with Path(path).open('rb') as handle:
            handle.seek(offset)
            data = handle.read()
    except OSError:
        return [], offset
    end = data.rfind(b'\n') + 1
    found = []
    for line in data[:end].splitlines():
        try:
            found.append(json.loads(line))
        except ValueError:
            continue
    return found, offset + end


def wait_for(run_dir, minutes, poll=15, clock=time.monotonic, sleep=time.sleep):
    """Block until a notable farm event, a dead farm, or the timeout: (reason, events).

    The OpenCode coordinator runs this as a background shell command and is resumed when
    it returns, so it spends no tokens while the farm works."""
    run_dir = Path(run_dir)
    events_path, state_path = run_dir / 'events.jsonl', run_dir / 'state.json'
    offset = events_path.stat().st_size if events_path.exists() else 0
    deadline = clock() + minutes * 60
    while True:
        found, offset = _read_events(events_path, offset)
        notable = [e for e in found if e.get('event') in NOTABLE_EVENTS]
        if notable:
            return 'event', notable
        try:
            stale = time.time() - state_path.stat().st_mtime > STALE_SECONDS
        except OSError:
            stale = True
        if stale:
            return 'farm not running', []
        if clock() >= deadline:
            return 'timeout', []
        sleep(poll)


def _agent_file(description, mode, model, rules, body, steps=None):
    lines = ['---', f'description: {description}', f'mode: {mode}', f'model: {model}']
    if steps:
        lines.append(f'steps: {steps}')
    lines.append('permissions:')
    lines += [f'  - {{ action: "{a}", resource: "{r}", effect: {e} }}' for a, r, e in rules]
    return '\n'.join(lines + ['---', body.strip(), ''])


def coordinator_agent():
    rules = [('*', '*', 'ask'), ('read', '*', 'allow'), ('grep', '*', 'allow'), ('glob', '*', 'allow'),
             ('todowrite', '*', 'allow'), ('question', '*', 'allow'), ('skill', '*', 'allow'),
             ('edit', 'docs/NEXT-SESSION-PROMPT.md', 'allow')]
    shell = [f'{PY} scripts/cmre_farm.py *', f'{PY} scripts/cmre_session.py *', f'{PY} -m cmre *',
             'rg *', 'git status*', 'git diff*', 'git log*']
    rules += [('shell', command, 'allow') for command in shell]
    rules += [('subagent', 'cm-defect', 'allow')]
    body = """
You coordinate CM0102 decompilation from OpenCode. Read AGENTS.md, then follow
docs/agent/FARM.md section "OpenCode coordinator" exactly. The Python farm does all
per-function dispatch, testing and acceptance; you start it, wait on it, act on its
events, and send each open defect to one cm-defect subagent.

Your information comes from the output of `cmre_farm.py` status, wait and defects,
and from subagent result lines. Those outputs are complete: act on them directly and
keep each event to one short line, so your context stays small for a 10-hour session.
"""
    return _agent_file('CM0102 coordinator: runs the recovery farm and its defect lane (docs/agent/FARM.md)',
                       'primary', COORDINATOR_MODEL, rules, body)


def defect_agent():
    rules = [('*', '*', 'deny'), ('read', '*', 'allow'), ('grep', '*', 'allow'), ('glob', '*', 'allow'),
             ('edit', 'CM0102/Analysis/cmre/farm/*/hand/*.c', 'allow')]
    shell = [f'{PY} scripts/cmre_farm.py test *']
    shell += [f'{PY} -m cmre {command} *' for command in WORKER_COMMANDS if command not in ('test', 'explain')]
    shell += list(READ_ONLY_SHELL)
    rules += [('shell', command, 'allow') for command in shell]
    body = """
You resolve one open CM0102 audit defect. The coordinator gives you RUN (a farm run
directory) and fn_X. Follow docs/agent/FARM.md section "Defect subagent" exactly and
finish with its one result line.
"""
    return _agent_file('CM0102 defect fixer: checks one open audit defect against the listing, then fixes or dismisses it',
                       'subagent', DEFAULT_CONFIRM_MODEL, rules, body, steps=40)


def install_agents(root=ROOT):
    """Write the OpenCode coordinator and defect agents into the local .opencode/agents/."""
    directory = Path(root) / '.opencode/agents'
    directory.mkdir(parents=True, exist_ok=True)
    paths = []
    for name, text in (('cm-coordinator.md', coordinator_agent()), ('cm-defect.md', defect_agent())):
        path = directory / name
        path.write_text(text, encoding='utf-8')
        paths.append(path)
    return paths


def defect_lines(state):
    """One line per open defect: fn, bytes, verdict source, first finding, last attempt."""
    lines = []
    for fn in sorted(state.get('defects', {})):
        audit = state.get('audits', {}).get(fn, {})
        findings = state['defects'][fn] or audit.get('findings') or []
        first = findings[0] if findings else {}
        detail = first.get('detail', str(first)) if isinstance(first, dict) else str(first)
        last = (state.get('attempts', {}).get(fn) or [{}])[-1]
        bytes_ = state.get('accepted', {}).get(fn, {}).get('bytes', 0)
        lines.append(f"{fn} {bytes_}B | {len(findings)} finding(s), confirmed by "
                     f"{audit.get('confirmed_by') or audit.get('model') or '?'}: {detail[:160]} | last "
                     f"{last.get('kind', '-')} {last.get('outcome', '-')}: {str(last.get('detail', ''))[:120]}")
    return lines


def farm_live(run_dir):
    """A farm process is working this run: state saved recently and no exit after its start."""
    run_dir = Path(run_dir)
    try:
        if time.time() - (run_dir / 'state.json').stat().st_mtime > STALE_SECONDS:
            return False
    except OSError:
        return False
    found, _ = _read_events(run_dir / 'events.jsonl', 0)
    lifecycle = [e['event'] for e in found if e.get('event') in ('farm-start', 'farm-exit')]
    return not lifecycle or lifecycle[-1] == 'farm-start'


def apply_requests_now(run_dir):
    """Apply queued adopt/dismiss requests in this process when no farm runs the directory."""
    run_dir = Path(run_dir)
    state = json.loads((run_dir / 'state.json').read_text(encoding='utf-8'))
    config = FarmConfig(workers=0, ladder=['-'], audit_workers=0, audit_model=None, attempt_minutes=1,
                        hours=None, min_bytes=0, max_bytes=None, include_held=False, owner=state.get('owner', ''))
    farm = Farm(run_dir, ROOT, CmreAdapter(), None, config)
    farm.tick()
    while farm.accept_queue:   # another writer holds the accept lease
        time.sleep(5)
        farm.tick()
    return farm


def _request(run_dir, fn, kind, **data):
    directory = Path(run_dir) / 'requests'
    directory.mkdir(parents=True, exist_ok=True)
    path = directory / f'{time.time_ns()}-{fn}-{kind}.json'
    tmp = path.with_suffix('.tmp')
    tmp.write_text(json.dumps({'fn': fn, 'kind': kind, **data}), encoding='utf-8')
    tmp.replace(path)
    return path


def status_line(s):
    models = ', '.join(f"{m} {v['passes']}/{v['attempts']}" for m, v in s['by_model'].items())
    return (f"accepted {s['accepted_functions']} fn / {s['accepted_bytes']} B in {s['hours']} h "
            f"({s['bytes_per_hour']} B/h) | running {len(s['running'])} | pending {s['pending']} | "
            f"parked {s['parked']} skipped {s['skipped']} | audits ok {s['audits_ok']} defect {s['audits_defect']} "
            f"error {s.get('audit_errors', 0)} unconfirmed {s.get('audits_unconfirmed', 0)} "
            f"disputed {s.get('audits_disputed', 0)} "
            f"open {len(s['open_defects'])} | models {models}"
            + ''.join(f' | UNAVAILABLE {m}: {e[:80]}' for m, e in s.get('unavailable_models', {}).items())
            + ''.join(f" | LIMITED {p} until {time.strftime('%H:%M', time.localtime(until))}"
                      for p, until in s.get('limited', {}).items())
            + (' | STOPPING' if s['stopping'] else ''))


# --- real adapters ---------------------------------------------------------------

class CmreAdapter:
    """Selection and claims through the Python API; gate and acceptance through the CLI."""

    def __init__(self, root=ROOT):
        self.root = Path(root)

    def _cli(self, *args, timeout=1800):
        result = subprocess.run(['py', '-3.13', '-m', 'cmre', *args], cwd=self.root, capture_output=True,
                                text=True, encoding='utf-8', errors='replace', timeout=timeout)
        return result.returncode, result.stdout, result.stderr

    def frontier(self, limit, min_bytes, max_bytes, include_held, owner):
        from cmre import queue
        items = queue.c_frontier(limit, max_bytes=max_bytes, min_bytes=min_bytes,
                                 include_held=include_held, owner=owner)
        return [Item(f'fn_{i.va:08x}', i.body_bytes, i.ready, i.pending_callees, i.module) for i in items]

    def claim(self, fn, owner, hours):
        from cmre import coordination, work
        try:
            work.claim([int(fn[3:], 16)], owner, hours)
        except coordination.BusyError as exc:   # another farm is claiming or testing it right now
            raise ValueError(str(exc)) from exc

    def release(self, owner, fns):
        from cmre import coordination, work
        for _ in range(3):
            try:
                work.release(owner, None if fns is None else [int(f[3:], 16) for f in fns])
                return
            except coordination.BusyError:   # queue lock busy past its 5 s wait
                time.sleep(2)
        # Give up: the claim expires on its own.

    def held(self, fns):
        from cmre import work
        records = work.read()['functions']
        return {fn for fn in fns if (records.get(fn) or {}).get('state', 'ready') != 'ready'}

    def hold(self, fn):
        from cmre import work
        record = work.read()['functions'].get(fn)
        if record and record.get('state') != 'ready':
            return {k: record.get(k) for k in ('state', 'blocker', 'reason', 'retry_when') if record.get(k)}
        return None

    def retry(self, fn, reason):
        self._cli('work', 'retry', fn, '--reason', reason)

    def brief(self, fn):
        code, out, err = self._cli('brief', fn, timeout=300)
        if code != 0:
            raise RuntimeError((err or out)[-300:])
        return out

    def test(self, fn, source=None):
        args = ['test', fn, '--c', '--json'] + (['--source', source] if source else [])
        code, out, err = self._cli(*args)
        try:
            return json.loads(out)
        except ValueError:
            return {'status': 'error', 'problems': [(err or out)[-800:]]}

    def accept(self, fn):
        code, out, err = self._cli('accept', fn, '--c')
        if code != 0 and 'Writer active' in (err + out):
            code = 2
        return code, (out + err)[-2000:]


INFRA_ERROR = re.compile(r'API key|statusCode"?\s*:\s*(?:401|402|403)|quota|insufficient balance|'
                         r'model not found|ProviderModelNotFound', re.I)


LIMIT_ERROR = re.compile(r'usage limit|rate limit|too many requests|go-limit', re.I)


class OpencodeHandle:
    def __init__(self, process, log):
        self.process, self.log = process, Path(log)

    def finish_reason(self):
        """Reason of the worker's last step ('length' = output cap), or None."""
        try:
            text = self.log.read_text(encoding='utf-8', errors='replace')
        except OSError:
            return None
        reason = None
        for line in text.splitlines():
            if '"step_finish"' not in line:
                continue
            try:
                reason = (json.loads(line).get('part') or {}).get('reason') or reason
            except ValueError:
                continue
        return reason

    def last_activity(self):
        """Log mtime expressed on the farm's monotonic clock."""
        try:
            return time.monotonic() - (time.time() - self.log.stat().st_mtime)
        except OSError:
            return None

    def limit_error(self):
        """(message, seconds until reset) for a provider usage/rate limit in the stream, or None."""
        try:
            text = self.log.read_text(encoding='utf-8', errors='replace')
        except OSError:
            return None
        for line in text.splitlines():
            if '"type":"error"' not in line.replace(' ', ''):
                continue
            try:
                data = json.loads(line).get('error') or {}
            except (ValueError, AttributeError):
                continue
            if not isinstance(data, dict):
                continue
            message = str(data.get('message') or (data.get('data') or {}).get('message') or '')
            body = str((data.get('response') or {}).get('body') or '')
            if data.get('status') == 429 or LIMIT_ERROR.search(f"{data.get('type', '')} {message}"):
                if data.get('retryAfterMs'):
                    return message[:300], int(data['retryAfterMs']) // 1000
                reset = re.search(r'resets in (?:(\d+)h)?\s*(?:(\d+)m)?', f'{message} {body}')
                seconds = (int(reset.group(1) or 0) * 3600 + int(reset.group(2) or 0) * 60) if reset else 0
                return message[:300], seconds or DEFAULT_LIMIT_SECONDS
        return None

    def infra_error(self):
        """A provider/auth error event in the worker's JSON stream, if any."""
        try:
            text = self.log.read_text(encoding='utf-8', errors='replace')
        except OSError:
            return None
        for line in text.splitlines():
            if '"type":"error"' in line.replace(' ', '') and INFRA_ERROR.search(line):
                try:
                    data = json.loads(line)['error']
                    return str((data.get('data') or {}).get('message') or data.get('name'))
                except (ValueError, KeyError, TypeError, AttributeError):
                    return line[:300]
        return None

    def poll(self):
        return self.process.poll()

    def kill(self):
        if os.name == 'nt':
            subprocess.run(['taskkill', '/PID', str(self.process.pid), '/T', '/F'], capture_output=True)
        else:
            self.process.kill()


# opencode 1.x caps a step's output at 32k tokens unless this is set; hidden MiniMax thinking hit
# that cap on 5 of 8 parked production jobs (2026-11-13). 2.x has a fixed 256k ceiling.
V1_OUTPUT_TOKEN_MAX = '131072'
V2_EXECUTABLE = Path(os.environ.get('APPDATA', '')) / 'npm/node_modules/@opencode/cli/bin/opencode.exe'


class OpencodeV2Handle(OpencodeHandle):
    """Removes the job's temporary agent file once the worker has exited or been killed."""

    def __init__(self, process, log, agent, executable=V2_EXECUTABLE):
        super().__init__(process, log)
        self.agent, self.executable = Path(agent), str(executable)

    def poll(self):
        code = self.process.poll()
        if code is not None:
            self.agent.unlink(missing_ok=True)
        return code

    def session_id(self):
        try:
            match = re.search(r'"sessionID":"(ses_[A-Za-z0-9]+)"', self.log.read_text(encoding='utf-8', errors='replace'))
        except OSError:
            return None
        return match.group(1) if match else None

    def kill(self):
        # The session runs in the shared service; killing the client alone leaves it working.
        sid = self.session_id()
        if sid:
            try:
                subprocess.run([self.executable, 'api', 'session.interrupt', '--param', f'sessionID={sid}'],
                               capture_output=True, timeout=30)
            except (OSError, subprocess.TimeoutExpired):
                pass
        super().kill()
        self.agent.unlink(missing_ok=True)


class OpencodeV2Launcher:
    """opencode 2.x: one temporary agent file per job carries its permissions."""

    def __init__(self, root, run_dir, executable=V2_EXECUTABLE):
        self.root, self.run_dir, self.executable = Path(root), Path(run_dir), str(executable)

    def start(self, job, prompt, config):
        logs = self.run_dir / 'logs'
        logs.mkdir(parents=True, exist_ok=True)
        stem = f"{job['fn']}-{job['kind']}-{job['n']}"
        (logs / f'{stem}.prompt.txt').write_text(prompt, encoding='utf-8')
        name = re.sub(r'[^A-Za-z0-9-]', '-', f"farm-{self.run_dir.name}-{stem}")
        agent = self.root / '.opencode/agents' / f'{name}.md'
        agent.parent.mkdir(parents=True, exist_ok=True)
        agent.write_text(v2_agent(self.root, job), encoding='utf-8')
        self._wait_for_agent(name)
        # Shared background service, not --standalone: concurrent private servers hung (2026-11-13).
        command = [self.executable, 'run', '--agent', name, '-m', job['model'],
                   '--format', 'json', '--title', f"farm {job['kind']} {job['fn']}", prompt]
        flags = (subprocess.CREATE_NO_WINDOW | subprocess.CREATE_NEW_PROCESS_GROUP) if os.name == 'nt' else 0
        out = (logs / f'{stem}.jsonl').open('wb')
        process = subprocess.Popen(command, cwd=self.root, env=dict(os.environ), stdout=out,
                                   stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL, creationflags=flags)
        out.close()
        return OpencodeV2Handle(process, logs / f'{stem}.jsonl', agent, self.executable)

    def _wait_for_agent(self, name, attempts=30):
        """The shared service picks up a new agent file after a short delay; launching
        before then fails with 'Agent not found' (2026-11-13)."""
        for _ in range(attempts):
            try:
                done = subprocess.run([self.executable, 'api', 'agent.get', '--param', f'agentID={name}',
                                       '--param', f'location[directory]={self.root}'],
                                      capture_output=True, timeout=30)
                if done.returncode == 0:
                    return True
            except (OSError, subprocess.TimeoutExpired):
                pass
            time.sleep(1)
        return False


class OpencodeLauncher:
    def __init__(self, root, run_dir, executable='opencode'):
        self.root, self.run_dir, self.executable = Path(root), Path(run_dir), executable

    def start(self, job, prompt, config):
        logs = self.run_dir / 'logs'
        logs.mkdir(parents=True, exist_ok=True)
        stem = f"{job['fn']}-{job['kind']}-{job['n']}"
        (logs / f'{stem}.prompt.txt').write_text(prompt, encoding='utf-8')
        env = dict(os.environ, OPENCODE_CONFIG_CONTENT=json.dumps(config),
                   OPENCODE_EXPERIMENTAL_OUTPUT_TOKEN_MAX=V1_OUTPUT_TOKEN_MAX)
        command = [self.executable, '--pure', 'run', '--dir', str(self.root), '--model', job['model'],
                   '--format', 'json', '--title', f"farm {job['kind']} {job['fn']}", prompt]
        flags = (subprocess.CREATE_NO_WINDOW | subprocess.CREATE_NEW_PROCESS_GROUP) if os.name == 'nt' else 0
        out = (logs / f'{stem}.jsonl').open('wb')
        process = subprocess.Popen(command, cwd=self.root, env=env, stdout=out, stderr=subprocess.STDOUT,
                                   stdin=subprocess.DEVNULL, creationflags=flags)
        out.close()
        return OpencodeHandle(process, logs / f'{stem}.jsonl')


def make_launcher(build, root, run_dir):
    """opencode 2.x (npm, 256k output, current credentials) or 1.x (Chocolatey `opencode` on PATH)."""
    return OpencodeV2Launcher(root, run_dir) if build == 'v2' else OpencodeLauncher(root, run_dir)


def probe_command(build, model):
    prompt = 'Reply with exactly: OK'
    if build == 'v2':
        return [str(V2_EXECUTABLE), 'run', '-m', model, '--format', 'json', prompt]
    return ['opencode', '--pure', 'run', '--model', model, '--format', 'json', prompt]


def probe(models, build='v2'):
    """One trivial prompt per model: {model: None if it answers, else the error text}."""
    results = {}
    for model in models:
        try:
            done = subprocess.run(probe_command(build, model), cwd=ROOT, capture_output=True, text=True,
                                  encoding='utf-8', errors='replace', timeout=180)
            text = done.stdout + done.stderr
        except (OSError, subprocess.TimeoutExpired) as exc:
            results[model] = str(exc)[:200]
            continue
        error = None
        for line in text.splitlines():
            try:
                event = json.loads(line)
            except ValueError:
                continue
            if event.get('type') == 'error':
                data = event.get('error') or {}
                error = str((data.get('data') or {}).get('message') or data.get('name') or 'error')
            elif event.get('type') == 'text' and 'OK' in str((event.get('part') or {}).get('text', '')):
                error = None
                break
        else:
            error = error or 'no answer'
        results[model] = error
    return results


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    sub = p.add_subparsers(dest='action', required=True)
    r = sub.add_parser('run', help='dispatch, gate and accept until the deadline or STOP')
    r.add_argument('run_dir')
    r.add_argument('--workers', type=int, default=12)
    r.add_argument('--ladder', default=','.join(DEFAULT_LADDER), help='comma-separated provider/model, cheapest first')
    r.add_argument('--audit-workers', type=int, default=4)
    r.add_argument('--audit-model', default=DEFAULT_AUDIT_MODEL)
    r.add_argument('--attempt-minutes', type=float, default=30)
    r.add_argument('--stall-minutes', type=float, default=DEFAULT_STALL_MINUTES, help='kill a worker with no output for this long')
    r.add_argument('--hours', type=float, default=10)
    r.add_argument('--min-bytes', type=int, default=0)
    r.add_argument('--max-bytes', type=int)
    r.add_argument('--include-held', action='store_true', help='also take held functions; prompts carry the hold')
    r.add_argument('--owner')
    r.add_argument('--poll', type=float, default=10)
    r.add_argument('--opencode', choices=('v2', 'v1'), default='v2',
                   help='v2: npm opencode 2.x (default); v1: Chocolatey opencode 1.x on PATH')
    r.add_argument('--confirm-model', default=DEFAULT_CONFIRM_MODEL,
                   help="second auditor that must confirm a defect before a fix; '' disables")
    r.add_argument('--skip-ranges', default=','.join(f'0x{lo:08x}-0x{hi:08x}' for lo, hi in DEFAULT_SKIP_RANGES),
                   help="VA ranges never dispatched (default: x87 runtime helpers); '' disables")
    r.add_argument('--verify-threads', type=int, default=8, help='parallel gate re-tests')
    st = sub.add_parser('start', help='run the farm detached (hidden console, logs in RUN); args as for run')
    st.add_argument('run_dir')
    st.add_argument('run_args', nargs=argparse.REMAINDER)
    w = sub.add_parser('wait', help='block until a notable event, a dead farm, or --minutes')
    w.add_argument('run_dir')
    w.add_argument('--minutes', type=float, default=60)
    ad = sub.add_parser('adopt', help='ask the running farm to gate and accept a hand-fixed source')
    ad.add_argument('run_dir')
    ad.add_argument('fn')
    ad.add_argument('--source', required=True)
    ad.add_argument('--note', default='')
    di = sub.add_parser('dismiss', help='close an audit defect as a false positive, with the reason')
    di.add_argument('run_dir')
    di.add_argument('fn')
    di.add_argument('--reason', required=True)
    te = sub.add_parser('test', help="cmre test --source (or explain with --case) as the run's claim owner")
    te.add_argument('run_dir')
    te.add_argument('fn')
    te.add_argument('source')
    te.add_argument('--case', type=int)
    sub.add_parser('install-agents', help='write the OpenCode coordinator and defect agents into .opencode/agents/')
    pr = sub.add_parser('probe', help='check every ladder/audit model answers before a run')
    pr.add_argument('--ladder', default=','.join(DEFAULT_LADDER + (DEFAULT_AUDIT_MODEL, DEFAULT_CONFIRM_MODEL)))
    pr.add_argument('--opencode', choices=('v2', 'v1'), default='v2')
    for name in ('status', 'stop', 'defects'):
        s = sub.add_parser(name)
        s.add_argument('run_dir')
        if name == 'status':
            s.add_argument('--json', action='store_true')
    a = p.parse_args(argv)
    if a.action == 'probe':
        models = list(dict.fromkeys(m.strip() for m in a.ladder.split(',') if m.strip()))
        results = probe(models, a.opencode)
        for model, error in results.items():
            print(f'{model}: OK' if error is None else f'{model}: UNAVAILABLE {error[:160]}')
        return 0 if all(e is None for e in results.values()) else 1
    if a.action == 'install-agents':
        for path in install_agents(ROOT):
            print(path)
        return 0
    run_dir = Path(a.run_dir)
    if a.action == 'start':
        return start(run_dir, a.run_args)
    if a.action == 'defects':
        for line in defect_lines(json.loads((run_dir / 'state.json').read_text(encoding='utf-8'))):
            print(line)
        return 0
    if a.action == 'wait':
        reason, found = wait_for(run_dir, a.minutes)
        print(f'wait: {reason}')
        for event in found:
            print(json.dumps(event))
        if (run_dir / 'state.json').exists():
            main(['status', str(run_dir)])
        return 0
    if a.action == 'adopt':
        source = Path(a.source).resolve()
        if not source.is_file():
            print(f'no such source: {source}', file=sys.stderr)
            return 2
        print(_request(run_dir, a.fn, 'adopt', source=str(source), note=a.note))
    if a.action == 'dismiss':
        print(_request(run_dir, a.fn, 'dismiss', reason=a.reason))
    if a.action in ('adopt', 'dismiss'):
        if farm_live(run_dir):
            print('queued: the running farm applies it on its next tick')
            return 0
        applied = apply_requests_now(run_dir)
        failed = [e for e in _read_events(run_dir / 'events.jsonl', 0)[0][-5:]
                  if e.get('event') == 'request-failed' and e.get('fn') == a.fn]
        print(f"applied now (no farm running): {'FAILED ' + failed[-1]['detail'] if failed else 'ok'}; "
              f"open defects {sorted(applied.state['defects'])}")
        return 1 if failed else 0
    if a.action == 'test':
        owner = json.loads((run_dir / 'state.json').read_text(encoding='utf-8')).get('owner', '')
        args = (['explain', a.fn, '--c', '--source', a.source, '--case', str(a.case)] if a.case is not None
                else ['test', a.fn, '--c', '--source', a.source])
        return subprocess.call(['py', '-3.13', '-m', 'cmre', *args], cwd=ROOT,
                               env=dict(os.environ, CMRE_CLAIM_OWNER=owner))
    if a.action == 'stop':
        run_dir.mkdir(parents=True, exist_ok=True)
        (run_dir / 'STOP').write_text('stop requested\n', encoding='utf-8')
        print('stop requested; running workers finish, nothing new is dispatched')
        return 0
    if a.action == 'status':
        state = json.loads((run_dir / 'state.json').read_text(encoding='utf-8'))
        config = FarmConfig(0, ['-'], 0, None, 1, None, 0, None, False, state.get('owner', ''))
        f = Farm(run_dir, ROOT, None, None, config)
        s = f.summary()
        s['running'] = running_from_events(run_dir / 'events.jsonl')
        print(json.dumps(s, indent=1) if a.json else status_line(s))
        return 0
    config = FarmConfig(workers=a.workers, ladder=[m.strip() for m in a.ladder.split(',') if m.strip()],
                        audit_workers=a.audit_workers, audit_model=a.audit_model,
                        attempt_minutes=a.attempt_minutes, hours=a.hours, min_bytes=a.min_bytes,
                        max_bytes=a.max_bytes, include_held=a.include_held,
                        owner=a.owner or f'farm-{run_dir.name}', stall_minutes=a.stall_minutes,
                        confirm_model=a.confirm_model or None, skip_ranges=tuple(parse_ranges(a.skip_ranges)))
    (run_dir / 'STOP').unlink(missing_ok=True)
    from concurrent.futures import ThreadPoolExecutor
    with ThreadPoolExecutor(max_workers=max(1, a.verify_threads)) as verifier:
        f = Farm(run_dir, ROOT, CmreAdapter(), make_launcher(a.opencode, ROOT, run_dir), config, verifier=verifier)
        f.run(poll=a.poll)
    print(status_line(f.summary()))
    return 0


def start(run_dir, run_args):
    """Launch `run` detached from the caller's shell, with a hidden console that its
    `py -m cmre` children share (a fully detached farm opens a console window per child)."""
    run_dir = Path(run_dir)
    run_dir.mkdir(parents=True, exist_ok=True)
    command = [sys.executable, str(Path(__file__).resolve()), 'run', str(run_dir), *run_args]
    flags = 0
    if os.name == 'nt':
        flags = subprocess.CREATE_NO_WINDOW | subprocess.CREATE_NEW_PROCESS_GROUP
    with (run_dir / 'farm.out').open('ab') as out, (run_dir / 'farm.err').open('ab') as err:
        try:   # leave the caller's job object so closing that shell or session cannot kill the farm
            process = subprocess.Popen(command, cwd=ROOT, stdin=subprocess.DEVNULL, stdout=out, stderr=err,
                                       creationflags=flags | getattr(subprocess, 'CREATE_BREAKAWAY_FROM_JOB', 0),
                                       close_fds=True)
        except OSError:   # the job forbids breakaway
            process = subprocess.Popen(command, cwd=ROOT, stdin=subprocess.DEVNULL, stdout=out, stderr=err,
                                       creationflags=flags, close_fds=True)
    (run_dir / 'farm.pid').write_text(f'{process.pid}\n', encoding='utf-8')
    print(f'farm started: pid {process.pid}, logs {run_dir}/farm.out and farm.err')
    return 0


if __name__ == '__main__':
    sys.exit(main())
