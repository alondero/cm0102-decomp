"""Synthetic event streams only: these are not live import recordings."""
import json

import pytest

from cmre.import_observe import ImportObserver, MemoryRead, Snapshot


REGS = dict(eax=1, ebx=2, ecx=3, edx=4, esi=5, edi=6, ebp=0x9000,
            eip=0x70001000, esp=0x8000, eflags=0x202)


def snap(*, ip=0x70001000, sp=0x8000, site=0x401020, reads=(), gaps=()):
    return Snapshot({**REGS, "eip": ip, "esp": sp},
                    (MemoryRead(sp, 4, site.to_bytes(4, "little"), "return-word"), *reads),
                    gaps)


def observer():
    result = ImportObserver(enabled=True, source={"kind": "synthetic-test", "session": "unit"})
    result.thread_start(10)
    return result


def enter(obs, *, tid=10, generation=1, site=0x401020, sp=0x8000, after_sp=0x8004):
    return obs.entry(tid, "capture-1", "fixture!raw", snap(sp=sp, site=site),
                     generation=generation, return_site=site, return_sp=after_sp,
                     return_sp_basis="Authored test instruction C3; not an API assumption")


def ack_all(obs):
    for request in obs.breakpoint_requests():
        obs.acknowledge_breakpoint(request["address"], request["revision"], success=True)


def test_opt_in_endpoint_pair_retains_raw_evidence_and_requests_cleanup():
    obs = observer()
    invocation = enter(obs)
    assert obs.breakpoint_requests()[0]["operation"] == "arm"
    ack_all(obs)
    raw = MemoryRead(0x6000, 4, b"\xaa\xbb", "explicit-output-range", "partial read")
    assert obs.return_hit(10, 0x401020, snap(ip=0x401020, sp=0x8004, reads=(raw,)), generation=1) == invocation
    report = json.loads(json.dumps(obs.report()))
    record = report["invocations"][0]
    assert record["state"] == "returned-incomplete"
    assert record["return"]["registers"]["eax"] == 1
    assert record["return"]["memory"][1] == {
        "address": 0x6000, "requested_size": 4, "data_hex": "aabb",
        "purpose": "explicit-output-range", "error": "partial read", "complete": False,
    }
    assert record["replayable"] is False
    assert obs.breakpoint_requests()[0]["operation"] == "remove"
    obs.detach("test finished")
    assert obs.report()["cleanup_complete"] is False
    ack_all(obs)
    assert obs.report()["cleanup_complete"] is True


def test_recursion_and_threads_share_one_return_breakpoint_without_early_removal():
    obs = observer()
    obs.thread_start(20)
    outer = enter(obs)
    inner = enter(obs, sp=0x7f00, after_sp=0x7f04)
    other = enter(obs, tid=20)
    assert len(obs.breakpoint_requests()) == 1
    ack_all(obs)
    assert obs.return_hit(10, 0x401020, snap(ip=0x401020, sp=0x7f04), generation=1) == inner
    assert obs.breakpoint_requests() == []
    assert obs.return_hit(20, 0x401020, snap(ip=0x401020, sp=0x8004), generation=1) == other
    assert obs.breakpoint_requests() == []
    assert obs.return_hit(10, 0x401020, snap(ip=0x401020, sp=0x8004), generation=1) == outer
    assert obs.breakpoint_requests()[0]["operation"] == "remove"
    assert [r["parent"] for r in obs.report()["invocations"]] == [None, outer, None]
    assert {r["state"] for r in obs.report()["invocations"]} == {"paired-endpoints"}


