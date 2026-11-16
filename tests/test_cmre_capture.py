"""Captured entry inputs (ADR 0009). Authored instructions; no supplied executable required."""
import copy
import inspect
import math
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


# --- opt-in seeded import-scaffolding mutation ---------------------------------

SEED = 0x5D3A1B2C


def _seed_parent():
    return {'name': 'a' * 64, 'regs': {n: 0 for n in capture.REG_NAMES}, 'stack': [7, 8, 9],
            'pages': {HEAP: bytes(PAGE)}, 'reads': [(HEAP + 4, 4)], 'seed': SEED, 'fpcw': 0x27f}


def _legacy_mutate(rng, machine, parent):
    """The pre-flag implementation, to prove the default path keeps its exact stream."""
    regs, stack = dict(parent['regs']), list(parent['stack'])
    pages = {page: bytearray(blob) for page, blob in parent['pages'].items()}
    notes = []
    for _ in range(rng.choice((1, 1, 2, 3))):
        choice = rng.random()
        if choice < 0.15 and stack:
            index = rng.randrange(min(len(stack), 10))
            stack[index] = capture_expand._value(rng, stack[index], 4)
            notes.append(['stack', index, stack[index]])
        elif choice < 0.25:
            name = rng.choice(('eax', 'ecx', 'edx', 'ebx', 'esi', 'edi'))
            regs[name] = capture_expand._value(rng, regs[name], 4)
            notes.append(['reg', name, regs[name]])
        elif parent['reads']:
            address, size = rng.choice(parent['reads'])
            page, offset = address & ~(PAGE - 1), address & (PAGE - 1)
            if offset + size > PAGE:
                continue
            if page not in pages:
                if not any(lo <= page < hi for lo, hi in machine._writable_image):
                    continue
                pages[page] = bytearray(machine.pristine[page])
            old = int.from_bytes(pages[page][offset:offset + size], 'little')
            new = capture_expand._value(rng, old, size)
            pages[page][offset:offset + size] = new.to_bytes(size, 'little')
            notes.append(['mem', address, size, new])
    return ({'regs': regs, 'stack': stack, 'pages': {p: bytes(b) for p, b in pages.items()}}, notes)


class _FirstDraw(random.Random):
    """Random whose first random() is fixed; later draws delegate to a seeded stream."""

    def __init__(self, first):
        super().__init__(1234)
        self.first = first
        self.draws = 0

    def random(self):
        if self.draws == 0:
            self.draws += 1
            return self.first
        return super().random()


def test_seed_mutation_is_off_by_default_and_preserves_the_mutation_stream():
    machine = emu.Machine(_image(bytes.fromhex('8b118901c3')))
    parent = _seed_parent()
    for seed in range(8):
        legacy_rng, this_rng = random.Random(seed), random.Random(seed)
        legacy, legacy_notes = _legacy_mutate(legacy_rng, machine, parent)
        mutant, notes = capture_expand._mutate(this_rng, machine, parent)
        assert (mutant['regs'], mutant['stack'], mutant['pages'], notes) == (
            legacy['regs'], legacy['stack'], legacy['pages'], legacy_notes)
        assert mutant['seed'] == SEED
        assert this_rng.getstate() == legacy_rng.getstate()  # no draw was added to the default path


@pytest.mark.parametrize(('draw', 'seed_mutated'), [
    (0.25, False),  # the comparison is < 0.25: the exact boundary keeps ordinary edits
    (math.nextafter(0.25, 0.0), True),
])
def test_seed_branch_boundary_is_exactly_one_quarter(draw, seed_mutated):
    machine = emu.Machine(_image(bytes.fromhex('8b118901c3')))
    parent = _seed_parent()
    mutant, notes = capture_expand._mutate(_FirstDraw(draw), machine, parent, mutate_seed=True)
    assert (bool(notes) and notes[0][0] == 'import_seed') is seed_mutated
    if seed_mutated:
        assert notes == [['import_seed', SEED, mutant['seed']]] and mutant['seed'] != SEED
    else:
        assert mutant['seed'] == SEED and all(note[0] != 'import_seed' for note in notes)


@pytest.mark.parametrize('old', [0, 1, 0x12345678, 0xFFFFFFFF])
def test_equal_seed_draw_falls_back_to_a_different_u32(monkeypatch, old):
    parent = _seed_parent()
    parent['seed'] = old
    monkeypatch.setattr(capture_expand, '_value', lambda rng, value, size: value)

    class _Draw:
        def random(self):
            return 0.0

    mutant, notes = capture_expand._mutate(_Draw(), None, parent, mutate_seed=True)
    assert notes == [['import_seed', old, old ^ 1]]
    assert mutant['seed'] == old ^ 1 and 0 <= mutant['seed'] <= 0xFFFFFFFF
    assert mutant['seed'] != old


