"""Real persistent Queue; fake native IDs/artifacts, not native execution proof.

Default collection imports cmre.native_queue. CMRE_F01_QUEUE_MODULE is an explicit
steward-only before/after selection, propagated unchanged to spawned children.
"""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import sys

REPO = next(p for p in Path(__file__).resolve().parents if (p / 'cmre/coordination.py').is_file())
sys.path.insert(0, str(REPO))

import pytest

HERE = Path(__file__).resolve().parent
if os.environ.get('CMRE_F01_QUEUE_MODULE'):
    spec = importlib.util.spec_from_file_location('candidate_native_queue', os.environ['CMRE_F01_QUEUE_MODULE'])
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
else:
    from cmre import native_queue as module


def pin(path, value='fixture'):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(value, encoding='utf-8')
    return {'path': str(path), 'sha256': hashlib.sha256(path.read_bytes()).hexdigest()}


class Parent:
    """Fake boundary asserts durable ownership and lock release BEFORE launch."""
    def __init__(self, queue):
        self.queue, self.calls = queue, []

    def launch(self, action):
        assert self.queue.read()['requests'][action['id']]['state'] == 'intent'
        with module.lock(self.queue.root / 'queue.lock'):
            pass
        sid = 'FAKE-' + str(len(self.calls) + 1)
        self.calls.append((action, sid))
        self.queue.event({'kind': 'admit', 'event_id': 'admit-' + action['id'],
                          'request': action['id'], 'subagent_id': sid})
        return sid


def setup(tmp_path, **changes):
    clock = [100.0]
    queue = module.Queue(tmp_path / 'queue', clock=lambda: clock[0])
    config = {'parent_owner': 'parent', 'steward': 'sol-steward', 'steward_model': module.SOL,
              'parent_receipt': pin(tmp_path / 'parent.json'),
              'validated_plan': pin(tmp_path / 'plan.json'),
              'private_root': str(tmp_path / 'private'), 'deadline': 1000,
              'dispatch_budget': 20, 'supply_limit': 10, 'buffer_limit': 4,
              'ready_limit': 2, 'author_cap': 2, 'reviewer_cap': 2,
              'reserve_gib': 4, 'worker_gib': 1, **changes}
    queue.initialize(config)
    return queue, Parent(queue), clock


def prepare(queue, key, **changes):
    private = Path(queue.read()['config']['private_root']) / key
    task = {'id': key, 'author': key + '-author', 'reviewer': key + '-reviewer',
            'author_model': 'minimax-coding-plan/MiniMax-M3.1-Flash',
            'reviewer_model': 'opencode-go/deepseek-v4.1-flash',
            'max_messages': 8, 'max_seconds': 60, 'remaining_failures': 1,
            'required_checks': ['full-source', 'original-evidence'],
            'packets': {phase: pin(private / (phase + '-packet.txt'),
                       'ROLE=WORKER. Private fixture only; no source acceptance.')
                        for phase in ('author', 'reviewer')},
            'outputs': {'author': [str(private / name) for name in ('source', 'hints', 'report')],
                        'reviewer': [str(private / 'verdict.json')]}, **changes}
    event = {'kind': 'prepare', 'event_id': 'prepare-' + key, 'task': task}
    queue.event(event)
    return event


def reserve(queue, clock, memory=12):
    return queue.reserve(owner='parent', available_gib=memory, sampled_at=clock[0])


def finish(queue, action, sid, **changes):
    state = queue.read()
    task = state['tasks'][action['task']]
    definition = task['definition']
    event = {'kind': 'terminal', 'event_id': 'terminal-' + action['id'],
             'request': action['id'], 'subagent_id': sid,
             'outcome': 'succeeded', 'observed_model': None, 'usage': None}
    if action['phase'] == 'author':
        event['bundle'] = {key: pin(Path(path), 'FAKE fixture ' + key)
                           for key, path in zip(('source', 'hints', 'report'),
                                                definition['outputs']['author'])}
    else:
        verdict = {'schema': 'cmre-native-queue-review/1', 'task': action['task'],
                   'author': definition['author'], 'reviewer': definition['reviewer'],
                   'bundle_sha256': task['bundle_sha256'], 'pins': task['bundle'],
                   'verdict': 'approve', 'unknowns': [], 'checks': definition['required_checks']}
        event['verdict'] = pin(Path(definition['outputs']['reviewer'][0]), json.dumps(verdict))
    event.update(changes)
    queue.event(event)
    return event


