"""ReadFile/WriteFile stubs define the byte-count out-parameter (and ReadFile's buffer).

Windows documents that ReadFile and WriteFile set *lpNumberOfBytesRead/Written to
zero before any work, that a synchronous success transfers 0..nNumberOfBytes
bytes (0 from ReadFile = end of file), and that ReadFile fills lpBuffer with the
bytes it reports. Without these writes callers read stale stack, which differs
between the original and the recovered build (fn_00933610 seeds 3, 117, 286)."""
import struct
from types import SimpleNamespace

from cmre import emu, imports

K = "KERNEL32.dll!"
COUNT, BUF, HANDLE = 0x3000, 0x4000, 0x77


def _read_args(n, count=COUNT, overlapped=0):
    return (HANDLE, BUF, n, count, overlapped)


def _mem(read_ok=True):
    def read(address, size):
        return bytes(size) if read_ok else None
    return read


def _call(name, args, value, seed, index=0, read=None):
    return imports.outputs(name, args, value, seed, index, read or _mem())


def _count(out):
    (address, blob), = [w for w in out if w[0] == COUNT]
    assert len(blob) == 4
    return struct.unpack("<I", blob)[0]


def test_read_success_writes_count_within_bounds_and_exactly_that_many_bytes():
    for seed in range(300):
        for n in (0, 1, 7, 4096, 100000):
            out = _call(K + "ReadFile", _read_args(n), 1, seed)
            got = _count(out)
            assert 0 <= got <= min(n, 65536)
            buf = [blob for address, blob in out if address == BUF]
            if got:
                (blob,) = buf
                assert len(blob) == got
            else:
                assert buf == []
            assert [a for a, _ in out if a not in (COUNT, BUF)] == []


def test_read_mix_reaches_eof_one_byte_and_full_size():
    counts = {_count(_call(K + "ReadFile", _read_args(512), 1, seed)) for seed in range(400)}
    assert {0, 1, 512} <= counts
    assert any(1 < c < 512 for c in counts)


def test_read_larger_than_cap_picks_a_count_inside_the_cap():
    counts = {_count(_call(K + "ReadFile", _read_args(1 << 30), 5, seed)) for seed in range(400)}
    assert max(counts) == 65536 and all(c <= 65536 for c in counts)


def test_read_buffer_bytes_are_seeded_not_constant():
    blobs = set()
    for seed in range(50):
        out = _call(K + "ReadFile", _read_args(64), 1, seed)
        blobs.update(blob for a, blob in out if a == BUF)
    assert len(blobs) > 10


def test_read_failure_writes_zero_count_only():
    out = _call(K + "ReadFile", _read_args(512), 0, 9)
    assert out == [(COUNT, b"\0\0\0\0")]


def test_read_writes_nothing_for_null_count_or_overlapped_success():
    assert _call(K + "ReadFile", _read_args(512, count=0), 1, 9) == []
    assert _call(K + "ReadFile", _read_args(512, count=0), 0, 9) == []
    assert _call(K + "ReadFile", _read_args(512, overlapped=0x5000), 1, 9) == []


def test_read_writes_nothing_when_the_count_pointer_is_unreadable():
    assert _call(K + "ReadFile", _read_args(512), 1, 9, read=_mem(False)) == []


def test_read_is_deterministic_and_varies_with_index():
    a = _call(K + "ReadFile", _read_args(512), 1, 4, 3)
    assert a == _call(K + "ReadFile", _read_args(512), 1, 4, 3)
    assert len({_count(_call(K + "ReadFile", _read_args(512), 1, 4, i)) for i in range(60)}) > 1


def test_write_success_writes_everything_and_failure_zero():
    for requested in (0, 1, 300, 1 << 30):
        args = (HANDLE, BUF, requested, COUNT, 0)
        for s in range(50):
            assert _call(K + "WriteFile", args, 1, s) == [(COUNT, requested.to_bytes(4, "little"))]
    args = (HANDLE, BUF, 300, COUNT, 0)
    assert _call(K + "WriteFile", args, 0, 3) == [(COUNT, b"\0\0\0\0")]
    assert _call(K + "WriteFile", (HANDLE, BUF, 300, 0, 0), 1, 3) == []
    assert _call(K + "WriteFile", (HANDLE, BUF, 300, COUNT, 0x5000), 1, 3) == []


def test_failure_zeroes_the_count_even_with_overlapped():
    for name in ("ReadFile", "WriteFile"):
        out = _call(K + name, (HANDLE, BUF, 300, COUNT, 0x5000), 0, 3)
        assert out == [(COUNT, b"\0\0\0\0")]
        assert _call(K + name, (HANDLE, BUF, 300, 0, 0x5000), 0, 3) == []


def test_read_with_unmapped_buffer_still_writes_a_zero_count():
    def read(address, size):
        return bytes(size) if COUNT <= address < COUNT + 4 else None
    for seed in range(100):
        out = _call(K + "ReadFile", _read_args(512), 1, seed, read=read)
        assert out == [(COUNT, b"\0\0\0\0")]


class _FakeUc:
    def mem_read(self, address, size):
        import unicorn
        if COUNT <= address and address + size <= COUNT + 4 or BUF <= address and address + size <= BUF + 0x20000:
            return bytes(size)
        raise unicorn.UcError(unicorn.UC_ERR_READ_UNMAPPED)


def test_the_emulator_applies_read_file_outputs():
    writes = []
    fake = SimpleNamespace(uc=_FakeUc(), import_seed=3, import_count=0, find_patterns={},
                           _transcript_write=lambda address, blob: writes.append((address, blob)))
    emu.Machine._stub_outputs(fake, K + "ReadFile", _read_args(256), 1)
    count = struct.unpack("<I", dict(writes)[COUNT])[0]
    assert 0 <= count <= 256
    assert sum(len(b) for a, b in writes if a == BUF) == count
