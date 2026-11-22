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
from contextlib import ExitStack
from dataclasses import dataclass, field
import hashlib
import json
import os
from pathlib import Path
from typing import Protocol
import re
import subprocess
import sys
import time
from uuid import uuid4

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))

from cmre import acceptance_request, operand_evidence
from cmre.acceptance_request import validate_worker_hints as _validate_worker_hints
from cmre.operand_evidence import bridge_call_sites, bridge_mutant_sources, bridge_mutant_outcome

# 2026-11-13 head-to-head, 300-1500 B, opencode 2.x: MiniMax M3.1 Flash 6/11 passes (free plan),
# MiniMax M3 3/13 (dropped), DeepSeek v4.1 Flash #low 4/12 then default effort 2/4; DeepSeek
# solved functions MiniMax failed. All arms ~1 KB/h per worker, bounded by reasoning tokens.
# DeepSeek runs use v4.1 Flash (Adam's choice); fix jobs use the top rung.
# Adam added Terra as an ordinary farm lane on 2026-11-15 after provider quotas ran low.
TERRA_MODEL = 'openai/gpt-6.1-terra'
COMMANDCODE_FLASH_MODEL = 'commandcode/deepseek/deepseek-v4.1-flash'
HY4_FLASH_MODEL = 'opencode-go/hy4-flash-free'
# Same capability/effort tiers as Go, with independent provider availability. Not Flash Fast.
DEFAULT_LADDER = ('minimax-coding-plan/MiniMax-M3.1-Flash', 'opencode-go/deepseek-v4.1-flash#low',
                  COMMANDCODE_FLASH_MODEL + '#low', 'opencode-go/deepseek-v4.1-flash',
                  COMMANDCODE_FLASH_MODEL, HY4_FLASH_MODEL, TERRA_MODEL)
DEFAULT_AUDIT_MODEL = TERRA_MODEL
# A defect needs a second auditor before any fix: the MiniMax M3 auditor flagged 8 of 8 functions
# with false positives, DeepSeek #low passed real defects (cross-audits, 2026-11-13). The second
# opinion is a separate worker run that must prove each finding from the listing.
DEFAULT_CONFIRM_MODEL = TERRA_MODEL
# The coordinator must outlive an OpenCode Go usage limit, so it runs on the MiniMax plan.
COORDINATOR_MODEL = 'minimax-coding-plan/MiniMax-M3.1-Flash'
# Runtime-library x87 helpers: 7 of 10 production audit defects (control word, ST0, caller
# slots the gate cannot see), 99 functions / 18 KB of supply (2026-11-13).
DEFAULT_SKIP_RANGES = ((0x00940000, 0x00953000),)
# Events that wake a waiting coordinator (`wait`). Supply events are absent on purpose: a
# continuous farm idles and rescans by itself, and waking a coordinator on supply-empty led it
# to stop the farm with 20 workers still running (2026-11-15 21:27, then nine idle hours).
NOTABLE_EVENTS = ('farm-exit', 'open-defect', 'model-unavailable', 'model-limited', 'farm-crash-loop',
                  'request-failed', 'acceptance-quarantined', 'hint-evidence-needed', 'fix-evidence-needed')
# Settings `set` may change on a running farm (RUN/control.json), applied on the next tick.
CONTROL_KEYS = ('workers', 'audit_workers', 'min_bytes', 'max_bytes', 'ladder', 'audit_model',
                'confirm_model', 'include_held', 'skip_ranges', 'attempt_minutes', 'stall_minutes',
                'lift_lane_minutes', 'input_lane_workers')
SUPPLY_KEYS = ('min_bytes', 'max_bytes', 'include_held', 'skip_ranges')   # changes that reselect the frontier
RESTART_SECONDS = 30         # supervisor pause before relaunching a farm that died
MAX_RESTARTS_PER_HOUR = 5    # more than this is a crash loop: stop and wake the coordinator
# OpenCode Go has a 5-hour usage limit per account (HTTP 429, hit 2026-11-13 evening); it pauses
# every model of that provider until it resets. Without a reset time, retry after this long.
DEFAULT_LIMIT_SECONDS = 900
# A limit message that says "Switch to a free model" (the Go monthly limit, 2026-11-21) covers
# paid models only. A message without that phrase keeps pausing the provider's free models too.
FREE_EXEMPT = re.compile(r'switch to a free model', re.I)


# A provider that no longer routes one model (2026-11-21 22:58: "provider.no-route: Model
# unavailable: opencode-go/hy4-flash-free") pauses that model only; it is retried after this long.
NO_ROUTE = re.compile(r'provider\.no-route|model unavailable', re.I)
NO_ROUTE_SECONDS = 3600


def _is_free_model(model):
    return model.split('#', 1)[0].endswith('-free')


def _limit_labels(limits, now):
    """Active limits for status display; a paid-only limit is labelled so."""
    return {f"{key} (paid)" if ':' not in key and FREE_EXEMPT.search(data.get('message', '')) else key:
            data['until']
            for key, data in limits.items() if data['until'] > now}


def _limit_key(model, message=''):
    """limited_providers key: the provider, provider:free for its free tier, or the model itself
    when the provider stopped routing just that model."""
    if NO_ROUTE.search(message or ''):
        return model.split('#', 1)[0]
    provider = model.split('/', 1)[0]
    return f'{provider}:free' if _is_free_model(model) else provider

# Translations this large start from the machine draft in their source file. 2026-11-21: workers
# asked to retype 1,000-line drafts stalled silently or wrote stubs, while those drafts had only
# 2-4 build errors each. A self-disclaimed candidate under 1/DRAFT_STUB_FRACTION of the draft's
# size is a stub and is replaced by the draft.
DRAFT_SEED_MIN_BYTES = 2000
DRAFT_STUB_FRACTION = 4
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
AUDIT_AUTHORITY = (
    'The target source, hints and audit bundle MUST be the supplied immutable snapshots, never moving canonical files.',
    'Callee C is navigation only: never authority or proof for signatures, ABI or fidelity. Every ABI finding must '
    'cite original instructions at preferred-image VAs and the pinned CM0102 3.9.68 binary SHA-256 '
    '0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.',
    'A bound target review is not callee composition or runtime proof; callee reads introduce no automatic global dependency.',
    'Unproven consumption (Adam, 2026-11-19): a value whose upper bits or register residue differ from the original '
    '(a zero-extended byte where the original pushes a whole register, ECX/EDX/EDI left by an earlier call) is a '
    'finding only when listing VAs show its consumer reads those bits. Without that evidence, list it under '
    '"boundaries" with the same line/detail/evidence fields, not under "findings"; it is an explicit unknown '
    'boundary, not a defect. An open concern of this kind is covered by moving it to "boundaries".',
)


@dataclass
class Item:
    fn: str
    bytes: int
    ready: bool
    waits: int
    module: str


FARM_LANE_STOP = 'stopped by the farm (deadline or drain)'


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
    continuous: bool = False       # idle and rescan on empty supply; only STOP or the deadline ends the run
    rescan_seconds: float = 120    # how often an empty frontier is fetched again
    lift_lane_minutes: float = 0   # captures -> accepted lifts pass (scripts/cmre_lift_lane.py); 0 disables
    input_lane_workers: int = 0    # authored-input workers (scripts/cmre_input_lane.py, ADR 0013); 0 disables
    only: tuple = ()               # restrict the frontier to these functions (trial runs); () takes all


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
_RETURN_TYPE_WEAKENING = re.compile(
    r'\s*(?:GCC|clang)\s+diagnostic\s+(?:ignored|warning)\s+"-Wreturn-type"\s*')
_PRAGMA_DIRECTIVE = re.compile(r'^[ \t]*(?P<op>#[ \t]*pragma)\b(?P<body>[^\n]*)', re.M)
_PRAGMA_OPERATOR = re.compile(r'\b_Pragma\s*\(\s*L?"(?P<body>(?:\\.|[^"\\])*)"\s*\)')


def _mask_noncode(match):
    return re.sub(r'[^\r\n]', ' ', match.group())


def _weakens_return_type(text, code):
    """Recognize literal diagnostic pragmas, not arbitrary preprocessor expansion or UB.

    Keep pragma operands but mask comments. The code-only view proves the
    directive/operator itself is not inside an ordinary string or character.
    Both views retain offsets and line boundaries after C line splicing.
    """
    operands = _NONCODE.sub(
        lambda m: _mask_noncode(m) if m.group().startswith('/') else m.group(), text)
    for match in _PRAGMA_DIRECTIVE.finditer(operands):
        if code[match.start('op')] == '#' and _RETURN_TYPE_WEAKENING.fullmatch(match['body']):
            return True
    for match in _PRAGMA_OPERATOR.finditer(operands):
        if code[match.start():match.start() + 7] != '_Pragma':
            continue
        # C _Pragma destringizing removes only escaped double quotes/backslashes.
        body = re.sub(r'\\(["\\])', r'\1', match['body'])
        if _RETURN_TYPE_WEAKENING.fullmatch(body):
            return True
    return False


def mechanical_problems(fn, text):
    """Cheap rejections; ignore comments/ordinary strings, inspect literal pragma operands."""
    spliced = text.replace('\\\r\n', '').replace('\\\n', '')
    code = _NONCODE.sub(_mask_noncode, spliced)
    problems = [message for pattern, message in _BANNED if pattern.search(code)]
    if _weakens_return_type(spliced, code):
        problems.append('return-type diagnostic weakening permits undefined register capture')
    if re.search(r'/\*\s*WIP', text):
        problems.append('WIP marker present: the source is not finished')
    if not re.search(r'\bcm_%s\s*\(' % re.escape(fn), code):
        problems.append(f'missing entry definition cm_{fn}(...)')
    return problems


# --- pre-accept safeguards ---------------------------------------------------

# A worker that writes either marker declares the candidate is not acceptance
# evidence. The farm refuses it before the gate and records a BLOCKER, instead
# of letting a passing random corpus accept an explicitly unfinished source.
_CANDIDATE_DISCLAIMER = re.compile(r'\bUNVALIDATED\s+CANDIDATE\b', re.I)
_CANDIDATE_BLOCKER = re.compile(r'(?<![A-Za-z0-9_])BLOCKER\s*:')
_CANDIDATE_MUST_NOT_ACCEPT = re.compile(r'\bmust\s+not\s+be\s+accepted\b', re.I)
_X87_BRIDGE = 'cm_x87_CIpow'


def candidate_disclaimer_problems(text):
    """Self-declarations that the source is not validated evidence."""
    problems = []
    if _CANDIDATE_DISCLAIMER.search(text):
        problems.append('candidate self-declares UNVALIDATED CANDIDATE; it is not acceptance evidence')
    if _CANDIDATE_BLOCKER.search(text):
        problems.append('candidate self-declares BLOCKER: unresolved work; it is not acceptance evidence')
    if _CANDIDATE_MUST_NOT_ACCEPT.search(text):
        problems.append('candidate self-declares that it must not be accepted; it is not acceptance evidence')
    return problems


def candidate_safeguard_relevant(raw):
    """True when this candidate needs disclaimer or ADR 0010 pre-accept evidence."""
    if not raw:
        return False
    if _X87_BRIDGE.encode() in raw:
        return True
    return bool(candidate_disclaimer_problems(raw.decode('utf-8', errors='replace')))


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
        proposal = job.get('hint_source', str(Path(job['source']).with_suffix('.hints.json')))
        for name in {_rel(root, proposal), Path(proposal).as_posix(), str(Path(proposal))}:
            edit[name] = 'allow'
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
        proposal = job.get('hint_source', str(Path(job['source']).with_suffix('.hints.json')))
        rules.append(('edit', _rel(root, proposal), 'allow'))
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
        parts += ['', *AUDIT_AUTHORITY]
        parts += ['', checklist]
        source = job.get('_snap', {}).get('c')
        text = source.decode('utf-8', errors='replace') if source is not None else _file(root, f'recovery/c/{fn}.c')
        parts += _fenced(f'Accepted source recovery/c/{fn}.c', text, 'c')
        hints = job.get('_snap', {}).get('hint')
        if hints is not None:
            parts += _fenced('Reviewed hints snapshot', hints.decode('utf-8', errors='replace'), 'json')
        if job.get('_open_findings'):
            parts += _fenced('Open concerns to review explicitly (unbound, not assumed true)',
                             json.dumps(job['_open_findings'], indent=1), 'json')
    else:
        parts += ['', '## Rules (docs/agent/C.md, Loop step 2)',
                  _section(_file(root, 'docs/agent/C.md'), r'2\. Write ', r'3\. '),
                  '', _section(farm_md, r'## Worker rules', r'## ')]
        if kind == 'fix':
            parts += ['', checklist]
            parts += _fenced('Accepted starting source snapshot',
                             (job.get('_snap', {}).get('c') or b'').decode('utf-8', errors='replace'), 'c')
            parts += _fenced('Starting hints snapshot',
                             (job.get('_snap', {}).get('hint') or b'').decode('utf-8', errors='replace'), 'json')
        draft = re.search(r'^draft: (\S+)', brief, re.M)
        if draft and job.get('_omit_draft'):
            parts += ['', f'## Draft {draft.group(1)}',
                      'The draft is already in your source file; it is not repeated here.']
        elif draft:
            parts += _fenced(f'Draft {draft.group(1)}', _file(root, draft.group(1)), 'c')
    parts += _fenced('Header recovery/include/cmre.h', _file(root, 'recovery/include/cmre.h'), 'c')
    return '\n'.join(parts) + '\n'


def _bundle_steps(job, first):
    """Prompt lines that point at the bundle, or the classic brief step without one."""
    if job.get('bundle'):
        return [f"1. Read {job['bundle']} completely, once. It holds the brief with the full listing, "
                f"{first}, and recovery/include/cmre.h. Do not rerun the brief or reread those files."]
    return [f"1. {PY} -m cmre brief {job['fn']}   (evidence, callers, callees, globals, draft path, listing)"]


def _hint_steps(root, job):
    proposal = job.get('hint_source', str(Path(job['source']).with_suffix('.hints.json')))
    return [f'Private c_hints proposals go ONLY in {_rel(root, proposal)} (initialized from dispatch hints).',
            'Canonical recovery/hints is read-only. Tests use current canonical hints, not your proposal.',
            'If your proposal changes, finish FARM-RESULT: evidence-needed <why>; the controller retains it '
            'for parent validation/application at an authorized stopped boundary. A proposal is not gate evidence.']


DRAFT_SEEDED_LINES = [
    'Your source file already holds the machine draft: Ghidra pseudocode converted by cmre draft. '
    'It is translation input, not evidence, and usually does not build yet. Do not rewrite the file. '
    'Test it first, fix the build errors, then fix mismatches one at a time. Repairing in place does '
    'not narrow the review: check every statement against the listing, edited or not.',
    'The gate can take several minutes on a function this large: run each cmre test or explain with '
    'a shell timeout of 600000 ms and wait for its result.',
    'A build failure of the seeded draft is not a result: fix each reported error from the listing '
    '(argument counts, receivers, types) and test again.',
    'Do not read the hints file unless you are proposing inputs: for large functions it holds '
    'thousands of lines of captured state that you do not need to translate.',
]


def translate_prompt(root, job):
    fn, src = job['fn'], _rel(root, job['source'])
    lines = [
        f'CM0102 farm worker. Translate {fn} ({job["bytes"]} bytes, {job.get("module") or "unknown module"}) '
        'to plain C that passes the semantic gate. Do the work now; do not reply with a plan.',
        '',
        f'Write your C ONLY to {src}. '
        'Other than the private hints proposal below, every other edit is denied.',
        *_hint_steps(root, job),
        *AUDIT_AUTHORITY[1:],
    ]
    if not job.get('bundle'):
        lines += ['Read docs/agent/FARM.md section "Worker rules" once before writing.']
    lines += [
        'Everything you need is the brief, the draft it names, the C.md Rules, and the header '
        'recovery/include/cmre.h (types, CM_GLOBAL, CM_THISCALL, CM_FS). Do not search the repository, '
        'list directories or read cmre/ Python sources.',
        *(DRAFT_SEEDED_LINES if job.get('draft_seeded') else
          ['Write a first complete version within your first few steps and test it early; '
           'iterate from test output.']),
        '',
        *_bundle_steps(job, 'the draft, the C.md rules, the worker rules'),
        (f'2. Repair {src} in place with small edits; do not rewrite the file. Check every statement '
         'against the listing and resolve every NEEDS-REVIEW construct the brief lists.'
         if job.get('draft_seeded') else
         f'2. Write {src} from the draft or the starting source below; check every statement against the listing.'),
        f'3. {PY} -m cmre test {fn} --c --source {src}',
        f'4. On a mismatch: {PY} -m cmre explain {fn} --c --source {src} --case N   then fix the first difference.',
        ('   Stop after 5 failed gate runs; build failures of the seeded draft do not count.'
         if job.get('draft_seeded') else '   Stop after 5 failed tests. Build failures count.'),
        '',
        'Finish with exactly one line: FARM-RESULT: pass | FARM-RESULT: fail <first remaining mismatch> | '
        'FARM-RESULT: evidence-needed <private hint proposal needs parent>',
    ]
    if job.get('prior'):
        p = job['prior']
        lines += ['', f'Previous attempt ({p["model"]}) failed: {p["detail"][:1500]}']
        if job.get('draft_reseeded'):
            lines.append('It left only a partial stub, so the farm replaced it with the machine draft; '
                         'start from the draft.')
        elif job.get('_seed_bytes'):
            lines.append(f'Its source is already in {src}; start from it.')
        else:
            lines.append('No C source was carried forward; start from the draft and write the C source.')
        if p['detail'].startswith('worker self-declared unfinished'):
            lines += ['Finish the work it names. Remove its unfinished-work statement only once that work '
                      'is done and the gate passes.']
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
        f'The accepted source is already in {src}. Edit ONLY that file and private hints proposal '
        f'{_rel(root, job.get("hint_source", str(Path(job["source"]).with_suffix(".hints.json"))))}.',
        *_hint_steps(root, job),
        *AUDIT_AUTHORITY[1:],
        f'Review the immutable starting inputs {job.get("review_source", src)} and '
        f'{job.get("review_hints", "no hints snapshot")}. The scratch source is your editable candidate.',
        'The originating finding artifacts are fixed inputs, not a mutable latest review: '
        + json.dumps(job.get('_origin_review')),
        *([] if job.get('bundle') else ['Read docs/agent/FARM.md sections "Worker rules" and "Audit checklist".']),
        *_bundle_steps(job, 'the draft, the C.md rules, the worker rules, the audit checklist'),
        '   Verify each finding against the listing. A finding without listing evidence may be a false '
        'positive: keep the original code for it.',
        f'2. Fix confirmed defects, then {PY} -m cmre test {fn} --c --source {src} until it passes (5 failures max).',
        '',
        'Audit findings:', findings, '',
        'Finish with exactly one line: FARM-RESULT: pass <what changed> | FARM-RESULT: fail <why> | '
        'FARM-RESULT: evidence-needed <private hint proposal needs parent>',
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
        f'Review only the immutable source snapshot {job.get("review_source", f"recovery/c/{fn}.c")} '
        f'and hints snapshot {job.get("review_hints", f"recovery/hints/{fn}.json")} (absent means no hints). '
        'Do not substitute the live canonical files.',
        *AUDIT_AUTHORITY,
        'Independently review every open concern below; "ok" must cover them, not silently discard them: '
        + json.dumps(job.get('_open_findings', [])),
        *(_bundle_steps(job, 'the audit checklist, the accepted source') if job.get('bundle') else [
            'Read docs/agent/FARM.md section "Audit checklist", then:',
            f'1. {PY} -m cmre brief {fn}   (it includes the listing) and read {job.get("review_source", f"recovery/c/{fn}.c")}']),
        '2. Walk every checklist item against the original listing. Callee C may locate evidence only, '
        'never establish a signature or ABI. ' + ('Apart from the bundle, do' if job.get('bundle') else 'Do')
        + ' not read farm run files, the ledger, transcripts or directories.',
        f'3. Write {out} as JSON: {{"verdict": "ok"|"defect", "findings": '
        '[{"line": N, "detail": "...", "evidence": "listing VA and instruction"}], "boundaries": [same shape]}',
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
        f'Review only the immutable source snapshot {job.get("review_source", f"recovery/c/{fn}.c")} '
        f'and hints snapshot {job.get("review_hints", f"recovery/hints/{fn}.json")} (absent means no hints). '
        'Do not substitute the live canonical files.',
        *AUDIT_AUTHORITY,
        'Also independently review these open/unbound concerns; "ok" must cover them: '
        + json.dumps(job.get('_open_findings', [])),
        *(_bundle_steps(job, 'the audit checklist, the accepted source') if job.get('bundle') else [
            f'1. {PY} -m cmre brief {fn}   (it includes the listing) and read {job.get("review_source", f"recovery/c/{fn}.c")}']),
        '2. For each finding, confirm it with the listing VA and instruction, or reject it.',
        f'3. Write {out} as JSON: {{"verdict": "defect"|"ok", "findings": [only the confirmed findings, '
        'each with "line", "detail" and "evidence"], "boundaries": [unproven-consumption items, same shape]}. '
        '"ok" when none is confirmed.',
        '',
        'Findings to verify:', findings, '',
        'Finish with exactly one line: FARM-AUDIT: ok | FARM-AUDIT: defect <count>',
    ]) + '\n'


# --- the scheduler ---------------------------------------------------------------

def _read(path):
    path = Path(path)
    return path.read_bytes() if path.exists() else None


def _replace(tmp, target, attempts=50, delay=0.1):
    """os.replace that waits up to ~5 s for readers: on Windows an open handle denies it."""
    for attempt in range(attempts):
        try:
            return os.replace(tmp, target)
        except PermissionError:
            if attempt == attempts - 1:
                raise
            time.sleep(delay)


