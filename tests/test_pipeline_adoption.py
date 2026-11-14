"""Automatic adoption preserves native gates, current holds and fixture domains."""
import copy
import json
from pathlib import Path
from types import SimpleNamespace

import pytest

from cmre import env, inventory, ledger, pipeline_adoption as adoption
from cmre import provider_campaign as pc, provider_jobs, recover, tasks, work, image
from cmre.pipeline_flow import pin
from cmre.provider_runtime import Executor


def test_merge_preserves_both_fixture_prefixes_and_policies():
    hints = {'auto': True, 'cases': 3000, 'budget': 200000, 'args': ['any'],
             'fixtures': [{'old': 1}], 'c_hints': {'fixtures': [{'c': 2}], 'ecx': 'ptr'}}
    frozen = copy.deepcopy(hints)
    result = adoption.merge_fixtures(hints, [{'new': 3}, {'new': 3}])
    assert hints == frozen
    assert result['fixtures'] == [{'old': 1}, {'new': 3}]
    assert result['c_hints']['fixtures'] == [{'c': 2}, {'new': 3}]
    assert result['cases'] == 3000 and result['budget'] == 200000
    assert result['args'] == ['any'] and result['c_hints']['ecx'] == 'ptr'
    assert 'auto' not in result


def make_case(tmp_path, monkeypatch):
    va = 0x401000
    from cmre import coordination
    monkeypatch.setattr(coordination, '_path', lambda: tmp_path / 'command.lock')
    monkeypatch.setattr(env, 'ANALYSIS', tmp_path)
    monkeypatch.setattr(env, 'HINTS_DIR', tmp_path / 'hints')
    monkeypatch.setattr(env, 'C_DIR', tmp_path / 'c')
    env.HINTS_DIR.mkdir()
    env.C_DIR.mkdir()
    monkeypatch.setattr(inventory, 'load', lambda: SimpleNamespace(functions={va: None}))
    monkeypatch.setattr(tasks, 'status', lambda: [])
    monkeypatch.setattr(work, 'claims', lambda: {})
    monkeypatch.setattr(image, 'primary', lambda: SimpleNamespace(read=lambda a, n: b'\xc3' * n))
    monkeypatch.setattr(ledger, 'record_path', lambda target: tmp_path / 'ledger.json')
    hints = env.HINTS_DIR / 'fn_00401000.json'
    hints.write_text(json.dumps({'cases': 600, 'fixtures': [{'old': 7}]}))

    def identity(target):
        return {'target': 'pinned', 'hints': pin(hints)['sha256'] if hints.exists() else None}

    monkeypatch.setattr(work, 'identity', identity)
    old = {'state': 'evidence-needed', 'reason': 'prove argument', 'retry_when': 'original byte proof',
           'blocker': 'input', 'identity': identity(va), 'attempts': 4, 'failures': 2}
    work.path().write_text(json.dumps({'schema': 'cmre-work/1', 'functions': {'fn_00401000': old}}))
    directory = tmp_path / 'run'
    root = directory / 'jobs' / 'input'
    fixture = root / 'fixture.json'
    resolution = {'reason': 'Original argument load established', 'obligations': ['prove argument'],
                  'unknowns': [], 'evidence': [{'va': '0x00401000', 'bytes': 'c3', 'supports': 'return'}]}
    guard = {'work': old, 'identity': identity(va), 'hints': pin(hints), 'source': None}
    config = {'function': '0x00401000', 'mode': 'input', 'owner': 'deepseek',
              'reviewer': 'minimax', 'goal': 'prove input', 'directory': str(root),
              'adoption_guard': guard}
    manifest = {'schema': pc.SCHEMA, 'duration_seconds': 60, 'concurrency': 2,
                'roles': {'deepseek': {'model': 'd'}, 'minimax': {'model': 'm'}},
                'jobs': [{'id': 'input', 'config': config}]}
    pc.initialize(directory, manifest, {})
    root.mkdir(parents=True)
    fixture.write_text(json.dumps({'observed': 9}))
    required = ['named-input-obligation-resolution', 'fixture-prefix-preservation']
    payload = {'required_checks': required, 'execution': {'target': 'pinned'},
               'artifacts': {'fixture.json': pin(fixture)}, 'adoption_guard': guard,
               'resolution': resolution, 'fixtures': [{'pin': pin(fixture),
                                                       'data': json.loads(fixture.read_text())}]}
    bundle_path = root / 'bundle.json'
    bundle_path.write_text(json.dumps(payload))
    bundle = {'sha256': pin(bundle_path)['sha256'], 'pin': pin(bundle_path), 'required_checks': required}
    review = {'verdict': 'pass', 'bundle_sha256': bundle['sha256'], 'checks': required, 'unknowns': []}
    current = provider_jobs.load(directory)['jobs']['input']
    provider_jobs.update(directory, 'input', {'state': 'ready', 'bundle': bundle,
        'review': review, 'author_model': 'd', 'review_model': 'm'}, expected_revision=current['revision'])
    executor = Executor(directory)
    monkeypatch.setattr(executor, '_execution', lambda: {'target': 'pinned'})
    gates = []

    def accept(target, **options):
        gates.append((target, options))
        ledger.record_path(target).write_text(json.dumps({'lifted': {'status': 'pass'}}))
        return True, {'status': 'pass'}

    monkeypatch.setattr(recover, 'accept', accept)
    return directory, executor, hints, old, gates


