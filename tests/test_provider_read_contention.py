"""Deterministic read-contention regressions for the provider job store."""

from __future__ import annotations

import errno
import hashlib
import json
import sys
import threading
import time
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[1]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from cmre import provider_campaign as pc  # noqa: E402
from cmre import provider_jobs as pj  # noqa: E402

MODELS = {
    "deepseek": "deepseek-test",
    "minimax": "minimax-test",
}


def make_manifest(job_ids, concurrency=2):
    return {
        "schema": pc.SCHEMA,
        "duration_seconds": 60,
        "concurrency": concurrency,
        "roles": {role: {"model": model} for role, model in MODELS.items()},
        "jobs": [
            {
                "id": job_id,
                "config": {
                    "function": "0x401000",
                    "mode": "input",
                    "goal": f"goal {job_id}",
                },
            }
            for job_id in job_ids
        ],
    }


class Backend:
    def __init__(self, delay=0.0):
        self.delay = delay
        self.lock = threading.Lock()
        self.active = 0
        self.max_active = 0
        self.active_jobs = set()
        self.duplicate_jobs = False
        self.actor_calls = 0

    def actor(self, job, role, context, stage_path):
        with self.lock:
            self.active += 1
            self.max_active = max(self.max_active, self.active)
            if job["id"] in self.active_jobs:
                self.duplicate_jobs = True
            self.active_jobs.add(job["id"])
            self.actor_calls += 1
        if self.delay:
            time.sleep(self.delay)
        if context["phase"] == "owner":
            action = {"kind": "propose", "artifacts": ["a0001"], "summary": "ok"}
        else:
            action = {
                "kind": "review",
                "verdict": "pass",
                "bundle_sha256": context["bundle"]["sha256"],
                "checks": list(context["required_checks"]),
                "unknowns": [],
            }
        return {"action": action, "model": MODELS[role]}

    def execute(self, job, action, output_path, prior=None):
        try:
            if action["kind"] == "bundle":
                sha = hashlib.sha256(
                    json.dumps(
                        {
                            "artifacts": action["artifacts"],
                            "summary": action["summary"],
                        },
                        sort_keys=True,
                    ).encode("utf-8")
                ).hexdigest()
                return {
                    "ready": True,
                    "sha256": sha,
                    "required_checks": ["pc"],
                    "payload": {
                        "artifacts": list(action["artifacts"]),
                        "summary": action["summary"],
                    },
                }
            if action["kind"] == "finalize":
                return {
                    "state": "ready",
                    "bundle_sha256": action["bundle"]["sha256"],
                }
            return {"kind": action["kind"], "ok": True, "result": {}}
        finally:
            with self.lock:
                self.active -= 1
                self.active_jobs.discard(job["id"])


def install_state_reader(monkeypatch, *, fail_attempts=(), always_permission=False):
    original = Path.read_text
    counter = {"count": 0}

    def read_text(self, *args, **kwargs):
        if Path(self).name != "state.json":
            return original(self, *args, **kwargs)
        counter["count"] += 1
        if always_permission or counter["count"] in fail_attempts:
            raise PermissionError(errno.EACCES, "simulated sharing violation")
        return original(self, *args, **kwargs)

    monkeypatch.setattr(Path, "read_text", read_text)
    return counter


def make_store(tmp_path, name="store"):
    directory = tmp_path / name
    pj.create(directory, {"jobs": [{"id": "alpha"}]}, {"provider": "test"})
    return directory


def test_transient_state_read_permission_error_is_recovered(tmp_path, monkeypatch):
    directory = make_store(tmp_path)
    attempts = install_state_reader(monkeypatch, fail_attempts={1, 2})
    state = pj.load(directory)
    assert state["jobs"]["alpha"]["state"] == "prepared"
    assert attempts["count"] == 3


def test_concurrent_dispatch_recovers_from_transient_state_read_permission(
    tmp_path, monkeypatch
):
    directory = tmp_path / "camp"
    pc.initialize(
        directory,
        make_manifest(["j1", "j2", "j3"], concurrency=2),
        {"provider": "test"},
    )
    attempts = install_state_reader(monkeypatch, fail_attempts={3})
    backend = Backend(delay=0.01)
    result = pc.run(directory, actor=backend.actor, execute=backend.execute)
    state = pj.load(directory)
    assert all(job["state"] == "ready" for job in state["jobs"].values()), {
        job_id: (job["state"], job.get("hold_reason"))
        for job_id, job in state["jobs"].items()
    }
    assert backend.max_active <= 2
    assert backend.duplicate_jobs is False
    assert backend.actor_calls == 6
    assert result["counts"]["ready"] == 3
    assert attempts["count"] >= 3


def test_permanent_permission_denial_is_bounded_and_diagnosable(tmp_path, monkeypatch):
    directory = make_store(tmp_path)
    attempts = install_state_reader(monkeypatch, always_permission=True)
    with pytest.raises(pj.CorruptStore) as raised:
        pj.load(directory)
    message = str(raised.value)
    assert attempts["count"] == pj.READ_ATTEMPTS
    assert "unreadable file" in message
    assert "PermissionError" in message
    assert f"errno={errno.EACCES}" in message


def test_missing_state_file_is_not_retried(tmp_path, monkeypatch):
    directory = make_store(tmp_path)
    attempts = install_state_reader(monkeypatch)
    (directory / "state.json").unlink()
    with pytest.raises(pj.CorruptStore, match="missing file"):
        pj.load(directory)
    assert attempts["count"] == 1


def test_corrupt_state_file_is_not_retried(tmp_path, monkeypatch):
    directory = make_store(tmp_path)
    attempts = install_state_reader(monkeypatch)
    (directory / "state.json").write_text("{", encoding="utf-8")
    with pytest.raises(pj.CorruptStore, match="invalid JSON"):
        pj.load(directory)
    assert attempts["count"] == 1


def test_tampered_state_snapshot_is_not_retried(tmp_path, monkeypatch):
    directory = make_store(tmp_path)
    pj.update(directory, "alpha", {"state": "running"}, expected_revision=0)
    path = directory / "state.json"
    state = json.loads(path.read_text(encoding="utf-8"))
    state["jobs"]["alpha"]["state"] = "accepted"
    path.write_text(json.dumps(state), encoding="utf-8")
    attempts = install_state_reader(monkeypatch)
    with pytest.raises(pj.CorruptStore, match="snapshot does not match"):
        pj.load(directory)
    assert attempts["count"] == 1
