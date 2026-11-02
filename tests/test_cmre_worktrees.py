"""Same-machine isolation: authoritative claims, evidence and serial acceptance."""
import json
from datetime import datetime, timezone
import os
import subprocess
import sys
from types import SimpleNamespace

import pytest

from cmre import coordination, env, tasks, work, worktrees
from test_cmre_tasks import VA, workspace  # shared synthetic instruction fixture


@pytest.fixture
def worker(workspace, tmp_path, monkeypatch):
    monkeypatch.setattr(env, 'HINTS_DIR', env.ROOT / 'recovery/hints')
    worker = tmp_path.parent / (tmp_path.name + '-worker')
    worker.mkdir()
    monkeypatch.setattr(worktrees, '_git_identity', lambda root: {'common': 'same', 'head': 'first'})
    monkeypatch.setattr(worktrees, '_code_identity', lambda root: 'same-code')
    monkeypatch.setattr(worktrees, '_pins', lambda: None)
    worktrees.bootstrap(worker)
    return worker


def candidate(worker):
    path = worker / 'recovery/c/fn_00401000.c'
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text('void cm_fn_00401000(void) {}\n')
    return path


def test_worker_bootstrap_is_local_and_claim_collides_with_shared_checkout(worker):
    result = worktrees.claim(worker, VA, 'translator')
    assert result['status'] == 'claimed'
    assert not (worker / 'CM0102/Game').exists()
    assert not (worker / 'recovery/ledger').exists()
    assert (worker / 'CM0102/Analysis/cmre/worktree/fn_00401000/packet.json').is_file()
    assert work.claims()['fn_00401000']['owner'] == result['owner']
    with pytest.raises(ValueError, match='claimed'):
        work.claim([VA], 'shared-checkout')
    with pytest.raises(ValueError, match='claim'):
        worktrees.claim(worker, VA, 'duplicate')


def test_worker_cannot_replace_an_existing_shared_claim(worker):
    work.claim([VA], 'existing')
    with pytest.raises(ValueError, match='claimed'):
        worktrees.claim(worker, VA, 'translator')
    assert work.claims()['fn_00401000']['owner'] == 'existing'


def test_submission_requires_review_and_imports_only_bound_source(worker, monkeypatch):
    worktrees.claim(worker, VA, 'translator')
    source = candidate(worker)
    result = worktrees.submit(worker, VA)
    assert result['status'] == 'submitted'
    assert not (env.C_DIR / source.name).exists()
    with pytest.raises(ValueError, match='review'):
        worktrees.publish(worker, VA)
    report = worker / result['report']
    worktrees.review(worker, VA, reviewer='reviewer', reason='Checked against instructions',
                     source_sha256=env.digest(source), report_sha256=env.digest(report))

    def accept(va, tier):
        assert va == VA and tier == 'c'
        assert (env.C_DIR / source.name).read_bytes() == source.read_bytes()
        record = json.loads((env.LEDGER_DIR / 'fn_00401000.json').read_text())
        record['c'] = {'source_sha256': env.digest(source)}
        coordination.atomic_json(env.LEDGER_DIR / 'fn_00401000.json', record)
        return True, {'status': 'accepted'}

    monkeypatch.setattr(tasks.recover, 'accept', accept)
    assert worktrees.publish(worker, VA)['status'] == 'published'
    assert worktrees.publish(worker, VA)['status'] == 'published'
    assert 'fn_00401000' not in work.claims()


def test_source_report_and_snapshot_drift_reject_review_or_submission(worker, workspace):
    worktrees.claim(worker, VA, 'translator')
    source = candidate(worker)
    result = worktrees.submit(worker, VA)
    report = worker / result['report']
    hashes = dict(source_sha256=env.digest(source), report_sha256=env.digest(report))
    source.write_text('changed')
    with pytest.raises(ValueError, match='changed'):
        worktrees.review(worker, VA, reviewer='reviewer', reason='reviewed', **hashes)
    source.write_text('void cm_fn_00401000(void) {}\n')
    report.write_text('{}')
    with pytest.raises(ValueError, match='report'):
        worktrees.review(worker, VA, reviewer='reviewer', reason='reviewed', **hashes)
    workspace['harness'] = 'new'
    with pytest.raises(ValueError, match='Stale'):
        worktrees.submit(worker, VA)


