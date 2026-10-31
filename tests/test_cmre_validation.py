"""Impact selection and reuse must save work without hiding changed evidence."""
import copy
import json

import pytest

from cmre import env, ledger, queue, recover, validation as v, validation_compat as compat


@pytest.fixture
def corpus(tmp_path, monkeypatch):
    monkeypatch.setattr(queue, 'coverage_summary', lambda: {
        'inventory_functions': 2, 'inventory_body_bytes': 20,
        'legacy_functions': 0, 'legacy_body_bytes': 0,
        'source_functions': 2, 'source_body_bytes': 20,
        'c_tier_functions': 2, 'c_tier_body_bytes': 20,
        'lifted_functions': 2, 'recovered_functions': 2,
        'recovered_body_bytes': 20, 'integrated_functions': 0})
    monkeypatch.setattr(env, 'ROOT', tmp_path)
    monkeypatch.setattr(env, 'ANALYSIS', tmp_path / 'analysis')
    for name, suffix in [('C_DIR', 'c'), ('HINTS_DIR', 'hints'), ('LIFTED_DIR', 'lifted'), ('LEDGER_DIR', 'ledger')]:
        path = tmp_path / suffix
        path.mkdir()
        monkeypatch.setattr(env, name, path)
    (tmp_path / 'recovery/include').mkdir(parents=True)
    header = tmp_path / 'recovery/include/shared.h'
    header.write_text('one')
    (tmp_path / 'recovery/include/unrelated.h').write_text('one')
    for va in (1, 2):
        (env.LIFTED_DIR / f'fn_{va:08x}.S').write_text('original assembly')
        source = env.C_DIR / f'fn_{va:08x}.c'
        source.write_text('C source')
        ledger.record_path(va).write_text(json.dumps({'lifted': {'source_sha256': 'lift'},
                                                      'c': {'source_sha256': env.digest(source)}}))
    identity = {'harness': 'original', 'target': 'pinned', 'policy': 'original'}
    monkeypatch.setattr(v, 'global_identity', lambda: copy.deepcopy(identity))
    monkeypatch.setattr(v, 'worker_identity', lambda: v.global_identity())

    def dependencies(source):
        files = {env.rel(source): env.digest(source)}
        if '00000001' in source.name:
            files[env.rel(header)] = env.digest(header)
        return {'files': files, 'preprocessed': {'-O0': v.digest(files), '-O2': v.digest(files)}}
    monkeypatch.setattr(recover, 'c_dependencies', dependencies)
    return identity


def complete(plan, monkeypatch, fail=None):
    monkeypatch.setattr(v, 'execute_item', lambda va, tier, refresh: (f'{va:08x}-{tier}' != fail, 'pass' if not fail else 'mismatch'))
    run = v.Run(plan)
    for key, item in plan['items'].items():
        if item['action'] == 'execute':
            run.record(key, v.execute(item, plan['identity'], plan['refresh']))
    run.finish()
    return run


def selected(plan):
    return {key for key, item in plan['items'].items() if item['action'] == 'execute'}


def test_unchanged_repeat_executes_nothing_and_keeps_origin(corpus, monkeypatch):
    first = complete(v.plan([1, 2]), monkeypatch)
    original = first.path.read_bytes()
    plan = v.plan([1, 2])
    assert not selected(plan)
    monkeypatch.setattr(v, 'execute_item', lambda *a: pytest.fail('unchanged evidence executed'))
    second = v.Run(plan)
    second.finish()
    assert v.summary(second.data)['reused'] == 4
    assert first.path.read_bytes() == original
    assert second.data['results']['00000001-c']['origin']['receipt'] == str(first.path)


def test_c_only_hints_preserve_lifted_inputs(corpus, monkeypatch):
    complete(v.plan([1, 2]), monkeypatch)
    (env.HINTS_DIR / 'fn_00000001.json').write_text('{"c_hints": {"ecx": "ptr"}}')
    assert selected(v.plan([1, 2])) == {'00000001-c'}


