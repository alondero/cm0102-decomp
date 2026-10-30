"""Normalization maps recovered-image addresses back to original VAs, and nothing else."""
import struct

import pytest

from cmre import difftest, emu, env

LABEL = env.LINK_BASE + 0x1010


def amap():
    return emu.AddressMap({"v_00450a70": LABEL, "v_00450a73": LABEL + 3}, {}, env.LINK_BASE + 0x1000, env.LINK_BASE + 0x2000)


def result(outcome, detail):
    return emu.Result(outcome, detail, (0,) * 9, [], {}, set())


def test_budget_stop_inside_recovered_code_is_mapped():
    n = difftest._normalize(result("budget", (LABEL + 3,)), amap(), "exact")
    assert n[1] == (0x00450A73,)


def test_fault_access_address_and_eip_are_mapped():
    n = difftest._normalize(result("fault", ((21, LABEL + 3, 4), LABEL)), amap(), "exact")
    assert n[1] == ((21, 0x00450A73, 4), 0x00450A70)


def test_coincidental_value_equal_to_recovered_label_is_not_rewritten():
    # Random data equal on both sides must compare equal even if it looks like a recovered address.
    a = emu.Result("return", (), (LABEL,) + (0,) * 8, [("r", emu.ARENA, 4, LABEL)], {}, set())
    b = emu.Result("return", (), (LABEL,) + (0,) * 8, [("r", emu.ARENA, 4, LABEL)], {}, set())
    m = amap()
    assert difftest._diff(difftest._normalize(a, None, "exact"), difftest._normalize(b, m, "exact"), m) == {}


def test_recovered_return_address_matches_original():
    m = emu.AddressMap({}, {LABEL + 5: 0x00450A75}, env.LINK_BASE + 0x1000, env.LINK_BASE + 0x2000)
    a = emu.Result("return", (), (0,) * 9, [("w", emu.STACK, 4, 0x00450A75)], {}, set())
    b = emu.Result("return", (), (0,) * 9, [("w", emu.STACK, 4, LABEL + 5)], {}, set())
    assert difftest._diff(difftest._normalize(a, None, "exact"), difftest._normalize(b, m, "exact"), m) == {}


def test_link_base_is_outside_small_value_byte_patterns():
    # Bytes 00..03, 7f, 80 and ff dominate the arena; the image must not start with them.
    assert (env.LINK_BASE >> 24) not in (0x00, 0x01, 0x02, 0x03, 0x7F, 0x80, 0xFF)


def test_return_address_copied_bytewise_matches_original():
    # A callee copying dead stack bytes one at a time sees the return address in pieces.
    m = emu.AddressMap({}, {LABEL + 5: 0x00450A75}, env.LINK_BASE + 0x1000, env.LINK_BASE + 0x2000)
    rec_ret = LABEL + 5
    a_events = [("w", emu.STACK, 4, 0x00450A75)] + [("r", emu.STACK + i, 1, (0x00450A75 >> 8 * i) & 0xFF) for i in range(4)]
    b_events = [("w", emu.STACK, 4, rec_ret)] + [("r", emu.STACK + i, 1, (rec_ret >> 8 * i) & 0xFF) for i in range(4)]
    a = emu.Result("return", (), (0,) * 9, a_events, {}, set())
    b = emu.Result("return", (), (0,) * 9, b_events, {}, set())
    assert difftest._diff(difftest._normalize(a, None, "exact"), difftest._normalize(b, m, "exact"), m) == {}


def test_bytewise_tolerance_needs_a_pushed_return_address():
    m = emu.AddressMap({}, {LABEL + 5: 0x00450A75}, env.LINK_BASE + 0x1000, env.LINK_BASE + 0x2000)
    a = emu.Result("return", (), (0,) * 9, [("r", emu.STACK, 1, 0x75)], {}, set())
    b = emu.Result("return", (), (0,) * 9, [("r", emu.STACK, 1, (LABEL + 5) & 0xFF)], {}, set())
    assert difftest._diff(difftest._normalize(a, None, "exact"), difftest._normalize(b, m, "exact"), m) != {}


@pytest.mark.parametrize("offset", [1, 2, 3])
def test_unaligned_code_pointer_store_matches_final_memory(offset):
    # fn_00729ff0 case 7 stores a relocated label at arena VA 0x03023009.
    # The store agrees in the event trace; its final bytes must agree too.
    original = bytearray(b"\xa8" * 12)
    recovered = bytearray(original)
    struct.pack_into("<I", original, offset, 0x00450A70)
    struct.pack_into("<I", recovered, offset, LABEL)
    a = emu.Result("return", (), (0,) * 9,
                   [("w", emu.ARENA + offset, 4, 0x00450A70)],
                   {emu.ARENA: bytes(original)}, set())
    b = emu.Result("return", (), (0,) * 9,
                   [("w", emu.ARENA + offset, 4, LABEL)],
                   {emu.ARENA: bytes(recovered)}, set())
    m = amap()
    assert difftest._diff(difftest._normalize(a, None, "exact"),
                          difftest._normalize(b, m, "exact"), m) == {}


@pytest.mark.parametrize("offset", [1, 2, 3])
def test_unaligned_code_pointer_cannot_hide_an_adjacent_changed_byte(offset):
    original = bytearray(b"\xa8" * 12)
    recovered = bytearray(original)
    struct.pack_into("<I", original, offset, 0x00450A70)
    struct.pack_into("<I", recovered, offset, LABEL)
    recovered[offset + 4] ^= 1
    assert not difftest._equal_blob(bytes(original), bytes(recovered), amap())


@pytest.mark.parametrize("offset", [1, 2, 3])
def test_unaligned_pointer_needs_an_exact_original_address(offset):
    original = bytearray(b"\xa8" * 12)
    recovered = bytearray(original)
    struct.pack_into("<I", original, offset, 0x00450A71)
    struct.pack_into("<I", recovered, offset, LABEL)
    assert not difftest._equal_blob(bytes(original), bytes(recovered), amap())


def test_abi_profile_ignores_writes_below_final_esp():
    # -O0 frames write below the final ESP; that dead stack must not count as a change.
    page = emu.STACK_TOP - 0x1000
    pristine = {page: bytes(0x1000)}
    dirty = bytearray(0x1000)
    dirty[0xF00:0xF10] = b"\x55" * 16
    regs = (0, 0, 0, 0, emu.STACK_TOP + 4, 0, 0, 0, 0x202)
    a = emu.Result("return", (), regs, [], {}, set(), pristine)
    b = emu.Result("return", (), regs, [], {page: bytes(dirty)}, set(), pristine)
    assert difftest._diff(difftest._normalize(a, None, "abi"), difftest._normalize(b, None, "abi")) == {}
