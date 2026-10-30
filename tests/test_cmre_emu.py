"""Emulator regressions. Fixtures are real functions in the pinned primary executable."""
import pytest

from cmre import emu, image

SCRATCH = 0x05000000


@pytest.fixture(scope="module")
def machine():
    return emu.Machine(image.primary(), watch=(0x00450A70, 0x00450A74))


def case(**regs):
    return emu.Case({"eflags": 0x202, **regs}, [0] * 4, bytes(emu.ARENA_SIZE))


def test_return_reaches_stop_with_flat_segments(machine):
    # 0x004671B0: lea eax, [ecx + 0x20]; ret
    r = machine.run(0x004671B0, case(ecx=0x1234), 100)
    assert r.outcome == "return"
    assert r.regs[0] == 0x1254


def test_fs_segment_reads_teb_seh_terminator(machine):
    machine.uc.mem_map(SCRATCH, 0x1000)
    machine.uc.mem_write(SCRATCH, bytes.fromhex("64a100000000c3"))  # mov eax, fs:[0]; ret
    r = machine.run(SCRATCH, case(), 100)
    assert r.outcome == "return"
    assert r.regs[0] == 0xFFFFFFFF


def test_unmapped_write_is_a_fault_not_a_crash(machine):
    # 0x00412EF0: mov dword ptr [ecx], imm; ret
    r = machine.run(0x00412EF0, case(ecx=0x10), 100)
    assert r.outcome == "fault"


def test_faulting_instruction_is_not_covered(machine):
    # 0x00450A70: mov ax, word ptr [ecx]; ret  -- faults immediately when ecx is unmapped
    r = machine.run(0x00450A70, case(ecx=0), 100)
    assert r.outcome == "fault"
    assert r.covered == set()


def test_completed_instructions_are_covered(machine):
    r = machine.run(0x00450A70, case(ecx=emu.ARENA), 100)
    assert r.outcome == "return"
    assert r.covered == {0x00450A70, 0x00450A73}


def test_cases_are_isolated_by_pristine_reset(machine):
    first = machine.run(0x00412EF0, case(ecx=emu.ARENA), 100)
    second = machine.run(0x00450A70, case(ecx=emu.ARENA), 100)
    assert first.outcome == second.outcome == "return"
    assert second.regs[0] & 0xFFFF == 0  # arena rewritten from the case, not left over


def test_code_section_is_not_writable(machine):
    # .text is read/execute in the real process; a store into it must fault on both sides.
    machine.uc.mem_map(SCRATCH + 0x1000, 0x1000)
    machine.uc.mem_write(SCRATCH + 0x1000, bytes.fromhex("a300104000c3"))  # mov [0x401000], eax; ret
    r = machine.run(SCRATCH + 0x1000, case(), 100)
    assert r.outcome == "fault"
    assert 0x00401000 not in r.memory  # the faulted store changed nothing, so nothing is reported


def _slot(name):
    return next(slot for slot, n in image.primary().imports.items() if n == name)


def _code(machine, offset, blob):
    machine.uc.mem_map(SCRATCH + offset, 0x1000)
    machine.uc.mem_write(SCRATCH + offset, blob)
    return SCRATCH + offset


def test_import_stub_pops_stdcall_args_and_records_the_call(machine):
    import struct

    # push 7; push 9; call [GetDeviceCaps]; ret
    entry = _code(machine, 0x2000, bytes.fromhex("6a076a09ff15") + struct.pack("<I", _slot("GDI32.dll!GetDeviceCaps")) + b"\xc3")
    r = machine.run(entry, emu.Case({"eflags": 0x202}, [0] * 4, bytes(emu.ARENA_SIZE), import_seed=5), 100)
    assert r.outcome == "return"  # stub popped both arguments, so ret reached STOP
    calls = [e for e in r.events if e[0] == "c"]
    assert calls == [("c", "GDI32.dll!GetDeviceCaps", 2, (9, 7))]


def test_import_stub_result_is_seeded(machine):
    import struct

    entry = _code(machine, 0x3000, bytes.fromhex("ff15") + struct.pack("<I", _slot("KERNEL32.dll!GetLastError")) + b"\xc3")
    a = machine.run(entry, emu.Case({"eflags": 0x202}, [0] * 4, bytes(emu.ARENA_SIZE), import_seed=1), 100)
    b = machine.run(entry, emu.Case({"eflags": 0x202}, [0] * 4, bytes(emu.ARENA_SIZE), import_seed=1), 100)
    c = machine.run(entry, emu.Case({"eflags": 0x202}, [0] * 4, bytes(emu.ARENA_SIZE), import_seed=2), 100)
    assert a.regs[0] == b.regs[0]
    values = {machine.run(entry, emu.Case({"eflags": 0x202}, [0] * 4, bytes(emu.ARENA_SIZE), import_seed=s), 100).regs[0]
              for s in range(16)}
    assert len(values) > 2


