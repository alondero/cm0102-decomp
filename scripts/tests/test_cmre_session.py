"""Session safety: failures gate machine jobs, receipts survive, writers exclude."""
import json
import os
import subprocess
import sys
from contextlib import nullcontext

import pytest

from scripts import cmre_session as session


def commands_with_failure(failed, code=1):
    seen = []

    def execute(args):
        seen.append(args)
        return code if args == failed else 0

    return seen, execute


def test_bad_doctor_stops_before_tests_and_machine_work():
    seen, execute = commands_with_failure(("cmre", "doctor"))
    assert session.drive("run", ["batch", "-n", "500"], execute) == 1
    assert seen == [("cmre", "doctor")]


@pytest.mark.parametrize("job", ["batch", "revalidate"])
def test_failing_baseline_prevents_large_jobs(job):
    seen, execute = commands_with_failure(("pytest",))
    assert session.drive("run", [job, "--jobs", "16"], execute) == 1
    assert ("pytest",) in seen
    assert not any(job in cmd for cmd in seen)


def test_check_records_both_gates_and_keeps_generated_status_on_failure():
    seen, execute = commands_with_failure(("pytest",))
    assert session.drive("check", [], execute) == 1
    assert ("cmre", "verify") in seen
    assert ("cmre", "status", "--write") not in seen


def test_verification_failure_also_prevents_status_write():
    seen, execute = commands_with_failure(("cmre", "verify"), 3)
    assert session.drive("check", [], execute) == 3
    assert ("cmre", "status", "--write") not in seen


def test_green_check_regenerates_status_through_cmre():
    seen, execute = commands_with_failure(None)
    assert session.drive("check", [], execute) == 0
    assert seen[-1] == ("cmre", "status", "--write")


def test_machine_failure_propagates_and_options_reach_cli_unchanged():
    job = ("cmre", "batch", "-n", "7", "--max-bytes", "400")
    seen, execute = commands_with_failure(job, 9)
    assert session.drive("run", list(job[1:]), execute) == 9
    assert seen[-1] == job


def test_lock_excludes_another_writer_and_releases_after_exception(tmp_path):
    lock = tmp_path / "writer.lock"
    with pytest.raises(ValueError):
        with session.writer_lock(lock, tmp_path / "first.json"):
            with pytest.raises(session.BusyError, match="first.json"):
                with session.writer_lock(lock, tmp_path / "second.json"):
                    pytest.fail("second writer acquired a live lock")
            raise ValueError("interrupted work")
    # The persistent file is metadata; its existence must not block the next job.
    with session.writer_lock(lock, tmp_path / "third.json"):
        pass


def test_receipt_keeps_utf8_output_pid_and_real_exit_code(tmp_path, monkeypatch):
    monkeypatch.setattr(session, "ROOT", tmp_path)
    (tmp_path / "session_probe.py").write_text(
        "print('progress \\u2192 done')\nraise SystemExit(7)\n", encoding="utf-8")
    journal = session.Journal(tmp_path / "receipt", "start")
    assert journal.execute(("session_probe",)) == 7
    journal.finish(7)
    receipt = json.loads(journal.path.read_text(encoding="utf-8"))
    assert receipt["state"] == "finished"
    assert receipt["exit_code"] == 7
    assert receipt["steps"][0]["pid"] > 0
    assert receipt["steps"][0]["exit_code"] == 7
    log = journal.path.parent / receipt["steps"][0]["log"]
    assert "progress \u2192 done" in log.read_text(encoding="utf-8")


def test_receipt_starts_as_unfinished_evidence(tmp_path):
    journal = session.Journal(tmp_path / "receipt", "run")
    receipt = json.loads(journal.path.read_text(encoding="utf-8"))
    assert receipt["state"] == "running"
    assert receipt["pid"] > 0
    assert "exit_code" not in receipt


