import hashlib
import json

import pytest

from cmre import coordination, provider_jobs, provider_queue


def precondition_store(tmp_path, monkeypatch):
    from cmre import env, provider_campaign as pc
    monkeypatch.setenv('CMRE_COORDINATION_PATH', str(tmp_path / 'command.lock'))
    monkeypatch.delenv('CMRE_WRITER_TOKEN', raising=False)
    monkeypatch.delenv('CMRE_WRITER_PATH', raising=False)
    directory = tmp_path / 'run'
    source = directory / 'jobs/held/source.c'
    spec = {'schema': pc.SCHEMA, 'duration_seconds': 3600,
            'roles': {'deepseek': {'model': 'd'}, 'minimax': {'model': 'm'}},
            'jobs': [{'id': 'held', 'config': {'function': '0x00401000', 'mode': 'c',
                     'goal': 'Recover', 'candidate': str(source), 'self_service_controls': True}}]}
    pc.initialize(directory, spec, {'controller': {
        'cmre/provider_runtime.py': env.digest(env.ROOT / 'cmre/provider_runtime.py')}})
    source.parent.mkdir(parents=True)
    source.write_text('exact source')
    job = provider_jobs.load(directory)['jobs']['held']
    step = {'action_id': 'a0001', 'phase': 'owner', 'role': 'minimax', 'model': 'm',
            'action': {'kind': 'control-plan', 'controls': []},
            'actor_response': {'source': {'sha256': hashlib.sha256(source.read_bytes()).hexdigest()}},
            'output_rel': 'jobs/held/rounds/1/a0001-output'}
    _, ref = pc._write_record(directory, job, step, ok=False,
                             error=provider_queue.CONTROLS_PRECONDITION)
    provider_jobs.update(directory, 'held', {'state': 'held', 'attempts': 1, 'action_count': 1,
        'round_actions': 1, 'results': [ref], 'pending_step': step,
        'hold_reason': provider_queue.CONTROLS_PRECONDITION,
        'escalation': {'adapter': 'execute'}}, expected_revision=0)
    pc._ensure_run(directory, provider_jobs.load(directory), lambda: 100)
    return directory, source


def test_controls_ordering_reopen_preserves_counters_source_and_deadline(tmp_path, monkeypatch):
    directory, source = precondition_store(tmp_path, monkeypatch)
    before = provider_jobs.load(directory)['jobs']['held']
    timing = (directory / 'run.json').read_bytes()
    plan = provider_queue.retry_precondition(directory, 'held', reason='Proven no-side-effect refusal', clock=lambda: 200)
    assert not plan['apply']
    assert provider_jobs.load(directory)['jobs']['held'] == before
    result = provider_queue.retry_precondition(directory, 'held', reason='Proven no-side-effect refusal',
                                                apply=True, clock=lambda: 200)
    after = provider_jobs.load(directory)['jobs']['held']
    assert result['state'] == 'finished' and after['state'] == 'prepared'
    for field in ('attempts', 'action_count', 'round_actions', 'results', 'config'):
        assert after[field] == before[field]
    assert source.read_text() == 'exact source'
    assert (directory / 'run.json').read_bytes() == timing
    assert after['pending_step'] is None


@pytest.mark.parametrize('condition', ['source', 'output', 'service-hold', 'deadline'])
def test_uncertain_or_expired_controls_refusal_cannot_reopen(tmp_path, monkeypatch, condition):
    directory, source = precondition_store(tmp_path, monkeypatch)
    if condition == 'source':
        source.write_text('different source')
    elif condition == 'output':
        (directory / 'jobs/held/rounds/1/a0001-output').mkdir(parents=True)
    elif condition == 'service-hold':
        job = provider_jobs.load(directory)['jobs']['held']
        provider_jobs.update(directory, 'held', {'pipeline_hold': {'reason': 'Uncertain operation'}},
                             expected_revision=job['revision'])
    with pytest.raises(ValueError):
        provider_queue.retry_precondition(directory, 'held', reason='Inspect refusal', apply=True,
                                         clock=lambda: 5000 if condition == 'deadline' else 200)
    assert provider_jobs.load(directory)['jobs']['held']['state'] == 'held'


def store(tmp_path, monkeypatch):
    monkeypatch.setenv('CMRE_COORDINATION_PATH', str(tmp_path / 'command.lock'))
    monkeypatch.delenv('CMRE_WRITER_TOKEN', raising=False)
    monkeypatch.delenv('CMRE_WRITER_PATH', raising=False)
    directory = tmp_path / 'run'
    provider_jobs.create(directory, {'jobs': [
        {'id': name, 'config': {'function': '0x00401000', 'mode': mode}}
        for name, mode in [('fresh', 'c'), ('paid', 'c'), ('leased', 'c'), ('upstream', 'input')]
    ]}, {})
    provider_jobs.update(directory, 'paid', {'attempts': 1}, expected_revision=0)
    lease = directory / 'jobs/leased/task-lease.json'
    lease.parent.mkdir(parents=True)
    lease.write_text('{}')
    return directory


def test_plan_is_read_only_and_excludes_paid_owned_and_upstream_jobs(tmp_path, monkeypatch):
    directory = store(tmp_path, monkeypatch)
    before = (directory / 'state.json').read_bytes()
    result = provider_queue.cancel_unstarted(directory, reason='Finish started cohort')
    assert [row['job'] for row in result['selected']] == ['fresh']
    assert (directory / 'state.json').read_bytes() == before
    assert not (directory / 'scope-changes').exists()


def test_apply_retains_candidate_and_attempts_with_hash_chained_cancellation(tmp_path, monkeypatch):
    directory = store(tmp_path, monkeypatch)
    candidate = directory / 'jobs/fresh/candidate.c'
    candidate.parent.mkdir(parents=True)
    candidate.write_bytes(b'keep exact private source')
    result = provider_queue.cancel_unstarted(directory, reason='Finish started cohort', apply=True)
    state = provider_jobs.load(directory)
    assert state['jobs']['fresh']['state'] == 'cancelled'
    assert state['jobs']['paid']['attempts'] == 1
    assert state['jobs']['upstream']['state'] == 'prepared'
    assert state['jobs']['leased']['state'] == 'prepared'
    assert candidate.read_bytes() == b'keep exact private source'
    assert result['selected'][0]['candidate']['sha256']
    with open(result['receipt']) as stream:
        receipt = json.load(stream)
    assert receipt['state'] == 'finished' and receipt['cancelled'] == ['fresh']


def test_live_run_lock_prevents_cancellation(tmp_path, monkeypatch):
    directory = store(tmp_path, monkeypatch)
    with coordination.lock(directory / 'run.lock'):
        with pytest.raises(coordination.BusyError):
            provider_queue.cancel_unstarted(directory, reason='Finish cohort', apply=True)
    assert provider_jobs.load(directory)['jobs']['fresh']['state'] == 'prepared'


def test_prepared_service_hold_is_retained_as_uncertain_work(tmp_path, monkeypatch):
    directory = store(tmp_path, monkeypatch)
    provider_jobs.update(directory, 'fresh', {'pipeline_hold': {'reason': 'Interrupted initialization'}},
                         expected_revision=0)
    result = provider_queue.cancel_unstarted(directory, reason='Finish started work', apply=True)
    job = provider_jobs.load(directory)['jobs']['fresh']
    assert result['cancelled'] == []
    assert job['state'] == 'prepared'
    assert job['pipeline_hold']['reason'] == 'Interrupted initialization'
