import json
import threading
import time

import pytest

from cmre import provider_jobs as pj
from cmre.coordination import BusyError


def make_manifest(*jobs):
    return {
        "campaign": "test",
        "jobs": list(jobs) or [
            {"id": "alpha", "config": {"limit": 1}},
            {"id": "beta", "config": {"limit": 2}},
        ],
    }


def make_identity():
    return {"provider": "local", "account": "tester"}


def test_create_and_reload(tmp_path):
    directory = tmp_path / "store"
    state = pj.create(directory, make_manifest(), make_identity())
    assert state["schema"] == "cmre-provider-jobs/1"
    assert state["revision"] == 0
    assert set(state["jobs"]) == {"alpha", "beta"}
    assert state["jobs"]["alpha"] == {
        "id": "alpha",
        "config": {"limit": 1},
        "revision": 0,
        "state": "prepared",
        "history": [],
    }
    assert state["manifest_hash"]
    assert state["identity_hash"]
    assert pj.load(directory) == state
    assert pj.load(directory) is not state


def test_caller_mutation_does_not_change_store(tmp_path):
    directory = tmp_path / "store"
    manifest = make_manifest()
    identity = make_identity()
    state = pj.create(directory, manifest, identity)
    manifest["jobs"][0]["config"]["limit"] = 999
    identity["provider"] = "other"
    state["jobs"]["alpha"]["config"]["limit"] = 777
    state["manifest"]["campaign"] = "changed"
    loaded = pj.load(directory)
    assert loaded["manifest"]["jobs"][0]["config"]["limit"] == 1
    assert loaded["manifest"]["campaign"] == "test"
    assert loaded["identity"]["provider"] == "local"
    assert loaded["jobs"]["alpha"]["config"]["limit"] == 1


def test_create_refuses_existing_directory(tmp_path):
    directory = tmp_path / "store"
    directory.mkdir()
    with pytest.raises(FileExistsError):
        pj.create(directory, make_manifest(), make_identity())


def test_duplicate_job_ids_refused(tmp_path):
    directory = tmp_path / "store"
    manifest = {"jobs": [{"id": "same"}, {"id": "same"}]}
    with pytest.raises(pj.InvalidChange):
        pj.create(directory, manifest, {})
    assert not directory.exists()


def test_nonfinite_json_refused(tmp_path):
    directory = tmp_path / "store"
    manifest = {"jobs": [{"id": "alpha", "config": {"value": float("nan")}}]}
    with pytest.raises(pj.InvalidChange):
        pj.create(directory, manifest, {})
    pj.create(directory, make_manifest(), make_identity())
    with pytest.raises(pj.InvalidChange):
        pj.update(directory, "alpha", {"value": float("inf")}, expected_revision=0)
    with pytest.raises(pj.InvalidChange):
        pj.update(directory, "alpha", {"value": object()}, expected_revision=0)