def test_enabled_seed_mutation_is_a_bounded_scaffolding_input_only():
    machine = emu.Machine(_image(bytes.fromhex('8b118901c3')))
    parent = _seed_parent()
    before = copy.deepcopy(parent)
    rng = random.Random(11)
    seed_edits = other_edits = 0
    for _ in range(200):
        mutant, notes = capture_expand._mutate(rng, machine, parent, mutate_seed=True)
        assert parent == before  # every parent byte and its seed stay untouched
        assert 0 <= mutant['seed'] <= 0xFFFFFFFF
        if any(note[0] == 'import_seed' for note in notes):
            seed_edits += 1
            assert notes == [['import_seed', SEED, mutant['seed']]]
            assert mutant['seed'] != SEED
            assert mutant['regs'] == parent['regs']
            assert mutant['stack'] == parent['stack']
            assert mutant['pages'] == parent['pages']
        else:
            other_edits += 1
            assert mutant['seed'] == SEED
    assert seed_edits and other_edits  # both forms occur, neither replaces the other


def test_expand_propagates_a_mutant_seed_to_run_saved_capture_and_follow_ons(monkeypatch):
    parent_name, parent_seed = 'a' * 64, 5
    starts = {0x401000 + i for i in range(64)}
    runs, parents_seen, saved = [], [], []
    monkeypatch.setattr(capture_expand, '_function', lambda va: (starts, (0x401000, 0x402000)))
    monkeypatch.setattr(capture_expand, '_machine', lambda va, region: (
        SimpleNamespace(arg_count=0, budget=100, hints={}), None))

    def run(machine, va, opts, regs, stack, pages, seed, fpcw):
        runs.append(seed)
        return SimpleNamespace(covered={0x401000 + seed % 64}, outcome='return', events=[])

    def mutate(rng, machine, parent, *, mutate_seed=False):
        assert mutate_seed is True
        old, new = parent['seed'], parent['seed'] + 1
        parents_seen.append(old)
        return {'regs': dict(parent['regs']), 'stack': list(parent['stack']),
                'pages': dict(parent['pages']), 'seed': new}, [['import_seed', old, new]]

    def save(va, regs, fpcw, stack, pages, seed, meta):
        name = f'{len(saved) + 1:064x}'
        saved.append((seed, meta, name))
        return name

    monkeypatch.setattr(capture_expand, '_run', run)
    monkeypatch.setattr(capture_expand, '_mutate', mutate)
    monkeypatch.setattr(capture_expand.capture, 'names_for', lambda va: [parent_name])
    monkeypatch.setattr(capture_expand.capture, 'load', lambda va, name: SimpleNamespace(
        regs={n: 0 for n in capture.REG_NAMES}, stack=[], pages={}, import_seed=parent_seed,
        fpcw=None, meta={'kind': 'observed'}))
    monkeypatch.setattr(capture_expand.capture, 'save', save)

    out = capture_expand.expand(0x401000, iterations=40, target=1.0, limit=64,
                                mutate_import_seed=True)
    assert out['added'] == len(saved) > 1 and runs[0] == parent_seed
    assert max(parents_seen) > parent_seed  # a saved mutant later seeded its own child
    assert len({seed for seed, _, _ in saved}) == len(saved)
    names = {parent_name}
    for seed, meta, name in saved:
        assert seed != parent_seed and meta['kind'] == 'mutated'
        assert meta['parent'] in names  # an original or an earlier saved derivative
        names.add(name)
        assert meta['edits'] == [['import_seed', seed - 1, seed]]
        assert 'not observed' in meta['scaffolding']
        assert seed in runs  # the original run used precisely the saved mutant seed