def test_two_lane_vertical_slice_and_single_steward(tmp_path):
    queue, parent, clock = setup(tmp_path)
    for key in ('a', 'b', 'c', 'd'):
        prepare(queue, key)
    first = reserve(queue, clock)
    assert len(first['native_actions']) == 2
    for action in first['native_actions']:
        finish(queue, action, parent.launch(action))
    clock[0] += 0.01  # F01 fresh sample
    second = reserve(queue, clock)
    assert [a['phase'] for a in second['native_actions']] == ['reviewer', 'reviewer', 'author', 'author']
    for action in second['native_actions']:
        sid = parent.launch(action)
        if action['phase'] == 'reviewer':
            finish(queue, action, sid)
    third = reserve(queue, clock)
    assert third['steward_action']['task'] == 'a'
    assert reserve(queue, clock)['steward_action'] is None
    assert queue.read()['tasks']['b']['state'] == 'ready'
    assert queue.read()['tasks']['c']['state'] == 'author-pending'
    assert third['metrics']['review_backpressure']
    assert third['metrics']['acceptance_count'] is None
    receipt = pin(tmp_path / 'gate.json', 'FAKE receipt, NOT acceptance')
    queue.event({'kind': 'steward-receipt', 'event_id': 'receipt-a', 'task': 'a',
                 'steward': 'sol-steward', 'outcome': 'completed', 'receipt': receipt,
                 'bundle_sha256': third['steward_action']['bundle_sha256']})
    assert reserve(queue, clock)['steward_action']['task'] == 'b'
    assert queue.read()['steward_receipts'] == 1


def test_local_hold_replaces_eligible_work_and_state_metadata(tmp_path):
    queue, parent, clock = setup(tmp_path, author_cap=1)
    event = prepare(queue, 'held')
    queue.event(event)
    queue.event({'kind': 'hold', 'event_id': 'hold', 'task': 'held',
                 'reason': 'missing original evidence', 'retry_when': 'fresh independently pinned supply'})
    prepare(queue, 'a')
    prepare(queue, 'b')
    action = reserve(queue, clock)['native_actions'][0]
    assert action['task'] == 'a'
    finish(queue, action, parent.launch(action), outcome='failed')
    clock[0] += 0.01  # F01 fresh sample
    assert reserve(queue, clock)['native_actions'][0]['task'] == 'b'
    assert queue.read()['tasks']['held']['state'] == 'held'
    # Prior nine-test collision pattern: actual event path accepts state metadata.
    with module.lock(queue.root / 'queue.lock'):
        state = queue.read()
        queue._commit(state, 'fixture', state='held')
    assert queue.read()['events'][-1]['state'] == 'held'


def test_restart_dedup_and_unknown_identity(tmp_path):
    queue, parent, clock = setup(tmp_path)
    prepare(queue, 'a')
    action = reserve(queue, clock)['native_actions'][0]
    sid = parent.launch(action)
    event = finish(queue, action, sid)
    queue.event(event)
    queue.event({**event, 'event_id': 'another-terminal-notification'})
    restarted = module.Queue(queue.root, clock=lambda: clock[0])
    clock[0] += 0.01  # F01 fresh sample
    next_actions = reserve(restarted, clock)['native_actions']
    assert len(next_actions) == 1 and next_actions[0]['phase'] == 'reviewer'
    assert reserve(restarted, clock)['native_actions'] == []
    request = restarted.read()['requests'][action['id']]
    assert request['observed_model'] is None and request['model'] == action['model']
    assert request['admitted_at'] == 100 and request['terminal_at'] == 100
    with pytest.raises(ValueError, match='Conflicting'):
        queue.event({**event, 'outcome': 'failed'})