@pytest.mark.parametrize("case, reason", [
    ("outer-first", "non-lifo-return"),
    ("same-frame", "ambiguous-return"),
    ("wrong-sp", "unmatched-return"),
    ("wrong-site", "unmatched-return"),
    ("wrong-ip", "endpoint-ip-mismatch"),
    ("unarmed", "unarmed-return"),
])
def test_near_misses_never_consume_an_invocation(case, reason):
    obs = observer()
    enter(obs)
    if case == "outer-first":
        enter(obs, sp=0x7f00, after_sp=0x7f04)
    if case == "same-frame":
        enter(obs)
    if case != "unarmed":
        ack_all(obs)
    address = 0x401021 if case == "wrong-site" else 0x401020
    ip = 0x401022 if case == "wrong-ip" else address
    sp = 0x8008 if case == "wrong-sp" else 0x8004
    assert obs.return_hit(10, address, snap(ip=ip, sp=sp), generation=1) is None
    assert {r["state"] for r in obs.report()["invocations"]} == {"pending"}
    assert obs.report()["events"][-1]["kind"] == reason
    obs.detach("near miss")
    assert {r["state"] for r in obs.report()["invocations"]} == {"incomplete"}


def test_return_on_unregistered_thread_does_not_poison_other_threads():
    obs = observer()
    enter(obs)
    ack_all(obs)
    assert obs.return_hit(77, 0x401020, snap(ip=0x401020, sp=0x8004), generation=1) is None
    assert obs.report()["invocations"][0]["gaps"] == []


def test_unknown_cleanup_size_is_not_inferred_from_import_name():
    obs = observer()
    obs.entry(10, "capture-1", "KERNEL32.dll!ReadFile", snap(),
              generation=1, return_site=0x401020)
    ack_all(obs)
    assert obs.return_hit(10, 0x401020, snap(ip=0x401020, sp=0x8018), generation=1) is None
    assert "unknown-return-sp" in obs.report()["invocations"][0]["gaps"]


def test_exception_does_not_infer_unwind_but_taints_all_active_frames():
    obs = observer()
    outer = enter(obs)
    inner = enter(obs, sp=0x7f00, after_sp=0x7f04)
    ack_all(obs)
    obs.exception(10, generation=1, code=0xc0000005, address=0x70001010,
                  first_chance=True, snapshot=snap(ip=0x70001010, sp=0x7ee0))
    report = obs.report()
    assert [r["state"] for r in report["invocations"]] == ["pending", "pending"]
    assert report["events"][-1]["code"] == 0xc0000005
    assert report["events"][-1]["snapshot"]["registers"]["esp"] == 0x7ee0
    assert obs.return_hit(10, 0x401020, snap(ip=0x401020, sp=0x7f04), generation=1) == inner
    assert obs.return_hit(10, 0x401020, snap(ip=0x401020, sp=0x8004), generation=1) == outer
    assert {r["state"] for r in obs.report()["invocations"]} == {"returned-incomplete"}


def test_explicit_unwind_aborts_only_the_observed_thread_suffix():
    obs = observer()
    outer = enter(obs)
    inner = enter(obs, sp=0x7f00, after_sp=0x7f04)
    child = enter(obs, sp=0x7e00, after_sp=0x7e04)
    ack_all(obs)
    obs.unwind(10, inner, generation=1, reason="adapter observed unwind through fixture frame",
               snapshot=snap(ip=0x401030, sp=0x7ff0))
    records = obs.report()["invocations"]
    assert [r["state"] for r in records] == ["pending", "incomplete", "incomplete"]
    assert records[2]["terminal_sequence"] == records[1]["terminal_sequence"]
    assert obs.breakpoint_requests() == []
    with pytest.raises(ValueError, match="active"):
        obs.unwind(10, child, generation=1, reason="duplicate", snapshot=snap())
    assert obs.return_hit(10, 0x401020, snap(ip=0x401020, sp=0x8004), generation=1) == outer


def test_thread_exit_releases_only_its_leases_and_thread_id_reuse_is_distinct():
    obs = observer()
    obs.thread_start(20)
    enter(obs)
    enter(obs, tid=20)
    ack_all(obs)
    obs.thread_exit(10, generation=1, exit_code=7)
    assert obs.report()["invocations"][0]["state"] == "incomplete"
    assert obs.breakpoint_requests() == []
    obs.thread_start(10)
    enter(obs, generation=2)
    assert [r["thread_generation"] for r in obs.report()["invocations"]] == [1, 1, 2]
    assert obs.report()["invocations"][2]["parent"] is None
    obs.thread_exit(20, generation=1, exit_code=0)
    obs.thread_exit(10, generation=2, exit_code=0)
    assert obs.breakpoint_requests()[0]["operation"] == "remove"