@pytest.mark.parametrize("state,code", [("running", None), ("finished", 0), ("finished", 1)])
def test_status_is_bounded_read_only_and_preserves_unfinished_or_failed_state(tmp_path, monkeypatch, capsys, state, code):
    receipt = tmp_path / "receipt.json"
    raw = {"mode": "check", "state": state, "pid": 123, "steps": [
        {"command": ["py", "-3.13", "-m", "pytest"], "state": state, "pid": 456,
         "exit_code": code, "log": "01-pytest.log", "inputs_before": {"PRIVATE-LARGE-INVENTORY": "x" * 500000}}]}
    if code is not None:
        raw["exit_code"] = code
    receipt.write_text(json.dumps(raw), encoding="utf-8")
    before = receipt.read_bytes()
    monkeypatch.setattr(session, "ROOT", tmp_path)
    def forbidden(*_args, **_kwargs):
        pytest.fail("read-only status started work or acquired a writer lease")
    monkeypatch.setattr(session, "writer_lock", forbidden)
    monkeypatch.setattr(session, "Journal", forbidden)
    assert session.main(["status", str(receipt)]) == 0
    output = capsys.readouterr().out
    report = json.loads(output)
    assert len(output) < 4000 and "PRIVATE-LARGE-INVENTORY" not in output
    assert report["state"] == state and report["exit_code"] == code
    assert report["steps"][0]["pid"] == 456
    assert report["steps"][0]["log"] == str(tmp_path / "01-pytest.log")
    assert receipt.read_bytes() == before and list(tmp_path.iterdir()) == [receipt]


def test_status_limits_steps_and_does_not_expand_arbitrary_nested_fields(tmp_path):
    receipt = tmp_path / "receipt.json"
    receipt.write_text(json.dumps({"mode": "run", "state": "running", "steps": [
        {"command": ["x" * 10000] * 50, "state": "running", "inputs_after": {"hidden": "x" * 10000}}
        for _ in range(100)]}), encoding="utf-8")
    report = session.receipt_status(receipt)
    assert report["step_count"] == 100 and report["omitted_steps"] == 92
    assert len(json.dumps(report)) < 11000


def test_status_distinguishes_missing_process_from_recorded_running(tmp_path, monkeypatch):
    receipt = tmp_path / "receipt.json"
    receipt.write_text(json.dumps({"state": "running", "pid": 123, "steps": [
        {"state": "finished", "pid": 789, "exit_code": 0},
        {"state": "running", "pid": 456},
        {"state": "starting"},
        {"state": "running", "pid": 777},
    ]}), encoding="utf-8")
    original = receipt.read_bytes()
    queried = []

    def observe(pid):
        queried.append(pid)
        return {123: "absent", 456: "present", 777: "unknown"}[pid]

    monkeypatch.setattr(session, "_observe_pid", observe)
    report = session.receipt_status(receipt)
    assert report["state"] == "running" and report["exit_code"] is None
    assert report["pid_observation"] == "absent"
    assert [step.get("pid_observation") for step in report["steps"]] == [
        None, "present", "unknown", "unknown"]
    assert queried == [123, 456, 777]
    assert receipt.read_bytes() == original


def test_status_cli_observes_exited_runner_and_child_without_finishing_receipt(tmp_path):
    receipt = tmp_path / "receipt.json"
    child_code = 259 if os.name == "nt" else 3
    runner = subprocess.Popen([sys.executable, "-c", "pass"])
    child = subprocess.Popen([sys.executable, "-c", f"raise SystemExit({child_code})"])
    assert runner.wait(timeout=10) == 0
    assert child.wait(timeout=10) == child_code
    receipt.write_text(json.dumps({"pid": runner.pid, "state": "running", "steps": [
        {"pid": child.pid, "state": "running"},
    ]}), encoding="utf-8")
    original = receipt.read_bytes()
    result = subprocess.run([sys.executable, session.__file__, "status", str(receipt)],
                            cwd=tmp_path, capture_output=True, check=True, timeout=10)
    report = json.loads(result.stdout)
    for row in (report, report["steps"][0]):
        assert row["pid_observation"] == "absent"
        assert row["state"] == "running" and row["exit_code"] is None
    assert receipt.read_bytes() == original


@pytest.mark.parametrize("raw", [[], {"steps": "wrong"}, {"steps": [None]},
    {"steps": [{"command": [None]}]}, {"steps": [{"log": "../outside.log"}]}])
def test_status_rejects_malformed_receipts_without_creating_a_journal(tmp_path, capsys, raw):
    receipt = tmp_path / "receipt.json"
    receipt.write_text(json.dumps(raw), encoding="utf-8")
    assert session.main(["status", str(receipt)]) == 2
    assert not capsys.readouterr().out
    assert list(tmp_path.iterdir()) == [receipt]


