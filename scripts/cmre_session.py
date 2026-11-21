"""Tool-neutral preflight, green gate, and supervised machine jobs.

Invoke with py -3.13. All receipts/logs are local, ignored analysis artifacts.
This runner changes no acceptance rules and never writes recovery records itself.
"""
from __future__ import annotations

import argparse
from contextlib import contextmanager
from datetime import datetime, timezone
import hashlib
import json
import os
from pathlib import Path
import re
import signal
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]


PROGRESS_KEYS = ("source_functions", "source_body_bytes", "c_tier_functions", "lifted_functions")
SPLIT_TOOLING = ('pytest', '-p', 'cmre.workspace_audit', '--cmre-workspace-audit', '-m', 'not workspace')
SPLIT_WORKSPACE = ('pytest', '-p', 'cmre.workspace_audit', '--cmre-workspace-audit', '-m', 'workspace')


def now() -> str:
    return datetime.now(timezone.utc).isoformat()


def condense(args: tuple[str, ...], lines: list[str]) -> list[str]:
    """Console view of a step; the log keeps everything. Pytest progress dots cost
    agent context on every preflight, so only failures and the summary are shown."""
    if args[0] != "pytest":
        return lines
    keep = ("FAILED", "ERROR", "passed", "failed", "error", "no tests ran")
    return [line for line in lines if line.startswith(keep[:2]) or any(k in line for k in keep[2:])
            and not line.rstrip().endswith("%]")]


def progress(before: dict, after: dict) -> dict:
    return {k: after.get(k, 0) - before.get(k, 0) for k in PROGRESS_KEYS}


def coverage() -> dict:
    from cmre import queue

    summary = queue.coverage_summary()
    return {k: summary[k] for k in PROGRESS_KEYS}


def _observe_windows_pid(pid: int) -> str:
    import _winapi

    try:
        handle = _winapi.OpenProcess(_winapi.SYNCHRONIZE, False, pid)
    except OSError as exc:
        return "absent" if exc.winerror == 87 else "unknown"
    try:
        result = _winapi.WaitForSingleObject(handle, 0)
    except OSError:
        return "unknown"
    finally:
        _winapi.CloseHandle(handle)
    return {_winapi.WAIT_OBJECT_0: "absent", _winapi.WAIT_TIMEOUT: "present"}.get(result, "unknown")


def _observe_pid(pid: int) -> str:
    if os.name == "nt":
        return _observe_windows_pid(pid)
    try:
        os.kill(pid, 0)
    except ProcessLookupError:
        return "absent"
    except OSError:
        return "unknown"
    return "present"


def receipt_status(path: Path) -> dict:
    """Bounded navigation view; raw input inventories stay in the receipt."""
    path = path.resolve()
    data = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(data, dict) or not isinstance(data.get("steps"), list):
        raise ValueError("Session receipt requires an object with steps")

    def fields(record, names):
        result = {}
        for name in names:
            value = record.get(name)
            if value is None or type(value) in (bool, int):
                result[name] = value
            elif isinstance(value, str):
                result[name] = value[:240]
            else:
                raise ValueError(f"Invalid session status field: {name}")
        return result

    def status_fields(record, names):
        result = fields(record, names)
        if result["state"] in ("starting", "running"):
            pid = result["pid"]
            result["pid_observation"] = _observe_pid(pid) if type(pid) is int and 0 < pid < 2**31 else "unknown"
        return result

    owner = status_fields(data, ("mode", "state", "pid", "exit_code", "started", "ended"))
    steps = []
    for index, step in enumerate(data["steps"][:8], 1):
        if not isinstance(step, dict):
            raise ValueError("Session receipt steps must be objects")
        row = status_fields(step, ("state", "pid", "exit_code", "started", "ended", "evidence", "detail"))
        command = step.get("command", [])
        if not isinstance(command, list) or not all(isinstance(word, str) for word in command):
            raise ValueError("Session step command must be a string list")
        row.update(index=index, command=[word[:120] for word in command[:6]])
        log = step.get("log")
        if log is not None:
            if not isinstance(log, str) or Path(log).name != log or len(log) > 120:
                raise ValueError("Session step log must be a local filename")
            row["log"] = str(path.parent / log)
        steps.append(row)
    return {"receipt": str(path), **owner,
            "step_count": len(data["steps"]), "steps": steps,
            "omitted_steps": max(0, len(data["steps"]) - len(steps)),
             "note": "State and exit code are recorded. PID observations do not prove ownership or current inputs."}