def _durable_json(path, data):
    """Replace one journal record only after its complete contents reach the OS."""
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix('.tmp')
    with tmp.open('w', encoding='utf-8') as stream:
        json.dump(data, stream, indent=1)
        stream.flush()
        os.fsync(stream.fileno())
    _replace(tmp, path)


def _acceptance_pin(path, expected=...):
    path = Path(path).resolve()
    digest = acceptance_request.optional_digest(_read(path))
    if expected is not ... and digest != expected:
        raise ValueError(f'Acceptance evidence changed: {path}')
    return {'path': str(path), 'sha256': digest}


def _identity(snapshot):
    return {k: hashlib.sha256(v).hexdigest() if v is not None else None
            for k, v in snapshot.items()}


def _encode_snapshot(snapshot):
    return {k: v.hex() if v is not None else None for k, v in snapshot.items()}


def _decode_snapshot(snapshot):
    return {k: bytes.fromhex(v) if v is not None else None for k, v in snapshot.items()}


def _restore(path, data):
    path = Path(path)
    if data is None:
        path.unlink(missing_ok=True)
    else:
        path.parent.mkdir(parents=True, exist_ok=True)
        tmp = path.with_name(path.name + '.farm-tmp')
        with tmp.open('wb') as stream:
            stream.write(data)
            stream.flush()
            os.fsync(stream.fileno())
        _replace(tmp, path)


def _gate_summary(diff):
    """What a failed differential run says: counts, coverage and the first mismatch."""
    mismatches = diff.get('mismatches') or []
    text = (f"{len(mismatches)} mismatches, coverage {diff.get('coverage')}, "
            f"inconclusive {diff.get('inconclusive')}/{diff.get('cases')}")
    if mismatches:
        text += '; first: ' + json.dumps(mismatches[0], default=str)[:500]
    elif diff.get('uncovered'):
        text += f"; {len(diff['uncovered'])} uncovered instructions from {diff['uncovered'][0]}"
    return text


def report_detail(report):
    parts = [str(report.get('status'))]
    parts += [str(p) for p in report.get('problems') or []]
    for opt, build in (report.get('builds') or {}).items():
        if isinstance(build, dict) and build.get('status') not in (None, 'pass'):
            diff = build.get('difftest')
            detail = _gate_summary(diff) if isinstance(diff, dict) else build.get('detail', '')
            parts.append(f"{opt} {build.get('status')}: {detail}")
            if build.get('stderr'):
                parts.append(str(build['stderr'])[:800])
    return ' | '.join(parts)[:2500]


def input_bound(report):
    """The lowest coverage when the candidate failed on test inputs alone, else None.

    Every build compiled and ran with no mismatch, without the budget exhaustion that hides
    non-terminating C, and with no code-as-data cases: coverage is the only failing condition.
    Coverage counts original instructions the inputs reach, so no translation can change it."""
    builds = list((report.get('builds') or {}).values())
    if report.get('status') != 'fail' or report.get('problems') or not builds:
        return None
    lowest = None
    for build in builds:
        diff = build.get('difftest') if isinstance(build, dict) else None
        if build.get('status') != 'fail' or not isinstance(diff, dict) or diff.get('mismatches'):
            return None
        cases, coverage = diff.get('cases') or 0, diff.get('coverage')
        if coverage is None or diff.get('code_as_data_cases') or (diff.get('inconclusive') or 0) * 2 >= cases:
            return None
        lowest = coverage if lowest is None else min(lowest, coverage)
    return lowest


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


class ModelLauncher(Protocol):
    def start(self, job: dict, prompt: str, config: dict): ...


