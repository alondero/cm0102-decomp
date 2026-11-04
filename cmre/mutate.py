"""Sensitivity diagnostics; acceptance and its differential gate are unchanged."""
from __future__ import annotations

import re
import tempfile
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


def run(va, source, gate=None):
    from . import recover
    gate = gate or recover.test_c
    source = Path(source)
    original = source.read_text(encoding='utf-8')
    survivors, killed, invalid = [], [], []
    with tempfile.TemporaryDirectory(prefix='cmre-mutate-') as tmp:
        candidate = Path(tmp) / source.name
        candidate.write_text(original, encoding='utf-8', newline='')
        baseline = gate(va, source=candidate, profile='semantic', fresh=True)
        if baseline.get('status') != 'pass':
            raise ValueError('Original source did not pass the unchanged semantic gate in the diagnostic workspace')
        for row in mutants(original):
            candidate.write_text(row.pop('source'), encoding='utf-8', newline='')
            report = gate(va, source=candidate, profile='semantic', fresh=True)
            builds = report.get('builds', {})
            if any(b.get('status') == 'build-failed' for b in builds.values()):
                invalid.append(row)
            elif report.get('status') == 'pass':
                survivors.append(row)
            else:
                killed.append(row)
    return {'va': f'{va:#010x}', 'survivors': survivors, 'killed': killed, 'invalid': invalid}
