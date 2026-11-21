"""Comparison-guided expansion of captured inputs (input-to-state mutation).

Authored instructions on a synthetic image; no supplied executable required.
"""
from types import SimpleNamespace

from cmre import capture, capture_expand, emu, image

PAGE = 0x1000
HEAP = 0x05000000
CODE = 0x401000
OPTS = SimpleNamespace(arg_count=2, budget=200)
GUARDED = CODE + 9  # `mov edx, 1` in every program below


def _image(code: bytes):
    return SimpleNamespace(base=0x400000, headers=bytes(PAGE), imports={},
                           sections=[image.Section('.text', CODE, PAGE, code, 0x60000000),
                                     image.Section('.data', 0x402000, PAGE, bytes(PAGE), 0xc0000000)])


def _parent(regs=None, stack=(0, 0), pages=None):
    return {'name': 'p', 'regs': {**{n: 0 for n in capture.REG_NAMES}, 'eflags': 0x202, **(regs or {})},
            'stack': list(stack), 'pages': dict(pages or {}), 'seed': 0, 'fpcw': None, 'reads': []}


def _explore(code: bytes, parent: dict):
    """Trace the parent, then run every comparison-derived mutant; return (machine, mutants, covered)."""
    machine = emu.Machine(_image(code), watch=(CODE, CODE + len(code)))
    starts = capture_expand._starts_from_bytes(CODE, code)
    sites = capture_expand._compare_sites(machine, starts)
    result, observed = capture_expand._traced_run(machine, CODE, OPTS, parent, sites)
    parent['reads'] = capture_expand._input_reads(result)
    before = {k: (dict(v) if isinstance(v, dict) else list(v) if isinstance(v, list) else v)
              for k, v in parent.items()}
    mutants = capture_expand._i2s_mutants(machine, parent, observed)
    assert {k: parent[k] for k in ('regs', 'stack', 'pages')} == {k: before[k] for k in ('regs', 'stack', 'pages')}
    covered = set()
    for mutant, _ in mutants:
        covered |= capture_expand._run(machine, CODE, OPTS, mutant['regs'], mutant['stack'],
                                       mutant['pages'], mutant['seed'], None).covered
    return machine, mutants, result.covered, covered


def test_register_compare_is_solved_from_the_memory_input_it_was_loaded_from():
    # mov eax,[ecx]; cmp eax,0x12345; jne +5; mov edx,1; ret
    code = bytes.fromhex('8b01' '3d45230100' '7505' 'ba01000000' 'c3')
    _, mutants, before, after = _explore(code, _parent({'ecx': HEAP + 8}, pages={HEAP: bytes(PAGE)}))
    assert GUARDED not in before and GUARDED in after
    solved = [m for m, notes in mutants if ['mem', HEAP + 8, 4, 0x12345] in notes]
    assert solved and int.from_bytes(solved[0]['pages'][HEAP][8:12], 'little') == 0x12345


def test_memory_operand_compare_edits_exactly_the_compared_address():
    # cmp dword [ecx],0x77; jne +5; mov edx,1; ret  (pad cmp to keep GUARDED at +9)
    code = bytes.fromhex('813977000000' '90' '7505' 'ba01000000' 'c3')
    assert code[9] == 0xba
    _, mutants, before, after = _explore(code, _parent({'ecx': HEAP + 0x20}, pages={HEAP: bytes(PAGE)}))
    assert GUARDED not in before and GUARDED in after
    edits = {tuple(n) for _, notes in mutants for n in notes}
    assert ('mem', HEAP + 0x20, 4, 0x77) in edits


def test_stack_argument_compare_edits_the_argument_word():
    # cmp dword [esp+4],9; jne +3; mov edx,1 ... laid out so GUARDED is at +9
    code = bytes.fromhex('837c240409' '90' '90' '7505' 'ba01000000' 'c3')
    assert code[9] == 0xba
    _, mutants, before, after = _explore(code, _parent(stack=(3, 0)))
    assert GUARDED not in before and GUARDED in after
    assert any(m['stack'][0] == 9 for m, _ in mutants)


def test_small_switch_bound_offers_every_case_value():
    # mov eax,[ecx]; cmp eax,3; nop; nop; ja +5; mov edx,1; ret
    code = bytes.fromhex('8b01' '83f803' '9090' '7705' 'ba01000000' 'c3')
    assert code[9] == 0xba
    parent = _parent({'ecx': HEAP}, pages={HEAP: (50).to_bytes(4, 'little') + bytes(PAGE - 4)})
    _, mutants, before, after = _explore(code, parent)
    assert GUARDED not in before and GUARDED in after
    values = {n[3] for _, notes in mutants for n in notes if n[0] == 'mem' and n[1] == HEAP}
    assert {0, 1, 2, 3, 4} <= values


def test_bit_test_sets_and_clears_the_tested_bits():
    # test byte [ecx],0x40; nop*4; je +5; mov edx,1; ret
    code = bytes.fromhex('f60140' '90909090' '7405' 'ba01000000' 'c3')
    assert code[9] == 0xba
    _, mutants, before, after = _explore(code, _parent({'ecx': HEAP}, pages={HEAP: bytes(PAGE)}))
    assert GUARDED not in before and GUARDED in after
    assert any(n == ['mem', HEAP, 1, 0x40] for _, notes in mutants for n in notes)


