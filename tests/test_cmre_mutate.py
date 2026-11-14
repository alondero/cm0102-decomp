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
    monkeypatch.setattr(mutate, 'run', lambda va, source, **kwargs: seen.append((va, source)) or {'survivors': []})
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


def _cli_workspace(tmp_path, monkeypatch):
    from cmre import env, work, recover
    import contextlib
    monkeypatch.setattr(env, 'ROOT', tmp_path)
    monkeypatch.setattr(env, 'C_DIR', tmp_path / 'recovery' / 'c')
    monkeypatch.setattr(env, 'ANALYSIS', tmp_path / 'analysis')
    monkeypatch.setattr(env, 'HINTS_DIR', tmp_path / 'hints')
    env.C_DIR.mkdir(parents=True)
    env.ANALYSIS.mkdir()
    env.HINTS_DIR.mkdir()
    (env.C_DIR / 'fn_00401000.c').write_text('int f(int x) { return x < 1; }', encoding='utf-8')
    monkeypatch.setattr(work, 'locked', contextlib.nullcontext)
    monkeypatch.setattr(work, 'claims', lambda: {})
    monkeypatch.setattr(recover, 'c_profile', lambda va: 'semantic')
    return env, work, recover


def test_mutate_cli_refuses_live_claim(tmp_path, monkeypatch):
    import pytest
    from cmre import cli
    env, work, recover = _cli_workspace(tmp_path, monkeypatch)
    monkeypatch.setattr(work, 'claims', lambda: {'fn_00401000': {'owner': 'worker'}})
    with pytest.raises(ValueError, match='shared claim'):
        cli.cmd_mutate(type('Args', (), {'va': 'fn_00401000'})())


def test_mutate_cli_refuses_live_task_lease(tmp_path, monkeypatch):
    import json, time, pytest
    from cmre import cli
    env, work, recover = _cli_workspace(tmp_path, monkeypatch)
    path = env.ANALYSIS / 'cmre/tasks/c-00401000-test/task.json'
    path.parent.mkdir(parents=True)
    path.write_text(json.dumps({'va': '0x00401000', 'lease': {'expires_at': time.time() + 3600}}))
    with pytest.raises(ValueError, match='task lease'):
        cli.cmd_mutate(type('Args', (), {'va': 'fn_00401000'})())


def test_mutate_cli_holds_attempt_fence_for_all_gate_calls(tmp_path, monkeypatch):
    from cmre import cli
    from contextlib import contextmanager
    env, work, recover = _cli_workspace(tmp_path, monkeypatch)
    state = {'held': False, 'calls': 0}
    @contextmanager
    def fence(va):
        assert not state['held']
        state['held'] = True
        try:
            yield
        finally:
            state['held'] = False
    def gate(*a, **kw):
        assert state['held']
        state['calls'] += 1
        return {'status': 'pass'}
    monkeypatch.setattr(work, 'attempt_fence', fence)
    monkeypatch.setattr(recover, 'test_c', gate)
    assert cli.cmd_mutate(type('Args', (), {'va': 'fn_00401000'})()) == 0
    assert state['calls'] == 3 and not state['held']


def test_mutate_reuses_guard_protected_source_check(tmp_path, monkeypatch):
    import pytest
    from cmre import cli
    env, work, recover = _cli_workspace(tmp_path, monkeypatch)
    staged = env.C_DIR / 'tasks/c-00401000-test/fn_00401000.c'
    staged.parent.mkdir(parents=True)
    staged.write_text('x')
    with pytest.raises(ValueError, match='staged task sources'):
        with cli._diagnostic_source(0x401000, staged):
            pass


def test_mutate_reuses_guard_worker_source_check(tmp_path, monkeypatch):
    import json, pytest
    from cmre import cli
    env, work, recover = _cli_workspace(tmp_path, monkeypatch)
    worker = tmp_path / 'worker'
    source = worker / 'recovery/c/fn_00401000.c'
    source.parent.mkdir(parents=True)
    source.write_text('x')
    binding = env.ANALYSIS / 'cmre/worktrees/worker.json'
    binding.parent.mkdir(parents=True)
    binding.write_text(json.dumps({'schema': 'cmre-worktree/1', 'coordinator': str(env.ROOT), 'worker': str(worker)}))
    with pytest.raises(ValueError, match='worker-bound sources'):
        with cli._diagnostic_source(0x401000, source):
            pass


def test_claim_owner_may_run_explicit_diagnostics(tmp_path, monkeypatch):
    # The farm claims a function, then its worker tests a scratch source under that claim.
    from cmre import cli
    env, work, recover = _cli_workspace(tmp_path, monkeypatch)
    monkeypatch.setattr(work, 'claims', lambda: {'fn_00401000': {'owner': 'farm-x'}})
    scratch = tmp_path / 'scratch' / 'fn_00401000.c'
    scratch.parent.mkdir()
    scratch.write_text('int f(void) { return 0; }', encoding='utf-8')
    monkeypatch.setenv('CMRE_CLAIM_OWNER', 'farm-x')
    with cli._diagnostic_source(0x401000, scratch) as resolved:
        assert resolved == scratch.resolve()


def test_other_claim_owner_still_refused(tmp_path, monkeypatch):
    import pytest
    from cmre import cli
    env, work, recover = _cli_workspace(tmp_path, monkeypatch)
    monkeypatch.setattr(work, 'claims', lambda: {'fn_00401000': {'owner': 'farm-x'}})
    scratch = tmp_path / 'scratch.c'
    scratch.write_text('int f(void) { return 0; }', encoding='utf-8')
    monkeypatch.setenv('CMRE_CLAIM_OWNER', 'someone-else')
    with pytest.raises(ValueError, match='shared claim'):
        with cli._diagnostic_source(0x401000, scratch):
            pass