def test_cleanup_failure_is_retained_and_requires_explicit_retry_and_fresh_ack():
    obs = observer()
    enter(obs)
    arm = obs.breakpoint_requests()[0]
    obs.detach("cancel before installation acknowledgment")
    remove = obs.breakpoint_requests()[0]
    with pytest.raises(ValueError, match="stale"):
        obs.acknowledge_breakpoint(arm["address"], arm["revision"], success=True)
    obs.acknowledge_breakpoint(remove["address"], remove["revision"],
                               success=False, error="partial restore")
    assert obs.report()["cleanup_complete"] is False
    assert obs.report()["events"][-1]["error"] == "partial restore"
    obs.retry_cleanup()
    retry = obs.breakpoint_requests()[0]
    assert retry["revision"] > remove["revision"]
    ack_all(obs)
    assert obs.report()["cleanup_complete"] is True


def test_process_exit_does_not_claim_original_breakpoint_bytes_were_restored():
    obs = observer()
    enter(obs)
    ack_all(obs)
    obs.process_exit(exit_code=0xc0000005)
    report = obs.report()
    assert report["closed"] is True
    assert report["invocations"][0]["state"] == "incomplete"
    assert report["breakpoints"][0]["physical"] == "process-exited"
    assert obs.breakpoint_requests() == []
    assert report["events"][-1]["kind"] == "process-exit"
    with pytest.raises(RuntimeError, match="closed"):
        enter(obs)


def test_disabled_session_cannot_emit_requests_or_observations():
    obs = ImportObserver(source={"kind": "synthetic-test", "session": "disabled"})
    with pytest.raises(RuntimeError, match="enabled"):
        obs.thread_start(10)
    with pytest.raises(RuntimeError, match="enabled"):
        enter(obs)
    assert obs.breakpoint_requests() == []
    assert obs.report()["invocations"] == []


def test_entry_snapshot_and_report_cannot_be_mutated_to_rewrite_evidence():
    source = {"kind": "synthetic-test", "session": "unit", "identity": {"hash": "test-only"}}
    obs = ImportObserver(enabled=True, source=source)
    obs.thread_start(10)
    entry = snap(reads=(MemoryRead(0x8004, 8, b"\x11" * 8, "stack-arguments"),))
    obs.entry(10, "capture-1", "fixture!raw", entry, generation=1, return_site=0x401020,
              return_sp=0x8004, return_sp_basis="fixture C3")
    entry.registers["eax"] = 999
    source["identity"]["hash"] = "changed"
    report = obs.report()
    report["invocations"][0]["entry"]["memory"][1]["data_hex"] = ""
    report["invocations"][0]["gaps"].append("tamper")
    assert obs.report()["invocations"][0]["entry"]["registers"]["eax"] == 1
    assert obs.report()["invocations"][0]["entry"]["memory"][1]["data_hex"] == "11" * 8
    assert obs.report()["invocations"][0]["gaps"] == []
    assert obs.report()["source"]["identity"]["hash"] == "test-only"


def test_missing_registers_and_unreadable_argument_evidence_never_become_zeroes():
    obs = observer()
    entry = Snapshot({"esp": 0x8000, "eip": 0x70001000},
                     (MemoryRead(0x8000, 4, bytes.fromhex("20104000"), "return-word"),
                      MemoryRead(0x8004, 16, b"", "stack-arguments", "unmapped")))
    obs.entry(10, "capture-1", "fixture!raw", entry, generation=1, return_site=0x401020,
              return_sp=0x8004, return_sp_basis="fixture C3")
    ack_all(obs)
    obs.return_hit(10, 0x401020, snap(ip=0x401020, sp=0x8004), generation=1)
    record = obs.report()["invocations"][0]
    assert "eax" not in record["entry"]["registers"]
    assert "missing-register:eax" in record["gaps"]
    assert "incomplete-read:1" in record["gaps"]
    assert record["state"] == "returned-incomplete"


