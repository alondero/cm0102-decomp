"""Stack reads below the live ESP follow the compiler's frame layout, so a C case that
differs after the original made one is a hazard (inconclusive), never a silent pass.
Authored instructions and results only; no supplied executable required."""
from types import SimpleNamespace

import pytest

from cmre import difftest, emu, env, image, recover

PAGE = 0x1000
CODE = 0x401000
GLOBAL = 0x402000
PROBE = difftest.STACK_PROBE_CODE[0][0]
READER = 0x0090a35a


def _image(code: bytes):
    return SimpleNamespace(base=0x400000, headers=bytes(PAGE), imports={},
                           sections=[image.Section('.text', CODE, PAGE, code, 0x60000000),
                                     image.Section('.data', GLOBAL, PAGE, bytes(PAGE), 0xc0000000)])


def _run(code: bytes, track: bool = True):
    machine = emu.Machine(_image(code))
    machine.track_dead_stack_reads = track
    return machine.run(CODE, emu.Case({'eflags': 0x202}, [0x11223344], bytes(emu.ARENA_SIZE)), 50)


def test_read_below_esp_is_recorded_with_its_reader():
    result = _run(bytes.fromhex('8b4424f8c3'))  # mov eax,[esp-8]; ret
    assert [(a, eip) for _i, a, eip in result.dead_stack_reads] == [(emu.STACK_TOP - 8, CODE)]


def test_read_ending_exactly_at_esp_is_dead():
    result = _run(bytes.fromhex('8b4424fcc3'))  # mov eax,[esp-4]
    assert len(result.dead_stack_reads) == 1


@pytest.mark.parametrize('code', [
    '8b442404c3',  # mov eax,[esp+4]: a stack argument
    '8b0424c3',    # mov eax,[esp]: the return address slot
    '5059c3',      # push eax; pop ecx; ret: pop reads at ESP itself
    'c3',          # ret reads the return address at ESP
    '8b4424fec3',  # mov eax,[esp-2]: straddles ESP, so part of it is live
])
def test_live_stack_reads_are_not_dead(code):
    assert _run(bytes.fromhex(code)).dead_stack_reads == ()


def test_reads_outside_the_stack_are_never_dead():
    # GLOBAL lies numerically below ESP; only the stack range makes a read "dead stack".
    result = _run(bytes.fromhex('a1') + GLOBAL.to_bytes(4, 'little') + b'\xc3')
    assert result.dead_stack_reads == ()


def test_tracking_is_off_unless_requested():
    assert _run(bytes.fromhex('8b4424f8c3'), track=False).dead_stack_reads == ()


def test_dead_reads_do_not_carry_over_between_cases():
    machine = emu.Machine(_image(bytes.fromhex('8b4424f8c3')))
    machine.track_dead_stack_reads = True
    case = emu.Case({'eflags': 0x202}, [], bytes(emu.ARENA_SIZE))
    machine.run(CODE, case, 50)
    assert len(machine.run(CODE, case, 50).dead_stack_reads) == 1


# classification ---------------------------------------------------------------------------

_READ_THEN_CALL = [('r', emu.ARENA, 4, 0), ('c', 'x', 1, (1,))]


def _result(call_argument=1, dead=(), eax=0, events=None, covered=()):
    events = [('c', 'x', 1, (call_argument,))] if events is None else events
    return emu.Result('return', (), (eax,) + (0,) * 8, events, {}, set(covered), dead_stack_reads=tuple(dead))


def _verdict(original, recovered, tracked='same', profile='semantic'):
    """tracked: the original re-run with tracking ('same' = the original itself; None = no hazard rule)."""
    na = difftest._normalize(original, None, profile)
    nb = difftest._normalize(recovered, None, profile)
    hazard = None if tracked is None else (lambda: original if tracked == 'same' else tracked)
    return difftest._classify(na, nb, original, None, (CODE, CODE + 16), [], layout_hazard=hazard,
                              profile=profile)[0]


def test_difference_after_a_dead_stack_read_is_a_layout_hazard():
    original = _result(events=[('r', emu.ARENA, 4, 0), ('c', 'x', 1, (1,))], dead=[(0, emu.STACK_TOP - 8, READER)])
    assert _verdict(original, _result(2)) == 'stack-layout'


def test_dead_read_after_the_first_differing_call_does_not_excuse_it():
    events = [('r', emu.ARENA, 4, 0), ('c', 'x', 1, (1,)), ('r', emu.ARENA, 4, 0)]
    original = _result(events=events, dead=[(2, emu.STACK_TOP - 8, READER)])
    assert _verdict(original, _result(2)) == 'mismatch'


