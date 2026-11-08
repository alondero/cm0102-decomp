"""No-game controller regressions: injected gates are scaffolding only."""
import copy
import hashlib
import json
from contextlib import contextmanager
from pathlib import Path

import pytest

from cmre import mutate


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


@pytest.fixture
def setup(tmp_path):
    source = tmp_path / 'source.c'
    source.write_text('int f(void) { return 1; } // café\n', encoding='utf-8')
    hints = tmp_path / 'hint.json'
    hints.write_text('{}', encoding='utf-8')
    identity = {'execution_identity': {'target': 'pinned'}, 'environment': {}, 'cases': 600,
                'options': {'cases': 600, 'budget': 200000, 'seed': 0x401000, 'inputs': 'v2'}}
    plan = {'schema': 'cmre-control-plan/1', 'va': '0x00401000',
            'source': {'path': str(source), 'sha256': sha(source)},
            'hints': {'path': str(hints), 'sha256': sha(hints)},
            'identity': identity,
            'controls': [{'name': 'return-value', 'transform': {'old': 'return 1', 'new': 'return 2', 'count': 1},
                          'expect': {'kind': 'mismatch', 'field': 'regs'}}]}
    path = tmp_path / 'plan.json'
    calls = []
    held = []

    @contextmanager
    def fence(va, candidate):
        assert not held
        held.append(va)
        try:
            yield candidate
        finally:
            held.pop()

    def gate(va, **kw):
        assert held == [va]
        assert kw == {'source': kw['source'], 'profile': 'semantic', 'fresh': True}
        calls.append(kw['source'].read_text(encoding='utf-8'))
        from cmre import difftest
        difference = difftest._diff(('return', (), (1,), None, {}), ('return', (), (2,), None, {}))
        mismatches = [] if len(calls) == 1 else [{'case': identity['options']['cases'] - 1, **difference}]
        return {'status': 'pass' if not mismatches else 'fail', 'key': f'{len(calls):016x}',
                'va': f'{va:#010x}', 'tier': 'c',
                'execution_identity': identity['execution_identity'], 'profile': 'semantic',
                'source_sha256': sha(kw['source']), 'problems': [],
                'builds': {opt: {'status': 'pass' if not mismatches else 'fail',
                                 'difftest': {'cases': identity['options']['cases'], 'budget': 200000, 'seed': 0x401000, 'inputs': 'v2',
                                              'profile': 'semantic', 'mismatches': copy.deepcopy(mismatches)}}
                           for opt in ('-O0', '-O2')}}

    def builds(va, candidate, directory):
        result = {}
        for opt in ('-O0', '-O2'):
            p = directory / (opt + '.exe')
            p.write_bytes(b'fake-test-PE')
            result[opt] = {'path': str(p), 'sha256': sha(p), 'commands': [['fake-compiler', str(candidate), opt]]}
        return result

    def run(**kw):
        path.write_text(json.dumps(plan, ensure_ascii=False), encoding='utf-8')
        return mutate.run_plan(path, tmp_path / 'bundle', gate=gate, identity=lambda va, src: identity,
                               fence=fence, retain_build=builds, **kw)
    return plan, path, calls, run, identity


def test_named_control_retains_reports_builds_and_missing_trace(setup):
    plan, path, calls, run, identity = setup
    result = run()
    assert len(calls) == 2
    assert result['stages']['return-value']['classification'] == 'killed'
    assert result['trace']['status'] == 'trace_missing'
    assert result['readiness'] == 'proposal'
    assert result['independent_review'] == result['acceptance'] == 'pending'
    assert result['artifact_index']['sha256'] == sha(Path(result['artifact_index']['path']))
    assert 'café' in calls[0]


@pytest.mark.parametrize('change', ['source', 'hint', 'environment', 'missing-hash', 'unknown-field', 'count'])
def test_plan_rejects_drift_before_launch(setup, change):
    plan, path, calls, run, identity = setup
    if change in ('source', 'hint'):
        Path(plan['source' if change == 'source' else 'hints']['path']).write_text('changed')
    elif change == 'environment':
        plan['identity'] = {**identity, 'environment': {'CPATH': 'changed'}}
    elif change == 'missing-hash':
        del plan['source']['sha256']
    elif change == 'unknown-field':
        plan['command'] = 'arbitrary shell'
    else:
        plan['controls'][0]['transform']['count'] = 2
    with pytest.raises(ValueError):
        run()
    assert not calls


