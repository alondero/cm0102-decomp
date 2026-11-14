"""Captured entry inputs (ADR 0009). Authored instructions; no supplied executable required."""
import inspect
import random
from types import SimpleNamespace

import pytest

from cmre import capture, capture_expand, difftest, emu, env, image, recover

PAGE = 0x1000
HEAP = 0x05000000
NAME = 'KERNEL32.dll!GetLastError'


def _image(code: bytes, imports=None):
    return SimpleNamespace(base=0x400000, headers=bytes(PAGE), imports=imports or {},
                           sections=[image.Section('.text', 0x401000, PAGE, code, 0x60000000),
                                     image.Section('.data', 0x402000, PAGE, bytes(PAGE), 0xc0000000)])


def _case(regs=None, pages=None):
    return emu.Case({'eflags': 0x202, **(regs or {})}, [], bytes(emu.ARENA_SIZE), pages=pages)


def test_case_pages_exist_for_one_case_only():
    machine = emu.Machine(_image(bytes.fromhex('8b01c3')))  # mov eax,[ecx]; ret
    page = (0x11223344).to_bytes(4, 'little') * (PAGE // 4)
    with_page = machine.run(0x401000, _case({'ecx': HEAP + 0x10}, {HEAP: page}), 20)
    assert with_page.outcome == 'return' and with_page.regs[0] == 0x11223344
    without = machine.run(0x401000, _case({'ecx': HEAP + 0x10}), 20)
    assert without.outcome == 'fault' and without.detail[0][1] == HEAP + 0x10
    assert HEAP not in machine.pristine


def test_case_page_writes_are_observed_and_reset():
    machine = emu.Machine(_image(bytes.fromhex('8b118901c3')))  # mov edx,[ecx]; mov [ecx],eax; ret
    case = _case({'ecx': HEAP, 'eax': 0xAABBCCDD}, {HEAP: bytes(PAGE)})
    first = machine.run(0x401000, case, 20)
    assert first.memory[HEAP][:4] == bytes.fromhex('ddccbbaa')
    again = machine.run(0x401000, case, 20)
    assert again.regs[2] == 0 and again.memory == first.memory  # the earlier store did not survive


@pytest.mark.parametrize('pages', [
    {emu.STACK: bytes(PAGE)},      # harness stack
    {emu.ARENA: bytes(PAGE)},      # harness arena
    {0x401000: bytes(PAGE)},       # code
    {HEAP + 4: bytes(PAGE)},       # unaligned
    {HEAP: bytes(PAGE - 1)},       # short
])
def test_case_pages_never_replace_code_or_harness_memory(pages):
    machine = emu.Machine(_image(b'\xc3'))
    with pytest.raises(ValueError, match='case page'):
        machine.run(0x401000, _case(pages=pages), 20)
    assert machine.run(0x401000, _case(), 20).outcome == 'return'  # the machine stays usable


def test_writable_image_page_is_replaced_but_import_slots_keep_their_stubs():
    # call [0x402000]; mov edx,[0x402010]; ret
    code = bytes.fromhex('ff1500204000' '8b1510204000' 'c3')
    machine = emu.Machine(_image(code, {0x402000: NAME}))
    result = machine.run(0x401000, _case(pages={0x402000: b'\x7f' * PAGE}), 50)
    assert result.outcome == 'return' and result.regs[2] == 0x7f7f7f7f
    assert [e[1] for e in result.events if e[0] == 'c'] == [NAME]
    plain = machine.run(0x401000, _case(), 50)
    assert plain.outcome == 'return' and plain.regs[2] == 0  # image bytes restored


def test_pager_is_recorder_only_and_supplies_absent_pages():
    machine = emu.Machine(_image(bytes.fromhex('8b01c3')))
    assert machine.pager is None
    assert 'pager' not in inspect.getsource(difftest) and 'pager' not in inspect.getsource(recover)
    asked = []
    machine.pager = lambda page: asked.append(page) or (b'\x05' * PAGE if page == HEAP else None)
    assert machine.run(0x401000, _case({'ecx': HEAP + 8}), 20).regs[0] == 0x05050505
    assert set(machine.paged) == {HEAP}
    refused = machine.run(0x401000, _case({'ecx': HEAP + PAGE}), 20)
    assert refused.outcome == 'fault' and asked == [HEAP, HEAP + PAGE] and machine.paged == {}


def test_inputs_without_captures_keep_their_exact_stream():
    plain = difftest.Options(2, 100, 0, 2, .6, 1, {})
    listed = difftest.Options(2, 100, 0, 2, .6, 1, {'captures': []})
    templates = difftest.arena_templates()
    for seed in range(8):
        a = difftest.make_case(random.Random(seed), plain, [], templates)
        b = difftest.make_case(random.Random(seed), listed, [], templates)
        assert (a.regs, a.stack, a.arena, a.globals, a.import_seed, a.pages) == (
            b.regs, b.stack, b.arena, b.globals, b.import_seed, None)


@pytest.mark.parametrize('captures', ['a' * 64, ['A' * 64], ['a' * 63], ['a' * 64, 'a' * 64],
                                      [f'{i:064x}' for i in range(difftest.MAX_CAPTURES + 1)]])
def test_capture_hints_are_validated(captures):
    with pytest.raises(ValueError, match='captures'):
        difftest.validate_hints({'captures': captures})


def test_store_round_trip_identity_and_replay(tmp_path, monkeypatch):
    monkeypatch.setattr(env, 'LOCAL_CAPTURES', tmp_path)
    difftest._capture.cache_clear()
    va = 0x401000
    regs = {name: index for index, name in enumerate(capture.REG_NAMES)}
    pages = {HEAP: bytes(range(256)) * 16}
    name = capture.save(va, regs, 0x27f, [7, 8, 9], pages, 5, {'kind': 'test'})
    assert capture.valid_name(name) and capture.names_for(va) == [name]
    assert capture.save(va, regs, 0x27f, [7, 8, 9], pages, 5, {'kind': 'test'}) == name
    found = capture.load(va, name)
    assert (found.regs, found.stack, found.pages, found.fpcw, found.import_seed) == (regs, (7, 8, 9), pages, 0x27f, 5)
    opts = difftest.Options(4, 100, 0, 2, .6, 1, {'captures': [name]}, 'v2', va)
    cases = [difftest.make_case(random.Random(seed), opts, [], difftest.arena_templates()) for seed in range(12)]
    replayed = [case for case in cases if case.pages]
    assert replayed and len(replayed) < len(cases)  # random cases remain alongside captures
    assert replayed[0].pages == pages and replayed[0].stack == [7, 8] and replayed[0].fpcw == 0x27f
    assert not replayed[0].regs['eflags'] & difftest.EFLAGS_DF and not any(replayed[0].arena)
    # A capture belongs to one function of the pinned target and to its exact bytes.
    with pytest.raises(ValueError, match='belong'):
        other = tmp_path / 'fn_00402000'
        other.mkdir()
        (other / f'{name}.cap').write_bytes(capture.path_for(va, name).read_bytes())
        capture.load(0x402000, name)
    path = capture.path_for(va, name)
    path.write_bytes(path.read_bytes()[:-1] + b'\0')
    with pytest.raises(Exception):
        capture.load(va, name)
    path.unlink()
    difftest._capture.cache_clear()
    with pytest.raises(ValueError, match='capture unavailable'):
        for seed in range(12):
            difftest.make_case(random.Random(seed), opts, [], difftest.arena_templates())
    difftest._capture.cache_clear()


def test_expansion_only_edits_bytes_a_run_read_as_input():
    machine = emu.Machine(_image(bytes.fromhex('8b118901c3')))  # read [ecx], then write it
    result = machine.run(0x401000, _case({'ecx': HEAP + 4}, {HEAP: bytes(PAGE)}), 20)
    assert capture_expand._input_reads(result) == [(HEAP + 4, 4)]
    parent = {'regs': {n: 0 for n in capture.REG_NAMES}, 'stack': [], 'pages': {HEAP: bytes(PAGE)},
              'reads': [(HEAP + 4, 4)]}
    rng = random.Random(1)
    for _ in range(40):
        mutant, notes = capture_expand._mutate(rng, machine, parent)
        changed = [i for i, (x, y) in enumerate(zip(mutant['pages'][HEAP], bytes(PAGE))) if x != y]
        assert all(4 <= i < 8 for i in changed) and parent['pages'][HEAP] == bytes(PAGE)


def test_capture_store_is_part_of_the_harness_identity():
    # The bytes a test reads are loaded by a harness module; recorder and expander are tooling.
    assert capture.load.__module__ == capture.save.__module__ == 'cmre.difftest'
    assert 'difftest.py' in recover.HARNESS_MODULES and 'emu.py' in recover.HARNESS_MODULES
    assert not {'capture.py', 'capture_record.py', 'capture_expand.py'} & set(recover.HARNESS_MODULES)