class Farm:
    def __init__(self, run_dir, root, cmre, launcher: ModelLauncher | None, config,
                 clock=time.monotonic, verifier=None, *, reconcile=True):
        self.run_dir, self.root, self.cmre, self.config = Path(run_dir), Path(root), cmre, config
        # Only live construction supplies the dispatch capability. Saved controls
        # tune a live farm and cannot give an offline drain a launcher.
        self.launcher = launcher
        self.clock = clock
        # Gate re-tests (median 48 s per job) run here so they never block dispatch.
        self.verifier = verifier or InlineExecutor()
        self.verifying = []        # (job, future)
        self.supply_empty = False
        self.monitor = None        # enabled only by the live dispatcher
        self.lease_waits = set()   # job numbers for the operator display
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
        self.last_scan = None      # clock() of the last frontier fetch
        self.last_input_check = None
        self.lift_launch = None     # callable(command) -> process with poll(); None: subprocess.Popen
        self.lift_proc, self.last_lift = None, None
        self.input_launch = None    # callable(command) -> process; None: subprocess.Popen
        self.input_proc, self.last_input_launch = None, None
        self.input_drain = False    # an exit-when-empty farm stopped its lane to finish
        self.recheck_ready = set()  # parked work stays durable until its verification finishes
        self.retried = {}          # fn -> parked record of a function returned to the queue
        self.state['deadline_at'] = None if config.hours is None else time.time() + config.hours * 3600
        self.seen = set(self.state['accepted']) | set(self.state['parked']) | set(self.state['skipped'])
        self.counter = sum(len(v) for v in self.state['attempts'].values())
        events, _ = _read_events(self.run_dir / 'events.jsonl', 0)
        self.counter = max([self.counter, *(e.get('n', 0) for e in events)])
        registered = [int(path.name) for path in (self.run_dir / 'dispatch').glob('*')
                      if path.is_dir() and path.name.isdigit()]
        self.counter = max([self.counter, *registered])
        for kind in ('audit', 'fix'):
            retained = [int(path.name) for path in (self.run_dir / kind / 'snapshots').glob('*')
                        if path.is_dir() and path.name.isdigit()]
            self.counter = max([self.counter, *retained])
        if reconcile:
            self._reconcile_transactions()
        changed_audits = False
        for fn in self.state['accepted'] if reconcile and config.audit_workers else ():
            changed_audits = self._backfill_accepted_identity(fn) or changed_audits
        if changed_audits:
            self.save()   # review_paths below reads the saved state
        for fn in self.state['accepted'] if reconcile and config.audit_workers else ():
            audit = self.state['audits'].get(fn, {})
            invalid = audit.get('identity') != _identity(self._inputs(fn))
            if audit.get('verdict') in ('ok', 'defect'):
                invalid = invalid or review_paths(self.run_dir, self.root, fn)['status'] != 'bound'
            if invalid:
                self._reset_audit(fn)
                changed_audits = True
                if config.audit_workers and fn not in self.audit_queue:
                    self.audit_queue.append(fn)
        if changed_audits:
            self.save()

    def _backfill_accepted_identity(self, fn):
        """Bind an accept recorded before identities existed, when the ledger proves it.

        The ledger's accepted C hash must equal the current source, and the latest audit must
        still be bound to intact snapshot evidence of the current source and hints. Without
        both, the record stays unbound and the function is audited afresh."""
        accepted, audit = self.state['accepted'][fn], self.state['audits'].get(fn, {})
        if ('identity' in accepted or not audit.get('review')
                or audit.get('verdict') not in ('ok', 'defect', 'unconfirmed')):
            return False
        identity = _identity(self._inputs(fn))
        if identity.get('c') is None or audit.get('identity') != identity:
            return False
        try:
            if getattr(self.cmre, 'ledger_c_sha256', lambda fn: None)(fn) != identity['c']:
                return False
            _bound_audit_reviews(self.run_dir, self.root, fn, identity, audit['review'])
        except (OSError, ValueError, KeyError, TypeError):
            return False
        accepted['identity'] = identity
        accepted['identity_source'] = 'ledger c.source_sha256 and bound audit snapshot'
        self._event('identity-backfilled', fn=fn)
        return True

    def _inputs(self, fn):
        return {'c': _read(self.root / 'recovery/c' / f'{fn}.c'),
                'hint': _read(self.root / 'recovery/hints' / f'{fn}.json')}

    def _writer_busy(self, fn):
        if fn in self.state.get('quarantined', {}):
            return True
        jobs = ([j for j, _, _ in self.running if j['kind'] != 'audit']
                + [j for j, _ in self.verifying] + self.accept_queue + self.pending)
        return any(j['fn'] == fn for j in jobs)

    def _reset_audit(self, fn):
        prior = self.state['audits'].get(fn)
        if prior:
            self.state.setdefault('audit_history', {}).setdefault(fn, []).append(prior.copy())
        self.state['audits'][fn] = {'rounds': 0, 'fresh_review_required': True}
        # Unknown or stale evidence cannot dismiss an unresolved legacy concern.
        if fn in self.state['defects']:
            self.state['audits'][fn]['unbound_findings'] = self.state['defects'][fn]

    def _fix_evidence_error(self, job, *, before_dispatch=False):
        if job['kind'] != 'fix':
            return None
        try:
            identity, origin = job['_finding_identity'], job['_origin_review']
            if identity.get('c') is None:
                raise ValueError('origin has no reviewed C source')
            paths, _, _ = _bound_audit_reviews(self.run_dir, self.root, job['fn'], identity, origin)
            raw = Path(paths['audit_path']).read_bytes()
            if hashlib.sha256(raw).hexdigest() != origin['verdict_sha256']:
                raise ValueError('origin verdict changed while checking findings')
            verdict = json.loads(raw)
            if verdict.get('verdict') != 'defect' or (verdict.get('findings') or []) != job['findings']:
                raise ValueError('origin verdict does not authorize these findings')
            if self.state['defects'].get(job['fn'], []) != job['findings']:
                raise ValueError('open concerns changed since the originating review')
            if before_dispatch and _identity(self._inputs(job['fn'])) != identity:
                raise ValueError('canonical inputs changed since the originating review')
            if not before_dispatch or '_fix_dispatch' in job:
                if _identity(job['_snap']) != identity:
                    raise ValueError('fix dispatch is not based on the reviewed inputs')
                _, metadata = _review_input_artifacts(
                    self.run_dir, self.root, job['fn'], identity, job['_fix_dispatch'])
                if metadata.get('origin_review') != origin or metadata.get('open_findings') != job['findings']:
                    raise ValueError('fix dispatch origin/concerns binding changed')
        except (OSError, ValueError, KeyError, TypeError) as exc:
            return f'fix evidence unavailable or changed: {exc}'
        return None

    def _invalidate_fix(self, job, detail):
        """Retire only an unaccepted owned installation; never resurrect stale concerns."""
        if '_transaction' in job:
            safe = self._rollback(job)
            self._terminal(job, 'failed' if safe else 'conflict')
        self._event('result', fn=job['fn'], kind='fix', model=job['model'], n=job['n'],
                    outcome='stale-fix', detail=detail[:300])  # evidence failure is not a model attempt
        fn, identity = job['fn'], _identity(self._inputs(job['fn']))
        audit = self.state['audits'].get(fn, {})
        if (audit.get('verdict') in ('ok', 'defect') and audit.get('identity') == identity
                and audit.get('review') != job.get('_origin_review')
                and self.state['defects'].get(fn, []) == (
                    audit.get('findings', []) if audit['verdict'] == 'defect' else [])):
            try:
                _bound_audit_reviews(self.run_dir, self.root, fn, identity, audit['review'])
            except (OSError, ValueError, KeyError, TypeError):
                pass
            else:
                return  # newer intact evidence stays authoritative, not this stale fix
        self._reset_audit(fn)
        if fn not in self.audit_queue:
            self.audit_queue.append(fn)
        self._event('fix-evidence-needed', fn=fn, n=job['n'], detail=detail[:300])

    def _retry_worker(self, job):
        if job['kind'] == 'fix':
            # A provider retry is a new immutable dispatch, with the same origin.
            extra = {k: job[k] for k in ('bytes', 'module', 'hold', 'findings',
                                        '_finding_identity', '_origin_review') if k in job}
            again = self._new_job('fix', job['fn'], job['model'], job['rung'], **extra)
        else:
            extra = {k: job[k] for k in ('bytes', 'module', 'hold', 'prior') if k in job}
            again = self._new_job(job['kind'], job['fn'], job['model'], job['rung'], **extra)
            # The retry restarts from the same seed, so a draft-seeded job stays in repair mode.
            again['_from_draft'] = bool(job.get('draft_seeded'))
        again['_seed'] = job.get('_seed_bytes')
        self.pending.insert(0, again)

    def _transaction(self, job, phase=None, **updates):
        path = Path(job['_transaction'])
        data = json.loads(path.read_text(encoding='utf-8'))
        if _identity(_decode_snapshot(data['candidate'])) != data['identity']:
            raise RuntimeError(f'candidate journal identity mismatch: {path}')
        if phase is not None or updates:
            data.update(updates)
        if phase is not None:
            data['phase'] = phase
        if phase is not None or updates:
            _durable_json(path, data)
        return data

    def _begin_transaction(self, job):
        # The journal contains bytes, not merely names of mutable draft files.
        hint = self._inputs(job['fn'])['hint']
        # A fresh read is an input, not proof that this job wrote it.
        owned_hint = self._owns_hint(job, hint)
        candidate = {'c': _read(job['source']), 'hint': hint if owned_hint else job['_snap']['hint']}
        job['_unowned_hint'] = not owned_hint
        path = self.run_dir / 'transactions' / f"{job['n']:08d}.json"
        job['_transaction'] = str(path)
        metadata = {k: v for k, v in job.items()
                    if k not in ('_snap', '_seed', '_seed_bytes')}
        _durable_json(path, {'schema': 'cmre-farm-candidate/1', 'phase': 'prepared',
                            'job': metadata, 'prior': _encode_snapshot(job['_snap']),
                            'candidate': _encode_snapshot(candidate), 'identity': _identity(candidate)})

    def _rollback(self, job):
        data = self._transaction(job)
        current = self._inputs(job['fn'])
        prior, candidate = _decode_snapshot(data['prior']), _decode_snapshot(data['candidate'])
        conflicts = []
        for key, directory, suffix in (('c', 'c', '.c'), ('hint', 'hints', '.json')):
            # Canonical hints are read-only: legacy journals did not prove
            # ownership of worker/external changes, so never restore those bytes.
            allowed = ((prior[key], candidate[key]) if key == 'c' and data['phase'] != 'prepared'
                       else (prior[key],))
            if current[key] in allowed:
                if key == 'c' and current[key] != prior[key]:
                    _restore(self.root / 'recovery' / directory / (job['fn'] + suffix), prior[key])
            else:
                conflicts.append(key)
        if conflicts:
            self._event('transaction-conflict', fn=job['fn'], n=job['n'], fields=conflicts)
        return not conflicts

    @staticmethod
    def _owns_hint(job, hint):
        return hint == job['_snap']['hint']

    def _running_inputs_match(self, job):
        """Provider failures cannot claim whatever happened to appear after dispatch."""
        current = self._inputs(job['fn'])
        if current['c'] != job['_snap']['c'] or not self._owns_hint(job, current['hint']):
            self._event('running-input-conflict', fn=job['fn'], n=job['n'])
            self._park(job['fn'], 'running job lost canonical input ownership')
            return False
        return True

    def _quarantine(self, job, reason):
        self._transaction(job, 'quarantined', quarantine_reason=reason)
        self.state.setdefault('quarantined', {})[job['fn']] = {
            'n': job['n'], 'journal': job['_transaction'], 'reason': reason}
        self.seen.add(job['fn'])
        self._event('acceptance-quarantined', fn=job['fn'], n=job['n'], reason=reason)

    def _terminal(self, job, phase):
        if '_transaction' not in job:
            return
        self._transaction(job, phase)
        if job.get('_resume_collect') and phase in ('complete', 'failed'):
            self.state.setdefault('resume_collected', {})[job['fn']] = job['_resume_collect']
        if job.get('_request') and phase in ('complete', 'failed', 'conflict'):
            Path(job['_request']).unlink(missing_ok=True)

    def _reconcile_transactions(self):
        for path in sorted((self.run_dir / 'transactions').glob('*.json')):
            data = json.loads(path.read_text(encoding='utf-8'))
            if data.get('schema') != 'cmre-farm-candidate/1':
                raise RuntimeError(f'unsupported candidate journal: {path}')
            job = data['job']
            if _identity(_decode_snapshot(data['candidate'])) != data['identity']:
                raise RuntimeError(f'candidate journal identity mismatch: {path}')
            self.counter = max(self.counter, job['n'])
            if data['phase'] in ('complete', 'failed', 'conflict', 'reverify'):
                # A crash may fall between recording terminal status and removing its request.
                if data['phase'] != 'reverify':
                    self._terminal(job, data['phase'])
                continue
            job['_snap'] = _decode_snapshot(data['prior'])
            if data['phase'] == 'quarantined':
                self._quarantine(job, data['quarantine_reason'])
                continue
            if job['kind'] == 'fix' and data['phase'] not in ('accepting', 'accepted'):
                detail = self._fix_evidence_error(job)
                if detail:
                    self._invalidate_fix(job, detail)
                    continue
            matched = False
            nonacceptance_proven = False
            if data['phase'] in ('accepting', 'accepted'):
                if data['phase'] == 'accepted':
                    matched = True  # persisted successful adapter return
                else:
                    try:
                        evidence = self.cmre.acceptance_identity(job['fn'])
                    except (AttributeError, NotImplementedError) as exc:
                        self._event('acceptance-evidence-unavailable', fn=job['fn'], detail=str(exc))
                    else:
                        matched = evidence == data['identity']
                        nonacceptance_proven = evidence == {'status': 'not-accepted'}
            if data['phase'] in ('accepting', 'accepted') and (
                    not nonacceptance_proven and (
                        not matched or _identity(self._inputs(job['fn'])) != data['identity'])):
                self._quarantine(job, 'acceptance unresolved; preserve inputs for manual/current cmre ledger review')
                continue
            current_identity = _identity(self._inputs(job['fn']))
            if matched and current_identity == data['identity']:
                if data['phase'] != 'accepted':
                    self._transaction(job, 'accepted', acceptance_evidence=evidence)
                self._accepted(job)
            else:
                pending_blocker = None
                candidate_bytes = _decode_snapshot(data['candidate'])['c']
                if (data['phase'] in ('installed', 'accepting')
                        and current_identity == data['identity']
                        and candidate_safeguard_relevant(candidate_bytes)):
                    pending_blocker = self._pre_accept_blocker(job, data)
                    if pending_blocker is None and data['phase'] == 'installed':
                        # The retained proof is intact: the normal serial guard decides acceptance.
                        self.accept_queue.append(job)
                        continue
                safe = self._rollback(job)
                self._terminal(job, 'reverify' if safe else 'conflict')
                if not safe:
                    self._park(job['fn'], 'interrupted candidate conflicts with current canonical inputs')
                elif pending_blocker:
                    job['_safeguard_blocker'] = True
                    self._event('acceptance-blocked', fn=job['fn'], n=job['n'],
                                detail=pending_blocker[:300])
                    self._park(job['fn'], pending_blocker)
                self._event('candidate-reverification-required', fn=job['fn'], n=job['n'])
        self.save()

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
        _replace(tmp, self.state_path)

    def observe(self, *, accepting=None, acceptance_queue=None):
        """Publish operator telemetry without touching the scheduler or its journal."""
        if self.monitor is None:
            return
        now, tick = time.time(), self.clock()

        def row(job, stage, since=None):
            return {key: job[key] for key in ('n', 'fn', 'kind', 'model', 'confirm', 'module', 'bytes')
                    if key in job} | {'stage': stage, 'since': since}

        jobs = [row(job, 'model', now - (tick - started)) for job, _, started in self.running]
        jobs += [row(job, 'gate complete' if future.done() else 'gating' if future.running() else 'gate queue')
                 for job, future in self.verifying]
        queue = self.accept_queue if acceptance_queue is None else acceptance_queue
        jobs += [row(job, 'awaiting writer' if job['n'] in self.lease_waits else 'accept queue') for job in queue]
        if accepting is not None:
            jobs.append(row(accepting, 'accepting', now))

        frontier = [item for item in (self.frontier or []) if item.fn not in self.seen]
        scan = self.last_scan
        next_scan = None
        if scan is not None and self.frontier is not None and (self.config.continuous or not self.frontier):
            next_scan = now + scan - tick + self.config.rescan_seconds * (5 if self.frontier else 1)
        accepted = self.state['accepted']
        inputs = self.state.get('input_needed', {})
        payload = {
            'schema': 'cmre-farm-live/1', 'pid': os.getpid(), 'at': now, **self.monitor,
            'stopping': self.stopping(),
            'settings': {key: getattr(self.config, key) for key in CONTROL_KEYS},
            'counts': {'accepted_functions': len(accepted), 'accepted_bytes': sum(a['bytes'] for a in accepted.values()),
                       'parked': len(self.state['parked']), 'input_needed': len(inputs),
                       'input_bytes': sum(a.get('bytes', 0) for a in inputs.values()),
                       'defects': len(self.state['defects']), 'quarantined': len(self.state.get('quarantined', {}))},
            'scheduler': {
                'frontier': len(frontier) if self.frontier is not None else None,
                'frontier_bytes': sum(item.bytes for item in frontier) if self.frontier is not None else None,
                'pending': len(self.pending), 'audit_queue': len(self.audit_queue),
                'last_scan': now - (tick - scan) if scan is not None else None,
                'next_scan': next_scan, 'deadline': self.state.get('deadline_at'),
                'supply_empty_since': self.state.get('supply_empty_since') if self.supply_empty else None,
                'unavailable': sorted(self.state['unavailable_models']),
                'limited': _limit_labels(self.state['limited_providers'], now),
                'translation_model': self._rung_model(0)[0],
                'audit_model': self._audit_model(self.config.audit_model) if self.config.audit_model else None,
            },
            'jobs': jobs,
        }
        path = self.run_dir / 'live.json'
        tmp = path.with_suffix('.tmp')
        try:
            tmp.write_text(json.dumps(payload), encoding='utf-8')
            tmp.replace(path)
        except OSError:
            pass  # Sharing violations or telemetry I/O must never stop recovery.

    def stopping(self):
        return (self.run_dir / 'STOP').exists() or (self.deadline is not None and self.clock() >= self.deadline)

    def finished(self):
        return (self.stopping() and not self.running and not self.accept_queue
                and not self.verifying and self.lift_proc is None and self.input_proc is None)

    def drained(self):
        """The run is over. A continuous farm ends only on STOP or its deadline: with no supply
        it idles and rescans, so work that arrives later starts without a restart.
        Both modes wait for an active lift pass to finish and report its result."""
        if self.finished():
            return True
        if self.config.continuous:
            return False
        supply_done = self.frontier == [] or self.config.workers == 0
        done = (not self.running and not self.pending and not self.accept_queue
                and not self.verifying and not self.audit_queue and self.lift_proc is None and supply_done)
        if done and self.input_proc is not None:
            self.input_drain = True   # the lane would supply forever: stop it, then exit
            self._stop_input_lane()
            return False
        return done

    # -- live control
    def _control(self):
        """Apply RUN/control.json (written by `set`): no drain or restart for a setting change."""
        try:
            control = json.loads((self.run_dir / 'control.json').read_text(encoding='utf-8'))
        except (OSError, ValueError):
            return
        changes = {}
        for key in CONTROL_KEYS:
            if key not in control:
                continue
            value = control[key]
            if key == 'skip_ranges':
                value = tuple(tuple(pair) for pair in value)
            if value != getattr(self.config, key):
                changes[key] = [getattr(self.config, key), value]
                setattr(self.config, key, value)
        if any(key in changes for key in SUPPLY_KEYS):
            self.frontier, self.last_scan = None, None
        if 'ladder' in changes:
            for job in self.pending:    # rungs index the ladder: keep requeued jobs on a real rung
                job['rung'] = min(job['rung'], len(self.config.ladder) - 1)
        applied = self.state.setdefault('control_applied', [])
        for extension in control.get('add_hours', []):
            if extension['id'] in applied:
                continue
            applied.append(extension['id'])
            if self.deadline is not None:
                self.deadline += extension['hours'] * 3600
                self.state['deadline_at'] = (self.state.get('deadline_at') or time.time()) + extension['hours'] * 3600
                changes['deadline_hours'] = [None, extension['hours']]
        if changes:
            self._event('config-changed', changes=changes)

    def summary(self):
        accepted = self.state['accepted']
        by_model = {}
        for rows in self.state['attempts'].values():
            for row in rows:
                if row.get('kind') == 'audit' or row.get('recheck'):
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
                'queue': self.state.get('queue', 0), 'idle': self.state.get('idle_slots', 0),
                'supply_empty_since': self.state.get('supply_empty_since'),
                'input_needed': len(self.state.get('input_needed', {})),
                'audits_ok': audits.count('ok'),
                'audits_defect': audits.count('defect'),
                'audit_errors': audits.count('error'),
                'audits_unconfirmed': audits.count('unconfirmed'),
                'audits_disputed': sum(a.get('verdict') == 'ok' and bool(a.get('disputed') or a.get('dismissed'))
                                       for a in self.state['audits'].values()),
                 'open_defects': sorted(self.state['defects']),
                 'boundaries': sorted(self.state.get('boundaries', {})),
                 'quarantined': dict(self.state.get('quarantined', {})),
                'unavailable_models': dict(self.state['unavailable_models']),
                'limited': _limit_labels(self.state.get('limited_providers', {}), time.time()),
                'input_lane': input_lane_summary(self.run_dir),
                'stopping': self.stopping()}

    # -- supply
    def _scan_due(self):
        """Fetch the frontier at start, when it ran out (at most every rescan_seconds), and in a
        continuous run every five intervals so newly lifted larger functions jump the queue."""
        if self.frontier is None or self.last_scan is None:
            return True
        age = self.clock() - self.last_scan
        if not self.frontier:
            return age >= self.config.rescan_seconds
        return self.config.continuous and age >= 5 * self.config.rescan_seconds

    def _next_item(self, *, recheck_only=False):
        include_held = self.config.include_held
        if self._scan_due():
            self.last_scan = self.clock()
            items = self.cmre.frontier(FRONTIER_LIMIT, self.config.min_bytes, self.config.max_bytes,
                                       include_held, self.config.owner)
            items = [i for i in items if (i.fn not in self.seen or i.fn in self.recheck_ready)
                     and not _skipped(i.fn, self.config.skip_ranges)
                     and (not self.config.only or i.fn in self.config.only)]
            held = self.cmre.held([i.fn for i in items]) if include_held else set()
            # Reuse retained work first, then prefer unheld, callee-ready, larger items.
            self.frontier = sorted(items, key=lambda i: (i.fn not in self.recheck_ready,
                                                        i.fn in held, not i.ready, -i.bytes, i.fn))
        while self.frontier:
            if recheck_only and self.frontier[0].fn not in self.recheck_ready:
                return None
            item = self.frontier.pop(0)
            recheck = item.fn in self.recheck_ready
            if (item.fn in self.seen and not recheck) or self._writer_busy(item.fn):
                continue
            self.seen.add(item.fn)
            try:
                self.cmre.claim(item.fn, self.config.owner, max(self.config.hours or 24, 1),
                                include_held=include_held)
            except ValueError as exc:
                if recheck:
                    # Keep the parked record discoverable across rescans and restarts.
                    self._event('recheck-deferred', fn=item.fn, reason=str(exc)[:300])
                else:
                    self.state['skipped'][item.fn] = str(exc)[:300]
                    self._event('skipped', fn=item.fn, reason=str(exc)[:300])
                continue
            return item
        return None

    def _limited_until(self, model):
        """Wall-clock end of the model's provider usage limit, or None when it is usable.
        A paid-usage limit whose message exempts free models does not pause the free tier."""
        provider = model.split('/', 1)[0]
        limits = self.state['limited_providers']
        found = [limits.get(_limit_key(model)), limits.get(model.split('#', 1)[0])]
        if _is_free_model(model):
            paid = limits.get(provider)
            found.append(None if paid and FREE_EXEMPT.search(paid.get('message', '')) else paid)
        ends = [limit['until'] for limit in found if limit and limit['until'] > time.time()]
        return max(ends) if ends else None

    def _available(self, model):
        return model not in self.state['unavailable_models'] and self._limited_until(model) is None

    def _audit_model(self, preferred):
        """Keep the preferred auditor when usable, otherwise use an available ladder rung."""
        return next((model for model in (preferred, *reversed(self.config.ladder))
                     if self._available(model)), None)

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
            if job['fn'] in self.state.get('quarantined', {}):
                i += 1
                continue
            detail = self._fix_evidence_error(job, before_dispatch=True)
            if detail:
                self.pending.pop(i)
                self._invalidate_fix(job, detail)
                continue
            active = ([j for j, _, _ in self.running if j['kind'] != 'audit']
                      + [j for j, _ in self.verifying] + self.accept_queue)
            if any(j['fn'] == job['fn'] for j in active):
                i += 1
                continue
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
            name = f'{fn}-{self.counter}.confirm.json' if extra.get('confirm') else f'{fn}-{self.counter}.json'
            job['audit_path'] = str(self.run_dir / 'audit' / name)
        else:
            job['source'] = str(self.run_dir / 'src' / f'{fn}.c')
            job['hint_source'] = str(self.run_dir / 'hints' / f'{fn}-{self.counter}.json')
        return job

    def _resume_seed(self, fn, *, collect=False):
        """An interrupted candidate is a hash-bound draft, never gate evidence."""
        manifest = self.run_dir / 'resume-candidates.json'
        if not manifest.exists():
            return None
        try:
            data = json.loads(manifest.read_text(encoding='utf-8'))
            if data.get('schema') != 'cmre-farm-resume-candidates/1':
                raise ValueError('unsupported resume-candidate schema')
            row = data.get('sources', {}).get(fn)
            if not row:
                return None
            if collect and not row.get('collect'):
                return None
            if collect and self.state.get('resume_collected', {}).get(fn) == row.get('sha256'):
                return None
            if collect:
                hints = _read(self.root / 'recovery/hints' / f'{fn}.json')
                digest = hashlib.sha256(hints).hexdigest() if hints is not None else None
                if 'hints_sha256' not in row or row['hints_sha256'] != digest:
                    raise ValueError('resume candidate hints identity changed or missing')
            source = (self.run_dir / row['path']).resolve()
            if not source.is_relative_to(self.run_dir.resolve()):
                raise ValueError('resume candidate escapes run directory')
            content = source.read_bytes()
            if hashlib.sha256(content).hexdigest() != row['sha256']:
                raise ValueError('resume candidate hash mismatch')
            self._event('resume-candidate', fn=fn, sha256=row['sha256'])
            return content
        except (OSError, ValueError, KeyError, TypeError) as exc:
            self._event('resume-rejected', fn=fn, reason=str(exc)[:300])
            return None

    def _launch(self, job):
        launcher = self.launcher
        if launcher is None:
            raise RuntimeError('Model dispatch requires a live launcher')
        detail = self._fix_evidence_error(job, before_dispatch=True)
        if detail:
            self._invalidate_fix(job, detail)
            return False
        c_path = self.root / 'recovery/c' / f"{job['fn']}.c"
        hint_path = self.root / 'recovery/hints' / f"{job['fn']}.json"
        job['_snap'] = {'c': _read(c_path), 'hint': _read(hint_path)}
        if job['kind'] in ('audit', 'fix'):
            snapshot = self.run_dir / job['kind'] / 'snapshots' / str(job['n'])
            snapshot.mkdir(parents=True, exist_ok=False)
            for key, name in (('c', 'source.c'), ('hint', 'hints.json')):
                _restore(snapshot / name, job['_snap'][key])
            job['review_source'] = _rel(self.root, snapshot / 'source.c')
            job['review_hints'] = _rel(self.root, snapshot / 'hints.json')
            job['_audit_state'] = json.loads(json.dumps(self.state['audits'].get(job['fn'], {})))
            job['_open_findings'] = json.loads(json.dumps(self.state['defects'].get(job['fn'], [])))
            snapshot_metadata = {
                'identity': _identity(job['_snap']), 'fn': job['fn'], 'n': job['n'],
                'kind': job['kind'], 'origin_review': job.get('_origin_review'),
                'model': job['model'], 'confirm': bool(job.get('confirm')),
                'audit_state': job['_audit_state'], 'open_findings': job['_open_findings']}
        job.pop('bundle', None)
        bundle = self._bundle(job)
        if bundle is not None:
            job['bundle'] = _rel(self.root, bundle)
        if job['kind'] in ('audit', 'fix'):
            job['_bundle_sha256'] = _identity({'bundle': _read(bundle) if bundle is not None else None})['bundle']
            snapshot_metadata.update(bundle=job.get('bundle'), bundle_sha256=job['_bundle_sha256'])
            _durable_json(snapshot / 'identity.json', snapshot_metadata)
            job['_snapshot_metadata_sha256'] = hashlib.sha256((snapshot / 'identity.json').read_bytes()).hexdigest()
            if job['kind'] == 'fix':
                job['_fix_dispatch'] = {
                    'snapshot': str(snapshot), 'snapshot_metadata_sha256': job['_snapshot_metadata_sha256'],
                    'bundle': job.get('bundle'), 'bundle_sha256': job['_bundle_sha256']}
        if job['kind'] == 'audit':
            Path(job['audit_path']).parent.mkdir(parents=True, exist_ok=True)
            Path(job['audit_path']).unlink(missing_ok=True)
            prompt = confirm_prompt(self.root, job) if job.get('confirm') else audit_prompt(self.root, job)
        else:
            proposal = Path(job['hint_source'])
            proposal.parent.mkdir(parents=True, exist_ok=True)
            old_proposal = _read(proposal)
            if old_proposal is not None and old_proposal != job['_snap']['hint']:
                dispatch = _read(proposal.with_suffix('.dispatch.json'))
                _durable_json(proposal.with_name(f'{proposal.stem}.retained-{time.time_ns()}.json'), {
                    'proposal': old_proposal.hex(),
                    'dispatch': json.loads(dispatch) if dispatch is not None else None})
            _restore(proposal, job['_snap']['hint'])
            job['_hint_dispatched'] = True
            _durable_json(proposal.with_suffix('.dispatch.json'), {
                'fn': job['fn'], 'n': job['n'], 'identity': _identity(job['_snap'])})
            source = Path(job['source'])
            source.parent.mkdir(parents=True, exist_ok=True)
            seed = job.pop('_seed', None)
            draft = _read(self.root / job['_draft_seed']) if job.get('_draft_seed') else None
            if draft:
                job['draft_reseeded'] = seed is not None   # a self-declared stub was replaced
                seed = draft
                job['draft_seeded'] = True
                self._event('draft-seeded', fn=job['fn'], n=job['n'], draft=job['_draft_seed'],
                            bytes=len(draft))
            elif job.get('_from_draft') and seed is not None:
                job['draft_seeded'] = True   # the previous rung's repair of the machine draft
            if seed is None and job['kind'] == 'translate' and not job.get('prior') and job['_snap']['c'] is not None:
                seed = job['_snap']['c']
                job['seeded'] = True
            if seed is None and job['kind'] == 'translate' and not job.get('prior'):
                seed = self._resume_seed(job['fn'])
                if seed is not None:
                    job['seeded'] = True
            _restore(source, seed)
            job['_seed_bytes'] = seed
            prompt = fix_prompt(self.root, job) if job['kind'] == 'fix' else translate_prompt(self.root, job)
        detail = self._fix_evidence_error(job, before_dispatch=True)
        if detail:
            self._invalidate_fix(job, detail)
            return False
        if job['kind'] != 'audit':
            self._register_dispatch(job)
        handle = launcher.start(job, prompt, worker_config(self.root, job))
        self.running.append((job, handle, self.clock()))
        self._event('dispatch', fn=job['fn'], kind=job['kind'], model=job['model'], n=job['n'],
                    confirm=bool(job.get('confirm')), bytes=job.get('bytes'))
        self.observe()
        self.save()  # large first dispatch must not look dead to the event monitor
        return True

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
        if job['kind'] == 'translate':
            job['_draft_seed'] = self._draft_seed(job, text)
            # The previous rung's repair of the draft is the source: the draft is not repeated.
            job['_omit_draft'] = bool(job['_draft_seed'] or job.get('_from_draft'))
        name = f"{job['fn']}-{job['n']}.md" if job['kind'] in ('audit', 'fix') else f"{job['fn']}.md"
        path = self.run_dir / 'bundle' / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(bundle_text(self.root, job, text), encoding='utf-8')
        return path

    def _draft_seed(self, job, brief):
        """Root-relative machine draft a large translation starts from, or None. The draft is
        translation input (cmre draft), never evidence; the gate and audit still decide."""
        if job.get('bytes', 0) < DRAFT_SEED_MIN_BYTES or job['_snap']['c'] is not None:
            return None
        match = re.search(r'^draft: (\S+)', brief, re.M)
        if not match:
            return None
        path = (self.root / match.group(1)).resolve()
        draft = _read(path) if path.is_relative_to(self.root.resolve()) else None
        if not draft:
            return None
        seed = job.get('_seed')
        if seed is not None:
            # A seed far smaller than the draft is a stub. In the draft lineage any stub is reseeded,
            # since the next rung is told its file holds the draft; elsewhere only a self-declared one.
            small = len(seed) * DRAFT_STUB_FRACTION < len(draft)
            stub = small and (job.get('_from_draft')
                              or candidate_disclaimer_problems(seed.decode('utf-8', errors='replace')))
            return match.group(1) if stub else None
        if job.get('_from_draft'):
            return match.group(1)
        return None if self._has_resume_candidate(job['fn']) else match.group(1)

    def _has_resume_candidate(self, fn):
        try:
            data = json.loads((self.run_dir / 'resume-candidates.json').read_text(encoding='utf-8'))
        except (OSError, ValueError):
            return False
        return isinstance(data, dict) and bool((data.get('sources') or {}).get(fn))

    def _dispatch(self):
        if self.launcher is None or self.stopping():
            return
        translators = [j for j, _, _ in self.running if j['kind'] != 'audit']
        translators.extend(job for job, _ in self.verifying if job.get('_recheck'))
        while len(translators) < self.config.workers:
            job = self._next_pending()
            if job is None:
                model, rung = self._rung_model(0)
                if model is None and not self.recheck_ready:
                    break
                item = self._next_item(recheck_only=model is None)
                if item is None:
                    if not self.supply_empty and not self.pending:
                        self.supply_empty = True
                        self.state['supply_empty_since'] = time.time()
                        self._event('supply-empty', running=len(translators),
                                    idle=self.config.workers - len(translators))
                    break
                if self.supply_empty:
                    self.supply_empty = False
                    since = self.state.pop('supply_empty_since', None)
                    self._event('supply-resumed', fn=item.fn,
                                dry_minutes=round((time.time() - since) / 60, 1) if since else None)
                hold = self.cmre.hold(item.fn) if self.config.include_held else None
                job = self._new_job('translate', item.fn, model, rung, bytes=item.bytes,
                                    module=item.module, hold=hold)
                if item.fn in self.recheck_ready:
                    if self._start_recheck(job):
                        translators.append(job)
                    continue
                previous, candidate = self.retried.pop(item.fn, None), _read(Path(job['source']))
                if previous and candidate is not None:
                    job['prior'] = {'model': (previous.get('models') or ['unknown'])[-1],
                                    'detail': previous.get('detail', '')}
                    job['_seed'] = candidate
                seed = self._resume_seed(item.fn, collect=True)
                if seed is not None:
                    # The operator must quiesce old sessions and retain hash-bound
                    # inputs before opting into collection. Neither a worker's
                    # claimed pass nor an interrupted test is gate evidence.
                    job['_snap'] = {
                        'c': _read(self.root / 'recovery/c' / f'{item.fn}.c'),
                        'hint': _read(self.root / 'recovery/hints' / f'{item.fn}.json')}
                    Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
                    Path(job['source']).write_bytes(seed)
                    job['_resume_collect'] = hashlib.sha256(seed).hexdigest()
                    self._register_dispatch(job)
                    self._event('resume-collect', fn=item.fn, n=job['n'])
                    self.save()
                    self._verify(job, False)
                    translators.append(job)
                    continue
            if self._launch(job):
                translators.append(job)
        self.state['idle_slots'] = max(0, self.config.workers - len(translators))
        self.state['queue'] = len(self.frontier or [])
        audits = [j for j, _, _ in self.running if j['kind'] == 'audit']
        while len(audits) < self.config.audit_workers and self.audit_queue and self.config.audit_model:
            index = next((i for i, fn in enumerate(self.audit_queue)
                          if not self._writer_busy(fn) and all(j['fn'] != fn for j in audits)), None)
            if index is None:
                break
            fn = self.audit_queue[index]
            prior = self.state['audits'].get(fn, {})
            if prior.get('verdict') and prior.get('identity') != _identity(self._inputs(fn)):
                self._reset_audit(fn)
                prior = self.state['audits'][fn]
            if prior.get('verdict') == 'unconfirmed':
                try:
                    _bound_audit_reviews(self.run_dir, self.root, fn, _identity(self._inputs(fn)), prior['review'])
                except (OSError, ValueError, KeyError, TypeError):
                    self._reset_audit(fn)
                    prior = self.state['audits'][fn]
            confirm = prior.get('verdict') == 'unconfirmed' and self.config.confirm_model
            model = self._audit_model(self.config.confirm_model if confirm else self.config.audit_model)
            if model is None:
                break   # every audit lane is paused or retired; retain the queue
            self.audit_queue.pop(index)
            accepted = self.state['accepted'][fn]
            if confirm:
                job = self._new_job('audit', fn, model, bytes=accepted['bytes'],
                                    hold=accepted.get('hold'), confirm=True,
                                    findings=prior.get('findings', []), first_model=prior.get('model'))
            else:
                job = self._new_job('audit', fn, model, bytes=accepted['bytes'],
                                    hold=accepted.get('hold'))
            self._launch(job)
            audits.append(job)

    # -- results
    def _record(self, job, outcome, detail):
        recheck = {'recheck': True} if job.get('_recheck') else {}
        self.state['attempts'].setdefault(job['fn'], []).append(
            {'model': job['model'], 'kind': job['kind'], 'outcome': outcome,
             'detail': detail[:600], 'at': time.time(), **recheck})
        self._event('result', fn=job['fn'], kind=job['kind'], model=job['model'], n=job['n'],
                    outcome=outcome, detail=detail[:300], **recheck)

    def _fail(self, job, detail):
        safe = self._rollback(job) if '_transaction' in job else True
        self._terminal(job, 'failed' if safe else 'conflict')
        self._record(job, 'fail', detail)
        fn = job['fn']
        if job['kind'] == 'translate':
            self.state.get('input_needed', {}).pop(fn, None)
        if job.get('_hint_evidence_needed'):
            self.state.setdefault('hint_proposals', {})[fn] = job['_hint_evidence_needed']
            self._event('hint-evidence-needed', fn=fn, n=job['n'], **job['_hint_evidence_needed'])
            self._park(fn, detail)
            return
        if not safe:
            # Do not turn identity loss into a fresh retry based on somebody
            # else's canonical work. An operator must explicitly resolve it.
            self._park(fn, 'canonical identity conflict: ' + detail)
            return
        if job['kind'] == 'fix':
            self._event('open-defect' if fn in self.state['defects'] else 'fix-failed',
                        fn=fn, detail=detail[:300])
            return
        if job['kind'] == 'adopt':   # a coordinator hand fix: the accepted source stays
            self._event('request-failed', fn=fn, kind='adopt', detail=detail[:300])
            return
        if job.get('_safeguard_blocker'):
            # A self-disclaimer, unproven operand mutation or unsupported bridge
            # shape is an evidence gap: no model rung can validate it.
            self._event('acceptance-blocked', fn=fn, n=job['n'], detail=detail[:300])
            self._park(fn, detail)
            return
        if job.get('_input_bound') is not None:
            # The candidate ran without a mismatch; the test inputs do not reach enough of the
            # function. A stronger model cannot change that, so the next rung is not spent: the
            # candidate stays in RUN/src and the supply lane owns the missing inputs.
            tested = self._transaction(job)['identity']
            self.state.setdefault('input_needed', {})[fn] = {
                'bytes': job.get('bytes', 0), 'coverage': job['_input_bound'], 'source': job['source'],
                'model': job['model'], 'at': time.time(),
                'hints_sha256': tested['hint'], 'source_sha256': tested['c']}
            self._event('input-needed', fn=fn, coverage=job['_input_bound'], bytes=job.get('bytes', 0), n=job['n'])
            self._park(fn, detail)
            return
        start = 0 if job.get('_recheck') else job['rung'] + 1
        model, rung = self._rung_model(start)
        # A rung paused by a usage limit is not exhausted: the job waits in pending for it.
        paused = model is None and any(self._limited_until(m) for m in self.config.ladder[start:]
                                       if m not in self.state['unavailable_models'])
        if (model is not None or paused) and self.stopping():
            # The ladder is not exhausted; the next run's frontier scan dispatches it again.
            self.cmre.release(self.config.owner, [fn])
        elif model is not None or paused:
            model, rung = (model, rung) if model is not None else (self.config.ladder[start], start)
            source = Path(job['source'])
            prior_source = source.read_bytes() if source.exists() else None
            nxt = self._new_job('translate', fn, model, rung, bytes=job['bytes'],
                                module=job.get('module'), hold=job.get('hold'),
                                prior={'model': job['model'], 'detail': detail})
            nxt['_seed'] = prior_source
            nxt['_from_draft'] = bool(job.get('draft_seeded'))
            self.pending.append(nxt)
            self.state['parked'].pop(fn, None)
        else:
            self._park(fn, detail)

    def _unpark(self, fn, event_name, **data):
        """Return a parked function to the queue: the next frontier scan may dispatch it again."""
        record = self.state['parked'].pop(fn, None)
        if record:
            self.retried[fn] = record   # its next job starts from the last candidate and failure
        self.seen.discard(fn)
        self.frontier, self.last_scan = None, None
        self._event(event_name, fn=fn, **data)

    def _input_arrivals(self):
        """Prioritize rechecking parked candidates when their tested input identity changes."""
        now = self.clock()
        if self.last_input_check is not None and now - self.last_input_check < self.config.rescan_seconds:
            return
        self.last_input_check = now
        ready = set()
        for fn, row in self.state.get('input_needed', {}).items():
            if fn in self.state['accepted'] or self._writer_busy(fn):
                continue
            hint = _read(self.root / 'recovery/hints' / f'{fn}.json')
            current = _identity({'hint': hint})['hint']
            if current == row.get('hints_sha256') and not row.get('retry_requested'):
                continue
            ready.add(fn)
            if fn not in self.recheck_ready:
                self._event('input-arrived', fn=fn, hints_sha256=current)
        if ready:
            self.frontier, self.last_scan = None, None
        self.recheck_ready = ready

    def _start_recheck(self, job):
        fn = job['fn']
        self.recheck_ready.discard(fn)
        row = self.state['input_needed'][fn]
        snap = self._inputs(fn)
        if _identity(snap)['hint'] == row.get('hints_sha256') and not row.get('retry_requested'):
            self._park(fn, 'hints returned to the previously tested identity')
            return False
        source = Path(job['source']).resolve()
        retained = (self.root / row['source']).resolve()
        candidate = _read(source) if retained == source and source.is_relative_to(self.run_dir.resolve()) else None
        digest = _identity({'c': candidate})['c']
        if candidate is None or row.get('source_sha256', digest) != digest:
            self.state['input_needed'].pop(fn)
            detail = 'retained candidate missing or changed; explicit retry required'
            self._event('recheck-refused', fn=fn, reason=detail)
            self._park(fn, detail)
            return False
        job.update(model=row['model'], _recheck=True, _snap=snap, expected_source_sha256=digest)
        self._event('recheck', fn=fn, n=job['n'], coverage_was=row.get('coverage'))
        self._register_dispatch(job)
        self.save()
        self._verify(job, False)
        return True

    def _park(self, fn, detail):
        self.seen.add(fn)
        self.state['parked'][fn] = {'detail': detail[:600],
                                    'models': [r['model'] for r in self.state['attempts'].get(fn, [])]}
        self.cmre.release(self.config.owner, [fn])

    def _limited(self, job, message, seconds):
        """Provider usage limit: pause the provider, requeue the job, spend no attempt."""
        detail = self._fix_evidence_error(job, before_dispatch=True)
        if detail:
            self._invalidate_fix(job, detail)
        safe = not detail and (job['kind'] == 'audit' or self._running_inputs_match(job))
        provider = _limit_key(job['model'], message)
        until = time.time() + max(60, seconds)
        self.state['limited_providers'][provider] = {'until': until, 'message': message[:300]}
        self._event('model-limited', provider=provider, model=job['model'], until=until,
                    message=message[:300], fn=job['fn'], n=job['n'])
        if not safe:
            return
        if job['kind'] == 'audit':
            self.audit_queue.insert(0, job['fn'])
            return
        self._retry_worker(job)

    def _infra(self, job, error):
        """Provider/auth failure: not the model's attempt. Retire the model, requeue the job."""
        detail = self._fix_evidence_error(job, before_dispatch=True)
        if detail:
            self._invalidate_fix(job, detail)
        safe = not detail and (job['kind'] == 'audit' or self._running_inputs_match(job))
        self.state['unavailable_models'][job['model']] = error[:300]
        self._event('model-unavailable', model=job['model'], error=error[:300], fn=job['fn'], n=job['n'])
        if not safe:
            return
        if job['kind'] == 'audit':
            if self.config.audit_model:
                self.audit_queue.insert(0, job['fn'])
            return
        self._retry_worker(job)

    def _verify(self, job, timed_out):
        if job['fn'] in self.state.get('quarantined', {}):
            raise RuntimeError('acceptance quarantined: manual/current cmre ledger review required')
        self._begin_transaction(job)
        self.verifying.append((job, self.verifier.submit(self._check, job, timed_out)))

    def _check(self, job, timed_out):
        """None when the candidate passed both gate runs, else the failure detail.

        Runs on a verifier thread: it reads the job and calls the gate; only the canonical copy
        into recovery/c/ is written here, and the main loop restores it on failure."""
        fn, source = job['fn'], Path(job['source'])
        detail = self._fix_evidence_error(job)
        if detail:
            job['_fix_evidence_invalid'] = detail
            return detail
        if job.get('_unowned_hint'):
            return 'canonical hints changed since dispatch; read-only input conflict'
        if job.get('_stalled'):
            return f'stalled: no worker output for {self.config.stall_minutes} min'
        if timed_out:
            return f'timeout after {self.config.attempt_minutes} min'
        if job.get('_hint_dispatched'):
            proposal = _read(job['hint_source'])
            if proposal != job['_snap']['hint']:
                try:
                    _validate_worker_hints(job['_snap']['hint'], proposal)
                except (ValueError, TypeError, UnicodeError) as exc:
                    return f'worker hints rejected: {exc}'
                receipt = Path(job['hint_source']).with_suffix('.proposal.json')
                _durable_json(receipt, {'fn': fn, 'n': job['n'],
                                        'dispatch_identity': _identity(job['_snap']),
                                        'proposal': proposal.hex() if proposal is not None else None,
                                        'sha256': _identity({'hint': proposal})['hint']})
                job['_hint_evidence_needed'] = {
                    'receipt': str(receipt), 'dispatch_identity': _identity(job['_snap']),
                    'sha256': _identity({'hint': proposal})['hint']}
                return 'private hint proposal requires parent validation/application at an authorized boundary'
        text_bytes = _read(source)
        if text_bytes is None:
            if job.get('_finish') == 'length':
                return 'worker hit the output cap before writing source'
            return 'worker wrote no source'
        if job.get('expected_source_sha256') and hashlib.sha256(text_bytes).hexdigest() != job['expected_source_sha256']:
            return 'queued candidate bytes changed; explicit resubmission required'
        if text_bytes == job.get('_seed_bytes'):
            return 'worker left the starting source unchanged'
        text = text_bytes.decode('utf-8', errors='replace')
        from cmre import publication
        for finding in publication.candidate_listing_findings(self.root, int(fn[3:], 16), text_bytes):
            self._event('publication-review-needed', fn=fn, sha256=hashlib.sha256(text_bytes).hexdigest(),
                        finding=finding.kind, detail=finding.detail)
        problems = mechanical_problems(fn, text)
        if problems:
            return 'mechanical: ' + '; '.join(problems)
        try:
            _validate_worker_hints(job['_snap']['hint'], _read(self.root / 'recovery/hints' / f'{fn}.json'))
        except (ValueError, TypeError, UnicodeError) as exc:
            # _collect routes this to _fail, which restores the original bytes
            # (or absence). Do not replace the dispatch snapshot with the edits.
            return f'worker hints rejected: {exc}'
        declared = candidate_disclaimer_problems(text)
        if declared and (_CANDIDATE_BLOCKER.search(text) or _CANDIDATE_MUST_NOT_ACCEPT.search(text)):
            # A gate pass is exactly what "must not be accepted" says is not enough: triage it.
            job['_safeguard_blocker'] = True
            return 'BLOCKER: ' + '; '.join(declared)
        if declared:
            # An unfinished-work statement without a BLOCKER: is a failed attempt, not an
            # evidence gap: the next rung continues from this candidate.
            return 'worker self-declared unfinished: ' + '; '.join(declared)
        report = self.cmre.test(fn, str(source))
        if report.get('status') != 'pass':
            coverage = input_bound(report)
            if coverage is not None and job['kind'] == 'translate':
                tested = {'c': _read(source), 'hint': self._inputs(fn)['hint']}
                if tested['c'] != text_bytes or _identity(tested) != self._transaction(job)['identity']:
                    return 'candidate inputs changed during verification'
                job['_input_bound'] = coverage
                return 'input-bound: ' + report_detail(report)
            return report_detail(report)
        try:
            detail = self._x87_operand_check(job, text_bytes, report)
        except Exception as exc:   # evidence creation or retention failed
            detail = f'BLOCKER: x87 operand safeguard failed closed: {exc}'
        if detail:
            job['_safeguard_blocker'] = True
            return detail
        target = self.root / 'recovery/c' / f'{fn}.c'
        data = self._transaction(job)
        if _read(target) != job['_snap']['c']:
            return 'canonical source changed since dispatch'
        if _identity({'c': text_bytes, 'hint': self._inputs(fn)['hint']}) != data['identity']:
            return 'candidate inputs changed during verification'
        detail = self._fix_evidence_error(job)
        if detail:
            job['_fix_evidence_invalid'] = detail
            return detail
        self._transaction(job, 'installing')
        _restore(target, text_bytes)
        self._transaction(job, 'installed')
        report = self.cmre.test(fn, None)
        if report.get('status') != 'pass':
            return 'canonical: ' + report_detail(report)
        path = self.run_dir / 'acceptance' / f"{job['n']:08d}-gate.json"
        _durable_json(path, report)
        self._transaction(job, canonical_gate=_acceptance_pin(path))
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
            elif job.get('_fix_evidence_invalid'):
                self._invalidate_fix(job, detail)
            else:
                self._fail(job, detail)
        self.verifying = waiting

    def _audit_result(self, job, timed_out):
        fn = job['fn']
        raw = None
        try:
            raw = _read(job['audit_path'])
            if raw is None:
                raise ValueError('no audit file')
            data = json.loads(raw)
            if not isinstance(data, dict):
                raise ValueError('audit verdict must be an object')
            verdict = data.get('verdict')
            findings = data.get('findings') or []
            boundaries = data.get('boundaries') or []
            if not isinstance(boundaries, list):
                raise ValueError('boundaries must be a list')
        except (OSError, ValueError):
            verdict, findings, boundaries = 'error', ['timeout' if timed_out else 'no audit file'], []
        if verdict not in ('ok', 'defect'):
            verdict = 'error'
        identity = _identity(job['_snap'])
        stale = identity != _identity(self._inputs(fn))
        findings_changed = job.get('_open_findings', []) != self.state['defects'].get(fn, [])
        stale = stale or findings_changed
        review = {
            'audit_path': job['audit_path'], 'bundle': job.get('bundle'),
            'bundle_sha256': job.get('_bundle_sha256'),
            'snapshot': str(self.run_dir / 'audit/snapshots' / str(job['n'])),
            'snapshot_metadata_sha256': job.get('_snapshot_metadata_sha256'),
            'verdict_sha256': _identity({'verdict': raw})['verdict']}
        invalid_artifacts = False
        try:
            _review_input_artifacts(self.run_dir, self.root, fn, identity, review)
            if verdict in ('ok', 'defect'):
                _bound_review(self.run_dir, self.root, fn, identity, review)
            if job.get('confirm'):
                _bound_audit_reviews(self.run_dir, self.root, fn, identity, job['_audit_state']['review'])
        except (OSError, ValueError, KeyError, TypeError) as exc:
            invalid_artifacts = True
            self._event('audit-evidence-invalid', fn=fn, n=job['n'], detail=str(exc)[:300])
        stale = stale or invalid_artifacts
        if job.get('confirm'):
            stale = stale or job.get('_audit_state') != self.state['audits'].get(fn, {})
        self.state.setdefault('audit_history', {}).setdefault(fn, []).append(
            {'n': job['n'], 'identity': identity, 'path': job['audit_path'],
             'outcome': 'stale' if stale else verdict})
        if stale:
            self._record(job, 'stale', json.dumps(findings))
            current_audit = self.state['audits'].get(fn, {})
            current_identity = _identity(self._inputs(fn))
            if (current_audit.get('verdict') in ('ok', 'defect')
                    and current_audit.get('identity') == current_identity
                    and current_audit.get('review', {}).get('audit_path') != job['audit_path']
                    and self.state['defects'].get(fn, []) == (
                        current_audit.get('findings', []) if current_audit['verdict'] == 'defect' else [])):
                try:
                    _bound_audit_reviews(self.run_dir, self.root, fn, current_identity, current_audit['review'])
                except (OSError, ValueError, KeyError, TypeError):
                    pass  # current evidence also needs fresh review below
                else:
                    return  # this old stale result cannot invalidate a newer bound verdict
            if invalid_artifacts or findings_changed or current_audit.get('identity') != current_identity:
                self._reset_audit(fn)
                if fn not in self.audit_queue:
                    self.audit_queue.append(fn)
            elif self._needs_audit(self.state['audits'].get(fn, {})) and fn not in self.audit_queue:
                self.audit_queue.append(fn)
            return
        audit = self.state['audits'].setdefault(fn, {'rounds': 0})
        audit['identity'] = identity
        if verdict in ('ok', 'defect'):
            audit.pop('fresh_review_required', None)
            audit.pop('unbound_findings', None)
            audit['review'] = review
        if job.get('confirm'):
            return self._confirm_result(job, audit, verdict, findings, boundaries)
        if verdict in ('ok', 'defect'):
            self._set_boundaries(fn, boundaries)   # a fresh first review replaces earlier ones
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

    def _set_boundaries(self, fn, items):
        """Unproven-consumption items: explicit unknown boundaries, never open defects."""
        boundaries = self.state.setdefault('boundaries', {})
        if items:
            boundaries[fn] = items
        else:
            boundaries.pop(fn, None)

    def _confirm_result(self, job, audit, verdict, findings, boundaries=()):
        fn = job['fn']
        if verdict in ('ok', 'defect') and boundaries:
            known = self.state.setdefault('boundaries', {}).get(fn, [])
            self._set_boundaries(fn, known + [b for b in boundaries if b not in known])
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
                                bytes=self.state['accepted'][fn]['bytes'],
                                findings=json.loads(json.dumps(findings)),
                                _origin_review=json.loads(json.dumps(self.state['audits'].get(fn, {}).get('review'))))
            fix['_seed'] = _read(self.root / 'recovery/c' / f'{fn}.c')
            fix['_finding_identity'] = _identity(self._inputs(fn))
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

    def _x87_evidence_path(self, job, digest):
        """Deterministic, hash-bound receipt location for one job's operand mutants."""
        return self.run_dir / 'x87-mutants' / f"{job['n']:08d}-{digest[:12]}" / 'summary.json'

    def _x87_operand_check(self, job, raw, baseline):
        fn = job['fn']
        hints = self._inputs(fn)['hint']
        hints_sha = _identity({'hint': hints})['hint']
        path = self._x87_evidence_path(job, hashlib.sha256(raw).hexdigest())

        def check_inputs():
            if _read(job['source']) != raw:
                return 'candidate source changed during execution (source drift)'
            if self._inputs(fn)['hint'] != hints:
                return 'canonical hints changed during execution (hints drift)'
            return None

        return operand_evidence.generate(
            path, fn, job['n'], raw, baseline, hints_sha,
            gate=lambda source: self.cmre.test(fn, str(source)),
            check_inputs=check_inputs,
            report_event=lambda status: self._event(
                'x87-mutant-evidence', fn=fn, n=job['n'], path=str(path), status=status),
        )

    def _pre_accept_blocker(self, job, data):
        """Regenerate the expected bridge evidence and verify every retained byte."""
        fn = job['fn']
        raw = _read(self.root / 'recovery/c' / f'{fn}.c')
        if raw is None:
            return 'BLOCKER: acceptance blocked: canonical source is missing'
        text = raw.decode('utf-8', errors='replace')
        declared = candidate_disclaimer_problems(text)
        if declared:
            return 'BLOCKER: ' + '; '.join(declared)
        if _X87_BRIDGE.encode() not in raw:
            return None
        try:
            text = raw.decode('utf-8')
            sites = bridge_call_sites(text)
        except (ValueError, UnicodeDecodeError) as exc:
            return f'BLOCKER: x87 operand safeguard: {exc}'
        if not sites:
            return None
        digest = hashlib.sha256(raw).hexdigest()
        path = self._x87_evidence_path(job, digest)
        try:
            return operand_evidence.verify_receipt(
                path, raw, fn=fn, job_number=job['n'], hints_sha=data['identity'].get('hint'),
                current_execution=self.cmre.current_execution_identity,
            )
        except Exception as exc:   # malformed evidence must never crash acceptance
            return f'BLOCKER: x87 operand evidence is malformed or unreadable: {path}: {exc}'

    def _write_acceptance_request(self, job, data):
        fn, number = job['fn'], job['n']
        if '_dispatch_authority' not in job:
            raise ValueError('Historical pending job has no immutable dispatch authority; explicit redispatch required')
        proof_path = self._x87_evidence_path(job, data['identity']['c'])
        gate = data.get('canonical_gate')
        if gate is None:
            # A controller upgrade can resume an installed legacy transaction.
            # Recheck its exact canonical bytes before creating new gate evidence.
            report = self.cmre.test(fn)
            if report.get('status') != 'pass':
                raise ValueError('Retained transaction no longer passes the canonical gate')
            path = self.run_dir / 'acceptance' / f'{number:08d}-gate.json'
            _durable_json(path, report)
            gate = _acceptance_pin(path)
            self._transaction(job, canonical_gate=gate)
        if _identity(self._inputs(fn)) != data['identity']:
            raise ValueError('Canonical inputs changed while preparing farm acceptance')
        request = {'schema': acceptance_request.SCHEMA, 'fn': fn, 'job': number, 'kind': job['kind'],
                   'source_sha256': data['identity']['c'], 'hint_sha256': data['identity']['hint'],
                   'dispatch': job['_dispatch_authority'], 'canonical_gate': gate,
                   'operand_proof': _acceptance_pin(proof_path) if proof_path.is_file() else None}
        path = self.run_dir / 'acceptance' / f'{number:08d}-{acceptance_request.digest_json(request)[:16]}.json'
        if not path.exists():
            _durable_json(path, request)
        elif json.loads(path.read_bytes()) != request:
            raise ValueError('Retained farm acceptance request changed')
        return path

    def _register_dispatch(self, job):
        origin = None
        if job['kind'] == 'fix':
            origin = {'review': job['_origin_review'], 'findings': job['findings']}
        elif job['kind'] == 'adopt':
            origin = {'receipt': _acceptance_pin(job['review_receipt'], job['review_receipt_sha256']),
                      'findings': job['reviewed_findings']}
        job['_dispatch_authority'] = acceptance_request.register_dispatch(
            self.run_dir, self.root, job['fn'], job['n'], job['kind'], job['_snap'], origin=origin)

    def _accept(self):
        waiting = []
        for index, job in enumerate(self.accept_queue):
            fn = job['fn']
            detail = self._fix_evidence_error(job)
            if detail:
                self._invalidate_fix(job, detail)
                continue
            hold = self.cmre.hold(fn)
            for _ in range(5):
                if not hold or hold.get('state') not in HARD_HOLDS:
                    break
                self.cmre.retry(fn, f"farm {self.config.owner}: gate pass under current harness; "
                                    f"async audit owns the held concern ({hold.get('reason', '')[:120]})")
                hold = self.cmre.hold(fn)
            # Holds/retries can take time: compare the exact tested bytes at the last
            # controller boundary before entering the serial acceptance adapter.
            data = self._transaction(job)
            detail = self._fix_evidence_error(job)
            if detail:
                self._invalidate_fix(job, detail)
                continue
            if ('reviewed_findings' in job
                    and job['reviewed_findings'] != self.state['defects'].get(fn, [])):
                self._fail(job, 'reviewed concerns changed before acceptance; fresh review required')
                continue
            if _identity(self._inputs(fn)) != data['identity']:
                self._fail(job, 'canonical identity changed before acceptance')
                continue
            try:
                blocker = self._pre_accept_blocker(job, data)
            except Exception as exc:   # malformed evidence must never certify acceptance
                blocker = f'BLOCKER: x87 operand safeguard failed closed: {exc}'
            if blocker:
                job['_safeguard_blocker'] = True
                self._fail(job, blocker)
                continue
            try:
                request = self._write_acceptance_request(job, data)
            except (OSError, ValueError, KeyError, TypeError) as exc:
                self._fail(job, f'Farm acceptance evidence unavailable: {exc}')
                continue
            self._transaction(job, 'accepting')
            self.observe(accepting=job, acceptance_queue=[*waiting, *self.accept_queue[index + 1:]])
            code, output = self.cmre.accept(fn, request=request)
            if code == 2:
                self._transaction(job, 'waiting-lease', acceptance_result={'code': code, 'output': output})
                waiting.append(job)   # another writer holds the lease; try next tick
                self.lease_waits.add(job['n'])
                continue
            if code == 3:
                self._fail(job, f'Farm acceptance refused before ledger write: {output[-600:]}')
                continue
            if code != 0:
                # An error return alone does not prove the ledger wasn't written.
                # Use the same authoritative evidence contract as restart.
                self._transaction(job, acceptance_result={'code': code, 'output': output})
                try:
                    evidence = self.cmre.acceptance_identity(fn)
                except (AttributeError, NotImplementedError) as exc:
                    self._event('acceptance-evidence-unavailable', fn=fn, detail=str(exc))
                    evidence = None
                if evidence == {'status': 'not-accepted'}:
                    self._fail(job, f'accept refused: {output[-600:]}')
                elif evidence == data['identity'] and _identity(self._inputs(fn)) == data['identity']:
                    self._transaction(job, 'accepted', acceptance_evidence=evidence)
                    self._accepted(job)
                else:
                    self._quarantine(job, 'accept error without authoritative outcome; manual/current cmre ledger review required')
                continue
            self._transaction(job, 'accepted', acceptance_result={'code': code, 'output': output})
            self._accepted(job)
        self.accept_queue = waiting
        self.lease_waits.intersection_update(job['n'] for job in waiting)

    def _accepted(self, job):
        fn = job['fn']
        self.state['parked'].pop(fn, None)
        if self.state['accepted'].get(fn, {}).get('transaction_n') != job['n']:
            self._record(job, 'pass', 'accepted')
            was_held = bool(job.get('hold')) or bool(self.state['accepted'].get(fn, {}).get('was_held'))
            self.state['accepted'][fn] = {'bytes': job['bytes'], 'model': job['model'], 'kind': job['kind'],
                                          'at': time.time(), 'was_held': was_held, 'hold': job.get('hold'),
                                          'identity': self._transaction(job)['identity'], 'transaction_n': job['n']}
            self._event('accepted', fn=fn, bytes=job['bytes'], model=job['model'], kind=job['kind'])
            self.state.get('input_needed', {}).pop(fn, None)
            self._reset_audit(fn)
            if job['kind'] == 'adopt':
                self.state['audits'][fn]['adopted'] = job.get('note', '')
                if job.get('reviewed_findings', []) == self.state['defects'].get(fn, []):
                    self.state['defects'].pop(fn, None)  # only concerns actually covered by this review
                    self.state['audits'][fn].pop('unbound_findings', None)
        self.seen.add(fn)
        if self.config.audit_workers and fn not in self.audit_queue:
            self.audit_queue.append(fn)
        # Save acceptance before marking the transaction completed. Recovery of a
        # persisted "accepted" phase can repeat bookkeeping but never acceptance.
        self.save()
        self._terminal(job, 'complete')
        self.save()

    def _has_unquarantined_requests(self):
        for path in (self.run_dir / 'requests').glob('*.json'):
            try:
                fn = json.loads(path.read_bytes())['fn']
            except (OSError, ValueError, KeyError, TypeError):
                return True  # incomplete external requests retain their existing wait behavior
            if fn not in self.state.get('quarantined', {}):
                return True
        return False

    def _requests(self):
        """Apply coordinator requests (`adopt`, `dismiss`) written into RUN/requests/."""
        for path in sorted((self.run_dir / 'requests').glob('*.json')):
            try:
                request = json.loads(path.read_text(encoding='utf-8'))
            except (OSError, ValueError):
                continue   # still being written; next tick
            fn = request.get('fn', '')
            if self._writer_busy(fn):
                continue
            if request.get('kind') == 'reaudit':
                # A fresh audit under the current rules; open concerns stay until it covers them.
                if fn in self.state['accepted'] and fn not in self.state.get('quarantined', {}):
                    self._reset_audit(fn)
                    if self.config.audit_workers and fn not in self.audit_queue:
                        self.audit_queue.append(fn)
                    self._event('reaudit', fn=fn, reason=request.get('reason', '')[:300])
                    self.save()
                path.unlink(missing_ok=True)
                continue
            if request.get('kind') == 'retry':
                if fn in self.state['parked'] and fn not in self.state.get('quarantined', {}):
                    reason = request.get('reason', '')[:300]
                    retained = self.state.get('input_needed', {}).get(fn)
                    if retained is None:
                        self._unpark(fn, 'retry', reason=reason)
                    else:
                        retained['retry_requested'] = True
                        self.last_input_check = None
                        self._event('retry', fn=fn, reason=reason)
                    self.save()
                path.unlink(missing_ok=True)
                continue
            if request.get('kind') in ('adopt', 'dismiss'):
                try:
                    snapshot, digest, reviewed_findings = read_review_receipt(
                        request['review_receipt'], self.run_dir, self.root, fn)
                    if (digest != request['review_receipt_sha256']
                            or _identity(snapshot) != request['expected_identity']
                            or _identity(self._inputs(fn)) != request['expected_identity']
                            or reviewed_findings != self.state['defects'].get(fn, [])):
                        raise ValueError('reviewed inputs changed; fresh review required')
                    if request['kind'] == 'adopt':
                        if (not request.get('source_sha256')
                                or _identity({'c': _read(request['source'])})['c'] != request['source_sha256']):
                            raise ValueError('queued proposal missing or changed; explicit resubmission required')
                except (OSError, ValueError, KeyError, TypeError) as exc:
                    rejected = self.run_dir / 'requests/rejected'
                    rejected.mkdir(parents=True, exist_ok=True)
                    path.replace(rejected / path.name)
                    self._event('request-failed', fn=fn, kind=request['kind'],
                                detail=f'fresh review required: {exc}'[:300])
                    continue
            if request.get('kind') == 'dismiss':
                audit = self.state['audits'].setdefault(fn, {'rounds': 0})
                audit.update(verdict='ok', identity=_identity(self._inputs(fn)), dismissed=request.get('reason', ''),
                             dismissed_findings=audit.get('findings', []), findings=[])
                self.state['defects'].pop(fn, None)
                self._event('dismissed', fn=fn, reason=request.get('reason', '')[:300])
                self.save()
                path.unlink(missing_ok=True)
            elif request.get('kind') == 'adopt':
                job = self._new_job('adopt', fn, 'hand', bytes=self.state['accepted'].get(fn, {}).get('bytes', 0),
                                    note=request.get('note', ''), review_receipt=request['review_receipt'],
                                    review_receipt_sha256=digest, expected_identity=request['expected_identity'],
                                    reviewed_findings=reviewed_findings,
                                    expected_source_sha256=request['source_sha256'])
                job['source'] = request['source']
                job['_request'] = str(path)
                job['_snap'] = snapshot
                self._register_dispatch(job)
                self._event('adopt', fn=fn, source=request['source'])
                self._verify(job, False)

    def tick(self):
        self._control()
        self._reap()
        self._requests()
        self._collect()
        self._accept()
        self._requests()  # admit requests unblocked by acceptance before deciding the run drained
        self._input_arrivals()
        self._lift_lane()
        self._input_lane()
        self._dispatch()
        self.save()
        self.observe()

    def _lift_lane(self):
        """Run one lift-lane pass at a time: stored captures -> expand -> apply -> cmre accept.

        Before 19 Nov these steps ran only when an expert session remembered them; 145 captured
        functions had never been expanded. New lifts enter the C frontier through a rescan."""
        out = self.run_dir / 'lift-lane' / 'last.json'
        if self.lift_proc is not None:
            code = self.lift_proc.poll()
            if code is None:
                return
            self.lift_proc, self.last_lift = None, self.clock()
            try:
                summary = json.loads(out.read_text(encoding='utf-8')) if code == 0 else None
            except (OSError, ValueError):
                summary = None
            if summary is None:
                self._event('lift-lane-done', code=code,
                            detail=f'pass exited {code} without a summary; see {self.run_dir / "lift-lane" / "lane.err"}')
                return
            self._event('lift-lane-done', code=code, processed=summary.get('processed', 0),
                        statuses=summary.get('statuses', {}), accepted=summary.get('accepted', []),
                        accepted_bytes=summary.get('accepted_bytes', 0),
                        accept_waiting=summary.get('accept_waiting', []))
            if summary.get('accepted'):
                self.frontier, self.last_scan = None, None
            return
        minutes = self.config.lift_lane_minutes or 0
        if minutes <= 0 or self.stopping():
            return
        if self.last_lift is not None and self.clock() - self.last_lift < minutes * 60:
            return
        out.parent.mkdir(parents=True, exist_ok=True)
        out.unlink(missing_ok=True)
        command = [sys.executable, str(Path(__file__).resolve().parent / 'cmre_lift_lane.py'), 'once',
                   '--jobs', '4', '--output', str(out)]
        self.lift_proc = (self.lift_launch or self._popen_lift)(command)
        self.last_lift = self.clock()
        self._event('lift-lane-start')

    def _input_lane(self):
        """Keep one input-lane process running beside the farm (ADR 0013).

        It is the farm's own supply source: model workers author entry inputs for unlifted
        functions and every pass becomes an accepted lift the C frontier picks up on its
        next rescan. On 21 Nov the farm sat supply-empty while only the coordinator did this.
        The lane reads RUN/STOP and RUN/control.json itself; the farm relaunches it if it exits."""
        if self.input_proc is not None:
            code = self.input_proc.poll()
            if code is None:
                if self.stopping():
                    self._stop_input_lane()   # the lane reads STOP files, not the farm deadline
                return
            self.input_proc = None
            self._event('input-lane-exit', code=code)
            if code == 0:   # a clean exit may follow new lifts; a refused start (lock held) does not
                self.frontier, self.last_scan = None, None
        workers = self.config.input_lane_workers or 0
        if workers <= 0 or self.stopping() or self.launcher is None or self.input_drain:
            return
        stop = self.run_dir / 'input-lane' / 'STOP'
        try:
            mine = stop.read_text(encoding='utf-8') == FARM_LANE_STOP
        except OSError:
            mine = None
        if mine is False:
            return   # someone paused the input lane by hand
        if mine:
            stop.unlink(missing_ok=True)   # the farm's own stop from an earlier deadline
        if self.last_input_launch is not None and self.clock() - self.last_input_launch < 300:
            return
        command = [sys.executable, str(Path(__file__).resolve().parent / 'cmre_input_lane.py'), 'run',
                   str(self.run_dir), '--workers', str(workers), '--ladder', ','.join(self.config.ladder),
                   '--attempt-minutes', str(self.config.attempt_minutes)]
        (self.run_dir / 'input-lane').mkdir(parents=True, exist_ok=True)
        self.input_proc = (self.input_launch or self._popen_input)(command)
        self.last_input_launch = self.clock()
        self._event('input-lane-start', workers=workers)

    def _stop_input_lane(self):
        stop = self.run_dir / 'input-lane' / 'STOP'
        if not stop.exists():
            stop.parent.mkdir(parents=True, exist_ok=True)
            stop.write_text(FARM_LANE_STOP, encoding='utf-8')
            self._event('input-lane-stop')

    def _popen_input(self, command):
        logs = self.run_dir / 'input-lane'
        return subprocess.Popen(command, cwd=self.root, stdin=subprocess.DEVNULL,
                                stdout=(logs / 'lane.out').open('ab'), stderr=(logs / 'lane.err').open('ab'),
                                creationflags=getattr(subprocess, 'CREATE_NO_WINDOW', 0))

    def _popen_lift(self, command):
        logs = self.run_dir / 'lift-lane'
        return subprocess.Popen(command, cwd=self.root, stdin=subprocess.DEVNULL,
                                stdout=(logs / 'lane.out').open('ab'), stderr=(logs / 'lane.err').open('ab'),
                                creationflags=getattr(subprocess, 'CREATE_NO_WINDOW', 0))

    def run(self, poll=10.0, verifier_slots=1):
        self.save()
        self._event('farm-start', pid=os.getpid(), workers=self.config.workers,
                    audit_workers=self.config.audit_workers, ladder=self.config.ladder,
                    continuous=self.config.continuous)
        self.monitor = {'poll_seconds': poll, 'verifier_slots': verifier_slots}
        self.observe()
        try:
            while True:
                self.tick()
                if self.drained():
                    break
                time.sleep(poll)
        finally:
            self.monitor = None
            self.cmre.release(self.config.owner, None)
            self.save()
            self._event('farm-exit', status=status_line(self.summary()))