def test_five_attempts_survive_expiry_reclaim_and_snapshot_replacement(worker, workspace, monkeypatch):
    worktrees.claim(worker, VA, 'translator')
    candidate(worker)
    monkeypatch.setattr(tasks.recover, 'test_c', lambda *a, **k: {'status': 'fail'})
    for _ in range(5):
        result = worktrees.submit(worker, VA)
    assert result['status'] == 'exhausted'
    assert work.read()['functions']['fn_00401000']['failures'] == 5
    workspace['harness'] = 'new'
    with pytest.raises(ValueError, match='five|exhausted'):
        worktrees.claim(worker, VA, 'replacement')


def test_expired_claim_and_changed_code_reject_worker(worker, monkeypatch):
    manifest = worktrees.claim(worker, VA, 'translator', seconds=1)
    candidate(worker)
    monkeypatch.setattr(tasks.time, 'time', lambda: manifest['expires_at'] + 1)
    with pytest.raises(ValueError, match='expired'):
        worktrees.submit(worker, VA)
    monkeypatch.setattr(worktrees, '_code_identity', lambda root: 'changed')
    with pytest.raises(ValueError, match='snapshot'):
        worktrees.claim(worker, VA, 'new')


def test_path_redirection_is_rejected(worker, tmp_path):
    worktrees.claim(worker, VA, 'translator')
    source = candidate(worker)
    elsewhere = tmp_path / 'elsewhere.c'
    elsewhere.write_text('unreviewed')
    source.unlink()
    try:
        source.symlink_to(elsewhere)
    except OSError:
        pytest.skip('symlinks unavailable')
    with pytest.raises(ValueError, match='redirect'):
        worktrees.submit(worker, VA)


def test_bootstrap_rejects_unrelated_git_and_bad_pins(worker, monkeypatch, tmp_path):
    other = worker.parent / (worker.name + '-other')
    other.mkdir()
    monkeypatch.setattr(worktrees, '_git_identity', lambda root: {'common': str(root), 'head': 'first'})
    with pytest.raises(ValueError, match='Git'):
        worktrees.bootstrap(other)


def test_interrupted_submit_counts_attempt_and_can_retry_after_lease_expiry(worker, monkeypatch):
    claimed = worktrees.claim(worker, VA, 'translator', seconds=1)
    candidate(worker)
    def interrupt(*a, **k):
        raise KeyboardInterrupt('process ended')
    monkeypatch.setattr(tasks.recover, 'test_c', interrupt)
    with pytest.raises(KeyboardInterrupt):
        worktrees.submit(worker, VA)
    task = tasks.status(claimed['task_id'])
    assert len(task['attempts']) == 1
    monkeypatch.setattr(tasks.time, 'time', lambda: claimed['expires_at'] + 2)
    monkeypatch.setattr(work, '_now', lambda: datetime.fromtimestamp(
        claimed['expires_at'] + 2, timezone.utc))
    worktrees.claim(worker, VA, 'retry')
    monkeypatch.setattr(tasks.recover, 'test_c', lambda *a, **k: {'status': 'pass'})
    assert worktrees.submit(worker, VA)['status'] == 'submitted'
    assert len(tasks.status(claimed['task_id'])['attempts']) == 2


