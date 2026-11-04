"""Sensitivity diagnostics; acceptance and its differential gate are unchanged."""
from __future__ import annotations

import re
import json
import tempfile
from contextlib import nullcontext
from pathlib import Path

_TOKEN = re.compile(r'''(?P<directive>^[ \t]*\#(?:[^\n]*\\\n)*[^\n]*)|(?P<comment>/\*.*?\*/|//[^\n]*)|(?P<string>"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*')|(?P<identifier>[A-Za-z_]\w*)|(?P<number>\b(?:0[xX][0-9a-fA-F]+|[0-9]+)(?:[uUlL]*)\b)|(?P<op><=|>=|==|!=|(?<![<>=!])[<>](?![<>=]))''', re.M | re.S)
_OPS = {'<': '<=', '<=': '<', '>': '>=', '>=': '>', '==': '!=', '!=': '=='}


def mutants(source):
    for token in _TOKEN.finditer(source):
        old = token.group()
        if token.lastgroup == 'op':
            new = _OPS[old]
        elif token.lastgroup == 'number':
            parts = re.fullmatch(r'(0[xX][0-9a-fA-F]+|[0-9]+)([uUlL]*)', old)
            value, suffix = parts.groups()
            new = (hex(int(value, 16) + 1) if value.lower().startswith('0x') else str(int(value) + 1)) + suffix
        else:
            continue
        yield {'line': source.count('\n', 0, token.start()) + 1, 'column': token.start() - source.rfind('\n', 0, token.start()),
               'old': old, 'new': new, 'source': source[:token.start()] + new + source[token.end():]}


def classify(report):
    if report.get('status') == 'pass':
        return 'survivors', None
    builds = report.get('builds', {})
    if any(b.get('status') == 'build-failed' for b in builds.values()):
        return 'invalid', 'build-failed'
    if report.get('problems'):
        return 'unresolved', 'contract-or-report-error'
    mismatches = [m for b in builds.values() for m in b.get('difftest', {}).get('mismatches', [])]
    if any(m.get('field') or m.get('kind') not in (None, 'budget-asymmetry') for m in mismatches):
        return 'killed', 'differential-mismatch'
    return 'unresolved', 'gate-failed-without-mismatch'


def run(va, source, gate=None, *, fence=None):
    from . import env, recover
    source = Path(source)
    parent = env.ANALYSIS / 'cmre/mutations'
    parent.mkdir(parents=True, exist_ok=True)
    tmp = tempfile.mkdtemp(prefix=f'fn_{va:08x}-', dir=parent)
    candidate = Path(tmp) / source.name
    with (fence(va, candidate) if fence else nullcontext(candidate)) as candidate:
        if gate is None and recover.c_profile(va) != 'semantic':
            raise ValueError('Mutation diagnostics require the accepted semantic profile')
        return _run_fenced(va, source, candidate, gate or recover.test_c, tmp)


def _run_fenced(va, source, candidate, gate, tmp):
    from . import env
    original = source.read_bytes().decode('utf-8')
    baseline_sha = env.digest_bytes(original.encode('utf-8'))
    result = {'va': f'{va:#010x}', 'baseline_sha256': baseline_sha,
              'survivors': [], 'killed': [], 'invalid': [], 'unresolved': [], 'directory': tmp}
    candidate.write_text(original, encoding='utf-8', newline='')
    baseline = gate(va, source=candidate, profile='semantic', fresh=True)
    (Path(tmp) / 'baseline.json').write_text(json.dumps(baseline, indent=2), encoding='utf-8')
    (Path(tmp) / 'baseline.c').write_text(original, encoding='utf-8', newline='')
    result.update(baseline_gate_key=baseline.get('key'),
                  baseline_report_path=str(Path(tmp) / 'baseline.json'),
                  baseline_source_path=str(Path(tmp) / 'baseline.c'))
    if baseline.get('status') != 'pass':
        raise ValueError('Original source did not pass the unchanged semantic gate in the diagnostic workspace')
    for index, row in enumerate(mutants(original)):
        text = row.pop('source')
        candidate.write_text(text, encoding='utf-8', newline='')
        saved_source = Path(tmp) / f'{index:04d}.c'
        saved_source.write_text(text, encoding='utf-8', newline='')
        try:
            report = gate(va, source=candidate, profile='semantic', fresh=True)
            category, error = classify(report)
        except (Exception, SystemExit) as exc:
            category, error = 'unresolved', type(exc).__name__
            report = {'status': 'error', 'error_class': error, 'detail': str(exc)}
        report_path = Path(tmp) / f'{index:04d}.json'
        report_path.write_text(json.dumps(report, indent=2), encoding='utf-8')
        row.update(baseline_sha256=baseline_sha, source_sha256=env.digest(saved_source),
                   source_path=str(saved_source), report_path=str(report_path),
                   gate_key=report.get('key'), error_class=error)
        result[category].append(row)
    return result
