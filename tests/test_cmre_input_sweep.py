"""Every hinted input fixture and capture runs once in the gate, before the random cases.

Random case k keeps number k and its exact input; hinted input i is case number
opts.cases + i. The random stream, and so every pre-release random case, is unchanged.
"""
import random

import pytest

from cmre import build, difftest, emu, lift, mutate, recover

LEAP = 0x0054CFE0  # leap test on the low word of its first stack argument; every path returns
ODD_YEAR = 0x1235  # not a leap year: the original returns 0


def year_fixture(year, seed):
    regs = dict.fromkeys(emu.REG_NAMES, 0)
    regs['eflags'] = 0x202
    return {'regs': regs, 'stack': [year] + [0] * 9, 'arena_template': 0,
            'arena_writes': {}, 'globals': {}, 'import_seed': seed}


def year_fixtures(count=40, odd_at=None):
    return [year_fixture(ODD_YEAR if i == odd_at else 1990 + i, i + 1) for i in range(count)]


def gate(tmp_path, text, hints, cases, profile='exact', budget=None):
    lifted = lift.discover(LEAP)
    src = tmp_path / f'fn_{LEAP:08x}.S'
    src.write_text(text, encoding='utf-8')
    built = build.build(src, lifted.symbol, tmp_path)
    opts = difftest.Options.for_function(LEAP, {'cases': cases, **({'budget': budget} if budget else {})})
    opts.hints = hints
    args = (LEAP, (lifted.va, lifted.region_end), set(lifted.insns), built,
            recover._call_returns(lifted, built), opts, profile)
    return opts, args


def test_every_hinted_fixture_runs_once_before_the_random_cases(tmp_path, monkeypatch):
    seen = []
    fixture_case = difftest._fixture_case
    def spy(fixture, templates, arg_count):
        seen.append(fixture['import_seed'])
        return fixture_case(fixture, templates, arg_count)
    monkeypatch.setattr(difftest, '_fixture_case', spy)
    fixtures = year_fixtures()
    _, args = gate(tmp_path, lift.render(lift.discover(LEAP)), {'fixtures': fixtures}, cases=8)
    result = difftest.run(*args)
    assert seen[:40] == list(range(1, 41)), 'a hinted fixture never ran'
    assert result['mismatches'] == []
    assert result['cases'] == 8 and result['input_cases'] == 40
    assert sum(result['outcomes'].values()) == 48


def test_a_fixture_only_mismatch_fails_the_gate(tmp_path):
    text = lift.render(lift.discover(LEAP))
    entry = '    mov ecx, dword ptr [esp + 4]\n'
    assert entry in text
    # Report a leap year for one odd year only; random cases almost never hit that word.
    altered = text.replace(entry, entry + f'    cmp cx, {ODD_YEAR:#x}\n    je v_0054d02c\n', 1)
    _, args = gate(tmp_path, altered, {'fixtures': year_fixtures(odd_at=27)}, cases=8)
    result = difftest.run(*args)
    assert result['mismatches'], 'the mismatching fixture was not run'
    assert result['mismatches'][0]['case'] == 8 + 27


def test_random_cases_keep_their_numbers_inputs_and_order():
    opts = difftest.Options.for_function(0x00450A70, {'cases': 30})
    fixtures = year_fixtures(5)
    opts.hints = {'fixtures': fixtures}
    templates = difftest.arena_templates()
    stream = list(difftest.gate_cases(opts, set(), templates))
    assert [number for number, _, _ in stream] == [30, 31, 32, 33, 34] + list(range(30))
    assert [case.import_seed for _, case, _ in stream[:5]] == [1, 2, 3, 4, 5]
    assert [discovering for _, _, discovering in stream] == [False] * 5 + [i < opts.discovery for i in range(30)]
    rng = random.Random(opts.seed)
    assert [case for _, case, _ in stream[5:]] == [difftest.make_case(rng, opts, [], templates) for _ in range(30)]


def test_captures_follow_fixtures_in_hint_order(monkeypatch):
    names = ['a' * 64, 'b' * 64]
    seen = []
    def capture(va, name, arena_size, arg_count):
        seen.append(name)
        return difftest._fixture_case(year_fixture(2000, 99), difftest.arena_templates(), arg_count)
    monkeypatch.setattr(difftest, '_capture_case', capture)
    opts = difftest.Options.for_function(0x00450A70, {'cases': 3})
    opts.hints = {'fixtures': [year_fixture(1999, 7)], 'captures': names}
    stream = list(difftest.gate_cases(opts, set(), difftest.arena_templates()))
    assert [number for number, _, _ in stream[:3]] == [3, 4, 5]
    assert stream[0][1].import_seed == 7
    assert seen[:2] == names


def test_v2_entry_policy_and_transcript_apply_to_hinted_inputs():
    data = year_fixture(2000, 1)
    data['regs']['eflags'] = 0x602  # DF set in the recorded input
    opts = difftest.Options.for_function(0x00450A70, {'cases': 1})
    opts.hints = {'fixtures': [data]}
    opts.inputs = 'v2'
    case = next(iter(difftest.gate_cases(opts, set(), difftest.arena_templates())))[1]
    assert case.regs['eflags'] & difftest.EFLAGS_DF == 0


