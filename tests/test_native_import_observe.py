"""Live authored child, actual caller continuations; never attach to a game."""
import json
import uuid

import pytest

from native_debug_fixture import ROOT, Unavailable, build_fixture
from native_import_observer import observe_calls


@pytest.fixture(scope="module")
def external_fixture():
    directory = ROOT / "local/native-import-observe" / uuid.uuid4().hex
    try:
        return build_fixture(directory, external_breakpoints=True)
    except Unavailable as exc:
        pytest.skip(str(exc))


def test_actual_caller_continuation_is_paired_and_breakpoints_are_stepped(external_fixture):
    result = observe_calls(external_fixture, "ordinary")
    records = result["protocol"]["invocations"]
    assert len(records) == 1
    assert records[0]["state"] == "paired-endpoints"
    assert records[0]["return"]["registers"]["eax"] == 117
    assert records[0]["return"]["registers"]["edx"] == 0x2468ace0
    assert records[0]["return_site"] != result["manifest"]["return"]
    assert records[0]["entry"]["registers"]["debugger_raw_eip"] == result["manifest"]["entry"] + 1
    assert records[0]["return"]["registers"]["debugger_raw_eip"] == records[0]["return_site"] + 1
    assert all(event["wow64_context_hex"] and event["context_flags"]
               for event in result["debug_events"] if event["code"] == 1)
    assert all(type(event["generation"]) is int and event["generation"] > 0
               for event in result["debug_events"])
    assert not any(event["kind"] == "rejected-thread-event" for event in result["protocol"]["events"])
    assert [step["address"] for step in result["steps"]] == [
        result["manifest"]["entry"], records[0]["return_site"],
    ]
    assert all(step["trap_flag_after"] == 0 for step in result["steps"])
    assert result["cleanup"]["all_debugger_bytes_restored"] is True
    assert result["protocol"]["cleanup_complete"] is True
    assert result["child_returncode"] == 0
    assert json.loads(result["stdout"]) == {"cleanup_ok": True}


@pytest.mark.parametrize("scenario,count", [
    ("nested", 3), ("shared", 3), ("threads", 4), ("unreadable", 1),
])
def test_native_recursive_shared_threaded_and_unreadable_calls(external_fixture, scenario, count):
    result = observe_calls(external_fixture, scenario)
    records = result["protocol"]["invocations"]
    assert len(records) == count
    unpaired = [event for event in result["protocol"]["events"] if event["kind"] == "unmatched-return"]
    assert len(result["steps"]) == count * 2 + len(unpaired)
    assert result["protocol"]["cleanup_complete"] is True
    assert result["cleanup"]["all_debugger_bytes_restored"] is True
    assert result["child_returncode"] == 0
    assert json.loads(result["stdout"]) == {"cleanup_ok": True}
    for record in records:
        events = {event["sequence"]: event for event in result["protocol"]["events"]}
        assert events[record["entry_sequence"]]["generation"] == record["thread_generation"]
        assert events[record["return_sequence"]]["generation"] == record["thread_generation"]
        fact = result["fixture_facts"][record["invocation"]]
        assert record["return_site"] != result["manifest"]["return"]
        assert record["return"]["registers"]["eax"] == fact["token"] + fact["depth"] + 17
        assert record["return"]["registers"]["edx"] == 0x2468ace0
        output = record["return"]["memory"][0]
        if scenario == "unreadable":
            assert record["state"] == "returned-incomplete"
            assert output["complete"] is False and output["error"]
        else:
            assert record["state"] in ("paired-endpoints", "returned-incomplete")
            if record["state"] == "returned-incomplete":
                assert record["gaps"] == ["unmatched-return"] or set(record["gaps"]) == {"unmatched-return"}
            assert int.from_bytes(bytes.fromhex(output["data_hex"]), "little") == fact["token"] ^ 0x13579bdf
        assert record["replayable"] is False
    if scenario in ("nested", "threads"):
        assert any(record["parent"] is not None for record in records)
    if scenario == "shared":
        assert len({record["return_site"] for record in records}) == 1
        entry_steps = [s for s in result["steps"] if s["address"] == result["manifest"]["entry"]]
        assert len(entry_steps) == 3 and all(s["rearmed"] for s in entry_steps)
    if scenario == "threads":
        assert len({record["tid"] for record in records}) == 2
        outers = [record for record in records if record["parent"] is None]
        assert len(outers) == 2 and outers[0]["return_site"] == outers[1]["return_site"]
        assert max(record["entry_sequence"] for record in outers) < min(
            record["return_sequence"] for record in outers)
        assert any(step["peers_suspended"] for step in result["steps"])
        releases = result["cleanup"]["suspension_releases"]
        assert releases and all(r["observed_count"] == r["restored_count"] + 1 for r in releases)
        # These observations remain scheduling-dependent, not required witnesses.
        for deferred in result.get("deferred_peer_traps", []):
            assert deferred["generation"] > 0 and deferred["raw_context_hex"]
            assert deferred["rewound_ip"] == deferred["address"]