def test_code_constants_and_harness_memory_are_never_edited():
    # cmp dword [0x401000],5; jne +5 would need a code edit: no mutant may change code.
    code = bytes.fromhex('813d0010400005000000' '7505' 'ba01000000' 'c3')
    machine = emu.Machine(_image(code), watch=(CODE, CODE + len(code)))
    starts = capture_expand._starts_from_bytes(CODE, code)
    parent = _parent()
    _, observed = capture_expand._traced_run(machine, CODE, OPTS, parent, capture_expand._compare_sites(machine, starts))
    assert observed, 'the comparison was traced'
    for mutant, notes in capture_expand._i2s_mutants(machine, parent, observed):
        assert all(page >= HEAP or any(lo <= page < hi for lo, hi in machine._writable_image)
                   for page in mutant['pages'])
        assert not any(n[0] == 'mem' and CODE <= n[1] < CODE + PAGE for n in notes)


def test_tracing_leaves_the_run_unchanged():
    code = bytes.fromhex('8b01' '3d45230100' '7505' 'ba01000000' 'c3')
    machine = emu.Machine(_image(code), watch=(CODE, CODE + len(code)))
    parent = _parent({'ecx': HEAP + 8}, pages={HEAP: bytes(PAGE)})
    plain = capture_expand._run(machine, CODE, OPTS, parent['regs'], parent['stack'], parent['pages'], 0, None)
    sites = capture_expand._compare_sites(machine, capture_expand._starts_from_bytes(CODE, code))
    traced, _ = capture_expand._traced_run(machine, CODE, OPTS, parent, sites)
    again = capture_expand._run(machine, CODE, OPTS, parent['regs'], parent['stack'], parent['pages'], 0, None)
    for result in (traced, again):
        assert (result.outcome, result.regs, result.covered, result.events) == (
            plain.outcome, plain.regs, plain.covered, plain.events)


# mov eax,[ecx]; cmp eax,0x12345; jne +5; mov edx,1;
# mov eax,[ecx+4]; cmp eax,0x777; jne +5; mov edx,2; ret
TWO_BRANCHES = bytes.fromhex('8b01' '3d45230100' '7505' 'ba01000000'
                             '8b4104' '3d77070000' '7505' 'ba02000000' 'c3')


def _expand_world(monkeypatch, code=TWO_BRANCHES):
    """`expand` over a synthetic image with one stored capture; returns (saved, runs)."""
    machine = emu.Machine(_image(code), watch=(CODE, CODE + len(code)))
    starts = capture_expand._starts_from_bytes(CODE, code)
    parent = _parent({'ecx': HEAP + 8}, pages={HEAP: bytes(PAGE)})
    stored = SimpleNamespace(regs=parent['regs'], stack=parent['stack'], pages=parent['pages'],
                             import_seed=0, fpcw=None)
    saved, runs = [], []
    real_run = capture_expand._run
    monkeypatch.setattr(capture_expand, '_function', lambda va: (set(starts), (CODE, CODE + len(code))))
    monkeypatch.setattr(capture_expand, '_machine', lambda va, region: (OPTS, machine))
    monkeypatch.setattr(capture, 'names_for', lambda va: ['p'])
    monkeypatch.setattr(capture, 'load', lambda va, name: stored)
    monkeypatch.setattr(capture, 'save', lambda va, regs, fpcw, stack, pages, seed, meta:
                        saved.append(meta) or f'd{len(saved)}')
    monkeypatch.setattr(capture_expand, '_run', lambda *a: runs.append(1) or real_run(*a))
    return saved, runs


def test_expand_solves_both_branches_with_comparison_guidance(monkeypatch):
    saved, _ = _expand_world(monkeypatch)
    result = capture_expand.expand(CODE, iterations=40, target=1.0)
    assert result['coverage'] == 1.0 and result['added_compare'] == 2
    assert all(meta['strategy'] == 'compare' for meta in saved)


def test_expand_without_compare_never_traces(monkeypatch):
    saved, _ = _expand_world(monkeypatch)
    def refuse(*a, **k):
        raise AssertionError('compare phase ran')
    monkeypatch.setattr(capture_expand, '_traced_run', refuse)
    monkeypatch.setattr(capture_expand, '_compare_sites', refuse)
    result = capture_expand.expand(CODE, iterations=40, target=1.0, compare=False)
    assert result['added_compare'] == 0
    assert all('strategy' not in meta for meta in saved)


def test_expand_stays_within_its_iteration_budget(monkeypatch):
    _, runs = _expand_world(monkeypatch)  # `_traced_run` executes through `_run`, so traces count here
    capture_expand.expand(CODE, iterations=3, target=1.0)
    corpus_replays = 1
    assert len(runs) - corpus_replays <= 3


def test_expand_stops_comparison_mutants_at_the_coverage_target(monkeypatch):
    saved, _ = _expand_world(monkeypatch)
    result = capture_expand.expand(CODE, iterations=40, target=0.85)  # 8 of 9 instructions
    assert result['added'] == 1 and len(saved) == 1