def last_start(sessions: Path, exclude: Path) -> dict | None:
    for path in sorted(sessions.glob("*-start/receipt.json"), reverse=True):
        if path.parent == exclude:
            continue
        try:
            data = json.loads(path.read_text(encoding="utf-8"))
        except (OSError, ValueError):
            continue
        if data.get("coverage") and data.get("exit_code") == 0:
            return {"receipt": str(path), **data}
    return None


def load_parent(path: Path, *, allow_failed: bool = False) -> dict:
    """A continuation inherits a completed preflight, never an unfinished job."""
    path = path.resolve()
    data = json.loads(path.read_text(encoding="utf-8"))
    required = [("cmre", "doctor"), ("cmre", "status"), ("pytest",)]
    steps = data.get("steps", [])
    commands = [tuple(s.get("command", [])[3:]) for s in steps]
    valid = (data.get("mode") == "start" and data.get("state") == "finished" and bool(steps)
             and commands == required[:len(steps)]
             and all(s.get("state") == "finished" and type(s.get("exit_code")) is int for s in steps)
             and all(s["exit_code"] == 0 for s in steps[:-1])
             and steps[-1]["exit_code"] == data.get("exit_code"))
    green = data.get("exit_code") == 0 and commands == required and bool(data.get("coverage"))
    if not valid or not (green or allow_failed and data.get("exit_code") != 0):
        raise ValueError("Parent must be a successful, finished preflight receipt with all three steps; "
                         "only the closing check may bind a failed baseline")
    return {**data, "receipt": str(path)}


def latest_gate(parent: dict, sessions: Path) -> dict | None:
    """Preserve recorded session-level failures as well as per-function holds."""
    for path in sorted(sessions.glob("*-check/receipt.json"), reverse=True):
        try:
            data = json.loads(path.read_text(encoding="utf-8"))
        except (OSError, ValueError):
            continue
        if data.get("parent", data.get("progress", {}).get("since")) != parent["receipt"]:
            continue
        failures = []
        for step in data.get("steps", []):
            if step.get("exit_code") == 0:
                continue
            failures.append(" ".join(step.get("command", [])) + f" exit={step.get('exit_code', 'unfinished')}")
            log = path.parent / Path(step.get("log", "missing.log")).name
            if log.is_file():
                failures += [line[:240] for line in log.read_text(encoding="utf-8").splitlines()
                             if line.startswith(("FAILED ", "ERROR "))]
        return {"receipt": str(path), "state": data.get("state"), "exit_code": data.get("exit_code"),
                "failures": failures[:3]}
    return None


# Retired coordination workflows (2026-11-13, replaced by FARM.md). Their names stay valid
# so older checkpoints resume, but they resolve under docs/agent/archive/.
ARCHIVED_WORKFLOWS = {"THROUGHPUT.md", "PROVIDER-CAMPAIGN.md", "PROVIDER-CONTROLLER.md",
                      "PIPELINE.md", "NATIVE-COORDINATION.md"}


def workflow_path(workflow: str) -> str:
    return ("docs/agent/archive/" if workflow in ARCHIVED_WORKFLOWS else "docs/agent/") + workflow


def validate_handoff(workflow: str, next_task: str, functions: list[str]) -> None:
    """Bound context before reading per-function records or publishing a receipt."""
    if len(functions) > 20:
        raise ValueError("A checkpoint contains at most 20 function IDs")
    if workflow not in {"C.md", "HARNESS.md", "TRIAGE.md", "SUBSYSTEM.md", "REVIEW.md",
                        "CAMPAIGN.md", "PORT.md", "BATCH.md", "REA.md", "FARM.md",
                        "PARALLEL.md", "TASKS.md"} | ARCHIVED_WORKFLOWS:
        raise ValueError("Select a workflow from docs/agent/PLAYBOOK.md")
    if not next_task.strip() or len(next_task) > 500 or any(c in next_task for c in "\r\n"):
        raise ValueError("Next task must be a nonempty single line of at most 500 characters")
    if any(not re.fullmatch(r"fn_[0-9a-f]{8}", f) for f in functions):
        raise ValueError("Function IDs must have the form fn_00401000")