def test_dead_read_just_before_the_differing_call_counts():
    events = [('r', emu.ARENA, 4, 0), ('c', 'x', 1, (1,))]
    assert _verdict(_result(events=events, dead=[(0, emu.STACK_TOP - 8, READER)]), _result(2)) == 'stack-layout'
    assert _verdict(_result(events=events, dead=[(1, emu.STACK_TOP - 8, READER)]), _result(2)) == 'mismatch'


def test_a_later_call_difference_needs_a_dead_read_before_that_call():
    events = [('c', 'x', 1, (1,)), ('r', emu.ARENA, 4, 0), ('c', 'x', 1, (5,))]
    recovered = _result(events=[('c', 'x', 1, (1,)), ('c', 'x', 1, (6,))])
    late = _result(events=events, dead=[(1, emu.STACK_TOP - 8, READER)])
    early_only = _result(events=events, dead=[(3, emu.STACK_TOP - 8, READER)])
    assert _verdict(late, recovered) == 'stack-layout'
    assert _verdict(early_only, recovered) == 'mismatch'


def test_final_state_difference_takes_any_dead_read():
    original = _result(eax=1, dead=[(7, emu.STACK_TOP - 8, READER)])
    assert _verdict(original, _result(eax=2)) == 'stack-layout'


def test_equal_results_stay_equal_after_a_dead_stack_read():
    original = _result(dead=[(0, emu.STACK_TOP - 8, READER)])
    assert _verdict(original, _result(1)) == 'equal'


def test_difference_without_a_dead_stack_read_is_a_mismatch():
    assert _verdict(_result(), _result(2)) == 'mismatch'


def test_only_the_c_tier_treats_dead_reads_as_hazards():
    original = _result(dead=[(0, emu.STACK_TOP - 8, READER)])
    assert _verdict(original, _result(2), tracked=None) == 'mismatch'


def test_the_abi_profile_never_applies_the_rule():
    original = _result(dead=[(0, emu.STACK_TOP - 8, READER)])
    assert _verdict(original, _result(2), tracked=None, profile='abi') == 'mismatch'


def test_stack_probe_reads_do_not_excuse_a_difference():
    probes = [(0, emu.STACK_TOP - 0x1000, PROBE + 0x17), (0, emu.STACK_TOP - 0x2000, PROBE)]
    assert _verdict(_result(events=_READ_THEN_CALL, dead=probes), _result(2)) == 'mismatch'


def test_a_real_read_beside_probe_reads_is_still_a_hazard():
    reads = [(0, emu.STACK_TOP - 0x1000, PROBE + 0x17), (0, emu.STACK_TOP - 8, READER)]
    assert _verdict(_result(events=_READ_THEN_CALL, dead=reads), _result(2)) == 'stack-layout'


def test_reader_just_outside_the_probe_routine_counts():
    end = difftest.STACK_PROBE_CODE[0][1]
    assert _verdict(_result(events=_READ_THEN_CALL, dead=[(0, emu.STACK_TOP - 8, end)]), _result(2)) == 'stack-layout'


def test_a_rerun_that_diverges_from_the_first_run_is_not_a_hazard():
    first = _result(events=[('c', 'x', 1, (1,))])
    rerun = _result(events=[('c', 'x', 1, (9,))], dead=[(0, emu.STACK_TOP - 8, READER)])
    assert _verdict(first, _result(2), tracked=rerun) == 'mismatch'


# whole run -------------------------------------------------------------------------------

class _Original:
    """Stub original machine: every case yields `base` untracked and `tracked` when tracking is on."""

    def __init__(self, cases):
        self.cases, self.calls, self.track_dead_stack_reads, self.seen = cases, 0, False, []
        self.uc = None

    def run(self, *args):
        if not self.track_dead_stack_reads:
            self.seen.append((self.calls, False))
            self.calls += 1
            return self.cases[self.calls - 1][0]
        self.seen.append((self.calls - 1, True))  # the re-run repeats the case just run
        return self.cases[self.calls - 1][1]


