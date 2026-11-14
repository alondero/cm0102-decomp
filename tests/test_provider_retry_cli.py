"""Fake-data tests for the safe held-job retry preparation.

Every fixture is synthetic: no provider, no network, no inference.  The tests
prove that a clean terminal actor failure can be prepared again, that every
unsafe or uncertain situation is refused, that counters, action references and
the original deadline survive, and that the official command line refuses
more than two workers.
"""
from __future__ import annotations

import inspect
import json
from pathlib import Path
import sys
import time

import pytest

ROOT = Path(__file__).resolve().parents[1]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from cmre import provider_campaign as campaign  # noqa: E402
from cmre import provider_jobs  # noqa: E402
from cmre import provider_retry  # noqa: E402
from cmre.coordination import BusyError, lock  # noqa: E402
import scripts.cmre_provider_campaign as cli  # noqa: E402

JOB_ID = 'job1'
OWNER_MODEL = 'deepseek/model'
REVIEW_MODEL = 'minimax/model'
STARTED = 1000.0
DURATION = 600.0
DEADLINE = STARTED + DURATION
ACTOR_ERROR = 'actor error: RuntimeError: provider unavailable'


def _manifest(concurrency=1, duration=DURATION):
    return {
        'schema': 'cmre-provider-campaign/1',
        'duration_seconds': duration,
        'concurrency': concurrency,
        'roles': {'deepseek': {'model': OWNER_MODEL},
                  'minimax': {'model': REVIEW_MODEL}},
        'jobs': [{'id': JOB_ID, 'config': {
            'function': '0x004d1600', 'mode': 'input', 'goal': 'fake goal',
            'owner': 'deepseek', 'reviewer': 'minimax'}}],
    }


def _identity():
    return {'execution': {'fake': True}, 'controller': {}, 'profiles': {},
            'parent_sha256': '0' * 64}


def _write_run(directory, started=STARTED):
    state = provider_jobs.load(directory)
    payload = campaign._run_payload(state, started)
    payload['hash'] = campaign._run_hash(payload)
    (directory / 'run.json').write_text(json.dumps(payload, indent=2) + '\n',
                                        encoding='utf-8')
    return payload


def _campaign(tmp_path, identity=None, concurrency=1):
    directory = tmp_path / 'campaign'
    campaign.initialize(directory, _manifest(concurrency=concurrency),
                        identity if identity is not None else _identity())
    _write_run(directory)
    return directory


def _worker_receipt(model, observed, out_of_scope, pin_drift, *,
                    state='incomplete', observed_present=True):
    receipt = {
        'schema': 'cmre-provider-worker/1',
        'state': state,
        'started': 1.0,
        'ended': 2.0,
        'model_requested': model,
        'out_of_scope': list(out_of_scope),
        'pin_drift': list(pin_drift),
    }
    if observed_present:
        receipt['models_observed'] = list(observed)
    return receipt


def _write_receipts(directory, stage_rel, model, observed, out_of_scope, pin_drift,
                    *, state='incomplete', observed_present=True):
    stage_path = directory / stage_rel
    stage_path.mkdir(parents=True, exist_ok=True)
    (stage_path / 'context.json').write_text('{}\n', encoding='utf-8')
    payloads = {
        'smoke': _worker_receipt(model, [model], (), (), state='completed'),
        'work': _worker_receipt(model, observed, out_of_scope, pin_drift,
                               state=state, observed_present=observed_present),
    }
    paths = {}
    for name, worker in payloads.items():
        receipt = stage_path / name / 'receipt.json'
        receipt.parent.mkdir(parents=True, exist_ok=True)
        receipt.write_text(json.dumps(worker, indent=2) + '\n', encoding='utf-8')
        paths[name] = receipt
    pair = {
        'schema': 'cmre-provider-pair/1',
        'state': state,
        'started': 1.0,
        'ended': 2.0,
        'current_stage': 'work',
        'stages': [{'stage': name, 'state': worker['state'],
                    'receipt': str(paths[name])}
                   for name, worker in payloads.items()],
    }
    (stage_path / 'receipt.json').write_text(json.dumps(pair, indent=2) + '\n',
                                             encoding='utf-8')
    return paths


