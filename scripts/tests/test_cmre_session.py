"""Session safety: failures gate machine jobs, receipts survive, writers exclude."""
import json
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


@pytest.mark.parametrize("workflow", ["PARALLEL.md", "TASKS.md"])
def test_checkpoint_accepts_the_parallel_and_task_packet_routes(workflow):
    assert workflow in session.handoff({"receipt": "parent"}, workflow, "next", [], {})


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


def test_checkpoint_and_resume_never_invoke_validation_or_overwrite_handoff(tmp_path, monkeypatch, capsys):
    from cmre import coordination, work

    parent = preflight(tmp_path)
    monkeypatch.setattr(session, "ROOT", tmp_path)
    monkeypatch.setattr(coordination, "writer", nullcontext)
    monkeypatch.setattr(work, "read", lambda: {"functions": {}})
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
