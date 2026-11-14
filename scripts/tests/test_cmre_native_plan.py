"""Asset-free declared native policy checks; no game, network or model calls."""
import hashlib
import json

import pytest

from scripts import cmre_native_plan as native
from scripts import cmre_session as session


def file_pin(path):
    return {"path": str(path), "sha256": hashlib.sha256(path.read_bytes()).hexdigest()}


@pytest.fixture
def plan(tmp_path, monkeypatch):
    monkeypatch.setattr(native, "ROOT", tmp_path)
    monkeypatch.setattr(session, "ROOT", tmp_path)
    parent = tmp_path / "parent.json"
    parent.write_text(json.dumps({
        "mode": "start", "state": "finished", "exit_code": 0, "coverage": {"source_functions": 1},
        "steps": [{"command": ["py", "-3.13", "-m", *args], "state": "finished", "exit_code": 0}
                  for args in [("cmre", "doctor"), ("cmre", "status"), ("pytest",)]]}), encoding="utf-8")
    receipt = file_pin(parent)
    workers = [{"id": tier, "class": tier, "model": model, "role": "WORKER",
                "preflight": "inherit", "parent_receipt": receipt}
               for tier, model in native.MODELS.items()]
    roles = {role: ("expert" if classes == {"expert"} else
                    "senior" if "routine" not in classes else "routine")
             for role, classes in native.ROLE_CLASSES.items()}
    return {"schema": "cmre-native-coordination/1", "mode": "coordination-only",
            "authorization": "Explicit user instruction, 2026-11-12", "parent_receipt": receipt,
            "workers": workers, "roles": roles, "steward": "expert", "writer_slots": 1,
            "tasks": [{"id": "bounded-docs", "kind": "routine", "author": "routine",
                       "reviewer": "senior", "steward": "expert",
                       "writable_paths": ["CM0102/Analysis/cmre/private.txt"],
                       "bounds": {"max_messages": 30, "max_seconds": 900, "remaining_failures": 5},
                       "state": "planned", "pins": {}}]}


def test_valid_planned_and_ready_bundle(plan, tmp_path):
    assert native.validate_plan(plan)["ready"] == 0
    task = plan["tasks"][0]
    task["state"] = "ready"
    for field in ("source", "hints", "report"):
        path = tmp_path / field
        path.write_text(field, encoding="utf-8")
        task["pins"][field] = file_pin(path)
    verdict_path = tmp_path / "verdict"
    verdict_path.write_text(json.dumps({"schema": "cmre-native-review/1", "task": task["id"],
                                       "author": task["author"], "reviewer": task["reviewer"],
                                       "verdict": "approve", "pins": dict(task["pins"])}), encoding="utf-8")
    task["pins"]["verdict"] = file_pin(verdict_path)
    assert native.validate_plan(plan)["ready"] == 1
    verdict = json.loads(verdict_path.read_text(encoding="utf-8"))
    verdict["pins"]["source"]["sha256"] = "0" * 64
    verdict_path.write_text(json.dumps(verdict), encoding="utf-8")
    task["pins"]["verdict"] = file_pin(verdict_path)
    with pytest.raises(ValueError, match="Review verdict"):
        native.validate_plan(plan)
    verdict["pins"]["source"] = dict(task["pins"]["source"])
    verdict_path.write_text(json.dumps(verdict), encoding="utf-8")
    task["pins"]["verdict"] = file_pin(verdict_path)
    (tmp_path / "source").write_text("changed", encoding="utf-8")
    with pytest.raises(ValueError, match="changed pinned"):
        native.validate_plan(plan)


@pytest.mark.parametrize("role", native.ROLE_CLASSES)
def test_every_role_must_be_delegated(plan, role):
    plan["roles"][role] = "parent"
    with pytest.raises(ValueError, match="Wrong model class"):
        native.validate_plan(plan)


@pytest.mark.parametrize("role", [r for r, classes in native.ROLE_CLASSES.items() if "routine" not in classes])
def test_routine_cannot_take_senior_or_expert_role(plan, role):
    plan["roles"][role] = "routine"
    with pytest.raises(ValueError, match="Wrong model class"):
        native.validate_plan(plan)


@pytest.mark.parametrize("mutate", [
    lambda p: p["roles"].pop("inputs"),
    lambda p: p.update(writer_slots=2),
    lambda p: p.update(writer_slots=True),
    lambda p: p["workers"][0].update(model="other/model"),
    lambda p: p["workers"][0].update(role="PARENT"),
    lambda p: p["workers"][1].update(preflight="start"),
    lambda p: p["workers"][1].update(parent_receipt={"path": "elsewhere", "sha256": "0" * 64}),
    lambda p: p["tasks"][0].update(reviewer="routine"),
    lambda p: p["tasks"][0].update(author="expert"),
    lambda p: p["tasks"][0].update(kind="hard"),
    lambda p: p["tasks"][0].update(bounds=""),
    lambda p: p["tasks"][0]["bounds"].update(remaining_failures=6),
    lambda p: p["tasks"][0]["bounds"].update(max_messages=31),
    lambda p: p["tasks"][0]["bounds"].update(max_seconds=True),
    lambda p: p["tasks"][0].update(writable_paths=["private/*"]),
    lambda p: p["tasks"][0].update(writable_paths=["."]),
    lambda p: p["tasks"][0].update(writable_paths=["recovery/c/fn_00401000.c"]),
    lambda p: p["tasks"][0].update(writable_paths=["CM0102/Analysis/cmre/../../Game/x"]),
    lambda p: p["tasks"][0].update(writable_paths=["C:/outside/private.txt"]),
    lambda p: p["tasks"][0].update(writable_paths=["CM0102/Analysis/cmre/a", "CM0102/Analysis/cmre/a/b"]),
    lambda p: p["tasks"][0].update(state="ready"),
    lambda p: p["tasks"].append({**p["tasks"][0], "id": "overlap"}),
])
def test_bad_plan_rejected(plan, mutate):
    mutate(plan)
    with pytest.raises(ValueError):
        native.validate_plan(plan)