@pytest.mark.parametrize("site", [None, 0x401021])
def test_unknown_or_contradictory_return_word_never_arms_a_breakpoint(site):
    obs = observer()
    obs.entry(10, "capture-1", "fixture!raw", snap(), generation=1, return_site=site,
              return_sp=0x8004, return_sp_basis="fixture C3")
    assert obs.breakpoint_requests() == []
    assert "unverified-return-word" in obs.report()["invocations"][0]["gaps"]


def test_conflicting_return_word_reads_are_not_silently_resolved_by_list_order():
    obs = observer()
    entry = snap(reads=(MemoryRead(0x8000, 4, bytes.fromhex("21104000"), "second-read"),))
    obs.entry(10, "capture-1", "fixture!raw", entry, generation=1, return_site=0x401020,
              return_sp=0x8004, return_sp_basis="fixture C3")
    assert obs.breakpoint_requests() == []
    assert "unverified-return-word" in obs.report()["invocations"][0]["gaps"]


def test_interrupted_breakpoint_step_invalidates_readiness_and_retains_a_gap():
    obs = observer()
    enter(obs)
    ack_all(obs)
    obs.breakpoint_lost(0x401020, reason="single-step interruption; not rearmed")
    assert obs.breakpoint_requests()[0]["operation"] == "arm"
    assert obs.return_hit(10, 0x401020, snap(ip=0x401020, sp=0x8004), generation=1) is None
    assert obs.report()["events"][-1]["kind"] == "unarmed-return"
    ack_all(obs)
    assert obs.return_hit(10, 0x401020, snap(ip=0x401020, sp=0x8004), generation=1) == 1
    assert obs.report()["invocations"][0]["state"] == "returned-incomplete"
    assert "breakpoint-lost:single-step interruption; not rearmed" in obs.report()["invocations"][0]["gaps"]


def test_install_failure_keeps_pending_evidence_and_cleanup_requires_restoration():
    obs = observer()
    enter(obs)
    request = obs.breakpoint_requests()[0]
    obs.acknowledge_breakpoint(request["address"], request["revision"],
                               success=False, error="short write")
    assert obs.return_hit(10, 0x401020, snap(ip=0x401020, sp=0x8004), generation=1) is None
    obs.detach("install failed")
    assert obs.breakpoint_requests()[0]["operation"] == "remove"
    assert "breakpoint-operation-failed" in obs.report()["invocations"][0]["gaps"]
    ack_all(obs)
    assert obs.report()["cleanup_complete"] is True


def test_new_owner_cancels_pending_removal_and_rejects_its_stale_ack():
    obs = observer()
    enter(obs)
    ack_all(obs)
    obs.return_hit(10, 0x401020, snap(ip=0x401020, sp=0x8004), generation=1)
    old_remove = obs.breakpoint_requests()[0]
    second = enter(obs)
    assert obs.breakpoint_requests()[0]["operation"] == "arm"
    with pytest.raises(ValueError, match="stale"):
        obs.acknowledge_breakpoint(old_remove["address"], old_remove["revision"], success=True)
    ack_all(obs)
    assert obs.return_hit(10, 0x401020, snap(ip=0x401020, sp=0x8004), generation=1) == second


@pytest.mark.parametrize("read_args", [
    (-1, 4, b"", "test"),
    (0xffffffff, 2, b"", "test"),
    (0x8000, -1, b"", "test"),
    (0x8000, 1, b"\x00\x01", "test"),
    (0x8000, 1, b"", ""),
    (True, 4, b"", "test"),
])
def test_invalid_memory_evidence_is_rejected(read_args):
    with pytest.raises(ValueError):
        MemoryRead(*read_args)


def test_typed_identifiers_cannot_alias_threads_or_invocations_via_boolean_values():
    obs = ImportObserver(enabled=True, source={"kind": "synthetic-test", "session": "unit"})
    obs.thread_start(1)
    enter(obs, tid=1)
    with pytest.raises(ValueError):
        enter(obs, tid=True)
    with pytest.raises(ValueError):
        obs.return_hit(True, 0x401020, snap(ip=0x401020, sp=0x8004), generation=1)
    with pytest.raises(ValueError):
        obs.unwind(1, True, generation=1, reason="invalid token", snapshot=snap())


