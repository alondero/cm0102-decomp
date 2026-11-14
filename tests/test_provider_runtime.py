"""Proposal readiness is bound to native observations and retained artifacts."""
import gzip
import hashlib
import json
from pathlib import Path

import pytest

from cmre.provider_runtime import Executor, _hash, _pin, prepare_manifest
from cmre import env


def setup_job(tmp_path, monkeypatch, **extra):
    root = tmp_path / 'campaign' / 'jobs' / 'compute'
    root.mkdir(parents=True)
    config = {'directory': str(root), 'function': '0x004d1600', 'mode': 'input',
              'reviewer': 'minimax', 'required_pcs': ['0x004d16ea'], **extra}
    job = {'id': 'compute', 'config': config}
    executor = Executor(root.parent.parent)
    monkeypatch.setattr(executor, '_execution', lambda: {'target': 'pinned'})
    return executor, job, root


def _write_raw(root, *, outcome='return', pcs=(0x4d16ea,), imports=0,
               identity=None, instructions=None, arrivals=None, returns=None):
    raw = {
        'outcome': outcome,
        'covered_pcs': [f'0x{value:x}' for value in pcs],
        'import_count': imports,
        'execution_identity': identity if identity is not None else {'target': 'pinned'},
        'instructions': instructions or [{'va': '0x4d1600', 'mnemonic': 'ret'}],
        'arrivals': arrivals or [{'pc': '0x4d1600'}],
        'returns': returns or [{'pc': '0x4d16ea'}],
    }
    path = root / 'artifacts' / 'raw.json.gz'
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(gzip.compress(json.dumps(raw).encode('utf-8')))
    return _pin(path)


def probe(root, *, outcome='return', pcs=(0x4d16ea,), imports=0, identity=None,
          fixture=True, coverage=.2, raw_pin=None):
    raw = raw_pin or _write_raw(root, outcome=outcome, pcs=pcs, imports=imports,
                                identity=identity)
    artifacts = {'artifacts/raw.json.gz': raw}
    if fixture:
        fixture_path = root / 'artifacts' / 'fixture.json'
        fixture_path.parent.mkdir(parents=True, exist_ok=True)
        fixture_path.write_text(json.dumps({'regs': {}, 'stack': []}), encoding='utf-8')
        artifacts['artifacts/fixture.json'] = _pin(fixture_path)
    return {'a0001': {'id': 'a0001', 'action': {'kind': 'probe'},
                      'result': {'kind': 'probe', 'ok': True,
                                 'result': {'function': '0x4d1600',
                                            'summary': {'outcome': outcome,
                                                        'covered_pcs': [f'0x{value:x}' for value in pcs],
                                                        'coverage': coverage,
                                                        'import_count': imports,
                                                        'execution_identity': {'target': 'pinned'}}},
                                 'artifacts': artifacts}}}


def test_ready_input_freezes_checks_and_raw_backed_observations(tmp_path, monkeypatch):
    executor, job, root = setup_job(tmp_path, monkeypatch)
    result = executor._bundle(job, job['config'], root,
                              {'artifacts': ['a0001'], 'summary': 'native path'},
                              root / 'bundle', probe(root))
    assert result['ready'] is True
    path = Path(result['pin']['path'])
    payload = json.loads(path.read_text())
    assert _hash(path) == result['sha256']
    assert payload['required_checks'] == result['required_checks']
    assert payload['mode'] == 'input'
    assert payload['fixtures'][0]['data'] == {'regs': {}, 'stack': []}
    assert payload['raw'][0]['id'] == 'a0001/artifacts/raw.json.gz'
    assert payload['observations'][0]['returns'] == [{'pc': '0x4d16ea'}]
    assert payload['observations'][0]['summary']['import_count'] == 0


@pytest.mark.parametrize('change', [{'outcome': 'fault'}, {'outcome': 'budget'},
                                   {'pcs': ()}, {'imports': 1}])
def test_native_goal_failure_cannot_be_proposed_as_ready(tmp_path, monkeypatch, change):
    executor, job, root = setup_job(tmp_path, monkeypatch)
    result = executor._bundle(job, job['config'], root,
                              {'artifacts': ['a0001'], 'summary': 'candidate'},
                              root / 'bundle', probe(root, **change))
    assert result['ready'] is False
    assert not (root / 'bundle').exists()


def test_mutable_summary_without_matching_raw_is_rejected(tmp_path, monkeypatch):
    executor, job, root = setup_job(tmp_path, monkeypatch)
    records = probe(root)
    records['a0001']['result']['result']['summary']['outcome'] = 'fault'
    result = executor._bundle(job, job['config'], root,
                              {'artifacts': ['a0001'], 'summary': 'candidate'},
                              root / 'bundle', records)
    assert result['ready'] is False
    assert 'raw outcome' in result['feedback']


