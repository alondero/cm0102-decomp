"""Exit-transition regression at the real multi-thread breakpoint/step seam."""
import json
import uuid
import ctypes
import time
from types import SimpleNamespace

import pytest

from native_debug_fixture import ROOT, Unavailable, build_fixture
from native_import_observer import observe_calls, _exit_transition_candidate
from cmre import capture_record as api
import native_import_observer as backend


@pytest.fixture(scope="module")
def exit_race_fixture():
    directory = ROOT / "local/native-peer-exit" / uuid.uuid4().hex
    try:
        return build_fixture(directory, external_breakpoints=True)
    except Unavailable as exc:
        pytest.skip(str(exc))


def _worker_exit_events(protocol):
    """Exclude reused non-worker TIDs: they once made 24 worker exits count as 25."""
    worker_identities = {(record["tid"], record["thread_generation"])
                         for record in protocol["invocations"]}
    return [event for event in protocol["events"]
            if event["kind"] == "thread-exit"
            and (event["tid"], event["generation"]) in worker_identities]


def test_worker_exit_selection_distinguishes_reused_thread_generations():
    # Selection-only synthetic sample, not a complete protocol witness.
    # Multiple invocations can belong to the same worker generation.
    protocol = {
        "invocations": [
            {"tid": 41, "thread_generation": 2},
            {"tid": 41, "thread_generation": 2},
            {"tid": 41, "thread_generation": 3},
            {"tid": 42, "thread_generation": 1},
        ],
        "events": [
            {"kind": "thread-exit", "tid": 41, "generation": 1, "exit_code": 0},
            {"kind": "thread-exit", "tid": 41, "generation": 2, "exit_code": 259},
            {"kind": "thread-exit", "tid": 43, "generation": 1, "exit_code": 259},
            {"kind": "thread-start", "tid": 41, "generation": 3},
            {"kind": "thread-exit", "tid": 41, "generation": 3, "exit_code": 0},
            {"kind": "thread-exit", "tid": 42, "generation": 1, "exit_code": 259},
        ],
    }
    # Exclude an older non-worker generation, not a worker's wrong exit code:
    # the live test must still catch that zero exit rather than hide it.
    assert _worker_exit_events(protocol) == [
        protocol["events"][1], protocol["events"][4], protocol["events"][5]]


def test_native_multiwave_peer_exit_transition(exit_race_fixture):
    # 12 waves x 2 threads: many exit/peer-trap interleavings in one owned child.
    for _ in range(12):
        result = observe_calls(exit_race_fixture, "exit-race", seconds=12)
        records = result["protocol"]["invocations"]
        assert len(records) == 48 and all("return" in record for record in records)
        assert result["child_returncode"] == 0
        assert json.loads(result["stdout"]) == {"cleanup_ok": True}
        assert result["protocol"]["cleanup_complete"]
        assert result["cleanup"]["all_debugger_bytes_restored"]
        assert all(step["trap_flag_after"] == 0 for step in result["steps"])
        exits = _worker_exit_events(result["protocol"])
        assert len(exits) == 24 and all(event["exit_code"] == 259 for event in exits)
        for proof in result.get("peer_exit_deferrals", []):
            confirmed = proof["confirmed_by_exit_event"]
            assert (confirmed["tid"], confirmed["generation"], confirmed["handle"]) == (
                proof["tid"], proof["generation"], proof["handle"])
            failure = proof["suspend_failure"]
            assert failure["ntstatus"] == 0xc000004b and failure["granted_access"] & 2
            assert failure["actual_tid"] == proof["tid"]
            assert failure["actual_pid"] == failure["expected_pid"]
        if result.get("peer_exit_deferrals"):
            assert result["reply_later"] and result["reply_replays"]
            assert all(read["complete"] and read["data_hex"] == "cc"
                       for hold in result["reply_later"] for read in hold["armed_readback"])
        assert all(release["observed_count"] == release["restored_count"] + 1
                   for release in result["cleanup"]["suspension_releases"])


def test_only_identified_native_termination_permits_wait_not_death():
    # Minimal retained native failure facts; this policy merely permits waiting
    # for a real EXIT event, never permits proceeding past an unknown peer.
    fact = dict(error=5, ntstatus=0xc000004b, tid=7, actual_tid=7, generation=1,
                expected_pid=9, actual_pid=9, object_query_status=0, granted_access=2,
                queried_exit=True, exit_code=259, wait_result=258)
    assert _exit_transition_candidate(fact)
    for key, value in (("ntstatus", 0xc0000022), ("actual_tid", 8),
                       ("actual_pid", 10), ("granted_access", 0),
                       ("queried_exit", False), ("wait_result", 0xffffffff), ("generation", None)):
        assert not _exit_transition_candidate({**fact, key: value})