@pytest.mark.parametrize("state,code", [("running", 0), ("finished", 1)])
def test_preflight_must_be_finished_green(plan, state, code):
    path = native.ROOT / plan["parent_receipt"]["path"]
    data = json.loads(path.read_text(encoding="utf-8"))
    data.update(state=state, exit_code=code)
    path.write_text(json.dumps(data), encoding="utf-8")
    plan["parent_receipt"].update(file_pin(path))
    with pytest.raises(ValueError, match="successful, finished preflight"):
        native.validate_plan(plan)


def test_allocation_checkpoint_contract_and_handoff(plan, tmp_path):
    allocation = {"schema": "cmre-worker-allocation/1", "mode": "coordination-only",
                  "authorization": "Explicit user instruction", "assignments": [],
                  "reason": "Prepared bounded plan", "retry_when": "Dispatch after steward lint",
                  "native_plan": plan}
    assert session.validate_allocation(allocation) is allocation
    session.validate_native_parent(allocation, {"receipt": plan["parent_receipt"]["path"]})
    with pytest.raises(ValueError, match="differs from checkpoint"):
        session.validate_native_parent(allocation, {"receipt": "other-parent.json"})
    text = session.handoff({"receipt": "parent.json"}, "HARNESS.md", "Continue assigned task", [], {},
                           allocation=allocation, allocation_path=tmp_path / "allocation.json")
    assert "NATIVE-COORDINATION.md" in text
    assert "Workflow: docs/agent/archive/NATIVE-COORDINATION.md" in text
    session.validate_handoff("NATIVE-COORDINATION.md", "Continue", [])
    assert "parent reads bounded receipts only" in text
    assert "Read relevant evidence packets" not in text
    plan["writer_slots"] = 2
    with pytest.raises(ValueError):
        session.validate_allocation(allocation)


def test_cli_is_read_only_and_bounded(plan, tmp_path, capsys):
    path = tmp_path / "plan.json"
    path.write_text(json.dumps(plan), encoding="utf-8")
    before = path.read_bytes()
    assert native.main([str(path)]) == 0
    assert "no dispatch" in capsys.readouterr().out
    assert path.read_bytes() == before
    path.write_text("{}", encoding="utf-8")
    assert native.main([str(path)]) == 2
    assert "Invalid native plan fields" in capsys.readouterr().err


def test_expert_task_requires_second_expert_author(plan):
    worker = {**plan["workers"][0], "id": "expert-author"}
    plan["workers"].append(worker)
    plan["roles"]["authoring"] = worker["id"]
    plan["tasks"][0].update(kind="expert", author=worker["id"])
    assert native.validate_plan(plan)["workers"] == 4


def test_malformed_pinned_preflight_is_rejected_without_traceback(plan, tmp_path, capsys):
    parent = tmp_path / "parent.json"
    parent.write_text("[]", encoding="utf-8")
    plan["parent_receipt"].update(file_pin(parent))
    path = tmp_path / "plan.json"
    path.write_text(json.dumps(plan), encoding="utf-8")
    assert native.main([str(path)]) == 2
    assert "Malformed inherited preflight" in capsys.readouterr().err


def test_native_checkpoint_roundtrip_without_any_gate(plan, tmp_path, monkeypatch):
    from contextlib import nullcontext
    from cmre import coordination, work

    monkeypatch.setattr(coordination, "writer", nullcontext)
    monkeypatch.setattr(work, "read", lambda: {"functions": {}})
    monkeypatch.setattr(session.Journal, "execute", lambda *args: pytest.fail("gate ran"))
    allocation = {"schema": "cmre-worker-allocation/1", "mode": "coordination-only",
                  "authorization": plan["authorization"], "native_plan": plan,
                  "assignments": [{"provider": "senior", "task": "Review bounded docs",
                                   "state": "held", "reason": "Waiting for frozen implementation",
                                   "retry_when": "Author returns pinned change manifest"}]}
    source = tmp_path / "allocation.json"
    source.write_text(json.dumps(allocation), encoding="utf-8")
    args = ["checkpoint", "--parent", plan["parent_receipt"]["path"], "--next", "Continue",
            "--workflow", "NATIVE-COORDINATION.md", "--allocation", str(source), "--write-handoff"]
    assert session.main(args) == 0
    sessions = tmp_path / "CM0102/Analysis/cmre/sessions"
    receipt = next(sessions.glob("*-checkpoint/receipt.json"))
    saved = json.loads(receipt.read_text(encoding="utf-8"))
    assert saved["worker_allocation"] == allocation
    assert saved["validation"] == "not-run"
    assert session.main(["resume", "--parent", plan["parent_receipt"]["path"]]) == 0
    handoff = (tmp_path / "docs/NEXT-SESSION-PROMPT.md").read_text(encoding="utf-8")
    assert "Workflow: docs/agent/archive/NATIVE-COORDINATION.md" in handoff
    assert "parent reads bounded receipts only" in handoff