def _hold(directory, *, phase='owner', action_id='a0001', adapter='actor',
          retryable=True, kind=None, model=None, observed=None, out_of_scope=(),
          pin_drift=(), receipts=True, stage=None, escalation_checkpoint=None,
          checkpoint=None, bundle=None, error=ACTOR_ERROR, state='incomplete',
          observed_present=True):
    """Publish a held job that carries one clean terminal actor failure."""
    role = 'deepseek' if phase == 'owner' else 'minimax'
    model = model or (OWNER_MODEL if role == 'deepseek' else REVIEW_MODEL)
    observed = list(observed) if observed is not None else [model]
    stage_rel = stage or f'jobs/{JOB_ID}/rounds/1/{action_id}-{phase}'
    record = {
        'id': action_id, 'phase': phase, 'round': 1, 'role': role, 'model': None,
        'kind': kind, 'action': None, 'response': None, 'result': None,
        'ok': False, 'output_path': f'jobs/{JOB_ID}/rounds/1/{action_id}-output',
        'actor_response': None, 'error': error,
    }
    record['hash'] = campaign._record_hash(record)
    record_path = directory / 'jobs' / JOB_ID / f'{action_id}.json'
    record_path.parent.mkdir(parents=True, exist_ok=True)
    record_path.write_text(json.dumps(record, indent=2) + '\n', encoding='utf-8')
    ref = {'id': action_id, 'kind': kind, 'phase': phase, 'ok': False,
           'path': f'jobs/{JOB_ID}/{action_id}.json', 'hash': record['hash'],
           'summary': 'actor failure'}
    failure = {'action': action_id, 'stage': stage_rel, 'phase': phase,
               'role': role, 'error': error, 'retryable': retryable}
    escalation = {'reason': error, 'adapter': adapter, 'stage': stage_rel}
    if escalation_checkpoint is not None:
        escalation['checkpoint'] = escalation_checkpoint
    changes = {
        'state': 'held',
        'phase': phase,
        'hold_reason': error,
        'escalation': escalation,
        'last_failure': failure,
        'failures': [failure],
        'results': [ref],
        'action_count': 1,
        'attempts': 1,
        'round': 1,
        'round_actions': 1,
        'checkpoint': checkpoint,
    }
    if bundle is not None:
        changes['bundle'] = bundle
    provider_jobs.update(directory, JOB_ID, changes, expected_revision=0)
    if receipts:
        _write_receipts(directory, stage_rel, model, observed, out_of_scope,
                        pin_drift, state=state,
                        observed_present=observed_present)
    return {'record': record, 'ref': ref, 'failure': failure}


def _retry(directory, reason='operator retry', identity=None,
           clock=lambda: STARTED):
    return provider_retry.retry_held(
        directory, JOB_ID, reason,
        identity if identity is not None else _identity(), clock=clock)


def _profiles(tmp_path):
    path = tmp_path / 'profiles.json'
    path.write_text(json.dumps({
        'deepseek': {'model': OWNER_MODEL, 'executable': str(tmp_path / 'opencode'),
                     'client_version': '1.18.3', 'environment': {}},
        'minimax': {'model': REVIEW_MODEL, 'executable': str(tmp_path / 'opencode2'),
                    'client_version': '1.18.3', 'environment': {}},
    }, indent=2), encoding='utf-8')
    return path


def _parent(tmp_path):
    path = tmp_path / 'parent.json'
    path.write_text(json.dumps({'schema': 'cmre-provider-parent/1',
                                'state': 'finished',
                                'steps': [{'state': 'finished', 'exit_code': 0}]}),
                    encoding='utf-8')
    return path


def test_owner_retry_prepares_the_job_and_preserves_the_evidence(tmp_path):
    directory = _campaign(tmp_path)
    fixture = _hold(directory)
    run_bytes = (directory / 'run.json').read_bytes()

    result = _retry(directory)

    assert result['state'] == 'prepared'
    assert result['phase'] == 'owner'
    job = provider_jobs.load(directory)['jobs'][JOB_ID]
    assert job['state'] == 'prepared'
    assert job['phase'] == 'owner'
    assert job['retry_reason'] == 'operator retry'
    assert job['hold_reason'] is None
    assert job['escalation'] is None
    assert job['action_count'] == 1
    assert job['attempts'] == 1
    assert job['round'] == 1
    assert job['round_actions'] == 1
    assert job['results'] == [fixture['ref']]
    assert job['failures'] == [fixture['failure']]
    assert job['last_failure'] == fixture['failure']
    assert (directory / 'run.json').read_bytes() == run_bytes


def test_review_retry_reopens_the_reviewer_in_a_single_event(tmp_path):
    directory = _campaign(tmp_path)
    bundle = {'ready': True, 'sha256': 'b' * 64, 'required_checks': ['raw']}
    _hold(directory, phase='review', bundle=bundle)

    result = _retry(directory)

    assert result['state'] == 'needs-review'
    assert result['phase'] == 'review'
    job = provider_jobs.load(directory)['jobs'][JOB_ID]
    assert job['state'] == 'needs-review'
    assert job['phase'] == 'review'
    assert job['bundle'] == bundle
    assert job['action_count'] == 1
    assert job['attempts'] == 1
    events = sorted((directory / 'events').iterdir())
    assert len(events) == 2
    states = [json.loads(item.read_text(encoding='utf-8'))['changes'].get('state')
              for item in events]
    assert states == ['held', 'needs-review']