def test_fixture_payloads_are_hashed_instead_of_copied_into_receipts(corpus):
    # Large valid metadata stands in for fixture bytes; all effective fields
    # are hashed, including metadata and fixture contents.
    hints = env.HINTS_DIR / 'fn_00000001.json'
    hints.write_text(json.dumps({'c_hints': {'fixture_evidence': 'x' * 1_000_000}}))
    plan = v.plan([1])
    assert len(json.dumps(plan)) < 20_000
    before = plan['items']['00000001-c']['dependencies']['options']['effective_hints_sha256']
    hints.write_text(json.dumps({'c_hints': {'fixture_evidence': 'y' * 1_000_000}}))
    assert v.plan([1])['items']['00000001-c']['dependencies']['options']['effective_hints_sha256'] != before


@pytest.mark.parametrize('hints', [{'seed': 123}, {'cases': 123}, {'min_coverage': 0.9}, {'ecx': 'ptr'}])
def test_shared_hints_invalidate_both_tiers(corpus, monkeypatch, hints):
    complete(v.plan([1, 2]), monkeypatch)
    (env.HINTS_DIR / 'fn_00000001.json').write_text(json.dumps(hints))
    assert selected(v.plan([1, 2])) == {'00000001-c', '00000001-lifted'}


def test_c_source_and_actual_header_dependents(corpus, monkeypatch):
    complete(v.plan([1, 2]), monkeypatch)
    (env.ROOT / 'recovery/include/shared.h').write_text('two')
    assert selected(v.plan([1, 2])) == {'00000001-c'}
    (env.C_DIR / 'fn_00000002.c').write_text('different C')
    assert selected(v.plan([1, 2])) == {'00000001-c', '00000002-c'}


def test_unrelated_header_does_not_invalidate_covered_build(corpus, monkeypatch):
    complete(v.plan([1, 2]), monkeypatch)
    (env.ROOT / 'recovery/include/unrelated.h').write_text('two')
    assert not selected(v.plan([1, 2]))
    # A newly available include can affect __has_include and must be conservative.
    (env.ROOT / 'recovery/include/new.h').write_text('two')
    # New include availability conservatively invalidates C dependency coverage.
    assert selected(v.plan([1, 2])) == {'00000001-c', '00000002-c'}


def test_volatile_preprocessor_dependencies_are_recomputed(corpus, monkeypatch):
    complete(v.plan([1, 2]), monkeypatch)
    source = env.C_DIR / 'fn_00000001.c'
    source.write_text('const char *x = __TIME__;')
    complete(v.plan([1, 2]), monkeypatch)
    calls = []
    original = recover.c_dependencies
    def dependencies(path):
        calls.append(path)
        return original(path)
    monkeypatch.setattr(recover, 'c_dependencies', dependencies)
    v.plan([1, 2])
    assert calls == [source]


def test_real_global_identity_declares_compiler_environment_and_workspace(monkeypatch):
    monkeypatch.setattr(recover, 'execution_identity', lambda: {'target': 'same'})
    monkeypatch.delenv('CPATH', raising=False)
    before = v.global_identity()
    monkeypatch.setenv('CPATH', 'new-compiler-input')
    after = v.global_identity()
    assert before != after and after['build_environment']['CPATH'] == 'new-compiler-input'
    assert after['workspace'] == str(env.ROOT.resolve())


@pytest.mark.parametrize('field', ['harness', 'target', 'policy', 'toolchain', 'build_environment'])
def test_global_changes_select_every_tier(corpus, monkeypatch, field):
    complete(v.plan([1, 2]), monkeypatch)
    corpus[field] = 'changed'
    assert len(selected(v.plan([1, 2]))) == 4


