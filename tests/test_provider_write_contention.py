"""Publication failures seen with concurrent Windows provider stages."""
import errno
import json
import os
from pathlib import Path
import subprocess
import sys
import time

import pytest

from cmre import coordination, provider_jobs


def sharing_error(code=5):
    error = PermissionError(errno.EACCES, "simulated Windows sharing conflict")
    error.winerror = code
    return error


@pytest.mark.parametrize("code", [5, 32, 33])
def test_store_update_survives_transient_replace_conflict(tmp_path, monkeypatch, code):
    directory = tmp_path / "store"
    provider_jobs.create(directory, {"jobs": [{"id": "alpha"}]}, {})
    original = coordination.os.replace
    attempts = []

    def replace(source, destination):
        if destination.name == "state.json":
            attempts.append(source)
            if len(attempts) <= 2:
                raise sharing_error(code)
        return original(source, destination)

    monkeypatch.setattr(coordination.os, "replace", replace)
    provider_jobs.update(directory, "alpha", {"state": "running"}, expected_revision=0)
    state = provider_jobs.load(directory)
    assert state["jobs"]["alpha"]["state"] == "running"
    assert state["revision"] == 1
    assert len(attempts) == 3
    assert len(set(attempts)) == 1
    assert not list(directory.glob(".*.tmp"))


def test_persistent_replace_conflict_preserves_old_record(tmp_path, monkeypatch):
    path = tmp_path / "record.json"
    coordination.atomic_json(path, {"old": True})
    attempts = []

    def replace(source, destination):
        attempts.append(source)
        raise sharing_error()

    monkeypatch.setattr(coordination.os, "replace", replace)
    monkeypatch.setattr(coordination.time, "sleep", lambda seconds: None)
    with pytest.raises(PermissionError):
        coordination.atomic_json(path, {"new": True})
    assert 1 < len(attempts) <= 100
    assert json.loads(path.read_text()) == {"old": True}
    assert list(tmp_path.iterdir()) == [path]


def test_unrelated_replace_error_is_not_retried(tmp_path, monkeypatch):
    calls = []

    def replace(source, destination):
        calls.append(source)
        raise FileNotFoundError(errno.ENOENT, "missing publication target")

    monkeypatch.setattr(coordination.os, "replace", replace)
    with pytest.raises(FileNotFoundError):
        coordination.atomic_json(tmp_path / "record.json", {})
    assert len(calls) == 1
    assert not list(tmp_path.iterdir())


def test_lease_expiry_during_replace_retry_blocks_publication(tmp_path, monkeypatch):
    path = tmp_path / "record.json"
    coordination.atomic_json(path, {"old": True})
    monkeypatch.setenv("CMRE_COORDINATION_PATH", str(tmp_path / "command.lock"))
    monkeypatch.setenv("CMRE_WRITER_PATH", str(tmp_path / "command.lock"))
    monkeypatch.setenv("CMRE_WRITER_TOKEN", "inherited-test-token")
    live = [True]
    attempts = []
    monkeypatch.setattr(coordination, "_live", lambda *args: live[0])

    def replace(source, destination):
        attempts.append(source)
        live[0] = False
        raise sharing_error()

    monkeypatch.setattr(coordination.os, "replace", replace)
    with pytest.raises(coordination.BusyError, match="expired before publication"):
        coordination.atomic_json(path, {"new": True})
    assert len(attempts) == 1
    assert json.loads(path.read_text()) == {"old": True}
    assert not list(tmp_path.glob(".*.tmp"))


def test_parent_exit_during_child_replace_retry_blocks_publication(tmp_path, monkeypatch):
    monkeypatch.setenv("CMRE_COORDINATION_PATH", str(tmp_path / "command.lock"))
    monkeypatch.delenv("CMRE_WRITER_TOKEN", raising=False)
    monkeypatch.delenv("CMRE_WRITER_PATH", raising=False)
    destination = tmp_path / "record.json"
    destination.write_bytes(b"previous record")
    child_script = """
import sys, time
from pathlib import Path
from cmre import coordination

root = Path(sys.argv[1])
original_replace = coordination.os.replace
attempts = 0

def replace(source, destination):
    global attempts
    attempts += 1
    if attempts == 1:
        (root / 'ready').touch()
        deadline = time.monotonic() + 10
        while not (root / 'go').exists():
            if time.monotonic() >= deadline:
                raise RuntimeError('parent did not release child')
            time.sleep(0.005)
        error = PermissionError('simulated reader sharing conflict')
        error.winerror = 5
        raise error
    return original_replace(source, destination)

coordination.os.replace = replace
coordination.atomic_bytes(root / 'record.json', b'orphan publication')
"""
    child = None
    try:
        with coordination.writer():
            child = subprocess.Popen(
                [sys.executable, "-c", child_script, str(tmp_path)],
                cwd=Path(coordination.__file__).resolve().parents[1],
                stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True,
            )
            deadline = time.monotonic() + 10
            while not (tmp_path / "ready").exists() and child.poll() is None:
                if time.monotonic() >= deadline:
                    break
                time.sleep(0.005)
            assert (tmp_path / "ready").exists(), "child did not reach replacement"
        (tmp_path / "go").touch()
        _, error = child.communicate(timeout=10)
        assert child.returncode != 0
        assert "Inherited command lease expired before publication" in error
        assert destination.read_bytes() == b"previous record"
        assert not list(tmp_path.glob(".*.tmp"))
    finally:
        if child is not None and child.poll() is None:
            child.kill()
            child.wait(timeout=10)


@pytest.mark.skipif(os.name != "nt", reason="Windows delete-sharing semantics")
def test_real_windows_reader_releases_snapshot_before_replace_retry(tmp_path, monkeypatch):
    directory = tmp_path / "store"
    provider_jobs.create(directory, {"jobs": [{"id": "alpha"}]}, {})
    reader = (directory / "state.json").open("rb")
    original = coordination.os.replace
    conflicts = []

    def replace(source, destination):
        try:
            return original(source, destination)
        except PermissionError as error:
            conflicts.append(error.winerror)
            reader.close()
            raise

    monkeypatch.setattr(coordination.os, "replace", replace)
    try:
        provider_jobs.update(directory, "alpha", {"state": "running"}, expected_revision=0)
    finally:
        reader.close()
    assert conflicts
    assert provider_jobs.load(directory)["jobs"]["alpha"]["state"] == "running"