# --- lanes board ---------------------------------------------------------------
# Every state the farm cannot leave by itself, with the lane that owns it and its next action.
# Before 19 Nov these were one "parked" count; nothing said who moved them, so they piled up.
LANES = {
    'lift': ('input lane + lift lane (automatic; farm runs scripts/cmre_input_lane.py and cmre_lift_lane.py)',
             'low-coverage: the input lane authors inputs (py -3.13 scripts/cmre_input_lane.py status RUN); '
             'captures (CAPTURE.md) add game-observed ones'),
    'input-needed': ('input lane (farm re-gates by itself when hints change)',
                     'py -3.13 scripts/cmre_farm.py inputs RUN; add evidenced hints'),
    'budget-bound': ('Adam: shared budget hints for accepted lifts',
                     'C exhausts the 200k budget; re-gate with --budget 800000 and decide on a shared budget hint'),
    'gate-mismatch': ('expert hand translation (cm-decompile skill)',
                      'cmre explain fn_X --c --case N on the retained RUN/src candidate, fix, accept'),
    'contract': ('expert hand translation (cm-decompile skill)',
                 'fix the declared return width or build/preprocessing problem, then retry RUN fn_X'),
    'safeguard-blocker': ('triage (cm-triage skill)', 'resolve the declared blocker; then retry RUN fn_X'),
    'hint-evidence': ('parent input lane', 'review RUN/hints proposal; scripts/cmre_hint_apply.py'),
    'harness-blocker': ('harness release (worktree and independent review)',
                        'Follow docs/agent/HARNESS.md for the blocker group holding most bytes'),
    'evidence-needed': ('triage (cm-triage skill): largest blocker group first',
                        'establish the named ABI/layout/consumer evidence, then cmre work retry fn_X --changed'),
    'worker-failure': ('coordinator', 'retry RUN fn_X --reason "transient worker failure"'),
    'other': ('coordinator', 'read the parked detail in RUN/state.json'),
    'open-defect': ('defect lane (FARM.md "Defect subagent")', 'fix or dismiss with evidence'),
    'acceptance-quarantined': ('coordinator (manual ledger review)',
                               'py -3.13 -m cmre work show fn_X; reconcile the retained acceptance before retry'),
    'boundary': ('none: explicit unknown (unproven callee consumption)',
                 'becomes a defect only when a consumer is shown to read the bits'),
}
_FIRST = re.compile(r'first: (\{.*?\})(?: \||$)')


