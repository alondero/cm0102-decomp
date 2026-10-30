"""Compare every stable export from two independent analysis runs."""
import argparse
import hashlib
import json
from pathlib import Path
from analyze_baseline import SHA256, ARCHIVE_SHA256

def hashes(path):
    record = json.loads((path / 'run.json').read_text())
    if record['exit_code'] != 0 or not record.get('scripts_unchanged') or record['input_sha256'] != record['preserved_input_sha256_after']:
        raise SystemExit(f'Unsuccessful analysis: {path}')
    if record['input_sha256'] != SHA256 or record['archive_sha256'] != ARCHIVE_SHA256:
        raise SystemExit(f'Unexpected input or tool identity: {path}')
    actual = {p.name: hashlib.sha256(p.read_bytes()).hexdigest()
              for p in sorted((path / 'exports').iterdir()) if p.is_file()}
    required = {'analysis-options.tsv', 'pe.json', 'raw-strings.tsv', 'program.txt', 'memory.tsv', 'defined-strings.tsv',
                'functions.tsv', 'imports.tsv', 'startup-disassembly.tsv', 'startup-references.tsv'}
    required.update(f'startup-{v}.c' for v in ['947a13','68d370','5cc6d0','5cc6b0','68d520','5cc7d0'])
    if not required <= actual.keys() or actual != record['outputs']:
        raise SystemExit(f'Missing or altered exports: {path}')
    return actual

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('first', type=Path)
    parser.add_argument('second', type=Path)
    args = parser.parse_args()
    if args.first.resolve() == args.second.resolve():
        raise SystemExit('Two distinct analysis directories are required')
    records = [json.loads((path / 'run.json').read_text()) for path in [args.first, args.second]]
    for key in ['scripts', 'distribution_manifest_sha256', 'java', 'ghidra_version']:
        if not records[0].get(key) or records[0][key] != records[1].get(key):
            raise SystemExit(f'Analysis provenance differs or missing: {key}')
    first, second = hashes(args.first), hashes(args.second)
    changed = [name for name in sorted(first.keys() | second.keys()) if first.get(name) != second.get(name)]
    if not first or not second:
        raise SystemExit('Cannot compare empty export directories')
    print(json.dumps(dict(first=str(args.first), second=str(args.second),
                          exports_compared=len(first.keys() | second.keys()), differences=changed), indent=2))
    raise SystemExit(bool(changed))

if __name__ == '__main__':
    main()
