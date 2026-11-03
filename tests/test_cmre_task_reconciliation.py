"""Cross-packet lease exclusion and durable crash reconciliation."""
import time
from datetime import datetime, timedelta, timezone

import pytest

from cmre import env, tasks, work
from test_cmre_tasks import VA, workspace


@pytest.mark.parametrize('second_owner', ['first-worker', 'second-worker'])
def test_changed_fingerprint_does_not_bypass_live_task_lease(workspace, second_owner):
    first = tasks.prepare(VA)
    leased = tasks.claim(first['task_id'], 'first-worker', lease_seconds=60)
    workspace['harness'] = 'changed-while-leased'
    second = tasks.prepare(VA)
    assert second['task_id'] != first['task_id']
    with pytest.raises(ValueError, match='live.*lease|lease.*live'):
        tasks.claim(second['task_id'], second_owner)
    assert tasks.status(first['task_id'])['status'] == 'claimed'
    assert tasks.status(second['task_id'])['status'] == 'prepared'
    assert tasks._load(first['task_id'])['lease']['token'] == leased['lease']['token']


def test_expired_other_fingerprint_allows_new_claim_without_erasing_old_attempt(workspace, monkeypatch):
    first = tasks.prepare(VA)
    leased = tasks.claim(first['task_id'], 'first-worker', lease_seconds=60)
    workspace['harness'] = 'changed-after-expiry'
    second = tasks.prepare(VA)
    monkeypatch.setattr(tasks.time, 'time', lambda: leased['lease']['expires_at'] + 1)
    assert tasks.claim(second['task_id'], 'second-worker')['status'] == 'claimed'
    assert tasks._load(first['task_id'])['lease']['token'] == leased['lease']['token']


@pytest.mark.parametrize('status', ['submitted', 'publishing'])
def test_other_fingerprint_pending_submission_blocks_new_claim(workspace, status):
    packet = tasks.prepare(VA)
    leased = tasks.claim(packet['task_id'], 'first-worker')
    (env.ROOT / leased['candidate_path']).write_text('void cm_fn_00401000(void) {}')
    first = tasks.submit(packet['task_id'], 'first-worker', leased['lease']['token'])
    if status == 'publishing':
        first['status'] = 'publishing'
        tasks.save(first)
    workspace['harness'] = 'changed-after-submission'
    other = tasks.prepare(VA)
    with pytest.raises(ValueError, match='pending.*' + status):
        tasks.claim(other['task_id'], 'second-worker')
    assert tasks._load(first['task_id'])['status'] == status
    assert tasks._load(other['task_id'])['status'] == 'prepared'


def test_expired_fifth_testing_attempt_is_reconciled_without_new_lease(workspace):
    packet = tasks.prepare(VA)
    manifest = tasks.claim(packet['task_id'], 'first-worker')
    manifest['attempts'] = [
        {'number': number, 'owner': 'first-worker', 'status': 'failed' if number < 5 else 'testing'}
        for number in range(1, 6)
    ]
    manifest['lease']['expires_at'] = time.time() - 1
    tasks.save(manifest)
    with pytest.raises(ValueError, match='exhausted|attempt'):
        tasks.claim(packet['task_id'], 'reconciler')
    reconciled = tasks._load(packet['task_id'])
    assert reconciled['status'] == 'exhausted'
    assert reconciled['lease'] is None
    assert len(reconciled['attempts']) == 5
    assert reconciled['attempts'][-1]['status'] == 'interrupted'
    assert any(event['event'] == 'attempt-reconciled' for event in reconciled['journal'])


def test_new_fingerprint_reconciles_expired_fifth_attempt_before_rejecting(workspace):
    first = tasks.prepare(VA)
    manifest = tasks.claim(first['task_id'], 'first-worker')
    manifest['attempts'] = [
        {'number': number, 'owner': 'first-worker', 'status': 'failed' if number < 5 else 'testing'}
        for number in range(1, 6)
    ]
    manifest['lease']['expires_at'] = time.time() - 1
    tasks.save(manifest)
    workspace['harness'] = 'new-fingerprint'
    second = tasks.prepare(VA)
    with pytest.raises(ValueError, match='exhausted|attempt'):
        tasks.claim(second['task_id'], 'reconciler')
    reconciled = tasks._load(first['task_id'])
    assert reconciled['status'] == 'exhausted'
    assert reconciled['lease'] is None
    assert reconciled['attempts'][-1]['status'] == 'interrupted'
    assert tasks._load(second['task_id'])['status'] == 'prepared'