def parked_lane(fn, detail, state):
    detail = detail or ''
    if fn in state.get('input_needed', {}) or detail.startswith('input-bound'):
        return 'input-needed'
    if detail.startswith('BLOCKER') or detail.startswith('acceptance blocked'):
        return 'safeguard-blocker'
    if detail.startswith('private hint proposal'):
        return 'hint-evidence'
    if re.match(r'(timeout after|stalled:|worker |mechanical:)', detail):
        return 'worker-failure'
    first = _FIRST.search(detail)
    if first and '"kind": "budget-asymmetry"' in first.group(1) and '"recovered": "budget"' in first.group(1):
        return 'budget-bound'
    if re.search(r'fail: \d+ mismatches', detail):
        return 'gate-mismatch'
    if detail.startswith('fail |'):
        return 'contract'
    return 'other'


def _lift_function_rows(lift_state):
    functions = lift_state.get('functions') if isinstance(lift_state, dict) else None
    return {fn: row for fn, row in functions.items()
            if re.fullmatch(r'fn_[0-9a-f]{8}', fn) and isinstance(row, dict)} if isinstance(functions, dict) else {}


def lanes_board(state, size_of, lift_state=None, top=10, work_of=None, *, actionable=False):
    """work_of(fn) -> the function's `cmre work` record or None. A recorded harness or evidence
    blocker owns a parked gate failure: on 19 Nov 185 of 196 parked mismatches (260 KB) carried
    one, in 81 groups, so hand translation was the wrong lane for nearly all of them."""
    board = {name: {'owner': owner, 'next': nxt, 'count': 0, 'bytes': 0, 'top': [], 'groups': {}}
             for name, (owner, nxt) in LANES.items() if actionable or name != 'acceptance-quarantined'}
    if actionable:
        board['lift']['next'] = 'For unheld work, ' + board['lift']['next']
        for name in ('input-needed', 'hint-evidence'):
            board[name]['next'] = 'Check current work holds first; ' + board[name]['next']

    def add(lane, fn, group=None, **extra):
        size = size_of(fn)
        board[lane]['count'] += 1
        board[lane]['bytes'] += size
        board[lane]['top'].append({'fn': fn, 'bytes': size, **extra})
        if group is not None:
            row = board[lane]['groups'].setdefault(group, [0, 0])
            row[0] += 1
            row[1] += size

    quarantined = state.get('quarantined', {}) if actionable else {}
    for fn, value in sorted(quarantined.items()):
        reason = value.get('reason', 'unresolved acceptance') if isinstance(value, dict) else value
        add('acceptance-quarantined', fn, first=str(reason)[:300])
    for fn, row in sorted(state.get('parked', {}).items()):
        if fn in quarantined:
            continue
        detail = row.get('detail') or ''
        first = _FIRST.search(detail)
        lane, group = parked_lane(fn, detail, state), None
        record = (work_of(fn) if work_of else None) or {}
        if (lane in ('gate-mismatch', 'contract') and isinstance(record, dict)
                and record.get('state') in ('harness-blocker', 'evidence-needed')):
            blocker = record.get('blocker')
            lane = record['state']
            group = blocker if isinstance(blocker, str) and blocker else '(unnamed)'
        add(lane, fn, group, first=first.group(1) if first else detail[:120])
    for fn in sorted(state.get('defects', {})):
        if fn not in quarantined:
            add('open-defect', fn)
    for fn in sorted(state.get('boundaries', {})):
        if fn not in quarantined:
            add('boundary', fn)
    for fn, row in sorted(_lift_function_rows(lift_state).items()):
        if row.get('status') != 'pass' and fn not in quarantined:
            add('lift', fn, first=f"{row.get('status')} coverage {row.get('coverage')}")
    for lane in board.values():
        lane['top'] = sorted(lane['top'], key=lambda r: (-r['bytes'], r['fn']))[:top]
    return board


def lanes_text(board):
    lines = []
    for name, lane in sorted(board.items(), key=lambda kv: -kv[1]['bytes']):
        if not lane['count']:
            continue
        lines.append(f"{name:<18} {lane['count']:>4} fn {lane['bytes']:>8} B  owner: {lane['owner']}")
        lines.append(f"{'':<18} next: {lane['next']}")
        groups = sorted(lane.get('groups', {}).items(), key=lambda kv: -kv[1][1])
        if groups:
            lines.append(f"{'':<18} {len(groups)} groups; largest: " + ', '.join(
                f'{name} {n} fn/{size} B' for name, (n, size) in groups[:6]))
        for row in lane['top'][:5]:
            lines.append(f"{'':<20}{row['fn']} {row['bytes']:>6} B  {str(row.get('first', ''))[:90]}")
            if hold := row.get('hold'):
                lines.append(f"{'':<22}held: {hold['blocker'] or hold['state']}; {row['work_command']}")
                if hold['retry_when']:
                    lines.append(f"{'':<22}Retry when: {hold['retry_when']}")
            for kind, path in row.get('evidence', {}).items():
                lines.append(f"{'':<22}{kind}: {path}")
    return '\n'.join(lines)


def _saved_lift_report(fn, identity):
    from cmre import difftest, env, recover

    va = int(fn[3:], 16)
    directory = env.RUNS / fn
    if not directory.is_dir():
        return None
    try:
        opts = difftest.Options.for_function(va)
        request = recover._key(identity, va, opts.__dict__, recover.LIFTED_PROFILE)
        key = json.loads((directory / f'request-{request}.json').read_bytes())['key']
        if not re.fullmatch(r'[a-f0-9]{16}', key):
            return None
        path = directory / key / 'report.json'
        report = json.loads(path.read_bytes())
        if (report.get('key'), report.get('tier'), report.get('va'), report.get('execution_identity')) == (
                key, 'lifted', f'{va:#010x}', identity):
            return path, report
    except (OSError, ValueError, KeyError, TypeError, AttributeError):
        pass
    return None


def _lift_branch(fn, coverage, saved):
    from cmre import lift

    path, report = saved
    try:
        diff = report['difftest']
        if report['status'] != 'low-coverage' or diff['coverage'] != coverage:
            return None
        target = int(diff['uncovered'][0], 16)
    except (ValueError, IndexError, KeyError, TypeError):
        return None
    decoded = lift.discover(int(fn[3:], 16))
    if decoded.problems or target not in decoded.insns:
        return None
    incoming = []
    for pc, op in sorted(decoded.insns.items()):
        if op.target == target or (pc + op.size == target and op.kind in ('plain', 'call', 'jcc')):
            incoming.append({'at': f'{pc:#010x}', 'instruction': f'{op.mnemonic} {op.op_str}'.strip()})
    inspect = incoming[0]['at'] if incoming else f'{target:#010x}'
    return {'fn': fn, 'coverage': coverage, 'uncovered': f'{target:#010x}', 'incoming': incoming[:5],
            'evidence': str(path), 'command': f'py -3.13 -m cmre disasm {inspect} --count 12'}