def test_missing_raw_or_identity_is_rejected(tmp_path, monkeypatch):
    executor, job, root = setup_job(tmp_path, monkeypatch)
    records = probe(root)
    records['a0001']['result']['artifacts'].pop('artifacts/raw.json.gz')
    result = executor._bundle(job, job['config'], root,
                              {'artifacts': ['a0001'], 'summary': 'candidate'},
                              root / 'bundle', records)
    assert result['ready'] is False
    assert 'raw' in result['feedback']
    records = probe(root, identity={'target': 'other'})
    result = executor._bundle(job, job['config'], root,
                              {'artifacts': ['a0001'], 'summary': 'candidate'},
                              root / 'bundle2', records)
    assert result['ready'] is False
    assert 'Execution identity' in result['feedback']


def test_probe_function_must_match_the_configured_goal(tmp_path, monkeypatch):
    executor, job, root = setup_job(tmp_path, monkeypatch)
    records = probe(root)
    records['a0001']['result']['result']['function'] = '0x004d1700'
    result = executor._bundle(job, job['config'], root,
                              {'artifacts': ['a0001'], 'summary': 'candidate'},
                              root / 'bundle', records)
    assert result['ready'] is False
    assert 'goal function' in result['feedback']


def test_cross_job_artifact_cannot_enter_a_bundle(tmp_path, monkeypatch):
    executor, job, root = setup_job(tmp_path, monkeypatch)
    records = probe(root)
    other = root.parent / 'other.txt'
    other.write_text('another job')
    records['a0001']['result']['artifacts']['artifacts/raw.json.gz'] = _pin(other)
    with pytest.raises(ValueError, match='escapes'):
        executor._bundle(job, job['config'], root,
                         {'artifacts': ['a0001'], 'summary': 'candidate'}, root / 'bundle', records)


def test_prior_record_ref_sha_is_verified(tmp_path, monkeypatch):
    executor, job, root = setup_job(tmp_path, monkeypatch)
    record = {'id': 'a0001', 'action': {'kind': 'probe'}, 'result': {}}
    path = root / 'a0001.json'
    path.write_text(json.dumps(record), encoding='utf-8')
    ref = {'id': 'a0001', 'path': 'jobs/compute/a0001.json', 'sha256': '0' * 64}
    with pytest.raises(ValueError, match='hash changed'):
        executor._records(root, [ref])
    ref['sha256'] = _hash(path)
    assert set(executor._records(root, [ref])) == {'a0001'}


def test_records_include_all_durable_actions_not_just_the_context_window(tmp_path, monkeypatch):
    executor, job, root = setup_job(tmp_path, monkeypatch)
    for action_id in ('a0001', 'a0002'):
        (root / f'{action_id}.json').write_text(
            json.dumps({'id': action_id, 'action': {'kind': 'probe'}, 'result': {}}),
            encoding='utf-8')
    ref = {'id': 'a0002', 'path': 'jobs/compute/a0002.json'}
    records = executor._records(root, [ref])
    assert set(records) == {'a0001', 'a0002'}


def test_broker_response_exposes_prefixed_artifact_ids(tmp_path, monkeypatch):
    executor, job, root = setup_job(tmp_path, monkeypatch)
    output = root / 'rounds' / '1' / 'a0003-output.json'
    pin = _pin(root / 'existing.json') if (root / 'existing.json').exists() else None
    target = root / 'existing.json'
    target.write_text('{}', encoding='utf-8')
    pin = _pin(target)

    def broker(config, action, out, *, prior=None, candidate=None):
        return {'kind': action['kind'], 'ok': True,
                'artifacts': {'artifacts/instructions.json': pin}}

    executor.broker = broker
    response = executor(job, {'kind': 'evidence', 'function': '0x4d1600',
                              'start': 0, 'limit': 10}, output)
    assert response['artifact_ids'] == ['a0003/artifacts/instructions.json']


def test_manifest_does_not_touch_existing_c_or_start_a_clock(tmp_path, monkeypatch):
    analysis = tmp_path / 'analysis'
    analysis.mkdir()
    source = tmp_path / 'c'
    source.mkdir()
    monkeypatch.setattr(env, 'ANALYSIS', analysis)
    monkeypatch.setattr(env, 'C_DIR', source)
    seed = tmp_path / 'seed.json'
    seed.write_text('{}')
    manifest = {'jobs': [{'id': 'compute', 'config': {'function': '0x004d1600', 'mode': 'input',
                                                   'seed_fixture': _pin(seed)}}]}
    target = analysis / 'campaign'
    result = prepare_manifest(target, manifest)
    assert result['jobs'][0]['config']['directory'] == str(target / 'jobs' / 'compute')
    assert not target.exists()
    manifest['jobs'][0]['config']['mode'] = 'c'
    (source / 'fn_004d1600.c').write_text('pre-existing source')
    with pytest.raises(ValueError, match='Existing canonical'):
        prepare_manifest(target, manifest)
    assert (source / 'fn_004d1600.c').read_text() == 'pre-existing source'


