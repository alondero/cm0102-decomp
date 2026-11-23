"""FindFirstFileA/FindNextFileA names match the caller's search pattern.

FindFirstFileA documents that it returns entries whose names match lpFileName,
and FindNextFileA continues that search. fn_005b2170 searches "*.tt?" and
branches on .ttf/.ttc/.t2k; names that ignore the pattern can never reach
those branches (revalidation 1795393926977889300: coverage 0.78 -> 0.34)."""
import fnmatch
import struct

from cmre import emu, image, imports

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


PROPERTY_PATTERNS = [b"*", b"*.*", b"*.", b"?", b"*.tt?", b"dir\\*", b"a*b", b"x?y*"]
PROPERTY_SEEDS = range(2000)
DOT_ENTRIES = (b".", b"..")


def _entry(pattern, seed, index=0):
    """(name, attributes, nFileSizeLow) of the first entry for a pattern."""
    blob = _first(pattern, seed, index)
    attrs, = struct.unpack_from("<I", blob, 0)
    _, size_low = struct.unpack_from("<II", blob, 28)
    return _name(blob), attrs, size_low


def _match_pattern(pattern):
    """The final component with trailing dots and spaces removed, as Windows matches it."""
    return pattern.rstrip(b". ").rsplit(b"\\", 1)[-1].decode("latin-1")


def test_names_are_never_empty():
    for pattern in PROPERTY_PATTERNS:
        for seed in PROPERTY_SEEDS:
            name, _, _ = _entry(pattern, seed)
            assert name, (pattern, seed)


def test_names_are_at_most_255_bytes():
    for pattern in PROPERTY_PATTERNS:
        for seed in PROPERTY_SEEDS:
            name, _, _ = _entry(pattern, seed)
            assert len(name) <= 255, (pattern, seed, name)


def test_names_never_end_in_a_dot_or_space_unless_they_are_dot_entries():
    for pattern in PROPERTY_PATTERNS:
        for seed in PROPERTY_SEEDS:
            name, _, _ = _entry(pattern, seed)
            if name not in DOT_ENTRIES:
                assert name[-1:] not in (b".", b" "), (pattern, seed, name)


def test_names_contain_no_illegal_characters():
    for pattern in PROPERTY_PATTERNS:
        for seed in PROPERTY_SEEDS:
            name, _, _ = _entry(pattern, seed)
            assert not set(name) & set(b'<>":|\\/'), (pattern, seed, name)
            assert min(name) >= 0x20, (pattern, seed, name)


def test_dot_entries_are_always_directories_of_size_zero():
    for pattern in PROPERTY_PATTERNS:
        for seed in PROPERTY_SEEDS:
            name, attrs, size = _entry(pattern, seed)
            if name in DOT_ENTRIES:
                assert (attrs, size) == (0x10, 0), (pattern, seed, name)


def test_names_match_the_trailing_dot_stripped_pattern():
    for pattern in PROPERTY_PATTERNS:
        expected = _match_pattern(pattern).lower()
        for seed in PROPERTY_SEEDS:
            name, _, _ = _entry(pattern, seed)
            assert fnmatch.fnmatchcase(name.decode("latin-1").lower(), expected), (pattern, seed, name)


def test_patterns_with_dos_illegal_characters_are_unconstrained():
    for pattern in (b"a<*", b'a"*', b"a|*", b"a>*", b"a\x01*", b"a\x1f*"):
        for seed in range(50):
            assert _first(pattern, seed) == imports.find_data(seed, 0), (pattern, seed)


def test_pattern_ending_in_a_separator_is_unconstrained():
    for pattern in (b"C:\\dir\\", b"C:/dir/"):
        for seed in range(50):
            assert _first(pattern, seed) == imports.find_data(seed, 0), (pattern, seed)


def test_pattern_of_260_bytes_or_more_is_unconstrained():
    for pattern in (b"a*" * 130, b"a*" * 150):
        for seed in range(50):
            assert _first(pattern, seed) == imports.find_data(seed, 0), (pattern, seed)


def test_unterminated_pattern_is_unconstrained_without_crashing():
    raw = b"*.tt?"

    def read(address, size):
        if PATTERN_AT <= address and address + size <= PATTERN_AT + len(raw):
            return raw[address - PATTERN_AT:address - PATTERN_AT + size]
        return None
    for seed in range(20):
        out = imports.outputs(K + "FindFirstFileA", (PATTERN_AT, BUFFER), HANDLE, seed, 0, read, {})
        assert out == [(BUFFER, imports.find_data(seed, 0))]


def test_names_longer_than_255_fall_back_to_unconstrained():
    pattern = b"*" * 100 + b"?.dat"
    for seed in range(100):
        blob = _first(pattern, seed)
        name = _name(blob)
        assert len(name) <= 255, (seed, name)
        assert blob == imports.find_data(seed, 0) or fnmatch.fnmatchcase(name.decode("ascii"), "*" * 100 + "?.dat")


def test_a_reused_handle_with_an_unreadable_pattern_drops_the_stale_pattern():
    patterns = {}
    imports.outputs(K + "FindFirstFileA", (PATTERN_AT, BUFFER), HANDLE, 3, 0, _memory(rb"*.tt?"), patterns)
    assert patterns == {HANDLE: b"*.tt?"}
    imports.outputs(K + "FindFirstFileA", (PATTERN_AT, BUFFER), HANDLE, 3, 1, NOTHING_READABLE, patterns)
    assert patterns == {}
    for index in range(2, 30):
        (_, blob), = imports.outputs(K + "FindNextFileA", (HANDLE, BUFFER), 1, 3, index, _memory(b""), patterns)
        assert blob == imports.find_data(3, index)


def test_question_mark_patterns_never_list_dot_entries():
    names = {_entry(b"?", seed)[0] for seed in range(400)}
    assert not set(DOT_ENTRIES) & names


def test_patterns_starting_with_a_literal_dot_may_list_dot_entries():
    names = {_entry(b".*", seed)[0] for seed in range(200)}
    assert set(DOT_ENTRIES) <= names


def test_output_depends_only_on_seed_index_and_pattern():
    first = [_entry(b"*.*", seed, index) for seed in range(20) for index in range(5)]
    for seed in reversed(range(20)):
        for index in range(5):
            imports.find_data(99, index, b"*.tt?")
    assert first == [_entry(b"*.*", seed, index) for seed in range(20) for index in range(5)]


def test_machine_starts_with_no_find_patterns():
    from types import SimpleNamespace
    page = 0x1000
    primary = SimpleNamespace(base=0x400000, headers=bytes(page), imports={},
                              sections=[image.Section(".text", 0x401000, page, b"\xc3", 0x60000000)])
    assert emu.Machine(primary).find_patterns == {}


NOTHING_READABLE = lambda address, size: None  # noqa: E731

