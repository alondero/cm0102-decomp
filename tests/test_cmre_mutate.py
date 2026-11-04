from cmre import mutate


def test_mutants_skip_comments_literals_and_directives():
    source = '#define N 42\nint f(int x) { /* x < 9 */ return x <= 10 && x != 0; }\n'
    rows = list(mutate.mutants(source))
    assert [(r['old'], r['new']) for r in rows] == [('<=', '<'), ('10', '11'), ('!=', '=='), ('0', '1')]
    assert all('#define N 42' in r['source'] and '/* x < 9 */' in r['source'] for r in rows)


def test_mutation_run_requires_passing_baseline_and_reports_survivors(tmp_path):
    source = tmp_path / 'source.c'
    source.write_text('int f(int x) { return x < 3; }', encoding='utf-8')
    calls = []
    def gate(va, **kwargs):
        calls.append(kwargs)
        return {'status': 'pass'}
    result = mutate.run(1, source, gate)
    assert len(result['survivors']) == 2
    assert all(call['profile'] == 'semantic' and call['fresh'] for call in calls)
    assert source.read_text(encoding='utf-8') == 'int f(int x) { return x < 3; }'


def test_mutate_cli_dispatches_semantic_diagnostic(monkeypatch, capsys):
    from cmre import cli, env
    seen = []
    monkeypatch.setattr(mutate, 'run', lambda va, source: seen.append((va, source)) or {'survivors': []})
    assert cli.main(['mutate', 'fn_00401000']) == 0
    assert seen == [(0x401000, env.C_DIR / 'fn_00401000.c')]
    assert 'survivors' in capsys.readouterr().out


def test_mutation_rejects_failing_baseline(tmp_path):
    import pytest
    source = tmp_path / 'source.c'
    source.write_text('int f(void) { return 1; }', encoding='utf-8')
    with pytest.raises(ValueError, match='Original source'):
        mutate.run(1, source, lambda *a, **k: {'status': 'fail'})


def test_mutant_failure_classification_and_retained_provenance(tmp_path, monkeypatch):
    from cmre import env
    monkeypatch.setattr(env, 'ANALYSIS', tmp_path / 'analysis')
    source = tmp_path / 'source.c'
    source.write_text('int f(int x) { return x < 3; }', encoding='utf-8')
    reports = iter([
        {'status': 'pass', 'key': 'baseline'},
        {'status': 'fail', 'key': 'build', 'builds': {'dependencies': {'status': 'build-failed'}}},
        {'status': 'fail', 'key': 'coverage', 'builds': {'-O0': {'status': 'fail', 'difftest': {'mismatches': [], 'coverage': .2}}}},
    ])
    result = mutate.run(1, source, lambda *a, **k: next(reports))
    assert result['invalid'][0]['error_class'] == 'build-failed'
    assert result['unresolved'][0]['error_class'] == 'gate-failed-without-mismatch'
    assert not result['killed']
    for row in result['invalid'] + result['unresolved']:
        assert row['source_sha256'] and row['baseline_sha256'] and row['gate_key']
        assert __import__('pathlib').Path(row['source_path']).is_file()
        assert __import__('pathlib').Path(row['report_path']).is_file()


def test_mutant_only_behavioral_mismatch_counts_as_killed():
    assert mutate.classify({'status': 'fail', 'builds': {'-O0': {'status': 'fail', 'difftest': {'mismatches': [{'kind': 'value'}]}}}}) == ('killed', 'differential-mismatch')
    assert mutate.classify({'status': 'fail', 'builds': {'-O0': {'status': 'fail', 'difftest': {'mismatches': [{'kind': 'budget-asymmetry'}]}}}})[0] == 'unresolved'


def test_mutation_refuses_non_semantic_accepted_profile(tmp_path, monkeypatch):
    import pytest
    from cmre import recover
    monkeypatch.setattr(recover, 'c_profile', lambda va: 'abi')
    with pytest.raises(ValueError, match='semantic profile'):
        mutate.run(1, tmp_path / 'source.c')


def test_mutation_preserves_failure_exception(tmp_path, monkeypatch):
    from cmre import env
    monkeypatch.setattr(env, 'ANALYSIS', tmp_path / 'analysis')
    source = tmp_path / 'source.c'
    source.write_text('int f(int x) { return x < 1; }', encoding='utf-8')
    calls = iter([{'status': 'pass', 'key': 'baseline'}, RuntimeError('gate broke'), {'status': 'pass', 'key': 'survivor'}])
    def mixed_gate(*args, **kwargs):
        outcome = next(calls)
        if isinstance(outcome, Exception):
            raise outcome
        return outcome
    result = mutate.run(1, source, mixed_gate)
    assert result['unresolved'][0]['error_class'] == 'RuntimeError'
