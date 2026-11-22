"""File-search and file-time import stubs write the documented out-parameters.

Without these writes the original and recovered runs read different stack garbage
from the same buffer and branch apart (parked file-search functions)."""
import struct

from cmre import imports

K = "KERNEL32.dll!"
NOTHING = lambda address, size: None  # noqa: E731


def test_find_data_is_valid_and_deterministic():
    blob = imports.find_data(7, 3)
    assert len(blob) == imports.FIND_DATA_SIZE == 320
    assert blob == imports.find_data(7, 3)
    assert blob != imports.find_data(7, 4)
    attrs, = struct.unpack_from("<I", blob, 0)
    assert attrs in (0x10, 0x20, 0x01, 0x80)
    for offset in (4, 12, 20):
        ft, = struct.unpack_from("<Q", blob, offset)
        assert imports.systemtime(ft) is not None
    name = blob[44:44 + 260]
    assert b"\0" in name and name.index(b"\0") <= 12
    assert blob[44 + 260:] == bytes(320 - 44 - 260)  # cAlternateFileName empty


def test_find_calls_write_the_buffer_only_on_success():
    first = imports.outputs(K + "FindFirstFileA", (0x1000, 0x2000), 0x2345, 1, 5, NOTHING)
    assert first == [(0x2000, imports.find_data(1, 5))]
    nxt = imports.outputs(K + "FindNextFileA", (0x2345, 0x2000), 1, 1, 6, NOTHING)
    assert nxt == [(0x2000, imports.find_data(1, 6))]
    # Near misses: the documented failure results leave the buffer alone.
    assert imports.outputs(K + "FindFirstFileA", (0x1000, 0x2000), 0xFFFFFFFF, 1, 5, NOTHING) == []
    assert imports.outputs(K + "FindNextFileA", (0x2345, 0x2000), 0, 1, 6, NOTHING) == []


def test_local_file_time_copies_a_valid_input():
    raw = struct.pack("<Q", 125911584000000000)
    assert imports.outputs(K + "FileTimeToLocalFileTime", (0x10, 0x20), 1, 0, 0,
                           lambda a, n: raw if (a, n) == (0x10, 8) else None) == [(0x20, raw)]


def test_local_file_time_near_misses_write_nothing():
    raw = struct.pack("<Q", 1 << 63)
    assert imports.outputs(K + "FileTimeToLocalFileTime", (0x10, 0x20), 1, 0, 0, lambda a, n: raw) == [(0x20, raw)]
    assert imports.outputs(K + "FileTimeToLocalFileTime", (0x10, 0x20), 0, 0, 0, lambda a, n: bytes(8)) == []
    assert imports.outputs(K + "FileTimeToLocalFileTime", (0x10, 0x20), 1, 0, 0, NOTHING) == []


def test_system_time_converts_exactly():
    # 2000-01-01 00:00:00.000 UTC was a Saturday (wDayOfWeek 6).
    assert struct.unpack("<8H", imports.systemtime(125911584000000000)) == (2000, 1, 6, 1, 0, 0, 0, 0)
    # One day, 1 h 2 min 3 s 456 ms later.
    ticks = 125911584000000000 + 864000000000 + (3600 + 120 + 3) * 10000000 + 456 * 10000
    assert struct.unpack("<8H", imports.systemtime(ticks)) == (2000, 1, 0, 2, 1, 2, 3, 456)
    assert struct.unpack("<8H", imports.systemtime(0)) == (1601, 1, 1, 1, 0, 0, 0, 0)


def test_system_time_rejects_out_of_range_filetimes():
    assert imports.systemtime(0x7FFF35F4F06C58F0) is not None
    assert struct.unpack("<8H", imports.systemtime(0x7FFF35F4F06C58F0 + 864000000000))[:2] == (30828, 1)
    assert struct.unpack("<8H", imports.systemtime((1 << 63) - 1)) == (30828, 9, 4, 14, 2, 48, 5, 477)
    assert imports.systemtime(1 << 63) is None
    assert imports.systemtime(-1) is None


def test_file_time_to_system_time_uses_the_input_and_writes_sixteen_bytes():
    raw = struct.pack("<Q", 125911584000000000)
    out = imports.outputs(K + "FileTimeToSystemTime", (0x10, 0x20), 1, 0, 0, lambda a, n: raw)
    assert out == [(0x20, imports.systemtime(125911584000000000))] and len(out[0][1]) == 16
    bad = struct.pack("<Q", 1 << 63)
    assert imports.outputs(K + "FileTimeToSystemTime", (0x10, 0x20), 1, 0, 0, lambda a, n: bad) == []


def test_other_imports_write_nothing():
    for name in (K + "DeleteFileA", K + "GetLocalTime", K + "FindClose", K + "GetLastError"):
        assert imports.outputs(name, (1, 2), 1, 0, 0, lambda a, n: bytes(8)) == []


def test_system_time_handles_leap_days_and_the_documented_maximum():
    leap = 125911584000000000 + 59 * 864000000000
    assert struct.unpack("<8H", imports.systemtime(leap)) == (2000, 2, 2, 29, 0, 0, 0, 0)
    assert struct.unpack("<8H", imports.systemtime(leap + 864000000000)) == (2000, 3, 3, 1, 0, 0, 0, 0)
    assert struct.unpack("<8H", imports.systemtime(0x7FFF35F4F06C58F0))[:2] == (30827, 12)