@pytest.mark.parametrize('encoding', ['utf-16', 'utf-8-sig'])
def test_plan_rejects_encoding_before_launch(setup, encoding, tmp_path):
    plan, path, calls, run, identity = setup
    path.write_text(json.dumps(plan), encoding=encoding)
    with pytest.raises(ValueError, match='UTF-8'):
        mutate.run_plan(path, tmp_path / 'bundle', gate=lambda *a, **k: calls.append(1))
    assert not calls


def test_resume_reuses_terminal_stages_only_and_rejects_missing_artifact(setup):
    plan, path, calls, run, identity = setup
    result = run()
    assert run(resume=True)['stages'] == result['stages']
    assert len(calls) == 2
    Path(result['stages']['return-value']['report']['path']).unlink()
    with pytest.raises(ValueError, match='artifact'):
        run(resume=True)
    assert len(calls) == 2


def test_interruption_retains_pending_stage_and_never_reexecutes_it(setup, monkeypatch):
    plan, path, calls, run, identity = setup
    original = mutate._execute_control_stage
    monkeypatch.setattr(mutate, '_execute_control_stage', lambda *a, **k: (_ for _ in ()).throw(KeyboardInterrupt()))
    with pytest.raises(KeyboardInterrupt):
        run()
    monkeypatch.setattr(mutate, '_execute_control_stage', original)
    with pytest.raises(ValueError, match='pending'):
        run(resume=True)
    assert not calls


@pytest.mark.parametrize('variation', ['one-opt', 'different-case', 'budget', 'build', 'contract', 'missing-build'])
def test_control_requires_numeric_case_meaningful_mismatch_both_opts(variation):
    report = {'status': 'fail', 'problems': [], 'builds': {
        opt: {'status': 'fail', 'difftest': {'mismatches': [{'case': 4, 'field': 'regs'}]}}
        for opt in ('-O0', '-O2')}}
    if variation == 'one-opt':
        report['builds']['-O2']['difftest']['mismatches'] = []
    elif variation == 'different-case':
        report['builds']['-O2']['difftest']['mismatches'][0]['case'] = '4'
    elif variation == 'budget':
        report['builds']['-O2']['difftest']['mismatches'][0]['kind'] = 'budget-asymmetry'
    elif variation == 'build':
        report['builds']['-O2']['status'] = 'build-failed'
    elif variation == 'contract':
        report['problems'] = ['contract violation']
    else:
        del report['builds']['-O2']
    assert mutate.classify_control(report, {'kind': 'mismatch', 'field': 'regs'})[0] == 'unresolved'


def test_different_numeric_witness_cases_in_each_optimization_are_valid():
    report = {'status': 'fail', 'problems': [], 'builds': {
        opt: {'status': 'fail', 'difftest': {'mismatches': [{'case': case, 'field': 'regs'}]}}
        for opt, case in [('-O0', 4), ('-O2', 17)]}}
    assert mutate.classify_control(report, {'kind': 'mismatch', 'field': 'regs'})[0] == 'killed'


def test_equivalent_extra_is_separate_and_requires_pinned_proof(setup, tmp_path):
    plan, path, calls, run, identity = setup
    proof = tmp_path / 'proof.txt'
    proof.write_text('Native instruction review reference')
    plan['controls'][0]['expect'] = {'kind': 'equivalent', 'proof': {'path': str(proof), 'sha256': sha(proof)}}
    result = run()
    assert result['stages']['return-value']['classification'] == 'unexpected-mismatch'
    proof.write_text('drift')
    with pytest.raises(ValueError):
        run(resume=True)


def test_successful_equivalence_is_not_counted_as_a_kill():
    report = {'status': 'pass', 'builds': {opt: {'status': 'pass', 'difftest': {'mismatches': []}} for opt in ('-O0', '-O2')}}
    assert mutate.classify_control(report, {'kind': 'equivalent'}) == ('expected-equivalent', None)


def test_resume_rejects_identity_drift_before_launch(setup):
    plan, path, calls, run, identity = setup
    run()
    identity['environment']['CPATH'] = 'changed'
    with pytest.raises(ValueError, match='drift'):
        run(resume=True)
    assert len(calls) == 2


