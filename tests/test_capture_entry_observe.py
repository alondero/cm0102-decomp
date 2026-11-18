"""Function scopes are observed native endpoints, not emulator completion."""
import struct

import pytest

from cmre.import_observe import ImportObserver, MemoryRead, Snapshot
from cmre.capture_entry_observe import EntryImportScopes


def snap(ip=0x401000, sp=0x10000):
    return Snapshot(dict(eax=0, ebx=0, ecx=0, edx=0, esi=0, edi=0,
                         ebp=0, esp=sp, eip=ip, eflags=0x202))


def setup_scope():
    obs = ImportObserver(enabled=True, source=dict(kind="synthetic", session="entry-test"))
    generation = obs.thread_start(7)
    scopes = EntryImportScopes(obs, max_scopes=2)
    return obs, generation, scopes


def enter(scopes, generation, *, word=None, **kwargs):
    return scopes.function_entry(
        7, generation, snap(), function=0x401000,
        return_word=word or MemoryRead(0x10000, 4, struct.pack("<I", 0x402000), "return-word"),
        return_sp=0x10004, return_sp_basis="authored C3 contract", **kwargs)


def ack(obs):
    for request in obs.breakpoint_requests():
        obs.acknowledge_breakpoint(request["address"], request["revision"], success=True)


def test_scope_follows_real_endpoint_not_emulator_completion():
    obs, gen, scopes = setup_scope()
    root = enter(scopes, gen)
    ack(obs)
    scope = scopes.scope_for(7, gen)
    assert scope is not None
    assert scopes.report()["scopes"][0]["state"] == "pending"
    # Import protocol remains the only pairing and breakpoint owner.
    entry = snap(0x70001000, 0xfff0)
    entry = Snapshot(entry.registers, (
        MemoryRead(0xfff0, 4, struct.pack("<I", 0x401020), "return-word"),))
    child = obs.entry(7, scope, "KERNEL32.dll!HeapAlloc", entry,
                      generation=gen, return_site=0x401020, return_sp=0x10000,
                      return_sp_basis="independently checked selected stdcall")
    ack(obs)
    assert obs.return_hit(7, 0x401020, snap(0x401020, 0x10000), generation=gen) == child
    assert scopes.scope_for(7, gen) == scope
    assert obs.return_hit(7, 0x402000, snap(0x402000, 0x10004), generation=gen) == root
    assert scopes.scope_for(7, gen) is None
    report = scopes.report()
    assert report["scopes"][0]["state"] == "paired-endpoints"
    assert report["scopes"][0]["imports"] == [child]
    assert not report["replayable"] and not report["effects_complete"]
    assert not report["game_recorder_wired"]
    assert "import_transcript" not in report


@pytest.mark.parametrize("word", [
    MemoryRead(0x10000, 4, b"\x00", "return-word", "partial"),
    MemoryRead(0x10004, 4, struct.pack("<I", 0x402000), "return-word"),
])
def test_bad_entry_never_arms(word):
    obs, gen, scopes = setup_scope()
    with pytest.raises(ValueError):
        enter(scopes, gen, word=word)
    assert obs.breakpoint_requests() == []
    assert scopes.report()["scopes"] == []


def test_generation_overlap_and_budget_fail_closed():
    obs, gen, scopes = setup_scope()
    enter(scopes, gen)
    with pytest.raises(ValueError, match="overlap"):
        enter(scopes, gen)
    assert scopes.scope_for(7, gen + 1) is None
    obs.thread_exit(7, generation=gen, exit_code=0)
    next_gen = obs.thread_start(7)
    with pytest.raises(ValueError, match="generation"):
        enter(scopes, gen)
    enter(scopes, next_gen)
    obs.thread_exit(7, generation=next_gen, exit_code=0)
    last_gen = obs.thread_start(7)
    with pytest.raises(ValueError, match="budget"):
        enter(scopes, last_gen)
    assert scopes.scope_for(7, last_gen) is None
    assert [r["state"] for r in scopes.report()["scopes"]] == ["incomplete", "incomplete"]


def test_detach_retains_incomplete_scope_and_requests_cleanup():
    obs, gen, scopes = setup_scope()
    enter(scopes, gen)
    ack(obs)
    obs.detach("interrupted native invocation")
    assert scopes.scope_for(7, gen) is None
    assert scopes.report()["scopes"][0]["state"] == "incomplete"
    assert not scopes.report()["cleanup_complete"]
    ack(obs)
    assert scopes.report()["cleanup_complete"]


def test_unknown_cleanup_basis_cannot_create_scope():
    obs, gen, scopes = setup_scope()
    with pytest.raises(ValueError):
        scopes.function_entry(7, gen, snap(), function=0x401000,
                              return_word=MemoryRead(0x10000, 4, b"\0" * 4, "return-word"),
                              return_sp=None, return_sp_basis=None)
    assert not obs.report()["invocations"]


def test_report_is_detached_and_marks_root_as_function_not_import():
    obs, gen, scopes = setup_scope()
    root = enter(scopes, gen)
    report = scopes.report()
    assert report["scopes"][0]["invocation"] == root
    assert report["scopes"][0]["kind"] == "function-scope"
    report["scopes"].clear()
    assert len(scopes.report()["scopes"]) == 1