def test_uncertain_and_lost_reservation_never_relaunch(tmp_path):
    queue, parent, clock = setup(tmp_path, author_cap=2)
    prepare(queue, 'a')
    action = reserve(queue, clock)['native_actions'][0]
    queue.event({'kind': 'launch-uncertain', 'event_id': 'uncertain',
                 'request': action['id'], 'reason': 'lost native return; reconcile with parent'})
    prepare(queue, 'b')
    restarted = module.Queue(queue.root, clock=lambda: clock[0])
    clock[0] += 0.01  # F01 fresh sample
    actions = reserve(restarted, clock)['native_actions']
    assert len(actions) == 1 and actions[0]['task'] == 'b'
    assert reserve(restarted, clock)['native_actions'] == []
    assert not reserve(restarted, clock)['metrics']['drained']
    # Actual returned identity may reconcile uncertainty; no new dispatch intent.
    sid = 'FAKE-reconciled-native-ID'
    queue.event({'kind': 'admit', 'event_id': 'reconcile-a', 'request': action['id'], 'subagent_id': sid})
    finish(queue, action, sid, outcome='failed')
    assert queue.read()['attempts'] == 2


def test_headroom_queue_age_buffer_and_independent_caps(tmp_path):
    queue, parent, clock = setup(tmp_path, author_cap=2, reviewer_cap=1, buffer_limit=2)
    for key in ('a', 'b', 'c'):
        prepare(queue, key)
    assert reserve(queue, clock, 4.9)['native_actions'] == []
    clock[0] += 12
    result = reserve(queue, clock, 5.1)
    assert result['metrics']['author_queue_age_seconds'] == 12
    action = result['native_actions'][0]
    finish(queue, action, parent.launch(action))
    clock[0] += 0.01  # F01 fresh sample
    result = reserve(queue, clock)
    assert [a['phase'] for a in result['native_actions']] == ['reviewer', 'author']
    assert result['metrics']['author_backpressure']
    assert queue.read()['tasks']['c']['state'] == 'prepared'
    assert queue.reserve(owner='parent', available_gib=12, sampled_at=0)['native_actions'] == []


@pytest.mark.parametrize('stop', ['stop', 'deadline', 'budget'])
def test_finite_stop_drain_preserves_live_workers(tmp_path, stop):
    queue, parent, clock = setup(tmp_path, dispatch_budget=1 if stop == 'budget' else 20)
    prepare(queue, 'a')
    action = reserve(queue, clock)['native_actions'][0]
    sid = parent.launch(action)
    prepare(queue, 'b')
    if stop == 'stop':
        queue.event({'kind': 'stop', 'event_id': 'stop'})
    elif stop == 'deadline':
        clock[0] = 1001
    result = reserve(queue, clock)
    assert result['native_actions'] == [] and not result['metrics']['drained']
    assert queue.read()['requests'][action['id']]['state'] == 'running'
    finish(queue, action, sid, outcome='failed')
    assert reserve(queue, clock)['metrics']['drained']
    with pytest.raises(ValueError, match='cannot be reset'):
        queue.initialize({})


def test_wrong_id_pin_review_and_steward_rejected(tmp_path):
    queue, parent, clock = setup(tmp_path)
    prepare(queue, 'a')
    action = reserve(queue, clock)['native_actions'][0]
    sid = parent.launch(action)
    with pytest.raises(ValueError, match='actual subagent'):
        finish(queue, action, 'WRONG')
    finish(queue, action, sid)
    clock[0] += 0.01  # F01 fresh sample
    review = reserve(queue, clock)['native_actions'][0]
    sid = parent.launch(review)
    task = queue.read()['tasks']['a']
    bad = pin(Path(task['definition']['outputs']['reviewer'][0]), json.dumps({
        'verdict': 'approve', 'bundle_sha256': 'wrong'}))
    queue.event({'kind': 'terminal', 'event_id': 'bad-review', 'request': review['id'],
                 'subagent_id': sid, 'outcome': 'succeeded', 'verdict': bad})
    assert queue.read()['tasks']['a']['state'] == 'held'
    assert reserve(queue, clock)['steward_action'] is None
    with pytest.raises(ValueError, match='Single canonical'):
        queue.event({'kind': 'steward-receipt', 'event_id': 'illegal',
                     'task': 'a', 'steward': 'a-author'})


