"""Read-only planning reuses current packets without duplicating live ownership."""
import copy
import time
from types import SimpleNamespace

import pytest

from cmre import campaign, env, tasks, work

A, B, C = 0x00401000, 0x00402000, 0x00403000


@pytest.fixture
def setup(monkeypatch, tmp_path):
    inv = SimpleNamespace(functions={v: SimpleNamespace(body_bytes=n) for v, n in ((A, 20), (B, 30), (C, 700))},
                          callees={A: [B], B: [], C: []})
    monkeypatch.setattr(campaign.inventory, "load", lambda: inv)
    records = {v: SimpleNamespace(record={"lifted": {"source": "x"}, "status": {"validated": True}},
                                  legacy=False, source="ledger.json") for v in (A, B, C)}
    monkeypatch.setattr(campaign.ledger, "recovered", lambda: records)
    monkeypatch.setattr(env, "C_DIR", tmp_path)
    monkeypatch.setattr(env, "HINTS_DIR", tmp_path / 'hints')
    packets = []
    monkeypatch.setattr(tasks, "status", lambda: packets)
    monkeypatch.setattr(tasks, "validate_current", lambda packet: None)
    return campaign.create("slice", [A, B, C]), records, packets


def packet(va, state="prepared", *, live=False, name="packet"):
    return {"task_id": name, "va": hex(va), "status": state, "created_at": 1,
            "abi": {"analyst_contract": {"claims": {}}},
            "attempts": [], "lease": {"expires_at": time.time() + 3600, "token": "secret"} if live else None}


def test_reuses_current_packet_and_prepares_only_contracted_scoped_functions(setup):
    manifest, _, packets = setup
    packets.append(packet(B))
    (env.C_DIR / "contracts").mkdir()
    (env.C_DIR / "contracts/fn_00401000.json").write_text("{}")
    before = copy.deepcopy(packets)
    plan = campaign.plan(manifest, workers=2)
    assert [r["function"] for r in plan["dispatch"]] == ["fn_00402000"]
    assert [r["function"] for r in plan["prepare"]] == ["fn_00401000"]
    assert plan["prepare"][0]["contract_status"] == "present-unverified"
    assert plan["blocked"][0]["reason"] == "outside-packet-lane"
    assert plan["source_coverage"]["accepted_c_legacy_body_bytes"] == 0
    assert plan["source_coverage"]["selected_body_bytes"] == 750
    assert packets == before
    assert "secret" not in str(plan)
    assert plan["milestone_accepted"] is False


def test_live_stale_lease_is_not_redispatched_and_submission_returns_to_review(setup, monkeypatch):
    manifest, _, packets = setup
    packets.extend([packet(A, "claimed", live=True, name="live"), packet(B, "submitted", name="review")])

    def validate(value):
        if value["task_id"] == "live":
            raise ValueError("Stale task inputs")

    monkeypatch.setattr(tasks, "validate_current", validate)
    plan = campaign.plan(manifest, workers=1)
    assert not plan["dispatch"] and not plan["prepare"]
    assert plan["active"][0]["task_id"] == "live"
    assert plan["active"][0]["input_error"] == "Stale task inputs"
    assert plan["review"][0]["task_id"] == "review"


@pytest.mark.parametrize("state", ["escalated", "exhausted", "publishing", "publication-failed"])
def test_escalations_and_publication_failures_require_coordinator_action(setup, state):
    manifest, _, packets = setup
    packets.append(packet(B, state))
    plan = campaign.plan(manifest)
    assert not plan["dispatch"]
    assert any(row["task_id"] == "packet" for row in plan["review"] + plan["blocked"])


def test_dispatch_limit_reserves_live_workers_and_prioritizes_dependencies(setup):
    manifest, _, packets = setup
    packets.extend([packet(A, name="caller"), packet(B, name="callee")])
    plan = campaign.plan(manifest, workers=1)
    assert [row["task_id"] for row in plan["dispatch"]] == ["callee"]
    packets[0] = packet(A, "claimed", live=True, name="caller")
    plan = campaign.plan(manifest, workers=2)
    assert len(plan["active"]) == len(plan["dispatch"]) == 1
    assert plan["dispatch"][0]["task_id"] == "callee"