@pytest.mark.parametrize('return_kind,callers,blocker', [
    ('int', {}, None),
    ('void', {}, 'return-provenance-needed'),
    ('void', {0x401000: [0x004d1600]}, None),
    (None, {}, 'return-provenance-needed'),
    ('unrecognized', {}, 'return-provenance-needed'),
])
def test_c_bundle_requires_current_source_and_distinct_complete_o0_o2(
        tmp_path, monkeypatch, return_kind, callers, blocker):
    from types import SimpleNamespace
    from cmre import inventory
    monkeypatch.setattr(inventory, 'load', lambda: SimpleNamespace(callees=callers))
    executor, job, root = setup_job(tmp_path, monkeypatch)
    job['config']['mode'] = 'c'
    source = root / 'fn.c'
    source.write_text('int f(void) { return 1; }\n', encoding='utf-8')
    job['config']['candidate'] = str(source)
    checks = root / 'artifacts'
    checks.mkdir()
    o0 = checks / 'O0.disasm.txt'
    o2 = checks / 'O2.disasm.txt'
    o0.write_text('O0 text\n')
    o2.write_text('O2 text\n')
    report = checks / 'report.json'
    source_sha = _hash(source)
    report.write_text(json.dumps({'status': 'pass', 'returns': return_kind,
                                 'source_sha256': source_sha, 'va': job['config']['function']}))
    artifacts = {'artifacts/build/O0.disasm.txt': _pin(o0),
                 'artifacts/build/O2.disasm.txt': _pin(o2),
                 'artifacts/report.json': _pin(report)}
    source_sha = _hash(source)
    selected = [
        {'id': 'a0001', 'action': {'kind': 'build'},
         'result': {'kind': 'build', 'ok': True,
                    'result': {'build': {'source_sha256': source_sha}},
                    'artifacts': {}}},
        {'id': 'a0002', 'action': {'kind': 'test'},
         'result': {'kind': 'test', 'ok': True, 'source_sha256': source_sha,
                    'report_status': 'pass', 'task_id': 't1', 'attempt': 1,
                    'input_fingerprint': 'fp', 'report': _pin(report),
                    'artifacts': {}}},
    ]
    payload = {'job': job['id'], 'function': job['config']['function'], 'mode': 'c',
               'summary': 'candidate', 'artifacts': artifacts, 'execution': {'target': 'pinned'},
               'records': selected}
    result = executor._c_bundle(payload, job['config'], selected, artifacts, root)
    old_report = checks / 'old-report.json'
    old_report.write_text(json.dumps({'status': 'pass', 'returns': 'int',
                                     'source_sha256': '0' * 64, 'va': job['config']['function']}))
    mixed = {'older/report.json': _pin(old_report), **artifacts}
    mixed_result = executor._c_bundle(payload, job['config'], selected, mixed, root)
    stale_only = {**artifacts, 'artifacts/report.json': _pin(old_report)}
    assert executor._c_bundle(payload, job['config'], selected, stale_only, root)['ready'] is False
    if blocker:
        assert result['blocker'] == blocker
        assert mixed_result['blocker'] == blocker
        return
    assert result is None
    assert mixed_result is None
    assert set(payload['emitted']) == {'artifacts/build/O0.disasm.txt',
                                       'artifacts/build/O2.disasm.txt'}
    assert payload['task']['attempt'] == 1
    missing_report = {key: pin for key, pin in artifacts.items() if not key.endswith('/report.json')}
    assert executor._c_bundle(payload, job['config'], selected, missing_report, root)['ready'] is False
    job['config']['require_controls'] = True
    stale = {'id': 'a0003', 'action': {'kind': 'control-plan'}, 'result': {
        'kind': 'control-plan', 'ok': True, 'source_sha256': '0' * 64}}
    refused = executor._c_bundle(payload, job['config'], [*selected, stale], artifacts, root)
    assert refused['blocker'] == 'controls-required'
    job['config']['require_controls'] = False
    o2.write_text('O0 text\n')
    artifacts['artifacts/build/O2.disasm.txt'] = _pin(o2)
    assert executor._c_bundle(payload, job['config'], selected, artifacts, root) is not None