def test_real_access_denied_live_peer_fails_closed(exit_race_fixture, monkeypatch):
    """REAL wrong-rights duplicate: same Win32 error 5, no invented API result."""
    k = api._kernel32()
    duplicate = k.DuplicateHandle
    duplicate.argtypes = (
        ctypes.wintypes.HANDLE, ctypes.wintypes.HANDLE, ctypes.wintypes.HANDLE,
        ctypes.POINTER(ctypes.wintypes.HANDLE), ctypes.wintypes.DWORD,
        ctypes.wintypes.BOOL, ctypes.wintypes.DWORD)
    k.GetThreadId.argtypes = (ctypes.wintypes.HANDLE,)
    k.GetThreadId.restype = ctypes.wintypes.DWORD
    thread_duplicates = []

    def limited_duplicate(source_process, source, dest_process, dest, access, inherit, options):
        tid = k.GetThreadId(source)
        if tid:
            thread_duplicates.append(tid)
            if len(thread_duplicates) == 3:  # main, attach, then first worker
                access, options = 0x100858, 0  # query/context/synchronize, NOT suspend/resume
        return duplicate(source_process, source, dest_process, dest, access, inherit, options)

    k.DuplicateHandle = limited_duplicate
    monkeypatch.setattr(api, "_kernel32", lambda: k)
    report = {}
    with pytest.raises(RuntimeError, match="unknown peer suspend failure"):
        observe_calls(exit_race_fixture, "threads", report=report)
    failures = report["suspend_failures"]
    assert any(f["error"] == 5 and f["ntstatus"] == 0xc0000022
               and not f["granted_access"] & 2 for f in failures)
    assert not report.get("peer_exit_deferrals")
    assert report["protocol"]["process_exited"]
    assert not report["cleanup"].get("detached")
    assert not report["cleanup"].get("all_debugger_bytes_restored")
    terminated = report["cleanup"]["terminated_owned_child"]
    assert terminated["pid"] == report["protocol"]["source"]["pid"]
    assert terminated["returncode"] != 0


def test_unconfirmed_exit_transport_times_out_without_stepping_or_detach(
        exit_race_fixture, monkeypatch):
    """Withhold a REAL exit event to test the recovery deadline, not import data."""
    k = api._kernel32()
    wait = k.WaitForDebugEvent
    report, held, history = {}, {}, []

    def delayed_exit(event_pointer, timeout):
        if held:
            if report.get("cleanup", {}).get("unavailable"):
                ctypes.memmove(event_pointer, held.pop("raw"), ctypes.sizeof(api._DebugEvent))
                return 1  # deliver the exact withheld native event for kill/drain
            ctypes.set_last_error(121)
            time.sleep(0.01)
            return 0
        ok = wait(event_pointer, timeout)
        if ok:
            event = ctypes.cast(event_pointer, ctypes.POINTER(api._DebugEvent)).contents
            candidates = {(p["tid"], p["generation"]) for p in report.get("peer_exit_deferrals", [])}
            if event.dwDebugEventCode == api.EXIT_THREAD_DEBUG_EVENT and any(
                    tid == event.dwThreadId for tid, _ in candidates):
                held["raw"] = bytes(event)
                history.append(dict(tid=event.dwThreadId, actual_native_event=True,
                                    step_count_at_hold=len(report["steps"])))
                report["withheld_native_exit_test_control"] = history
                ctypes.set_last_error(121)
                return 0
        return ok

    # The underlying real wait keeps its proper pointer-width declaration.
    wait.argtypes = (ctypes.POINTER(api._DebugEvent), ctypes.wintypes.DWORD)
    k.WaitForDebugEvent = delayed_exit
    monkeypatch.setattr(api, "_kernel32", lambda: k)
    for _ in range(12):
        report = {}
        try:
            observe_calls(exit_race_fixture, "exit-race", report=report, seconds=4)
        except RuntimeError as exc:
            assert history
            assert "terminating peer exit event not confirmed within 1s" in str(exc)
            assert not report["cleanup"].get("detached")
            assert report["cleanup"]["terminated_owned_child"]["returncode"] != 0
            assert all(not p.get("confirmed_by_exit_event") for p in report["peer_exit_deferrals"])
            assert len(report["steps"]) == history[-1]["step_count_at_hold"]
            return
    pytest.skip("native scheduling produced no terminating-peer transition for deadline injection")