def test_explain_replays_a_hinted_input_by_its_case_number(tmp_path):
    fixtures = year_fixtures(6)
    _, args = gate(tmp_path, lift.render(lift.discover(LEAP)), {'fixtures': fixtures}, cases=4)
    out = difftest.explain(*args, case_index=4 + 3)
    assert out['case'] == 7 and out['stack_in'][0] == hex(1993)
    random_case = difftest.explain(*args, case_index=2)
    rng = random.Random(args[5].seed)
    cases = [difftest.make_case(rng, args[5], [], difftest.arena_templates()) for _ in range(3)]
    assert random_case['stack_in'] == [hex(v) for v in cases[2].stack]
    with pytest.raises(ValueError, match='case'):
        difftest.explain(*args, case_index=4 + 6)


@pytest.mark.parametrize('profile', ['abi', 'semantic'])
def test_hinted_inputs_count_toward_inconclusive_against_the_random_denominator(tmp_path, profile):
    _, args = gate(tmp_path, lift.render(lift.discover(LEAP)), {'fixtures': year_fixtures(3)},
                   cases=4, profile=profile, budget=1)
    result = difftest.run(*args)
    assert result['cases'] == 4 and result['input_cases'] == 3
    assert result['inconclusive'] == 7 and result['outcomes'] == {'budget': 7}


@pytest.mark.parametrize('index,killed', [(600, True), (601, True), (602, False)])
def test_control_witnesses_may_name_pinned_hinted_inputs(index, killed):
    identity = {'cases': 600, 'options': {'cases': 600, 'hints': {'fixtures': [{}], 'captures': ['a' * 64]}}}
    numbers = mutate._control_case_numbers(identity)
    assert numbers == 602
    report = {'status': 'fail', 'problems': [], 'builds': {
        opt: {'status': 'fail', 'difftest': {'mismatches': [{'case': index, 'field': 'regs'}]}}
        for opt in ('-O0', '-O2')}}
    classification, _ = mutate.classify_control(report, {'kind': 'mismatch', 'field': 'regs'}, cases=numbers)
    assert (classification == 'killed') is killed


@pytest.mark.parametrize('ran,valid', [(2, True), (0, False), (None, False), (3, False)])
def test_control_report_must_run_every_pinned_hinted_input(ran, valid):
    options = {'cases': 600, 'budget': 9, 'seed': 5, 'inputs': 'v2',
               'hints': {'fixtures': [{}], 'captures': ['a' * 64]}}
    identity = {'execution_identity': {'harness': 'h'}, 'cases': 600, 'options': options}
    diff = {'cases': 600, 'budget': 9, 'seed': 5, 'inputs': 'v2', 'profile': 'semantic', 'mismatches': []}
    if ran is not None:
        diff['input_cases'] = ran
    report = {'execution_identity': {'harness': 'h'}, 'source_sha256': 's', 'profile': 'semantic',
              'va': '0x0054cfe0', 'tier': 'c', 'key': '0' * 16, 'status': 'pass', 'problems': [],
              'builds': {opt: {'status': 'pass', 'difftest': dict(diff)} for opt in ('-O0', '-O2')}}
    if valid:
        mutate._validate_control_report(report, LEAP, 's', identity)
    else:
        with pytest.raises(ValueError, match='options differ'):
            mutate._validate_control_report(report, LEAP, 's', identity)


def test_hinted_mismatches_cannot_crowd_out_random_case_witnesses(tmp_path):
    # Mutation proofs read witnesses from the reported mismatches; hinted inputs
    # run first, so they get their own report allowance.
    text = lift.render(lift.discover(LEAP))
    assert '    jns v_0054cff8' in text
    altered = text.replace('    jns v_0054cff8', '    js v_0054cff8', 1)
    fixtures = [year_fixture(1991 + 2 * i, i + 1) for i in range(6)]
    _, args = gate(tmp_path, altered, {'fixtures': fixtures}, cases=40)
    result = difftest.run(*args)
    numbered = [m['case'] for m in result['mismatches'] if 'note' not in m]
    assert [n for n in numbered if n >= 40][:5] == [40, 41, 42, 43, 44]
    assert len([n for n in numbered if n < 40]) == 5, result['mismatches']


def test_replaying_one_case_builds_only_what_it_needs(monkeypatch):
    def unavailable(*args):
        raise ValueError('capture unavailable')
    monkeypatch.setattr(difftest, '_capture_case', unavailable)
    opts = difftest.Options.for_function(0x00450A70, {'cases': 6})
    opts.hints = {'fixtures': year_fixtures(3), 'captures': ['a' * 64]}
    templates = difftest.arena_templates()
    assert [n for n, _, _ in difftest.gate_cases(opts, set(), templates, only=2)] == [0, 1, 2]
    assert [n for n, _, _ in difftest.gate_cases(opts, set(), templates, only=7)] == [7]
    with pytest.raises(ValueError, match='capture'):
        list(difftest.gate_cases(opts, set(), templates))