def _actionable_hold(fn, record, hard_holds):
    if not isinstance(record, dict):
        hold = {'state': 'unknown', 'blocker': 'invalid work record', 'retry_when': ''}
        invalid = True
    elif record.get('state') in hard_holds:
        fields = {key: record.get(key, '') for key in ('state', 'blocker', 'retry_when')}
        invalid = any(not isinstance(value, str) for value in fields.values())
        hold = {key: value[:240] if isinstance(value, str) else '' for key, value in fields.items()}
    else:
        return {}
    return {'hold': hold, 'work_command': f'py -3.13 -m cmre work show {fn}' + (' --json' if invalid else '')}


def actionable_lanes(run_dir, board, functions, lift_state, records, state):
    from cmre import capture, env, ledger, recover, work
    from cmre.guard import PUBLICATION_VISIBILITIES
    from scripts.cmre_lift_lane import MAX_ERRORS
    from scripts.cmre_session import receipt_status

    recovered = ledger.recovered()
    blocked = ledger.blocked()
    identity = recover.execution_identity()
    lift_rows = _lift_function_rows(lift_state)
    capped = {fn: {'count': result['errors'], 'detail': str(result.get('error') or '')[:240]}
              for fn, result in lift_rows.items()
              if int(fn[3:], 16) not in recovered and type(result.get('errors')) is int
              and result['errors'] >= MAX_ERRORS}
    fixture_lifts = []
    low = {}
    automatic_reports = {}
    quarantined = state.get('quarantined', {})

    hints_by_va = {}
    for path in env.HINTS_DIR.glob('fn_*.json'):
        fn = path.stem
        if re.fullmatch(r'fn_[0-9a-f]{8}', fn):
            va = int(fn[3:], 16)
            if va in functions and va not in recovered and va not in blocked and fn not in quarantined:
                hints_by_va[va] = path
    for va, path in sorted(hints_by_va.items(), key=lambda item: (-functions[item[0]].body_bytes, item[0])):
        fn = path.stem
        try:
            hints = json.loads(path.read_text(encoding='utf-8'))
        except (OSError, ValueError):
            continue
        fixtures = hints.get('fixtures') if isinstance(hints, dict) else None
        if not isinstance(fixtures, list) or not fixtures:
            continue
        if saved := _saved_lift_report(fn, identity):
            if fn in lift_rows:
                automatic_reports[fn] = saved
            else:
                _, report = saved
                diff = report.get('difftest')
                coverage = diff.get('coverage') if isinstance(diff, dict) else None
                if report.get('status') == 'low-coverage' and type(coverage) in (float, int) and 0 <= coverage < 1:
                    low[fn] = (coverage, saved)
            continue
        if len(fixture_lifts) >= 5:
            continue
        record = records.get(fn)
        fixture_lifts.append({'fn': fn, 'bytes': functions[va].body_bytes, 'fixture_count': len(fixtures),
                              'stored_captures': len(capture.names_for(va)), 'evidence': str(path),
                              'work_state': record.get('state') if isinstance(record, dict) else None,
                              'command': f'py -3.13 -m cmre test {fn} --json'})
    for fn, row in lift_rows.items():
        if fn in quarantined or int(fn[3:], 16) in recovered:
            continue
        coverage = row.get('coverage')
        if row.get('status') == 'low-coverage' and type(coverage) in (float, int) and 0 <= coverage < 1:
            low[fn] = (coverage, automatic_reports.get(fn))
    branches = []
    for fn, (coverage, saved) in sorted(low.items(), key=lambda pair: (-pair[1][0], pair[0])):
        if saved := saved or _saved_lift_report(fn, identity):
            if row := _lift_branch(fn, coverage, saved):
                branches.append(row)
        if len(branches) == 5:
            break
    for row in [*fixture_lifts, *branches]:
        fn = row['fn']
        if routed := _actionable_hold(fn, records.get(fn, {}), work.HARD_HOLDS):
            row.update(hold=routed['hold'], command=routed['work_command'])
        if fn in capped:
            row['tool_error'] = capped[fn]
            if 'hold' not in row:
                row['command'] = 'py -3.13 scripts/cmre_lift_lane.py status'
    evidence_fields = (('input_needed', 'source'), ('hint_proposals', 'receipt'))
    for name, lane in board.items():
        for item in lane['top']:
            fn = item['fn']
            if name in ('input-needed', 'hint-evidence'):
                item.update(_actionable_hold(fn, records.get(fn, {}), work.HARD_HOLDS))
            paths = {name: state.get(name, {}).get(fn, {}).get(field) for name, field in evidence_fields}
            review = state.get('audits', {}).get(fn, {}).get('review') or {}
            paths['audit'] = review.get('audit_path')
            quarantine = state.get('quarantined', {}).get(fn)
            if quarantine is not None:
                journal = quarantine.get('journal') if isinstance(quarantine, dict) else None
                paths['acceptance_record'] = journal or str(run_dir / 'state.json')
            item['evidence'] = {key: path for key, path in paths.items() if isinstance(path, str)}
    check = next(iter(sorted((env.ANALYSIS / 'cmre/sessions').glob('*-check/receipt.json'), reverse=True)), None)
    try:
        session = receipt_status(check) if check else None
    except (OSError, ValueError):
        session = None
    if session and session['exit_code'] not in (0, None):
        session['next'] = f'py -3.13 scripts/cmre_session.py status "{session["receipt"]}"'
        for step in session['steps']:
            if step['exit_code'] in (0, None):
                continue
            step['failures'] = []
            if not step.get('log'):
                continue
            try:
                with Path(step['log']).open(encoding='utf-8', errors='replace') as log:
                    for line in log:
                        if line.startswith(('FAILED ', 'ERROR ', 'BAD ')):
                            step['failures'].append(line.rstrip()[:240])
                            if len(step['failures']) == 3:
                                break
            except OSError:
                pass
    head = subprocess.run(['git', 'rev-parse', '--verify', 'HEAD'], cwd=ROOT, capture_output=True, text=True)
    commit = head.stdout.strip()
    publication = env.ANALYSIS / 'cmre/publication' / f'{commit}.json'
    try:
        receipt = json.loads(publication.read_bytes()) if re.fullmatch(r'[0-9a-f]{40}', commit) else None
    except (OSError, ValueError):
        receipt = None
    publication_status = None
    if isinstance(receipt, dict) and receipt.get('commit') == commit:
        findings = [f for f in receipt.get('findings', []) if isinstance(f, dict)]
        identity_finding = next((f for f in findings if f.get('kind') == 'accepted-mismatch'), None)
        content_finding = next((f for f in findings if f.get('kind') != 'accepted-mismatch'), None)
        destination_issues = []
        remote = receipt.get('remote')
        if receipt.get('result') == 'fail' and isinstance(remote, dict):
            if not remote.get('url'):
                destination_issues.append('push destination has no resolved URL')
            if remote.get('visibility') not in PUBLICATION_VISIBILITIES:
                destination_issues.append('push destination visibility is unknown')
        publication_status = {
            'receipt': str(publication), 'result': receipt.get('result'),
            'first_source_mismatch': identity_finding, 'first_other_finding': content_finding,
            'destination_issues': destination_issues,
            'next': 'Read docs/agent/PUBLICATION.md; resolve the scan failure and rerun before any push.'
            if receipt.get('result') == 'fail' else None,
        }
    summary = {}
    for name, lane in board.items():
        if lane['count']:
            groups = sorted(lane['groups'].items(), key=lambda item: (-item[1][1], item[0]))
            summary[name] = {**lane, 'group_count': len(groups),
                             'groups': [{'name': group, 'count': n, 'bytes': size}
                                        for group, (n, size) in groups[:6]]}
    return {'farm_state': str(run_dir / 'state.json'), 'last_session_check': str(check) if check else None,
             'session': session,
             'lift_state': str(ROOT / 'CM0102/Analysis/cmre/lift-lane/state.json'),
             'fixture_lifts': fixture_lifts, 'lift_branches': branches,
             'lift_tool_hold_count': len(capped),
             'lift_tool_holds': [{'fn': fn, **capped[fn]} for fn in sorted(capped)[:5]],
             'publication': publication_status,
            'lanes': summary}


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
        elif event['event'] in ('result', 'model-unavailable', 'model-limited', 'interrupted') and 'n' in event:
            live.pop(event['n'], None)
        elif event['event'] == 'farm-exit':
            live.clear()
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
    shell = [f'{PY} scripts/cmre_farm.py *', f'{PY} scripts/cmre_session.py *',
             'rg *', 'git status*', 'git diff*', 'git log*']
    for command in ('status', 'pipeline survey', 'work show', 'family', 'show', 'job status'):
        shell.extend((f'{PY} -m cmre {command}', f'{PY} -m cmre {command} *'))
    rules += [('shell', command, 'allow') for command in shell]
    rules += [('subagent', 'cm-defect', 'allow')]
    body = """
You coordinate CM0102 decompilation from OpenCode. Read AGENTS.md, then follow
docs/agent/FARM.md sections "Session rules" and "OpenCode coordinator" exactly. The
Python farm does all per-function dispatch, testing and acceptance; you start it once,
wait on it, act on its events, and send each open defect to one cm-defect subagent.

The farm stays up for the whole session: it idles and rescans when supply is empty and
a supervisor restarts it if it dies. Stop it only at the end. Change workers, size
bounds or models with `cmre_farm.py set RUN ...`, never with a restart. When status
shows SUPPLY EMPTY or idle slots, report the first supply-lane item to Adam in that
turn and keep waiting on the same run.

Your information comes from the output of `cmre_farm.py` status, wait and defects,
and from subagent result lines. Those outputs are complete: act on them directly and
keep each event to one short line, so your context stays small for a 10-hour session.
For defect evidence, use `py -3.13 scripts/cmre_farm.py review RUN fn_X`;
its identity-bound paths replace stable-name audit/bundle guesses. Quarantined
acceptance requires manual/current cmre ledger review, not another adoption.
The defect worker explicitly claims its actual reviewed inputs with
`review RUN fn_X --claim NAME`. Pass its immutable review_receipt on every
adopt/dismiss as --review-receipt PATH. Never mint a new claim to rebase an older
hand fix. A hint-evidence-needed event belongs to the parent input lane, which
validates and applies it while the farm runs; workers have no canonical hint-write
permission.
Direct cmre auto-grants are read-only status/survey/work-show/family/show/job-status
commands. Acceptance, retraction, work retry/block and source mutations are not
automatically allowed; do not bypass the farm's checked protocol.
Quarantine is function-local: unrelated offline requests still drain. An action
for the blocked target returns bounded JSON/exit 2, not a global queue abort.
fix-evidence-needed means the originating review or immutable fix inputs lost
their binding: preserve the concern and let fresh review reestablish evidence.
"""
    return _agent_file('CM0102 coordinator: runs the recovery farm and its defect lane (docs/agent/FARM.md)',
                       'primary', COORDINATOR_MODEL, rules, body)


def defect_agent():
    rules = [('*', '*', 'deny'), ('read', '*', 'allow'), ('grep', '*', 'allow'), ('glob', '*', 'allow'),
             ('edit', 'CM0102/Analysis/cmre/farm/*/hand/*.c', 'allow'),
             ('write', 'CM0102/Analysis/cmre/farm/*/hand/*.c', 'allow')]
    shell = [f'{PY} scripts/cmre_farm.py test *', f'{PY} scripts/cmre_farm.py review *',
             f'{PY} scripts/cmre_farm.py prepare-hand *']
    shell += [f'{PY} -m cmre {command} *' for command in WORKER_COMMANDS if command not in ('test', 'explain')]
    shell += list(READ_ONLY_SHELL)
    rules += [('shell', command, 'allow') for command in shell]
    body = """
You resolve one open CM0102 audit defect. The coordinator gives you RUN (a farm run
directory) and fn_X. Follow docs/agent/FARM.md section "Defect subagent" exactly and
finish with its one result line.
First run `py -3.13 scripts/cmre_farm.py review RUN fn_X --claim NAME` with a
new unique claim name. Read the returned immutable source/hints paths and keep
review_receipt. For status "bound", also read the returned review/first_review
listing/verdict paths.
Expected claim failures return bounded JSON and exit 2. For "conflict", preserve
the existing claim and return UNRESOLVED; never mint a new claim to rebase an old
fix. For "fresh-review-required" without a review_receipt, obtain/correct the
inputs before review; a successful legacy fresh-review claim does have a receipt.
For "fresh-review-required", old paths are unbound evidence: perform a fresh
listing review of current source/hints before deciding. For
"acceptance-quarantined", return UNRESOLVED pending manual/current cmre ledger
review. Repeat the lookup before any ADOPT/DISMISS result; if identity changed
or previously bound evidence became unbound, return UNRESOLVED rather than reuse
stale findings. A legacy/fresh-review branch requires a complete fresh review
and unchanged identity/open_findings. Explicitly verify every open_findings item
in the claim before deciding. For a confirmed defect, use
`py -3.13 scripts/cmre_farm.py prepare-hand RUN fn_X --review-receipt PATH --claim-scoped`
to materialize the immutable claim source without overwriting existing work.
Return ADOPT <source> <review_receipt> <note> or
DISMISS <review_receipt> <reason>; never replace the receipt with a new current
claim to make an old fix admissible. Never guess stable audit/bundle paths.
"""
    body += '\n' + '\n'.join(AUDIT_AUTHORITY)
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


def _review_artifact(run_dir, root, value):
    path = Path(value)
    path = (path if path.is_absolute() else Path(root) / path).resolve()
    if not path.is_relative_to(Path(run_dir).resolve()):
        raise ValueError('review artifact escapes run directory')
    return path


def _review_input_artifacts(run_dir, root, fn, identity, review):
    """Shared admission/lookup checks for the exact retained dispatch inputs."""
    if identity.get('c') is None:
        raise ValueError('review has no target C source')
    snapshot = _review_artifact(run_dir, root, review['snapshot'])
    raw = (snapshot / 'identity.json').read_bytes()
    if hashlib.sha256(raw).hexdigest() != review.get('snapshot_metadata_sha256'):
        raise ValueError('dispatch snapshot metadata changed or has no dispatch binding')
    metadata = json.loads(raw)
    if metadata['identity'] != identity or metadata['fn'] != fn:
        raise ValueError('review snapshot identity mismatch')
    if (metadata.get('bundle') != review.get('bundle')
            or metadata.get('bundle_sha256') != review.get('bundle_sha256')):
        raise ValueError('bundle identity was not bound at dispatch')
    if _identity({'c': _read(snapshot / 'source.c'), 'hint': _read(snapshot / 'hints.json')}) != identity:
        raise ValueError('review snapshot bytes changed')
    paths = {'source_snapshot': str(snapshot / 'source.c'),
             'hints_snapshot': str(snapshot / 'hints.json'), 'bundle': None}
    if 'audit_path' in review:
        paths['audit_path'] = str(_review_artifact(run_dir, root, review['audit_path']))
    if review.get('bundle'):
        bundle = _review_artifact(run_dir, root, review['bundle'])
        if _identity({'bundle': _read(bundle)})['bundle'] != review['bundle_sha256']:
            raise ValueError('review bundle changed')
        paths['bundle'] = str(bundle)
    return paths, metadata


def _bound_review(run_dir, root, fn, identity, review):
    paths, metadata = _review_input_artifacts(run_dir, root, fn, identity, review)
    raw = _read(paths['audit_path'])
    if raw is None or hashlib.sha256(raw).hexdigest() != review['verdict_sha256']:
        raise ValueError('review verdict missing or changed')
    return paths, metadata


def _bound_audit_reviews(run_dir, root, fn, identity, review):
    paths, metadata = _bound_review(run_dir, root, fn, identity, review)
    first = None
    if metadata.get('confirm'):
        first, _ = _bound_review(run_dir, root, fn, identity, metadata['audit_state']['review'])
    return paths, metadata, first


def review_paths(run_dir, root, fn):
    """Read-only, fail-closed locator for current accepted-source-bound evidence."""
    if not re.fullmatch(r'fn_[0-9a-fA-F]{8}', fn):
        raise ValueError('expected fn_XXXXXXXX')
    run_dir, root = Path(run_dir).resolve(), Path(root).resolve()
    state_bytes = (run_dir / 'state.json').read_bytes()
    state = json.loads(state_bytes)
    source = root / 'recovery/c' / f'{fn}.c'
    hints = root / 'recovery/hints' / f'{fn}.json'
    current = lambda: _identity({'c': _read(source), 'hint': _read(hints)})
    identity = current()
    result = {'fn': fn, 'identity': identity, 'source': str(source), 'hints': str(hints),
              'open_findings': state.get('defects', {}).get(fn, []),
              'status': 'fresh-review-required',
              'instruction': 'Perform a fresh listing review of current source/hints; old verdicts are unbound.',
              'unbound_legacy_evidence': [
                  str(path) for path in (run_dir / f'audit/{fn}.json',
                                         run_dir / f'audit/{fn}.confirm.json',
                                         run_dir / f'bundle/{fn}.md') if path.exists()]}
    if fn in state.get('quarantined', {}):
        result.update(status='acceptance-quarantined',
                      instruction='Preserve inputs; manual/current cmre ledger review is required.')
        return result

    audit = state.get('audits', {}).get(fn, {})
    if (state.get('accepted', {}).get(fn, {}).get('identity') != identity
            or audit.get('identity') != identity or not audit.get('review')
            or audit.get('verdict') not in ('ok', 'defect', 'unconfirmed')):
        return result
    try:
        paths, metadata, first = _bound_audit_reviews(run_dir, root, fn, identity, audit['review'])
    except (OSError, ValueError, KeyError, TypeError) as exc:
        result['reason'] = str(exc)
        return result
    if current() != identity or (run_dir / 'state.json').read_bytes() != state_bytes:
        result['reason'] = 'inputs or state changed during lookup; repeat the fresh review'
        return result
    result.update(status='bound', instruction='Use the returned immutable review paths; repeat lookup before acting.',
                  verdict=audit.get('verdict'), review=paths)
    if first is not None:
        result['first_review'] = first
    return result


def claim_review(run_dir, root, fn, name):
    """Explicitly freeze reviewed inputs; a named claim is never a mutable latest alias."""
    if not re.fullmatch(r'[A-Za-z0-9_-]{1,80}', name):
        raise ValueError('review claim name must be 1-80 letters, digits, underscores or hyphens')
    run_dir, root = Path(run_dir).resolve(), Path(root).resolve()
    view = review_paths(run_dir, root, fn)
    if view['status'] == 'acceptance-quarantined':
        return view
    snapshot = {'c': _read(view['source']), 'hint': _read(view['hints'])}
    if snapshot['c'] is None:
        raise ValueError('canonical C source is missing; obtain actual source before claiming a fresh review')
    if _identity(snapshot) != view['identity']:
        raise ValueError('inputs changed during review claim; repeat lookup')
    directory = run_dir / 'reviews' / name
    directory.mkdir(parents=True, exist_ok=False)
    for key, filename in (('c', 'source.c'), ('hint', 'hints.json')):
        if snapshot[key] is not None:
            with (directory / filename).open('xb') as stream:
                stream.write(snapshot[key])
                stream.flush()
                os.fsync(stream.fileno())
    receipt = directory / 'receipt.json'
    authority = None
    if view['status'] == 'bound':
        audit = json.loads((run_dir / 'state.json').read_bytes())['audits'][fn]
        if audit['identity'] != view['identity'] or audit['verdict'] != view['verdict']:
            raise ValueError('Audit changed while claiming review inputs')
        authority = audit['review']
    _durable_json(receipt, {'schema': 'cmre-farm-review/2', 'fn': fn,
                            'identity': view['identity'], 'review': view,
                            'verdict': view.get('verdict', 'input-claim'), 'audit_origin': authority})
    view.update(review_receipt=str(receipt), source=str(directory / 'source.c'),
                hints=str(directory / 'hints.json'))
    return view


def read_review_receipt(path, run_dir, root, fn):
    path = Path(path)
    path = (path if path.is_absolute() else Path(root) / path).resolve()
    if not path.is_relative_to(Path(run_dir).resolve() / 'reviews') or path.name != 'receipt.json':
        raise ValueError('expected an explicitly claimed RUN/reviews/NAME/receipt.json')
    raw = path.read_bytes()
    receipt = json.loads(raw)
    if (not isinstance(receipt, dict) or receipt.get('schema') != 'cmre-farm-review/2'
            or receipt.get('fn') != fn):
        raise ValueError('review receipt schema/function mismatch')
    snapshot = {'c': _read(path.parent / 'source.c'), 'hint': _read(path.parent / 'hints.json')}
    if snapshot['c'] is None or receipt.get('identity', {}).get('c') is None:
        raise ValueError('review receipt has no C source; fresh review of actual source is required')
    if _identity(snapshot) != receipt['identity']:
        raise ValueError('review snapshot changed; fresh review required')
    digest = hashlib.sha256(raw).hexdigest()
    findings = receipt.get('review', {}).get('open_findings', [])
    acceptance_request.check_origin(Path(run_dir).resolve(), Path(root).resolve(), fn,
                                    receipt['identity'], 'adopt',
                                    {'receipt': {'path': str(path), 'sha256': digest},
                                     'findings': findings})
    return snapshot, digest, findings


def review_request_data(receipt, run_dir, root, fn):
    if not receipt:
        raise ValueError('use review RUN fn_X --claim NAME, then --review-receipt PATH')
    snapshot, digest, _ = read_review_receipt(receipt, run_dir, root, fn)
    return {'review_receipt': str((Path(root) / receipt).resolve()),
            'review_receipt_sha256': digest, 'expected_identity': _identity(snapshot)}