def test_failed_pid_receipt_stops_child_before_releasing_ownership(tmp_path, monkeypatch):
    monkeypatch.setattr(session, "ROOT", tmp_path)
    (tmp_path / "session_probe.py").write_text("import time\ntime.sleep(30)\n", encoding="utf-8")
    journal = session.Journal(tmp_path / "receipt", "run")
    processes = []
    real_popen = session.subprocess.Popen
    real_save = journal.save

    def launch(*args, **kwargs):
        process = real_popen(*args, **kwargs)
        processes.append(process)
        return process

    def fail_pid_write():
        if journal.data["steps"] and journal.data["steps"][-1].get("state") == "running":
            raise OSError("receipt disk error after spawn")
        real_save()

    monkeypatch.setattr(session.subprocess, "Popen", launch)
    monkeypatch.setattr(journal, "save", fail_pid_write)
    try:
        with pytest.raises(OSError, match="receipt disk error"):
            journal.execute(("session_probe",))
        assert processes[0].poll() is not None, "child survived a receipt failure"
    finally:
        for process in processes:
            session.stop_tree(process)


def test_runner_rejects_unrelated_commands_before_creating_receipts():
    with pytest.raises(SystemExit) as exc:
        session.main(["run", "push"])
    assert exc.value.code == 2


def test_resume_requires_successful_finished_preflight(tmp_path):
    parent = tmp_path / "receipt.json"
    for data in ({"mode": "check", "state": "finished", "exit_code": 0},
                 {"mode": "start", "state": "running", "exit_code": 0},
                 {"mode": "start", "state": "finished", "exit_code": 1}):
        parent.write_text(json.dumps(data))
        with pytest.raises(ValueError, match="preflight"):
            session.load_parent(parent)


def test_explicit_parent_progress_ignores_a_newer_unrelated_session(tmp_path):
    parent = tmp_path / "old-start" / "receipt.json"
    parent.parent.mkdir()
    data = {"mode": "start", "state": "finished", "exit_code": 0,
            "started": "2026-10-24T00:00:00+00:00", "coverage": {"source_body_bytes": 10},
            "steps": [{"command": ["py", "-3.13", "-m", *cmd], "exit_code": 0,
                       "state": "finished"} for cmd in (("cmre", "doctor"), ("cmre", "status"), ("pytest",))]}
    parent.write_text(json.dumps(data))
    assert session.load_parent(parent)["coverage"]["source_body_bytes"] == 10


def test_checkpoint_does_not_run_tests_and_preserves_blocker_obligations(tmp_path):
    parent = {"receipt": str(tmp_path / "start" / "receipt.json"), "started": "today",
              "coverage": {"source_body_bytes": 10}}
    states = {"fn_00401000": {"state": "evidence-needed", "reason": "Unknown receiver",
                              "retry_when": "Inspect original callers", "failures": 2}}
    text = session.handoff(parent, "C.md", "Inspect original receiver", ["fn_00401000"], states)
    assert "Unknown receiver" in text and "Inspect original callers" in text
    assert "failures=2" in text
    assert "not a green gate" in text
    assert len(text.splitlines()) <= 40


def test_checkpoint_rejects_more_context_than_one_bounded_batch():
    with pytest.raises(ValueError, match="20"):
        session.handoff({"receipt": "parent"}, "C.md", "next", [f"fn_{n:08x}" for n in range(21)], {})


def test_checkpoint_separates_matching_c_acceptance_from_historical_review():
    states = {'fn_00401000': {'state': 'needs-review', 'reason': 'pass', 'failures': 0}}
    accepted = {'fn_00401000': {'state': 'accepted-c', 'source': 'recovery/c/fn_00401000.c'}}
    text = session.handoff({'receipt': 'parent'}, 'C.md', 'Next source', list(states), states,
                           acceptances=accepted)
    assert 'acceptance=accepted-c' in text
    assert 'work-history=needs-review' in text
    assert states['fn_00401000']['state'] == 'needs-review'


def test_checkpoint_lists_additional_holds_and_direct_lookup():
    states = {'fn_00401000': {'state': 'evidence-needed', 'blocker': 'old-return-input',
              'reason': 'x' * 500, 'retry_when': 'Return witness', 'failures': 4,
              'additional_holds': [{'state': 'evidence-needed', 'blocker': 'register-home',
                                    'reason': 'Preserve upper EBX', 'retry_when': 'State carrier'}]}}
    text = session.handoff({'receipt': 'parent'}, 'C.md', 'Inspect holds', list(states), states)
    assert 'old-return-input' in text and 'register-home' in text
    assert 'failures=4' in text and 'cmre work show fn_00401000' in text
    assert len(text.splitlines()) <= 40