def test_partial_terminal_resume_does_not_rerun_success(setup, monkeypatch):
    plan, path, calls, run, identity = setup
    from cmre import coordination
    original = coordination.atomic_json
    def interrupted(path, data):
        if Path(path).name == 'controller.json' and data['stages'].get('baseline', {}).get('state') == 'terminal':
            original(path, data)
            raise KeyboardInterrupt()
        return original(path, data)
    monkeypatch.setattr(coordination, 'atomic_json', interrupted)
    with pytest.raises(KeyboardInterrupt):
        run()
    assert len(calls) == 1
    monkeypatch.setattr(coordination, 'atomic_json', original)
    assert run(resume=True)['status'] == 'controls-complete'
    assert len(calls) == 2


def test_frozen_private_control_is_pinned_and_retained(setup, tmp_path):
    plan, path, calls, run, identity = setup
    private = tmp_path / 'frozen.c'
    private.write_text('int f(void) { return 2; }', encoding='utf-8')
    del plan['controls'][0]['transform']
    plan['controls'][0]['source'] = {'path': str(private), 'sha256': sha(private)}
    assert run()['stages']['return-value']['classification'] == 'killed'
    private.unlink()
    with pytest.raises(ValueError):
        run(resume=True)
    assert len(calls) == 2


def test_control_plan_cli_routing_is_opt_in(setup, monkeypatch, tmp_path, capsys):
    from cmre import cli
    plan, path, calls, run, identity = setup
    path.write_text(json.dumps(plan), encoding='utf-8')
    seen = []
    monkeypatch.setattr(mutate, 'run_plan', lambda *a, **kw: seen.append((a, kw)) or {'readiness': 'proposal', 'status': 'controls-complete'})
    assert cli.main(['mutate', 'fn_00401000', '--plan', str(path), '--output', str(tmp_path / 'out'), '--resume']) == 0
    assert seen[0][1] == {'resume': True, 'fence': cli._diagnostic_source}
    assert 'proposal' in capsys.readouterr().out
    assert cli.main(['mutate', 'fn_00401001', '--plan', str(path), '--output', str(tmp_path / 'out')]) != 0
    assert 'VA differs' in capsys.readouterr().err


@pytest.mark.parametrize('cases', [600, 2432])
def test_default_identity_captures_build_environment_without_game(setup, monkeypatch, cases):
    from cmre import difftest, recover
    plan, path, calls, run, identity = setup
    monkeypatch.setattr(recover, 'c_profile', lambda va: 'semantic')
    monkeypatch.setattr(recover, 'c_inputs', lambda va: 'v2')
    monkeypatch.setattr(recover, 'execution_identity', lambda: {'target': 'synthetic'})
    monkeypatch.setattr(recover, 'c_dependencies', lambda p: {'files': {}, 'preprocessed': {}})
    monkeypatch.setattr(difftest.Options, 'for_function', lambda *a, **kw: difftest.Options(cases, 200000, 24, 10, .6, 1, {}))
    monkeypatch.setenv('CPATH', 'selected-include')
    captured = mutate.control_identity(0x401000, Path(plan['source']['path']))
    assert captured['environment']['CPATH'] == 'selected-include'
    assert captured['options']['inputs'] == 'v2'
    assert captured['optimizations'] == ['-O0', '-O2']
    assert captured['cases'] == captured['options']['cases'] == cases


def test_normal_2432_case_plan_retains_late_witnesses_and_resumes(setup):
    plan, path, calls, run, identity = setup
    identity['cases'] = identity['options']['cases'] = 2432
    result = run()
    assert result['status'] == 'controls-complete'
    control = result['stages']['return-value']
    assert control['classification'] == 'killed'
    assert {opt: [m['case'] for m in matches] for opt, matches in control['witnesses'].items()} == {
        '-O0': [2431], '-O2': [2431]}
    assert run(resume=True)['stages'] == result['stages']
    assert len(calls) == 2


@pytest.mark.parametrize('cases,index,killed', [(600, 599, True), (600, 600, False),
                                              (2432, 2431, True), (2432, 2432, False)])