def test_expired_deadline_is_refused(tmp_path):
    directory = _campaign(tmp_path)
    _hold(directory)
    with pytest.raises(provider_retry.RetryRefused):
        _retry(directory, clock=lambda: DEADLINE)
    assert provider_jobs.load(directory)['jobs'][JOB_ID]['state'] == 'held'


def test_missing_run_metadata_is_refused(tmp_path):
    directory = _campaign(tmp_path)
    _hold(directory)
    (directory / 'run.json').unlink()
    with pytest.raises(provider_retry.RetryRefused):
        _retry(directory)


def test_identity_mismatch_is_refused(tmp_path):
    directory = _campaign(tmp_path)
    _hold(directory)
    with pytest.raises(provider_retry.RetryRefused):
        _retry(directory, identity={'execution': {'fake': False}})
    assert provider_jobs.load(directory)['jobs'][JOB_ID]['state'] == 'held'


def test_unknown_job_is_refused(tmp_path):
    directory = _campaign(tmp_path)
    _hold(directory)
    with pytest.raises(provider_retry.RetryRefused):
        provider_retry.retry_held(directory, 'missing', 'operator retry',
                                  _identity(), clock=lambda: STARTED)


def test_empty_reason_is_refused(tmp_path):
    directory = _campaign(tmp_path)
    _hold(directory)
    with pytest.raises(provider_retry.RetryRefused):
        _retry(directory, reason='   ')


def test_active_checkpoint_is_refused(tmp_path):
    directory = _campaign(tmp_path)
    _hold(directory, checkpoint={'stage': 'jobs/job1/rounds/1/a0001-owner'})
    with pytest.raises(provider_retry.RetryRefused):
        _retry(directory)


def test_missing_receipts_are_refused(tmp_path):
    directory = _campaign(tmp_path)
    _hold(directory, receipts=False)
    with pytest.raises(provider_retry.RetryRefused):
        _retry(directory)


def test_missing_stage_receipt_is_refused(tmp_path):
    directory = _campaign(tmp_path)
    _hold(directory)
    (directory / 'jobs' / JOB_ID / 'rounds' / '1' / 'a0001-owner' / 'work'
     / 'receipt.json').unlink()
    with pytest.raises(provider_retry.RetryRefused):
        _retry(directory)


def test_unterminated_pair_receipt_is_refused(tmp_path):
    directory = _campaign(tmp_path)
    _hold(directory)
    pair = directory / 'jobs' / JOB_ID / 'rounds' / '1' / 'a0001-owner' / 'receipt.json'
    payload = json.loads(pair.read_text(encoding='utf-8'))
    payload.pop('ended')
    pair.write_text(json.dumps(payload, indent=2), encoding='utf-8')
    with pytest.raises(provider_retry.RetryRefused):
        _retry(directory)


def test_interrupted_hold_is_refused(tmp_path):
    directory = _campaign(tmp_path)
    _hold(directory, adapter='controller',
          escalation_checkpoint={'stage': 'jobs/job1/rounds/1/a0001-owner'})
    with pytest.raises(provider_retry.RetryRefused):
        _retry(directory)


def test_controller_and_executor_holds_are_refused(tmp_path):
    for adapter in ('controller', 'execute'):
        directory = _campaign(tmp_path / adapter)
        _hold(directory, adapter=adapter)
        with pytest.raises(provider_retry.RetryRefused):
            _retry(directory)


def test_deliberate_review_hold_is_refused(tmp_path):
    directory = _campaign(tmp_path)
    _hold(directory, phase='review', kind='review',
          bundle={'ready': True, 'sha256': 'b' * 64, 'required_checks': ['raw']})
    with pytest.raises(provider_retry.RetryRefused):
        _retry(directory)


def test_foreign_stage_is_refused(tmp_path):
    directory = _campaign(tmp_path)
    _hold(directory, stage='jobs/other-job/rounds/1/a0001-owner')
    with pytest.raises(provider_retry.RetryRefused):
        _retry(directory)


def test_dirty_stage_is_refused(tmp_path):
    directory = _campaign(tmp_path)
    _hold(directory, out_of_scope=['packet/action.json'])
    with pytest.raises(provider_retry.RetryRefused):
        _retry(directory)


