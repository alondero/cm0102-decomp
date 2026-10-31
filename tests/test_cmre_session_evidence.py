"""Full-suite receipt reuse requires exact declared inputs and intact evidence."""
import json

import pytest

from cmre import env, session_evidence as evidence


def receipt(tmp_path, inputs, **overrides):
    path = tmp_path / 'start/receipt.json'
    path.parent.mkdir(exist_ok=True)
    log = path.parent / 'pytest.log'
    log.write_text('1265 passed')
    step = {'command': ['py', '-3.13', '-m', 'pytest'], 'state': 'finished', 'exit_code': 0,
            'inputs_before': inputs, 'inputs_after': inputs, 'evidence': 'executed',
            'log': log.name, 'log_sha256': env.digest(log), **overrides}
    path.write_text(json.dumps({'steps': [step]}))
    return path


def test_full_suite_reuse_preserves_receipt_and_log(tmp_path):
    inputs = {'files': {'test.py': 'one'}, 'python': '3.13'}
    path = receipt(tmp_path, inputs)
    original = path.read_bytes()
    found = evidence.successful_step(path, inputs)
    assert found['receipt'] == str(path.resolve()) and found['step'] == 0
    assert path.read_bytes() == original


@pytest.mark.parametrize('change', [
    {'state': 'running'}, {'exit_code': 1}, {'evidence': 'reused'},
    {'command': ['py', '-3.13', '-m', 'pytest', '-k', 'focused']},
    {'inputs_after': {'files': 'changed'}}, {'inputs_before': {'files': 'changed'}}])
def test_failed_incomplete_focused_and_changed_inputs_cannot_seed_reuse(tmp_path, change):
    inputs = {'files': {'test.py': 'one'}}
    assert evidence.successful_step(receipt(tmp_path, inputs, **change), inputs) is None


def test_modified_test_log_rejects_reuse(tmp_path):
    inputs = {'files': {}}
    path = receipt(tmp_path, inputs)
    (path.parent / 'pytest.log').write_text('failure')
    assert evidence.successful_step(path, inputs) is None


def test_declared_code_test_fixture_and_new_file_changes_invalidate(tmp_path, monkeypatch):
    import subprocess
    subprocess.run(['git', 'init', '-q', str(tmp_path)], check=True)
    monkeypatch.setattr(env, 'INVENTORY_SHA256', {})
    monkeypatch.setattr(env, 'tool', lambda name: str(tmp_path / 'compiler'))
    (tmp_path / 'compiler').write_text('compiler')
    for name in ('code.py', 'test.py', 'local/fixtures/input', 'recovery/hints/a.json'):
        path = tmp_path / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text('one')
    (tmp_path / '.gitignore').write_text('/local/\n/recovery/\n')
    base = evidence.identity(tmp_path)
    for name in ('code.py', 'test.py', 'local/fixtures/input', 'recovery/hints/a.json', 'compiler', 'new.py'):
        path = tmp_path / name
        path.write_text('two')
        assert evidence.identity(tmp_path) != base
        if name == 'new.py':
            path.unlink()
        else:
            path.write_text('one' if name != 'compiler' else 'compiler')
    assert evidence.identity(tmp_path) == base
    monkeypatch.setenv('CPATH', 'changed-compiler-environment')
    assert evidence.identity(tmp_path) != base


def test_journal_reuses_pytest_but_always_runs_integrity(tmp_path, monkeypatch):
    from scripts import cmre_session as runner
    monkeypatch.setattr(runner, 'ROOT', tmp_path)
    inputs = {'files': {'tests.py': 'one'}}
    monkeypatch.setattr(evidence, 'identity', lambda root: inputs)
    prior = receipt(tmp_path, inputs)
    journal = runner.Journal(tmp_path / 'check', 'check')
    monkeypatch.setattr(runner.subprocess, 'Popen', lambda *a, **kw: pytest.fail('repeated full pytest'))
    assert journal.execute(('pytest',)) == 0
    step = journal.data['steps'][0]
    assert step['evidence'] == 'reused' and step['reused_from']['receipt'] == str(prior.resolve())