def test_breakpoint_ack_retains_raw_verification_reads_without_claiming_independent_proof():
    obs = observer()
    enter(obs)
    request = obs.breakpoint_requests()[0]
    memory = (MemoryRead(0x401020, 1, b"\xcc", "adapter breakpoint verification"),)
    obs.acknowledge_breakpoint(request["address"], request["revision"], success=True,
                               memory=memory)
    assert obs.report()["events"][-1]["memory"][0]["data_hex"] == "cc"
    assert obs.report()["cleanup_evidence"] == "adapter-acknowledgments"
    with pytest.raises(ValueError, match="stale"):
        obs.acknowledge_breakpoint(request["address"], request["revision"], success=True)


def test_distinct_imports_and_return_sites_keep_identity_and_independent_cleanup():
    obs = observer()
    obs.entry(10, "capture-outer", "fixture!one", snap(), generation=1, return_site=0x401020,
              return_sp=0x8004, return_sp_basis="fixture C3")
    obs.entry(10, "capture-inner", "fixture!two", snap(sp=0x7f00, site=0x401040),
              generation=1, return_site=0x401040, return_sp=0x7f10, return_sp_basis="fixture C2 0c00")
    assert len(obs.breakpoint_requests()) == 2
    ack_all(obs)
    assert obs.return_hit(10, 0x401040, snap(ip=0x401040, sp=0x7f10), generation=1) == 2
    assert obs.breakpoint_requests()[0]["address"] == 0x401040
    assert obs.report()["breakpoints"][0]["owners"] == [1]
    records = obs.report()["invocations"]
    assert [(r["import_name"], r["scope_id"]) for r in records] == [
        ("fixture!one", "capture-outer"), ("fixture!two", "capture-inner"),
    ]


@pytest.mark.parametrize("invalid", [
    {"return_sp": 0x8004, "return_sp_basis": None},
    {"return_sp": None, "return_sp_basis": "unsupported assertion"},
    {"return_sp": -1, "return_sp_basis": "invalid"},
    {"return_site": 0x100000000},
])
def test_invalid_entry_contract_is_rejected_without_partial_state(invalid):
    obs = observer()
    options = dict(return_site=0x401020, return_sp=0x8004, return_sp_basis="fixture C3")
    options.update(invalid)
    before = obs.report()
    with pytest.raises(ValueError):
        obs.entry(10, "capture-1", "fixture!raw", snap(), generation=1, **options)
    assert obs.report() == before


def test_invalid_register_values_are_not_truncated_or_replaced():
    obs = observer()
    bad = snap()
    bad.registers["eax"] = -1
    with pytest.raises(ValueError):
        obs.entry(10, "capture-1", "fixture!raw", bad, generation=1, return_site=0x401020)
    assert obs.report()["invocations"] == []


@pytest.mark.parametrize("source", [
    {"kind": "synthetic-test", "session": "unit", "object": object()},
    {"kind": "synthetic-test", "session": "unit", "not_json": float("nan")},
    {"kind": "", "session": "unit"},
])
def test_provenance_must_be_retained_as_strict_json(source):
    with pytest.raises(ValueError):
        ImportObserver(enabled=True, source=source)


def test_detach_is_idempotent_and_no_late_event_can_reopen_the_session():
    obs = observer()
    enter(obs)
    obs.detach("cancel")
    first = obs.report()
    obs.detach("again")
    assert obs.report() == first
    with pytest.raises(RuntimeError, match="closed"):
        obs.return_hit(10, 0x401020, snap(ip=0x401020, sp=0x8004), generation=1)
    assert obs.report() == first


