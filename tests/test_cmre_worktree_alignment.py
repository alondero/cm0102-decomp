"""Explicit alignment copies clean tooling without replacing worker work."""
import argparse
import json
import subprocess

import pytest

from cmre import env, worktrees
from test_cmre_tasks import workspace


def git(root, *args):
    result = subprocess.run(['git', '-C', str(root), *args], capture_output=True, text=True)
    assert result.returncode == 0, result.stderr
    return result.stdout


@pytest.fixture
def trees(workspace, tmp_path, monkeypatch):
    root = env.ROOT
    (root / 'cmre').mkdir()
    (root / 'cmre/a.py').write_bytes(b'old a\n')
    (root / 'cmre/z.py').write_bytes(b'old z\n')
    (root / '.gitignore').write_text('analysis/\nCM0102/\n')
    git(root, 'init', '-q')
    git(root, 'add', '.gitignore', 'cmre', 'recovery/include')
    git(root, '-c', 'user.name=Test', '-c', 'user.email=test@example.invalid',
        'commit', '-qm', 'fixture')
    worker = tmp_path.parent / (tmp_path.name + '-alignment-worker')
    git(root, 'worktree', 'add', '--detach', str(worker))
    (root / 'cmre/a.py').write_bytes(b'current a\r\n')
    (root / 'cmre/z.py').write_bytes(b'current z\n')
    monkeypatch.setattr(worktrees, '_pins', lambda: None)
    return root, worker


def test_alignment_is_explicit_and_preserves_unrelated_candidate(trees):
    root, worker = trees
    candidate = worker / 'recovery/c/fn_00401000.c'
    candidate.parent.mkdir(parents=True)
    candidate.write_bytes(b'worker candidate\n')
    with pytest.raises(ValueError, match='snapshot differs'):
        worktrees.bootstrap(worker)
    assert (worker / 'cmre/a.py').read_bytes() == b'old a\n'
    state = worktrees.bootstrap(worker, align=True)
    assert worktrees._code_identity(worker) == worktrees._code_identity(root) == state['code']
    assert candidate.read_bytes() == b'worker candidate\n'
    assert (worker / 'cmre/a.py').read_bytes() == b'current a\r\n'
    assert not (worker / 'CM0102/Game').exists()
    assert not (worker / 'recovery/ledger').exists()
    assert worktrees.bootstrap(worker, align=True) == state


@pytest.mark.parametrize('conflict', ['modified', 'staged', 'untracked', 'ignored', 'deleted'])
def test_alignment_preflights_every_copy_before_writing(trees, conflict):
    root, worker = trees
    destination = worker / 'cmre/z.py'
    if conflict in ('untracked', 'ignored'):
        destination = worker / 'cmre/new.py'
        (root / 'cmre/new.py').write_bytes(b'coordinator addition\n')
        if conflict == 'ignored':
            (worker / '.gitignore').write_text('cmre/new.py\n')
    if conflict == 'deleted':
        destination.unlink()
    else:
        destination.write_bytes(b'worker edits\n')
    if conflict == 'staged':
        git(worker, 'add', 'cmre/z.py')
    before = destination.read_bytes() if destination.exists() else None
    with pytest.raises(ValueError, match='worker edits'):
        worktrees.bootstrap(worker, align=True)
    assert (worker / 'cmre/a.py').read_bytes() == b'old a\n'
    assert (destination.read_bytes() if destination.exists() else None) == before
    assert not worktrees._state_path(worker).exists()


def test_alignment_adds_missing_tooling_and_retries_interrupted_copy(trees, monkeypatch):
    root, worker = trees
    (root / 'cmre/new.py').write_bytes(b'new module\n')
    write = worktrees.coordination.atomic_bytes
    def interrupt(path, payload):
        if path == worker / 'cmre/z.py':
            raise OSError('interrupted copy')
        write(path, payload)
    monkeypatch.setattr(worktrees.coordination, 'atomic_bytes', interrupt)
    with pytest.raises(OSError, match='interrupted copy'):
        worktrees.bootstrap(worker, align=True)
    assert not worktrees._state_path(worker).exists()
    monkeypatch.setattr(worktrees.coordination, 'atomic_bytes', write)
    worktrees.bootstrap(worker, align=True)
    assert (worker / 'cmre/new.py').read_bytes() == b'new module\n'
    assert worktrees._code_identity(worker) == worktrees._code_identity(root)