def unresolved_allocation() -> dict:
    return {"schema": "cmre-worker-allocation/1", "mode": "unresolved", "authorization": "",
            "assignments": [], "reason": "No worker allocation was recorded",
            "retry_when": "Resolve current and inherited user authorization before recovery"}


def validate_allocation(data: dict) -> dict:
    """Validate navigation context; this neither grants authorization nor dispatches."""
    def line(value, limit=500):
        return (isinstance(value, str) and bool(value.strip()) and len(value) <= limit
                and not any(c in value for c in "\r\n\x00"))

    allowed = {"schema", "mode", "authorization", "assignments", "reason", "retry_when", "native_plan"}
    if (not isinstance(data, dict) or set(data) - allowed
            or data.get("schema") != "cmre-worker-allocation/1"
            or data.get("mode") not in ("providers", "solo", "unresolved", "coordination-only")):
        raise ValueError("Worker allocation requires schema cmre-worker-allocation/1 and a mode")
    mode = data["mode"]
    if mode == "coordination-only":
        from scripts.cmre_native_plan import validate_plan

        validate_plan(data.get("native_plan"), root=ROOT)
    elif "native_plan" in data:
        raise ValueError("native_plan requires coordination-only allocation")
    if (not line(data.get("authorization"))
            and not (mode == "unresolved" and data.get("authorization") == "")):
        raise ValueError("Worker allocation requires the user authorization or solo scope")
    assignments = data.get("assignments")
    if (not isinstance(assignments, list) or len(assignments) > 20
            or mode not in ("providers", "coordination-only") and assignments):
        raise ValueError("Worker allocation requires a bounded assignments list; only providers can have assignments")
    if mode not in ("providers", "coordination-only") or not assignments:
        if not all(line(data.get(k)) for k in ("reason", "retry_when")):
            raise ValueError("Worker allocation without assignments requires reason and retry_when")
    for assignment in assignments:
        if (not isinstance(assignment, dict)
                or set(assignment) - {"provider", "task", "state", "receipt", "reason", "retry_when"}
                or not line(assignment.get("provider"), 100) or not line(assignment.get("task"))
                or assignment.get("state") not in ("pending", "running", "completed", "held")):
            raise ValueError("Worker allocation assignments require provider, task and state")
        if assignment["state"] == "held" and not all(
                line(assignment.get(k)) for k in ("reason", "retry_when")):
            raise ValueError("Held worker allocation requires reason and retry_when")
        if assignment["state"] in ("running", "completed"):
            if not line(assignment.get("receipt")) or not (ROOT / assignment["receipt"]).is_file():
                raise ValueError("Running/completed worker allocation requires an existing receipt")
    for record in (data, *assignments):
        if any(not line(record[k]) for k in ("reason", "retry_when", "receipt") if k in record):
            raise ValueError("Worker allocation context must contain bounded single-line text")
    return data


def latest_checkpoint(parent: dict, sessions: Path) -> tuple[Path, dict] | None:
    for path in sorted(sessions.glob("*-checkpoint/receipt.json"), reverse=True):
        try:
            data = json.loads(path.read_text(encoding="utf-8"))
        except (OSError, ValueError):
            continue
        if (data.get("parent") == parent["receipt"] and data.get("state") == "finished"
                and data.get("exit_code", 0) == 0):
            return path, data
    return None


def checkpoint_allocation(previous: tuple[Path, dict] | None) -> dict:
    if previous is None or "worker_allocation" not in previous[1]:
        return unresolved_allocation()
    allocation = validate_allocation(previous[1]["worker_allocation"])
    path = previous[0].parent / "worker-allocation.json"
    if json.loads(path.read_text(encoding="utf-8")) != allocation:
        raise ValueError("Worker allocation snapshot differs from the checkpoint receipt; supply --allocation to replace it")
    return allocation