def test_pinned_source_drift_is_refused(tmp_path):
    directory = _campaign(tmp_path)
    _hold(directory, pin_drift=['CM0102/Analysis/cmre/provider_campaign.py'])
    with pytest.raises(provider_retry.RetryRefused):
        _retry(directory)


def test_foreign_model_stage_is_refused(tmp_path):
    directory = _campaign(tmp_path)
    _hold(directory, observed=[OWNER_MODEL, REVIEW_MODEL])
    with pytest.raises(provider_retry.RetryRefused):
        _retry(directory)


def test_foreign_requested_model_is_refused(tmp_path):
    directory = _campaign(tmp_path)
    _hold(directory, model=REVIEW_MODEL, observed=[REVIEW_MODEL])
    with pytest.raises(provider_retry.RetryRefused):
        _retry(directory)


@pytest.mark.parametrize('state', ('completed', 'timeout', 'step_limit',
                                   'incomplete'))
def test_real_terminal_actor_failure_states_are_retried(tmp_path, state):
    directory = _campaign(tmp_path)
    _hold(directory, state=state)
    result = _retry(directory)
    assert result['state'] == 'prepared'
    assert result['phase'] == 'owner'


@pytest.mark.parametrize('observed_present', (False, True))
def test_timeout_without_observed_model_is_retried(tmp_path, observed_present):
    directory = _campaign(tmp_path)
    _hold(directory, state='timeout', observed=[],
          observed_present=observed_present)
    result = _retry(directory)
    assert result['state'] == 'prepared'


def test_timeout_with_foreign_observed_model_is_refused(tmp_path):
    directory = _campaign(tmp_path)
    _hold(directory, state='timeout', observed=[OWNER_MODEL, REVIEW_MODEL])
    with pytest.raises(provider_retry.RetryRefused):
        _retry(directory)


@pytest.mark.parametrize('state', ('starting', 'running', 'cancelled',
                                   'controller_error', 'finalization_error',
                                   'session_identity_error', 'model_mismatch',
                                   'export_failed', 'scope_violation',
                                   'input_drift', 'readiness_failed',
                                   'orphaned-needs-reconciliation', 'failed',
                                   'error', 'aborted'))
def test_non_clean_or_unterminated_receipt_states_are_refused(tmp_path, state):
    directory = _campaign(tmp_path)
    _hold(directory, state=state)
    with pytest.raises(provider_retry.RetryRefused):
        _retry(directory)
    assert provider_jobs.load(directory)['jobs'][JOB_ID]['state'] == 'held'


def test_terminal_receipt_states_are_the_worker_terminal_states():
    assert provider_retry.TERMINAL_RECEIPT_STATES == frozenset(
        {'completed', 'timeout', 'step_limit', 'incomplete'})


def test_stale_failure_is_refused(tmp_path):
    directory = _campaign(tmp_path)
    fixture = _hold(directory)
    later = dict(fixture['ref'], id='a0002', ok=True)
    provider_jobs.update(directory, JOB_ID, {'results': [fixture['ref'], later]},
                         expected_revision=1)
    with pytest.raises(provider_retry.RetryRefused):
        _retry(directory)


def test_non_retryable_failure_is_refused(tmp_path):
    directory = _campaign(tmp_path)
    _hold(directory, retryable=False)
    with pytest.raises(provider_retry.RetryRefused):
        _retry(directory)


def test_active_run_lock_is_refused(tmp_path):
    directory = _campaign(tmp_path)
    _hold(directory)
    with lock(directory / 'run.lock'):
        with pytest.raises(BusyError):
            _retry(directory)
    assert provider_jobs.load(directory)['jobs'][JOB_ID]['state'] == 'held'


def test_cli_retry_prepares_the_held_job(tmp_path, monkeypatch):
    directory = _campaign(tmp_path)
    _hold(directory)
    _write_run(directory, time.time())
    monkeypatch.setattr(cli, 'identity', lambda profiles, parent: _identity())

    code = cli.main(['retry', str(directory), '--job', JOB_ID,
                      '--reason', 'operator retry',
                      '--profiles', str(_profiles(tmp_path)),
                      '--parent', str(_parent(tmp_path))])

    assert code == 0
    job = provider_jobs.load(directory)['jobs'][JOB_ID]
    assert job['state'] == 'prepared'
    assert job['phase'] == 'owner'
    assert job['retry_reason'] == 'operator retry'