def test_terminal_imports_still_end_the_case(machine):
    import struct

    entry = _code(machine, 0x4000, bytes.fromhex("6a00ff15") + struct.pack("<I", _slot("KERNEL32.dll!ExitProcess")) + b"\xc3")
    r = machine.run(entry, case(), 100)
    assert r.outcome == "import"


def test_untouched_input_pages_are_not_reported(machine):
    # 0x004671B0 only computes a register; no arena/stack page content changes.
    arena = bytes(range(256)) * (emu.ARENA_SIZE // 256)
    r = machine.run(0x004671B0, emu.Case({"ecx": emu.ARENA, "eflags": 0x202}, [1, 2, 3], arena), 100)
    assert r.outcome == "return"
    assert not any(emu.ARENA <= p < emu.ARENA + emu.ARENA_SIZE for p in r.memory)


def test_changed_input_page_is_reported_against_case_content(machine):
    # 0x00412EF0 stores into [ecx]; the arena page must be reported because it changed.
    r = machine.run(0x00412EF0, emu.Case({"ecx": emu.ARENA + 0x2000, "eflags": 0x202}, [0] * 4, b"\xaa" * emu.ARENA_SIZE), 100)
    assert emu.ARENA + 0x2000 in r.memory


@pytest.mark.parametrize('entry,divide', [(0x0094DAC0, 0x0094DAD4), (0x0094ED10, 0x0094ED23)])
def test_repeated_divide_errors_match_fresh_machines(entry, divide):
    import unicorn

    reused = emu.Machine(image.primary(), watch=(entry, entry + 0x75))
    for numerator, flags in ((0, 0x202), (0, 0x603), (1, 0x202), (0xFFFFFFFF, 0x246)):
        incoming = emu.Case({'ebx': emu.STACK_TOP + 4, 'esi': 0xFFFFFFFF, 'eflags': flags},
                            [numerator, 0, 0, 0], bytes(emu.ARENA_SIZE))
        fresh = emu.Machine(image.primary(), watch=(entry, entry + 0x75))
        expected = fresh.run(entry, incoming, 1000)
        actual = reused.run(entry, incoming, 1000)
        assert expected.outcome == 'error'
        assert expected.detail == (unicorn.UC_ERR_EXCEPTION, divide)
        assert actual == expected

    incoming = emu.Case({'eflags': 0x202}, [21, 0, 3, 0], bytes(emu.ARENA_SIZE))
    fresh = emu.Machine(image.primary(), watch=(entry, entry + 0x75))
    actual = reused.run(entry, incoming, 1000)
    assert actual == fresh.run(entry, incoming, 1000)
    assert actual.outcome == 'return'
    assert actual.regs[0] == (7 if entry == 0x0094DAC0 else 0)


def test_cpu_reset_keeps_real_budget_stops_distinct_from_divide_errors():
    vm = emu.Machine(image.primary())
    # A genuine instruction-budget stop remains a budget stop after a CPU error.
    vm.uc.mem_map(SCRATCH, 0x1000)
    vm.uc.mem_write(SCRATCH, bytes.fromhex('ebfe'))  # jmp to itself
    zero = emu.Case({'eflags': 0x202}, [0] * 4, bytes(emu.ARENA_SIZE))
    assert vm.run(0x0094DAC0, zero, 1000).outcome == 'error'
    result = vm.run(SCRATCH, case(), 10)
    assert result.outcome == 'budget'
    assert result.detail == (SCRATCH,)


def test_original_fpcw_is_an_explicit_fixture_input_and_resets(monkeypatch):
    import json
    from pathlib import Path
    from cmre import difftest, env

    va = 0x006EC010
    captured = json.loads((Path(__file__).parent / 'fixtures/fpcw/fn_006ec010.json').read_text())
    assert captured['target_sha256'] == env.TARGET_SHA256
    fixture = captured['fixture']
    templates = difftest.arena_templates()
    default = difftest._fixture_case(fixture, templates, 10)
    supplied = difftest._fixture_case({**fixture, 'fpcw': 0x007F}, templates, 10)
    vm = emu.Machine(image.primary(), watch=(va, 0x006EECAA))
    # Original code observes the incoming control word. Neither outcome is
    # replaced by a model of native RaiseException or a default FP mask.
    first = vm.run(va, default, 200000)
    assert first.outcome == 'import'
    assert first.detail == ('KERNEL32.dll!RaiseException',)
    assert vm.run(va, supplied, 200000).outcome == 'return'
    after = vm.run(va, default, 200000)
    fresh = emu.Machine(image.primary(), watch=(va, 0x006EECAA))
    assert after == first == fresh.run(va, default, 200000)

    explanations = []
    for incoming in (default, supplied):
        monkeypatch.setattr(difftest, 'make_case', lambda *args, incoming=incoming: incoming)
        explanations.append(difftest.explain(va, (va, 0x006EECAA), set(), None, {},
                                            difftest.Options.for_function(va), 'exact', 0))
    assert explanations[0]['regs_in'] == explanations[1]['regs_in']
    assert explanations[0]['stack_in'] == explanations[1]['stack_in']
    assert explanations[0]['fpcw_in'] is None
    assert explanations[1]['fpcw_in'] == 0x007F
    assert [item['original']['outcome'] for item in explanations] == ['import', 'return']


INTERIOR = 0x00873401  # inside an original routine, not a function entry
ENTRY = 0x004671B0     # lea eax, [ecx + 0x20]; ret


def slot_machine(code: bytes, words: int = 2):
    """A machine stopping before the indirect CALL/JMP at SCRATCH + offset sites."""
    m = emu.Machine(image.primary())
    m.uc.mem_map(SCRATCH, 0x1000)
    m.uc.mem_write(SCRATCH, code)
    return m


def call_eax(target: int) -> bytes:
    # mov eax, target; push 7; mov ecx, 0x1234; call eax; add esp, 4; ret
    return (b"\xb8" + target.to_bytes(4, "little") + b"\x6a\x07\xb9\x34\x12\x00\x00\xff\xd0\x83\xc4\x04\xc3")


CALL_SITE = SCRATCH + 12


@pytest.mark.parametrize("target, outcome", [
    (INTERIOR, "interior"),
    (0x00000010, "bad-target"),       # no code there
    (emu.ARENA, "bad-target"),        # data, not code
])
def test_indirect_call_to_an_uncomparable_target_stops_before_the_call(target, outcome):
    m = slot_machine(call_eax(target))
    m.stop_indirect({CALL_SITE: None}, frozenset({ENTRY}), ((SCRATCH, SCRATCH + 0x1000),), 2)
    r = m.run(SCRATCH, case(), 100)
    assert r.outcome == outcome
    target_, site, ecx, words, shape = r.detail
    assert (target_, site, ecx, words[0], shape) == (target, CALL_SITE, 0x1234, 7, None)


@pytest.mark.parametrize("target", [ENTRY, SCRATCH + 0x40])
def test_indirect_call_to_an_entry_or_own_code_runs(target):
    code = call_eax(target)
    m = slot_machine(code + bytes(0x40 - len(code)) + b"\xc3")
    m.stop_indirect({CALL_SITE: None}, frozenset({ENTRY}), ((SCRATCH, SCRATCH + 0x1000),), 2)
    assert m.run(SCRATCH, case(), 100).outcome == "return"


def test_indirect_call_through_memory_reads_the_slot():
    # mov eax, [ecx]; push 1; call [eax + 0x3c]; ret  -- the vtable aliases the object
    m = slot_machine(bytes.fromhex("8b016a01ff503cc3"))
    m.stop_indirect({SCRATCH + 4: None}, frozenset(), ((SCRATCH, SCRATCH + 0x1000),), 1)
    arena = bytearray(emu.ARENA_SIZE)
    arena[0:4] = emu.ARENA.to_bytes(4, "little")
    arena[0x3c:0x40] = INTERIOR.to_bytes(4, "little")
    r = m.run(SCRATCH, emu.Case({"eflags": 0x202, "ecx": emu.ARENA}, [0] * 4, bytes(arena)), 100)
    assert (r.outcome, r.detail[0], r.detail[3]) == ("interior", INTERIOR, (1,))


def test_unreadable_slot_still_faults_on_the_read():
    m = slot_machine(bytes.fromhex("8b016a01ff503cc3"))
    m.stop_indirect({SCRATCH + 4: None}, frozenset(), ((SCRATCH, SCRATCH + 0x1000),), 1)
    r = m.run(SCRATCH, case(ecx=emu.ARENA), 100)  # zero vtable pointer: [0x3c] is unmapped
    assert r.outcome == "fault" and r.detail[0][:2] == (19, 0x3c)


def test_tail_jump_arguments_start_above_the_return_address():
    # push 9; push 0x55; jmp eax  (the 0x55 stands in for a return address)
    m = slot_machine(bytes.fromhex("6a096a55ffe0"))
    m.stop_indirect({SCRATCH + 4: None}, frozenset(), ((SCRATCH, SCRATCH + 0x1000),), 1)
    r = m.run(SCRATCH, case(eax=INTERIOR), 100)
    assert (r.outcome, r.detail[3]) == ("interior", (9,))


def test_without_indirect_stops_the_interior_target_executes():
    m = slot_machine(call_eax(INTERIOR))
    assert m.run(SCRATCH, case(), 100).outcome != "interior"
