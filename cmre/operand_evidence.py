"""Retained ADR 0010 operand evidence shared by farm and direct C acceptance."""
from __future__ import annotations

import hashlib
import json
import re

from . import coordination

_X87_BRIDGE = 'cm_x87_CIpow'
_X87_BUILD_LEVELS = ('-O0', '-O2')

_NONCODE = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|/\*.*?\*/|//[^\n]*', re.S)

def _mask_noncode(match):
    return re.sub(r'[^\r\n]', ' ', match.group())


def operand_proof_needed(raw: bytes, gate: dict) -> bool:
    """Include compiler-expanded calls even when source spelling hid the bridge."""
    return (_X87_BRIDGE.encode() in raw
            or any(level.get('cipow_calls') for level in gate.get('builds', {}).values()))


def _compiled_sites(gate: dict) -> int:
    builds = gate.get('builds', {})
    ordinary = builds.get('-O0', {}).get('cipow_calls', 0)
    # O2 may clone an expression, so count its presence if O0 removed it.
    return ordinary or int(bool(builds.get('-O2', {}).get('cipow_calls')))


def dependency_hashes(report: dict) -> dict:
    """Compiler-selected dependencies shared by the scratch and canonical units."""
    name = report.get('source')
    return {path: value for path, value in report.get('source_dependencies', {}).items() if path != name}


def _options_compatible(report: dict, baseline: dict) -> bool:
    return (report.get('effective_options_sha256') == baseline.get('effective_options_sha256')
            and dependency_hashes(report) == dependency_hashes(baseline))


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
                                    profile=None, execution_identity=None, baseline=None):
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
    if baseline is not None and not _options_compatible(report, baseline):
        problems.append('effective C options or compiler dependencies differ from baseline')
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


def generate(summary_path, fn, job_number, text_bytes, baseline, hints_sha, *, gate, check_inputs, report_event):
    """ADR 0010 condition 3 before any canonical install or acceptance.

    Each cm_x87_CIpow call is copied twice, once with +1.0L on the base and
    once on the exponent. Every mutant must show a differential mismatch at
    both ordinary builds. Reports and sources are bound by SHA-256; exact
    decoded raw bytes drive generation so CRLF is preserved. Survivors,
    build failures, inconclusive runs, unbound reports and source/hint drift
    are BLOCKERs, and all received evidence stays under RUN/x87-mutants/."""
    if not operand_proof_needed(text_bytes, baseline):
        return None
    encoded = text_bytes
    digest = hashlib.sha256(encoded).hexdigest()
    directory = summary_path.parent
    directory.mkdir(parents=True, exist_ok=True)
    summary = {'schema': 'cmre-farm-x87-operands/1', 'fn': fn, 'job': job_number,
               'candidate_sha256': digest, 'hints_sha256': hints_sha,
               'baseline': None, 'mutants': [], 'status': 'running'}

    def retained(detail):
        summary.update(status='blocked', detail=detail)
        coordination.atomic_json(summary_path, summary)
        report_event(summary['status'])
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
    if len(mutants) // 2 < _compiled_sites(baseline):
        return retained(f'BLOCKER: x87 compiled bridge calls lack individually parsed source calls; evidence {summary_path}')
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
    coordination.atomic_bytes(baseline_source, encoded)
    coordination.atomic_json(baseline_report, baseline)
    summary['baseline'] = {
        'source': baseline_source.name, 'source_sha256': digest,
        'report': baseline_report.name,
        'report_sha256': hashlib.sha256(baseline_report.read_bytes()).hexdigest(),
        'gate_key': baseline.get('key')}
    for row in mutants:
        stem = f"call-{row['index']:03d}-{row['operand']}"
        mutant_path = directory / f'{stem}.c'
        report_path = directory / f'{stem}.json'
        coordination.atomic_bytes(mutant_path, row['source'].encode('utf-8'))
        gate_report = {'status': 'error', 'problems': ['gate did not return a report']}
        try:
            gate_report = gate(mutant_path)
        except (Exception, SystemExit) as exc:
            gate_report = {'status': 'error', 'problems': [f'{type(exc).__name__}: {exc}']}
        outcome, reason = bridge_mutant_outcome(gate_report)
        source_sha = hashlib.sha256(mutant_path.read_bytes()).hexdigest()
        identity_problems = bridge_report_identity_problems(
            gate_report, source_sha, expected_va, profile=profile, execution_identity=execution, baseline=baseline)
        if identity_problems:
            outcome, reason = 'unresolved', 'report-identity: ' + '; '.join(identity_problems)
        coordination.atomic_json(report_path, gate_report)
        row.update(source=mutant_path.name, source_sha256=source_sha,
                   report=report_path.name,
                   report_sha256=hashlib.sha256(report_path.read_bytes()).hexdigest(),
                   gate_key=gate_report.get('key') if isinstance(gate_report, dict) else None,
                   outcome=outcome, reason=reason)
        summary['mutants'].append(row)
        coordination.atomic_json(summary_path, summary)
        if detail := check_inputs():
            return retained(f'BLOCKER: x87 operand mutant evidence invalid: {detail}; evidence {summary_path}')
    blocked = [row for row in summary['mutants'] if row['outcome'] != 'killed']
    if blocked:
        row = blocked[0]
        return retained(
            f"BLOCKER: x87 operand mutant {row['operand']} for call {row['index'] + 1} "
            f"(line {row['line']}) {row['outcome']}: {row['reason']}; evidence {summary_path}")
    summary['status'] = 'killed'
    coordination.atomic_json(summary_path, summary)
    report_event('killed')
    return None

def verify_receipt(path, raw, *, fn, job_number, hints_sha, current_execution, active_gate=None):
    text = raw.decode('utf-8')
    digest = hashlib.sha256(raw).hexdigest()
    expected_va = '0x' + fn[3:]
    try:
        summary = json.loads(path.read_bytes())
    except (OSError, ValueError) as exc:
        return f'BLOCKER: x87 operand evidence missing or unreadable for acceptance: {path}: {exc}'
    if (not isinstance(summary, dict)
            or summary.get('schema') != 'cmre-farm-x87-operands/1'
            or summary.get('fn') != fn or summary.get('job') != job_number
            or summary.get('candidate_sha256') != digest
            or summary.get('hints_sha256') != hints_sha
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
        execution_now = current_execution()
    except (Exception, SystemExit) as exc:
        return (f'BLOCKER: x87 operand current execution identity unavailable: '
                f'{type(exc).__name__}; evidence {path}')
    if not valid_execution_identity(execution_now):
        return f'BLOCKER: x87 operand current execution identity is malformed: {path}'
    if execution != execution_now:
        return f'BLOCKER: x87 operand evidence differs from current execution identity: {path}'
    if active_gate is not None:
        problems = bridge_report_identity_problems(
            active_gate, digest, expected_va, profile=profile,
            execution_identity=execution, baseline=baseline_evidence,
        )
        if problems:
            return ('BLOCKER: x87 operand proof differs from current C gate: '
                    + '; '.join(problems) + f': {path}')
    expected = bridge_mutant_sources(text)
    if len(expected) // 2 < _compiled_sites(active_gate or baseline_evidence):
        return f'BLOCKER: x87 compiled bridge calls lack individually parsed source calls; evidence {path}'
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
            report, source_sha, expected_va, profile=profile, execution_identity=execution, baseline=baseline_evidence)
        if identity_problems:
            return ('BLOCKER: x87 operand report is not bound for '
                    f'{stem}: ' + '; '.join(identity_problems) + f': {path}')
    return None
