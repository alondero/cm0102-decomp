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
import hashlib
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
# Adam added Terra as an ordinary farm lane on 2026-11-15 after provider quotas ran low.
TERRA_MODEL = 'openai/gpt-6.1-terra'
COMMANDCODE_FLASH_MODEL = 'commandcode/deepseek/deepseek-v4.1-flash'
# Same capability/effort tiers as Go, with independent provider availability. Not Flash Fast.
DEFAULT_LADDER = ('minimax-coding-plan/MiniMax-M3.1-Flash', 'opencode-go/deepseek-v4.1-flash#low',
                  COMMANDCODE_FLASH_MODEL + '#low', 'opencode-go/deepseek-v4.1-flash',
                  COMMANDCODE_FLASH_MODEL, TERRA_MODEL)
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
                'confirm_model', 'include_held', 'skip_ranges', 'attempt_minutes', 'stall_minutes')
SUPPLY_KEYS = ('min_bytes', 'max_bytes', 'include_held', 'skip_ranges')   # changes that reselect the frontier
RESTART_SECONDS = 30         # supervisor pause before relaunching a farm that died
MAX_RESTARTS_PER_HOUR = 5    # more than this is a crash loop: stop and wake the coordinator
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
AUDIT_AUTHORITY = (
    'The target source, hints and audit bundle MUST be the supplied immutable snapshots, never moving canonical files.',
    'Callee C is navigation only: never authority or proof for signatures, ABI or fidelity. Every ABI finding must '
    'cite original instructions at preferred-image VAs and the pinned CM0102 3.9.68 binary SHA-256 '
    '0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.',
    'A bound target review is not callee composition or runtime proof; callee reads introduce no automatic global dependency.',
)


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
    continuous: bool = False       # idle and rescan on empty supply; only STOP or the deadline ends the run
    rescan_seconds: float = 120    # how often an empty frontier is fetched again


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
_X87_BUILD_LEVELS = ('-O0', '-O2')


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


def _bridge_directive_spans(text):
    """Character spans of preprocessor directives, including backslash continuations."""
    spans, start, offset = [], None, 0
    for line in text.splitlines(keepends=True):
        stripped = line.lstrip(' \t')
        if start is not None or stripped.startswith('#'):
            if start is None:
                start = offset
            if not line.rstrip('\r\n').endswith('\\'):
                spans.append((start, offset + len(line)))
                start = None
        offset += len(line)
    if start is not None:
        spans.append((start, len(text)))
    return spans


def _bridge_matching_paren(masked, opening):
    depth = 0
    for index in range(opening, len(masked)):
        char = masked[index]
        if char == '(':
            depth += 1
        elif char == ')':
            depth -= 1
            if depth == 0:
                return index
    raise ValueError('unsupported: unterminated cm_x87_CIpow call')


def _bridge_operand_spans(masked, start, end):
    """Split a call's top-level comma separators, preserving exact source offsets."""
    spans, depth, part = [], 0, start
    for index in range(start, end):
        char = masked[index]
        if char in '([{':
            depth += 1
        elif char in ')]}':
            depth -= 1
            if depth < 0:
                raise ValueError('unsupported: unbalanced brackets in cm_x87_CIpow operands')
        elif char == ',' and depth == 0:
            spans.append((part, index))
            part = index + 1
    spans.append((part, end))
    return spans


def bridge_call_sites(text):
    """Exact per-call operand spans, or ValueError when parsing is not provable.

    This is deliberately narrower than a C parser: only a direct call with two
    observable operands is inspectable. Macro aliases, directive occurrences,
    indirect references and irregular argument lists fail closed.
    """
    masked = _NONCODE.sub(_mask_noncode, text)
    directives = _bridge_directive_spans(text)
    sites = []
    for match in re.finditer(r'\b%s\b' % re.escape(_X87_BRIDGE), masked):
        if any(lo <= match.start() < hi for lo, hi in directives):
            raise ValueError('unsupported: cm_x87_CIpow inside a preprocessor directive '
                             'cannot be mutated per call')
        opening = match.end()
        while opening < len(masked) and masked[opening].isspace():
            opening += 1
        if opening >= len(masked) or masked[opening] != '(':
            raise ValueError('unsupported: cm_x87_CIpow occurrence is not a direct function call')
        closing = _bridge_matching_paren(masked, opening)
        spans = _bridge_operand_spans(masked, opening + 1, closing)
        if len(spans) != 2:
            raise ValueError(f'unsupported: cm_x87_CIpow call has {len(spans)} operand(s), expected 2')
        operands = []
        for start, end in spans:
            while start < end and masked[start].isspace():
                start += 1
            while end > start and masked[end - 1].isspace():
                end -= 1
            if start == end:
                raise ValueError('unsupported: empty cm_x87_CIpow operand')
            if '#' in masked[start:end]:
                raise ValueError('unsupported: preprocessor token inside a cm_x87_CIpow operand')
            operands.append((start, end))
        sites.append({'index': len(sites), 'line': text.count('\n', 0, match.start()) + 1,
                      'operands': operands})
    return sites