def test_unknown_dependencies_are_executed_and_fail_inconclusive(corpus, monkeypatch):
    complete(v.plan([1, 2]), monkeypatch)
    (env.C_DIR / 'fn_00000001.c').unlink()
    plan = v.plan([1, 2])
    assert selected(plan) == {'00000001-c'}
    result = v.execute(plan['items']['00000001-c'], plan['identity'], False)
    assert not result['ok'] and result['inconclusive']


def test_interruption_reuses_completed_success_retains_failures(corpus, monkeypatch):
    plan = v.plan([1, 2])
    run = v.Run(plan)
    monkeypatch.setattr(v, 'execute_item', lambda *a: (True, 'pass'))
    key = '00000001-c'
    run.record(key, v.execute(plan['items'][key], plan['identity'], False))
    # No finish: only the per-item file has completion evidence.
    again = v.plan([1, 2], reuse=str(run.path))
    assert selected(again) == set(plan['items']) - {key}
    failed = complete(again, monkeypatch, fail='00000002-c')
    before = failed.path.read_bytes()
    retry = v.plan([1, 2], resume=str(failed.path), retry_failed=True)
    assert selected(retry) == {'00000002-c'}
    assert failed.path.read_bytes() == before
    assert retry['items']['00000002-c']['previous']['detail'] == 'mismatch'


def test_inputs_changed_after_planning_cannot_pass(corpus, monkeypatch):
    plan = v.plan([1])
    (env.HINTS_DIR / 'fn_00000001.json').write_text('{"seed": 123}')
    monkeypatch.setattr(v, 'execute_item', lambda *a: pytest.fail('stale plan executed'))
    result = v.execute(plan['items']['00000001-lifted'], plan['identity'], False)
    assert not result['ok'] and 'after planning' in result['detail']


def test_reused_inputs_must_remain_frozen_until_final_gate(corpus, monkeypatch):
    complete(v.plan([1, 2]), monkeypatch)
    plan = v.plan([1, 2])
    assert not selected(plan)
    run = v.Run(plan)
    (env.HINTS_DIR / 'fn_00000001.json').write_text('{"seed": 123}')
    run.finish()
    assert run.data['summary']['failed'] == 2
    assert run.data['summary']['inconclusive'] == 2
    assert set(run.data['invalidated_during_run']) == {'00000001-c', '00000001-lifted'}


def test_global_drift_at_finish_cannot_seed_reuse(corpus, monkeypatch):
    plan = v.plan([1, 2])
    run = complete(plan, monkeypatch)
    corpus['harness'] = 'changed'
    run.finish()
    assert run.data['identity_drift'] and run.data['state'] == 'incomplete'
    corpus['harness'] = 'original'
    assert len(selected(v.plan([1, 2]))) == 4


def test_audit_explicitly_reruns_all_tiers(corpus, monkeypatch):
    complete(v.plan([1, 2]), monkeypatch)
    assert len(selected(v.plan([1, 2], audit=True))) == 4


def test_composed_dependents_are_reported_separately(corpus, monkeypatch):
    complete(v.plan([1, 2]), monkeypatch)
    root = env.ANALYSIS / 'cmre/compositions'
    root.mkdir(parents=True)
    (root / 'example.json').write_text(json.dumps({'campaign': {'functions': ['fn_00000001', 'fn_00000002']}, 'status': 'pass'}))
    (env.C_DIR / 'fn_00000001.c').write_text('two')
    composition = v.plan([1, 2])['compositions'][0]
    assert composition['action'] == 'revalidate separately'
    assert composition['affected'] == ['fn_00000001']


def _identity(difftest_source, recover_source='def test():\n    return 1\n'):
    sources = {'difftest.py': difftest_source, 'recover.py': recover_source}
    return {'harness': v.digest(sources), 'target': 'pinned', 'planner': 'same',
            'components': {n: env.digest_bytes(s.encode()) for n, s in sources.items()} | {'emu.py': 'same'},
            'dispatch_sources': sources}