def test_process_exit_during_cleanup_invalidates_pending_writes_but_is_not_restoration():
    obs = observer()
    enter(obs)
    ack_all(obs)
    obs.detach("cancel")
    remove = obs.breakpoint_requests()[0]
    obs.process_exit(exit_code=0)
    assert obs.report()["cleanup_complete"] is False
    assert obs.report()["process_exited"] is True
    with pytest.raises(ValueError, match="stale"):
        obs.acknowledge_breakpoint(remove["address"], remove["revision"], success=True)
    with pytest.raises(RuntimeError):
        obs.retry_cleanup()


def test_snapshot_gap_is_retained_even_when_both_endpoints_match():
    obs = observer()
    obs.entry(10, "capture-1", "fixture!raw", snap(gaps=("x87-context-unavailable",)),
              generation=1, return_site=0x401020, return_sp=0x8004, return_sp_basis="fixture C3")
    ack_all(obs)
    obs.return_hit(10, 0x401020, snap(ip=0x401020, sp=0x8004), generation=1)
    assert obs.report()["invocations"][0]["state"] == "returned-incomplete"
    assert "x87-context-unavailable" in obs.report()["invocations"][0]["gaps"]


@pytest.mark.parametrize("overlap", [
    MemoryRead(0x8001, 1, b"\x11", "overlapping partial sample"),
    MemoryRead(0x7fff, 8, b"\x00\x21\x10\x40\x00", "larger partial sample", "short read"),
])
def test_any_conflicting_observed_return_word_bytes_prevent_arming(overlap):
    obs = observer()
    obs.entry(10, "capture-1", "fixture!raw", snap(reads=(overlap,)),
              generation=1, return_site=0x401020, return_sp=0x8004, return_sp_basis="fixture C3")
    assert obs.breakpoint_requests() == []
    assert "unverified-return-word" in obs.report()["invocations"][0]["gaps"]


def test_superseded_breakpoint_requests_remain_in_the_audit_history():
    obs = observer()
    enter(obs)
    obs.detach("cancel before arm ack")
    requests = [event for event in obs.report()["events"] if event["kind"] == "breakpoint-request"]
    assert [(event["operation"], event["revision"], event["owners"]) for event in requests] == [
        ("arm", 1, [1]), ("remove", 2, []),
    ]


def test_stale_return_after_tid_reuse_cannot_complete_the_new_invocation():
    obs = ImportObserver(enabled=True, source={"kind": "synthetic-test", "session": "reuse"})
    old_generation = obs.thread_start(1)
    old = enter(obs, tid=1, generation=old_generation)
    ack_all(obs)
    old_endpoint = snap(ip=0x401020, sp=0x8004, gaps=("old-event-gap",))
    obs.thread_exit(1, generation=old_generation, exit_code=7)
    new_generation = obs.thread_start(1)
    new = enter(obs, tid=1, generation=new_generation)
    ack_all(obs)
    assert old != new
    before = obs.report()
    assert obs.return_hit(1, 0x401020, old_endpoint, generation=old_generation) is None
    assert obs.report()["invocations"][new - 1]["state"] == "pending"
    assert obs.report()["invocations"] == before["invocations"]
    assert obs.report()["breakpoints"] == before["breakpoints"]
    rejected = obs.report()["events"][-1]
    assert rejected["kind"] == "rejected-thread-event" and rejected["event_kind"] == "return"
    assert rejected["generation"] == old_generation
    assert rejected["active_generation"] == new_generation
    assert rejected["snapshot"] == old_endpoint.report()
    assert rejected["snapshot"]["gaps"] == ["old-event-gap"]
    assert obs.return_hit(1, 0x401020, snap(ip=0x401020, sp=0x8004),
                          generation=new_generation) == new


