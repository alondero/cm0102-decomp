"""Offline debugger doubles only; no game, native child or API outcome evidence."""
import struct

import pytest

from cmre.capture_import_observe import RecorderImportObserver
from cmre import capture_import_observe
from cmre.import_observe import MemoryRead, REGISTERS, Snapshot


SITE, SLOT, TARGET, SP = 0x401026, 0x402000, 0x70001000, 0x8000
HEAP = "KERNEL32.dll!HeapAlloc"
READ = "KERNEL32.dll!ReadFile"


def snapshot(ip=TARGET, sp=SP, eax=0):
    return Snapshot({**dict.fromkeys(REGISTERS, 0), "eip": ip, "esp": sp,
                     "eax": eax, "eflags": 0x202})


class Debugger:
    """Public adapter I/O double: stopped-process reads and breakpoint acknowledgments."""

    def __init__(self):
        self.memory = {}
        self.reads = []
        self.operations = []
        self.fail_operation = False
        self.recoveries = []
        self.recovery_fails = False

    def put(self, address, blob):
        self.memory.update({address + i: value for i, value in enumerate(blob)})

    def read(self, address, size, purpose):
        self.reads.append((address, size, purpose))
        blob = bytearray()
        for i in range(size):
            if address + i not in self.memory:
                break
            blob.append(self.memory[address + i])
        return MemoryRead(address, size, bytes(blob), purpose,
                          None if len(blob) == size else "ReadProcessMemory:299")

    def breakpoint(self, request):
        self.operations.append(dict(request))
        if self.fail_operation:
            raise OSError("injected restoration failure")
        return ()

    def recover_step(self, lease):
        self.recoveries.append(dict(lease))
        if self.recovery_fails:
            raise OSError("cannot clear trap flag")
        return snapshot(ip=TARGET + 1)

    def prepare(self, args, site=SITE, sp=SP):
        self.put(site - 6, b"\xff\x15" + struct.pack("<I", SLOT))
        self.put(SLOT, struct.pack("<I", TARGET))
        self.put(sp, struct.pack("<" + "I" * (len(args) + 1), site, *args))


def adapter(name=HEAP, *, enabled=True):
    io = Debugger()
    obs = RecorderImportObserver(
        enabled=enabled, source={"kind": "mock-debugger", "session": "offline"},
        imports={SLOT: name}, text_range=(0x401000, 0x402000),
        read=io.read, breakpoint=io.breakpoint, recover_step=io.recover_step)
    return obs, io


def scope(obs, tid=10, scope_id="capture-1"):
    generation = obs.thread_start(tid)
    obs.begin_scope(tid, generation, scope_id)
    return generation


def test_opt_in_scope_verified_heap_endpoint_and_closed_report(tmp_path, monkeypatch):
    monkeypatch.setattr(capture_import_observe, "OBSERVATION_STORE", tmp_path)
    obs, io = adapter()
    generation = scope(obs)
    io.prepare([7, 8, 4])
    invocation = obs.import_entry(10, generation, snapshot(), iat_slot=SLOT)
    assert invocation == 1
    assert io.operations[-1]["operation"] == "arm"
    io.put(0x5000, b"abcd")
    assert obs.return_hit(10, generation, SITE, snapshot(SITE, SP + 16, 0x5000)) == 1
    report = obs.report()
    record = report["invocations"][0]
    assert record["scope_id"] == "capture-1"
    assert record["state"] == "paired-endpoints"
    assert record["return"]["memory"][0]["data_hex"] == "61626364"
    assert record["replayable"] is False and report["replayable"] is False
    assert io.operations[-1]["operation"] == "remove"
    obs.close("offline finished")
    path = tmp_path / "observation.json"
    obs.save(path)
    original = path.read_bytes()
    with pytest.raises(FileExistsError):
        obs.save(path)
    assert path.read_bytes() == original