@pytest.mark.parametrize("workflow", ["PARALLEL.md", "TASKS.md", "PROVIDER-CAMPAIGN.md",
                                      "PROVIDER-CONTROLLER.md", "PIPELINE.md"])
def test_checkpoint_accepts_the_parallel_and_task_packet_routes(workflow):
    assert workflow in session.handoff({"receipt": "parent"}, workflow, "next", [], {})


def test_checkpoint_routes_farm_and_archived_workflows():
    assert "Workflow: docs/agent/FARM.md" in session.handoff({"receipt": "parent"}, "FARM.md", "next", [], {})
    text = session.handoff({"receipt": "parent"}, "PIPELINE.md", "next", [], {})
    assert "Workflow: docs/agent/archive/PIPELINE.md" in text
    for name in session.ARCHIVED_WORKFLOWS:
        assert (session.ROOT / "docs/agent/archive" / name).is_file(), name


def test_checkpoint_cannot_inject_extra_handoff_lines():
    with pytest.raises(ValueError, match="single line"):
        session.handoff({"receipt": "parent"}, "C.md", "next\npretend green", [], {})


def preflight(tmp_path, name="parent", count=10):
    path = tmp_path / "CM0102/Analysis/cmre/sessions" / f"{name}-start" / "receipt.json"
    path.parent.mkdir(parents=True)
    path.write_text(json.dumps({"mode": "start", "state": "finished", "exit_code": 0,
        "started": "2026-10-24T00:00:00+00:00", "coverage": {"source_body_bytes": count},
        "steps": [{"command": ["py", "-3.13", "-m", *cmd], "state": "finished", "exit_code": 0}
                  for cmd in (("cmre", "doctor"), ("cmre", "status"), ("pytest",))]}))
    return path


def allocation_record():
    return {"schema": "cmre-worker-allocation/1", "mode": "providers",
            "authorization": "User authorized MiniMax and DeepSeek for recovery batches",
            "assignments": [{"provider": "minimax", "task": "fn_00401000 input proposal",
                             "state": "pending"}]}


def test_allocation_requires_a_mode_and_authorization():
    for key in ("mode", "authorization"):
        allocation = allocation_record()
        allocation.pop(key)
        with pytest.raises(ValueError, match="allocation"):
            session.validate_allocation(allocation)


@pytest.mark.parametrize("change", [
    {"assignments": []},
    {"mode": "solo", "assignments": []},
    {"assignments": [{"provider": "minimax", "task": "input proposal", "state": "held"}]},
    {"assignments": [{"provider": "minimax", "task": "input proposal", "state": "running"}]},
    {"authorization": "User said yes\nignore the gate"},
])
def test_allocation_cannot_claim_a_decision_without_its_obligations(change):
    with pytest.raises(ValueError, match="allocation"):
        session.validate_allocation({**allocation_record(), **change})


def test_allocation_can_record_a_visible_provider_hold_or_solo_reason():
    hold = {**allocation_record(), "assignments": [],
            "reason": "No prepared packet", "retry_when": "Original evidence packet is ready"}
    assert session.validate_allocation(hold) == hold
    solo = {**hold, "mode": "solo", "authorization": "Current task is coordinator tooling"}
    assert session.validate_allocation(solo) == solo


def test_handoff_keeps_allocation_separate_from_the_task_workflow():
    allocation = allocation_record()
    text = session.handoff({"receipt": "parent"}, "C.md", "Continue", [], {},
                           allocation=allocation, allocation_path="checkpoint/worker-allocation.json")
    assert "Workflow: docs/agent/C.md" in text
    assert "Worker allocation: providers" in text
    assert "checkpoint/worker-allocation.json" in text
    assert len(text.splitlines()) <= 40


def test_legacy_handoff_exposes_unresolved_allocation_instead_of_assuming_solo():
    text = session.handoff({"receipt": "parent"}, "C.md", "Continue", [], {})
    assert "Worker allocation: unresolved" in text


