"""A stack-probe fault below the test stack: only the frame layout decides the exact address.

The original stack-probe helper (0x00945610..0x0094563E) reads one dword per page
below the incoming ESP. When the frame exceeds the test stack, the first unmapped
read lands in the page just below it, at an offset that depends on how deep the
caller's frame was (compiler layout). Both sides fault in the same original
instructions with the same access class and width; only that offset differs.
"""
from cmre import difftest, emu, semantic

PROBE_READ = semantic.STACK_PROBE + 0x17
PROBE_RET = semantic.STACK_PROBE_RET
PAGE = 0x1000
BELOW = emu.STACK - 0x100  # inside the page below the stack


def fault(access, address, size, eip):
    return emu.Result("fault", ((access, address, size), eip), (0,) * 9, [], {}, set())


def diff(a, b):
    na = difftest._normalize(a, None, "semantic", "int")
    nb = difftest._normalize(b, None, "semantic", "int")
    return difftest._diff(na, nb)


def test_probe_read_below_stack_with_other_frame_depth_is_equal():
    a = fault(19, BELOW - 0x1C, 4, PROBE_READ)
    b = fault(19, BELOW - 0x44, 4, PROBE_READ)
    assert diff(a, b) == {}


def test_probe_return_push_write_below_stack_is_equal():
    a = fault(20, emu.STACK - 8, 4, PROBE_RET)
    b = fault(20, emu.STACK - 0x40, 4, PROBE_RET)
    assert diff(a, b) == {}


def test_page_boundary_just_inside_the_guard_page_is_equal():
    a = fault(19, emu.STACK - PAGE - 4, 4, PROBE_READ)
    b = fault(19, emu.STACK - 4, 4, PROBE_READ)
    assert diff(a, b) == {}


def test_near_miss_address_two_pages_below_still_differs():
    a = fault(19, BELOW, 4, PROBE_READ)
    b = fault(19, BELOW - 2 * PAGE, 4, PROBE_READ)
    assert diff(a, b)["field"] == "detail"


def test_near_miss_fault_outside_the_probe_still_differs():
    a = fault(19, BELOW - 0x1C, 4, 0x00401000)
    b = fault(19, BELOW - 0x44, 4, 0x00401000)
    assert diff(a, b)["field"] == "detail"


def test_near_miss_recovered_fault_outside_the_probe_still_differs():
    a = fault(19, BELOW - 0x1C, 4, PROBE_READ)
    b = fault(19, BELOW - 0x44, 4, 0x00401000)
    assert diff(a, b)["field"] == "detail"


def test_near_miss_access_class_still_differs():
    a = fault(19, BELOW - 0x1C, 4, PROBE_READ)
    b = fault(20, BELOW - 0x44, 4, PROBE_READ)
    assert diff(a, b)["field"] == "detail"


def test_near_miss_width_still_differs():
    a = fault(19, BELOW - 0x1C, 4, PROBE_READ)
    b = fault(19, BELOW - 0x44, 2, PROBE_READ)
    assert diff(a, b)["field"] == "detail"


def test_near_miss_probe_fault_against_a_far_address_still_differs():
    a = fault(19, BELOW - 0x1C, 4, PROBE_READ)
    b = fault(19, 0x1000, 4, PROBE_READ)
    assert diff(a, b)["field"] == "detail"


def test_near_miss_address_above_the_stack_is_not_a_guard_fault():
    a = fault(19, emu.STACK + emu.STACK_SIZE + 8, 4, PROBE_READ)
    b = fault(19, emu.STACK + emu.STACK_SIZE + 32, 4, PROBE_READ)
    assert diff(a, b)["field"] == "detail"


def test_equal_raw_addresses_remain_equal_outside_the_probe():
    a = fault(19, 0x1234, 4, 0x00401000)
    assert diff(a, fault(19, 0x1234, 4, 0x00401000)) == {}