def allocation_summary(allocation: dict, path: Path | str | None) -> str:
    if allocation["mode"] == "unresolved":
        return ("Worker allocation: unresolved; complete docs/agent/PLAYBOOK.md#worker-allocation "
                "before recovery.")
    if path is None:
        raise ValueError("Worker allocation requires its snapshot path")
    counts = {state: sum(a["state"] == state for a in allocation["assignments"])
              for state in ("pending", "running", "completed", "held")}
    return (f"Worker allocation: {allocation['mode']}; "
            + ", ".join(f"{k}={v}" for k, v in counts.items()) + f"; read {path}")


def handoff(parent: dict, workflow: str, next_task: str, functions: list[str], states: dict,
            gate: dict | None = None, *, acceptances: dict | None = None,
            allocation: dict | None = None, allocation_path: Path | str | None = None) -> str:
    """Bounded continuation context, explicitly separate from validation evidence."""
    validate_handoff(workflow, next_task, functions)
    allocation = validate_allocation(unresolved_allocation() if allocation is None else allocation)
    lines = ["# Recovery continuation", "", f"Next: {next_task}", f"Workflow: {workflow_path(workflow)}",
             allocation_summary(allocation, allocation_path),
             f"Parent preflight: {parent['receipt']}",
             "Resume this parent receipt; retain function tests and acceptance.",
             "Checkpoint only; not a green gate. Run the parent closing check when the session ends.",
              "Read relevant evidence packets; follow every unresolved review obligation."]
    if allocation["mode"] == "coordination-only":
        lines[0] = "# Coordination-only native continuation"
        lines[3] = ("Workflow: docs/agent/archive/NATIVE-COORDINATION.md; task workflow: "
                    + workflow_path(workflow))
        lines[6:9] = [
            "Policy: docs/agent/archive/NATIVE-COORDINATION.md (all substantive work delegated).",
            "Assigned expert steward resumes preflight and executes gates; parent reads bounded receipts only.",
            "Checkpoint only; not a green gate. Preserve independent review and serialized acceptance."]
    if gate:
        lines.append(f"Latest recorded parent gate: state={gate['state']} exit={gate['exit_code']} {gate['receipt']}")
        lines.extend("Unresolved gate obligation: " + json.dumps(line) for line in gate["failures"])
        if gate["failures"]:
            lines.append("Read the complete gate receipt/logs for all failures; this summary is bounded.")
    for fn in dict.fromkeys(functions):
        state = states.get(fn, {})
        # JSON escaping keeps user/work-state prose within one bounded line.
        detail = {}
        for field in ("reason", "retry_when", "blocker", "last_report"):
            if not state.get(field):
                continue
            value = state[field]
            if len(encoded := json.dumps(value, ensure_ascii=True)) > 240:
                value = encoded[:237] + f"... [full value: cmre work show {fn}]"
            detail[field] = value
        if state.get('additional_holds'):
            detail['additional_holds'] = [
                {'state': hold.get('state'), 'blocker': hold.get('blocker')}
                for hold in state['additional_holds']]
        accepted = (acceptances or {}).get(fn, {}).get('state', 'not-inspected')
        lines.append(f"{fn} acceptance={accepted} work-history={state.get('state', 'unrecorded')} "
                     f"failures={state.get('failures', 0)} "
                     + json.dumps(detail, ensure_ascii=True))
    lines.append("Assigned workers inspect full work records and retained holds; parent consumes bounded receipts. Retry limits unchanged."
                 if allocation["mode"] == "coordination-only" else
                 "Use cmre work show fn_X for full selected records; work list for other holds. Retry limits remain unchanged.")
    return "\n".join(lines) + "\n"