def test_frozen_packet_outputs_owner_and_supply_budget(tmp_path):
    queue, parent, clock = setup(tmp_path, supply_limit=1)
    event = prepare(queue, 'a')
    Path(event['task']['packets']['author']['path']).write_text('drift', encoding='utf-8')
    with pytest.raises(ValueError, match='Pin mismatch'):
        reserve(queue, clock)
    assert queue.read()['attempts'] == 0
    with pytest.raises(ValueError, match='supply budget'):
        prepare(queue, 'b')
    with pytest.raises(ValueError, match='owner mismatch'):
        queue.reserve(owner='other', available_gib=12, sampled_at=clock[0])


def test_confirmed_no_launch_reconciliation_and_cli_status(tmp_path, capsys):
    queue, parent, clock = setup(tmp_path, author_cap=1)
    prepare(queue, 'a')
    prepare(queue, 'b')
    action = reserve(queue, clock)['native_actions'][0]
    queue.event({'kind': 'launch-not-created', 'event_id': 'no-launch',
                 'request': action['id'], 'steward': 'sol-steward',
                 'evidence': pin(tmp_path / 'no-launch.json', 'FAKE affirmative no-launch evidence')})
    clock[0] += 0.01  # F01 fresh sample
    assert reserve(queue, clock)['native_actions'][0]['task'] == 'b'
    assert queue.read()['attempts'] == 2
    module.main([str(queue.root), 'status'])
    output = json.loads(capsys.readouterr().out)
    assert output['tasks']['a']['state'] == 'held'
    assert output['requests'][action['id']]['state'] == 'terminal'


def sample_reserve(queue, stamp, memory=5):
    return queue.reserve(owner='parent', available_gib=memory, sampled_at=stamp)


@pytest.mark.parametrize('restart', [False, True])
def test_f01_reused_sample_across_phases(tmp_path, restart):
    queue, parent, clock = setup(tmp_path, author_cap=1)
    prepare(queue, 'a')
    prepare(queue, 'b')
    first = sample_reserve(queue, 100)['native_actions']
    assert len(first) == 1 and first[0]['phase'] == 'author'
    terminal = finish(queue, first[0], parent.launch(first[0]))
    queue.event(terminal)
    queue.event({**terminal, 'event_id': 'duplicate-new-id'})
    before = queue.read()
    assert sample_reserve(queue, 100)['native_actions'] == []
    if restart:
        queue = module.Queue(queue.root, clock=lambda: clock[0])
    assert sample_reserve(queue, 100)['native_actions'] == []
    assert sample_reserve(queue, 100, 1000)['native_actions'] == []
    after = queue.read()
    assert after['attempts'] == before['attempts'] == 1
    assert after['requests'] == before['requests']
    assert after['seen'] == before['seen']
    assert after['tasks']['b'] == before['tasks']['b']
    assert after['tasks']['b']['state'] == 'prepared'
    clock[0] = 100.1
    second = sample_reserve(queue, 100.1)['native_actions']
    assert len(second) == 1 and second[0]['phase'] == 'reviewer'
    assert second[0]['observed_model'] is None and second[0]['usage'] is None
    assert queue.read()['attempts'] == 2


@pytest.mark.parametrize('resolution', ['uncertain', 'not-created', 'failed'])
def test_f01_retry_and_restart_do_not_renew_sample(tmp_path, resolution):
    queue, parent, clock = setup(tmp_path, author_cap=1)
    prepare(queue, 'a')
    prepare(queue, 'b')
    first = sample_reserve(queue, 100)['native_actions'][0]
    if resolution in ('uncertain', 'not-created'):
        queue.event({'kind': 'launch-uncertain', 'event_id': 'lost-return',
                     'request': first['id'], 'reason': 'FAKE lost response'})
        if resolution == 'not-created':
            event = {'kind': 'launch-not-created', 'event_id': 'affirmative-no-worker',
                     'request': first['id'], 'steward': 'sol-steward',
                     'evidence': pin(tmp_path / 'no-worker.json', 'FAKE affirmative evidence')}
            queue.event(event)
            queue.event(event)
    else:
        event = finish(queue, first, parent.launch(first), outcome='failed')
        queue.event({**event, 'event_id': 'duplicate-failure'})
    restarted = module.Queue(queue.root, clock=lambda: clock[0])
    assert sample_reserve(restarted, 100)['native_actions'] == []
    assert restarted.read()['attempts'] == 1
    assert restarted.read()['requests'][first['id']]['state'] == (
        'uncertain' if resolution == 'uncertain' else 'terminal')
    clock[0] = 100.1
    actions = sample_reserve(restarted, 100.1)['native_actions']
    if resolution == 'uncertain':
        assert actions == []
        assert not restarted._metrics(restarted.read())['drained']
    else:
        assert len(actions) == 1 and actions[0]['task'] == 'b'
        assert restarted.read()['attempts'] == 2


