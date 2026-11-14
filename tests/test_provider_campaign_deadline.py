"""Public regression coverage for the provider campaign deadline boundary.

Ported from the reviewed private replay module (thirteen cases) into a
standalone public test module that ordinary pytest collects.  Every case
builds its own campaign store under pytest's ``tmp_path`` and drives the
real production modules ``cmre.provider_campaign``, ``cmre.provider_jobs``,
``cmre.pipeline_controller`` and ``cmre.pipeline_policy``.

Boundaries kept from the reviewed private evidence:

* the actor and the executor are injected offline callables; no provider,
  broker, network, engine or game code runs;
* preparation, adoption and inventory snapshots raise when they are reached;
* elapsed time is logical: the test clock advances seconds and never sleeps
  on the wall clock, so the whole module runs in seconds;
* the seams patched below (``_context_results``, ``_update``, ``_job_view``,
  ``_verified_records`` and ``_build_context``) only move the logical clock
  across the campaign deadline at a named I/O seam.  They never change a
  deadline, an assertion or production semantics.
"""
import sys
from pathlib import Path
from unittest.mock import patch

import pytest

sys.dont_write_bytecode = True


def _repo_root():
    """The repository root that owns the public ``cmre`` package."""
    for candidate in Path(__file__).resolve().parents:
        if (candidate / 'cmre' / 'provider_campaign.py').is_file():
            return candidate
    raise RuntimeError('cannot locate the cmre package root')


ROOT = _repo_root()
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from cmre import (  # noqa: E402  (the package root is resolved above)
    pipeline_controller,
    pipeline_policy,
    provider_campaign,
    provider_jobs,
)

DURATION_SECONDS = 10
DEADLINE = 10.0
AUTHOR_MODEL = 'fixture/author'
REVIEWER_MODEL = 'fixture/reviewer'
HELD_FUNCTION = '0x00f00001'
SUPPLY_FUNCTION = '0x00f00002'
REVIEW_FUNCTION = '0x00f00003'


def _definition(name, function):
    return {'id': name, 'config': {
        'function': function,
        'mode': 'c',
        'goal': 'Public deadline boundary fixture',
        'owner': 'minimax',
        'reviewer': 'deepseek',
        'max_rounds': 1,
        'defer_execution': True,
        'defer_publication': True,
        'pipeline_stage': 'author',
        'max_body_bytes': 400,
    }}


def _manifest():
    return {
        'schema': provider_campaign.SCHEMA,
        'pipeline': True,
        'duration_seconds': DURATION_SECONDS,
        'concurrency': 1,
        'role_capacity': {'minimax': 1, 'deepseek': 1},
        'roles': {'minimax': {'model': AUTHOR_MODEL},
                  'deepseek': {'model': REVIEWER_MODEL}},
        'jobs': [_definition('held', HELD_FUNCTION),
                 _definition('supply', SUPPLY_FUNCTION),
                 _definition('review', REVIEW_FUNCTION)],
    }


class _Clock:
    """Elapsed seconds only; the module never sleeps on the wall clock."""

    def __init__(self):
        self.now = 0.0

    def __call__(self):
        return self.now

    def sleep(self, seconds):
        self.now += seconds


class _Env:
    """One campaign store, its logical clock and the recorded events."""

    def __init__(self, store):
        self.store = store
        self.clock = _Clock()
        self.events = []


def _change(store, name, **changes):
    job = provider_jobs.load(store)['jobs'][name]
    return provider_jobs.update(store, name, changes,
                                expected_revision=job['revision'])


def _job(store, name):
    return provider_jobs.load(store)['jobs'][name]


def _forbidden(*args, **kwargs):
    raise AssertionError('unexpected preparation/adoption/provider boundary')