def test_stale_same_job_update_refused(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    updated = pj.update(directory, "alpha", {"state": "running"}, expected_revision=0)
    assert updated["revision"] == 1
    assert updated["state"] == "running"
    with pytest.raises(pj.StaleRevision):
        pj.update(directory, "alpha", {"state": "ready"}, expected_revision=0)
    assert pj.load(directory)["revision"] == 1


def test_disjoint_jobs_use_job_revisions(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    alpha = pj.update(directory, "alpha", {"state": "running"}, expected_revision=0)
    beta = pj.update(directory, "beta", {"state": "running"}, expected_revision=0)
    assert alpha["revision"] == 1
    assert beta["revision"] == 1
    state = pj.load(directory)
    assert state["revision"] == 2
    assert state["jobs"]["alpha"]["revision"] == 1
    assert state["jobs"]["beta"]["revision"] == 1


def test_protected_changes_and_unknown_job_refused(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    for changes in (
        {"id": "other"},
        {"config": {}},
        {"revision": 4},
        {"history": []},
        {"state": "bogus"},
    ):
        with pytest.raises(pj.InvalidChange):
            pj.update(directory, "alpha", changes, expected_revision=0)
    with pytest.raises(pj.UnknownJob):
        pj.update(directory, "missing", {"state": "running"}, expected_revision=0)
    assert pj.load(directory)["revision"] == 0


def test_terminal_states_cannot_reopen(tmp_path):
    directory = tmp_path / "terminal-store"
    pj.create(
        directory,
        {"jobs": [{"id": "a"}, {"id": "b"}, {"id": "c"}]},
        make_identity(),
    )
    pj.update(directory, "a", {"state": "accepted"}, expected_revision=0)
    pj.update(directory, "b", {"state": "exhausted"}, expected_revision=0)
    pj.update(directory, "c", {"state": "cancelled"}, expected_revision=0)
    for job_id, revision in (("a", 1), ("b", 1), ("c", 1)):
        with pytest.raises(pj.InvalidChange):
            pj.update(
                directory, job_id, {"state": "prepared"}, expected_revision=revision
            )
    same = pj.update(
        directory, "a", {"state": "accepted", "note": "kept"}, expected_revision=1
    )
    assert same["state"] == "accepted"


def test_held_requires_explicit_prepared_retry(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    pj.update(directory, "alpha", {"state": "held"}, expected_revision=0)
    with pytest.raises(pj.InvalidChange):
        pj.update(directory, "alpha", {"state": "running"}, expected_revision=1)
    with pytest.raises(pj.InvalidChange):
        pj.update(directory, "alpha", {"state": "prepared"}, expected_revision=1)
    with pytest.raises(pj.InvalidChange):
        pj.update(
            directory,
            "alpha",
            {"state": "prepared", "retry_reason": ""},
            expected_revision=1,
        )
    assert pj.load(directory)["jobs"]["alpha"]["revision"] == 1
    reopened = pj.update(
        directory,
        "alpha",
        {"state": "prepared", "retry_reason": "operator retry"},
        expected_revision=1,
    )
    assert reopened["state"] == "prepared"
    assert reopened["revision"] == 2
    assert reopened["retry_reason"] == "operator retry"


def test_interrupted_state_write_replays_exactly_once(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    snapshot = (directory / "state.json").read_bytes()
    pj.update(directory, "alpha", {"state": "running"}, expected_revision=0)
    (directory / "state.json").write_bytes(snapshot)

    first = pj.load(directory)
    assert first["revision"] == 1
    assert first["jobs"]["alpha"]["revision"] == 1
    assert first["jobs"]["alpha"]["state"] == "running"
    second = pj.load(directory)
    assert second["revision"] == 1
    assert second["jobs"]["alpha"]["revision"] == 1

    continued = pj.update(
        directory, "alpha", {"state": "needs-review"}, expected_revision=1
    )
    assert continued["revision"] == 2
    assert pj.load(directory)["revision"] == 2


def test_tampered_event_refused(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    pj.update(directory, "alpha", {"state": "running"}, expected_revision=0)
    event_path = directory / "events" / "00000001.json"
    event = json.loads(event_path.read_text(encoding="utf-8"))
    event["changes"]["state"] = "accepted"
    event_path.write_text(json.dumps(event), encoding="utf-8")
    with pytest.raises(pj.CorruptStore):
        pj.load(directory)


def test_corrupt_event_json_refused(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    pj.update(directory, "alpha", {"state": "running"}, expected_revision=0)
    (directory / "events" / "00000001.json").write_text("{", encoding="utf-8")
    with pytest.raises(pj.CorruptStore):
        pj.load(directory)


def test_gapped_events_refused(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    pj.update(directory, "alpha", {"state": "running"}, expected_revision=0)
    pj.update(directory, "beta", {"state": "running"}, expected_revision=0)
    (directory / "events" / "00000001.json").unlink()
    with pytest.raises(pj.CorruptStore):
        pj.load(directory)


def test_reordered_events_refused(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    pj.update(directory, "alpha", {"state": "running"}, expected_revision=0)
    pj.update(directory, "beta", {"state": "running"}, expected_revision=0)
    events = directory / "events"
    (events / "00000002.json").rename(events / "00000003.json")
    with pytest.raises(pj.CorruptStore):
        pj.load(directory)


def test_tampered_manifest_hash_refused(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    state_path = directory / "state.json"
    state = json.loads(state_path.read_text(encoding="utf-8"))
    state["manifest"]["campaign"] = "tampered"
    state_path.write_text(json.dumps(state), encoding="utf-8")
    with pytest.raises(pj.CorruptStore):
        pj.load(directory)


def test_history_is_bounded_metadata(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    for index in range(80):
        pj.update(
            directory,
            "alpha",
            {"state": "running", "secret": f"trace-{index}"},
            expected_revision=index,
        )
    job = pj.load(directory)["jobs"]["alpha"]
    assert job["revision"] == 80
    assert len(job["history"]) == pj.MAX_HISTORY
    assert all("trace-" not in json.dumps(entry) for entry in job["history"])
    assert job["secret"] == "trace-79"


def test_snapshot_state_edit_is_detected(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    pj.update(directory, "alpha", {"state": "running"}, expected_revision=0)
    path = directory / "state.json"
    state = json.loads(path.read_text(encoding="utf-8"))
    state["jobs"]["alpha"]["state"] = "accepted"
    path.write_text(json.dumps(state), encoding="utf-8")
    with pytest.raises(pj.CorruptStore):
        pj.load(directory)


def test_snapshot_hash_edit_is_detected(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    pj.update(
        directory,
        "alpha",
        {"state": "running", "bundle": {"sha256": "aa", "reviewer": "minimax"}},
        expected_revision=0,
    )
    path = directory / "state.json"
    state = json.loads(path.read_text(encoding="utf-8"))
    state["jobs"]["alpha"]["bundle"]["sha256"] = "bb"
    state["jobs"]["alpha"]["bundle"]["reviewer"] = "deepseek"
    path.write_text(json.dumps(state), encoding="utf-8")
    with pytest.raises(pj.CorruptStore):
        pj.load(directory)


def test_snapshot_revision_edit_is_detected(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    pj.update(directory, "alpha", {"state": "running"}, expected_revision=0)
    path = directory / "state.json"
    state = json.loads(path.read_text(encoding="utf-8"))
    state["revision"] = 0
    path.write_text(json.dumps(state), encoding="utf-8")
    with pytest.raises(pj.CorruptStore):
        pj.load(directory)


def test_genesis_is_immutable(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    path = directory / "genesis.json"
    genesis = json.loads(path.read_text(encoding="utf-8"))
    genesis["manifest"]["campaign"] = "tampered"
    path.write_text(json.dumps(genesis), encoding="utf-8")
    with pytest.raises(pj.CorruptStore):
        pj.load(directory)


def test_genesis_identity_is_immutable(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    path = directory / "genesis.json"
    genesis = json.loads(path.read_text(encoding="utf-8"))
    genesis["identity"]["provider"] = "other"
    path.write_text(json.dumps(genesis), encoding="utf-8")
    with pytest.raises(pj.CorruptStore):
        pj.load(directory)


def test_duplicate_json_keys_rejected(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    (directory / "state.json").write_text(
        '{"schema": "cmre-provider-jobs/1", "revision": 0, "revision": 0}',
        encoding="utf-8",
    )
    with pytest.raises(pj.CorruptStore):
        pj.load(directory)


def test_nonfinite_state_rejected(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    (directory / "state.json").write_text(
        '{"schema": "cmre-provider-jobs/1", "revision": NaN}', encoding="utf-8"
    )
    with pytest.raises(pj.CorruptStore):
        pj.load(directory)


def test_snapshot_reviewer_and_source_hash_edits_are_detected(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    pj.update(
        directory,
        "alpha",
        {
            "state": "running",
            "bundle": {"sha256": "aa", "reviewer": "minimax"},
            "results": [{"id": "a0001", "hash": "cc", "path": "jobs/alpha/a0001.json"}],
        },
        expected_revision=0,
    )
    path = directory / "state.json"
    state = json.loads(path.read_text(encoding="utf-8"))
    state["jobs"]["alpha"]["bundle"]["reviewer"] = "deepseek"
    state["jobs"]["alpha"]["results"][0]["hash"] = "dd"
    path.write_text(json.dumps(state), encoding="utf-8")
    with pytest.raises(pj.CorruptStore):
        pj.load(directory)


def test_event_duplicate_keys_and_nonfinite_are_rejected(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    pj.update(directory, "alpha", {"state": "running"}, expected_revision=0)
    event_path = directory / "events" / "00000001.json"

    event = json.loads(event_path.read_text(encoding="utf-8"))
    text = json.dumps(event)
    text = text.replace('"state": "running"', '"state": "running", "state": "accepted"')
    event_path.write_text(text, encoding="utf-8")
    with pytest.raises(pj.CorruptStore):
        pj.load(directory)

    event_path.write_text(
        '{"schema": "cmre-provider-jobs/1", "sequence": NaN}', encoding="utf-8"
    )
    with pytest.raises(pj.CorruptStore):
        pj.load(directory)


def test_ready_state_cannot_reopen(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    pj.update(directory, "alpha", {"state": "ready"}, expected_revision=0)
    for target in ("prepared", "held", "running"):
        with pytest.raises(pj.InvalidChange):
            pj.update(directory, "alpha", {"state": target}, expected_revision=1)


def test_held_retry_preserves_counters_results_and_history(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    pj.update(
        directory,
        "alpha",
        {"state": "running", "attempts": 3, "results": [{"id": "a0001"}]},
        expected_revision=0,
    )
    pj.update(directory, "alpha", {"state": "held"}, expected_revision=1)
    with pytest.raises(pj.InvalidChange):
        pj.update(
            directory,
            "alpha",
            {"state": "prepared", "retry_reason": "again", "results": []},
            expected_revision=2,
        )
    with pytest.raises(pj.InvalidChange):
        pj.update(
            directory,
            "alpha",
            {"state": "prepared", "retry_reason": "again", "attempts": 0},
            expected_revision=2,
        )
    reopened = pj.update(
        directory, "alpha", {"state": "prepared", "retry_reason": "again"},
        expected_revision=2,
    )
    assert reopened["attempts"] == 3
    assert reopened["results"] == [{"id": "a0001"}]
    assert len(reopened["history"]) == 3


def test_every_numbered_event_is_retained(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    for index in range(5):
        pj.update(
            directory, "alpha", {"state": "running", "note": index},
            expected_revision=index,
        )
    names = sorted(entry.name for entry in (directory / "events").iterdir())
    assert names == [f"{index:08d}.json" for index in range(1, 6)]
    assert pj.load(directory)["revision"] == 5


def test_busy_lock_is_reported_to_the_caller(tmp_path):
    from cmre.coordination import lock

    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    with lock(directory / ".lock"):
        with pytest.raises(BusyError):
            pj.update(directory, "alpha", {"state": "running"}, expected_revision=0)
    assert pj.load(directory)["jobs"]["alpha"]["revision"] == 0


def test_concurrent_disjoint_updates_keep_the_log_consistent(tmp_path):
    directory = tmp_path / "store"
    pj.create(directory, make_manifest(), make_identity())
    failures = []

    def worker(job_id):
        for index in range(5):
            for _attempt in range(100):
                try:
                    pj.update(
                        directory,
                        job_id,
                        {"note": f"{job_id}-{index}"},
                        expected_revision=index,
                    )
                    break
                except BusyError:
                    time.sleep(0.01)
            else:
                failures.append(job_id)

    threads = [
        threading.Thread(target=worker, args=("alpha",)),
        threading.Thread(target=worker, args=("beta",)),
    ]
    for thread in threads:
        thread.start()
    for thread in threads:
        thread.join(30)
    assert not failures
    state = pj.load(directory)
    assert state["jobs"]["alpha"]["revision"] == 5
    assert state["jobs"]["beta"]["revision"] == 5
    assert state["revision"] == 10
    names = sorted(entry.name for entry in (directory / "events").iterdir())
    assert names == [f"{index:08d}.json" for index in range(1, 11)]