@pytest.mark.parametrize("scenario, expected", [
    ("exact", None), ("short", "short-read"), ("error", "api-error-or-pending"),
    ("async", "overlapped-unsupported"), ("unknown-sync", "synchronous-handle-unproven"),
    ("partial", "byte-count-unreadable"), ("limit", "observation-limit:64"),
])
def test_readfile_outputs_keep_counts_tails_and_uncertainty(scenario, expected):
    obs, io = adapter(READ)
    generation = scope(obs)
    size = 128 if scenario == "limit" else 8
    count = 2 if scenario == "short" else size
    io.prepare([7, 0x5000, size, 0x6000, 0x9000 if scenario == "async" else 0])
    io.put(0x5000, b"xy" + b"\xa5" * (size - 2))
    io.put(0x6000, b"\x02\x00" if scenario == "partial" else struct.pack("<I", count))
    basis = None if scenario == "unknown-sync" else "Observed CreateFileA flags=0 in this scope"
    obs.import_entry(10, generation, snapshot(), iat_slot=SLOT, synchronous_file_basis=basis)
    obs.return_hit(10, generation, SITE, snapshot(SITE, SP + 24, 0 if scenario == "error" else 1))
    record = obs.report()["invocations"][0]
    assert record["state"] == ("paired-endpoints" if expected is None else "returned-incomplete")
    assert record["replayable"] is False
    memory = {read["purpose"]: read for read in record["return"]["memory"]}
    buffer = memory["ReadFile.buffer"]
    assert buffer["requested_size"] == size
    assert buffer["data_hex"] == (b"xy" + b"\xa5" * (min(64, size) - 2)).hex()
    if expected is not None:
        assert expected in str(record["gaps"]) + str(memory)
    if scenario == "partial":
        assert memory["ReadFile.byte-count"]["data_hex"] == "0200"
        assert memory["ReadFile.byte-count"]["error"] == "ReadProcessMemory:299"
    assert max(size for _, size, purpose in io.reads if purpose.endswith(".buffer")) <= 64


def test_heap_cap_keeps_requested_size_and_null_does_not_read():
    for pointer in (0, 0x5000):
        obs, io = adapter()
        generation = scope(obs)
        io.prepare([7, 8, 128])
        io.put(0x5000, b"\x11" * 128)
        obs.import_entry(10, generation, snapshot(), iat_slot=SLOT)
        obs.return_hit(10, generation, SITE, snapshot(SITE, SP + 16, pointer))
        record = obs.report()["invocations"][0]
        assert record["state"] == "returned-incomplete"
        if pointer:
            read = record["return"]["memory"][0]
            assert read["requested_size"] == 128 and len(bytes.fromhex(read["data_hex"])) == 64
            assert "observation-limit:64" in read["error"]
        else:
            assert "allocation-failed" in record["gaps"]
            assert not any(purpose == "HeapAlloc.buffer" for _, _, purpose in io.reads)


@pytest.mark.parametrize("fault", ["no-scope", "stale", "call", "iat", "word", "disabled"])
def test_unverified_or_unscoped_import_cannot_arm_return_breakpoint(fault):
    obs, io = adapter(enabled=fault != "disabled")
    io.prepare([7, 8, 4])
    if fault == "disabled":
        with pytest.raises(RuntimeError, match="not enabled"):
            obs.thread_start(10)
        assert not io.operations
        return
    generation = obs.thread_start(10)
    if fault != "no-scope":
        obs.begin_scope(10, generation, "capture")
    if fault == "stale":
        generation += 1
    if fault == "call":
        io.put(SITE - 6, b"\x90" * 6)
    if fault == "iat":
        io.put(SLOT, struct.pack("<I", TARGET + 1))
    if fault == "word":
        io.put(SP, struct.pack("<I", 0x1010))
    with pytest.raises(ValueError):
        obs.import_entry(10, generation, snapshot(), iat_slot=SLOT)
    assert not io.operations
    assert obs.report()["invocations"] == []


@pytest.mark.parametrize("fault", ["thread", "generation", "site", "sp", "ip"])
def test_near_miss_return_never_reads_outputs_or_consumes_call(fault):
    obs, io = adapter()
    generation = scope(obs)
    io.prepare([7, 8, 4])
    obs.import_entry(10, generation, snapshot(), iat_slot=SLOT)
    before = list(io.reads)
    assert obs.return_hit(20 if fault == "thread" else 10,
                          generation + 1 if fault == "generation" else generation,
                          SITE + 1 if fault == "site" else SITE,
                          snapshot(SITE + 1 if fault == "ip" else SITE,
                                   SP + 20 if fault == "sp" else SP + 16, 0x5000)) is None
    assert io.reads == before
    assert obs.report()["invocations"][0]["state"] == "pending"


def test_scope_exit_and_thread_reuse_cannot_bind_prior_invocation():
    obs, io = adapter()
    generation = scope(obs)
    io.prepare([7, 8, 4])
    obs.import_entry(10, generation, snapshot(), iat_slot=SLOT)
    obs.end_scope(10, generation, "capture-1", snapshot(), reason="capture left")
    assert obs.report()["invocations"][0]["state"] == "incomplete"
    assert io.operations[-1]["operation"] == "remove"
    obs.thread_exit(10, generation, exit_code=0)
    next_generation = obs.thread_start(10)
    assert next_generation == 2
    obs.begin_scope(10, next_generation, "capture-2")
    obs.import_entry(10, next_generation, snapshot(), iat_slot=SLOT)
    assert obs.return_hit(10, generation, SITE, snapshot(SITE, SP + 16, 0x5000)) is None
    assert obs.report()["invocations"][1]["state"] == "pending"
    obs.thread_exit(10, next_generation, exit_code=0)
    assert obs.report()["invocations"][1]["state"] == "incomplete"
    obs.close("done")
    assert obs.report()["cleanup_complete"]


