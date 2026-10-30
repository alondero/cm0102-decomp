"""Install the frozen M2 saves/config into an already prepared disposable runtime."""
import argparse
import json
import platform
import shutil
import sys
from pathlib import Path

from prepare_runtime import ROOT, SHA256, GDI_SHA256, digest
from record_runtime_reference import KNOWN_SAVES


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime',type=Path,required=True)
    args=parser.parse_args()
    runtime=args.runtime.resolve()
    runtime_root=ROOT/'CM0102/Runtime'
    if runtime==runtime_root or not runtime.is_relative_to(runtime_root):
        parser.error('runtime must be a disposable directory beneath CM0102/Runtime')
    record_path=runtime/'replay-inputs.json'
    if record_path.exists():raise ValueError('Replay preparation already recorded')
    for name,expected in (('cm0102.exe',SHA256),('cm0102_GDI.exe',GDI_SHA256)):
        if digest(runtime/name)!=expected:raise ValueError(f'Executable mismatch: {name}')
    reference=runtime_root/'reference-20260927-verified'
    manifest=json.loads((reference/'reference.json').read_text(encoding='utf-8'))
    inputs=manifest['runtime_baseline_files']
    for row in inputs:
        if digest(runtime/row['path'])!=row['sha256']:
            raise ValueError(f'Runtime input differs: {row["path"]}')
    artifacts={row['path']:row for row in manifest['artifacts']}
    names=list(KNOWN_SAVES)+['game.cfg']
    for name in names:
        expected=artifacts[name]['sha256']
        if name in KNOWN_SAVES and expected!=KNOWN_SAVES[name]:raise ValueError('Unknown scenario save')
        if digest(reference/name)!=expected:raise ValueError(f'Frozen reference changed: {name}')
        if (runtime/name).exists():raise ValueError(f'Refusing existing replay artifact: {name}')
    for name in names:
        shutil.copyfile(reference/name,runtime/name)
        if digest(runtime/name)!=artifacts[name]['sha256']:raise ValueError(f'Copy differs: {name}')
    record=dict(status='pass',command=sys.argv,python=sys.version,platform=platform.platform(),
        script_sha256=digest(Path(__file__)),reference_manifest_sha256=digest(reference/'reference.json'),
        source=reference.relative_to(ROOT).as_posix(),runtime=runtime.relative_to(ROOT).as_posix(),
        runtime_inputs=inputs,copied_artifacts={name:artifacts[name]for name in names},
        script_inputs={p.relative_to(ROOT).as_posix():digest(p)for p in
            (ROOT/'scripts/prepare_runtime.py',ROOT/'scripts/repair_runtime_formations.py',
             ROOT/'scripts/record_runtime_reference.py')},
        scope='Preparation only; no repeated scenario or primary/GDI equivalence established')
    record_path.write_text(json.dumps(record,indent=2)+'\n',encoding='utf-8')
    print(f'PASS replay input preparation: {len(inputs)} baseline/repair inputs; two frozen saves and configuration')


if __name__=='__main__':main()
