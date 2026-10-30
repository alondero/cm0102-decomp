"""Publication survives failure; direct writers and supervised children agree."""
import json
import os
import subprocess
import sys
import time

import pytest

from cmre import coordination, env, ledger


def test_atomic_failure_retains_previous_record(tmp_path, monkeypatch):
    path = tmp_path / 'record.json'
    path.write_text('{"old": true}', encoding='utf-8')
    def fail(*args):
        raise OSError('injected replace failure')
    monkeypatch.setattr(coordination.os, 'replace', fail)
    with pytest.raises(OSError, match='injected'):
        ledger.write_json(path, {'new': True})
    assert json.loads(path.read_text()) == {'old': True}
    assert list(tmp_path.iterdir()) == [path]


def test_lock_excludes_and_releases(tmp_path):
    path = tmp_path / 'writer.lock'
    with coordination.lock(path):
        with pytest.raises(coordination.BusyError):
            with coordination.lock(path):
                pytest.fail('overlapping writer')
    with coordination.lock(path):
        pass


def test_command_lease_allows_child_but_excludes_unrelated_writer(tmp_path, monkeypatch):
    path = tmp_path / 'command.lock'
    monkeypatch.setenv('CMRE_COORDINATION_PATH', str(path))
    script = 'from cmre.coordination import writer\nwith writer(): print("owned")'
    with coordination.writer():
        child = subprocess.run([sys.executable, '-c', script], cwd=env.ROOT,
                               capture_output=True, text=True)
        assert child.returncode == 0, child.stderr
        unrelated = dict(os.environ)
        unrelated.pop('CMRE_WRITER_TOKEN', None)
        child = subprocess.run([sys.executable, '-c', script], cwd=env.ROOT,
                               env=unrelated, capture_output=True, text=True)
        assert child.returncode != 0
        assert 'BusyError' in child.stderr
    with coordination.writer():
        pass


def test_stale_inherited_token_does_not_bypass_new_owner(tmp_path, monkeypatch):
    path = tmp_path / 'command.lock'
    monkeypatch.setenv('CMRE_COORDINATION_PATH', str(path))
    with coordination.writer():
        stale = os.environ['CMRE_WRITER_TOKEN']
    with coordination.writer():
        script = 'from cmre.coordination import writer\nwith writer(): pass'
        child = subprocess.run([sys.executable, '-c', script], cwd=env.ROOT,
                               env=dict(os.environ, CMRE_WRITER_TOKEN=stale), capture_output=True, text=True)
        assert child.returncode != 0


def test_orphaned_child_cannot_promote_itself_to_coordinator(tmp_path, monkeypatch):
    path = tmp_path / 'command.lock'
    monkeypatch.setenv('CMRE_COORDINATION_PATH', str(path))
    with coordination.writer():
        child_env = dict(os.environ)
    script = 'from cmre.coordination import writer\nwith writer(): print("orphan wrote")'
    child = subprocess.run([sys.executable, '-c', script], cwd=env.ROOT,
                           env=child_env, capture_output=True, text=True)
    assert child.returncode != 0
    assert 'expired' in child.stderr.lower()


def test_inflight_orphan_cannot_publish_after_parent_releases(tmp_path, monkeypatch):
    monkeypatch.setenv('CMRE_COORDINATION_PATH', str(tmp_path / 'command.lock'))
    script = '''
import sys, time
from pathlib import Path
from cmre.coordination import writer, atomic_bytes
directory = Path(sys.argv[1])
with writer():
    (directory / 'ready').touch()
    while not (directory / 'go').exists(): time.sleep(0.01)
    atomic_bytes(directory / 'result', b'unsafe')
'''
    with coordination.writer():
        child = subprocess.Popen([sys.executable, '-c', script, str(tmp_path)], cwd=env.ROOT,
                                 stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        deadline = time.monotonic() + 10
        while not (tmp_path / 'ready').exists() and time.monotonic() < deadline:
            time.sleep(0.01)
        ready = (tmp_path / 'ready').exists()
    try:
        assert ready, 'child did not enter inherited lease'
        (tmp_path / 'go').touch()
        _, error = child.communicate(timeout=10)
        assert child.returncode != 0
        assert 'expired before publication' in error
        assert not (tmp_path / 'result').exists()
    finally:
        if child.poll() is None:
            child.kill()
            child.wait()


def test_refresh_exception_keeps_accepted_record(tmp_path, monkeypatch):
    from cmre import cli, recover
    va = 0x004671b0
    monkeypatch.setattr(env, 'LEDGER_DIR', tmp_path)
    path = ledger.record_path(va)
    ledger.write_json(path, {'lifted': {'source_sha256': 'source', 'harness_sha256': 'old'}})
    before = path.read_bytes()
    monkeypatch.setattr(recover, 'test_lifted', lambda va: {'status': 'pass', 'source_sha256': 'source'})
    def fail(*args, **kwargs):
        assert path.read_bytes() == before
        raise RuntimeError('interrupted refresh')
    monkeypatch.setattr(recover, 'accept', fail)
    with pytest.raises(RuntimeError, match='interrupted refresh'):
        cli._revalidate_one(va, refresh=True)
    assert path.read_bytes() == before