def continuation(args, parent: dict, directory: Path) -> int:
    """No doctor, pytest, verification, acceptance or generated status writes."""
    from cmre import coordination, work

    previous = latest_checkpoint(parent, directory.parent)
    allocation = (unresolved_allocation() if getattr(args, "allocation", None)
                   else checkpoint_allocation(previous))
    if allocation["mode"] == "coordination-only":
        validate_native_parent(allocation, parent)
    if args.mode == "resume":
        print(f"Reuse preflight: {parent['receipt']}")
        print("No validation rerun. Checkpoints are navigation context, not green gates.")
        if previous:
            print(f"Latest checkpoint: {previous[0].parent / 'handoff.md'}")
        print(allocation_summary(allocation, previous[0].parent / "worker-allocation.json" if previous else None))
        print("Allocation is navigation context; recheck assignment receipts and pending work before recovery.")
        return 0
    validate_handoff(args.workflow, args.next_task, args.functions)
    allocation_source = {"checkpoint": str(previous[0])} if previous else None
    if args.allocation:
        source = args.allocation.resolve()
        payload = source.read_bytes()
        if len(payload) > 65536:
            raise ValueError("Worker allocation input exceeds 64 KiB")
        allocation = validate_allocation(json.loads(payload.decode("utf-8-sig")))
        validate_native_parent(allocation, parent)
        allocation_source = {"path": str(source), "sha256": hashlib.sha256(payload).hexdigest()}
    allocation_path = directory / "worker-allocation.json"
    gate = latest_gate(parent, directory.parent)
    states = work.read()['functions']
    details = {fn: work.describe(int(fn[3:], 16), states) for fn in args.functions}
    text = handoff(parent, args.workflow, args.next_task, args.functions, states, gate,
                   acceptances={fn: detail['acceptance'] for fn, detail in details.items()},
                   allocation=allocation, allocation_path=allocation_path)
    journal = Journal(directory, "checkpoint")
    journal.data.update(parent=parent["receipt"], workflow=args.workflow, functions=args.functions,
                        next_task=args.next_task, validation="not-run", last_gate=gate,
                        function_details=details, worker_allocation=allocation,
                        allocation_source=allocation_source)
    try:
        coordination.atomic_bytes(allocation_path, (json.dumps(allocation, indent=2) + "\n").encode("utf-8"))
        coordination.atomic_bytes(directory / "handoff.md", text.encode("utf-8"))
        if args.write_handoff:
            coordination.atomic_bytes(ROOT / "docs/NEXT-SESSION-PROMPT.md", text.encode("utf-8"))
    except (OSError, ValueError, coordination.BusyError) as exc:
        journal.data["error"] = f"{type(exc).__name__}: {exc}"
        journal.finish(2, "error")
        raise
    journal.finish(0)
    print(f"Checkpoint: {journal.path}; not a green gate")
    print(f"Handoff: {directory / 'handoff.md'}")
    return 0


def validate_native_parent(allocation: dict, parent: dict) -> None:
    """A native allocation cannot silently switch the session's inherited preflight."""
    if allocation["mode"] == "coordination-only":
        path = (ROOT / allocation["native_plan"]["parent_receipt"]["path"]).resolve()
        if path != Path(parent["receipt"]).resolve():
            raise ValueError("Native plan preflight differs from checkpoint parent")


class BusyError(RuntimeError):
    pass