def test_f01_older_sample_even_within_fresh_window(tmp_path):
    queue, parent, clock = setup(tmp_path, author_cap=1)
    prepare(queue, 'a')
    first = sample_reserve(queue, 100)['native_actions'][0]
    finish(queue, first, parent.launch(first))
    clock[0] = 101
    assert sample_reserve(queue, 99)['native_actions'] == []
    assert sample_reserve(queue, 100)['native_actions'] == []
    assert queue.read()['attempts'] == 1
    assert len(sample_reserve(queue, 101)['native_actions']) == 1


def test_f01_no_actions_or_cap_denial_does_not_consume(tmp_path):
    queue, parent, clock = setup(tmp_path, author_cap=1)
    assert sample_reserve(queue, 100)['native_actions'] == []
    prepare(queue, 'a')
    prepare(queue, 'b')
    first = sample_reserve(queue, 100)['native_actions'][0]
    clock[0] = 101
    assert sample_reserve(queue, 101)['native_actions'] == []
    finish(queue, first, parent.launch(first), outcome='failed')
    assert sample_reserve(queue, 101)['native_actions'][0]['task'] == 'b'
    assert queue.read()['attempts'] == 2


def test_f01_pin_failure_rolls_back_whole_batch(tmp_path):
    queue, parent, clock = setup(tmp_path)
    prepare(queue, 'a')
    second = prepare(queue, 'b')
    packet = Path(second['task']['packets']['author']['path'])
    original = packet.read_bytes()
    packet.write_bytes(b'FAKE drift')
    before = queue.path.read_bytes()
    with pytest.raises(ValueError, match='Pin mismatch'):
        sample_reserve(queue, 100, 6)
    assert queue.path.read_bytes() == before
    packet.write_bytes(original)
    assert len(sample_reserve(queue, 100, 6)['native_actions']) == 2
    assert queue.read()['attempts'] == 2
    assert sample_reserve(queue, 100, 6)['native_actions'] == []


def test_f01_old_schema_uses_latest_intent_not_reset(tmp_path):
    queue, parent, clock = setup(tmp_path, author_cap=1)
    prepare(queue, 'a')
    action = sample_reserve(queue, 99.5)['native_actions'][0]
    finish(queue, action, parent.launch(action))
    with module.lock(queue.root / 'queue.lock'):
        state = queue.read()
        state.pop('headroom_consumed_at', None)
        queue._commit(state, 'legacy-fixture')
    queue = module.Queue(queue.root, clock=lambda: clock[0])
    assert sample_reserve(queue, 99.5)['native_actions'] == []
    assert sample_reserve(queue, 100)['native_actions'] == []
    clock[0] = 100.1
    assert sample_reserve(queue, 100.1)['native_actions'][0]['phase'] == 'reviewer'
    assert queue.read()['attempts'] == 2


def test_f01_steward_progress_does_not_need_new_sample(tmp_path):
    queue, parent, clock = setup(tmp_path)
    prepare(queue, 'a')
    author = sample_reserve(queue, 100)['native_actions'][0]
    finish(queue, author, parent.launch(author))
    clock[0] = 100.1
    review = sample_reserve(queue, 100.1)['native_actions'][0]
    finish(queue, review, parent.launch(review))
    state = queue.read()
    result = sample_reserve(queue, 100.1)
    assert result['native_actions'] == []
    assert result['steward_action']['task'] == 'a'
    assert sample_reserve(queue, 100.1)['steward_action'] is None
    assert queue.read()['attempts'] == state['attempts'] == 2
    assert result['metrics']['acceptance_count'] is None


def test_f01_fresh_measurement_not_double_charged(tmp_path):
    queue, parent, clock = setup(tmp_path, author_cap=2)
    prepare(queue, 'a')
    first = sample_reserve(queue, 100)['native_actions'][0]
    parent.launch(first)
    prepare(queue, 'b')
    clock[0] = 100.1
    actions = sample_reserve(queue, 100.1)['native_actions']
    assert len(actions) == 1 and actions[0]['task'] == 'b'
    assert len(queue._metrics(queue.read())['live_obligations']) == 2


