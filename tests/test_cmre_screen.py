"""Dispatch screening retains the full corpus and cannot establish acceptance."""
import json
import random
from types import SimpleNamespace

import pytest

from cmre import build, difftest, emu, env, lift, recover, screen

VA = 0x401000


@pytest.fixture
def simulation(tmp_path, monkeypatch):
    monkeypatch.setattr(env, 'ANALYSIS', tmp_path)
    monkeypatch.setattr(env, 'ROOT', tmp_path)
    opts = difftest.Options(4, 20, 2, 0, 0.6, VA, {}, 'v2')
    lifted = SimpleNamespace(va=VA, region_end=VA + 8, insns={VA: None, VA + 7: None}, problems=[])
    monkeypatch.setattr(lift, 'discover', lambda va: lifted)
    monkeypatch.setattr(difftest.Options, 'for_function', lambda va, *, tier: opts)
    monkeypatch.setattr(recover, 'c_inputs', lambda va: 'v2')
    monkeypatch.setattr(recover, 'c_profile', lambda va: 'semantic')
    monkeypatch.setattr(env, 'HINTS_DIR', tmp_path / 'hints')
    identity = {'target': env.TARGET_SHA256,
                'inventory': {env.rel(p): h for p, h in env.INVENTORY_SHA256.items()}}
    monkeypatch.setattr(recover, 'execution_identity', lambda: dict(identity))
    monkeypatch.setattr(screen.image, 'primary', lambda: object())
    monkeypatch.setattr(difftest, 'arena_templates', lambda: ())
    monkeypatch.setattr(recover, '_indirect_calls', lambda lifted: {'original-site': 'shape'})
    armed, calls, discovered = [], [], []
    monkeypatch.setattr(difftest, '_stop_indirect', lambda *args: armed.append(args))
    monkeypatch.setattr(difftest, 'make_case', lambda rng, opts, globals_in, templates:
                        (rng.randrange(1000), opts.inputs, tuple(globals_in)))
    monkeypatch.setattr(difftest, '_global_reads', lambda *args: (discovered.append(args) or {VA + 0x100}))
    outcomes = ['fault'] * 4

    class Machine:
        def __init__(self, img, *, watch):
            assert watch == (VA, VA + 8)

        def run(self, va, case, budget):
            assert va == VA and budget == opts.budget
            calls.append(case)
            outcome = outcomes[len(calls) - 1]
            detail = ((19, 0, 4), VA + 0x100) if outcome == 'fault' else ()
            return emu.Result(outcome, detail, (), [], {}, {VA})

    monkeypatch.setattr(emu, 'Machine', Machine)
    return SimpleNamespace(opts=opts, lifted=lifted, identity=identity, calls=calls,
                           outcomes=outcomes, armed=armed, discovered=discovered)


def test_all_fault_cases_hold_without_task_attempts_or_candidate_build(simulation):
    result = screen.run(VA)
    assert result['status'] == 'evidence-needed'
    assert result['outcomes'] == {'fault': 4}
    assert result['first_return_case'] is None
    assert result['stops'] == {'fault in callee eip=0x00401100 read-unmapped': 4}
    assert result['coverage'] == 0.5
    assert len(simulation.calls) == 4
    assert not (env.ANALYSIS / 'cmre/tasks').exists()


def test_return_observation_keeps_later_faults_and_exact_discovery_sequence(simulation):
    simulation.outcomes[0] = 'return'
    result = screen.run(VA)
    assert result['status'] == 'return-observed' and result['first_return_case'] == 0
    assert result['outcomes'] == {'fault': 3, 'return': 1}
    rng = random.Random(VA)
    assert simulation.calls == [(rng.randrange(1000), 'v2', () if i == 0 else (VA + 0x100,)) for i in range(4)]
    assert len(simulation.discovered) == 2
    assert simulation.discovered[0][2] == (VA, VA + 8)
    assert simulation.armed[0][1] is None  # no recovered candidate/stub
    assert simulation.armed[0][-1] == {'original-site': 'shape'}


def test_reuse_is_identity_bound_and_retains_prior_receipts(simulation):
    first = screen.run(VA)
    assert not first['reused']
    assert screen.run(VA)['reused']
    assert len(simulation.calls) == 4
    simulation.calls.clear()
    simulation.opts.hints = {'ecx': 'ptr'}
    second = screen.run(VA)
    assert not second['reused'] and second['receipt'] != first['receipt']
    assert (env.ROOT / first['receipt']).is_file()
    simulation.calls.clear()
    simulation.identity['harness'] = 'changed'
    assert not screen.run(VA)['reused']


def test_tampered_cache_is_retained_but_never_reused(simulation):
    first = screen.run(VA)
    path = env.ROOT / first['receipt']
    data = json.loads(path.read_text())
    data['status'] = 'return-observed'
    path.write_text(json.dumps(data))
    simulation.calls.clear()
    second = screen.run(VA)
    assert not second['reused'] and second['status'] == 'evidence-needed'
    assert path.exists() and second['receipt'] != first['receipt']


def test_unpinned_target_rejected_before_execution(simulation):
    simulation.identity['target'] = 'different binary'
    with pytest.raises(ValueError, match='pinned'):
        screen.run(VA)
    assert not simulation.calls


def test_hint_bytes_invalidate_screen_even_when_effective_options_are_unchanged(simulation):
    first = screen.run(VA)
    env.HINTS_DIR.mkdir()
    (env.HINTS_DIR / 'fn_00401000.json').write_text('{}')
    simulation.calls.clear()
    second = screen.run(VA)
    assert not second['reused'] and second['receipt'] != first['receipt']


def test_mid_run_input_drift_never_publishes_reusable_result(simulation, monkeypatch):
    original = screen.observe
    def drift(*args):
        result = original(*args)
        simulation.identity['harness'] = 'changed'
        return result
    monkeypatch.setattr(screen, 'observe', drift)
    with pytest.raises(ValueError, match='changed during'):
        screen.run(VA)
    assert not list(env.ANALYSIS.rglob('latest.json'))


def test_real_original_screen_matches_c_differential_input_outcomes(tmp_path):
    # A real original-side run checks that the observer is usable without a C file.
    va = 0x0054CFE0
    lifted = lift.discover(va)
    opts = difftest.Options.for_function(va, {'cases': 8}, tier='c')
    opts.inputs = recover.c_inputs(va)
    result = screen.observe(va, lifted, opts)
    source = tmp_path / 'faithful.S'
    source.write_text(lift.render(lifted), encoding='utf-8')
    built = build.build(source, lifted.symbol, tmp_path)
    paired = difftest.run(va, (va, lifted.region_end), set(lifted.insns), built,
                          recover._call_returns(lifted, built), opts, 'semantic',
                          indirect=recover._indirect_calls(lifted))
    assert result['outcomes'] == paired['outcomes']
    assert round(result['coverage'], 4) == paired['coverage']
    assert not paired['mismatches']
    assert result['outcomes'] == {'return': 8}
    assert result['first_return_case'] == 0
    assert result['coverage'] >= 0.6