@pytest.mark.parametrize("event_kind", ["entry", "exception", "thread-exit", "unwind"])
def test_stale_thread_events_preserve_raw_evidence_without_touching_reused_tid(event_kind):
    obs = observer()
    enter(obs)
    obs.thread_exit(10, generation=1, exit_code=7)
    generation = obs.thread_start(10)
    new = enter(obs, generation=generation)
    ack_all(obs)
    raw = snap(ip=0x70001010, gaps=("stale-context-gap",),
               reads=(MemoryRead(0x6000, 8, b"\x11", "stale-partial-read", "partial"),))
    before = obs.report()
    if event_kind == "entry":
        result = obs.entry(10, "old-capture", "fixture!old", raw, generation=1,
                           return_site=0x401020, return_sp=0x8004, return_sp_basis="old C3")
    elif event_kind == "exception":
        result = obs.exception(10, generation=1, code=0xc0000005, address=0x70001010,
                               first_chance=True, snapshot=raw)
    elif event_kind == "thread-exit":
        result = obs.thread_exit(10, generation=1, exit_code=23)
    else:
        result = obs.unwind(10, new, generation=1, reason="old queued unwind", snapshot=raw)
    assert result is (None if event_kind == "entry" else False)
    after = obs.report()
    assert after["invocations"] == before["invocations"]
    assert after["breakpoints"] == before["breakpoints"]
    rejected = after["events"][-1]
    assert rejected["kind"] == "rejected-thread-event"
    assert rejected["event_kind"] == event_kind
    assert (rejected["generation"], rejected["active_generation"]) == (1, generation)
    assert rejected["gaps"] == ["stale-or-inactive-thread-generation"]
    if event_kind == "thread-exit":
        assert rejected["exit_code"] == 23
    else:
        assert rejected["snapshot"] == raw.report()
    assert obs.return_hit(10, 0x401020, snap(ip=0x401020, sp=0x8004), generation=generation) == new
    assert obs.report()["invocations"][new - 1]["state"] == "paired-endpoints"


@pytest.mark.parametrize("event_kind", ["entry", "return", "exception", "thread-exit", "unwind"])
def test_thread_event_generation_is_required_not_inferred_from_current_tid(event_kind):
    obs = observer()
    invocation = enter(obs)
    before = obs.report()
    with pytest.raises(TypeError, match="generation"):
        if event_kind == "entry":
            obs.entry(10, "capture", "fixture!raw", snap(), return_site=0x401020)
        elif event_kind == "return":
            obs.return_hit(10, 0x401020, snap(ip=0x401020, sp=0x8004))
        elif event_kind == "exception":
            obs.exception(10, code=0xc0000005, address=0x70001010, first_chance=True, snapshot=snap())
        elif event_kind == "thread-exit":
            obs.thread_exit(10, exit_code=0)
        else:
            obs.unwind(10, invocation, reason="no identity", snapshot=snap())
    assert obs.report() == before


@pytest.mark.parametrize("generation", [True, 0, -1, None, "1", 1.0])
def test_generation_tokens_are_exact_positive_integers(generation):
    obs = observer()
    enter(obs)
    before = obs.report()
    with pytest.raises(ValueError, match="generation"):
        obs.return_hit(10, 0x401020, snap(ip=0x401020, sp=0x8004), generation=generation)
    assert obs.report() == before


def test_return_after_exit_is_rejected_even_without_a_replacement_thread():
    obs = observer()
    enter(obs)
    obs.thread_exit(10, generation=1, exit_code=0)
    before = obs.report()
    raw = snap(ip=0x401020, sp=0x8004)
    assert obs.return_hit(10, 0x401020, raw, generation=1) is None
    assert obs.report()["invocations"] == before["invocations"]
    assert obs.report()["events"][-1]["active_generation"] is None
    assert obs.report()["events"][-1]["snapshot"] == raw.report()


def test_current_generation_exception_unwind_and_exit_are_still_accepted_after_reuse():
    obs = observer()
    obs.thread_exit(10, generation=1, exit_code=0)
    generation = obs.thread_start(10)
    invocation = enter(obs, generation=generation)
    assert obs.exception(10, generation=generation, code=0xc0000005, address=0x70001010,
                         first_chance=True, snapshot=snap()) is True
    assert obs.unwind(10, invocation, generation=generation,
                      reason="current observed unwind", snapshot=snap()) is True
    assert obs.thread_exit(10, generation=generation, exit_code=0) is True
    record = obs.report()["invocations"][0]
    assert record["thread_generation"] == generation
    assert record["state"] == "incomplete"