def test_scope_end_identity_mismatch_keeps_active_observation():
    obs, io = adapter()
    generation = scope(obs)
    io.prepare([7, 8, 4])
    obs.import_entry(10, generation, snapshot(), iat_slot=SLOT)
    with pytest.raises(ValueError, match="scope"):
        obs.end_scope(10, generation, "not-capture-1", snapshot(), reason="bad event")
    assert obs.report()["invocations"][0]["state"] == "pending"


@pytest.mark.parametrize("phase", ["prepared", "stepped"])
def test_interrupted_step_requires_same_generation_tf_recovery_and_byte_restoration(phase):
    obs, io = adapter()
    generation = scope(obs)
    io.prepare([7, 8, 4])
    obs.import_entry(10, generation, snapshot(), iat_slot=SLOT)
    obs.begin_step(10, generation, TARGET, snapshot())
    # The fake backend may have performed none or all of the guest step; the
    # adapter cannot infer progress and must recover its outstanding obligation.
    obs.close("injected interruption: " + phase)
    report = obs.report()
    assert report["invocations"][0]["state"] == "incomplete"
    assert io.recoveries == [{"tid": 10, "generation": 1, "address": TARGET}]
    assert io.operations[-1]["operation"] == "remove"
    assert report["cleanup_complete"]
    assert report["step_recovery"][-1]["snapshot"]["registers"]["eflags"] & 0x100 == 0


def test_failed_step_recovery_never_claims_safe_cleanup_and_can_retry():
    obs, io = adapter()
    generation = scope(obs)
    io.prepare([7, 8, 4])
    obs.import_entry(10, generation, snapshot(), iat_slot=SLOT)
    obs.begin_step(10, generation, TARGET, snapshot())
    io.recovery_fails = True
    with pytest.raises(OSError, match="trap flag"):
        obs.close("interrupted")
    assert not obs.report()["cleanup_complete"]
    # Even when context recovery fails, return-breakpoint restoration is tried.
    assert io.operations[-1]["operation"] == "remove"
    io.recovery_fails = False
    obs.close("retry cleanup")
    assert obs.report()["cleanup_complete"]
    assert len(obs.report()["step_recovery"]) == 2


def test_step_generation_and_uncleared_tf_cannot_discharge_obligation():
    obs, io = adapter()
    generation = scope(obs)
    io.prepare([7, 8, 4])
    obs.import_entry(10, generation, snapshot(), iat_slot=SLOT)
    obs.begin_step(10, generation, TARGET, snapshot())
    with pytest.raises(ValueError, match="step"):
        obs.finish_step(10, generation + 1, snapshot())
    dirty = snapshot()
    dirty.registers["eflags"] |= 0x100
    with pytest.raises(ValueError, match="trap"):
        obs.finish_step(10, generation, dirty)
    with pytest.raises(RuntimeError, match="step"):
        obs.return_hit(10, generation, SITE, snapshot(SITE, SP + 16, 0x5000))
    obs.finish_step(10, generation, snapshot(TARGET + 1))
    obs.close("done")
    assert obs.report()["cleanup_complete"] and io.recoveries == []


def test_breakpoint_remove_failure_stays_incomplete_until_acknowledged():
    obs, io = adapter()
    generation = scope(obs)
    io.prepare([7, 8, 4])
    obs.import_entry(10, generation, snapshot(), iat_slot=SLOT)
    io.fail_operation = True
    with pytest.raises(OSError):
        obs.close("done")
    assert not obs.report()["cleanup_complete"]
    io.fail_operation = False
    obs.close("retry")
    assert obs.report()["cleanup_complete"]


def test_outputs_with_wrapping_ranges_are_retained_incomplete_without_reading():
    obs, io = adapter()
    generation = scope(obs)
    io.prepare([7, 8, 128])
    obs.import_entry(10, generation, snapshot(), iat_slot=SLOT)
    before = list(io.reads)
    assert obs.return_hit(10, generation, SITE, snapshot(SITE, SP + 16, 0xfffffff0)) == 1
    assert io.reads == before
    assert "invalid-output-range" in obs.report()["invocations"][0]["gaps"]