def test_contradictory_return_sample_does_not_enroll_unarmable_scope():
    obs, gen, scopes = setup_scope()
    raw = snap()
    raw = Snapshot(raw.registers, (
        MemoryRead(0x10001, 1, b"\xff", "contradicting-overlap"),))
    with pytest.raises(ValueError, match="contradict"):
        scopes.function_entry(
            7, gen, raw, function=0x401000,
            return_word=MemoryRead(0x10000, 4, struct.pack("<I", 0x402000), "return-word"),
            return_sp=0x10004, return_sp_basis="authored C3")
    assert not obs.report()["invocations"]
    assert not obs.breakpoint_requests()


def test_pending_debugger_tf_is_not_a_clean_native_entry():
    obs, gen, scopes = setup_scope()
    raw = snap()
    raw.registers["eflags"] |= 0x100
    with pytest.raises(ValueError, match="trap"):
        scopes.function_entry(
            7, gen, raw, function=0x401000,
            return_word=MemoryRead(0x10000, 4, struct.pack("<I", 0x402000), "return-word"),
            return_sp=0x10004, return_sp_basis="authored C3")
    assert not obs.report()["invocations"]


def test_function_scope_cannot_overlap_unowned_pending_import():
    obs, gen, scopes = setup_scope()
    obs.entry(7, "foreign-scope", "fixture!unknown", snap(), generation=gen,
              return_site=None)
    with pytest.raises(ValueError, match="pending"):
        enter(scopes, gen)
    assert len(obs.report()["invocations"]) == 1


def test_boolean_generation_cannot_borrow_integer_scope():
    obs, gen, scopes = setup_scope()
    assert gen == 1
    enter(scopes, gen)
    assert scopes.scope_for(7, True) is None


def test_stale_function_endpoint_cannot_consume_replacement_scope():
    obs, gen, scopes = setup_scope()
    enter(scopes, gen)
    ack(obs)
    obs.thread_exit(7, generation=gen, exit_code=0)
    replacement = obs.thread_start(7)
    root = enter(scopes, replacement)
    ack(obs)
    assert obs.return_hit(7, 0x402000, snap(0x402000, 0x10004), generation=gen) is None
    assert scopes.scope_for(7, replacement) is not None
    assert obs.return_hit(7, 0x402000, snap(0x402000, 0x10004), generation=replacement) == root


def test_foreign_thread_and_wrong_sp_do_not_finish_function_scope():
    obs, gen, scopes = setup_scope()
    enter(scopes, gen)
    ack(obs)
    foreign = obs.thread_start(8)
    assert obs.return_hit(8, 0x402000, snap(0x402000, 0x10004), generation=foreign) is None
    assert obs.return_hit(7, 0x402000, snap(0x402000, 0x10008), generation=gen) is None
    assert scopes.scope_for(7, gen) is not None


def test_scope_never_overrides_failed_physical_acknowledgment():
    obs, gen, scopes = setup_scope()
    enter(scopes, gen)
    request, = obs.breakpoint_requests()
    obs.acknowledge_breakpoint(request["address"], request["revision"], success=False,
                               error="physical readback failed")
    assert obs.return_hit(7, 0x402000, snap(0x402000, 0x10004), generation=gen) is None
    obs.detach("physical instrumentation failure")
    assert scopes.report()["scopes"][0]["state"] == "incomplete"
    assert not scopes.report()["cleanup_complete"]


def test_scope_index_does_not_adopt_later_call_with_reused_label():
    obs, gen, scopes = setup_scope()
    enter(scopes, gen)
    scope = scopes.scope_for(7, gen)
    ack(obs)
    obs.return_hit(7, 0x402000, snap(0x402000, 0x10004), generation=gen)
    obs.entry(7, scope, "fixture!outside", snap(), generation=gen, return_site=None)
    assert scopes.report()["scopes"][0]["imports"] == []


@pytest.mark.parametrize("tid", [True, 1.0, False, 0.0, -1, 0x100000000, "1", None])
def test_tid_lookup_and_entry_reject_non_u32_integer_identity(tid):
    obs = ImportObserver(enabled=True, source=dict(kind="synthetic", session="tid-boundary"))
    gen = obs.thread_start(1)
    scopes = EntryImportScopes(obs)
    scopes.function_entry(
        1, gen, snap(), function=0x401000,
        return_word=MemoryRead(0x10000, 4, struct.pack("<I", 0x402000), "return-word"),
        return_sp=0x10004, return_sp_basis="authored C3 contract")
    before = obs.report()
    assert scopes.scope_for(tid, gen) is None
    with pytest.raises(ValueError, match="thread ID"):
        scopes.function_entry(
            tid, gen, snap(), function=0x401000,
            return_word=MemoryRead(0x10000, 4, struct.pack("<I", 0x402000), "return-word"),
            return_sp=0x10004, return_sp_basis="authored C3 contract")
    assert obs.report() == before
    assert scopes.scope_for(1, gen) is not None


@pytest.mark.parametrize("tid", [0, 1, 0xffffffff])
def test_tid_boundary_preserves_observer_unsigned_32_bit_range(tid):
    obs = ImportObserver(enabled=True, source=dict(kind="synthetic", session="tid-range"))
    gen = obs.thread_start(tid)
    scopes = EntryImportScopes(obs)
    scopes.function_entry(
        tid, gen, snap(), function=0x401000,
        return_word=MemoryRead(0x10000, 4, struct.pack("<I", 0x402000), "return-word"),
        return_sp=0x10004, return_sp_basis="authored C3 contract")
    assert scopes.scope_for(tid, gen) is not None
