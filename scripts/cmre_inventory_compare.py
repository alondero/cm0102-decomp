"""Compare one differential gate under canonical and candidate inventory pins.

Writes local diagnostics only. Neither process accepts, modifies hints, or
shares inventory or run caches with the other.
"""
from __future__ import annotations

import argparse
from contextlib import nullcontext
import csv
import json
import os
from pathlib import Path
import re
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
SCHEMA = 'cmre-inventory-comparison/1'
FILES = {'FUNCTIONS_TSV': 'functions.tsv', 'CALLS_TSV': 'calls.tsv',
         'MODULES_JSON': 'module-candidates.json'}


def _write(path, value):
    with path.open('x', encoding='utf-8') as stream:
        json.dump(value, stream, indent=2)
        stream.write('\n')


def _worker(label, va, output, manifest, source):
    sys.path.insert(0, str(ROOT))
    from cmre import cli, difftest, env, inventory, recover

    output.mkdir()
    with (cli._diagnostic_source(va, source) if source is not None else nullcontext()) as selected:
        hint = env.HINTS_DIR / f'fn_{va:08x}.json'
        try:
            raw_hint = hint.read_bytes()
        except FileNotFoundError:
            raw_hint = None
        env.HINTS_DIR = output / 'hints'
        env.HINTS_DIR.mkdir()
        if raw_hint is not None:
            (env.HINTS_DIR / hint.name).write_bytes(raw_hint)
        if label == 'candidate':
            pins = json.loads(manifest.read_bytes())['candidate_sha256']
            if set(pins) != set(FILES.values()) or not all(
                isinstance(digest, str) and re.fullmatch('[0-9a-f]{64}', digest) for digest in pins.values()
            ):
                raise ValueError('candidate requires exactly three SHA-256 pins')
            paths = {field: manifest.parent / name for field, name in FILES.items()}
            for field, path in paths.items():
                setattr(env, field, path)
            env.INVENTORY_SHA256 = {path: pins[path.name] for path in paths.values()}
        env.ANALYSIS = output / 'analysis'
        env.RUNS = output / 'runs'
        inv = inventory.load()
        with env.FUNCTIONS_TSV.open(encoding='utf-8', newline='') as stream:
            expected = frozenset(int(row['entry_va'], 16) for row in csv.DictReader(stream, delimiter='\t'))
        if frozenset(inv.functions) != expected or difftest._entries() != expected:
            raise ValueError('active indirect entries differ from pinned exports')
        source_sha = env.digest(selected) if selected is not None else None
        opts = difftest.Options.for_function(va, tier='c' if selected is not None else 'lifted')
        profile = recover.c_profile(va) if selected is not None else recover.LIFTED_PROFILE
        if selected is not None:
            opts.inputs = recover.c_inputs(va)
        gate_inputs = {'hints_sha256': env.digest_bytes(raw_hint) if raw_hint is not None else None,
                       'options_sha256': env.digest_bytes(json.dumps(opts.__dict__, sort_keys=True).encode()),
                       'profile': profile, 'execution': {key: value for key, value
                                                        in recover.execution_identity().items() if key != 'inventory'}}
        result = (recover.test_lifted(va, None, True) if selected is None else
                  recover.test_c(va, None, True, source=selected, profile=profile))
        if selected is not None and env.digest(selected) != source_sha:
            raise ValueError('C source changed during comparison')
        if selected is not None and recover.c_inputs(va) != opts.inputs:
            raise ValueError('C input policy changed during comparison')

    print(json.dumps({
        'schema': SCHEMA, 'label': label, 'pid': os.getpid(),
        'inventory': {str(path): sha for path, sha in env.INVENTORY_SHA256.items()},
        'entries': [f'{entry:08x}' for entry in inv.entries_sorted], 'report': result,
        'source_sha256': source_sha,
        'gate_inputs': gate_inputs,
    }))
    return 0


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument('fn')
    p.add_argument('--candidate', type=Path, help='JSON candidate_sha256 pins beside the three export files')
    p.add_argument('--output', type=Path, required=True, help='new local output directory')
    p.add_argument('--source', type=Path, help='compare this C source instead of the lift')
    p.add_argument('--worker', choices=('baseline', 'candidate'), help=argparse.SUPPRESS)
    p.add_argument('--manifest', type=Path, help=argparse.SUPPRESS)
    a = p.parse_args(argv)
    if not re.fullmatch('fn_[0-9a-f]{8}', a.fn):
        p.error('provide a function ID, such as fn_00401000')
    a.output = a.output.resolve()
    source = Path(os.path.abspath(a.source)) if a.source is not None else None
    if a.worker:
        if a.candidate is not None or (a.manifest is not None) != (a.worker == 'candidate'):
            p.error('invalid comparison worker scope')
        return _worker(a.worker, int(a.fn[3:], 16), a.output, a.manifest, source)
    if a.candidate is None or a.manifest is not None:
        p.error('--candidate is required')
    a.output.mkdir(parents=True)
    rows = {}
    for label in ('baseline', 'candidate'):
        command = [sys.executable, str(Path(__file__).resolve()), a.fn,
                   '--worker', label, '--output', str(a.output / label)]
        if label == 'candidate':
            command += ['--manifest', str(a.candidate.resolve(strict=True))]
        if source is not None:
            command += ['--source', str(source)]
        result = subprocess.run(command, cwd=ROOT, capture_output=True, encoding='utf-8', check=False)
        (a.output / f'{label}.stderr').write_text(result.stderr, encoding='utf-8')
        if result.returncode:
            print(f'{label} worker failed (exit {result.returncode}); see {a.output}', file=sys.stderr)
            return 2
        row = json.loads(result.stdout)
        if row.get('schema') != SCHEMA or row.get('label') != label:
            raise ValueError('comparison worker returned an unexpected report')
        rows[label] = row
    before, after = (set(rows[label]['entries']) for label in ('baseline', 'candidate'))
    if (not rows['baseline'].get('gate_inputs')
            or rows['baseline']['gate_inputs'] != rows['candidate'].get('gate_inputs')
            or rows['baseline'].get('source_sha256') != rows['candidate'].get('source_sha256')):
        print('Gate inputs changed between workers; no comparison receipt written', file=sys.stderr)
        return 2
    summary = {
        'schema': SCHEMA, 'diagnostic_only': True,
        'added': sorted(after - before), 'removed': sorted(before - after),
        **{label: {k: v for k, v in row.items() if k != 'entries'} for label, row in rows.items()},
    }
    _write(a.output / 'comparison.json', summary)
    print(json.dumps({'receipt': str(a.output / 'comparison.json'), 'added': summary['added'],
                      'removed': summary['removed'],
                      'inventory': {label: row['inventory'] for label, row in rows.items()},
                      'gate_inputs': rows['baseline']['gate_inputs'],
                      **{label: row['report']['status']
                                                        for label, row in rows.items()}}))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