def test_inconsistent_published_testing_attempt_is_preserved(workspace):
    first = tasks.prepare(VA)
    manifest = tasks.claim(first['task_id'], 'first-worker')
    manifest['attempts'] = [{'number': 1, 'owner': 'first-worker', 'status': 'testing'}]
    manifest['status'] = 'published'
    manifest['lease']['expires_at'] = time.time() - 1
    tasks.save(manifest)
    workspace['harness'] = 'new-fingerprint'
    second = tasks.prepare(VA)
    with pytest.raises(ValueError, match='manual reconciliation'):
        tasks.claim(second['task_id'], 'second-worker')
    assert tasks._load(first['task_id'])['status'] == 'published'
    assert tasks._load(first['task_id'])['attempts'][-1]['status'] == 'testing'


@pytest.mark.parametrize('expiry', [None, 'not-a-time', True, float('nan'), float('inf')])
def test_malformed_other_lease_expiration_is_not_assumed_expired(workspace, expiry):
    first = tasks.prepare(VA)
    manifest = tasks.claim(first['task_id'], 'first-worker')
    manifest['lease']['expires_at'] = expiry
    tasks.save(manifest)
    workspace['harness'] = 'new-fingerprint'
    second = tasks.prepare(VA)
    with pytest.raises(ValueError, match='lease expiration'):
        tasks.claim(second['task_id'], 'second-worker')
    assert tasks._load(first['task_id'])['status'] == 'claimed'
    assert tasks._load(second['task_id'])['status'] == 'prepared'


def test_missing_expiry_does_not_reconcile_testing_attempt(workspace):
    packet = tasks.prepare(VA)
    manifest = tasks.claim(packet['task_id'], 'first-worker')
    manifest['attempts'] = [{'number': 1, 'owner': 'first-worker', 'status': 'testing'}]
    del manifest['lease']['expires_at']
    tasks.save(manifest)
    with pytest.raises(ValueError, match='lease expiration'):
        tasks.claim(packet['task_id'], 'reconciler')
    retained = tasks._load(packet['task_id'])
    assert retained['status'] == 'claimed'
    assert retained['attempts'][-1]['status'] == 'testing'


def put_claim(owner):
    expires = (datetime.now(timezone.utc) + timedelta(minutes=5)).isoformat()
    with work.locked():
        work._write_claims({'fn_00401000': {'owner': owner, 'expires': expires}})


def test_submission_rejects_other_shared_owner_before_reserving_attempt(workspace):
    packet = tasks.prepare(VA)
    leased = tasks.claim(packet['task_id'], 'first-worker')
    (env.ROOT / leased['candidate_path']).write_text('void cm_fn_00401000(void) {}')
    put_claim('other-worker')
    with pytest.raises(ValueError, match='claim|owner'):
        tasks.submit(packet['task_id'], 'first-worker', leased['lease']['token'])
    assert tasks._load(packet['task_id'])['attempts'] == []


def test_submission_rechecks_shared_owner_before_committing_success(workspace, monkeypatch):
    packet = tasks.prepare(VA)
    leased = tasks.claim(packet['task_id'], 'first-worker')
    (env.ROOT / leased['candidate_path']).write_text('void cm_fn_00401000(void) {}')
    def gate(*args, **kwargs):
        put_claim('other-worker')
        return {'status': 'pass'}
    monkeypatch.setattr(tasks.recover, 'test_c', gate)
    with pytest.raises(ValueError, match='claim|owner'):
        tasks.submit(packet['task_id'], 'first-worker', leased['lease']['token'])
    manifest = tasks._load(packet['task_id'])
    assert manifest['status'] != 'submitted'
    assert manifest['attempts'][-1]['status'] == 'failed'