def _run_cases(monkeypatch, differing, cases=100, profile='semantic', dead_after=False, covered=None):
    """difftest.run over stub machines: case n differs when n is in `differing`; its tracked re-run
    shows a dead stack read before the differing call (after it when `dead_after`)."""
    dead = [(2 if dead_after else 0, emu.STACK_TOP - 8, READER)]
    events = [('r', emu.ARENA, 4, 0), ('c', 'x', 1, (1,))]
    covered = covered or {}
    plain = [(_result(1, events=events, covered=covered.get(n, {CODE})),
              _result(1, events=events, covered=covered.get(n, {CODE}), dead=dead if n in differing else ()))
             for n in range(cases)]
    recovered = iter([_result(2 if n in differing else 1) for n in range(cases)])
    stubs = []

    def machine(*args, **kwargs):
        stub = _Original(plain) if not stubs else SimpleNamespace(
            uc=None, run=lambda *a: next(recovered), logical_budget=False)
        stubs.append(stub)
        return stub

    monkeypatch.setattr(emu, 'Machine', machine)
    monkeypatch.setattr(image, 'primary', lambda: SimpleNamespace())
    monkeypatch.setattr(difftest, 'make_case', lambda *args: emu.Case({}, [], b''))
    monkeypatch.setattr(difftest, 'arena_templates', lambda: ())
    monkeypatch.setattr(difftest, 'semantic_fp', lambda *args: ())
    built = SimpleNamespace(entry=env.LINK_BASE + 0x1000, symbols={}, image=SimpleNamespace(
        text=SimpleNamespace(va=env.LINK_BASE + 0x1000, end=env.LINK_BASE + 0x2000), x87_bindings=()))
    options = difftest.Options(**{**difftest.DEFAULTS, 'cases': cases, 'discovery': 0}, seed=1, hints={})
    result = difftest.run(CODE, (CODE, CODE + 16), {CODE, CODE + 1}, built, {}, options, profile)
    return result, stubs[0]


def test_a_few_layout_dependent_cases_are_inconclusive_not_failures(monkeypatch):
    result, _ = _run_cases(monkeypatch, {7, 40})
    assert result['mismatches'] == []
    assert result['inconclusive'] == 2
    assert result['stack_layout_cases'] == 2 and result['stack_layout_examples'] == [7, 40]


def test_cases_run_untracked_and_only_differing_cases_are_rerun_with_tracking(monkeypatch):
    _, original = _run_cases(monkeypatch, {7, 40})
    assert [call for call in original.seen if call[1]] == [(7, True), (40, True)]
    assert len([call for call in original.seen if not call[1]]) == 100
    assert original.track_dead_stack_reads is False


def test_tracking_is_never_switched_on_outside_the_semantic_profile(monkeypatch):
    result, original = _run_cases(monkeypatch, {7}, profile='abi')
    assert not any(tracking for _call, tracking in original.seen)
    assert [m['case'] for m in result['mismatches']] == [7]
    assert 'stack_layout_cases' not in result


def test_a_dead_read_after_the_first_differing_call_stays_a_mismatch_in_a_run(monkeypatch):
    result, _ = _run_cases(monkeypatch, {7}, dead_after=True)
    assert [m['case'] for m in result['mismatches']] == [7] and result['inconclusive'] == 0


def test_many_layout_dependent_cases_remain_mismatches(monkeypatch):
    result, _ = _run_cases(monkeypatch, {1, 2, 3})  # 3% of 100 cases exceeds the 2% limit
    assert [m['case'] for m in result['mismatches']] == [1, 2, 3]
    assert all(m['kind'] == 'stack-layout-excess' for m in result['mismatches'])
    assert result['inconclusive'] == 0


def test_the_limit_itself_is_still_a_hazard(monkeypatch):
    result, _ = _run_cases(monkeypatch, {1, 2})  # exactly 2%
    assert result['mismatches'] == [] and result['inconclusive'] == 2


def test_a_run_without_layout_cases_reports_no_new_fields(monkeypatch):
    result, _ = _run_cases(monkeypatch, set())
    assert 'stack_layout_cases' not in result and result['inconclusive'] == 0


def test_instructions_reached_only_in_waived_cases_do_not_count_as_covered(monkeypatch):
    covered = {7: {CODE, CODE + 1}}
    waived, _ = _run_cases(monkeypatch, {7}, covered=covered)
    assert waived['coverage'] == 0.5
    counted, _ = _run_cases(monkeypatch, set(), covered=covered)
    assert counted['coverage'] == 1.0


def test_the_acceptance_record_keeps_the_waived_cases(tmp_path):
    source = tmp_path / 'fn.c'
    source.write_text('int x;')
    difftest_report = {'profile': 'semantic', 'returns': 'int', 'inputs': 'v2', 'cases': 600, 'seed': 1,
                       'outcomes': {'fault': 600}, 'coverage': 1.0, 'inconclusive': 1, 'result_digest': 'd',
                       'stack_layout_cases': 1, 'stack_layout_examples': [331]}
    report = {'tier': 'c', 'harness_version': 1, 'harness_sha256': 'h', 'toolchain': {}, 'key': 'k',
              'source_dependencies': [], 'preprocessed_sha256': 'p',
              'builds': {'-O0': {'difftest': difftest_report}}}
    record = recover._tier_record(report, source)
    assert record['validation']['-O0']['stack_layout_cases'] == 1
    assert record['validation']['-O0']['stack_layout_examples'] == [331]
    del report['builds']['-O0']['difftest']['stack_layout_cases']
    assert 'stack_layout_cases' not in recover._tier_record(report, source)['validation']['-O0']