@contextmanager
def writer_lock(path: Path, receipt: Path):
    """The OS owns the lock; an interrupted owner's metadata can remain on disk."""
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("a+b") as handle:
        if path.stat().st_size == 0:
            handle.write(b" ")
            handle.flush()
        handle.seek(0)
        try:
            if os.name == "nt":
                import msvcrt

                msvcrt.locking(handle.fileno(), msvcrt.LK_NBLCK, 1)
            else:
                import fcntl

                fcntl.flock(handle, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except OSError as exc:
            # Reading a locked byte is refused on Windows; metadata starts at byte 1.
            handle.seek(1)
            owner = handle.read().decode("utf-8", errors="replace")
            raise BusyError(f"Session runner already active: {owner}. Check that receipt before retrying.") from exc
        try:
            handle.seek(1)
            handle.truncate()
            handle.write(json.dumps({"pid": os.getpid(), "receipt": str(receipt)}).encode("utf-8"))
            handle.flush()
            yield
        finally:
            handle.seek(0)
            if os.name == "nt":
                msvcrt.locking(handle.fileno(), msvcrt.LK_UNLCK, 1)
            else:
                fcntl.flock(handle, fcntl.LOCK_UN)


def stop_tree(process: subprocess.Popen) -> None:
    """Cancel only this runner's child tree, including machine-job pool workers."""
    if process.poll() is not None:
        return
    if os.name == "nt":
        subprocess.run(["taskkill", "/PID", str(process.pid), "/T", "/F"],
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=False)
    else:
        os.killpg(process.pid, signal.SIGTERM)
    try:
        process.wait(timeout=5)
    except subprocess.TimeoutExpired:
        if os.name == "nt":
            process.kill()
        else:
            os.killpg(process.pid, signal.SIGKILL)
        process.wait()


class Journal:
    def __init__(self, directory: Path, mode: str):
        directory.mkdir(parents=True)
        self.path = directory / "receipt.json"
        self.data = {"mode": mode, "pid": os.getpid(), "started": now(),
                     "state": "running", "steps": []}
        self.save()

    def save(self) -> None:
        temporary = self.path.with_suffix(".tmp")
        temporary.write_text(json.dumps(self.data, indent=2) + "\n", encoding="utf-8", newline="\n")
        temporary.replace(self.path)

    def execute(self, args: tuple[str, ...]) -> int:
        from cmre import env, session_evidence

        argv = [sys.executable, "-m", *args]
        step = {"command": ["py", "-3.13", "-m", *args], "started": now(),
                "log": f"{len(self.data['steps']) + 1:02d}-{args[0]}.log", "state": "starting"}
        self.data["steps"].append(step)
        split_scope = {SPLIT_TOOLING: 'tooling', SPLIT_WORKSPACE: 'workspace'}.get(args)
        if args == ('pytest',) or split_scope:
            step['inputs_before'] = (session_evidence.identity(ROOT, scope=split_scope) if split_scope
                                     else session_evidence.identity(ROOT))
        if args == ('pytest',):
            previous = session_evidence.find(self.path.parent.parent, step['inputs_before'], self.path)
            if previous:
                step.update(state='finished', exit_code=0, ended=now(), evidence='reused', reused_from=previous)
                self.save()
                print(f"Reused full pytest: {previous['receipt']} step {previous['step']}", flush=True)
                return 0
        self.save()
        print("Running: " + " ".join(step["command"]), flush=True)
        child_env = dict(os.environ, PYTHONIOENCODING="utf-8", PYTHONUNBUFFERED="1", NO_COLOR="1")
        quiet = args[0] == "pytest"
        captured = []
        with (self.path.parent / step["log"]).open("w", encoding="utf-8", newline="\n") as log:
            process = subprocess.Popen(argv, cwd=ROOT, env=child_env, stdout=subprocess.PIPE,
                                       stderr=subprocess.STDOUT, text=True, encoding="utf-8", errors="replace",
                                       start_new_session=os.name != "nt")
            try:
                step.update(pid=process.pid, state="running")
                self.save()
                for line in process.stdout:
                    log.write(line)
                    log.flush()
                    if quiet:
                        captured.append(line)
                    else:
                        print(line, end="", flush=True)
                code = process.wait()
                for line in condense(args, captured):
                    print(line, end="", flush=True)
            except BaseException:
                stop_tree(process)
                step.update(state="interrupted", ended=now(), exit_code=process.returncode)
                self.save()
                raise
            finally:
                process.stdout.close()
        step.update(state="finished", ended=now(), exit_code=code)
        if args == ('pytest',) or split_scope:
            after = (session_evidence.identity(ROOT, scope=split_scope) if split_scope
                     else session_evidence.identity(ROOT))
            step.update(inputs_after=after, evidence='executed',
                        log_sha256=env.digest(self.path.parent / step['log']))
            if step['inputs_after'] != step['inputs_before']:
                step.update(exit_code=2, detail='Declared test inputs changed during pytest; evidence is inconclusive')
                code = 2
        self.save()
        print(f"Exit: {code}", flush=True)
        return code

    def finish(self, code: int, state: str = "finished") -> None:
        self.data.update(state=state, ended=now(), exit_code=code)
        self.save()


def drive(mode: str, command: list[str], execute) -> int:
    if mode == 'split-audit':
        tooling = execute(SPLIT_TOOLING)
        workspace = execute(SPLIT_WORKSPACE)
        return tooling or workspace
    if mode == "check":
        tests = execute(("pytest",))
        verify = execute(("cmre", "verify"))
        return tests or verify or execute(("cmre", "status", "--write"))
    for args in (("cmre", "doctor"), ("cmre", "status"), ("pytest",)):
        code = execute(args)
        if code:
            return code
    return execute(("cmre", *command)) if mode == "run" else 0


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    modes = parser.add_subparsers(dest="mode", required=True)
    modes.add_parser("start", help="one full parent preflight")
    status = modes.add_parser("status", help="bounded read-only receipt summary; no tests or lease")
    status.add_argument("receipt", type=Path)
    modes.add_parser('split-audit', help='experimental split audit; does not replace or seed a full gate')
    check = modes.add_parser("check", help="full parent closing gate")
    check.add_argument("--parent", type=Path, help="bind progress to this preflight, not another session's")
    run = modes.add_parser("run", help="supervised job with doctor/status and current full pytest evidence")
    run.add_argument("command", nargs=argparse.REMAINDER)
    resume = modes.add_parser("resume", help="reuse a completed preflight; no tests")
    checkpoint = modes.add_parser("checkpoint", help="prepare bounded continuation context; no closing gate")
    for command in (resume, checkpoint):
        command.add_argument("--parent", type=Path, required=True)
    checkpoint.add_argument("--workflow", default="C.md")
    checkpoint.add_argument("--next", dest="next_task", required=True)
    checkpoint.add_argument("--functions", nargs="*", default=[])
    checkpoint.add_argument("--write-handoff", action="store_true", help="also replace docs/NEXT-SESSION-PROMPT.md")
    checkpoint.add_argument("--allocation", type=Path,
                            help="worker-allocation JSON; omitted inherits this parent's latest checkpoint")
    args = parser.parse_args(argv)
    if args.mode == "run" and (not args.command or args.command[0] not in ("batch", "revalidate")):
        parser.error("run requires batch or revalidate; consult py -3.13 -m cmre <command> --help")
    if sys.version_info[:2] != (3, 13):
        parser.error("invoke this script with py -3.13")
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(encoding="utf-8")
    if args.mode == "status":
        try:
            print(json.dumps(receipt_status(args.receipt), indent=2))
            return 0
        except (OSError, ValueError) as exc:
            print(str(exc), file=sys.stderr)
            return 2
    sessions = ROOT / "CM0102/Analysis/cmre/sessions"
    directory = sessions / (datetime.now(timezone.utc).strftime("%Y%m%dT%H%M%S%fZ") + f"-{os.getpid()}-{args.mode}")
    # Scripts run with scripts/ on sys.path. Use the same mutation lease as
    # direct cmre commands, including throughout preflight and final checks.
    if str(ROOT) not in sys.path:
        sys.path.insert(0, str(ROOT))
    from cmre.coordination import writer, BusyError as CommandBusyError
    try:
        parent = (load_parent(args.parent, allow_failed=args.mode == "check")
                  if getattr(args, "parent", None) else None)
        if args.mode == "resume":
            return continuation(args, parent, directory)
        with writer_lock(sessions / "writer.lock", directory / "receipt.json"), writer():
            if args.mode == "checkpoint":
                return continuation(args, parent, directory)
            if args.mode == "check" and parent is None:
                parent = last_start(sessions, directory)
            journal = Journal(directory, args.mode)
            if parent:
                journal.data["parent"] = parent["receipt"]
                journal.data["baseline_exit_code"] = parent["exit_code"]
                journal.save()
            print(f"Receipt: {journal.path}", flush=True)
            try:
                code = drive(args.mode, getattr(args, "command", []), journal.execute)
                if code == 0 and args.mode in ("start", "check"):
                    journal.data["coverage"] = coverage()
                    if (args.mode == "check" and (start := parent or last_start(sessions, directory))
                            and start.get("coverage")):
                        delta = progress(start["coverage"], journal.data["coverage"])
                        journal.data["progress"] = {"since": start["receipt"], "started": start["started"], **delta}
                        print("Progress since " + start["started"][:16] + ": "
                              + ", ".join(f"{k} {v:+d}" for k, v in delta.items()), flush=True)
                    journal.save()
            except KeyboardInterrupt:
                journal.finish(130, "interrupted")
                return 130
            except Exception as exc:
                journal.data["error"] = f"{type(exc).__name__}: {exc}"
                journal.finish(2, "error")
                print(journal.data["error"], file=sys.stderr)
                return 2
            journal.finish(code)
            return code
    except (BusyError, CommandBusyError) as exc:
        print(str(exc), file=sys.stderr)
        return 2
    except (OSError, ValueError) as exc:
        print(str(exc), file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