def test_f01_spawn_same_brief_real_lock_contention(tmp_path, monkeypatch):
    """Shared S only; not protection against independently sampled timestamps.

    The holder takes the real lock before the barrier, proving the contender's
    first reserve collides. Its one retry waits for the holder's actual commit.
    This is deterministic contention, not a claim of overlapping native jobs.
    """
    import multiprocessing
    import time

    monkeypatch.syspath_prepend(str(HERE))
    from native_queue_spawn_worker import reserve_child

    queue, parent, clock = setup(tmp_path, author_cap=2)
    prepare(queue, 'a')
    prepare(queue, 'b')
    before = queue.read()
    ctx = multiprocessing.get_context('spawn')
    barrier = ctx.Barrier(2)
    contended, committed = ctx.Event(), ctx.Event()
    reports = ctx.Queue()
    brief = {'owner': 'parent', 'available_gib': 5, 'sampled_at': 100.0}
    processes, started = [], []
    results = []
    try:
        for role in ('holder', 'contender'):
            process = ctx.Process(target=reserve_child, args=(
                role, str(Path(module.__file__).resolve()), str(queue.root),
                brief, barrier, contended, committed, reports))
            processes.append(process)
            process.start()
            started.append(process)
        deadline = time.monotonic() + 45
        # Drain while children are alive: joining before reading can deadlock a
        # multiprocessing.Queue feeder on Windows when a report fills its pipe.
        results = [reports.get(timeout=max(0, deadline - time.monotonic()))
                   for _ in started]
        for process in started:
            process.join(max(0, deadline - time.monotonic()))
        assert all(not process.is_alive() for process in started), 'Spawn worker timed out'
        assert all(process.exitcode == 0 for process in started)
    finally:
        for process in started:
            if process.is_alive():
                process.terminate()
        for process in started:
            process.join(3)
            if process.is_alive():
                process.kill()
                process.join(3)
        unresolved = [process.pid for process in started if process.is_alive()]
        reports.close()
        reports.join_thread()
        assert not unresolved, f'Unresolved children: {unresolved}'
    assert len(results) == 2
    assert {result['role'] for result in results} == {'holder', 'contender'}
    assert len({result['pid'] for result in results}) == 2
    assert all(result['pid'] != os.getpid() for result in results)
    assert all(result['error'] is None for result in results), results
    by_role = {result['role']: result for result in results}
    assert by_role['holder']['contentions'] == 0
    assert by_role['holder']['reserve_calls'] == 1
    assert by_role['contender']['contentions'] == 1
    assert by_role['contender']['reserve_calls'] == 2
    assert all(result['brief'] == brief for result in results)
    batches = [result['native_actions'] for result in results]
    assert sorted(map(len, batches)) == [0, 1], results
    action = next(batch[0] for batch in batches if batch)
    assert action['phase'] == 'author' and action['task'] == 'a'
    state = queue.read()
    assert state['attempts'] == 1
    assert len(state['requests']) == 1
    assert set(state['requests']) == {action['id']}
    reserved_events = [event for event in state['events'] if event['kind'] == 'reserved']
    assert len(reserved_events) == 2  # BusyError wrote no event; both returns did.
    assert [rid for event in reserved_events for rid in event['actions']] == [action['id']]
    request = state['requests'][action['id']]
    assert request['state'] == 'intent' and request['phase'] == 'author'
    assert request['task'] == 'a'
    assert state['tasks']['a']['state'] == 'author-pending'
    assert state['tasks']['b'] == before['tasks']['b']
    assert state['seen'] == before['seen']
    assert state['headroom_consumed_at'] == brief['sampled_at']