def test_expand_discards_a_seed_mutant_without_fresh_original_coverage(monkeypatch):
    parent_name = 'a' * 64
    monkeypatch.setattr(capture_expand, '_function',
                        lambda va: ({0x401000, 0x401002}, (0x401000, 0x402000)))
    monkeypatch.setattr(capture_expand, '_machine', lambda va, region: (
        SimpleNamespace(arg_count=0, budget=100, hints={}), None))
    monkeypatch.setattr(capture_expand, '_run', lambda *args: SimpleNamespace(
        covered={0x401000}, outcome='return', events=[]))
    monkeypatch.setattr(capture_expand, '_mutate',
                        lambda rng, machine, parent, *, mutate_seed=False: (
        {**parent, 'seed': parent['seed'] + 1},
        [['import_seed', parent['seed'], parent['seed'] + 1]]))
    monkeypatch.setattr(capture_expand.capture, 'names_for', lambda va: [parent_name])
    monkeypatch.setattr(capture_expand.capture, 'load', lambda va, name: SimpleNamespace(
        regs={n: 0 for n in capture.REG_NAMES}, stack=[], pages={}, import_seed=5,
        fpcw=None, meta={'kind': 'observed'}))
    monkeypatch.setattr(capture_expand.capture, 'save',
                        lambda *a: pytest.fail('no fresh coverage, no capture'))

    out = capture_expand.expand(0x401000, iterations=10, target=1.0, mutate_import_seed=True)
    assert out['added'] == 0


def test_fresh_callee_coverage_cannot_admit_while_fresh_root_coverage_can(monkeypatch):
    parent_name, parent_seed = 'a' * 64, 5
    starts = {0x401000, 0x401002}
    callee_only, fresh_root = 0x500000, 0x401002
    assert callee_only not in starts and fresh_root in starts
    coverage = {parent_seed: {0x401000}, 6: {callee_only}, 7: {0x401000, fresh_root}}
    seeds, saved = iter((6, 7)), []
    monkeypatch.setattr(capture_expand, '_function', lambda va: (starts, (0x401000, 0x402000)))
    monkeypatch.setattr(capture_expand, '_machine', lambda va, region: (
        SimpleNamespace(arg_count=0, budget=100, hints={}), None))
    monkeypatch.setattr(capture_expand, '_run', lambda *args: SimpleNamespace(
        covered=coverage[args[6]], outcome='return', events=[]))
    monkeypatch.setattr(capture_expand, '_mutate',
                        lambda rng, machine, parent, *, mutate_seed=False: (
        {**parent, 'seed': next(seeds)},
        [['import_seed', parent['seed'], parent['seed'] + 1]]))
    monkeypatch.setattr(capture_expand.capture, 'names_for', lambda va: [parent_name])
    monkeypatch.setattr(capture_expand.capture, 'load', lambda va, name: SimpleNamespace(
        regs={n: 0 for n in capture.REG_NAMES}, stack=[], pages={}, import_seed=parent_seed,
        fpcw=None, meta={'kind': 'observed'}))
    monkeypatch.setattr(capture_expand.capture, 'save',
                        lambda va, regs, fpcw, stack, pages, seed, meta: saved.append(seed) or ('b' * 64))

    out = capture_expand.expand(0x401000, iterations=2, target=1.0, mutate_import_seed=True)
    assert saved == [7] and out['added'] == 1  # the callee-only mutant was discarded
    assert out['coverage'] == 1.0


def test_capture_expand_mutate_import_seed_is_an_explicit_opt_in(monkeypatch):
    from cmre import cli, inventory
    calls = []
    monkeypatch.setattr(inventory, 'load', lambda: SimpleNamespace(entries_sorted=[], functions={}))
    monkeypatch.setattr(capture_expand, 'run_many', lambda action, vas, jobs, **options:
                        calls.append((action, options)) or [])
    assert cli.main(['capture', 'expand', 'fn_00401000']) == 0
    assert calls[-1] == ('expand', {'iterations': 1500, 'target': 0.9, 'mutate_import_seed': False})
    assert cli.main(['capture', 'expand', 'fn_00401000', '--mutate-import-seed']) == 0
    assert calls[-1][1]['mutate_import_seed'] is True


@pytest.mark.parametrize('action', ['record', 'apply', 'status'])
def test_mutate_import_seed_is_rejected_for_other_capture_actions(monkeypatch, capsys, action):
    from cmre import cli, inventory
    monkeypatch.setattr(inventory, 'load', lambda: pytest.fail('validation must precede any read'))
    monkeypatch.setattr(capture_expand, 'run_many', lambda *a, **k: pytest.fail('no capture work'))
    assert cli.main(['capture', action, 'fn_00401000', '--mutate-import-seed']) == 2
    assert 'applies only to capture expand' in capsys.readouterr().err


def test_seed_scaffolding_is_tooling_only_and_keeps_the_capture_format():
    assert capture.FORMAT == difftest.CAPTURE_FORMAT == 'cmre-capture/1'
    assert 'capture_expand' not in recover.HARNESS_MODULES
    assert capture_expand.SEED_SCAFFOLDING not in inspect.getsource(capture)
