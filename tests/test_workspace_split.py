import os
import subprocess
import sys
from pathlib import Path

from cmre import env, session_evidence


def _pytest(tmp_path, marked):
    recovery = tmp_path / 'recovery'
    recovery.mkdir()
    (recovery / 'input').write_text('x')
    test = tmp_path / 'test_probe.py'
    mark = 'import pytest\npytestmark = pytest.mark.workspace\n' if marked else ''
    test.write_text(mark + f'def test_read():\n    open({str(recovery / "input")!r}).read()\n')
    child_env = dict(os.environ, PYTHONPATH=str(env.ROOT), PYTEST_DISABLE_PLUGIN_AUTOLOAD='1')
    return subprocess.run([sys.executable, '-m', 'pytest', '-q', '-p', 'cmre.workspace_audit',
                           '--cmre-workspace-audit', '--cmre-recovery-root', str(recovery), str(test)],
                          env=child_env, cwd=tmp_path, capture_output=True, text=True, timeout=30)


def test_audit_fails_unmarked_read(tmp_path):
    result = _pytest(tmp_path, False)
    assert result.returncode != 0 and 'unmarked recovery access' in result.stdout


def test_audit_allows_marked_read(tmp_path):
    result = _pytest(tmp_path, True)
    assert result.returncode == 0, result.stdout + result.stderr


def test_split_keys_are_separate_and_conservative(tmp_path, monkeypatch):
    subprocess.run(['git', 'init', '-q', str(tmp_path)], check=True)
    monkeypatch.setattr(env, 'INVENTORY_SHA256', {})
    monkeypatch.setattr(env, 'tool', lambda name: str(tmp_path / 'compiler'))
    (tmp_path / 'compiler').write_text('compiler')
    for name in ('cmre/code.py', 'tests/test_x.py', 'recovery/c/a.c'):
        path = tmp_path / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text('one')
    def keys():
        return (session_evidence.identity(tmp_path, scope='tooling'),
                session_evidence.identity(tmp_path, scope='workspace'))
    before = keys()
    (tmp_path / 'recovery/c/a.c').write_text('two')
    after = keys()
    assert before[0] == after[0] and before[1] != after[1]
    for name in ('cmre/code.py', 'tests/test_x.py'):
        path = tmp_path / name
        path.write_text('changed')
        changed = keys()
        assert all(a != b for a, b in zip(after, changed))
        path.write_text('one')
    monkeypatch.setenv('CPATH', 'changed')
    assert all(a != b for a, b in zip(after, keys()))


def test_experimental_split_runs_both_audited_groups_without_replacing_full_gate():
    from scripts import cmre_session
    calls = []
    def execute(args):
        calls.append(args)
        return 0
    assert cmre_session.drive('split-audit', [], execute) == 0
    assert calls == [cmre_session.SPLIT_TOOLING, cmre_session.SPLIT_WORKSPACE]
    calls.clear()
    assert cmre_session.drive('check', [], execute) == 0
    assert calls[0] == ('pytest',)


def test_audit_detects_swallowed_stat_probe(tmp_path):
    recovery = tmp_path / 'recovery'
    recovery.mkdir()
    test = tmp_path / 'test_probe.py'
    test.write_text(f'def test_stat():\n    from pathlib import Path\n    Path({str(recovery / "missing")!r}).exists()\n')
    child_env = dict(os.environ, PYTHONPATH=str(env.ROOT), PYTEST_DISABLE_PLUGIN_AUTOLOAD='1')
    result = subprocess.run([sys.executable, '-m', 'pytest', '-q', '-p', 'cmre.workspace_audit',
                             '--cmre-workspace-audit', '--cmre-recovery-root', str(recovery), str(test)],
                            env=child_env, cwd=tmp_path, capture_output=True, text=True, timeout=30)
    assert result.returncode != 0 and 'unmarked recovery access' in result.stdout
