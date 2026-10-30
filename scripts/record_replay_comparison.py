"""Freeze and compare the guarded July-28 GDI observations; claim no consumed-input parity."""
import argparse
import json
from pathlib import Path
import shutil
import sys
from prepare_runtime import ROOT, digest, SHA256, GDI_SHA256
from record_runtime_reference import KNOWN_SAVES


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    runtime, out = args.runtime.resolve(), args.output.resolve()
    area = ROOT / 'CM0102/Runtime'
    if runtime == area or out == area or not runtime.is_relative_to(area) or not out.is_relative_to(area):
        parser.error('Disposable runtime and fresh output beneath Runtime required')
    if digest(runtime / 'cm0102.exe') != SHA256 or digest(runtime / 'cm0102_GDI.exe') != GDI_SHA256:
        raise ValueError('Runtime executable changed')
    prep = json.loads((runtime / 'replay-inputs.json').read_text(encoding='utf-8'))
    for row in prep['runtime_inputs']:
        if digest(runtime / row['path']) != row['sha256']:
            raise ValueError(f'Runtime input changed: {row["path"]}')
    for name, expected in KNOWN_SAVES.items():
        if digest(runtime / name) != expected:
            raise ValueError('Frozen scenario save changed')
    out.mkdir(exist_ok=False)
    artifacts = []

    def freeze(relative):
        source, target = runtime / relative, out / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source, target)
        expected = digest(source)
        assert digest(target) == expected
        artifacts.append(dict(path=relative, sha256=expected, bytes=source.stat().st_size))

    runs = {}
    for name in ('b', 'c'):
        events, identity = [], None
        initial = json.loads((runtime / f'clean-{name}-initial.json').read_text(encoding='utf-8'))
        final = json.loads((runtime / f'clean-{name}-end.json').read_text(encoding='utf-8'))
        observer = ROOT / 'scripts/capture_replay_rng.py'
        assert digest(observer) == initial['script_sha256'] == final['script_sha256']
        for observation in (initial, final):
            assert observation['mapping_sha256'] == digest(ROOT / 'CM0102/Analysis/rng-gdi-map-b/report.json')
            assert observation['phase_mapping_sha256'] == digest(ROOT / 'CM0102/Analysis/phase-gdi-map-b/report.json')
        for batch in range(1, 7):
            relative = f'clean-{name}-batch{batch}/inputs.json'
            record = json.loads((runtime / relative).read_text(encoding='utf-8'))
            assert record['status'] == 'pass'
            if identity is None:
                identity = record['process']
            assert record['process'] == identity
            assert identity['pid'] == initial['pid'] == final['pid']
            assert identity['executable'] == initial['executable'] == final['executable']
            assert identity['executable_sha256'] == initial['executable_sha256'] == final['executable_sha256'] == GDI_SHA256
            assert initial['actual_module_base'] == final['actual_module_base'] == '0x00400000'
            assert digest(ROOT / 'CM0102/Analysis/replay-continue-batch.py') == record['script_sha256']
            for tool, expected in record['tools'].items():
                assert digest(ROOT / 'scripts' / tool) == expected
            assert len(record['events']) == (6 if batch == 6 else 10)
            freeze(relative)
            for index, event in enumerate(record['events'], 1):
                assert [(row['message'], row['wparam'], row['lparam'], row['posted']) for row in event['messages']] == [
                    ('0x200', 0, 7798829, True), ('0x201', 1, 7798829, True), ('0x202', 0, 7798829, True)]
                image = f'clean-{name}-batch{batch}/after-{index:02d}.png'
                assert digest(runtime / image) == event['capture_sha256']
                freeze(image)
                events.append(event)
        for relative in (f'clean-{name}-initial.json', f'clean-{name}-initial.png',
                f'clean-{name}-end.json', f'replay-clean-{name}.sav'):
            freeze(relative)
        for path in sorted(runtime.glob(f'clean-{name}-*.json')):
            if path.name not in (f'clean-{name}-initial.json', f'clean-{name}-end.json'):
                freeze(path.name)
        runs[name] = dict(process=identity, initial=initial['state'], final=final['state'],
            initial_observer_sha256=initial['script_sha256'], final_observer_sha256=final['script_sha256'],
            posted_continue_requests=len(events), events=events)
    initial_equal = {key: runs['b']['initial'][key]['bytes'] == runs['c']['initial'][key]['bytes'] for key in runs['b']['initial']}
    final_equal = {key: runs['b']['final'][key]['bytes'] == runs['c']['final'][key]['bytes'] for key in runs['b']['final']}
    images_equal = [a['capture_sha256'] == b['capture_sha256'] for a, b in zip(runs['b']['events'], runs['c']['events'])]
    a, b = (runtime / 'replay-clean-b.sav').read_bytes(), (runtime / 'replay-clean-c.sav').read_bytes()
    prefix = next((i for i, (x, y) in enumerate(zip(a, b)) if x != y), min(len(a), len(b)))
    freeze('replay-inputs.json')
    freeze('game.cfg')
    sources = ('scripts/replay_runtime_guard.py', 'scripts/replay_gdi_click.py', 'scripts/replay_gdi_text.py',
        'scripts/gdi_interact.py', 'scripts/capture_game.ps1', 'scripts/capture_replay_rng.py',
        'scripts/prepare_reference_replay.py', 'scripts/record_replay_comparison.py',
        'CM0102/Analysis/replay-continue-batch.py')
    pins = {}
    for relative in sources:
        source, target = ROOT / relative, out / 'source-used' / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source, target)
        pins[relative] = digest(source)
    report = dict(status='observed_divergence', command=sys.argv, python=sys.version,
        script_sha256=digest(Path(__file__)), source=runtime.relative_to(ROOT).as_posix(),
        executable_sha256=GDI_SHA256, primary_sha256=SHA256, checked_runtime_inputs=len(prep['runtime_inputs']),
        address_convention='Each observation labels preferred-image and actual runtime VAs; module base 0x00400000',
        initial_equal=initial_equal, final_equal=final_equal,
        same_number_screenshot_equality=images_equal, first_different_png_request=images_equal.index(False) + 1,
        raw_save_comparison=dict(equal=a == b, first_different_file_offset=prefix,
            a_bytes=len(a), b_bytes=len(b), normalized=False),
        runs=runs, artifacts=artifacts, source_pins=pins,
        limitations=['Identical posted payload order/count; actual intervals differ and consumption is unacknowledged',
            'Sequential unsuspended bounded readings are not atomic or complete world-state comparisons',
            'PNG differences may include blinking/window decoration; inspect semantic differences separately',
            'Current game.cfg is post-run configuration, not a frozen initial config',
            'Restart-inherited command lines were not independently captured',
            'No deterministic replay, primary runtime parity, M4 integration or standalone gate accepted'])
    (out / 'comparison.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print('Frozen two 56-request runs:', sum(images_equal), 'equal PNGs; endpoint/raw saves differ')


if __name__ == '__main__':
    main()