def test_alignment_rejects_stale_binding_without_touching_tooling_or_claims(trees):
    root, worker = trees
    state = worktrees.bootstrap(worker, align=True)
    state['functions'] = {'fn_00401000': {'task_id': 'retained-attempts', 'owner': 'owner'}}
    worktrees._save(worker, state)
    retained = worktrees._state_path(worker).read_bytes()
    (root / 'cmre/a.py').write_bytes(b'newer coordinator\n')
    with pytest.raises(ValueError, match='Stale'):
        worktrees.bootstrap(worker, align=True)
    assert (worker / 'cmre/a.py').read_bytes() == b'current a\r\n'
    assert worktrees._state_path(worker).read_bytes() == retained


def test_alignment_rejects_orphan_local_binding(trees):
    root, worker = trees
    binding = worker / worktrees.LOCAL / 'binding.json'
    binding.parent.mkdir(parents=True)
    binding.write_text(json.dumps({'coordinator': 'other coordinator', 'binding': 'retained'}))
    with pytest.raises(ValueError, match='binding'):
        worktrees.bootstrap(worker, align=True)
    assert (worker / 'cmre/a.py').read_bytes() == b'old a\n'
    assert json.loads(binding.read_text())['binding'] == 'retained'


def test_alignment_validates_pins_and_git_before_copy(trees, monkeypatch):
    root, worker = trees
    def bad_pins():
        raise ValueError('Pinned asset hash mismatch')
    monkeypatch.setattr(worktrees, '_pins', bad_pins)
    with pytest.raises(ValueError, match='Pinned'):
        worktrees.bootstrap(worker, align=True)
    assert (worker / 'cmre/a.py').read_bytes() == b'old a\n'
    monkeypatch.setattr(worktrees, '_pins', lambda: None)
    identity = worktrees._git_identity
    monkeypatch.setattr(worktrees, '_git_identity', lambda path: {
        **identity(path), 'common': str(path)})
    with pytest.raises(ValueError, match='Git common'):
        worktrees.bootstrap(worker, align=True)
    assert (worker / 'cmre/a.py').read_bytes() == b'old a\n'


def test_alignment_rejects_redirected_tooling(trees, tmp_path):
    root, worker = trees
    destination = worker / 'cmre/z.py'
    elsewhere = tmp_path / 'outside.py'
    elsewhere.write_bytes(b'outside\n')
    destination.unlink()
    try:
        destination.symlink_to(elsewhere)
    except OSError:
        pytest.skip('symlinks unavailable')
    with pytest.raises(ValueError, match='redirect'):
        worktrees.bootstrap(worker, align=True)
    assert elsewhere.read_bytes() == b'outside\n'
    assert (worker / 'cmre/a.py').read_bytes() == b'old a\n'


@pytest.mark.parametrize('flag', ['--assume-unchanged', '--skip-worktree'])
def test_alignment_preserves_edits_hidden_from_git_status(trees, flag):
    root, worker = trees
    git(worker, 'update-index', flag, 'cmre/z.py')
    (worker / 'cmre/z.py').write_bytes(b'hidden worker edits\n')
    with pytest.raises(ValueError, match='worker edits'):
        worktrees.bootstrap(worker, align=True)
    assert (worker / 'cmre/a.py').read_bytes() == b'old a\n'
    assert (worker / 'cmre/z.py').read_bytes() == b'hidden worker edits\n'


def test_alignment_preserves_extra_worker_modules(trees):
    root, worker = trees
    extra = worker / 'cmre/worker_only.py'
    extra.write_bytes(b'worker extension\n')
    with pytest.raises(ValueError, match='extra tooling'):
        worktrees.bootstrap(worker, align=True)
    assert extra.read_bytes() == b'worker extension\n'
    assert (worker / 'cmre/a.py').read_bytes() == b'old a\n'


def test_alignment_cli_option_reaches_bootstrap(trees, capsys):
    root, worker = trees
    parser = argparse.ArgumentParser()
    worktrees.add_commands(parser.add_subparsers())
    arguments = parser.parse_args(['worktree', 'bootstrap', str(worker), '--align'])
    assert arguments.func(arguments) == 0
    result = json.loads(capsys.readouterr().out)
    assert result['worker'] == str(worker)
    assert worktrees._code_identity(worker) == worktrees._code_identity(root)