@pytest.mark.parametrize("scenario,ending", [("thread-exit", "thread-exit"), ("fault", "process-exit")])
def test_native_nonreturning_calls_stay_incomplete(external_fixture, scenario, ending):
    result = observe_calls(external_fixture, scenario)
    records = result["protocol"]["invocations"]
    assert len(records) == 1
    assert records[0]["state"] == "incomplete"
    assert ending in records[0]["gaps"]
    assert "return" not in records[0]
    if scenario == "fault":
        faults = [event for event in result["protocol"]["events"] if event["kind"] == "exception"]
        assert [(event["code"], event["first_chance"]) for event in faults] == [
            (0xc0000005, True), (0xc0000005, False),
        ]
        assert result["protocol"]["process_exited"] is True
        assert result["protocol"]["cleanup_complete"] is False
        assert result["child_returncode"] != 0
    else:
        assert result["child_returncode"] == 0
        assert result["protocol"]["cleanup_complete"] is True


@pytest.mark.parametrize("scenario,point", [
    ("ordinary", "entry-hit"), ("ordinary", "step-prepared"), ("ordinary", "step-stop"),
    ("threads", "step-prepared"), ("threads", "step-stop"),
])
def test_native_detach_with_call_in_flight_restores_bytes_ip_tf_and_peer_suspensions(
        external_fixture, scenario, point):
    result = observe_calls(external_fixture, scenario, detach_at=point)
    assert result["child_returncode"] == 0
    assert json.loads(result["stdout"]) == {"cleanup_ok": True}
    assert result["protocol"]["cleanup_complete"] is True
    assert result["cleanup"]["all_debugger_bytes_restored"] is True
    assert result["cleanup"]["detached"] is True
    assert result["fixture_controls"] == [dict(
        address=result["manifest"]["done"], before_hex="cc", after_hex="90",
        purpose="allow authored child to complete after in-flight detach")]
    records = result["protocol"]["invocations"]
    assert records and all(record["state"] == "incomplete" and "return" not in record for record in records)
    if point != "entry-hit":
        step = result["cleanup"]["interrupted_step"]
        assert step["executed"] is (point == "step-stop")
        assert step["trap_flag_after"] == 0
        assert step["ip"] == result["manifest"]["entry"] + (1 if point == "step-stop" else 0)
    if scenario == "threads":
        assert result["cleanup"]["suspension_releases"]


@pytest.mark.parametrize("point", ["step-prepared", "step-stop"])
def test_native_host_exception_in_step_still_detaches_and_completes_child(external_fixture, point):
    result = {}
    with pytest.raises(RuntimeError, match="injected failure"):
        observe_calls(external_fixture, "ordinary", fail_at=point, report=result)
    assert result["child_returncode"] == 0
    assert json.loads(result["stdout"]) == {"cleanup_ok": True}
    assert result["cleanup"]["all_debugger_bytes_restored"] is True
    assert result["cleanup"]["interrupted_step"]["trap_flag_after"] == 0
    assert result["protocol"]["cleanup_complete"] is True
    assert result["protocol"]["invocations"][0]["state"] == "incomplete"
