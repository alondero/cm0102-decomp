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
