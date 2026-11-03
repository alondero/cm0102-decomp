"""Explicit diagnostic sources must not bypass active worker attempts."""
import json
import os
import time
from datetime import datetime, timedelta, timezone
from pathlib import Path

import pytest

from cmre import cli, coordination, env, recover, tasks, work

VA = 0x00401000


@pytest.fixture
def isolated(tmp_path, monkeypatch):
    root = tmp_path / 'coordinator'
    for name, path in [('ROOT', root), ('ANALYSIS', root / 'analysis'),
                       ('C_DIR', root / 'recovery/c'), ('HINTS_DIR', root / 'recovery/hints')]:
        path.mkdir(parents=True, exist_ok=True)
        monkeypatch.setattr(env, name, path)
    calls = []
    def test_c(va, overrides, fresh, *, source, profile):
        calls.append(('test', va, source))
        return {'status': 'pass'}
    def explain_c(va, case, overrides, *, source, profile, opt, window):
        calls.append(('explain', va, source))
        return {'status': 'ok'}
    monkeypatch.setattr(recover, 'test_c', test_c)
    monkeypatch.setattr(recover, 'explain_c', explain_c)
    monkeypatch.setattr(cli, '_print_report', lambda *a: None)
    monkeypatch.setattr(work, 'record_test', lambda *a: pytest.fail('diagnostic must not clear or record work state'))
    return root, calls


def source_args(command, source):
    return [command, 'fn_00401000', '--c', '--source', str(source)]


@pytest.mark.parametrize('command', ['test', 'explain'])
def test_staged_candidate_and_normalized_path_are_rejected(isolated, command):
    root, calls = isolated
    path = root / 'recovery/c/tasks/c-00401000-example/fn_00401000.c'
    path.parent.mkdir(parents=True)
    path.write_text('candidate')
    assert cli.main(source_args(command, path.parent / '..' / path.parent.name / path.name)) == 2
    assert not calls


def test_symlink_to_staged_candidate_is_rejected(isolated):
    root, calls = isolated
    candidate = root / 'recovery/c/tasks/c-00401000-example/fn_00401000.c'
    candidate.parent.mkdir(parents=True)
    candidate.write_text('candidate')
    alias = root / 'alias.c'
    try:
        alias.symlink_to(candidate)
    except OSError:
        pytest.skip('symlink creation unavailable on this host')
    assert cli.main(source_args('test', alias)) == 2
    assert not calls


def test_hardlink_to_staged_candidate_is_rejected(isolated):
    root, calls = isolated
    candidate = root / 'recovery/c/tasks/c-00401000-example/fn_00401000.c'
    candidate.parent.mkdir(parents=True)
    candidate.write_text('candidate')
    alias = root / 'hardlink.c'
    try:
        os.link(candidate, alias)
    except OSError:
        pytest.skip('hard link creation unavailable on this host')
    assert cli.main(source_args('test', alias)) == 2
    assert not calls


def test_explicit_canonical_source_cannot_bypass_work_record(isolated):
    root, calls = isolated
    source = env.C_DIR / 'fn_00401000.c'
    source.write_text('canonical')
    assert cli.main(source_args('test', source)) == 2
    assert not calls


def test_retained_worker_source_rejected_even_without_live_claim(isolated, tmp_path):
    root, calls = isolated
    worker = tmp_path / 'worker'
    path = worker / 'recovery/c/fn_00401000.c'
    path.parent.mkdir(parents=True)
    path.write_text('candidate')
    directory = env.ANALYSIS / 'cmre/worktrees'
    directory.mkdir(parents=True)
    (directory / 'binding.json').write_text(json.dumps({'schema': 'cmre-worktree/1',
        'worker': str(worker), 'coordinator': str(root)}))
    assert cli.main(source_args('test', path)) == 2
    assert not calls


@pytest.mark.parametrize('kind', ['shared', 'task'])
def test_explicit_diagnostic_rejects_live_claim(isolated, kind):
    root, calls = isolated
    source = root / 'diagnostic.c'
    source.write_text('diagnostic')
    if kind == 'shared':
        path = env.ANALYSIS / 'cmre/claims.json'
        path.parent.mkdir(parents=True)
        path.write_text(json.dumps({'schema': 'cmre-claims/1', 'claims': {'fn_00401000': {
            'owner': 'worker', 'expires': (datetime.now(timezone.utc) + timedelta(minutes=10)).isoformat()}}}))
    else:
        path = env.ANALYSIS / 'cmre/tasks/c-00401000-example/task.json'
        path.parent.mkdir(parents=True)
        path.write_text(json.dumps({'va': '0x00401000', 'lease': {'owner': 'worker', 'expires_at': time.time() + 600}}))
    assert cli.main(source_args('test', source)) == 2
    assert not calls


def test_unclaimed_held_diagnostic_remains_available_and_fenced(isolated, monkeypatch):
    root, calls = isolated
    source = root / 'diagnostic.c'
    source.write_text('diagnostic')
    held = env.HINTS_DIR / 'work-state.json'
    held.write_text(json.dumps({'schema': 'cmre-work/1', 'functions': {'fn_00401000': {
        'state': 'evidence-needed', 'reason': 'known hold'}}}))
    before = held.read_bytes()
    def run(va, overrides, fresh, *, source, profile):
        with pytest.raises(coordination.BusyError):
            with work.attempt_fence(va):
                pass
        calls.append(('test', va, source))
        return {'status': 'pass'}
    monkeypatch.setattr(recover, 'test_c', run)
    assert cli.main(source_args('test', source)) == 0
    assert calls == [('test', VA, source.resolve())]
    assert held.read_bytes() == before


def test_expired_task_lease_allows_diagnostic(isolated):
    root, calls = isolated
    source = root / 'diagnostic.c'
    source.write_text('diagnostic')
    path = env.ANALYSIS / 'cmre/tasks/c-00401000-example/task.json'
    path.parent.mkdir(parents=True)
    path.write_text(json.dumps({'va': '0x00401000', 'lease': {'owner': 'old', 'expires_at': time.time() - 10}}))
    assert cli.main(source_args('test', source)) == 0
    assert calls == [('test', VA, source.resolve())]


@pytest.mark.parametrize('kind', ['shared', 'task'])
def test_claim_cannot_start_during_explicit_diagnostic(isolated, monkeypatch, kind):
    root, calls = isolated
    source = root / 'diagnostic.c'
    source.write_text('diagnostic')
    monkeypatch.setattr(tasks, '_claim_with_work_lock', lambda *args, **kwargs: {'status': 'claimed'})
    def run(va, overrides, fresh, *, source, profile):
        with pytest.raises(coordination.BusyError):
            if kind == 'shared':
                work.claim([va], 'worker')
            else:
                tasks.claim('c-00401000-0123456789abcdef', 'worker')
        calls.append(('test', va, source))
        return {'status': 'pass'}
    monkeypatch.setattr(recover, 'test_c', run)
    assert cli.main(source_args('test', source)) == 0
    assert len(calls) == 1