def _quarantine_status(state, fn):
    if fn not in state.get('quarantined', {}):
        return None
    info = state['quarantined'][fn]
    reason = info.get('reason', 'unresolved acceptance') if isinstance(info, dict) else info
    return {'fn': fn, 'status': 'acceptance-quarantined', 'reason': str(reason)[:300],
            'instruction': 'Preserve this function and request; manual/current ledger review is required. '
                           'Unrelated functions remain actionable.'}


class _WindowsHandFiles:
    def __init__(self, leases):
        import ctypes
        from ctypes import wintypes as w

        class UnicodeString(ctypes.Structure):
            _fields_ = [('Length', w.USHORT), ('MaximumLength', w.USHORT), ('Buffer', w.LPWSTR)]

        class ObjectAttributes(ctypes.Structure):
            _fields_ = [('Length', w.DWORD), ('RootDirectory', w.HANDLE),
                        ('ObjectName', ctypes.POINTER(UnicodeString)), ('Attributes', w.DWORD),
                        ('SecurityDescriptor', w.LPVOID), ('SecurityQualityOfService', w.LPVOID)]

        class IoStatusBlock(ctypes.Structure):
            _fields_ = [('StatusOrPointer', w.LPVOID), ('Information', ctypes.c_size_t)]

        class RenameInformation(ctypes.Structure):
            _fields_ = [('ReplaceIfExists', ctypes.c_ubyte), ('RootDirectory', w.HANDLE),
                        ('FileNameLength', w.DWORD), ('FileName', w.WCHAR * 1)]

        kernel = ctypes.WinDLL('kernel32', use_last_error=True)
        native = ctypes.WinDLL('ntdll')
        self.open_root = kernel.CreateFileW
        self.open_root.argtypes = [w.LPCWSTR, w.DWORD, w.DWORD, w.LPVOID, w.DWORD, w.DWORD, w.HANDLE]
        self.open_root.restype = w.HANDLE
        self.close = kernel.CloseHandle
        self.close.argtypes = [w.HANDLE]
        self.close.restype = w.BOOL
        self.read_attributes = kernel.GetFileInformationByHandleEx
        self.read_attributes.argtypes = [w.HANDLE, ctypes.c_int, w.LPVOID, w.DWORD]
        self.read_attributes.restype = w.BOOL
        self.create = native.NtCreateFile
        self.create.argtypes = [ctypes.POINTER(w.HANDLE), w.DWORD, ctypes.POINTER(ObjectAttributes),
                                ctypes.POINTER(IoStatusBlock), w.LPVOID, w.DWORD, w.DWORD, w.DWORD,
                                w.DWORD, w.LPVOID, w.DWORD]
        self.create.restype = w.LONG
        self.set_info = native.NtSetInformationFile
        self.set_info.argtypes = [w.HANDLE, ctypes.POINTER(IoStatusBlock), w.LPVOID, w.DWORD, ctypes.c_int]
        self.set_info.restype = w.LONG
        self.dos_error = native.RtlNtStatusToDosError
        self.dos_error.argtypes = [w.LONG]
        self.dos_error.restype = w.DWORD
        self.ctypes, self.types = ctypes, w
        self.name_type, self.object_type, self.io_type = UnicodeString, ObjectAttributes, IoStatusBlock
        self.rename_type = RenameInformation
        self.leases = leases

    def _pin(self, handle):
        ctypes, w = self.ctypes, self.types
        self.leases.callback(self.close, handle)
        info = (w.DWORD * 2)()
        if not self.read_attributes(handle, 9, ctypes.byref(info), ctypes.sizeof(info)):
            raise ctypes.WinError(ctypes.get_last_error())
        if not info[0] & 0x10 or info[0] & 0x400:
            raise ValueError('hand directory must not be redirected')
        return handle

    def root(self, anchor):
        ctypes = self.ctypes
        handle = self.open_root(anchor, 0x81, 1, None, 3, 0x02200000, None)
        if handle == ctypes.c_void_p(-1).value:
            raise ctypes.WinError(ctypes.get_last_error())
        return self._pin(handle)

    def _open_child(self, parent, name, *, directory, create=False):
        ctypes, w = self.ctypes, self.types
        buffer = ctypes.create_unicode_buffer(name)
        length = ctypes.sizeof(buffer) - 2
        child_name = self.name_type(length, length + 2, ctypes.cast(buffer, w.LPWSTR))
        obj_case_insensitive = 0x40
        obj_dont_reparse = 0x1000
        attributes = self.object_type(ctypes.sizeof(self.object_type), parent, ctypes.pointer(child_name),
                                      obj_case_insensitive | obj_dont_reparse, None, None)
        handle, io = w.HANDLE(), self.io_type()
        access = 0x00100081 if directory else 0x00110002
        share = 3 if directory else 0
        disposition = (3 if create else 1) if directory else 2
        options = 0x00200020 | (1 if directory else 0x40)
        status = self.create(ctypes.byref(handle), access, ctypes.byref(attributes), ctypes.byref(io),
                             None, 0, share, disposition, options, None, 0)
        if status < 0:
            error = ctypes.WinError(self.dos_error(status))
            error.filename = name
            raise error
        return handle.value

    def directory(self, parent, name, *, create):
        return self._pin(self._open_child(parent, name, directory=True, create=create))

    def write(self, parent, name, content):
        import msvcrt

        ctypes = self.ctypes
        handle = self._open_child(parent, f'.prepare-{uuid4().hex}.tmp', directory=False)
        try:
            fd = msvcrt.open_osfhandle(handle, os.O_WRONLY | os.O_BINARY)
        except OSError:
            self._discard(handle)
            self.close(handle)
            raise
        published = False
        try:
            with os.fdopen(os.dup(fd), 'wb') as out:
                out.write(content)
            filename = name.encode('utf-16-le')
            offset = self.rename_type.FileName.offset
            data = ctypes.create_string_buffer(max(ctypes.sizeof(self.rename_type), offset + len(filename)))
            info = self.rename_type.from_buffer(data)
            info.RootDirectory, info.FileNameLength = parent, len(filename)
            ctypes.memmove(ctypes.addressof(data) + offset, filename, len(filename))
            io = self.io_type()
            status = self.set_info(handle, ctypes.byref(io), data, offset + len(filename), 10)
            if status < 0:
                raise ctypes.WinError(self.dos_error(status))
            published = True
        finally:
            if not published:
                self._discard(handle)
            os.close(fd)

    def _discard(self, handle):
        ctypes = self.ctypes
        io, remove = self.io_type(), ctypes.c_ubyte(1)
        self.set_info(handle, ctypes.byref(io), ctypes.byref(remove), ctypes.sizeof(remove), 13)


def _write_new_hand_source(source, content):
    with ExitStack() as leases:
        parts = source.parent.parts[1:]
        if os.name == 'nt':
            tree = _WindowsHandFiles(leases)
            directory = tree.root(source.anchor)
            for index, part in enumerate(parts):
                directory = tree.directory(directory, part, create=index == len(parts) - 1)
            tree.write(directory, source.name, content)
            return

        flags = os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW
        fd = os.open(source.anchor, flags)
        leases.callback(os.close, fd)
        parts = source.parent.parts[1:]
        for index, part in enumerate(parts):
            if index == len(parts) - 1:
                try:
                    os.mkdir(part, dir_fd=fd)
                except FileExistsError:
                    pass
            fd = os.open(part, flags, dir_fd=fd)
            leases.callback(os.close, fd)
        temporary = f'.prepare-{uuid4().hex}.tmp'
        new_file = os.open(temporary, os.O_WRONLY | os.O_CREAT | os.O_EXCL, 0o600, dir_fd=fd)
        try:
            with os.fdopen(new_file, 'wb') as out:
                out.write(content)
            os.link(temporary, source.name, src_dir_fd=fd, dst_dir_fd=fd, follow_symlinks=False)
        finally:
            os.unlink(temporary, dir_fd=fd)


def prepare_hand(run_dir, root, fn, receipt, *, claim_scoped=False):
    if not re.fullmatch(r'fn_[0-9a-f]{8}', fn):
        raise ValueError('expected a lowercase fn_XXXXXXXX')
    run_dir, root = Path(run_dir).resolve(), Path(root).resolve()
    if run_dir.parent != root / 'CM0102/Analysis/cmre/farm':
        raise ValueError('expected an existing run under CM0102/Analysis/cmre/farm')
    view = review_paths(run_dir, root, fn)
    if view['status'] == 'acceptance-quarantined':
        return view
    name = fn
    if claim_scoped:
        receipt = (root / receipt).resolve()
        claim = receipt.parent
        if claim.parent != run_dir / 'reviews' or not re.fullmatch(r'[A-Za-z0-9_-]{1,80}', claim.name):
            raise ValueError('expected a named review claim directly under RUN/reviews')
        name = f'{fn}-{claim.name}'
    snapshot, _, findings = read_review_receipt(receipt, run_dir, root, fn)
    if not findings or view['identity'] != _identity(snapshot) or view['open_findings'] != findings:
        raise ValueError('claimed inputs or open findings changed; fresh review required')
    directory = run_dir / 'hand'
    if directory.resolve() != directory or directory.is_symlink() or directory.is_junction():
        raise ValueError('hand directory must not be redirected')
    source = directory / f'{name}.c'
    _write_new_hand_source(source, snapshot['c'])
    return {'fn': fn, 'status': 'prepared', 'source': str(source)}


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
    with RunLock(run_dir):
        state = json.loads((run_dir / 'state.json').read_text(encoding='utf-8'))
        config = FarmConfig(workers=0, ladder=['-'], audit_workers=0, audit_model=None, attempt_minutes=1,
                            hours=None, min_bytes=0, max_bytes=None, include_held=False, owner=state.get('owner', ''))
        farm = Farm(run_dir, ROOT, CmreAdapter(), None, config)
        farm.tick()
        while farm.verifying or farm.accept_queue or farm._has_unquarantined_requests():
            time.sleep(5)
            farm.tick()
    return farm


def reclassify(run_dir, root=ROOT, cmre=None, threads=8):
    """Re-gate the candidates a stopped run retains for its parked functions: counts by class.

    An input-bound candidate is registered so the farm re-gates it when its hints change; one
    that passes now gets a recheck request, so the next farm tick re-gates and
    accepts it through the normal transaction. Nothing is unparked, installed or accepted here."""
    from concurrent.futures import ThreadPoolExecutor
    run_dir, root = Path(run_dir), Path(root)
    cmre = cmre or CmreAdapter(root)
    with RunLock(run_dir):
        state = json.loads((run_dir / 'state.json').read_text(encoding='utf-8'))
        known = state.setdefault('input_needed', {})
        sizes = {i.fn: i.bytes for i in cmre.frontier(FRONTIER_LIMIT, 0, None, True, state.get('owner', ''))}
        counts = {'input-bound': 0, 'passes-now': 0, 'failed': 0, 'no-candidate': 0}
        todo = []
        for fn in sorted(state['parked']):
            if fn in known:
                continue
            source = run_dir / 'src' / f'{fn}.c'
            if source.exists():
                todo.append((fn, source))
            else:
                counts['no-candidate'] += 1

        def gate(entry):
            fn, source = entry
            hint = root / 'recovery/hints' / f'{fn}.json'
            try:
                identity = _identity({'c': _read(source), 'hint': _read(hint)})
                report = cmre.test(fn, str(source))
                if identity != _identity({'c': _read(source), 'hint': _read(hint)}):
                    return {'status': 'error', 'problems': ['inputs changed during reclassification']}, None
                return report, identity
            except Exception as exc:   # one slow or broken candidate must not lose the others
                return {'status': 'error', 'problems': [str(exc)[:200]]}, None

        with ThreadPoolExecutor(max_workers=max(1, threads)) as pool:
            reports = list(pool.map(gate, todo))
        for (fn, source), (report, identity) in zip(todo, reports):
            coverage = input_bound(report)
            passes = report.get('status') == 'pass'
            if coverage is None and not passes:
                counts['failed'] += 1
                state['parked'][fn]['detail'] = report_detail(report)[:600]   # replaces an empty reason
                continue
            counts['passes-now' if passes else 'input-bound'] += 1
            models = state['parked'][fn].get('models') or ['hand']
            known[fn] = {'bytes': sizes.get(fn, 0), 'coverage': coverage, 'source': str(source),
                         'model': models[-1], 'at': time.time(),
                         'hints_sha256': identity['hint'], 'source_sha256': identity['c']}
            if passes:
                known[fn]['retry_requested'] = True
        tmp = (run_dir / 'state.json').with_suffix('.tmp')
        tmp.write_text(json.dumps(state, indent=1), encoding='utf-8')
        _replace(tmp, run_dir / 'state.json')
        with (run_dir / 'events.jsonl').open('a', encoding='utf-8') as out:
            out.write(json.dumps({'at': time.time(), 'event': 'reclassified', **counts}) + '\n')
    return counts


def _request(run_dir, fn, kind, **data):
    directory = Path(run_dir) / 'requests'
    directory.mkdir(parents=True, exist_ok=True)
    path = directory / f'{time.time_ns()}-{fn}-{kind}.json'
    if kind == 'adopt':
        # Pin the proposed output, not a fresh canonical/review ownership baseline.
        raw = Path(data['source']).read_bytes()
        proposal = directory / 'candidates' / (path.stem + '.c')
        proposal.parent.mkdir(parents=True, exist_ok=True)
        with proposal.open('xb') as stream:
            stream.write(raw)
            stream.flush()
            os.fsync(stream.fileno())
        data.update(proposed_source=str(data['source']), source=str(proposal.resolve()),
                    source_sha256=hashlib.sha256(raw).hexdigest())
    tmp = path.with_suffix('.tmp')
    tmp.write_text(json.dumps({'fn': fn, 'kind': kind, **data}), encoding='utf-8')
    _replace(tmp, path)
    return path


def input_lane_summary(run_dir):
    """Input-lane counts from RUN/input-lane/events.jsonl, or None when the lane never ran."""
    path = Path(run_dir) / 'input-lane' / 'events.jsonl'
    if not path.exists():
        return None
    events, _ = _read_events(path, 0)
    running, accepted, accepted_bytes = set(), set(), 0
    for event in events:
        name, key = event.get('event'), (event.get('fn'), event.get('n'))
        if name == 'dispatch':
            running.add(key)
        elif name in ('result', 'model-limited', 'model-unavailable'):
            running.discard(key)
        elif name in ('lane-start', 'lane-exit', 'interrupted'):
            if name == 'interrupted':
                running.discard(key)
            else:
                running.clear()   # a killed lane writes no exit; its successor's start ends its jobs
        counted = name == 'accepted' or (name == 'screen-accept' and event.get('outcome') == 'accepted')
        if counted and event.get('fn') not in accepted:
            accepted.add(event.get('fn'))
            accepted_bytes += event.get('bytes') or 0
    return {'running': len(running), 'accepted': len(accepted), 'accepted_bytes': accepted_bytes}