def test_writer_contention_blocks_claim_without_partial_claim(worker, monkeypatch):
    # An unrelated process owns the OS lock, rather than our inherited lease.
    script = ('import sys\nfrom cmre.coordination import writer\n'
              'with writer():\n print("ready", flush=True)\n sys.stdin.read()\n')
    process = subprocess.Popen([sys.executable, '-c', script], stdin=subprocess.PIPE,
                               stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    try:
        assert process.stdout.readline().strip() == 'ready'
        with pytest.raises(coordination.BusyError):
            worktrees.claim(worker, VA, 'translator')
        assert not work.claims()
    finally:
        process.communicate('', timeout=10)


def test_revise_preserves_budget_and_invalidates_review(worker):
    worktrees.claim(worker, VA, 'translator')
    source = candidate(worker)
    result = worktrees.submit(worker, VA)
    worktrees.review(worker, VA, reviewer='reviewer', reason='Reviewed original instructions',
                     source_sha256=env.digest(source), report_sha256=env.digest(worker / result['report']))
    worktrees.revise(worker, VA, reason='Revise the declaration against instruction evidence')
    assert not work.claims()
    worktrees.claim(worker, VA, 'translator')
    assert worktrees.submit(worker, VA)['attempt_count'] == 2
    with pytest.raises(ValueError, match='review'):
        worktrees.publish(worker, VA)


def test_bootstrap_retry_after_interrupted_binding_write_preserves_claims(worker):
    claimed = worktrees.claim(worker, VA, 'translator')
    binding = worker / worktrees.LOCAL / 'binding.json'
    before = binding.read_bytes()
    binding.unlink()
    state = worktrees.bootstrap(worker)
    assert binding.read_bytes() == before
    assert state['functions']['fn_00401000']['task_id'] == claimed['task_id']


def test_real_git_workers_can_validate_disjoint_functions_concurrently(workspace, tmp_path, monkeypatch):
    root = env.ROOT
    monkeypatch.setattr(worktrees, '_pins', lambda: None)
    def git(*args):
        result = subprocess.run(['git', '-C', str(root), *args], capture_output=True, text=True)
        assert result.returncode == 0, result.stderr
        return result.stdout
    monkeypatch.setattr(env, 'HINTS_DIR', root / 'recovery/hints')
    git('init', '-q')
    git('add', 'recovery/include/cmre.h')
    git('-c', 'user.name=Test', '-c', 'user.email=test@example.invalid', 'commit', '-qm', 'fixture')
    workers = [tmp_path.parent / (tmp_path.name + f'-git-{i}') for i in range(2)]
    for i, path in enumerate(workers):
        git('worktree', 'add', '--detach', str(path))
        worktrees.bootstrap(path)
    # Bootstrap normally checks the production pins; this fixture tests the
    # actual Git relationship and isolation with synthetic instruction evidence.
    va2 = VA + 0x10
    function = SimpleNamespace(name='second', body_bytes=1, signature='void second(void)')
    first = tasks.inventory.load().functions[VA]
    monkeypatch.setattr(tasks.inventory, 'load', lambda: SimpleNamespace(functions={VA: first, va2: function}))
    (env.LIFTED_DIR / f'fn_{va2:08x}.S').write_text('ret\n')
    (env.LEDGER_DIR / f'fn_{va2:08x}.json').write_text(json.dumps({'lifted': {}}))
    for i, (path, va) in enumerate(zip(workers, (VA, va2))):
        worktrees.claim(path, va, f'translator-{i}')
        source = path / f'recovery/c/fn_{va:08x}.c'
        source.parent.mkdir(parents=True)
        source.write_text(f'void cm_fn_{va:08x}(void) {{}}\n')
    assert len(work.claims()) == 2
    # Execute concurrently in independent processes. This also exercises OS
    # leases rather than the process-global inherited writer environment.
    script = '''
import json, sys, time
from pathlib import Path
from cmre import env, inventory, tasks, worktrees
root, worker, va = Path(sys.argv[1]), Path(sys.argv[2]), int(sys.argv[3])
env.ROOT, env.ANALYSIS = root, root / 'analysis'
env.C_DIR, env.HINTS_DIR = root / 'recovery/c', root / 'recovery/hints'
tasks.validate_current = lambda manifest: None
inventory.load = lambda: type('Inventory', (), {'functions': {va: None}})()
tasks.recover.test_c = lambda *a, **k: (time.sleep(0.3) or {'status': 'pass'})
print(json.dumps(worktrees.submit(worker, va)))
'''
    child_env = dict(os.environ)
    child_env.pop('CMRE_WRITER_TOKEN', None)
    children = [subprocess.Popen([sys.executable, '-c', script, str(root), str(path), str(va)],
                                 stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, env=child_env)
                for path, va in zip(workers, (VA, va2))]
    for child in children:
        output, error = child.communicate(timeout=15)
        assert child.returncode == 0, error
        assert json.loads(output)['status'] == 'submitted'
    assert not list(env.C_DIR.glob('fn_*.c'))
    assert all((path / worktrees.LOCAL / f'fn_{va:08x}/report-1.json').exists()
               for path, va in zip(workers, (VA, va2)))


@pytest.mark.parametrize('historical_failures', [0, 4])
def test_killed_process_releases_locks_and_retains_reserved_attempt(worker, monkeypatch, historical_failures):
    # Lease time is controlled in both processes: importing the child must not
    # race a one-second lease. OS locks and process termination remain real.
    now = 1_700_000_000.0
    monkeypatch.setattr(tasks.time, 'time', lambda: now)
    monkeypatch.setattr(work, '_now', lambda: datetime.fromtimestamp(now, timezone.utc))
    for _ in range(historical_failures):
        work.update(VA, 'translation-defect', 'Existing shared-checkout failure', report={'status': 'fail'})
    claimed = worktrees.claim(worker, VA, 'translator', seconds=1)
    candidate(worker)
    script = '''
import sys, time
from datetime import datetime, timezone
from pathlib import Path
from cmre import env, tasks, work, worktrees
root, worker, va = Path(sys.argv[1]), Path(sys.argv[2]), int(sys.argv[3])
now = float(sys.argv[4])
tasks.time.time = lambda: now
work._now = lambda: datetime.fromtimestamp(now, timezone.utc)
env.ROOT, env.ANALYSIS = root, root / 'analysis'
env.C_DIR, env.HINTS_DIR = root / 'recovery/c', root / 'recovery/hints'
worktrees._code_identity = lambda root: 'same-code'
worktrees._git_identity = lambda root: {'common': 'same', 'head': 'first'}
tasks.validate_current = lambda manifest: None
def gate(*a, **k):
    print('testing', flush=True)
    time.sleep(30)
tasks.recover.test_c = gate
worktrees.submit(worker, va)
'''
    process = subprocess.Popen([sys.executable, '-c', script, str(env.ROOT), str(worker), str(VA), str(now)],
                               stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    try:
        line = process.stdout.readline().strip()
    finally:
        process.kill()
        remaining, error = process.communicate(timeout=10)
    assert line == 'testing', (line, process.returncode, remaining, error)
    manifest = tasks._load(claimed['task_id'])
    assert manifest['attempts'][0]['status'] == 'testing'
    monkeypatch.setattr(tasks.time, 'time', lambda: claimed['expires_at'] + 2)
    monkeypatch.setattr(work, '_now', lambda: datetime.fromtimestamp(claimed['expires_at'] + 2, timezone.utc))
    if historical_failures:
        with pytest.raises(ValueError, match='exhausted'):
            worktrees.claim(worker, VA, 'retry')
        return
    reclaimed = worktrees.claim(worker, VA, 'retry')
    assert reclaimed['attempt_count'] == 1
    assert tasks._load(claimed['task_id'])['attempts'][0]['status'] == 'interrupted'
    assert worktrees.submit(worker, VA)['attempt_count'] == 2
    assert work.read()['functions']['fn_00401000']['failures'] == 1


def test_pinned_target_and_inventory_hashes_are_checked(tmp_path, monkeypatch):
    target, inventory = tmp_path / 'target', tmp_path / 'inventory'
    target.write_bytes(b'synthetic target')
    inventory.write_bytes(b'synthetic inventory')
    monkeypatch.setattr(env, 'TARGET', target)
    monkeypatch.setattr(env, 'TARGET_SHA256', env.digest(target))
    monkeypatch.setattr(env, 'INVENTORY_SHA256', {inventory: env.digest(inventory)})
    worktrees._pins()
    target.write_bytes(b'changed')
    with pytest.raises(ValueError, match='hash mismatch'):
        worktrees._pins()
    monkeypatch.setattr(env, 'TARGET_SHA256', env.digest(target))
    inventory.write_bytes(b'changed')
    with pytest.raises(ValueError, match='hash mismatch'):
        worktrees._pins()


def test_fifth_success_recovers_interrupted_report_export_without_another_attempt(worker, monkeypatch):
    claimed = worktrees.claim(worker, VA, 'translator')
    candidate(worker)
    monkeypatch.setattr(tasks.recover, 'test_c', lambda *a, **k: {'status': 'fail'})
    for _ in range(4):
        worktrees.submit(worker, VA)
    calls = []
    monkeypatch.setattr(tasks.recover, 'test_c', lambda *a, **k: (calls.append('gate') or {'status': 'pass'}))
    write = coordination.atomic_bytes
    def interrupt_export(path, payload):
        if path == worker / worktrees.LOCAL / 'fn_00401000/report-5.json':
            raise OSError('interrupted export')
        write(path, payload)
    monkeypatch.setattr(coordination, 'atomic_bytes', interrupt_export)
    with pytest.raises(OSError, match='interrupted export'):
        worktrees.submit(worker, VA)
    assert tasks.status(claimed['task_id'])['status'] == 'submitted'
    monkeypatch.setattr(coordination, 'atomic_bytes', write)
    result = worktrees.submit(worker, VA)
    assert result['status'] == 'submitted' and result['attempt_count'] == 5
    assert calls == ['gate']
    assert work.read()['functions']['fn_00401000']['attempts'] == 5
    source = candidate(worker)
    worktrees.review(worker, VA, reviewer='reviewer', reason='Reviewed recovered evidence',
                     source_sha256=env.digest(source), report_sha256=env.digest(worker / result['report']))


@pytest.mark.parametrize('replacement', [False, True])
def test_claim_is_rechecked_after_validation_before_success_commits(worker, monkeypatch, replacement):
    claimed = worktrees.claim(worker, VA, 'translator', seconds=1)
    candidate(worker)
    def gate(*a, **k):
        if replacement:
            work.release(claimed['owner'], [VA])
            work.claim([VA], 'another-worker')
        else:
            monkeypatch.setattr(tasks.time, 'time', lambda: claimed['expires_at'] + 2)
            monkeypatch.setattr(work, '_now', lambda: datetime.fromtimestamp(claimed['expires_at'] + 2, timezone.utc))
        return {'status': 'pass'}
    monkeypatch.setattr(tasks.recover, 'test_c', gate)
    with pytest.raises(ValueError, match='expired|replaced'):
        worktrees.submit(worker, VA)
    assert tasks.status(claimed['task_id'])['status'] != 'submitted'
    with pytest.raises(ValueError, match='validated'):
        worktrees.review(worker, VA, reviewer='reviewer', reason='expired', source_sha256='', report_sha256='')


def test_packet_is_pinned_and_explicit_sync_restores_interrupted_evidence_exports(worker):
    worktrees.claim(worker, VA, 'translator')
    candidate(worker)
    packet = worker / worktrees.LOCAL / 'fn_00401000/packet.json'
    original = packet.read_bytes()
    packet.write_text('{}')
    with pytest.raises(ValueError, match='packet'):
        worktrees.submit(worker, VA)
    worktrees.sync(worker, VA)
    assert packet.read_bytes() == original
    result = worktrees.submit(worker, VA)
    report = worker / result['report']
    report.write_text('{}')
    with pytest.raises(ValueError, match='report'):
        worktrees.submit(worker, VA)
    worktrees.sync(worker, VA)
    assert worktrees.submit(worker, VA)['attempt_count'] == 1