def test_bounded_dispatch_transition_and_material_comparison_near_miss():
    old = 'class Options:\n    def for_function(self):\n        return {}\ndef compare(a, b):\n    return a == b\n'
    new = old.replace('return {}', 'return {"tier": "c"}')
    compat.bounded(_identity(old), _identity(new))
    with pytest.raises(ValueError, match='comparison'):
        compat.bounded(_identity(old), _identity(new.replace('a == b', 'True')))
    with pytest.raises(ValueError, match='comparison'):
        compat.bounded(_identity(old), _identity(new.replace('return {"tier": "c"}', 'state.compare = True\n        return {}')))
    with pytest.raises(ValueError, match='Global'):
        compat.bounded(_identity(old), {**_identity(new), 'target': 'different'})
    with pytest.raises(ValueError, match='dispatch'):
        compat.bounded(_identity(old), {**_identity(new), 'components': _identity(new)['components'] | {'emu.py': 'changed'}})


def test_issue_23_dispatch_is_bounded(monkeypatch):
    # Synthetic authored AST fixtures represent the dispatch and metadata
    # edits. They work without historical commits, including shallow clones.
    import subprocess
    monkeypatch.setattr(subprocess, 'run', lambda *a, **kw: pytest.fail('dispatch regression must not read git history'))
    before_options = '''class Options:
    @classmethod
    def for_function(cls, va, overrides=None):
        hints = load_hints(va)
        return cls(seed=hints.get('seed', va), hints=hints)
def validate_hints(hints):
    allowed = {'seed', 'ecx'}
    if set(hints) - allowed:
        raise ValueError('unsupported hints')
def compare(a, b):
    return a == b
'''
    after_options = before_options.replace(
        'def for_function(cls, va, overrides=None):',
        "def for_function(cls, va, overrides=None, *, tier='lifted'):").replace(
        'hints = load_hints(va)',
        "hints = load_hints(va)\n        c_hints = hints.pop('c_hints', {})\n        if tier == 'c':\n            hints.update(c_hints)").replace(
        "{'seed', 'ecx'}", "{'seed', 'ecx', 'c_hints'}")
    before_recover = '''def test_c(va, overrides=None):
    options = difftest.Options.for_function(va, overrides)
    return compare(options)
def accept(va, tier):
    initial_options = difftest.Options.for_function(va).__dict__
    record = {'c': validate(initial_options)}
    return record
'''
    after_recover = before_recover.replace(
        'for_function(va, overrides)', "for_function(va, overrides, tier='c')").replace(
        'for_function(va)', 'for_function(va, tier=tier)').replace(
        '    return record', "    record['hints'] = hint_path\n    return record")
    compat.bounded(_identity(before_options, before_recover), _identity(after_options, after_recover))
    with pytest.raises(ValueError, match='comparison'):
        compat.bounded(_identity(before_options, before_recover),
                       _identity(after_options.replace('a == b', 'True'), after_recover))


