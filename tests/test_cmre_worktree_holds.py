"""Publication and review must respect unresolved per-function hard holds."""
import json
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from threading import Barrier
from types import SimpleNamespace

import pytest

from cmre import coordination, env, queue, tasks, work, worktrees
from test_cmre_tasks import VA, workspace
from test_cmre_worktrees import candidate, worker


def submitted(worker):
    claimed = worktrees.claim(worker, VA, 'translator')
    source = candidate(worker)
    result = worktrees.submit(worker, VA)
    report = worker / result['report']
    hashes = {'source_sha256': env.digest(source), 'report_sha256': env.digest(report)}
    return claimed, result, source, hashes


def review(worker, hashes):
    return worktrees.review(worker, VA, reviewer='independent',
                            reason='Checked original instructions and raw boundaries', **hashes)


def hold():
    return work.update(VA, 'harness-blocker', 'Nested fault gap',
                       'Establish raw fault evidence', 'nested-fault')


def allow_accept(monkeypatch, source):
    def accept(va, tier):
        assert (va, tier) == (VA, 'c')
        assert (env.C_DIR / source.name).read_bytes() == source.read_bytes()
        path = env.LEDGER_DIR / 'fn_00401000.json'
        record = json.loads(path.read_text())
        record['c'] = {'source_sha256': env.digest(source)}
        coordination.atomic_json(path, record)
        return True, {'status': 'accepted'}
    monkeypatch.setattr(tasks.recover, 'accept', accept)


@pytest.mark.parametrize('state', ['harness-blocker', 'evidence-needed'])
def test_hard_hold_after_submission_blocks_new_review_and_direct_publication(worker, state, monkeypatch):
    _, result, source, hashes = submitted(worker)
    work.update(VA, state, 'Boundary gap', 'Resolve named boundary', 'gap')
    with pytest.raises(ValueError, match='hold|block|evidence|retry'):
        review(worker, hashes)
    monkeypatch.setattr(tasks.recover, 'accept', lambda *a, **k: pytest.fail('hard hold allowed acceptance'))
    with pytest.raises(ValueError, match='hold|block|evidence|retry|review'):
        tasks.publish(result['task_id'])
    assert not (env.C_DIR / source.name).exists()
    assert tasks.status(result['task_id'])['status'] == 'submitted'
    assert len(tasks.status(result['task_id'])['attempts']) == 1


def test_hold_after_review_requires_fresh_review_even_after_retry(worker, monkeypatch):
    _, result, source, hashes = submitted(worker)
    review(worker, hashes)
    hold()
    with pytest.raises(ValueError, match='hold|block|evidence|retry'):
        worktrees.publish(worker, VA)
    assert not (env.C_DIR / source.name).exists()
    work.retry(VA, 'Nested fault boundary independently resolved')
    with pytest.raises(ValueError, match='review|stale'):
        worktrees.publish(worker, VA)
    assert tasks.status(result['task_id'])['status'] == 'submitted'
    review(worker, hashes)
    allow_accept(monkeypatch, source)
    assert worktrees.publish(worker, VA)['status'] == 'published'


def test_direct_revise_same_bytes_requires_review_of_new_attempt(worker, monkeypatch):
    original_claim, result, source, hashes = submitted(worker)
    first_review = review(worker, hashes)
    task_id = result['task_id']
    tasks.revise(task_id, 'Recheck the same source against instruction evidence')
    work.release(original_claim['owner'], [VA])
    claim = tasks.claim(task_id, 'direct-translator')
    second = tasks.submit(task_id, 'direct-translator', claim['lease']['token'])
    assert len(second['attempts']) == 2
    assert second['attempts'][-1]['candidate_sha256'] == hashes['source_sha256']
    assert second['attempts'][-1]['report_sha256'] == hashes['report_sha256']
    monkeypatch.setattr(tasks.recover, 'accept', lambda *a, **k: pytest.fail('unreviewed acceptance'))
    with pytest.raises(ValueError, match='worktree|review'):
        tasks.publish(task_id)
    worktrees.sync(worker, VA)
    assert first_review['attempt_number'] == 1
    with pytest.raises(ValueError, match='review|stale'):
        worktrees.publish(worker, VA)
    assert not (env.C_DIR / source.name).exists()
    assert review(worker, hashes)['attempt_number'] == 2
    allow_accept(monkeypatch, source)
    assert worktrees.publish(worker, VA)['status'] == 'published'