def bridge_mutant_sources(text):
    """One mutant per call operand: base + 1.0L, then exponent + 1.0L."""
    rows = []
    for site in bridge_call_sites(text):
        for operand, (start, end) in zip(('base', 'exp'), site['operands']):
            rows.append({'index': site['index'], 'line': site['line'], 'operand': operand,
                         'source': text[:start] + f'(({text[start:end]}) + 1.0L)' + text[end:]})
    return rows


def bridge_mutant_outcome(report):
    """'killed' only for a differential witness in both ordinary builds."""
    if not isinstance(report, dict):
        return 'unresolved', 'gate-report-not-an-object'
    if report.get('problems'):
        return 'unresolved', 'gate-problems'
    builds = report.get('builds')
    if not isinstance(builds, dict) or set(builds) != set(_X87_BUILD_LEVELS):
        return 'unresolved', 'optimization-builds-missing'
    if report.get('status') == 'pass':
        return 'survivor', 'differential-mismatch-not-observed'
    if report.get('status') != 'fail':
        return 'unresolved', f"gate-status-{report.get('status')}"
    for opt in _X87_BUILD_LEVELS:
        build = builds[opt]
        if not isinstance(build, dict):
            return 'unresolved', f'{opt}-build-record-missing'
        if build.get('status') == 'build-failed':
            return 'unresolved', f'{opt}-build-failed'
        if build.get('status') != 'fail':
            return 'unresolved', f"{opt}-gate-status-{build.get('status')}"
        diff = build.get('difftest')
        if not isinstance(diff, dict):
            return 'unresolved', f'{opt}-difftest-missing'
        inconclusive = diff.get('inconclusive')
        if type(inconclusive) is not int or inconclusive < 0:
            return 'unresolved', f'{opt}-inconclusive-counter-missing'
        if inconclusive:
            return 'unresolved', f'{opt}-inconclusive-{inconclusive}'
        mismatches = diff.get('mismatches')
        if not isinstance(mismatches, list) or not mismatches:
            return 'unresolved', f'{opt}-mismatch-missing'
        if not any(isinstance(m, dict) and type(m.get('case')) is int
                   and m.get('kind') != 'budget-asymmetry' for m in mismatches):
            return 'unresolved', f'{opt}-witness-missing'
    return 'killed', 'differential-mismatch-both-optimizations'


def bridge_report_identity_problems(report, source_sha256, expected_va, *,
                                    profile=None, execution_identity=None):
    """Require the real gate report's own identity fields, not summary claims."""
    if not isinstance(report, dict):
        return ['report is not an object']
    problems = []
    if report.get('va') != expected_va:
        problems.append(f"va={report.get('va')!r}")
    if report.get('tier') != 'c':
        problems.append(f"tier={report.get('tier')!r}")
    report_profile = report.get('profile')
    if not isinstance(report_profile, str) or not report_profile:
        problems.append('profile missing')
    elif profile is not None and report_profile != profile:
        problems.append(f'profile={report_profile!r} differs from baseline')
    if report.get('source_sha256') != source_sha256:
        problems.append('source_sha256 differs from the retained source')
    identity = report.get('execution_identity')
    if not isinstance(identity, dict) or not identity:
        problems.append('execution_identity missing')
    elif execution_identity is not None and identity != execution_identity:
        problems.append('execution_identity differs from baseline')
    key = report.get('key')
    if not isinstance(key, str) or not re.fullmatch(r'[0-9a-f]{16}', key):
        problems.append('gate key missing')
    return problems


def valid_execution_identity(identity):
    """Recognize the live recover.execution_identity contract, not a receipt claim."""
    def text(value):
        return isinstance(value, str) and bool(value)

    def sha(value):
        return isinstance(value, str) and re.fullmatch(r'[0-9a-f]{64}', value) is not None

    if not isinstance(identity, dict):
        return False
    if not all(sha(identity.get(key)) for key in ('target', 'harness')):
        return False
    if not text(identity.get('python')):
        return False
    for key in ('inventory', 'headers'):
        values = identity.get(key)
        if (not isinstance(values, dict) or not values
                or not all(text(name) and sha(value) for name, value in values.items())):
            return False
    dependencies = identity.get('dependencies')
    if (not isinstance(dependencies, dict)
            or not all(text(dependencies.get(name)) for name in ('capstone', 'unicorn', 'pefile'))):
        return False
    toolchain = identity.get('toolchain')
    if not isinstance(toolchain, dict):
        return False
    for name in ('gcc', 'as', 'ld', 'nm'):
        tool = toolchain.get(name)
        if (not isinstance(tool, dict) or not text(tool.get('version'))
                or not sha(tool.get('sha256'))):
            return False
    return True


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