@pytest.mark.parametrize("boundary", ["wall-late", "before", "at", "after"])
def test_received_native_exit_checked_before_dispatch(
        exit_race_fixture, monkeypatch, boundary):
    """Delay/clock-stamp a REAL matching exit, never fabricate native proof."""
    k = api._kernel32()
    wait, native_continue = k.WaitForDebugEvent, k.ContinueDebugEvent
    wait.argtypes = (ctypes.POINTER(api._DebugEvent), ctypes.wintypes.DWORD)
    report, control, deadlines, clock_origins = {}, {}, {}, {}
    real_monotonic = time.monotonic
    injection = {}

    def clock():
        now = real_monotonic()
        if injection:
            if injection.pop("first", False):
                return injection["receipt"]
            return injection["receipt"] + now - injection["real_start"]
        # The backend creates its deadline immediately after publishing the
        # candidate. Capture that clock read; do not infer an exit from time.
        active = [p for p in report.get("peer_exit_deferrals", [])
                  if not p.get("confirmed_by_exit_event")]
        for proof in active:
            key = proof["tid"], proof["generation"]
            if key not in deadlines:
                deadlines[key] = now + 1
                clock_origins[key] = now
        if active:
            anchor = clock_origins[active[-1]["tid"], active[-1]["generation"]]
            elapsed = now - anchor
            # Keep pre-receipt time below every boundary injection without ever
            # moving backwards. If native delivery stalls, expire after 3 real
            # seconds instead of freezing this owned-child controller forever.
            return anchor + (min(elapsed, 0.25) if elapsed < 3 else elapsed)
        return now

    if boundary != "wall-late":
        # Do NOT mutate the shared time module or native event bytes.
        monkeypatch.setattr(backend, "time", SimpleNamespace(monotonic=clock))

    popen = backend.subprocess.Popen

    class OwnedChild(popen):
        def kill(self):
            control["kill_started"] = True
            return super().kill()

    monkeypatch.setattr(backend.subprocess, "Popen", OwnedChild)

    def continued(pid, tid, status):
        if control.get("delivered") and (pid, tid) == control["event_identity"]:
            control.setdefault("matching_continues", []).append(dict(
                status=status, kill_started=bool(control.get("kill_started")),
                cleanup_unavailable=report.get("cleanup", {}).get("unavailable")))
        return native_continue(pid, tid, status)

    def delayed(event_pointer, timeout):
        ok = wait(event_pointer, timeout)
        if ok and not control.get("delivered"):
            event = ctypes.cast(event_pointer, ctypes.POINTER(api._DebugEvent)).contents
            candidates = {(p["tid"], p["generation"]) for p in report.get("peer_exit_deferrals", [])}
            key = next((key for key in candidates if key[0] == event.dwThreadId), None)
            if event.dwDebugEventCode == api.EXIT_THREAD_DEBUG_EVENT and key is not None:
                control.update(event_identity=(event.dwProcessId, event.dwThreadId),
                               generation=key[1], native_event_hex=bytes(event).hex(),
                               steps_before=len(report["steps"]),
                               replays_before=len(report.get("reply_replays", [])))
                if boundary == "wall-late":
                    time.sleep(1.2)
                else:
                    offset = {"before": -0.25, "at": 0, "after": 0.25}[boundary]
                    injection.update(receipt=deadlines[key] + offset,
                                     real_start=real_monotonic(), first=True)
                    control.update(deadline=deadlines[key], receipt=injection["receipt"])
                control["delivered"] = True
                report["late_native_exit_test_control"] = control
        return ok  # original TRUE and original native event bytes

    k.WaitForDebugEvent, k.ContinueDebugEvent = delayed, continued
    monkeypatch.setattr(api, "_kernel32", lambda: k)
    for _ in range(12):
        report, control = {}, {}
        deadlines.clear()
        clock_origins.clear()
        injection.clear()
        try:
            observe_calls(exit_race_fixture, "exit-race", report=report, seconds=6)
        except RuntimeError as exc:
            assert boundary != "before"
            assert control.get("delivered")
            assert "terminating peer exit event not confirmed within 1s" in str(exc)
            assert not report["cleanup"].get("detached")
            terminated = report["cleanup"]["terminated_owned_child"]
            assert terminated["pid"] == report["protocol"]["source"]["pid"]
            assert terminated["returncode"] != 0 and report["protocol"]["process_exited"]
            received = report["received_after_peer_exit_deadline"]
            assert (received["pid"], received["tid"]) == control["event_identity"]
            assert received["generation"] == control["generation"]
            assert received["code"] == api.EXIT_THREAD_DEBUG_EVENT
            assert received["raw_event_hex"] == control["native_event_hex"]
            assert received["received_at"] >= received["deadline"]
            if boundary == "at":
                assert received["received_at"] == received["deadline"]
            assert not any(e["kind"] == "thread-exit" and
                           (e["tid"], e["generation"]) == (received["tid"], received["generation"])
                           for e in report["protocol"]["events"])
            assert all(not p.get("confirmed_by_exit_event")
                       for p in report["peer_exit_deferrals"]
                       if (p["tid"], p["generation"]) == (
                           control["event_identity"][1], control["generation"]))
            assert len(report["steps"]) == control["steps_before"]
            assert len(report.get("reply_replays", [])) == control["replays_before"]
            assert control["matching_continues"]
            assert all(c["kill_started"] and c["cleanup_unavailable"]
                       for c in control["matching_continues"])
            return
        if control.get("delivered"):
            assert boundary == "before", "LATE_EXIT_ACCEPTED"
            assert report["child_returncode"] == 0 and report["cleanup"]["detached"]
            assert control["receipt"] < control["deadline"]
            assert "received_after_peer_exit_deadline" not in report
            assert any(p.get("confirmed_by_exit_event") and
                       (p["tid"], p["generation"]) == (
                           control["event_identity"][1], control["generation"])
                       for p in report["peer_exit_deferrals"])
            assert report["reply_replays"]
            return
    pytest.skip("native scheduling produced no matching exit for receipt-boundary injection")