def _drive(env, actor, execute, *, steps):
    """Drive the real Service and the durable campaign loop."""
    with pipeline_controller.Service(env.store, {}, execute, clock=env.clock,
                                     snapshot=_forbidden, prepare=_forbidden,
                                     adopt=_forbidden) as service:
        return provider_campaign.run(
            env.store, actor=actor, execute=execute, service=service,
            limits=lambda state: pipeline_policy.read(
                env.store, state['manifest']),
            max_steps=steps, clock=env.clock, sleep=env.clock.sleep)


def _author_actor(env):
    def actor(job, role, context, stage):
        env.events.append(('actor', job['id'], context['phase'], env.clock()))
        action = ({'kind': 'evidence'} if job['id'] == 'review'
                  else {'kind': 'hold', 'reason': 'Fixture terminal; no engine work'})
        model = AUTHOR_MODEL if role == 'minimax' else REVIEWER_MODEL
        return {'model': model, 'action': action}
    return actor


def _executor(env):
    def execute(job, action, output, *, prior):
        env.events.append(('execute', job['id'], action['kind'], env.clock()))
        return {'kind': action['kind'], 'ok': True,
                'result': {'instructions': []}}
    return execute


def _dispatch_pending(env, job, execute=None):
    return provider_campaign._execute_pending(
        env.store, job, execute or _executor(env), DEADLINE, env.clock,
        env.clock.sleep)


def _pending(env):
    """Leave one validated pending action on the supply job."""
    def actor(job, role, context, stage):
        env.events.append(('actor', job['id'], context['phase'], env.clock()))
        return {'model': AUTHOR_MODEL, 'action': {'kind': 'evidence'}}

    _drive(env, actor, _executor(env), steps=1)
    job = _job(env.store, 'supply')
    assert job['state'] == 'awaiting-execution'
    return job


def _assert_deferred(env, before):
    """The pending step, its records and every lifetime counter survive."""
    after = _job(env.store, 'supply')
    assert after['state'] == 'awaiting-execution'
    assert after['phase'] == 'execute'
    assert after['checkpoint'] is None
    for key in ('pending_step', 'action_count', 'round_actions', 'attempts',
                'results', 'config', 'provider_retries'):
        assert after.get(key) == before.get(key), key
    assert not [event for event in env.events if event[0] == 'execute']
    assert not (env.store / before['pending_step']['output_rel']).exists()
    return after


@pytest.fixture()
def env(tmp_path):
    store = tmp_path / 'store'
    provider_campaign.initialize(store, _manifest(),
                                 {'fixture': 'offline-public-deadline'})
    state = provider_jobs.load(store)
    policy = pipeline_policy.defaults(state['manifest'])
    # Inventory preparation and canonical adoption stay outside this fixture.
    policy['workers']['prepare'] = 0
    policy['workers']['adoption'] = 0
    pipeline_policy.write(store, policy, state['manifest'])
    _change(store, 'held', state='held',
            hold_reason='Retained historical guard rejection; no retry')
    _change(store, 'review', state='held',
            hold_reason='Inactive fixture unless the case enables it')
    return _Env(store)


def test_01_context_read_crosses_deadline_before_broker_dispatch(env):
    original = provider_campaign._context_results
    reads = []

    def retained_context(directory, job):
        result = original(directory, job)
        reads.append(job['state'])
        if job['phase'] == 'execute':
            # Isolated filesystem-latency seam: the real retained-record
            # reader runs, then deterministic elapsed I/O consumes the
            # remaining time.  No real sleep and no game execution.
            env.clock.now = DEADLINE
        return result

    def actor(job, role, context, stage):
        env.events.append(('actor', job['id'], context['phase'], env.clock()))
        return {'model': AUTHOR_MODEL, 'action': {'kind': 'evidence'}}

    with patch.object(provider_campaign, '_context_results', retained_context):
        _drive(env, actor, _executor(env), steps=2)
    assert reads
    late = [event for event in env.events
            if event[0] == 'execute' and event[3] >= DEADLINE]
    assert not late, ('LATE_BROKER_DISPATCH: the real pending-action controller '
                      'launched the executor after the deadline during context I/O')


