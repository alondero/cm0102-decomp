"""Composition impact must follow recorded execution-tier provenance."""
import json
from types import SimpleNamespace

import pytest

from cmre import composition, env, recover, validation


@pytest.fixture
def impact_fixture(tmp_path, monkeypatch):
    analysis = tmp_path / 'analysis'
    compositions = analysis / 'cmre' / 'compositions'
    compositions.mkdir(parents=True)
    monkeypatch.setattr(env, 'ANALYSIS', analysis)
    monkeypatch.setattr(env, 'C_DIR', tmp_path / 'c')
    monkeypatch.setattr(env, 'ROOT', tmp_path)
    monkeypatch.setattr(composition, '_LEGACY_LIFT_SOURCE', 'lift')
    monkeypatch.setattr(composition, '_LEGACY_FILES', {
        'recovery/date/ledger.json': 'ledger',
        'recovery/date/cm_date.c': 'source',
        'recovery/date/cm_date.h': 'header',
    })
    report = {
        'campaign': {'functions': ['fn_00417c10']}, 'status': 'pass',
        'execution_identity': {'target': 'target'}, 'composition_sha256': 'comp',
        'objdump': {'path': str(tmp_path / 'objdump'), 'version': 'test objdump', 'sha256': 'objdump'},
        'source_sha256': {'fn_00417c10.S': 'lift'},
        'execution_tiers': {'fn_00417c10': {
            'execution_tier': 'lift-companion', 'provenance': 'fresh-original-derived-legacy',
            'ledger': 'recovery/date/ledger.json', 'source_sha256': 'lift',
            'legacy_dependencies': {'recovery/date/ledger.json': 'ledger',
                                    'recovery/date/cm_date.c': 'source',
                                    'recovery/date/cm_date.h': 'header'}}},
        'source_dependencies': {'fixture': {'files': {}}},
        'options': {'fn_00417c10': {'inputs': 'v2'}},
    }
    paths = {tmp_path / 'recovery/date/ledger.json': 'ledger',
             tmp_path / 'objdump': 'objdump',
             tmp_path / 'recovery/date/cm_date.c': 'source',
             tmp_path / 'recovery/date/cm_date.h': 'header',
             # Deliberate same-name collision: never use this as lift provenance.
             tmp_path / 'c/fn_00417c10.S': 'lift'}
    monkeypatch.setattr(validation, '_file_digest', lambda path: paths.get(path))
    monkeypatch.setattr(env, 'digest', lambda path: 'comp')
    monkeypatch.setattr(env, 'tool', lambda name: str(tmp_path / 'objdump'))
    monkeypatch.setattr(env, 'run_tool', lambda args: SimpleNamespace(returncode=0, stdout='test objdump\n'))
    policy = {'default': 'v2', 'accepted': 'v2'}
    monkeypatch.setattr(validation.difftest.Options, 'for_function',
                        lambda *a, **k: SimpleNamespace(inputs=policy['default']))
    monkeypatch.setattr(recover, 'c_inputs', lambda *a: policy['accepted'])
    monkeypatch.setattr(recover, 'c_dependencies', lambda path: {
        'files': {'c/' + path.name: 'c-source'},
    })
    def run():
        (compositions / 'report.json').write_text(json.dumps(report), encoding='utf-8')
        return validation.composition_impact({}, {'target': 'target'})[0]
    return report, paths, run, policy


def test_unchanged_legacy_lift_and_v2_policy_are_not_invalidated(impact_fixture):
    _, _, run, policy = impact_fixture
    policy['default'] = 'v1'
    assert run()['action'] == 'outside selection; no new claim'


@pytest.mark.parametrize('name', ['ledger.json', 'cm_date.c', 'cm_date.h'])
def test_legacy_drift_is_detected_despite_same_name_c_directory_collision(impact_fixture, name):
    _, paths, run, _ = impact_fixture
    path = next(p for p in paths if p.name == name)
    paths[path] = 'changed'
    assert run()['action'] == 'revalidate separately'


@pytest.mark.parametrize('tier', [None, {'execution_tier': 'unknown'}])
def test_unknown_or_missing_tier_is_not_silently_accepted(impact_fixture, tier):
    report, _, run, _ = impact_fixture
    if tier is None:
        report['execution_tiers'] = {}
    else:
        report['execution_tiers']['fn_00417c10'] = tier
    assert run()['action'] in ('revalidate separately', 'unknown dependencies; revalidate separately')


@pytest.mark.parametrize('change', ['missing', 'hash', 'version'])
def test_object_inspector_drift_is_not_silently_accepted(impact_fixture, change):
    report, _, run, _ = impact_fixture
    if change == 'missing':
        report.pop('objdump')
    else:
        report['objdump'][{'hash': 'sha256', 'version': 'version'}[change]] = 'changed'
    assert run()['action'] == 'revalidate separately'


def test_malformed_build_inputs_are_not_accepted(impact_fixture):
    report, _, run, _ = impact_fixture
    report['source_dependencies'] = {'fixture': []}
    assert run()['action'] == 'unknown dependencies; revalidate separately'


def test_build_environment_override_prevents_unchanged_claim(impact_fixture, monkeypatch):
    _, _, run, _ = impact_fixture
    monkeypatch.setenv('LIBRARY_PATH', 'changed')
    assert run()['action'] == 'revalidate separately'


