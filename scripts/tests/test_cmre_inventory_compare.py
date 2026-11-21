import json
from pathlib import Path
import subprocess
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))


def test_comparison_runs_fresh_children_and_keeps_negative_gate(tmp_path, monkeypatch):
    import cmre_inventory_compare as compare

    calls = []
    manifest = tmp_path / 'pins.json'
    manifest.write_text('{}', encoding='utf-8')
    out = tmp_path / 'comparison'

    def child(command, **kwargs):
        calls.append(command)
        label = command[command.index('--worker') + 1]
        candidate = label == 'candidate'
        report = {'schema': compare.SCHEMA, 'label': label, 'pid': 80 + len(calls),
                  'inventory': {'functions': 'b' if candidate else 'a'},
                  'gate_inputs': {'hints_sha256': 'same'},
                  'entries': ['00401000', '00402000'] if candidate else ['00401000'],
                  'report': {'status': 'pass' if candidate else 'low-coverage'}}
        return subprocess.CompletedProcess(command, 0, json.dumps(report), '')

    monkeypatch.setattr(compare.subprocess, 'run', child)
    assert compare.main(['fn_00401000', '--candidate', str(manifest), '--output', str(out)]) == 0
    report = json.loads((out / 'comparison.json').read_bytes())
    assert report['added'] == ['00402000']
    assert report['baseline']['report']['status'] == 'low-coverage'
    assert report['candidate']['report']['status'] == 'pass'
    assert len(calls) == 2 and '--manifest' not in calls[0] and '--manifest' in calls[1]


def test_comparison_refuses_changed_gate_inputs_between_children(tmp_path, monkeypatch):
    import cmre_inventory_compare as compare

    manifest = tmp_path / 'pins.json'
    manifest.write_text('{}', encoding='utf-8')
    out = tmp_path / 'comparison'

    def child(command, **kwargs):
        label = command[command.index('--worker') + 1]
        row = {'schema': compare.SCHEMA, 'label': label, 'entries': ['00401000'],
               'gate_inputs': {'hints_sha256': label}, 'report': {'status': 'pass'}}
        return subprocess.CompletedProcess(command, 0, json.dumps(row), '')

    monkeypatch.setattr(compare.subprocess, 'run', child)
    assert compare.main(['fn_00401000', '--candidate', str(manifest), '--output', str(out)]) == 2
    assert not (out / 'comparison.json').exists()


def test_worker_rejects_preexisting_cache_and_bad_inventory_pins(tmp_path):
    import cmre_inventory_compare as compare

    manifest = tmp_path / 'pins.json'
    manifest.write_text(json.dumps({'candidate_sha256': {name: '0' * 64
                                                        for name in compare.FILES.values()}}), encoding='utf-8')
    for name in compare.FILES.values():
        (tmp_path / name).write_text('not the pinned export', encoding='utf-8')
    out = tmp_path / 'existing'
    (out / 'analysis/cmre').mkdir(parents=True)
    marker = out / 'analysis/cmre/cache.json'
    marker.write_text('preserve', encoding='utf-8')
    command = [sys.executable, compare.__file__, 'fn_00401000', '--worker', 'candidate',
               '--manifest', str(manifest), '--output']
    stale = subprocess.run([*command, str(out)], capture_output=True, text=True)
    assert stale.returncode and 'FileExistsError' in stale.stderr
    assert marker.read_text() == 'preserve'
    mismatch = subprocess.run([*command, str(tmp_path / 'fresh')], capture_output=True, text=True)
    assert mismatch.returncode and 'Pinned inventory hash differs' in mismatch.stderr