def test_control_witness_must_fit_pinned_case_count(cases, index, killed):
    report = {'status': 'fail', 'problems': [], 'builds': {
        opt: {'status': 'fail', 'difftest': {'mismatches': [{'case': index, 'field': 'regs'}]}}
        for opt in ('-O0', '-O2')}}
    classification, _ = mutate.classify_control(report, {'kind': 'mismatch', 'field': 'regs'}, cases=cases)
    assert (classification == 'killed') is killed


@pytest.mark.parametrize('cases,option_cases', [(2432, 600), (600, 2432), (True, True), (0, 0)])
def test_plan_rejects_inconsistent_case_pins_before_gate(setup, cases, option_cases):
    plan, path, calls, run, identity = setup
    identity['cases'], identity['options']['cases'] = cases, option_cases
    with pytest.raises(ValueError, match='case|identity'):
        run()
    assert not calls


@pytest.mark.parametrize('field,value', [('cases', 600), ('cases', 2431), ('cases', 2433), ('cases', True),
                                       ('budget', 200001), ('seed', 7), ('inputs', 'v1')])
def test_report_rejects_wrong_scope_for_normal_2432_cases(setup, field, value):
    plan, path, calls, run, identity = setup
    identity['cases'] = identity['options']['cases'] = 2432
    report = {'status': 'pass', 'key': 'a' * 16, 'va': plan['va'], 'tier': 'c',
              'execution_identity': identity['execution_identity'], 'profile': 'semantic',
              'source_sha256': plan['source']['sha256'], 'problems': [],
              'builds': {opt: {'status': 'pass', 'difftest': {
                  'cases': 2432, 'budget': 200000, 'seed': 0x401000, 'inputs': 'v2',
                  'profile': 'semantic', 'mismatches': []}} for opt in ('-O0', '-O2')}}
    report['builds']['-O0']['difftest'][field] = value
    with pytest.raises(ValueError, match='effective options'):
        mutate._validate_control_report(report, 0x401000, plan['source']['sha256'], identity)
    assert not calls


def test_stale_600_identity_rejected_before_2432_gate(setup):
    plan, path, calls, run, identity = setup
    plan['identity'] = copy.deepcopy(identity)
    identity['cases'] = identity['options']['cases'] = 2432
    with pytest.raises(ValueError, match='identity|drift'):
        run()
    assert not calls


@pytest.mark.parametrize('field,value', [('budget', 200001), ('seed', 7), ('inputs', 'v1')])
def test_effective_option_drift_rejected_before_2432_gate(setup, field, value):
    plan, path, calls, run, identity = setup
    identity['cases'] = identity['options']['cases'] = 2432
    plan['identity'] = copy.deepcopy(identity)
    identity['options'][field] = value
    with pytest.raises(ValueError, match='identity|drift'):
        run()
    assert not calls


def test_candidate_drift_during_build_is_unresolved(setup, monkeypatch):
    plan, path, calls, run, identity = setup
    original = mutate._execute_control_stage
    def corrupt(va, candidate, directory, gate, retain, expected, check):
        def build(*args):
            result = retain(*args)
            candidate.write_text('changed after gate')
            return result
        return original(va, candidate, directory, gate, build, expected, check)
    monkeypatch.setattr(mutate, '_execute_control_stage', corrupt)
    result = run()
    assert result['stages']['baseline']['classification'] == 'unresolved'
    assert result['status'] == 'incomplete'
    assert len(calls) == 1


@pytest.mark.parametrize('damage', ['empty-stage', 'schema', 'source', 'report', 'builds', 'command', 'artifact-list', 'extra-stage'])
def test_resume_rejects_malformed_terminal_evidence_before_any_launch(setup, tmp_path, damage):
    plan, path, calls, run, identity = setup
    run()
    receipt_path = tmp_path / 'bundle/controller.json'
    receipt = json.loads(receipt_path.read_text(encoding='utf-8'))
    stage = receipt['stages']['return-value']
    if damage == 'empty-stage':
        receipt['stages']['return-value'] = {'state': 'terminal', 'classification': 'killed', 'artifacts': []}
        del receipt['artifact_index'], receipt['status']
        (tmp_path / 'bundle/artifact-index.json').unlink()
    elif damage == 'schema':
        receipt['schema'] = 'unknown'
    elif damage == 'artifact-list':
        stage['artifacts'] = []
    elif damage == 'extra-stage':
        receipt['stages']['invented'] = copy.deepcopy(stage)
    else:
        del stage[damage]
    receipt_path.write_text(json.dumps(receipt), encoding='utf-8')
    with pytest.raises(ValueError):
        run(resume=True)
    assert len(calls) == 2