def test_dependency_drift_during_impact_check_invalidates_result(impact_fixture, monkeypatch):
    report, paths, run, policy = impact_fixture
    policy['default'] = policy['accepted'] = 'v1'
    report.pop('execution_tiers')
    report['campaign']['functions'] = ['fn_004671b0']
    report['source_sha256'] = {'fn_004671b0.c': 'c-source'}
    report['source_dependencies'] = {'fn_004671b0.c': {'files': {'c/fn_004671b0.c': 'c-source'}}}
    report['options'] = {'fn_004671b0': {'inputs': 'v1'}}
    paths[env.C_DIR / 'fn_004671b0.c'] = 'c-source'
    values = iter([{'files': {'c/fn_004671b0.c': 'c-source'}}, {'files': {'c/fn_004671b0.c': 'changed'}}])
    monkeypatch.setattr(recover, 'c_dependencies', lambda path: next(values))
    assert run()['action'] == 'revalidate separately'


@pytest.mark.parametrize('kind', ['legacy', 'inspector', 'policy', 'report'])
def test_non_c_input_drift_during_impact_check_invalidates_result(impact_fixture, monkeypatch, kind):
    _, paths, run, policy = impact_fixture
    if kind == 'legacy':
        original = validation._file_digest
        calls = {'n': 0}
        def digest(path):
            if path.name == 'ledger.json':
                calls['n'] += 1
                if calls['n'] > 1:
                    return 'changed'
            return original(path)
        monkeypatch.setattr(validation, '_file_digest', digest)
    elif kind == 'inspector':
        calls = {'n': 0}
        def version(args):
            calls['n'] += 1
            return SimpleNamespace(returncode=0, stdout=('test objdump\n' if calls['n'] == 1 else 'changed\n'))
        monkeypatch.setattr(env, 'run_tool', version)
    elif kind == 'policy':
        calls = {'n': 0}
        def inputs(*args):
            calls['n'] += 1
            return policy['accepted'] if calls['n'] == 1 else 'changed'
        monkeypatch.setattr(recover, 'c_inputs', inputs)
    else:
        original = validation.read
        def read_then_change(path):
            result = original(path)
            path.write_text(json.dumps({**result, 'status': 'changed'}), encoding='utf-8')
            return result
        monkeypatch.setattr(validation, 'read', read_then_change)
    assert run()['action'] == 'revalidate separately'


@pytest.mark.parametrize('change', ['missing-header', 'changed-preprocessed', 'added-include'])
def test_incomplete_full_c_dependency_snapshot_is_not_accepted(impact_fixture, monkeypatch, change):
    report, paths, run, policy = impact_fixture
    policy['default'] = policy['accepted'] = 'v1'
    report.pop('execution_tiers')
    report['campaign']['functions'] = ['fn_004671b0']
    report['source_sha256'] = {'fn_004671b0.c': 'c-source'}
    recorded = {'files': {'c/fn_004671b0.c': 'c-source', 'recovery/include/cmre.h': 'header'},
                'preprocessed': {'-O0': 'p0', '-O2': 'p2'}}
    current = json.loads(json.dumps(recorded))
    paths[env.C_DIR / 'fn_004671b0.c'] = 'c-source'
    paths[env.ROOT / 'recovery/include/cmre.h'] = 'header'
    if change == 'missing-header':
        del recorded['files']['recovery/include/cmre.h']
    elif change == 'changed-preprocessed':
        recorded['preprocessed']['-O2'] = 'stale'
    else:
        current['files']['recovery/include/other.h'] = 'new'
    report['source_dependencies'] = {'fn_004671b0.c': recorded}
    report['options'] = {'fn_004671b0': {'inputs': 'v1'}}
    monkeypatch.setattr(recover, 'c_dependencies', lambda path: current)
    assert run()['action'] == 'revalidate separately'


def test_older_c_only_report_without_tiers_remains_readable(impact_fixture):
    report, paths, run, policy = impact_fixture
    policy['default'] = policy['accepted'] = 'v1'
    report.pop('execution_tiers')
    report['campaign']['functions'] = ['fn_004671b0']
    report['source_sha256'] = {'fn_004671b0.c': 'c-source'}
    report['source_dependencies'] = {'fn_004671b0.c': {'files': {'c/fn_004671b0.c': 'c-source'}}}
    report['options'] = {'fn_004671b0': {'inputs': 'v1'}}
    paths[env.C_DIR / 'fn_004671b0.c'] = 'c-source'
    assert run()['action'] == 'outside selection; no new claim'


@pytest.mark.parametrize('missing', ['options', 'member-option', 'member-dependencies',
                                      'dependency-source', 'source-hash', 'source-file'])
def test_incomplete_c_member_evidence_fails_closed(impact_fixture, missing):
    report, paths, run, policy = impact_fixture
    policy['default'] = policy['accepted'] = 'v1'
    report.pop('execution_tiers')
    report['campaign']['functions'] = ['fn_004671b0']
    report['source_sha256'] = {'fn_004671b0.c': 'c-source'}
    report['source_dependencies'] = {'fn_004671b0.c': {'files': {'c/fn_004671b0.c': 'c-source'}}}
    report['options'] = {'fn_004671b0': {'inputs': 'v1'}}
    paths[env.C_DIR / 'fn_004671b0.c'] = 'c-source'
    if missing == 'options':
        report.pop('options')
    elif missing == 'member-option':
        report['options'] = {'fn_00549ef0': {'inputs': 'v1'}}
    elif missing == 'member-dependencies':
        report['source_dependencies'] = {'other.c': {'files': {}}}
    elif missing == 'dependency-source':
        report['source_dependencies']['fn_004671b0.c']['files'] = {}
    elif missing == 'source-hash':
        report['source_sha256'] = {}
    else:
        del paths[env.C_DIR / 'fn_004671b0.c']
    assert run()['action'] in ('revalidate separately', 'unknown dependencies; revalidate separately')
