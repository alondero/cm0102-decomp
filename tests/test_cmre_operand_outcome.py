"""ADR 0010 mutant outcomes when the passing baseline itself has budget-inconclusive cases."""
import pytest

from cmre import operand_evidence

WITNESS = {'case': 234, 'field': 'memory', 'pages': ['0xde1000']}
BUDGET = {'case': 2, 'kind': 'budget-asymmetry', 'original': 'budget', 'recovered': 'fault'}


def report(status, inconclusive, mismatches=()):
    build_status = 'fail' if mismatches else status
    return {'status': status, 'builds': {
        opt: {'status': build_status,
              'difftest': {'inconclusive': inconclusive, 'mismatches': list(mismatches)}}
        for opt in ('-O0', '-O2')}}


def baseline(inconclusive):
    return report('pass', inconclusive)


def test_baseline_equal_inconclusive_with_concrete_witness_is_a_kill():
    outcome = operand_evidence.bridge_mutant_outcome(report('fail', 24, [BUDGET, WITNESS]), baseline(24))
    assert outcome == ('killed', 'differential-mismatch-both-optimizations')


def test_fewer_inconclusive_than_baseline_with_witness_is_a_kill():
    outcome = operand_evidence.bridge_mutant_outcome(report('fail', 20, [WITNESS]), baseline(24))
    assert outcome[0] == 'killed'


def test_more_inconclusive_than_baseline_stays_unresolved():
    outcome = operand_evidence.bridge_mutant_outcome(report('fail', 25, [WITNESS]), baseline(24))
    assert outcome == ('unresolved', '-O0-inconclusive-25-exceeds-baseline-24')


def test_only_budget_asymmetry_at_baseline_inconclusive_stays_unresolved():
    outcome = operand_evidence.bridge_mutant_outcome(report('fail', 24, [BUDGET]), baseline(24))
    assert outcome == ('unresolved', '-O0-witness-missing')


@pytest.mark.parametrize('base', [None, baseline(0)])
def test_any_inconclusive_without_a_baseline_allowance_stays_unresolved(base):
    outcome = operand_evidence.bridge_mutant_outcome(report('fail', 1, [WITNESS]), base)
    assert outcome == ('unresolved', '-O0-inconclusive-1')


@pytest.mark.parametrize('base', [
    {'status': 'pass', 'builds': {}},
    {'status': 'pass', 'builds': {opt: {'difftest': {'inconclusive': -1}} for opt in ('-O0', '-O2')}},
    {'status': 'pass', 'builds': {opt: {'difftest': {'inconclusive': True}} for opt in ('-O0', '-O2')}},
    {'status': 'fail', 'builds': {opt: {'difftest': {'inconclusive': 24}} for opt in ('-O0', '-O2')}},
])
def test_malformed_or_failing_baseline_grants_no_allowance(base):
    outcome = operand_evidence.bridge_mutant_outcome(report('fail', 24, [WITNESS]), base)
    assert outcome == ('unresolved', '-O0-inconclusive-24')


def test_allowance_is_per_optimization_level():
    base = baseline(24)
    base['builds']['-O2']['difftest']['inconclusive'] = 10
    outcome = operand_evidence.bridge_mutant_outcome(report('fail', 24, [WITNESS]), base)
    assert outcome == ('unresolved', '-O2-inconclusive-24-exceeds-baseline-10')