def _hint_steps(root, job):
    proposal = job.get('hint_source', str(Path(job['source']).with_suffix('.hints.json')))
    return [f'Private c_hints proposals go ONLY in {_rel(root, proposal)} (initialized from dispatch hints).',
            'Canonical recovery/hints is read-only. Tests use current canonical hints, not your proposal.',
            'If your proposal changes, finish FARM-RESULT: evidence-needed <why>; the controller retains it '
            'for parent validation/application at an authorized stopped boundary. A proposal is not gate evidence.']


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
        'Write a first complete version within your first few steps and test it early; iterate from test output.',
        '',
        *_bundle_steps(job, 'the draft, the C.md rules, the worker rules'),
        f'2. Write {src} from the draft or the starting source below; check every statement against the listing.',
        f'3. {PY} -m cmre test {fn} --c --source {src}',
        f'4. On a mismatch: {PY} -m cmre explain {fn} --c --source {src} --case N   then fix the first difference.',
        '   Stop after 5 failed tests. Build failures count.',
        '',
        'Finish with exactly one line: FARM-RESULT: pass | FARM-RESULT: fail <first remaining mismatch> | '
        'FARM-RESULT: evidence-needed <private hint proposal needs parent>',
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
        'each with "line", "detail" and "evidence"]}. "ok" when none is confirmed.',
        '',
        'Findings to verify:', findings, '',
        'Finish with exactly one line: FARM-AUDIT: ok | FARM-AUDIT: defect <count>',
    ]) + '\n'


# --- the scheduler ---------------------------------------------------------------

def _read(path):
    path = Path(path)
    return path.read_bytes() if path.exists() else None


def _durable_json(path, data):
    """Replace one journal record only after its complete contents reach the OS."""
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix('.tmp')
    with tmp.open('w', encoding='utf-8') as stream:
        json.dump(data, stream, indent=1)
        stream.flush()
        os.fsync(stream.fileno())
    tmp.replace(path)


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
        tmp.replace(path)


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