def test_02_hold_is_task_local(env):
    _drive(env, _author_actor(env), _executor(env), steps=1)
    assert [event[1] for event in env.events] == ['supply']
    assert _job(env.store, 'held')['state'] == 'held'


def test_03_deadline_crossing_defers_broker_without_execution(env):
    def actor(job, role, context, stage):
        env.clock.now = DEADLINE
        env.events.append(('actor', job['id'], context['phase'], env.clock()))
        return {'model': AUTHOR_MODEL, 'action': {'kind': 'evidence'}}

    result = _drive(env, actor, _executor(env), steps=2)
    assert result['stopped'] == 'deadline'
    assert not any(event[0] == 'execute' for event in env.events)
    assert _job(env.store, 'supply')['state'] == 'awaiting-execution'


def test_04_action_exhaustion_never_calls_actor(env):
    _change(env.store, 'supply',
            round_actions=provider_campaign.MAX_ACTIONS_PER_ROUND)
    _drive(env, _author_actor(env), _executor(env), steps=1)
    assert env.events == []
    assert _job(env.store, 'supply')['state'] == 'held'


# Classification only: the retained historical receipt lives in the private
# review package and is not readable from a public test.  The fixture below
# copies its reviewed field shape and values.  The helper classifies a
# receipt; it never drives scheduler accounting.
GUARD_RECEIPT = {
    'new_invocations': 1,
    'C_builds': 0,
    'C_executions': 0,
    'remaining_batch_failed_invocation_allowance': 0,
}


def _classify_guard_receipt(receipt):
    """Classify a retained invocation receipt; no scheduler accounting."""
    if receipt['new_invocations'] <= 0:
        return 'no-new-invocation'
    if receipt['C_builds'] or receipt['C_executions']:
        return 'executed-c-failure'
    return 'guard-rejection-not-executed-c-failure'


def test_05_historical_guard_receipt_classifies_as_not_executed_c_failure():
    assert _classify_guard_receipt(GUARD_RECEIPT) == \
        'guard-rejection-not-executed-c-failure'
    assert GUARD_RECEIPT['remaining_batch_failed_invocation_allowance'] == 0
    assert _classify_guard_receipt(dict(GUARD_RECEIPT, C_executions=1)) == \
        'executed-c-failure'


def test_06_repeat_review_allows_supplied_author_during_broker_work(env):
    # The held review job keeps its retained review phase and bundle, so the
    # provider_jobs reopen guard still applies unchanged.
    _change(env.store, 'review', phase='review',
            bundle={'sha256': 'fixture', 'required_checks': []})
    _change(env.store, 'review', state='needs-review',
            retry_reason='Enable the retained review fixture')
    _drive(env, _author_actor(env), _executor(env), steps=3)
    actors = [event[1] for event in env.events if event[0] == 'actor']
    assert actors[0] == 'review'  # Independent review still drains first.
    assert 'supply' in actors


def test_07_context_crossing_preserves_pending_and_expired_resume(env):
    before = _pending(env)
    original = provider_campaign._context_results

    def crossing(directory, job):
        result = original(directory, job)
        env.clock.now = DEADLINE + 0.5
        return result

    with patch.object(provider_campaign, '_context_results', crossing):
        assert not _dispatch_pending(env, before)
    after = _assert_deferred(env, before)
    run_bytes = (env.store / 'run.json').read_bytes()
    result = _drive(env, _author_actor(env), _executor(env), steps=1)
    assert result['stopped'] == 'deadline'
    assert (env.store / 'run.json').read_bytes() == run_bytes
    assert _job(env.store, 'supply') == after
    _assert_deferred(env, before)


def test_08_exact_equality_after_update_is_deferred(env):
    before = _pending(env)
    original = provider_campaign._update

    def crossing(directory, job, changes, sleep):
        result = original(directory, job, changes, sleep)
        if changes.get('state') == 'running':
            env.clock.now = DEADLINE
        return result

    with patch.object(provider_campaign, '_update', crossing):
        assert not _dispatch_pending(env, before)
    _assert_deferred(env, before)


