"""Stack reads below the live ESP follow the compiler's frame layout, so a C case that
differs after the original made one is a hazard (inconclusive), never a silent pass.
Authored instructions and results only; no supplied executable required."""
from types import SimpleNamespace

import pytest

from cmre import difftest, emu, env, image

PAGE = 0x1000
CODE = 0x401000
PROBE = difftest.STACK_PROBE_CODE[0][0]


def _image(code: bytes):
    return SimpleNamespace(base=0x400000, headers=bytes(PAGE), imports={},
                           sections=[image.Section('.text', CODE, PAGE, code, 0x60000000),
                                     image.Section('.data', 0x402000, PAGE, bytes(PAGE), 0xc0000000)])


def _run(code: bytes, track: bool = True):
    machine = emu.Machine(_image(code))
    machine.track_dead_stack_reads = track
    return machine.run(CODE, emu.Case({'eflags': 0x202}, [0x11223344], bytes(emu.ARENA_SIZE)), 50)


def test_read_below_esp_is_recorded_with_its_reader():
    result = _run(bytes.fromhex('8b4424f8c3'))  # mov eax,[esp-8]; ret
    assert [(a, eip) for _i, a, eip in result.dead_stack_reads] == [(emu.STACK_TOP - 8, CODE)]


@pytest.mark.parametrize('code', [
    '8b442404c3',  # mov eax,[esp+4]: a stack argument
    '8b0424c3',    # mov eax,[esp]: the return address slot
    '5059c3',      # push eax; pop ecx; ret: pop reads at ESP itself
    'c3',          # ret reads the return address at ESP
])
def test_live_stack_reads_are_not_dead(code):
    assert _run(bytes.fromhex(code)).dead_stack_reads == ()


def test_read_straddling_esp_is_live():
    result = _run(bytes.fromhex('8b4424fec3'))  # mov eax,[esp-2]: bytes below and at ESP
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

def _result(call_argument=1, dead=()):
    return emu.Result('return', (), (0,) * 9, [('c', 'x', 1, (call_argument,))], {}, set(),
                      dead_stack_reads=tuple(dead))


def _verdict(original, recovered, **kwargs):
    na = difftest._normalize(original, None, 'semantic')
    nb = difftest._normalize(recovered, None, 'semantic')
    return difftest._classify(na, nb, original, None, (CODE, CODE + 16), [], **kwargs)[0]


def test_difference_after_a_dead_stack_read_is_a_layout_hazard():
    original = _result(dead=[(3, emu.STACK_TOP - 8, 0x0090a35a)])
    assert _verdict(original, _result(2), layout_hazard=True) == 'stack-layout'


def test_equal_results_stay_equal_after_a_dead_stack_read():
    original = _result(dead=[(3, emu.STACK_TOP - 8, 0x0090a35a)])
    assert _verdict(original, _result(1), layout_hazard=True) == 'equal'


def test_difference_without_a_dead_stack_read_is_a_mismatch():
    assert _verdict(_result(), _result(2), layout_hazard=True) == 'mismatch'


def test_only_the_c_tier_treats_dead_reads_as_hazards():
    original = _result(dead=[(3, emu.STACK_TOP - 8, 0x0090a35a)])
    assert _verdict(original, _result(2)) == 'mismatch'


def test_stack_probe_reads_do_not_excuse_a_difference():
    probes = [(0, emu.STACK_TOP - 0x1000, PROBE + 0x17), (1, emu.STACK_TOP - 0x2000, PROBE)]
    assert _verdict(_result(dead=probes), _result(2), layout_hazard=True) == 'mismatch'


def test_a_real_read_beside_probe_reads_is_still_a_hazard():
    reads = [(0, emu.STACK_TOP - 0x1000, PROBE + 0x17), (5, emu.STACK_TOP - 8, 0x0090a35a)]
    assert _verdict(_result(dead=reads), _result(2), layout_hazard=True) == 'stack-layout'


def test_reader_just_outside_the_probe_routine_counts():
    end = difftest.STACK_PROBE_CODE[0][1]
    assert _verdict(_result(dead=[(0, emu.STACK_TOP - 8, end)]), _result(2), layout_hazard=True) == 'stack-layout'


# whole run -------------------------------------------------------------------------------

def _run_cases(monkeypatch, differing, cases=100):
    """difftest.run over stub machines: case n differs when n is in `differing`, and each
    differing case follows a dead stack read by the original."""
    dead = [(0, emu.STACK_TOP - 8, 0x0090a35a)]
    originals = [_result(1, dead if n in differing else ()) for n in range(cases)]
    recovered = [_result(2 if n in differing else 1) for n in range(cases)]
    feeds = {'original': iter(originals), 'recovered': iter(recovered)}
    machines = []

    def machine(*args, **kwargs):
        which = 'original' if not machines else 'recovered'
        stub = SimpleNamespace(uc=None, run=lambda *a, _w=which: next(feeds[_w]), logical_budget=False,
                               track_dead_stack_reads=False)
        machines.append(stub)
        return stub

    monkeypatch.setattr(emu, 'Machine', machine)
    monkeypatch.setattr(image, 'primary', lambda: SimpleNamespace())
    monkeypatch.setattr(difftest, 'make_case', lambda *args: emu.Case({}, [], b''))
    monkeypatch.setattr(difftest, 'arena_templates', lambda: ())
    monkeypatch.setattr(difftest, 'semantic_fp', lambda *args: ())
    built = SimpleNamespace(entry=env.LINK_BASE + 0x1000, symbols={}, image=SimpleNamespace(
        text=SimpleNamespace(va=env.LINK_BASE + 0x1000, end=env.LINK_BASE + 0x2000), x87_bindings=()))
    options = difftest.Options(**{**difftest.DEFAULTS, 'cases': cases, 'discovery': 0}, seed=1, hints={})
    result = difftest.run(CODE, (CODE, CODE + 16), {CODE}, built, {}, options, 'semantic')
    return result, machines[0]


def test_a_few_layout_dependent_cases_are_inconclusive_not_failures(monkeypatch):
    result, original = _run_cases(monkeypatch, {7, 40})
    assert original.track_dead_stack_reads is True
    assert result['mismatches'] == []
    assert result['inconclusive'] == 2
    assert result['stack_layout_cases'] == 2 and result['stack_layout_examples'] == [7, 40]


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
