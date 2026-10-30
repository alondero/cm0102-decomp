"""Export unvalidated Ghidra pseudocode drafts for every function.

Copies the baseline Ghidra project (the inventory's source) so the original
project is never opened for writing, then runs ExportDrafts.java read-only.
Drafts land in CM0102/Analysis/ghidra-drafts/fn_XXXXXXXX.c and are read by
`cmre brief`. They are translation input, never acceptance evidence.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parents[1]
SHA256 = '0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86'
BASELINE = ROOT / 'CM0102/Analysis/baseline-c'
DEFAULT_OUT = ROOT / 'CM0102/Analysis/ghidra-drafts'


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, default=DEFAULT_OUT, help='New directory; existing paths are refused')
    parser.add_argument('--ghidra', type=Path, default=ROOT / 'tools/ghidra/ghidra_12.1.4_PUBLIC')
    parser.add_argument('--java-home', type=Path, default=Path('C:/Program Files/Java/jdk-21.0.10'))
    args = parser.parse_args()
    out = args.output.resolve()
    if out.exists():
        raise SystemExit(f'{out} exists; choose a new --output')
    if digest(BASELINE / 'cm0102.exe') != SHA256:
        raise SystemExit('Baseline project executable hash mismatch')
    work = out.with_name(out.name + '-project')
    if work.exists():
        raise SystemExit(f'{work} exists; remove it or choose a new --output')
    shutil.copytree(BASELINE, work, ignore=shutil.ignore_patterns('*.lock', '*.lock~', 'exports'))
    drafts = out
    command = [str(args.ghidra / 'support/analyzeHeadless.bat'), str(work), 'cm0102',
               '-process', 'cm0102.exe', '-noanalysis', '-readOnly',
               '-scriptPath', str(ROOT / 'scripts/ghidra'), '-postScript', 'ExportDrafts.java', str(drafts)]
    env = os.environ.copy()
    env['JAVA_HOME'] = str(args.java_home.resolve())
    started = time.time()
    work.mkdir(exist_ok=True)
    with (work / 'headless.log').open('w', encoding='utf-8') as log:
        result = subprocess.run(command, env=env, stdout=log, stderr=subprocess.STDOUT)
    index = drafts / 'index.tsv'
    rows = index.read_text(encoding='utf-8').splitlines()[1:] if index.is_file() else []
    record = dict(input_sha256=SHA256, ghidra_version='12.1.4', command=command, exit_code=result.returncode,
                  seconds=round(time.time() - started, 1), functions=len(rows),
                  failed=sum(1 for r in rows if '\tfailed\t' in r),
                  script_sha256=digest(ROOT / 'scripts/ghidra/ExportDrafts.java'))
    if drafts.is_dir():
        (drafts / 'run.json').write_text(json.dumps(record, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({k: v for k, v in record.items() if k != 'command'}))
    return 1 if result.returncode or not rows else 0


if __name__ == '__main__':
    sys.exit(main())
