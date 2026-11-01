"""Read-hook allocation and observation equivalence, without changing guest execution."""
import ctypes
import struct

import pytest
import unicorn

from cmre import emu, image


CODE = 0x05000000


@pytest.fixture(scope="module")
def reader():
    return emu.Machine(image.primary())


def test_read_hook_does_not_allocate_a_host_buffer_for_every_access(reader, monkeypatch):
    data = bytes.fromhex("efcdab8967452301")
    reader.uc.mem_write(emu.ARENA, data)
    allocated = []
    create = ctypes.create_string_buffer

    def counted(*args, **kwargs):
        allocated.append(args)
        return create(*args, **kwargs)

    monkeypatch.setattr(ctypes, "create_string_buffer", counted)
    reader.events = []
    widths = (1, 2, 4, 8) * 25
    for width in widths:
        reader._access(reader.uc, unicorn.UC_MEM_READ, emu.ARENA, width, 0, None)
    assert reader.events == [("r", emu.ARENA, n, int.from_bytes(data[:n], "little"))
                             for n in widths]
    assert len(allocated) <= 4, "repeated reads must reuse host scratch storage"


@pytest.mark.parametrize("size", [0, 1, 2, 3, 4, 6, 8, 10, 16])
@pytest.mark.parametrize("offset", [0, 1, emu.PAGE - 3, emu.PAGE - 1])
def test_read_values_match_public_binding_at_each_width_and_alignment(reader, size, offset):
    address = emu.ARENA + offset
    for data in (bytes(16), b"\xff" * 16, bytes(range(0xf0, 0x100))):
        reader.uc.mem_write(address, data)
        expected = int.from_bytes(reader.uc.mem_read(address, size), "little")
        assert reader._read_value(address, size) == expected


@pytest.mark.parametrize("size", [1, 2, 3, 4, 8, 16])
@pytest.mark.parametrize("address", [0x10, emu.ARENA + emu.ARENA_SIZE - 1])
def test_read_failure_matches_public_binding(reader, size, address):
    try:
        expected = int.from_bytes(reader.uc.mem_read(address, size), "little")
    except unicorn.UcError as original:
        with pytest.raises(unicorn.UcError) as actual:
            reader._read_value(address, size)
        assert actual.value.errno == original.errno
        assert actual.value.args == original.args
        assert str(actual.value) == str(original)
    else:
        assert reader._read_value(address, size) == expected


def test_read_buffers_belong_to_each_machine_and_log_immutable_values():
    first, second = (emu.Machine(image.primary()) for _ in range(2))
    for vm, value in ((first, 0x89abcdef), (second, 0x76543210)):
        vm.uc.mem_write(emu.ARENA, struct.pack("<I", value))
        vm._access(vm.uc, unicorn.UC_MEM_READ, emu.ARENA, 4, 0, None)
    first.uc.mem_write(emu.ARENA, b"\xff" * 4)
    first._access(first.uc, unicorn.UC_MEM_READ, emu.ARENA, 4, 0, None)
    assert first.events == [("r", emu.ARENA, 4, 0x89abcdef),
                            ("r", emu.ARENA, 4, 0xffffffff)]
    assert second.events == [("r", emu.ARENA, 4, 0x76543210)]


class PublicReadMachine(emu.Machine):
    """Reference the allocation-heavy public read path used before this change."""

    def _read_value(self, address, size):
        return int.from_bytes(self.uc.mem_read(address, size), "little")


@pytest.mark.parametrize("code, regs, outcome", [
    # Read/write aliasing, mixed widths and a following read of the changed bytes.
    ("8b06894601668b068a060fb706c3", {"esi": emu.ARENA + 0xffe}, "return"),
    ("f3a4c3", {"esi": emu.ARENA + 0xffe, "edi": emu.ARENA + 0xfff,
                 "ecx": 12}, "return"),
    ("f3a4c3", {"esi": emu.ARENA + 0x1008, "edi": emu.ARENA + 0x1007,
                 "ecx": 12, "eflags": 0x602}, "return"),
    ("dd06ddd8c3", {"esi": emu.ARENA + 0xffd}, "return"),
    ("f30f6f06c3", {"esi": emu.ARENA + 0xffd}, "return"),
    # Observe code bytes, a protected store, and a read crossing into unmapped memory.
    ("a100000005c3", {}, "return"),
    ("a300104000c3", {"eax": 0x89abcdef}, "fault"),
    ("8b06c3", {"esi": emu.ARENA + emu.ARENA_SIZE - 1}, "fault"),
    ("8b06894601ebf9", {"esi": emu.ARENA + 0xffe}, "budget"),
])
@pytest.mark.parametrize("budget", [1, 2, 100])
def test_guest_results_match_public_reads_through_budget_fault_and_aliasing(code, regs, outcome, budget):
    blob = bytes.fromhex(code)
    incoming = emu.Case({"eflags": 0x202, **regs}, [0] * 4,
                        bytes(range(256)) * (emu.ARENA_SIZE // 256), import_seed=71)
    results = []
    for cls in (PublicReadMachine, emu.Machine):
        vm = cls(image.primary(), watch=(CODE, CODE + len(blob)))
        vm.uc.mem_map(CODE, emu.PAGE)
        vm.uc.mem_write(CODE, blob)
        vm.attribute_reads = True
        results.append(vm.run(CODE, incoming, budget))
        # Scratch storage also survives reset and reuse, just like the original read path.
        assert vm.run(CODE, incoming, budget) == results[-1]
    assert results[0] == results[1]
    assert set(results[0].read_sites) == {i for i, event in enumerate(results[0].events)
                                         if event[0] == "r"}
    if budget == 100:
        assert results[0].outcome == outcome