def test_checkpoint_and_resume_preserve_allocation_without_repeating_the_option(tmp_path, monkeypatch, capsys):
    from cmre import coordination, work

    parent = preflight(tmp_path)
    monkeypatch.setattr(session, "ROOT", tmp_path)
    monkeypatch.setattr(coordination, "writer", nullcontext)
    monkeypatch.setattr(work, "read", lambda: {"functions": {}})
    monkeypatch.setattr(session.Journal, "execute", lambda *args: pytest.fail("validation ran"))
    allocation = allocation_record()
    source = tmp_path / "allocation.json"
    source.write_text(json.dumps(allocation))
    assert session.main(["checkpoint", "--parent", str(parent), "--next", "First batch",
                         "--allocation", str(source), "--write-handoff"]) == 0
    first = next(parent.parent.parent.glob("*-checkpoint/receipt.json"))
    first_data = json.loads(first.read_text())
    assert first_data["worker_allocation"] == allocation
    # The inherited snapshot must survive replacement of the original input file.
    source.write_text(json.dumps({"mode": "solo"}))
    assert session.main(["checkpoint", "--parent", str(parent), "--next", "Second batch",
                         "--write-handoff"]) == 0
    second = sorted(parent.parent.parent.glob("*-checkpoint/receipt.json"))[-1]
    assert second != first
    data = json.loads(second.read_text())
    assert data["worker_allocation"] == allocation
    snapshot = second.parent / "worker-allocation.json"
    assert json.loads(snapshot.read_text()) == allocation
    text = (tmp_path / "docs/NEXT-SESSION-PROMPT.md").read_text()
    assert "Worker allocation: providers" in text and str(snapshot) in text
    assert session.main(["resume", "--parent", str(parent)]) == 0
    output = capsys.readouterr().out
    assert "Worker allocation: providers" in output and str(snapshot) in output


def test_checkpoint_rejects_invalid_allocation_before_publishing(tmp_path, monkeypatch):
    from cmre import coordination, work

    parent = preflight(tmp_path)
    monkeypatch.setattr(session, "ROOT", tmp_path)
    monkeypatch.setattr(coordination, "writer", nullcontext)
    monkeypatch.setattr(work, "read", lambda: pytest.fail("read work before checking allocation"))
    source = tmp_path / "allocation.json"
    source.write_text(json.dumps({"mode": "providers"}))
    assert session.main(["checkpoint", "--parent", str(parent), "--next", "Continue",
                         "--allocation", str(source)]) == 2
    assert not list(parent.parent.parent.glob("*-checkpoint/receipt.json"))


def test_allocation_inherits_only_from_the_same_parent(tmp_path, monkeypatch):
    from cmre import coordination, work

    parent = preflight(tmp_path)
    other = preflight(tmp_path, "other")
    checkpoint = parent.parent.parent / "000-checkpoint"
    checkpoint.mkdir()
    (checkpoint / "receipt.json").write_text(json.dumps({"mode": "checkpoint", "state": "finished",
        "parent": str(other.resolve()), "worker_allocation": allocation_record()}))
    monkeypatch.setattr(session, "ROOT", tmp_path)
    monkeypatch.setattr(coordination, "writer", nullcontext)
    monkeypatch.setattr(work, "read", lambda: {"functions": {}})
    assert session.main(["checkpoint", "--parent", str(parent), "--next", "Continue"]) == 0
    latest = sorted(parent.parent.parent.glob("*-checkpoint/receipt.json"))[-1]
    assert json.loads(latest.read_text())["worker_allocation"]["mode"] == "unresolved"


@pytest.mark.parametrize("state", ["running", "completed"])
def test_allocation_retains_existing_receipts_and_requires_missing_ones(tmp_path, monkeypatch, state):
    monkeypatch.setattr(session, "ROOT", tmp_path)
    receipt = tmp_path / "worker-receipt.json"
    receipt.write_text("{}")
    allocation = allocation_record()
    allocation["assignments"][0].update(state=state, receipt=receipt.name)
    assert session.validate_allocation(allocation) == allocation
    receipt.unlink()
    with pytest.raises(ValueError, match="existing receipt"):
        session.validate_allocation(allocation)