def test_source_acceptance_does_not_hide_interrupted_publication(setup):
    manifest, records, packets = setup
    records[B].record["c"] = {"source": "c"}
    packets.append(packet(B, "publishing"))
    plan = campaign.plan(manifest)
    assert plan["source_coverage"]["accepted_c_legacy_body_bytes"] == 30
    assert plan["review"][0]["action"] == "reconcile-publication"


def test_stale_idle_packet_needs_new_inputs_and_missing_abi_is_not_dispatchable(setup, monkeypatch):
    manifest, _, packets = setup
    packets.append(packet(B))

    def validate(_):
        raise ValueError("changed hints")

    monkeypatch.setattr(tasks, "validate_current", validate)
    plan = campaign.plan(manifest)
    assert not plan["dispatch"] and not plan["prepare"]
    assert plan["stale_tasks"][0]["task_id"] == "packet"
    assert any(row["function"] == "fn_00402000" and row["reason"] == "abi-evidence-required"
               for row in plan["blocked"])
    with pytest.raises(ValueError, match="workers"):
        campaign.plan(manifest, workers=0)


def test_prepared_packet_without_analyst_contract_is_evidence_work(setup):
    manifest, _, packets = setup
    value = packet(B)
    value["abi"] = {}
    packets.append(value)
    plan = campaign.plan(manifest)
    assert not plan["dispatch"]
    assert any(row.get("task_id") == "packet" and row["reason"] == "abi-evidence-required"
               for row in plan["blocked"])


@pytest.mark.parametrize('state', ['harness-blocker', 'evidence-needed'])
def test_plan_reports_hard_hold_before_contract_or_dispatch(setup, monkeypatch, state):
    manifest, _, packets = setup
    (env.C_DIR / 'contracts').mkdir()
    (env.C_DIR / 'contracts/fn_00402000.json').write_text('{}')
    monkeypatch.setattr(work, 'read', lambda: {'functions': {'fn_00402000':
        {'state': state, 'retry_when': 'Resolve nested fault', 'blocker': 'nested-fault'}}})
    plan = campaign.plan(manifest, workers=2)
    assert not any(row['function'] == 'fn_00402000' for row in plan['prepare'] + plan['dispatch'])
    assert any(row['function'] == 'fn_00402000' and row['reason'] == 'work-state-hold'
               and row['retry_when'] == 'Resolve nested fault' for row in plan['blocked'])
    packets.append(packet(B, 'submitted', name='ready-for-review'))
    plan = campaign.plan(manifest, workers=2)
    assert any(row['task_id'] == 'ready-for-review' for row in plan['review'])


def test_plan_reports_unreviewed_existing_source_as_hold(setup):
    manifest, _, _ = setup
    (env.C_DIR / 'fn_00402000.c').write_text('retained WIP')
    plan = campaign.plan(manifest)
    assert any(row['function'] == 'fn_00402000' and row['reason'] == 'existing-source-needs-review'
               for row in plan['blocked'])


def test_plan_exposes_hold_on_live_and_submitted_tasks(setup, monkeypatch):
    manifest, _, packets = setup
    monkeypatch.setattr(work, 'read', lambda: {'functions': {'fn_00402000':
        {'state': 'harness-blocker', 'retry_when': 'Resolve nested fault', 'blocker': 'nested'}}})
    packets.append(packet(B, 'claimed', live=True, name='live-held'))
    plan = campaign.plan(manifest)
    assert plan['active'][0]['hold']['retry_when'] == 'Resolve nested fault'
    packets[0] = packet(B, 'submitted', name='submitted-held')
    plan = campaign.plan(manifest)
    assert plan['review'][0]['task_id'] == 'submitted-held'
    assert plan['review'][0]['hold']['retry_when'] == 'Resolve nested fault'