def test_adoption_rechecks_review_runs_gate_then_preserves_failure_history(tmp_path, monkeypatch):
    directory, executor, hints, old, gates = make_case(tmp_path, monkeypatch)
    result = adoption.adopt(directory, 'input', executor, directory / 'adoption', deadline=100, clock=lambda: 1)
    assert result['state'] == 'finished'
    assert gates == [(0x401000, {'refresh': False})]
    assert json.loads(hints.read_text())['fixtures'] == [{'old': 7}, {'observed': 9}]
    current = work.read()['functions']['fn_00401000']
    assert current['state'] == 'ready'
    assert current['attempts'] == old['attempts'] and current['failures'] == old['failures']
    assert provider_jobs.load(directory)['jobs']['input']['adoption_receipt']


def test_failed_native_gate_restores_hints_and_retains_hold(tmp_path, monkeypatch):
    directory, executor, hints, old, gates = make_case(tmp_path, monkeypatch)
    before = hints.read_bytes()
    monkeypatch.setattr(recover, 'accept', lambda *a, **k: (False, {'status': 'low-coverage'}))
    with pytest.raises(ValueError, match='Native acceptance failed'):
        adoption.adopt(directory, 'input', executor, directory / 'adoption', deadline=100, clock=lambda: 1)
    assert hints.read_bytes() == before
    assert work.read()['functions']['fn_00401000'] == old
    assert not provider_jobs.load(directory)['jobs']['input'].get('adoption_receipt')
    receipt = json.loads((directory / 'adoption/receipt.json').read_text())
    assert receipt['state'] == 'held'
    assert (directory / 'adoption/native-gate.json').is_file()


def test_existing_native_baseline_keeps_shared_fixture_domain(tmp_path, monkeypatch):
    directory, executor, hints, old, gates = make_case(tmp_path, monkeypatch)
    ledger.record_path(0x401000).write_text(json.dumps({'lifted': {'status': 'pass'}}))
    result = adoption.adopt(directory, 'input', executor, directory / 'adoption', deadline=100, clock=lambda: 1)
    assert result['state'] == 'finished'
    assert gates == [(0x401000, {'refresh': True})]
    current = json.loads(hints.read_text())
    assert current['fixtures'] == [{'old': 7}]
    assert current['c_hints']['fixtures'] == [{'old': 7}, {'observed': 9}]


@pytest.mark.parametrize('change', ['hints', 'hold', 'review', 'lease', 'deadline', 'writer'])
def test_stale_or_owned_input_never_reaches_native_gate(tmp_path, monkeypatch, change):
    directory, executor, hints, old, gates = make_case(tmp_path, monkeypatch)
    if change == 'hints':
        hints.write_text('{}')
    elif change == 'hold':
        state = work.read()
        state['functions']['fn_00401000']['reason'] = 'new obligation'
        work.path().write_text(json.dumps(state))
    elif change == 'review':
        job = provider_jobs.load(directory)['jobs']['input']
        provider_jobs.update(directory, 'input', {'review_model': 'd'}, expected_revision=job['revision'])
    elif change == 'lease':
        monkeypatch.setattr(tasks, 'status', lambda: [{'va': '0x00401000', 'status': 'claimed',
                            'lease': {'expires_at': float('inf')}, 'attempts': []}])
    from contextlib import nullcontext
    from cmre import coordination
    lock = coordination.lock(coordination._path()) if change == 'writer' else nullcontext()
    before = hints.read_bytes()
    with lock, pytest.raises((ValueError, coordination.BusyError)):
        adoption.adopt(directory, 'input', executor, directory / 'adoption',
                       deadline=0 if change == 'deadline' else 100, clock=lambda: 1)
    assert hints.read_bytes() == before
    assert gates == []


def test_resolution_cannot_clear_capability_or_additional_unknown_hold(tmp_path, monkeypatch):
    monkeypatch.setattr(image, 'primary', lambda: SimpleNamespace(read=lambda *a: b'\xc3'))
    resolution = {'reason': 'proof', 'obligations': ['first'], 'unknowns': [],
                  'evidence': [{'va': '0x00401000', 'bytes': 'c3', 'supports': 'return'}]}
    record = {'state': 'evidence-needed', 'reason': 'first', 'additional_holds': [
        {'state': 'evidence-needed', 'reason': 'second'}]}
    with pytest.raises(ValueError, match='every frozen obligation'):
        adoption.validate_resolution({'adoption_guard': {'work': record}}, resolution)
    record['additional_holds'][0]['state'] = 'harness-blocker'
    with pytest.raises(ValueError, match='separate reviewed release'):
        adoption.validate_resolution({'adoption_guard': {'work': record}}, resolution)