def test_resume_detects_a_changed_allocation_snapshot_and_explicit_update_repairs_it(tmp_path, monkeypatch):
    from cmre import coordination, work

    parent = preflight(tmp_path)
    monkeypatch.setattr(session, "ROOT", tmp_path)
    monkeypatch.setattr(coordination, "writer", nullcontext)
    monkeypatch.setattr(work, "read", lambda: {"functions": {}})
    source = tmp_path / "allocation.json"
    source.write_text(json.dumps(allocation_record()))
    assert session.main(["checkpoint", "--parent", str(parent), "--next", "Continue",
                         "--allocation", str(source)]) == 0
    checkpoint = next(parent.parent.parent.glob("*-checkpoint/receipt.json"))
    (checkpoint.parent / "worker-allocation.json").write_text("{}")
    assert session.main(["resume", "--parent", str(parent)]) == 2
    # Explicit replacement must bypass the damaged old snapshot, not erase authorization.
    assert session.main(["checkpoint", "--parent", str(parent), "--next", "Continue",
                         "--allocation", str(source)]) == 0
    assert session.main(["resume", "--parent", str(parent)]) == 0


def test_checkpoint_and_resume_never_invoke_validation_or_overwrite_handoff(tmp_path, monkeypatch, capsys):
    from cmre import coordination, work

    parent = preflight(tmp_path)
    monkeypatch.setattr(session, "ROOT", tmp_path)
    monkeypatch.setattr(coordination, "writer", nullcontext)
    monkeypatch.setattr(work, "read", lambda: {"functions": {}})
    monkeypatch.setattr(work, "describe", lambda va, records: {
        "function": f"fn_{va:08x}", "acceptance": {"state": "unaccepted"}})
    monkeypatch.setattr(session.Journal, "execute", lambda *args: pytest.fail("validation ran"))
    handoff_path = tmp_path / "docs/NEXT-SESSION-PROMPT.md"
    handoff_path.parent.mkdir()
    handoff_path.write_text("existing recovery task")
    assert session.main(["checkpoint", "--parent", str(parent), "--workflow", "HARNESS.md",
                         "--next", "Review packet invalidation", "--functions", "fn_00401000"]) == 0
    checkpoint = next(parent.parent.parent.glob("*-checkpoint/receipt.json"))
    data = json.loads(checkpoint.read_text())
    assert data["steps"] == [] and data["validation"] == "not-run"
    assert data["parent"] == str(parent.resolve())
    assert handoff_path.read_text() == "existing recovery task"
    assert session.main(["resume", "--parent", str(parent)]) == 0
    assert str(checkpoint.parent / "handoff.md") in capsys.readouterr().out


def test_checkpoint_retains_complete_function_details_without_changing_work(tmp_path, monkeypatch):
    from cmre import coordination, work

    parent = preflight(tmp_path)
    monkeypatch.setattr(session, 'ROOT', tmp_path)
    monkeypatch.setattr(coordination, 'writer', nullcontext)
    state = {'state': 'needs-review', 'reason': 'pass', 'failures': 0,
             'additional_holds': [{'blocker': 'register-home', 'reason': 'Full untruncated reason'}]}
    states = {'fn_00401000': state}
    monkeypatch.setattr(work, 'read', lambda: {'functions': states})
    monkeypatch.setattr(work, 'describe', lambda va, records: {
        'function': 'fn_00401000', 'acceptance': {'state': 'accepted-c'}, 'work': records['fn_00401000']})
    monkeypatch.setattr(session.Journal, 'execute', lambda *args: pytest.fail('validation ran'))
    assert session.main(['checkpoint', '--parent', str(parent), '--next', 'Continue',
                         '--functions', 'fn_00401000']) == 0
    path = next(parent.parent.parent.glob('*-checkpoint/receipt.json'))
    data = json.loads(path.read_text())
    assert data['function_details']['fn_00401000']['work'] == state
    assert data['function_details']['fn_00401000']['acceptance']['state'] == 'accepted-c'
    text = (path.parent / 'handoff.md').read_text()
    assert 'acceptance=accepted-c work-history=needs-review' in text
    assert state['state'] == 'needs-review'


def test_checkpoint_bounds_context_before_reading_function_details(tmp_path, monkeypatch):
    from cmre import coordination, work

    parent = preflight(tmp_path)
    monkeypatch.setattr(session, 'ROOT', tmp_path)
    monkeypatch.setattr(coordination, 'writer', nullcontext)
    monkeypatch.setattr(work, 'describe', lambda *args: pytest.fail('read unbounded context'))
    assert session.main(['checkpoint', '--parent', str(parent), '--next', 'Continue',
                         '--functions', *[f'fn_{n:08x}' for n in range(21)]]) == 2