def seal_public_submission():
    """Steward only: materialize exact two-file diff/pins, never adopt or test.

    Kept guarded so normal public pytest collection performs no sealing writes.
    """
    import ast
    import difflib

    if HERE != REPO / 'CM0102/Analysis/cmre/pipeline-repair-20261112/headroom-public':
        raise ValueError('Seal only the authorized private proposal')
    submission_path = HERE / 'submission.json'
    submission = json.loads(submission_path.read_text(encoding='utf-8'))
    if submission['sealing_status'] != 'unsealed-author-candidate':
        raise ValueError('Frozen proposal; do not reseal')
    target = REPO / 'tests/test_native_queue.py'
    original = target.read_bytes()
    assert hashlib.sha256(original).hexdigest() == 'fa2a9d04c352074cafbaaac4c8d0fda117e95b7b52cd824879f0cfa9113ae6cb'
    assert not (REPO / 'tests/native_queue_spawn_worker.py').exists()
    source = REPO / 'CM0102/Analysis/cmre/pipeline-repair-20261112/headroom-fix/native_queue.py'
    assert hashlib.sha256(source.read_bytes()).hexdigest() == 'dd13878e4b61ef7805cf81a626ce5a851e74d1bcbd0cde9a36db6b40aa5a6244'
    assert hashlib.sha256((REPO / 'cmre/native_queue.py').read_bytes()).hexdigest() == 'ae1b1d31719bceef57db2b525236c45926549186d526e6e2f7952e1def47b794'
    candidate = Path(__file__).read_bytes()
    advance = '    clock[0] += 0.01  # F01 fresh sample'
    assert candidate.decode().splitlines().count(advance) == 7
    # Every old test and shared fixture is identical modulo the seven advances.
    def originals(data):
        tree = ast.parse(data)
        return {node.name: ast.dump(node, include_attributes=False)
                for node in tree.body if isinstance(node, (ast.FunctionDef, ast.ClassDef))
                and node.name != 'seal_private_submission'}
    stripped = candidate.decode().replace('    clock[0] += 0.01  # F01 fresh sample\n', '')
    old_nodes, new_nodes = originals(original), originals(stripped)
    assert all(new_nodes.get(name) == node for name, node in old_nodes.items())
    helper = (HERE / 'native_queue_spawn_worker.py').read_bytes()
    patch = ''.join(difflib.unified_diff(
        original.decode().splitlines(keepends=True), candidate.decode().splitlines(keepends=True),
        fromfile='a/tests/test_native_queue.py', tofile='b/tests/test_native_queue.py'))
    patch += 'diff --git a/tests/native_queue_spawn_worker.py b/tests/native_queue_spawn_worker.py\nnew file mode 100644\n'
    patch += ''.join(difflib.unified_diff(
        [], helper.decode().splitlines(keepends=True),
        fromfile='/dev/null', tofile='b/tests/native_queue_spawn_worker.py'))
    (HERE / 'changes.patch').write_bytes(patch.encode())
    manifest_path = HERE / 'change-manifest.json'
    manifest = json.loads(manifest_path.read_text(encoding='utf-8'))
    manifest['existing_files_changed'][0]['after'] = hashlib.sha256(candidate).hexdigest()
    manifest['new_files'][0]['sha256'] = hashlib.sha256(helper).hexdigest()
    manifest['patch_sha256'] = hashlib.sha256(patch.encode()).hexdigest()
    manifest['sealing_status'] = 'sealed-by-steward-not-tested-not-reviewed'
    manifest_path.write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    submission['pins'] = {name: {'path': (HERE / name).relative_to(REPO).as_posix(),
                               'sha256': hashlib.sha256((HERE / name).read_bytes()).hexdigest()}
                          for name in ('test_native_queue.py', 'native_queue_spawn_worker.py',
                                       'changes.patch', 'change-manifest.json', 'no-hint-change.json')}
    submission['sealing_status'] = 'sealed-by-steward-not-tested-not-reviewed'
    submission_path.write_text(json.dumps(submission, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({'pins': submission['pins'], 'submission_sha256': hashlib.sha256(submission_path.read_bytes()).hexdigest()}))


if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--seal', action='store_true')
    parser.add_argument('--queue-module', type=Path)
    args, remaining = parser.parse_known_args()
    if args.seal:
        if args.queue_module or remaining:
            parser.error('--seal takes no other arguments')
        seal_public_submission()
    else:
        if args.queue_module is None:
            parser.error('Use pytest for public default, or explicit --queue-module')
        os.environ['CMRE_F01_QUEUE_MODULE'] = str(args.queue_module.resolve())
        raise SystemExit(pytest.main(['-p', 'no:cacheprovider', str(Path(__file__).resolve()), *remaining]))
