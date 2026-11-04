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


def _pytest_collection_read(tmp_path, marked):
    recovery = tmp_path / 'recovery'
    recovery.mkdir()
    (recovery / 'input').write_text('x')
    test = tmp_path / 'test_import.py'
    mark = '\nimport pytest\npytestmark = pytest.mark.workspace\n' if marked else '\n'
    test.write_text(f'CONTENT = open({str(recovery / "input")!r}).read()' + mark + 'def test_value():\n    assert CONTENT == "x"\n')
    child_env = dict(os.environ, PYTHONPATH=str(env.ROOT), PYTEST_DISABLE_PLUGIN_AUTOLOAD='1')
    return subprocess.run([sys.executable, '-m', 'pytest', '-q', '-p', 'cmre.workspace_audit',
                           '--cmre-workspace-audit', '--cmre-recovery-root', str(recovery), str(test)],
                          env=child_env, cwd=tmp_path, capture_output=True, text=True, timeout=30)


def test_audit_rejects_unmarked_import_time_recovery_read(tmp_path):
    result = _pytest_collection_read(tmp_path, False)
    assert result.returncode != 0 and 'unmarked recovery access during collection' in result.stdout


def test_audit_allows_marked_import_time_recovery_read(tmp_path):
    result = _pytest_collection_read(tmp_path, True)
    assert result.returncode == 0, result.stdout + result.stderr


def _pytest_class_collection_read(tmp_path, marked):
    recovery = tmp_path / 'recovery'
    recovery.mkdir()
    (recovery / 'input').write_text('1')
    test = tmp_path / 'test_class.py'
    mark = 'import pytest\npytestmark = pytest.mark.workspace\n' if marked else ''
    test.write_text(mark + f'''class TestExample:
    def pytest_generate_tests(self, metafunc):
        value = open({str(recovery / "input")!r}).read()
        metafunc.parametrize("value", [value])

    def test_value(self, value):
        assert value == "1"
''')
    child_env = dict(os.environ, PYTHONPATH=str(env.ROOT), PYTEST_DISABLE_PLUGIN_AUTOLOAD='1')
    return subprocess.run([sys.executable, '-m', 'pytest', '-q', '-p', 'cmre.workspace_audit',
                           '--cmre-workspace-audit', '--cmre-recovery-root', str(recovery), str(test)],
                          env=child_env, cwd=tmp_path, capture_output=True, text=True, timeout=30)


def test_audit_rejects_unmarked_class_parametrization_read(tmp_path):
    result = _pytest_class_collection_read(tmp_path, False)
    assert result.returncode != 0 and 'unmarked recovery access during collection' in result.stdout


def test_audit_allows_marked_class_parametrization_read(tmp_path):
    result = _pytest_class_collection_read(tmp_path, True)
    assert result.returncode == 0, result.stdout + result.stderr


def test_candidate_audit_runner_loads_main_package_and_candidate_plugin(tmp_path):
    import textwrap
    package = tmp_path / 'cmre'
    package.mkdir()
    (package / '__init__.py').write_text('')
    (package / 'env.py').write_text('from pathlib import Path\nROOT = Path(__file__).resolve().parents[1]\nRECOVERY = ROOT / "recovery"\n')
    (tmp_path / 'tests').mkdir()
    (tmp_path / 'scripts/tests').mkdir(parents=True)
    (tmp_path / 'tests/test_main.py').write_text(textwrap.dedent('''
        from cmre import env
        def test_main_package():
            assert env.ROOT.name == %r
    ''' % tmp_path.name))
    (tmp_path / 'scripts/tests/test_main_scripts.py').write_text('def test_script():\n    assert True\n')
    runner = env.ROOT / 'scripts/cmre_candidate_audit.py'
    child_env = dict(os.environ, PYTEST_DISABLE_PLUGIN_AUTOLOAD='1')
    result = subprocess.run([sys.executable, str(runner), str(tmp_path)], env=child_env, cwd=tmp_path,
                            capture_output=True, text=True, timeout=30)
    assert result.returncode == 0, result.stdout + result.stderr
    assert '2 passed' in result.stdout
    result = subprocess.run([sys.executable, str(runner), str(tmp_path), 'tests/test_main.py'],
                            env=child_env, cwd=tmp_path, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0, result.stdout + result.stderr
    assert '1 passed' in result.stdout


def test_workspace_catalogue_includes_main_audit_readers():
    from cmre.workspace_audit import WORKSPACE_FILES
    assert {'test_cmre_ctier.py', 'test_cmre_input_policy.py', 'test_cmre_lift_migration.py', 'test_cmre_shards.py'} <= WORKSPACE_FILES
    assert len(WORKSPACE_FILES) == 27