def test_cli_retry_reports_a_refusal(tmp_path, monkeypatch):
    directory = _campaign(tmp_path)
    _write_run(directory, time.time())
    monkeypatch.setattr(cli, 'identity', lambda profiles, parent: _identity())
    with pytest.raises(provider_retry.RetryRefused):
        cli.main(['retry', str(directory), '--job', JOB_ID, '--reason', 'again',
                  '--profiles', str(_profiles(tmp_path)),
                  '--parent', str(_parent(tmp_path))])


def test_cli_refuses_more_than_two_workers(tmp_path, monkeypatch):
    monkeypatch.setattr(cli, 'identity', lambda profiles, parent: _identity())
    profiles_path = _profiles(tmp_path)
    parent_path = _parent(tmp_path)
    for concurrency in (3, 4):
        manifest_path = tmp_path / f'manifest-{concurrency}.json'
        manifest_path.write_text(json.dumps(_manifest(concurrency=concurrency),
                                            indent=2), encoding='utf-8')
        directory = tmp_path / f'campaign-{concurrency}'
        with pytest.raises(ValueError):
            cli.main(['init', str(directory), '--manifest', str(manifest_path),
                      '--profiles', str(profiles_path), '--parent', str(parent_path)])
        assert not directory.exists()


def test_cli_run_refuses_more_than_two_workers(tmp_path, monkeypatch):
    monkeypatch.setattr(cli, 'identity', lambda profiles, parent: _identity())
    directory = _campaign(tmp_path, concurrency=3)
    revision = provider_jobs.load(directory)['revision']
    with pytest.raises(ValueError):
        cli.main(['run', str(directory), '--profiles', str(_profiles(tmp_path)),
                  '--parent', str(_parent(tmp_path))])
    assert provider_jobs.load(directory)['revision'] == revision


def test_library_still_allows_one_to_four_workers(tmp_path):
    directory = tmp_path / 'campaign'
    campaign.initialize(directory, _manifest(concurrency=4), _identity())
    assert provider_jobs.load(directory)['manifest']['concurrency'] == 4


def test_store_held_reopen_to_needs_review_is_narrow(tmp_path):
    directory = tmp_path / 'campaign'
    campaign.initialize(directory, _manifest(), _identity())
    provider_jobs.update(directory, JOB_ID, {'state': 'held', 'phase': 'owner'},
                         expected_revision=0)
    with pytest.raises(provider_jobs.InvalidChange):
        provider_jobs.update(directory, JOB_ID,
                             {'state': 'needs-review', 'retry_reason': 'again'},
                             expected_revision=1)
    provider_jobs.update(directory, JOB_ID, {'phase': 'review'}, expected_revision=1)
    with pytest.raises(provider_jobs.InvalidChange):
        provider_jobs.update(directory, JOB_ID,
                             {'state': 'needs-review', 'retry_reason': 'again'},
                             expected_revision=2)
    provider_jobs.update(directory, JOB_ID, {'bundle': {'sha256': 'c' * 64}},
                         expected_revision=2)
    with pytest.raises(provider_jobs.InvalidChange):
        provider_jobs.update(directory, JOB_ID,
                             {'state': 'needs-review', 'retry_reason': 'again',
                              'attempts': 0}, expected_revision=3)
    with pytest.raises(provider_jobs.InvalidChange):
        provider_jobs.update(directory, JOB_ID, {'state': 'needs-review'},
                             expected_revision=3)
    reopened = provider_jobs.update(directory, JOB_ID,
                                    {'state': 'needs-review', 'retry_reason': 'again'},
                                    expected_revision=3)
    assert reopened['state'] == 'needs-review'
    assert reopened['bundle'] == {'sha256': 'c' * 64}


def test_store_terminal_states_stay_closed(tmp_path):
    directory = tmp_path / 'campaign'
    manifest = _manifest()
    manifest['jobs'] = [dict(manifest['jobs'][0], id=f'job{index}')
                        for index in range(1, 5)]
    campaign.initialize(directory, manifest, _identity())
    for index, terminal in enumerate(('ready', 'accepted', 'exhausted', 'cancelled')):
        job_id = f'job{index + 1}'
        provider_jobs.update(directory, job_id, {'state': terminal},
                             expected_revision=0)
        with pytest.raises(provider_jobs.InvalidChange):
            provider_jobs.update(directory, job_id,
                                 {'state': 'needs-review', 'retry_reason': 'again'},
                                 expected_revision=1)


def test_controller_identity_covers_the_retry_module():
    # The real identity also hashes the game target, which is absent from
    # this workspace, so the controller module list is checked here.
    assert "'provider_retry'" in inspect.getsource(cli.identity)
    assert (ROOT / 'cmre' / 'provider_retry.py').is_file()