def test_reviewed_compatibility_rechecks_complete_scope_and_keeps_original_identity(corpus, monkeypatch):
    from types import SimpleNamespace
    from cmre import session_evidence

    old = 'class Options:\n    def for_function(self):\n        return {}\ndef compare(a, b):\n    return a == b\n'
    corpus.clear()
    corpus.update(_identity(old))
    before = complete(v.plan([1, 2]), monkeypatch)
    original_result = copy.deepcopy(before.data['results']['00000001-lifted'])
    corpus.clear()
    corpus.update(_identity(old.replace('return {}', 'return {"tier": "c"}')))
    (env.HINTS_DIR / 'fn_00000001.json').write_text('{"c_hints": {"ecx": "ptr"}}')
    after = v.plan([1, 2], reuse=str(before.path))
    after_path = env.ANALYSIS / 'after.json'
    after_path.write_text(json.dumps(after))
    entries = [SimpleNamespace(va=va, record=v.read(ledger.record_path(va))) for va in (1, 2)]
    monkeypatch.setattr(ledger, 'cmre_entries', lambda: entries)
    inputs = {'declared': 'unchanged full suite'}
    monkeypatch.setattr(session_evidence, 'identity', lambda root: inputs)
    regression = env.ANALYSIS / 'regressions.json'
    log = env.ANALYSIS / 'pytest.log'
    log.write_text('positive and negative regressions passed')
    regression.write_text(json.dumps({'steps': [{'command': ['py', '-3.13', '-m', 'pytest'],
        'state': 'finished', 'exit_code': 0, 'evidence': 'executed', 'inputs_before': inputs,
        'inputs_after': inputs, 'log': log.name, 'log_sha256': env.digest(log)}]}))
    output = env.ANALYSIS / 'compatibility.json'
    receipt = compat.create(before.path, after_path, regression, 'reviewer', 'Input dispatch; comparison unchanged', output)
    assert set(receipt['scope']) == set(after['items']) - {'00000001-c'}
    compatible = v.plan([1, 2], reuse=str(before.path), compatibility=str(output))
    assert selected(compatible) == {'00000001-c'}
    run = v.Run(compatible)
    assert run.data['results']['00000001-lifted'] == original_result
    assert run.data['results']['00000001-lifted']['identity'] != v.digest(corpus)
    # Even an explicitly compatible harness cannot reuse changed seed/policy.
    (env.HINTS_DIR / 'fn_00000001.json').write_text('{"seed": 123}')
    assert selected(v.plan([1, 2], reuse=str(before.path), compatibility=str(output))) == {'00000001-c', '00000001-lifted'}
    log.write_text('changed regression log')
    with pytest.raises(ValueError, match='regression inputs'):
        v.plan([1, 2], reuse=str(before.path), compatibility=str(output))


def test_plan_cli_executes_no_differentials(corpus, monkeypatch, capsys):
    from types import SimpleNamespace
    from cmre import cli

    monkeypatch.setattr(ledger, 'cmre_entries', lambda: [SimpleNamespace(va=1)])
    monkeypatch.setattr(v, 'execute', lambda *a: pytest.fail('plan executed'))
    args = SimpleNamespace(n=10**9, jobs=1, plan=True, json=True, output=None, refresh=False,
                           reuse=None, resume=None, retry_failed=False, audit=False, compatibility=None)
    assert cli.cmd_revalidate(args) == 0
    data = json.loads(capsys.readouterr().out)
    assert data['items']['00000001-c']['action'] == 'execute'
    assert data['items']['00000001-c']['dependencies']['optimizations'] == ['-O0', '-O2']


@pytest.mark.parametrize('retry_failed', [False, True])
def test_cli_resume_requires_explicit_failure_retry(corpus, monkeypatch, capsys, retry_failed):
    from types import SimpleNamespace
    from cmre import cli

    first = complete(v.plan([1, 2]), monkeypatch, fail='00000002-c')
    original = first.path.read_bytes()
    monkeypatch.setattr(ledger, 'cmre_entries', lambda: [SimpleNamespace(va=va) for va in (1, 2)])
    args = SimpleNamespace(n=10**9, jobs=1, plan=True, json=True, output=None, refresh=False,
                           reuse=None, resume=str(first.path), retry_failed=retry_failed,
                           audit=False, compatibility=None)
    assert cli.cmd_revalidate(args) == 0
    plan = json.loads(capsys.readouterr().out)
    assert selected(plan) == ({'00000002-c'} if retry_failed else set())
    assert plan['items']['00000002-c']['action'] == ('execute' if retry_failed else 'retain-failure')
    if not retry_failed:
        run = v.Run(plan)
        run.finish()
        assert run.data['results']['00000002-c'] == first.data['results']['00000002-c']
        assert run.data['summary']['failed'] == 1
        assert run.data['summary']['pending'] == 0
        assert run.data['summary']['new_differential_executions'] == 0
        monkeypatch.setattr(v, 'execute', lambda *a: pytest.fail('resume retried a retained failure'))
        args.plan = False
        assert cli.cmd_revalidate(args) == 1
    assert first.path.read_bytes() == original