def test_09_argument_preparation_crosses_at_real_executor_seam(env):
    before = _pending(env)
    original = provider_campaign._job_view

    def crossing(job):
        result = original(job)
        env.clock.now = DEADLINE
        return result

    with patch.object(provider_campaign, '_job_view', crossing):
        assert not _dispatch_pending(env, before)
    _assert_deferred(env, before)


def test_10_predeadline_dispatch_once_and_normal_resume(env):
    before = _pending(env)
    env.clock.now = 9.0
    assert _dispatch_pending(env, before)
    after = _job(env.store, 'supply')
    assert after['state'] == 'prepared'
    assert len(after['results']) == 1
    assert after['results'][0]['id'] == before['pending_step']['action_id']
    assert after['action_count'] == before['action_count']
    # Resume through the real runner: it must author again, not replay the
    # stale pending_step which remains retained but is no longer dispatchable.
    _drive(env, _author_actor(env), _executor(env), steps=1)
    assert [event for event in env.events if event[0] == 'execute'] == [
        ('execute', 'supply', 'evidence', 9.0)]
    final = _job(env.store, 'supply')
    assert final['state'] == 'held'
    assert final['results'][:-1] == after['results']
    assert final['results'][-1]['kind'] == 'hold'


def test_11_review_pending_context_crossing_is_deferred(env):
    before = _pending(env)
    step = dict(before['pending_step'], phase='review', role='deepseek')
    before = _change(env.store, 'supply', pending_step=step)
    original = provider_campaign._context_results

    def crossing(directory, job):
        result = original(directory, job)
        env.clock.now = DEADLINE
        return result

    with patch.object(provider_campaign, '_context_results', crossing):
        assert not _dispatch_pending(env, before)
    _assert_deferred(env, before)


def test_12_automatic_bundle_record_io_crossing_is_deferred(env):
    before = _pending(env)
    step = dict(before['pending_step'], automatic=True,
                action={'kind': 'bundle', 'artifacts': [], 'summary': 'Fixture'})
    before = _change(env.store, 'supply', pending_step=step)
    original = provider_campaign._verified_records
    original_context = provider_campaign._build_context
    context_done = []
    reads = []

    def built_context(*args, **kwargs):
        result = original_context(*args, **kwargs)
        context_done.append(True)
        return result

    def crossing(directory, job):
        result = list(original(directory, job))
        # Context can verify records more than once.  Cross only on the
        # automatic bundle's later artifact-selection read.
        if context_done:
            reads.append(job['state'])
            env.clock.now = DEADLINE
        return result

    with patch.object(provider_campaign, '_build_context', built_context), \
            patch.object(provider_campaign, '_verified_records', crossing):
        assert not _dispatch_pending(env, before)
    assert len(reads) == 1
    _assert_deferred(env, before)


def test_13_publication_update_crossing_preserves_approval(env):
    before = _change(env.store, 'supply', state='reviewed', phase='publish',
                     bundle={'sha256': 'fixture', 'required_checks': []},
                     review={'verdict': 'pass'}, review_model=REVIEWER_MODEL,
                     author_model=AUTHOR_MODEL)
    original = provider_campaign._update

    def crossing(directory, job, changes, sleep):
        result = original(directory, job, changes, sleep)
        if changes.get('state') == 'running':
            env.clock.now = DEADLINE
        return result

    with patch.object(provider_campaign, '_update', crossing):
        assert not provider_campaign._publish_reviewed(
            env.store, before, _executor(env), DEADLINE, env.clock,
            env.clock.sleep)
    after = _job(env.store, 'supply')
    assert (after['state'], after['phase'], after['checkpoint']) == \
        ('reviewed', 'publish', None)
    for key in ('bundle', 'review', 'review_model', 'author_model', 'results',
                'config'):
        assert after.get(key) == before.get(key)
    assert after['action_count'] == before.get('action_count', 0) + 1
    assert env.events == []