def test_direct_publish_cannot_bypass_worktree_review_after_retry(worker, monkeypatch):
    _, result, source, _ = submitted(worker)
    task_id = result['task_id']
    monkeypatch.setattr(tasks.recover, 'accept', lambda *a, **k: pytest.fail('unreviewed acceptance'))
    with pytest.raises(ValueError, match='worktree|review'):
        tasks.publish(task_id)
    hold()
    work.retry(VA, 'Nested fault boundary independently resolved')
    with pytest.raises(ValueError, match='worktree|review'):
        tasks.publish(task_id)
    assert tasks.status(task_id)['status'] == 'submitted'
    assert not (env.C_DIR / source.name).exists()


@pytest.mark.parametrize('status', ['submitted', 'publishing'])
def test_pending_task_prevents_new_queue_claim_atomically(worker, status):
    claim, result, _, _ = submitted(worker)
    work.release(claim['owner'], [VA])
    if status == 'publishing':
        manifest = tasks._load(result['task_id'])
        manifest['status'] = status
        tasks.save(manifest)
    with pytest.raises(ValueError, match='pending|submitted|publishing'):
        work.claim([0x401100, VA], 'another-worker')
    assert work.claims() == {}


def test_queue_claim_rejects_task_lease_missing_expiration(workspace):
    packet = tasks.prepare(VA)
    manifest = tasks.claim(packet['task_id'], 'original')
    manifest['lease'].pop('expires_at')
    tasks.save(manifest)
    with pytest.raises(ValueError, match='lease|expir|invalid'):
        work.claim([VA], 'other')
    assert work.claims() == {}


def test_direct_task_budget_survives_changed_input_fingerprint(workspace):
    first = tasks.prepare(VA)
    lease = tasks.claim(first['task_id'], 'first')
    for _ in range(4):
        tasks.submit(first['task_id'], 'first', lease['lease']['token'], outcome='failed')
    tasks.release_claim(first['task_id'], 'first', lease['lease']['token'], reason='Worker completed')
    workspace['harness'] = 'second'
    second = tasks.prepare(VA)
    assert second['task_id'] != first['task_id']
    lease2 = tasks.claim(second['task_id'], 'second')
    tasks.submit(second['task_id'], 'second', lease2['lease']['token'], outcome='failed')
    with pytest.raises(ValueError, match='five|budget|exhaust'):
        tasks.submit(second['task_id'], 'second', lease2['lease']['token'], outcome='failed')
    assert len(tasks.status(first['task_id'])['attempts']) == 4
    assert len(tasks.status(second['task_id'])['attempts']) == 1


def test_two_direct_tasks_cannot_both_reserve_the_fifth_attempt(workspace, monkeypatch):
    first = tasks.prepare(VA)
    lease1 = tasks.claim(first['task_id'], 'first')
    tasks.release_claim(first['task_id'], 'first', lease1['lease']['token'], reason='Worker completed')
    workspace['harness'] = 'second'
    second = tasks.prepare(VA)
    lease2 = tasks.claim(second['task_id'], 'second')
    # Simulate an older retained conflicting lease to isolate reservation safety
    # from the newer claim-time exclusion and input-staleness gate.
    old = tasks._load(first['task_id'])
    old.update(status='claimed', lease=lease1['lease'])
    tasks.save(old)
    monkeypatch.setattr(tasks, 'validate_current', lambda manifest: None)
    for _ in range(4):
        work.update(VA, 'translation-defect', 'Historical failed validation', report={'status': 'fail'})
    barrier = Barrier(2)

    def reserve(packet, lease, owner):
        barrier.wait(timeout=5)
        try:
            tasks.submit(packet['task_id'], owner, lease['lease']['token'], outcome='failed')
            return 'reserved'
        except (ValueError, coordination.BusyError):
            return 'rejected'

    with ThreadPoolExecutor(max_workers=2) as executor:
        a = executor.submit(reserve, first, lease1, 'first')
        b = executor.submit(reserve, second, lease2, 'second')
        assert sorted([a.result(timeout=10), b.result(timeout=10)]) == ['rejected', 'reserved']
    assert sum(len(tasks.status(packet['task_id'])['attempts']) for packet in (first, second)) == 1


def test_multi_claim_acquires_distinct_attempt_fences_in_address_order(workspace, monkeypatch):
    original = work.attempt_fence
    acquired = []

    def recorded(va):
        acquired.append(va)
        return original(va)

    monkeypatch.setattr(work, 'attempt_fence', recorded)
    work.claim([0x401100, VA, VA], 'worker')
    assert acquired == [VA, 0x401100]


@pytest.mark.parametrize('lane', ['work', 'queue', 'worktree'])
def test_claim_lane_cannot_cross_active_attempt_fence(worker, monkeypatch, lane):
    monkeypatch.setattr(queue, 'c_frontier', lambda *args, **kwargs: [SimpleNamespace(va=VA)])
    with work.attempt_fence(VA):
        if lane == 'queue':
            assert queue.claim_next(1, 'other') == []
        else:
            with pytest.raises(coordination.BusyError):
                if lane == 'work':
                    work.claim([VA], 'other')
                else:
                    worktrees.claim(worker, VA, 'other')
    assert work.claims() == {}