def test_resume_recomputes_classification_from_hashed_report(setup, tmp_path):
    plan, path, calls, run, identity = setup
    run()
    receipt_path = tmp_path / 'bundle/controller.json'
    receipt = json.loads(receipt_path.read_text(encoding='utf-8'))
    receipt['stages']['return-value']['classification'] = 'survivor'
    receipt_path.write_text(json.dumps(receipt), encoding='utf-8')
    result = run(resume=True)
    assert result['stages']['return-value']['classification'] == 'killed'
    assert len(calls) == 2


def test_successful_controls_are_complete_without_unrequested_trace(setup):
    plan, path, calls, run, identity = setup
    result = run()
    assert result['status'] == 'controls-complete'
    assert result['gate_pe_retained'] is False
    assert result['trace']['status'] == 'trace_missing'
    assert result['independent_review'] == result['acceptance'] == 'pending'
    assert all(b['role'] == 'diagnostic-rebuild' for b in result['stages']['baseline']['builds'].values())


@pytest.mark.parametrize('stage', ['raw-trace', 'actual-gate-pe'])
def test_unsupported_required_stage_rejected_before_launch(setup, stage):
    plan, path, calls, run, identity = setup
    plan['required_stages'] = [stage]
    with pytest.raises(ValueError, match='Unsupported required stage'):
        run()
    assert not calls


def test_integer_return_observable_comes_from_unchanged_real_comparator():
    from cmre import difftest
    difference = difftest._diff(('return', (), (1,), None, {}), ('return', (), (2,), None, {}))
    assert difference['field'] == 'regs'
    assert difference['field'] in mutate.CONTROL_FIELDS


def test_unsupported_observable_is_rejected_before_launch(setup):
    plan, path, calls, run, identity = setup
    plan['controls'][0]['expect']['field'] = 'eax'
    with pytest.raises(ValueError, match='observable'):
        run()
    assert not calls


@pytest.mark.parametrize('damage', ['identity', 'source_sha256', 'cases', 'budget', 'seed', 'inputs', 'profile', 'build-role', 'build-command'])
def test_rehashed_report_or_build_drift_is_rejected_on_partial_resume(setup, tmp_path, damage):
    plan, path, calls, run, identity = setup
    run()
    receipt_path = tmp_path / 'bundle/controller.json'
    receipt = json.loads(receipt_path.read_text(encoding='utf-8'))
    stage = receipt['stages']['return-value']
    if damage.startswith('build-'):
        if damage == 'build-role':
            stage['builds']['-O0']['role'] = 'actual-gate'
        else:
            stage['builds']['-O0']['commands'] = []
        (tmp_path / 'bundle/return-value/builds.json').write_text(json.dumps(stage['builds']), encoding='utf-8')
    else:
        report_path = Path(stage['report']['path'])
        report = json.loads(report_path.read_text(encoding='utf-8'))
        if damage == 'identity':
            report['execution_identity'] = {'target': 'different'}
        elif damage == 'source_sha256':
            report['source_sha256'] = '0' * 64
        else:
            report['builds']['-O0']['difftest'][damage] = 'tampered'
        report_path.write_text(json.dumps(report), encoding='utf-8')
        stage['report']['sha256'] = sha(report_path)
    stage['artifacts'] = [mutate._pin(p) for p in sorted((tmp_path / 'bundle/return-value').rglob('*')) if p.is_file()]
    del receipt['artifact_index'], receipt['status']
    (tmp_path / 'bundle/artifact-index.json').unlink()
    receipt_path.write_text(json.dumps(receipt), encoding='utf-8')
    with pytest.raises(ValueError):
        run(resume=True)
    assert len(calls) == 2


def test_cli_returns_failure_for_unresolved_selected_controls(setup, monkeypatch, tmp_path):
    from cmre import cli
    plan, path, calls, run, identity = setup
    path.write_text(json.dumps(plan), encoding='utf-8')
    monkeypatch.setattr(mutate, 'run_plan', lambda *a, **kw: {'status': 'incomplete'})
    assert cli.main(['mutate', 'fn_00401000', '--plan', str(path), '--output', str(tmp_path / 'out')]) == 1