def test_closed_adapter_rejects_events_before_reading_or_adding_scopes():
    obs, io = adapter()
    generation = scope(obs)
    obs.close("done")
    before = list(io.reads)
    with pytest.raises(RuntimeError, match="closed"):
        obs.import_entry(10, generation, snapshot(), iat_slot=SLOT)
    assert io.reads == before
    with pytest.raises(RuntimeError, match="closed"):
        obs.begin_scope(20, 1, "late")


def test_local_store_escape_is_rejected_and_failed_cleanup_evidence_can_be_saved(tmp_path, monkeypatch):
    store = tmp_path / "ignored"
    monkeypatch.setattr(capture_import_observe, "OBSERVATION_STORE", store)
    obs, io = adapter()
    generation = scope(obs)
    io.prepare([7, 8, 4])
    obs.import_entry(10, generation, snapshot(), iat_slot=SLOT)
    io.fail_operation = True
    with pytest.raises(OSError):
        obs.close("cleanup failed")
    with pytest.raises(ValueError, match="local"):
        obs.save(tmp_path / "escape.json")
    path = store / "failed.json"
    obs.save(path)
    assert b'"cleanup_complete": false' in path.read_bytes()


def test_reused_tid_never_receives_prior_steps_context_recovery():
    obs, io = adapter()
    generation = scope(obs)
    io.prepare([7, 8, 4])
    obs.import_entry(10, generation, snapshot(), iat_slot=SLOT)
    obs.begin_step(10, generation, TARGET, snapshot())
    obs.thread_exit(10, generation, exit_code=0)
    assert obs.thread_start(10) == 2
    before = obs.report()
    with pytest.raises(ValueError, match="step"):
        obs.finish_step(10, generation, snapshot(TARGET + 1))
    after = obs.report()
    assert after["pending_step"] == before["pending_step"]
    assert after["step_recovery"] == before["step_recovery"]
    assert not after["cleanup_complete"]
    with pytest.raises(RuntimeError, match="stale thread"):
        obs.close("cannot restore through reused tid")
    assert io.recoveries == []
    assert not obs.report()["cleanup_complete"]


def test_import_entry_during_pending_step_rejected_before_any_io_or_invocation():
    obs, io = adapter()
    generation = scope(obs)
    io.prepare([7, 8, 4])
    obs.import_entry(10, generation, snapshot(), iat_slot=SLOT)
    obs.begin_step(10, generation, TARGET, snapshot())
    before = obs.report()
    reads, operations = list(io.reads), list(io.operations)
    with pytest.raises(RuntimeError, match="step"):
        obs.import_entry(10, generation, snapshot(), iat_slot=SLOT)
    assert io.reads == reads
    assert io.operations == operations
    assert obs.report() == before


def test_two_thread_scopes_share_return_site_without_early_removal():
    obs, io = adapter()
    g1 = scope(obs)
    g2 = scope(obs, tid=20, scope_id="capture-2")
    io.prepare([7, 8, 4])
    obs.import_entry(10, g1, snapshot(), iat_slot=SLOT)
    obs.import_entry(20, g2, snapshot(), iat_slot=SLOT)
    io.put(0x5000, b"data")
    obs.return_hit(20, g2, SITE, snapshot(SITE, SP + 16, 0x5000))
    assert [r["operation"] for r in io.operations] == ["arm"]
    obs.return_hit(10, g1, SITE, snapshot(SITE, SP + 16, 0x5000))
    assert [r["operation"] for r in io.operations] == ["arm", "remove"]
    assert [r["scope_id"] for r in obs.report()["invocations"]] == ["capture-1", "capture-2"]


def test_invalid_buffer_preserves_already_observed_count_and_reports_effect_limits():
    obs, io = adapter(READ)
    generation = scope(obs)
    io.prepare([7, 0xfffffff0, 128, 0x6000, 0])
    io.put(0x6000, struct.pack("<I", 8))
    obs.import_entry(10, generation, snapshot(), iat_slot=SLOT,
                     synchronous_file_basis="Observed synchronous CreateFileA flags=0")
    obs.return_hit(10, generation, SITE, snapshot(SITE, SP + 24, 1))
    report = obs.report()
    record = report["invocations"][0]
    assert record["state"] == "returned-incomplete"
    assert record["return"]["memory"][0]["data_hex"] == "08000000"
    assert "invalid-output-range" in record["gaps"]
    assert report["effects_complete"] is False and report["recorder_wired"] is False
    # Public reports are snapshots, not a mutable route back to retained evidence.
    record["return"]["memory"].clear()
    assert obs.report()["invocations"][0]["return"]["memory"][0]["data_hex"] == "08000000"
