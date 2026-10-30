"""Campaign reports never confuse available source with recovered execution."""
import copy
from types import SimpleNamespace

import pytest

from cmre import campaign, env

A, B, C = 0x00401000, 0x00402000, 0x00403000


@pytest.fixture
def inventory(monkeypatch):
    value = SimpleNamespace(functions={v: SimpleNamespace(body_bytes=n) for v, n in ((A, 20), (B, 30), (C, 10))},
                            callees={A: [B, C], B: [], C: []})
    monkeypatch.setattr(campaign.inventory, "load", lambda: value)
    monkeypatch.setattr(campaign.ledger, "recovered", lambda: {})
    return value


def entry(c=False, legacy=False):
    record = {"lifted": {"source": "x"}, "status": {"validated": True, "integrated": False}}
    if c:
        record["c"] = {"source": "c"}
    if legacy:
        record = {"decompiled": True, "validated": True, "integrated": False}
    return SimpleNamespace(record=record, legacy=legacy, source="ledger.json")


def test_c_available_does_not_close_composition_or_external_dependencies(inventory, monkeypatch):
    monkeypatch.setattr(campaign.ledger, "recovered", lambda: {A: entry(True), B: entry(True), C: entry(legacy=True)})
    report = campaign.status(campaign.create("slice", [A, B]))
    assert report["counts"] == {"lifted": 2, "c": 2, "validated": 2, "integrated": 0}
    assert report["functions"][0]["outside_slice"] == ["fn_00403000"]
    assert report["functions"][0]["imports"]["status"] == "unknown"
    assert all(e["execution_binding"] == "original-until-composition-proven" for e in report["known_direct_edges"])
    assert not report["composition"]["closed"]
    assert not report["milestone_accepted"]


def test_queue_is_bounded_and_c_ready_before_size(inventory, monkeypatch):
    monkeypatch.setattr(campaign.ledger, "recovered", lambda: {A: entry(), B: entry(), C: entry()})
    manifest = campaign.create("slice", [A, B, A])
    assert manifest["functions"] == ["fn_00401000", "fn_00402000"]
    report = campaign.status(manifest)
    assert [r["function"] for r in report["c_queue"]] == ["fn_00402000", "fn_00401000"]
    assert report["c_queue"][1]["pending_callees"] == 2


def test_manifest_identity_membership_and_exclusive_write(inventory, tmp_path):
    with pytest.raises(ValueError, match="inventory"):
        campaign.create("bad", [0x00404000])
    manifest = campaign.create("slice", [A])
    path = tmp_path / "slice.json"
    campaign.save(path, manifest)
    assert campaign.load(path) == manifest
    with pytest.raises(FileExistsError):
        campaign.save(path, manifest)
    manifest["input_sha256"] = "0" * 64
    with pytest.raises(ValueError, match="primary"):
        campaign.status(manifest)


def replay():
    return {"schema": campaign.REPLAY_SCHEMA, "input_sha256": env.TARGET_SHA256,
            "actual_module_base": env.IMAGE_BASE, "scenario_sha256": "1" * 64,
            "environment_sha256": "2" * 64, "initial_state_sha256": "3" * 64,
            "observer_evidence_sha256": "4" * 64,
            "checkpoints": [{"id": f"day-{i}", "consumed_input_count": i,
                             "consumption": "observed", "consumed_inputs_sha256": str(i) * 64,
                             "state_sha256": str(i + 3) * 64, "rng_sha256": "6" * 64,
                             "ack_evidence_sha256": "7" * 64} for i in range(2)]}


def test_replay_matches_bounded_observations_without_accepting_m2():
    report = campaign.compare_replays(replay(), replay())
    assert report["status"] == "matching-observations"
    assert report["milestone_accepted"] is False


@pytest.mark.parametrize("field", ["consumed_inputs_sha256", "state_sha256", "rng_sha256"])
def test_replay_first_divergence(field):
    left = replay()
    right = copy.deepcopy(left)
    right["checkpoints"][1][field] = "f" * 64
    report = campaign.compare_replays(left, right)
    assert report["status"] == "diverged"
    assert report["checkpoint_index"] == 1
    assert report["field"] == field


def test_replay_rejects_posting_gdi_and_environment_mismatch():
    bad = replay()
    bad["checkpoints"][1]["consumption"] = "posted"
    with pytest.raises(ValueError, match="consumption"):
        campaign.compare_replays(replay(), bad)
    bad = replay()
    bad["input_sha256"] = "0" * 64
    with pytest.raises(ValueError, match="primary"):
        campaign.compare_replays(replay(), bad)
    bad = replay()
    bad["environment_sha256"] = "0" * 64
    assert campaign.compare_replays(replay(), bad)["status"] == "incomparable"


def test_replay_rejects_empty_and_unbound_initial_observations():
    bad = replay()
    bad["checkpoints"] = []
    with pytest.raises(ValueError, match="checkpoints"):
        campaign.compare_replays(replay(), bad)
    bad = replay()
    bad["checkpoints"][0]["state_sha256"] = "f" * 64
    with pytest.raises(ValueError, match="initial"):
        campaign.compare_replays(replay(), bad)