def test_numeric_coverage_counts_are_snapshots_in_plan_and_final_receipt(corpus, monkeypatch):
    from cmre import queue

    # Real coverage adapter, with its established ledger/inventory aggregation
    # supplied at the boundary. Counts describe the whole workspace, not -n.
    counts = {'inventory_functions': 20, 'inventory_body_bytes': 2000,
              'legacy_functions': 2, 'legacy_body_bytes': 200,
              'source_functions': 7, 'source_body_bytes': 700,
              'c_tier_functions': 5, 'c_tier_body_bytes': 500,
              'lifted_functions': 12, 'recovered_functions': 14,
              'recovered_body_bytes': 1400, 'integrated_functions': 3}
    monkeypatch.setattr(queue, 'coverage_summary', lambda: dict(counts))
    plan = v.plan([1])
    assert plan['counts'] == counts
    assert v.summary(plan)['counts'] == counts
    run = v.Run(plan)
    before = dict(counts)
    counts['integrated_functions'] = 4
    run.finish()
    assert run.data['counts_before'] == before
    assert run.data['counts'] == counts
    assert v.read(run.path)['summary']['counts']['integrated_functions'] == 4


def test_historical_receipts_report_missing_counts_as_unknown(corpus):
    historical = v.plan([1])
    historical.pop('counts')
    historical.pop('counts_scope')
    result = v.summary(historical)
    assert result['counts'] is None
    assert result['counts_scope'] == 'not recorded in this historical receipt'


def test_interrupted_resume_keeps_completed_failures_and_runs_only_pending(corpus, monkeypatch):
    plan = v.plan([1, 2])
    first = v.Run(plan)
    monkeypatch.setattr(v, 'execute_item', lambda va, tier, refresh: (va == 1, 'pass' if va == 1 else 'mismatch'))
    for key in ('00000001-c', '00000002-c'):
        first.record(key, v.execute(plan['items'][key], plan['identity'], False))
    original = first.path.read_bytes()
    resumed = v.plan([1, 2], resume=str(first.path))
    assert selected(resumed) == {'00000001-lifted', '00000002-lifted'}
    assert resumed['items']['00000001-c']['action'] == 'reuse'
    assert resumed['items']['00000002-c']['action'] == 'retain-failure'
    retry = v.plan([1, 2], resume=str(first.path), retry_failed=True)
    assert selected(retry) == {'00000001-lifted', '00000002-lifted', '00000002-c'}
    assert first.path.read_bytes() == original


def test_changed_failure_waits_for_retry_while_changed_success_is_invalidated(corpus, monkeypatch):
    first = complete(v.plan([1, 2]), monkeypatch, fail='00000002-c')
    for va in (1, 2):
        (env.HINTS_DIR / f'fn_{va:08x}.json').write_text('{"c_hints": {"ecx": "ptr"}}')
    resumed = v.plan([1, 2], resume=str(first.path))
    assert selected(resumed) == {'00000001-c'}
    assert resumed['items']['00000002-c']['action'] == 'retain-failure'
    assert selected(v.plan([1, 2], resume=str(first.path), retry_failed=True)) == {'00000001-c', '00000002-c'}
    assert selected(v.plan([1, 2], resume=str(first.path), audit=True)) == set(resumed['items'])


def test_retry_requires_a_resume_receipt(corpus):
    with pytest.raises(ValueError, match='explicit resume'):
        v.plan([1], retry_failed=True)


@pytest.mark.parametrize('value', ['00000001-c', '00000002-lifted'])
def test_tier_key_round_trip(value):
    key = v.TierKey.parse(value)
    assert str(key) == value
    assert key.function == f'fn_{key.va:08x}'


@pytest.mark.parametrize('value', ['1-c', '00000001-unknown', '00000001-c-extra'])
def test_invalid_tier_keys_are_rejected(value):
    with pytest.raises(ValueError, match='tier key'):
        v.TierKey.parse(value)