def test_closing_check_uses_explicit_parent_and_still_runs_both_gates(tmp_path, monkeypatch):
    from cmre import coordination

    parent = preflight(tmp_path, "old", 10)
    preflight(tmp_path, "zzz-new", 500)
    monkeypatch.setattr(session, "ROOT", tmp_path)
    monkeypatch.setattr(coordination, "writer", nullcontext)
    monkeypatch.setattr(session, "coverage", lambda: {"source_body_bytes": 40})
    seen = []
    monkeypatch.setattr(session.Journal, "execute", lambda self, cmd: seen.append(cmd) or 0)
    assert session.main(["check", "--parent", str(parent)]) == 0
    assert seen == [("pytest",), ("cmre", "verify"), ("cmre", "status", "--write")]
    path = next(parent.parent.parent.glob("*-check/receipt.json"))
    data = json.loads(path.read_text())
    assert data["progress"]["source_body_bytes"] == 30
    assert data["parent"] == str(parent.resolve())


def test_tooling_can_close_against_a_failed_baseline_without_reusing_it_as_green(tmp_path, monkeypatch):
    from cmre import coordination

    parent = preflight(tmp_path)
    data = json.loads(parent.read_text())
    data.update(exit_code=1)
    data.pop("coverage")
    data["steps"][-1]["exit_code"] = 1
    parent.write_text(json.dumps(data))
    monkeypatch.setattr(session, "ROOT", tmp_path)
    monkeypatch.setattr(coordination, "writer", nullcontext)
    monkeypatch.setattr(session, "coverage", lambda: {"source_body_bytes": 40})
    seen = []
    monkeypatch.setattr(session.Journal, "execute", lambda self, cmd: seen.append(cmd) or 0)
    assert session.main(["resume", "--parent", str(parent)]) == 2
    assert session.main(["check", "--parent", str(parent)]) == 0
    assert ("pytest",) in seen and ("cmre", "verify") in seen


def test_failed_handoff_publication_cannot_leave_a_successful_checkpoint(tmp_path, monkeypatch):
    from cmre import coordination, work

    parent = preflight(tmp_path)
    monkeypatch.setattr(session, "ROOT", tmp_path)
    monkeypatch.setattr(coordination, "writer", nullcontext)
    monkeypatch.setattr(work, "read", lambda: {"functions": {}})
    real_write = coordination.atomic_bytes

    def write(path, data):
        if path.name == "NEXT-SESSION-PROMPT.md":
            raise OSError("handoff publication failed")
        return real_write(path, data)

    monkeypatch.setattr(coordination, "atomic_bytes", write)
    assert session.main(["checkpoint", "--parent", str(parent), "--next", "next", "--write-handoff"]) == 2
    path = next(parent.parent.parent.glob("*-checkpoint/receipt.json"))
    data = json.loads(path.read_text())
    assert data["state"] == "error" and data["exit_code"] != 0


def test_checkpoint_preserves_parent_gate_failures_outside_function_work_state(tmp_path, monkeypatch):
    from cmre import coordination, work

    parent = preflight(tmp_path)
    failed = parent.parent.parent / "zzz-check" / "receipt.json"
    failed.parent.mkdir()
    failed.write_text(json.dumps({"mode": "check", "state": "finished", "exit_code": 1,
        "parent": str(parent.resolve()), "steps": [{"command": ["py", "-3.13", "-m", "pytest"],
            "exit_code": 1, "log": "01-pytest.log"}]}))
    (failed.parent / "01-pytest.log").write_text("FAILED tests/test_x.py::test_original_contract - mismatch\n")
    monkeypatch.setattr(session, "ROOT", tmp_path)
    monkeypatch.setattr(coordination, "writer", nullcontext)
    monkeypatch.setattr(work, "read", lambda: {"functions": {}})
    assert session.main(["checkpoint", "--parent", str(parent), "--next", "Resolve failing contract",
                         "--write-handoff"]) == 0
    text = (tmp_path / "docs/NEXT-SESSION-PROMPT.md").read_text()
    assert str(failed) in text and "FAILED tests/test_x.py::test_original_contract" in text
    assert len(text.splitlines()) <= 40