def test_queue_skips_busy_candidate_and_claims_free_candidate(workspace, monkeypatch):
    pool = [SimpleNamespace(va=VA), SimpleNamespace(va=0x401100)]
    monkeypatch.setattr(queue, 'c_frontier', lambda *args, **kwargs: pool)
    with work.attempt_fence(VA):
        assert [item.va for item in queue.claim_next(1, 'other')] == [0x401100]


def test_overlapping_queue_pools_claim_distinct_candidates(workspace, monkeypatch):
    pool = [SimpleNamespace(va=VA), SimpleNamespace(va=0x401100)]
    barrier = Barrier(2)

    def frontier(*args, **kwargs):
        barrier.wait(timeout=5)
        return pool

    monkeypatch.setattr(queue, 'c_frontier', frontier)
    with ThreadPoolExecutor(max_workers=2) as executor:
        results = list(executor.map(lambda owner: queue.claim_next(1, owner), ['first', 'second']))
    assert sorted(item.va for group in results for item in group) == [VA, 0x401100]


def test_published_is_idempotent_despite_later_hold(worker, monkeypatch):
    _, _, source, hashes = submitted(worker)
    review(worker, hashes)
    allow_accept(monkeypatch, source)
    assert worktrees.publish(worker, VA)['status'] == 'published'
    hold()
    assert worktrees.publish(worker, VA)['status'] == 'published'
    assert (env.C_DIR / source.name).read_bytes() == source.read_bytes()
    assert work.read()['functions']['fn_00401000']['state'] == 'harness-blocker'


@pytest.mark.parametrize('accepted', [False, True])
def test_publishing_reconciliation_ignores_later_hold(worker, accepted):
    _, result, source, hashes = submitted(worker)
    review(worker, hashes)
    destination = env.C_DIR / source.name
    destination.write_bytes(source.read_bytes())
    path = tasks.task_dir(result['task_id']) / 'task.json'
    manifest = json.loads(path.read_text())
    manifest['status'] = 'publishing'
    manifest['publication'] = {'previous_sha256': None, 'candidate_sha256': env.digest(source)}
    tasks.save(manifest)
    if accepted:
        ledger_path = env.LEDGER_DIR / 'fn_00401000.json'
        record = json.loads(ledger_path.read_text())
        record['c'] = {'source_sha256': env.digest(source)}
        coordination.atomic_json(ledger_path, record)
    hold()
    result = worktrees.publish(worker, VA)
    assert result['status'] == ('published' if accepted else 'publication-failed')
    assert destination.exists() is accepted
    assert work.read()['functions']['fn_00401000']['state'] == 'harness-blocker'


def test_hard_hold_setter_cannot_enter_during_acceptance(worker, monkeypatch):
    """A second OS process inherits the writer token but must not cross the fence."""
    _, _, source, hashes = submitted(worker)
    review(worker, hashes)
    observed = []
    script = '''
import sys
from pathlib import Path
from types import SimpleNamespace
from cmre import coordination, env, inventory, work
root = Path(sys.argv[1])
va = int(sys.argv[2])
env.ROOT, env.ANALYSIS = root, root / 'analysis'
env.C_DIR, env.HINTS_DIR, env.RECOVERY = root / 'recovery/c', root / 'recovery/hints', root / 'recovery'
inventory.load = lambda: SimpleNamespace(functions={0x401000: SimpleNamespace(), 0x401100: SimpleNamespace()})
try:
    work.update(va, 'harness-blocker', 'Concurrent boundary gap', 'Investigate boundary')
except coordination.BusyError:
    print('blocked', flush=True)
else:
    print('entered', flush=True)
'''
    def concurrent_hold(va):
        process = subprocess.Popen([sys.executable, '-c', script, str(env.ROOT), str(va)],
                                   stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        try:
            stdout, stderr = process.communicate(timeout=5)
        except subprocess.TimeoutExpired:
            process.kill()
            process.communicate(timeout=5)
            return 'waited'
        else:
            assert process.returncode == 0, stderr
            return stdout.strip()
    def accept(va, tier):
        observed.append(concurrent_hold(VA))
        assert observed[-1] in ('blocked', 'waited')
        assert work.read()['functions']['fn_00401000']['state'] != 'harness-blocker'
        assert concurrent_hold(VA + 0x100) == 'entered'
        return accepted(va, tier)
    allow_accept(monkeypatch, source)
    accepted = tasks.recover.accept
    monkeypatch.setattr(tasks.recover, 'accept', accept)
    assert worktrees.publish(worker, VA)['status'] == 'published'
    assert observed in (['blocked'], ['waited'])