class Farm:
    def __init__(self, run_dir, root, cmre, launcher, config, clock=time.monotonic, verifier=None, *, reconcile=True):
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
        self.last_scan = None      # clock() of the last frontier fetch
        self.last_input_check = None
        self.retried = {}          # fn -> parked record of a function returned to the queue
        self.state['deadline_at'] = None if config.hours is None else time.time() + config.hours * 3600
        self.seen = set(self.state['accepted']) | set(self.state['parked']) | set(self.state['skipped'])
        self.counter = sum(len(v) for v in self.state['attempts'].values())
        events, _ = _read_events(self.run_dir / 'events.jsonl', 0)
        self.counter = max([self.counter, *(e.get('n', 0) for e in events)])
        for kind in ('audit', 'fix'):
            retained = [int(path.name) for path in (self.run_dir / kind / 'snapshots').glob('*')
                        if path.is_dir() and path.name.isdigit()]
            self.counter = max([self.counter, *retained])
        if reconcile:
            self._reconcile_transactions()
        changed_audits = False
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
            again = {k: v for k, v in job.items() if k not in ('_snap', '_seed_bytes')}
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
        tmp.replace(self.state_path)

    def stopping(self):
        return (self.run_dir / 'STOP').exists() or (self.deadline is not None and self.clock() >= self.deadline)

    def finished(self):
        return self.stopping() and not self.running and not self.accept_queue and not self.verifying

    def drained(self):
        """The run is over. A continuous farm ends only on STOP or its deadline: with no supply
        it idles and rescans, so work that arrives later starts without a restart."""
        if self.finished():
            return True
        if self.config.continuous:
            return False
        supply_done = self.frontier == [] or self.config.workers == 0
        return (not self.running and not self.pending and not self.accept_queue
                and not self.verifying and not self.audit_queue and supply_done)

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
        if 'include_held' in changes and hasattr(self.cmre, 'include_held'):
            self.cmre.include_held = self.config.include_held   # the adapter claims with it
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
                 'quarantined': dict(self.state.get('quarantined', {})),
                'unavailable_models': dict(self.state['unavailable_models']),
                'limited': {p: v['until'] for p, v in self.state.get('limited_providers', {}).items()
                            if v['until'] > time.time()},
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

    def _next_item(self):
        if self._scan_due():
            self.last_scan = self.clock()
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
                self.cmre.claim(item.fn, self.config.owner, max(self.config.hours or 24, 1))
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
        handle = self.launcher.start(job, prompt, worker_config(self.root, job))
        self.running.append((job, handle, self.clock()))
        self._event('dispatch', fn=job['fn'], kind=job['kind'], model=job['model'], n=job['n'])
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
        name = f"{job['fn']}-{job['n']}.md" if job['kind'] in ('audit', 'fix') else f"{job['fn']}.md"
        path = self.run_dir / 'bundle' / name
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
                model, rung = self._rung_model(0)
                job = self._new_job('translate', item.fn, model, rung, bytes=item.bytes,
                                    module=item.module, hold=hold)
                retained = self.state.get('input_needed', {}).get(item.fn)
                if retained and Path(retained['source']) == Path(job['source']) and Path(job['source']).exists():
                    # Its inputs changed since it parked: gate the candidate the farm already
                    # paid for before spending another model attempt on the function.
                    job.update(model=retained['model'], _recheck=True)
                    job['_snap'] = {
                        'c': _read(self.root / 'recovery/c' / f'{item.fn}.c'),
                        'hint': _read(self.root / 'recovery/hints' / f'{item.fn}.json')}
                    self._event('recheck', fn=item.fn, n=job['n'], coverage_was=retained.get('coverage'))
                    self.save()
                    self._verify(job, False)
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
        self.state['attempts'].setdefault(job['fn'], []).append(
            {'model': job['model'], 'kind': job['kind'], 'outcome': outcome, 'detail': detail[:600], 'at': time.time()})
        self._event('result', fn=job['fn'], kind=job['kind'], model=job['model'], n=job['n'],
                    outcome=outcome, detail=detail[:300])

    def _fail(self, job, detail):
        safe = self._rollback(job) if '_transaction' in job else True
        self._terminal(job, 'failed' if safe else 'conflict')
        self._record(job, 'fail', detail)
        fn = job['fn']
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
            self.state.setdefault('input_needed', {})[fn] = {
                'bytes': job.get('bytes', 0), 'coverage': job['_input_bound'], 'source': job['source'],
                'model': job['model'], 'at': time.time(),
                'hints_sha256': _identity(self._inputs(fn))['hint']}
            self._event('input-needed', fn=fn, coverage=job['_input_bound'], bytes=job.get('bytes', 0), n=job['n'])
            self._park(fn, detail)
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

    def _unpark(self, fn, event_name, **data):
        """Return a parked function to the queue: the next frontier scan may dispatch it again."""
        record = self.state['parked'].pop(fn, None)
        if record:
            self.retried[fn] = record   # its next job starts from the last candidate and failure
        self.seen.discard(fn)
        self.frontier, self.last_scan = None, None
        self._event(event_name, fn=fn, **data)

    def _input_arrivals(self):
        """A parked candidate that failed on inputs alone goes back in the queue when its canonical
        hints change, so supply work reaches acceptance without an operator or a restart."""
        now = self.clock()
        if self.last_input_check is not None and now - self.last_input_check < self.config.rescan_seconds:
            return
        self.last_input_check = now
        for fn, row in self.state.get('input_needed', {}).items():
            if fn not in self.state['parked']:
                continue
            current = _identity(self._inputs(fn))['hint']
            if current != row.get('hints_sha256'):
                self._unpark(fn, 'input-arrived', hints_sha256=current)

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
        provider = job['model'].split('/', 1)[0]
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
        if declared:
            job['_safeguard_blocker'] = True
            return 'BLOCKER: ' + '; '.join(declared)
        report = self.cmre.test(fn, str(source))
        if report.get('status') != 'pass':
            coverage = input_bound(report)
            if coverage is not None and job['kind'] == 'translate':
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
        except (OSError, ValueError):
            verdict, findings = 'error', ['timeout' if timed_out else 'no audit file']
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

    def _x87_operand_check(self, job, text_bytes, baseline):
        """ADR 0010 condition 3 before any canonical install or acceptance.

        Each cm_x87_CIpow call is copied twice, once with +1.0L on the base and
        once on the exponent. Every mutant must show a differential mismatch at
        both ordinary builds. Reports and sources are bound by SHA-256; exact
        decoded raw bytes drive generation so CRLF is preserved. Survivors,
        build failures, inconclusive runs, unbound reports and source/hint drift
        are BLOCKERs, and all received evidence stays under RUN/x87-mutants/."""
        fn = job['fn']
        if _X87_BRIDGE.encode() not in text_bytes:
            return None
        encoded = text_bytes
        digest = hashlib.sha256(encoded).hexdigest()
        summary_path = self._x87_evidence_path(job, digest)
        directory = summary_path.parent
        directory.mkdir(parents=True, exist_ok=True)
        hints_sha = _identity({'hint': self._inputs(fn)['hint']})['hint']
        summary = {'schema': 'cmre-farm-x87-operands/1', 'fn': fn, 'job': job['n'],
                   'candidate_sha256': digest, 'hints_sha256': hints_sha,
                   'baseline': None, 'mutants': [], 'status': 'running'}

        def retained(detail):
            summary.update(status='blocked', detail=detail)
            _durable_json(summary_path, summary)
            self._event('x87-mutant-evidence', fn=fn, n=job['n'], path=str(summary_path),
                        status=summary['status'])
            return detail

        try:
            text = encoded.decode('utf-8')
        except UnicodeDecodeError as exc:
            return retained(f'BLOCKER: x87 operand safeguard: source is not UTF-8: {exc}; '
                            f'evidence {summary_path}')
        try:
            mutants = bridge_mutant_sources(text)
        except ValueError as exc:
            return retained(f'BLOCKER: x87 operand safeguard: {exc}; evidence {summary_path}')
        if not mutants:
            return None
        expected_va = '0x' + fn[3:]
        baseline_problems = bridge_report_identity_problems(baseline, digest, expected_va)
        if not isinstance(baseline, dict):
            baseline_problems.append('baseline report is not an object')
        else:
            if baseline.get('status') != 'pass':
                baseline_problems.append(f"baseline status={baseline.get('status')!r}")
            if baseline.get('problems'):
                baseline_problems.append('baseline gate problems')
        if baseline_problems:
            return retained('BLOCKER: x87 operand safeguard: baseline report is not bound: '
                            + '; '.join(baseline_problems) + f'; evidence {summary_path}')
        profile, execution = baseline.get('profile'), baseline.get('execution_identity')
        baseline_source = directory / 'baseline.c'
        baseline_report = directory / 'baseline.json'
        _restore(baseline_source, encoded)
        _durable_json(baseline_report, baseline)
        summary['baseline'] = {
            'source': baseline_source.name, 'source_sha256': digest,
            'report': baseline_report.name,
            'report_sha256': hashlib.sha256(baseline_report.read_bytes()).hexdigest(),
            'gate_key': baseline.get('key')}
        expected_source = encoded
        for row in mutants:
            stem = f"call-{row['index']:03d}-{row['operand']}"
            mutant_path = directory / f'{stem}.c'
            report_path = directory / f'{stem}.json'
            _restore(mutant_path, row['source'].encode('utf-8'))
            gate_report = {'status': 'error', 'problems': ['gate did not return a report']}
            try:
                gate_report = self.cmre.test(fn, str(mutant_path))
            except (Exception, SystemExit) as exc:
                gate_report = {'status': 'error', 'problems': [f'{type(exc).__name__}: {exc}']}
            outcome, reason = bridge_mutant_outcome(gate_report)
            source_sha = hashlib.sha256(mutant_path.read_bytes()).hexdigest()
            identity_problems = bridge_report_identity_problems(
                gate_report, source_sha, expected_va, profile=profile, execution_identity=execution)
            if identity_problems:
                outcome, reason = 'unresolved', 'report-identity: ' + '; '.join(identity_problems)
            _durable_json(report_path, gate_report)
            row.update(source=mutant_path.name, source_sha256=source_sha,
                       report=report_path.name,
                       report_sha256=hashlib.sha256(report_path.read_bytes()).hexdigest(),
                       gate_key=gate_report.get('key') if isinstance(gate_report, dict) else None,
                       outcome=outcome, reason=reason)
            summary['mutants'].append(row)
            _durable_json(summary_path, summary)
            if _read(job['source']) != expected_source:
                return retained('BLOCKER: x87 operand mutant evidence invalid: candidate source '
                                f'changed during execution (source drift); evidence {summary_path}')
            if _identity({'hint': self._inputs(fn)['hint']})['hint'] != hints_sha:
                return retained('BLOCKER: x87 operand mutant evidence invalid: canonical hints '
                                f'changed during execution (hints drift); evidence {summary_path}')
        blocked = [row for row in summary['mutants'] if row['outcome'] != 'killed']
        if blocked:
            row = blocked[0]
            return retained(
                f"BLOCKER: x87 operand mutant {row['operand']} for call {row['index'] + 1} "
                f"(line {row['line']}) {row['outcome']}: {row['reason']}; evidence {summary_path}")
        summary['status'] = 'killed'
        _durable_json(summary_path, summary)
        self._event('x87-mutant-evidence', fn=fn, n=job['n'], path=str(summary_path), status='killed')
        return None

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
            return self._retained_bridge_blocker(path, raw, text, digest, data, job)
        except Exception as exc:   # malformed evidence must never crash acceptance
            return f'BLOCKER: x87 operand evidence is malformed or unreadable: {path}: {exc}'

    def _retained_bridge_blocker(self, path, raw, text, digest, data, job):
        fn = job['fn']
        expected_va = '0x' + fn[3:]
        try:
            summary = json.loads(path.read_bytes())
        except (OSError, ValueError) as exc:
            return f'BLOCKER: x87 operand evidence missing or unreadable for acceptance: {path}: {exc}'
        if (not isinstance(summary, dict)
                or summary.get('schema') != 'cmre-farm-x87-operands/1'
                or summary.get('fn') != fn or summary.get('job') != job['n']
                or summary.get('candidate_sha256') != digest
                or summary.get('hints_sha256') != data['identity'].get('hint')
                or summary.get('status') != 'killed'):
            return f'BLOCKER: x87 operand evidence is not a killed intact receipt: {path}'
        baseline = summary.get('baseline')
        if (not isinstance(baseline, dict)
                or baseline.get('source') != 'baseline.c'
                or baseline.get('report') != 'baseline.json'):
            return f'BLOCKER: x87 operand evidence has a malformed baseline record: {path}'
        baseline_source, baseline_report = path.parent / 'baseline.c', path.parent / 'baseline.json'
        if not baseline_source.is_file() or not baseline_report.is_file():
            return f'BLOCKER: x87 operand baseline evidence is missing: {path}'
        baseline_bytes = baseline_source.read_bytes()
        baseline_sha = hashlib.sha256(baseline_bytes).hexdigest()
        if (baseline_bytes != raw or baseline_sha != digest
                or baseline.get('source_sha256') != digest):
            return f'BLOCKER: x87 operand baseline source does not match the candidate: {path}'
        report_bytes = baseline_report.read_bytes()
        if hashlib.sha256(report_bytes).hexdigest() != baseline.get('report_sha256'):
            return f'BLOCKER: x87 operand baseline report bytes changed: {path}'
        baseline_evidence = json.loads(report_bytes)
        if baseline_evidence.get('status') != 'pass' or baseline_evidence.get('problems'):
            return f'BLOCKER: x87 operand baseline report is not a passing gate: {path}'
        identity_problems = bridge_report_identity_problems(baseline_evidence, digest, expected_va)
        if identity_problems:
            return ('BLOCKER: x87 operand baseline report is not bound: '
                    + '; '.join(identity_problems) + f': {path}')
        profile, execution = baseline_evidence.get('profile'), baseline_evidence.get('execution_identity')
        try:
            current_execution = self.cmre.current_execution_identity()
        except (Exception, SystemExit) as exc:
            return (f'BLOCKER: x87 operand current execution identity unavailable: '
                    f'{type(exc).__name__}; evidence {path}')
        if not valid_execution_identity(current_execution):
            return f'BLOCKER: x87 operand current execution identity is malformed: {path}'
        if execution != current_execution:
            return f'BLOCKER: x87 operand evidence differs from current execution identity: {path}'
        expected = bridge_mutant_sources(text)
        rows = summary.get('mutants')
        if not isinstance(rows, list) or len(rows) != len(expected):
            return f'BLOCKER: x87 operand evidence has missing or extra mutant rows: {path}'
        by_key = {}
        for row in rows:
            if not isinstance(row, dict):
                return f'BLOCKER: x87 operand evidence has a malformed mutant row: {path}'
            key = (row.get('index'), row.get('operand'))
            if key in by_key:
                return f'BLOCKER: x87 operand evidence has duplicate mutant rows: {path}'
            by_key[key] = row
        if set(by_key) != {(row['index'], row['operand']) for row in expected}:
            return f'BLOCKER: x87 operand evidence does not cover every call operand: {path}'
        for expected_row in expected:
            row = by_key[(expected_row['index'], expected_row['operand'])]
            stem = f"call-{expected_row['index']:03d}-{expected_row['operand']}"
            if row.get('line') != expected_row['line']:
                return f'BLOCKER: x87 operand evidence line changed for {stem}: {path}'
            if row.get('source') != f'{stem}.c' or row.get('report') != f'{stem}.json':
                return f'BLOCKER: x87 operand evidence names changed for {stem}: {path}'
            source_path, report_path = path.parent / row['source'], path.parent / row['report']
            if not source_path.is_file() or not report_path.is_file():
                return f'BLOCKER: x87 operand evidence files are missing for {stem}: {path}'
            source_bytes = source_path.read_bytes()
            expected_bytes = expected_row['source'].encode('utf-8')
            if source_bytes != expected_bytes:
                return f'BLOCKER: x87 operand evidence source does not regenerate for {stem}: {path}'
            source_sha = hashlib.sha256(source_bytes).hexdigest()
            if row.get('source_sha256') != source_sha:
                return f'BLOCKER: x87 operand evidence source hash changed for {stem}: {path}'
            report_bytes = report_path.read_bytes()
            if hashlib.sha256(report_bytes).hexdigest() != row.get('report_sha256'):
                return f'BLOCKER: x87 operand evidence report bytes changed for {stem}: {path}'
            report = json.loads(report_bytes)
            outcome, reason = bridge_mutant_outcome(report)
            if outcome != 'killed' or row.get('outcome') != 'killed':
                return (f'BLOCKER: x87 operand evidence is not a kill for {stem}: '
                        f'{outcome}: {reason}; {path}')
            identity_problems = bridge_report_identity_problems(
                report, source_sha, expected_va, profile=profile, execution_identity=execution)
            if identity_problems:
                return ('BLOCKER: x87 operand report is not bound for '
                        f'{stem}: ' + '; '.join(identity_problems) + f': {path}')
        return None
    def _accept(self):
        waiting = []
        for job in self.accept_queue:
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
            self._transaction(job, 'accepting')
            code, output = self.cmre.accept(fn)
            if code == 2:
                self._transaction(job, 'waiting-lease', acceptance_result={'code': code, 'output': output})
                waiting.append(job)   # another writer holds the lease; try next tick
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

    def _accepted(self, job):
        fn = job['fn']
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
            if request.get('kind') == 'retry':
                if fn in self.state['parked'] and fn not in self.state.get('quarantined', {}):
                    self._unpark(fn, 'retry', reason=request.get('reason', '')[:300])
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
                                    expected_identity=request['expected_identity'], reviewed_findings=reviewed_findings,
                                    expected_source_sha256=request['source_sha256'])
                job['source'] = request['source']
                job['_request'] = str(path)
                job['_snap'] = snapshot
                self._event('adopt', fn=fn, source=request['source'])
                self._verify(job, False)

    def tick(self):
        self._control()
        self._input_arrivals()
        self._reap()
        self._requests()
        self._collect()
        self._accept()
        self._requests()  # admit requests unblocked by acceptance before deciding the run drained
        self._dispatch()
        self.save()

    def run(self, poll=10.0):
        self.save()
        self._event('farm-start', pid=os.getpid(), workers=self.config.workers,
                    audit_workers=self.config.audit_workers, ladder=self.config.ladder,
                    continuous=self.config.continuous)
        try:
            while True:
                self.tick()
                if self.drained():
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
             ('edit', 'CM0102/Analysis/cmre/farm/*/hand/*.c', 'allow')]
    shell = [f'{PY} scripts/cmre_farm.py test *', f'{PY} scripts/cmre_farm.py review *']
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
in the claim before deciding. Return ADOPT <source> <review_receipt> <note> or
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
    _durable_json(receipt, {'schema': 'cmre-farm-review/1', 'fn': fn,
                            'identity': view['identity'], 'review': view})
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
    if receipt.get('schema') != 'cmre-farm-review/1' or receipt.get('fn') != fn:
        raise ValueError('review receipt schema/function mismatch')
    snapshot = {'c': _read(path.parent / 'source.c'), 'hint': _read(path.parent / 'hints.json')}
    if snapshot['c'] is None or receipt.get('identity', {}).get('c') is None:
        raise ValueError('review receipt has no C source; fresh review of actual source is required')
    if _identity(snapshot) != receipt['identity']:
        raise ValueError('review snapshot changed; fresh review required')
    return snapshot, hashlib.sha256(raw).hexdigest(), receipt.get('review', {}).get('open_findings', [])


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
    that passes now is registered with no hints identity, so the next farm tick re-gates and
    accepts it through the normal transaction. Nothing is unparked, installed or accepted here."""
    from concurrent.futures import ThreadPoolExecutor
    run_dir, root = Path(run_dir), Path(root)
    cmre = cmre or CmreAdapter(root, include_held=True)
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
            try:
                return cmre.test(entry[0], str(entry[1]))
            except Exception as exc:   # one slow or broken candidate must not lose the others
                return {'status': 'error', 'problems': [str(exc)[:200]]}

        with ThreadPoolExecutor(max_workers=max(1, threads)) as pool:
            reports = list(pool.map(gate, todo))
        for (fn, source), report in zip(todo, reports):
            coverage = input_bound(report)
            passes = report.get('status') == 'pass'
            if coverage is None and not passes:
                counts['failed'] += 1
                state['parked'][fn]['detail'] = report_detail(report)[:600]   # replaces an empty reason
                continue
            counts['passes-now' if passes else 'input-bound'] += 1
            models = state['parked'][fn].get('models') or ['hand']
            hints = _read(root / 'recovery/hints' / f'{fn}.json')
            known[fn] = {'bytes': sizes.get(fn, 0), 'coverage': coverage, 'source': str(source),
                         'model': models[-1], 'at': time.time(),
                         'hints_sha256': 'regate-requested' if passes else _identity({'hint': hints})['hint']}
        tmp = (run_dir / 'state.json').with_suffix('.tmp')
        tmp.write_text(json.dumps(state, indent=1), encoding='utf-8')
        tmp.replace(run_dir / 'state.json')
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
    tmp.replace(path)
    return path


def status_line(s):
    models = ', '.join(f"{m} {v['passes']}/{v['attempts']}" for m, v in s['by_model'].items())
    return (f"accepted {s['accepted_functions']} fn / {s['accepted_bytes']} B in {s['hours']} h "
            f"({s['bytes_per_hour']} B/h) | running {len(s['running'])} | queue {s.get('queue', 0)} "
            f"idle {s.get('idle', 0)} | pending {s['pending']} | "
            f"parked {s['parked']} (input-needed {s.get('input_needed', 0)}) skipped {s['skipped']} | "
            f"audits ok {s['audits_ok']} defect {s['audits_defect']} "
            f"error {s.get('audit_errors', 0)} unconfirmed {s.get('audits_unconfirmed', 0)} "
            f"disputed {s.get('audits_disputed', 0)} "
            f"open {len(s['open_defects'])} | models {models}"
            + ''.join(f' | UNAVAILABLE {m}: {e[:80]}' for m, e in s.get('unavailable_models', {}).items())
             + ''.join(f" | LIMITED {p} until {time.strftime('%H:%M', time.localtime(until))}"
                       for p, until in s.get('limited', {}).items())
             + (f" | QUARANTINED {','.join(sorted(s['quarantined']))}" if s.get('quarantined') else '')
            + (f" | SUPPLY EMPTY {round((time.time() - s['supply_empty_since']) / 60)} min: work the supply lane"
               if s.get('supply_empty_since') and not s['stopping'] else '')
            + (' | STOPPING' if s['stopping'] else ''))


# --- real adapters ---------------------------------------------------------------

class CmreAdapter:
    """Selection and claims through the Python API; gate and acceptance through the CLI."""

    def __init__(self, root=ROOT, *, include_held=False):
        self.root = Path(root)
        self.include_held = include_held

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
            work.claim([int(fn[3:], 16)], owner, hours, include_held=self.include_held)
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
        elif code == 2:
            code = 1  # 2 is reserved by the scheduler for proven lease contention
        return code, (out + err)[-2000:]

    def current_execution_identity(self):
        """Read actual current gate inputs locally, without a recovery/inventory run."""
        from cmre import recover
        return recover.execution_identity()

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
    se.add_argument('--add-hours', type=float, help='extend the deadline')
    ip = sub.add_parser('inputs', help='parked functions whose candidate ran clean but needs more test inputs')
    ip.add_argument('run_dir')
    rt = sub.add_parser('retry', help='return parked functions to the running farm after something changed')
    rt.add_argument('run_dir')
    rt.add_argument('fns', nargs='+', metavar='fn_XXXXXXXX')
    rt.add_argument('--reason', required=True, help='what changed since they parked (harness release, new inputs)')
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
                        continuous=not a.exit_when_empty, rescan_seconds=a.rescan_minutes * 60)
    from concurrent.futures import ThreadPoolExecutor
    with RunLock(run_dir), ThreadPoolExecutor(max_workers=max(1, a.verify_threads)) as verifier:
        (run_dir / 'STOP').unlink(missing_ok=True)
        (run_dir / 'farm.pid').write_text(f'{os.getpid()}\n', encoding='utf-8')
        f = Farm(run_dir, ROOT, CmreAdapter(include_held=a.include_held),
                 make_launcher(a.opencode, ROOT, run_dir), config, verifier=verifier)
        f.run(poll=a.poll)
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
    tmp.replace(path)
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


def start(run_dir, run_args):
    """Launch the supervised farm detached from the caller, with a hidden console that its
    `py -m cmre` children share (a fully detached farm opens a console window per child).

    The farm must outlive the caller's job object. When the job forbids breakaway, a plain
    child dies silently with the shell or session that started it, so the fallback is WMI."""
    run_dir = Path(run_dir)
    run_dir.mkdir(parents=True, exist_ok=True)
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