def status_line(s):
    models = ', '.join(f"{m} {v['passes']}/{v['attempts']}" for m, v in s['by_model'].items())
    return (f"accepted {s['accepted_functions']} fn / {s['accepted_bytes']} B in {s['hours']} h "
            f"({s['bytes_per_hour']} B/h) | running {len(s['running'])} | queue {s.get('queue', 0)} "
            f"idle {s.get('idle', 0)} | pending {s['pending']} | "
            f"parked {s['parked']} (input-needed {s.get('input_needed', 0)}) skipped {s['skipped']} | "
            f"audits ok {s['audits_ok']} defect {s['audits_defect']} "
            f"error {s.get('audit_errors', 0)} unconfirmed {s.get('audits_unconfirmed', 0)} "
            f"disputed {s.get('audits_disputed', 0)} "
            f"open {len(s['open_defects'])} boundaries {len(s.get('boundaries', []))} | models {models}"
            + ''.join(f' | UNAVAILABLE {m}: {e[:80]}' for m, e in s.get('unavailable_models', {}).items())
             + ''.join(f" | LIMITED {p} until {time.strftime('%H:%M', time.localtime(until))}"
                       for p, until in s.get('limited', {}).items())
             + (f" | QUARANTINED {','.join(sorted(s['quarantined']))}" if s.get('quarantined') else '')
            + (f" | inputs: running {s['input_lane']['running']}, accepted {s['input_lane']['accepted']} fn / "
               f"{s['input_lane']['accepted_bytes']} B" if s.get('input_lane') else '')
            + (f" | SUPPLY EMPTY {round((time.time() - s['supply_empty_since']) / 60)} min: work the supply lane"
               if s.get('supply_empty_since') and not s['stopping'] else '')
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

    def claim(self, fn, owner, hours, *, include_held):
        from cmre import coordination, work
        try:
            work.claim([int(fn[3:], 16)], owner, hours, include_held=include_held)
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

    def accept(self, fn, *, request: Path):
        code, out, err = self._cli('farm-accept', fn, '--request', str(request))
        if code != 0 and 'Writer active' in (err + out):
            code = 2
        elif code == 2:
            code = 1  # 2 is reserved by the scheduler for proven lease contention
        return code, (out + err)[-2000:]

    def current_execution_identity(self):
        """Read actual current gate inputs locally, without a recovery/inventory run."""
        from cmre import recover
        return recover.execution_identity()

    def ledger_c_sha256(self, fn):
        """The C source hash the cmre ledger accepted for fn, or None."""
        try:
            record = json.loads((self.root / 'recovery/ledger' / f'{fn}.json').read_text(encoding='utf-8'))
            c = record['c']
            if Path(c['source']).as_posix() != f'recovery/c/{fn}.c':
                return None
            return c['source_sha256']
        except (OSError, ValueError, KeyError, TypeError):
            return None

    def acceptance_identity(self, fn):
        """No controller-level ledger proof is currently exposed by this adapter.

        A complete source/hints identity proves acceptance. The distinct
        {'status': 'not-accepted'} response requires authoritative nonacceptance
        evidence; None, a mismatch, and unavailability prove neither outcome.
        Recovery must quarantine unresolved acceptance without changing inputs.
        """
        raise NotImplementedError('ledger candidate/input identity evidence unavailable')


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
            if NO_ROUTE.search(f"{data.get('type', '')} {message}"):
                return f"provider.no-route: {message}"[:300], NO_ROUTE_SECONDS
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
                    return str(data.get('message') or (data.get('data') or {}).get('message')
                               or data.get('name') or data.get('type'))
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


class RunLock:
    """OS-held per-run writer exclusion; process death releases it, never a stale PID."""

    def __init__(self, run_dir):
        self.run_dir = Path(run_dir)
        self.handle = None

    def __enter__(self):
        self.run_dir.mkdir(parents=True, exist_ok=True)
        self.handle = (self.run_dir / 'supervisor.lock').open('a+b')
        try:
            if self.handle.seek(0, os.SEEK_END) == 0:
                self.handle.write(b'0')
                self.handle.flush()
            self.handle.seek(0)
            if os.name == 'nt':
                import msvcrt
                msvcrt.locking(self.handle.fileno(), msvcrt.LK_NBLCK, 1)
            else:
                import fcntl
                fcntl.flock(self.handle, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except OSError as exc:
            self.handle.close()
            raise RuntimeError('another supervisor owns this farm run') from exc
        (self.run_dir / 'supervisor.json').write_text(json.dumps({
            'pid': os.getpid(), 'started_at': time.time(), 'run_dir': str(self.run_dir.resolve()),
        }), encoding='utf-8')
        return self

    def __exit__(self, *args):
        if os.name == 'nt':
            import msvcrt
            self.handle.seek(0)
            msvcrt.locking(self.handle.fileno(), msvcrt.LK_UNLCK, 1)
        self.handle.close()


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    sub = p.add_subparsers(dest='action', required=True)
    r = sub.add_parser('run', help='dispatch, gate and accept until the deadline or STOP')
    r.add_argument('run_dir')
    r.add_argument('--workers', type=int, default=12)
    r.add_argument('--ladder', default=','.join(DEFAULT_LADDER), help='comma-separated provider/model escalation order')
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
    r.add_argument('--exit-when-empty', action='store_true',
                   help='end the run when supply is exhausted (default: idle, rescan and take new supply)')
    r.add_argument('--rescan-minutes', type=float, default=2, help='how often an empty queue is fetched again')
    r.add_argument('--lift-lane-minutes', type=float, default=60,
                   help='run scripts/cmre_lift_lane.py (captures -> accepted lifts) this often; 0 disables')
    r.add_argument('--input-lane-workers', type=int, default=4,
                   help='model workers authoring entry inputs for unlifted functions '
                        '(scripts/cmre_input_lane.py, ADR 0013); 0 disables')
    r.add_argument('--only', help='comma-separated fn_XXXXXXXX: translate only these (trial runs)')
    st = sub.add_parser('start', help='run the farm detached under a restart supervisor; args as for run')
    st.add_argument('run_dir')
    st.add_argument('run_args', nargs=argparse.REMAINDER)
    sv = sub.add_parser('supervise', help='run the farm in this process tree and relaunch it if it dies')
    sv.add_argument('run_dir')
    sv.add_argument('run_args', nargs=argparse.REMAINDER)
    se = sub.add_parser('set', help='change a running farm without a drain or restart (applies on its next tick)')
    se.add_argument('run_dir')
    se.add_argument('--workers', type=int)
    se.add_argument('--audit-workers', type=int)
    se.add_argument('--min-bytes', type=int)
    se.add_argument('--max-bytes', type=int, help='0 removes the ceiling')
    se.add_argument('--ladder')
    se.add_argument('--audit-model')
    se.add_argument('--confirm-model', help="'' disables the second auditor")
    se.add_argument('--include-held', action=argparse.BooleanOptionalAction, default=None)
    se.add_argument('--skip-ranges')
    se.add_argument('--attempt-minutes', type=float)
    se.add_argument('--stall-minutes', type=float)
    se.add_argument('--lift-lane-minutes', type=float, help='lift-lane pass interval; 0 disables')
    se.add_argument('--input-lane-workers', type=int, help='input-lane model workers; 0 dispatches no new input jobs')
    se.add_argument('--add-hours', type=float, help='extend the deadline')
    ln = sub.add_parser('lanes', help='every state the farm cannot leave by itself: owner, bytes, next action')
    ln.add_argument('run_dir')
    ln.add_argument('--json', action='store_true')
    ln.add_argument('--actionable', action='store_true', help='include bounded evidence and fixture-only lift candidates')
    ip = sub.add_parser('inputs', help='parked functions whose candidate ran clean but needs more test inputs')
    ip.add_argument('run_dir')
    rt = sub.add_parser('retry', help='return parked functions to the running farm after something changed')
    rt.add_argument('run_dir')
    rt.add_argument('fns', nargs='+', metavar='fn_XXXXXXXX')
    rt.add_argument('--reason', required=True, help='what changed since they parked (harness release, new inputs)')
    ra = sub.add_parser('reaudit', help='audit accepted functions afresh under the current audit rules')
    ra.add_argument('run_dir')
    ra.add_argument('fns', nargs='+', metavar='fn_XXXXXXXX')
    ra.add_argument('--reason', required=True, help='which rule changed')
    w = sub.add_parser('wait', help='block until a notable event, a dead farm, or --minutes')
    w.add_argument('run_dir')
    w.add_argument('--minutes', type=float, default=60)
    ad = sub.add_parser('adopt', help='ask the running farm to gate and accept a hand-fixed source')
    ad.add_argument('run_dir')
    ad.add_argument('fn')
    ad.add_argument('--source', required=True)
    ad.add_argument('--note', default='')
    ad.add_argument('--review-receipt', help='immutable receipt created explicitly by review --claim NAME')
    di = sub.add_parser('dismiss', help='close an audit defect as a false positive, with the reason')
    di.add_argument('run_dir')
    di.add_argument('fn')
    di.add_argument('--reason', required=True)
    di.add_argument('--review-receipt', help='immutable receipt created explicitly by review --claim NAME')
    te = sub.add_parser('test', help="cmre test --source (or explain with --case) as the run's claim owner")
    te.add_argument('run_dir')
    te.add_argument('fn')
    te.add_argument('source')
    te.add_argument('--case', type=int)
    rv = sub.add_parser('review', help='locate current identity-bound audit/bundle/snapshot evidence (read-only)')
    rv.add_argument('run_dir')
    rv.add_argument('fn')
    rv.add_argument('--claim', metavar='NAME', help='explicitly create immutable RUN/reviews/NAME inputs/receipt')
    ph = sub.add_parser('prepare-hand', help='copy claimed source to a hand-work path without overwriting')
    ph.add_argument('run_dir')
    ph.add_argument('fn')
    ph.add_argument('--review-receipt', required=True)
    ph.add_argument('--claim-scoped', action='store_true', help='name the hand source using its review claim')
    sub.add_parser('install-agents', help='write the OpenCode coordinator and defect agents into .opencode/agents/')
    pr = sub.add_parser('probe', help='check every ladder/audit model answers before a run')
    pr.add_argument('--ladder', default=','.join(DEFAULT_LADDER + (DEFAULT_AUDIT_MODEL, DEFAULT_CONFIRM_MODEL)))
    pr.add_argument('--opencode', choices=('v2', 'v1'), default='v2')
    rc = sub.add_parser('reclassify', help='re-gate the retained candidates of a stopped run and register '
                                          'the input-bound and now-passing ones')
    rc.add_argument('run_dir')
    rc.add_argument('--threads', type=int, default=8)
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
    if a.action == 'supervise':
        try:
            return supervise(run_dir, a.run_args)
        except Exception:   # the detached supervisor has no console: leave its failure in the run
            import traceback
            (run_dir / 'supervisor.err').write_text(traceback.format_exc(), encoding='utf-8')
            raise
    if a.action == 'set':
        return set_control(run_dir, a)
    if a.action == 'reclassify':
        try:
            print(json.dumps(reclassify(run_dir, threads=a.threads)))
        except RuntimeError as exc:
            print(f'reclassify needs a stopped run: {exc}', file=sys.stderr)
            return 2
        return 0
    if a.action == 'lanes':
        from cmre import inventory
        functions = inventory.load().functions
        state = json.loads((run_dir / 'state.json').read_text(encoding='utf-8'))
        try:
            lift = json.loads((ROOT / 'CM0102/Analysis/cmre/lift-lane/state.json').read_text(encoding='utf-8'))
        except (OSError, ValueError):
            lift = None
        from cmre import work
        records = work.read()['functions']
        board = lanes_board(state, lambda fn: getattr(functions.get(int(fn[3:], 16)), 'body_bytes', 0), lift,
                            top=3 if a.actionable else 10, work_of=records.get, actionable=a.actionable)
        if a.actionable:
            view = actionable_lanes(run_dir, board, functions, lift, records, state)
            if a.json:
                print(json.dumps(view, indent=1))
            else:
                print(f"Farm: {view['farm_state']}\nLifts: {view['lift_state']}")
                if view['last_session_check']:
                    print(f"Last session check: {view['last_session_check']}")
                for step in (view['session'] or {}).get('steps', []):
                    if step.get('exit_code') not in (None, 0):
                        print(f"  Exit {step['exit_code']}: {' '.join(step['command'])} [{step.get('log', 'no log')}]")
                        if step.get('detail'):
                            print(f"  {step['detail']}")
                        for failure in step.get('failures', []):
                            print(f"  {failure}")
                if (view['session'] or {}).get('next'):
                    print(f"  {view['session']['next']}")
                if view['publication']:
                    status = view['publication']
                    print(f"Publication: {status['result']} [{status['receipt']}]")
                    for field in ('first_source_mismatch', 'first_other_finding'):
                        if status[field]:
                            finding = status[field]
                            print(f"  {finding['kind']}: {finding['path']} — {finding.get('detail', '')[:300]}")
                    for issue in status['destination_issues']:
                        print(f"  {issue}")
                    if status['result'] == 'fail' and not (
                            status['first_source_mismatch'] or status['first_other_finding'] or status['destination_issues']):
                        print("  Scan failed without a recorded finding; inspect the receipt and scan output.")
                    if status['next']:
                        print(f"  {status['next']}")
                visible_tool_errors = set()
                for row in [*view['fixture_lifts'], *view['lift_branches']]:
                    note = (f"({row['bytes']} B), retained fixtures" if 'fixture_count' in row
                            else f"first uncovered {row['uncovered']}")
                    print(f"{row['fn']} {note}: {row['command']} [{row['evidence']}]")
                    if hold := row.get('hold'):
                        print(f"  held: {hold['blocker'] or hold['state']}. Inspect all holds with work show.")
                        if hold['retry_when']:
                            print(f"  Retry when: {hold['retry_when']}")
                    if error := row.get('tool_error'):
                        print(f"  tool error retry cap ({error['count']} errors); last error: {error['detail']}")
                        visible_tool_errors.add(row['fn'])
                    for op in row.get('incoming', []):
                        print(f"  {op['at']} {op['instruction']}")
                for error in view['lift_tool_holds']:
                    if error['fn'] not in visible_tool_errors:
                        print(f"{error['fn']} tool error retry cap ({error['count']} errors); "
                              f"last error: {error['detail']}")
                        visible_tool_errors.add(error['fn'])
                if omitted := view['lift_tool_hold_count'] - len(visible_tool_errors):
                    print(f"  {omitted} more capped tool errors in the lift state.")
                print(lanes_text(board))
        else:
            print(json.dumps(board, indent=1) if a.json else lanes_text(board))
        return 0
    if a.action == 'reaudit':
        accepted = json.loads((run_dir / 'state.json').read_text(encoding='utf-8'))['accepted']
        for fn in a.fns:
            if fn in accepted:
                _request(run_dir, fn, 'reaudit', reason=a.reason)
                print(f'{fn}: queued for a fresh audit')
            else:
                print(f'{fn}: not accepted in this run; nothing queued')
        return 0
    if a.action == 'retry':
        parked = json.loads((run_dir / 'state.json').read_text(encoding='utf-8'))['parked']
        for fn in a.fns:
            if fn in parked:
                _request(run_dir, fn, 'retry', reason=a.reason)
                print(f'{fn}: queued; a running farm takes it on its next scan')
            else:
                print(f'{fn}: not parked in this run; nothing queued')
        return 0
    if a.action == 'inputs':
        state = json.loads((run_dir / 'state.json').read_text(encoding='utf-8'))
        rows = sorted(state.get('input_needed', {}).items(), key=lambda kv: -kv[1].get('bytes', 0))
        for fn, row in rows:
            if fn in state['parked']:
                print(f"{fn} {row.get('bytes', 0)}B coverage {row.get('coverage')} candidate {row.get('source')}")
        return 0
    if a.action == 'review':
        try:
            view = claim_review(run_dir, ROOT, a.fn, a.claim) if a.claim else review_paths(run_dir, ROOT, a.fn)
        except FileExistsError as exc:
            view = {'fn': a.fn, 'status': 'conflict', 'reason': str(exc)[:300],
                    'instruction': 'Preserve the existing claim; use a new name only for an actual new review.'}
        except (OSError, ValueError, KeyError, TypeError) as exc:
            view = {'fn': a.fn, 'status': 'fresh-review-required', 'reason': str(exc)[:300],
                    'instruction': 'Correct the missing/changed review inputs, then repeat the explicit lookup/claim.'}
            print(json.dumps(view, indent=1))
            return 2
        print(json.dumps(view, indent=1))
        return 2 if view['status'] in ('acceptance-quarantined', 'conflict') else 0
    if a.action == 'prepare-hand':
        try:
            result = prepare_hand(run_dir, ROOT, a.fn, a.review_receipt, claim_scoped=a.claim_scoped)
        except FileExistsError:
            result = {'fn': a.fn, 'status': 'conflict', 'reason': 'Preserve the existing hand work file.'}
        except (OSError, ValueError, KeyError, TypeError) as exc:
            result = {'fn': a.fn, 'status': 'fresh-review-required', 'reason': str(exc)[:300]}
        print(json.dumps(result, indent=1))
        return 0 if result['status'] == 'prepared' else 2
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
    if a.action in ('adopt', 'dismiss'):
        state = json.loads((run_dir / 'state.json').read_text(encoding='utf-8'))
        blocked = _quarantine_status(state, a.fn)
        if blocked:
            print(json.dumps(blocked, indent=1))
            return 2
    if a.action == 'adopt':
        source = Path(a.source).resolve()
        if not source.is_file():
            print(f'no such source: {source}', file=sys.stderr)
            return 2
        try:
            reviewed = review_request_data(a.review_receipt, run_dir, ROOT, a.fn)
        except (OSError, ValueError, KeyError, TypeError) as exc:
            print(f'fresh review required: {exc}', file=sys.stderr)
            return 2
        request_path = _request(run_dir, a.fn, 'adopt', source=str(source), note=a.note, **reviewed)
    if a.action == 'dismiss':
        try:
            reviewed = review_request_data(a.review_receipt, run_dir, ROOT, a.fn)
        except (OSError, ValueError, KeyError, TypeError) as exc:
            print(f'fresh review required: {exc}', file=sys.stderr)
            return 2
        request_path = _request(run_dir, a.fn, 'dismiss', reason=a.reason, **reviewed)
    if a.action in ('adopt', 'dismiss'):
        if farm_live(run_dir):
            blocked = _quarantine_status(json.loads((run_dir / 'state.json').read_bytes()), a.fn)
            if blocked:
                print(json.dumps(blocked, indent=1))
                return 2
            print(request_path)
            print('queued: the running farm applies it on its next tick')
            return 0
        applied = apply_requests_now(run_dir)
        blocked = _quarantine_status(applied.state, a.fn)
        if blocked:
            print(json.dumps(blocked, indent=1))
            return 2
        print(request_path)
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
        f = Farm(run_dir, ROOT, None, None, config, reconcile=False)
        s = f.summary()
        s['running'] = running_from_events(run_dir / 'events.jsonl')
        print(json.dumps(s, indent=1) if a.json else status_line(s))
        return 0
    config = FarmConfig(workers=a.workers, ladder=[m.strip() for m in a.ladder.split(',') if m.strip()],
                        audit_workers=a.audit_workers, audit_model=a.audit_model,
                        attempt_minutes=a.attempt_minutes, hours=a.hours or None, min_bytes=a.min_bytes,
                        max_bytes=a.max_bytes, include_held=a.include_held,
                        owner=a.owner or f'farm-{run_dir.name}', stall_minutes=a.stall_minutes,
                        confirm_model=a.confirm_model or None, skip_ranges=tuple(parse_ranges(a.skip_ranges)),
                        continuous=not a.exit_when_empty, rescan_seconds=a.rescan_minutes * 60,
                        lift_lane_minutes=a.lift_lane_minutes, input_lane_workers=a.input_lane_workers,
                        only=tuple(fn.strip() for fn in (a.only or '').split(',') if fn.strip()))
    from concurrent.futures import ThreadPoolExecutor
    with RunLock(run_dir), ThreadPoolExecutor(max_workers=max(1, a.verify_threads)) as verifier:
        (run_dir / 'STOP').unlink(missing_ok=True)
        (run_dir / 'farm.pid').write_text(f'{os.getpid()}\n', encoding='utf-8')
        f = Farm(run_dir, ROOT, CmreAdapter(),
                 make_launcher(a.opencode, ROOT, run_dir), config, verifier=verifier)
        f.run(poll=a.poll, verifier_slots=max(1, a.verify_threads))
    print(status_line(f.summary()))
    return 0


def set_control(run_dir, a):
    """Merge the given settings into RUN/control.json; the farm applies them on its next tick
    and again after any restart, so no setting change needs a drain."""
    values = {key: getattr(a, key) for key in CONTROL_KEYS if getattr(a, key, None) is not None}
    if 'ladder' in values:
        values['ladder'] = [m.strip() for m in values['ladder'].split(',') if m.strip()]
        if not values['ladder']:
            print('set: --ladder needs at least one model', file=sys.stderr)
            return 2
    if 'max_bytes' in values:
        values['max_bytes'] = values['max_bytes'] or None
    if 'confirm_model' in values:
        values['confirm_model'] = values['confirm_model'] or None
    if 'skip_ranges' in values:
        values['skip_ranges'] = parse_ranges(values['skip_ranges'])
    if not values and a.add_hours is None:
        print('set: nothing to change', file=sys.stderr)
        return 2
    run_dir = Path(run_dir)
    run_dir.mkdir(parents=True, exist_ok=True)
    path = run_dir / 'control.json'
    try:
        control = json.loads(path.read_text(encoding='utf-8'))
    except (OSError, ValueError):
        control = {}
    control.update(values)
    if a.add_hours is not None:
        control.setdefault('add_hours', []).append({'id': str(time.time_ns()), 'hours': a.add_hours})
    tmp = path.with_suffix('.tmp')
    tmp.write_text(json.dumps(control, indent=1), encoding='utf-8')
    _replace(tmp, path)
    print(json.dumps({'control': str(path), **values,
                      **({'add_hours': a.add_hours} if a.add_hours is not None else {}),
                      'applies': 'next farm tick' if farm_live(run_dir) else 'when a farm runs this directory'}))
    return 0


def _run_command(run_dir, run_args):
    return [sys.executable, '-u', str(Path(__file__).resolve()), 'run', str(run_dir), *run_args]


def _launch_farm(run_dir):
    def launch(command):
        with (run_dir / 'farm.out').open('ab') as out, (run_dir / 'farm.err').open('ab') as err:
            return subprocess.call(command, cwd=ROOT, stdin=subprocess.DEVNULL, stdout=out, stderr=err)
    return launch


def supervise(run_dir, run_args, launch=None, sleep=time.sleep, clock=time.monotonic,
              max_restarts=MAX_RESTARTS_PER_HOUR):
    """Run the farm and relaunch it when it dies before STOP or its deadline.

    A farm process vanished without a traceback five times on 14-15 Nov and each time nothing
    ran until an agent or Adam noticed (up to 47 minutes). Restart resumes from state.json; the
    transaction journals decide what an interrupted job may keep."""
    run_dir = Path(run_dir)
    run_dir.mkdir(parents=True, exist_ok=True)
    launch = launch or _launch_farm(run_dir)
    (run_dir / 'supervisor.pid').write_text(f'{os.getpid()}\n', encoding='utf-8')
    args, restarts = list(run_args), []

    def event(name, **data):
        with (run_dir / 'events.jsonl').open('a', encoding='utf-8') as out:
            out.write(json.dumps({'at': time.time(), 'event': name, **data}) + '\n')

    while True:
        code = launch(_run_command(run_dir, args))
        try:
            deadline_at = json.loads((run_dir / 'state.json').read_text(encoding='utf-8')).get('deadline_at')
        except (OSError, ValueError):
            deadline_at = None
        # The farm times its deadline on a monotonic clock: allow for skew against wall time.
        expired = deadline_at is not None and time.time() >= deadline_at - 120
        if (run_dir / 'STOP').exists() or expired or (code == 0 and '--exit-when-empty' in run_args):
            return 0
        now = clock()
        restarts = [at for at in restarts if now - at < 3600]
        if len(restarts) >= max_restarts:
            event('farm-crash-loop', code=code, restarts=len(restarts),
                  detail=f'farm exited {len(restarts) + 1} times within an hour; see farm.err')
            return 1
        restarts.append(now)
        event('farm-restart', code=code, restart=len(restarts))
        if deadline_at is not None:   # keep the original deadline, not a fresh --hours window
            args = [*run_args, '--hours', f'{max(deadline_at - time.time(), 60) / 3600:.4f}']
        sleep(RESTART_SECONDS)


def _wmi_launch(command, cwd):
    """Create the process through WMI: its parent is the WMI host, outside the caller's job
    object, so it survives the shell, agent session or OpenCode server that asked for it."""
    quoted = subprocess.list2cmdline(command).replace("'", "''")
    script = ("$s = New-CimInstance -ClassName Win32_ProcessStartup -ClientOnly -Property @{ShowWindow=[uint16]0}; "
              f"$r = Invoke-CimMethod -ClassName Win32_Process -MethodName Create -Arguments "
              f"@{{CommandLine='{quoted}'; CurrentDirectory='{str(cwd)}'; ProcessStartupInformation=$s}}; "
              "if ($r.ReturnValue -ne 0) { exit 1 }; $r.ProcessId")
    result = subprocess.run(['powershell', '-NoProfile', '-NonInteractive', '-Command', script],
                            capture_output=True, text=True, timeout=60)
    if result.returncode != 0 or not result.stdout.strip().isdigit():
        raise OSError(f'WMI process creation failed: {(result.stderr or result.stdout)[-300:]}')
    return int(result.stdout.strip())


def _release_started_settings(run_dir, run_args):
    """Drop live-control keys that this `start` sets explicitly.

    control.json is applied on every tick and would otherwise override the new command line
    with an older `set` value (21 Nov: a stale MiniMax-first ladder hid the documented
    Hy4-first one). Only `start` does this: supervised restarts reuse the same arguments,
    and a later `set` still wins over them."""
    flags = {f'--{key.replace("_", "-")}': key for key in CONTROL_KEYS}
    flags.update({f'--no-{key.replace("_", "-")}': key for key in CONTROL_KEYS})
    given = set()
    for arg in run_args:
        head = arg.split('=', 1)[0]
        if not head.startswith('--') or len(head) < 3:
            continue
        keys = {flags[head]} if head in flags else {key for flag, key in flags.items() if flag.startswith(head)}
        if len(keys) == 1:   # argparse also accepts a unique prefix (`--lad` for `--ladder`)
            given |= keys
    path = run_dir / 'control.json'
    try:
        control = json.loads(path.read_text(encoding='utf-8'))
    except (OSError, ValueError):
        return
    kept = {key: value for key, value in control.items() if key not in given}
    if kept != control:
        tmp = path.with_suffix('.tmp')
        tmp.write_text(json.dumps(kept, indent=1), encoding='utf-8')
        _replace(tmp, path)


def start(run_dir, run_args):
    """Launch the supervised farm detached from the caller, with a hidden console that its
    `py -m cmre` children share (a fully detached farm opens a console window per child).

    The farm must outlive the caller's job object. When the job forbids breakaway, a plain
    child dies silently with the shell or session that started it, so the fallback is WMI."""
    run_dir = Path(run_dir)
    run_dir.mkdir(parents=True, exist_ok=True)
    _release_started_settings(run_dir, run_args)
    command = [sys.executable, '-u', str(Path(__file__).resolve()), 'supervise', str(run_dir), *run_args]
    if os.name != 'nt':
        pid, how = subprocess.Popen(command, cwd=ROOT, stdin=subprocess.DEVNULL, stdout=subprocess.DEVNULL,
                                    stderr=subprocess.DEVNULL, start_new_session=True).pid, 'new session'
    else:
        flags = subprocess.CREATE_NO_WINDOW | subprocess.CREATE_NEW_PROCESS_GROUP
        try:
            pid, how = subprocess.Popen(command, cwd=ROOT, stdin=subprocess.DEVNULL, stdout=subprocess.DEVNULL,
                                        stderr=subprocess.DEVNULL, close_fds=True,
                                        creationflags=flags | subprocess.CREATE_BREAKAWAY_FROM_JOB).pid, 'job breakaway'
        except OSError:   # the job forbids breakaway
            pid, how = _wmi_launch(command, ROOT), 'WMI'
    print(f'farm started under supervisor pid {pid} ({how}); it restarts the farm if it dies. '
          f'Logs {run_dir}/farm.out and farm.err; farm pid in farm.pid')
    return 0


if __name__ == '__main__':
    sys.exit(main())
