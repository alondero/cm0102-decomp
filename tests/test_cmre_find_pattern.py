"""FindFirstFileA/FindNextFileA names match the caller's search pattern.

FindFirstFileA documents that it returns entries whose names match lpFileName,
and FindNextFileA continues that search. fn_005b2170 searches "*.tt?" and
branches on .ttf/.ttc/.t2k; names that ignore the pattern can never reach
those branches (revalidation 1795393926977889300: coverage 0.78 -> 0.34)."""
import fnmatch
import struct

from cmre import imports

K = "KERNEL32.dll!"
PATTERN_AT, BUFFER, HANDLE = 0x5000, 0x2000, 0x2345


def _memory(text: bytes, at: int = PATTERN_AT):
    data = text + b"\0"

    def read(address, size):
        if at <= address and address + size <= at + len(data):
            return data[address - at:address - at + size]
        return None
    return read


def _name(blob: bytes) -> bytes:
    raw = blob[44:44 + 260]
    return raw[:raw.index(b"\0")]


def _first(pattern: bytes, seed: int, index: int = 0, patterns=None):
    out = imports.outputs(K + "FindFirstFileA", (PATTERN_AT, BUFFER), HANDLE, seed, index,
                          _memory(pattern), {} if patterns is None else patterns)
    (address, blob), = out
    assert address == BUFFER and len(blob) == imports.FIND_DATA_SIZE
    return blob


def test_first_names_match_the_final_path_component():
    for seed in range(300):
        name = _name(_first(rb"C:\fonts\*.tt?", seed))
        assert fnmatch.fnmatchcase(name.decode("ascii"), "*.tt?"), name


def test_question_mark_completions_vary_so_each_extension_branch_is_reachable():
    endings = {_name(_first(rb"C:\fonts\*.tt?", seed))[-1:] for seed in range(600)}
    assert {b"f", b"c"} <= endings


def test_next_continues_the_search_opened_on_that_handle():
    patterns = {}
    _first(rb"data\*.sav", 3, 0, patterns)
    for index in range(1, 40):
        out = imports.outputs(K + "FindNextFileA", (HANDLE, BUFFER), 1, 3, index, _memory(b""), patterns)
        (_, blob), = out
        assert fnmatch.fnmatchcase(_name(blob).decode("ascii"), "*.sav")


def test_dot_entries_appear_only_when_the_pattern_matches_them():
    star = {_name(_first(b"dir\\*", seed)) for seed in range(200)}
    assert {b".", b".."} <= star
    fonts = {_name(_first(b"*.tt?", seed)) for seed in range(200)}
    assert not {b".", b".."} & fonts


def test_a_literal_pattern_returns_that_name():
    for seed in range(20):
        assert _name(_first(rb"C:\game\cm0102.cfg", seed)) == b"cm0102.cfg"


def test_names_stay_valid_find_data():
    for seed in range(50):
        blob = _first(b"*", seed)
        attrs, = struct.unpack_from("<I", blob, 0)
        assert attrs in (0x10, 0x20, 0x01, 0x80)
        assert blob[44 + 260:] == bytes(320 - 44 - 260)


def test_file_sizes_follow_the_stub_value_mix():
    # Like imports.result: 0, 1, small and full-range values all occur. Uniform
    # 32-bit sizes made almost every file gigabytes long, so readers such as
    # fn_00933610 only ever took their allocation-failure path.
    sizes = []
    for seed in range(400):
        blob = imports.find_data(seed, 0, b"*.dat")
        attrs, = struct.unpack_from("<I", blob, 0)
        high, low = struct.unpack_from("<II", blob, 28)
        assert high == 0
        if attrs != 0x10:
            sizes.append(low)
        else:
            assert low == 0
    assert 0 in sizes and 1 in sizes
    assert any(1 < s < 0x10000 for s in sizes)
    assert any(s >= 0x10000 for s in sizes)


def test_unreadable_pattern_or_unknown_handle_keeps_the_unconstrained_names():
    # Near misses: no readable pattern means no constraint to honour.
    out = imports.outputs(K + "FindFirstFileA", (PATTERN_AT, BUFFER), HANDLE, 1, 5,
                          lambda address, size: None, {})
    assert out == [(BUFFER, imports.find_data(1, 5))]
    out = imports.outputs(K + "FindNextFileA", (0x9999, BUFFER), 1, 1, 6, _memory(b""), {})
    assert out == [(BUFFER, imports.find_data(1, 6))]


def test_failed_calls_still_write_nothing_and_record_no_search():
    patterns = {}
    assert imports.outputs(K + "FindFirstFileA", (PATTERN_AT, BUFFER), 0xFFFFFFFF, 1, 5,
                           _memory(b"*.tt?"), patterns) == []
    assert patterns == {}
    assert imports.outputs(K + "FindNextFileA", (HANDLE, BUFFER), 0, 1, 6, _memory(b""), patterns) == []


class _FakeUc:
    def __init__(self, memory):
        self.memory = memory

    def mem_read(self, address, size):
        import unicorn
        if all(address + i in self.memory for i in range(size)):
            return bytes(self.memory[address + i] for i in range(size))
        raise unicorn.UcError(unicorn.UC_ERR_READ_UNMAPPED)


def test_the_emulator_continues_each_search_with_its_own_pattern():
    from types import SimpleNamespace
    from cmre import emu
    memory = {PATTERN_AT + i: b for i, b in enumerate(rb"C:\fonts\*.tt?" + b"\0")}
    writes = []
    fake = SimpleNamespace(uc=_FakeUc(memory), import_seed=11, import_count=0, find_patterns={},
                           _transcript_write=lambda address, blob: writes.append((address, blob)))
    emu.Machine._stub_outputs(fake, K + "FindFirstFileA", (PATTERN_AT, BUFFER), HANDLE)
    for count in range(1, 30):
        fake.import_count = count
        emu.Machine._stub_outputs(fake, K + "FindNextFileA", (HANDLE, BUFFER), 1)
    assert fake.find_patterns == {HANDLE: b"*.tt?"}
    assert len(writes) == 30
    assert all(fnmatch.fnmatchcase(_name(blob).decode("ascii"), "*.tt?") for _, blob in writes)
