from types import SimpleNamespace

import pytest

from cmre import cli, env, inventory, ledger, queue, work


@pytest.fixture
def workspace(tmp_path, monkeypatch):
    # A supervised pytest inherits the parent's real writer lease. Keep mocked
    # lock contention in this test workspace, including the coordination path.
    monkeypatch.setenv("CMRE_COORDINATION_PATH", str(tmp_path / "command.lock"))
    for name in ("HINTS_DIR", "C_DIR", "ANALYSIS", "RECOVERY"):
        directory = tmp_path / name
        directory.mkdir()
        monkeypatch.setattr(env, name, directory)
    functions = {v: SimpleNamespace(name="test", body_bytes=100) for v in (0x401000, 0x401100, 0x401200)}
    monkeypatch.setattr(inventory, "load", lambda: SimpleNamespace(functions=functions, callees={}, modules={}))
    monkeypatch.setattr(ledger, "cmre_entries", lambda: [SimpleNamespace(va=v, size=100, record={}) for v in functions])
    monkeypatch.setattr(ledger, "legacy_entries", lambda: [])
    monkeypatch.setattr(queue, "compiler_generated", lambda va: None)
    return tmp_path


def test_queue_skips_existing_source_and_known_blocker_before_limiting(workspace):
    (env.C_DIR / "fn_00401000.c").write_text("/* WIP: missing ABI */")
    work.update(0x401100, "harness-blocker", "formatter", "formatter fixture corrected", "formatter")
    assert [i.va for i in queue.c_frontier(1)] == [0x401200]
    assert len(queue.c_frontier(10, include_held=True)) == 3
    work.retry(0x401100, "formatter has been investigated")
    assert [i.va for i in queue.c_frontier(1)] == [0x401100]


@pytest.fixture
def source_frontier(workspace, monkeypatch):
    functions = {va: SimpleNamespace(name=f"test_{va:x}", body_bytes=100)
                 for va in range(0x401000, 0x402000, 0x100)}
    inv = SimpleNamespace(functions=functions, callees={}, modules={})
    entries = [SimpleNamespace(va=0x401000, size=90, record={})]
    monkeypatch.setattr(inventory, "load", lambda: inv)
    monkeypatch.setattr(ledger, "cmre_entries", lambda: entries)
    monkeypatch.setattr(ledger, "blocked", lambda: {})
    monkeypatch.setattr(queue.image, "primary", lambda: SimpleNamespace(
        text=SimpleNamespace(va=0x401000, end=0x402000)))
    return inv, entries


def test_c_frontier_unlifted_is_opt_in_and_merges_without_duplicates(source_frontier, monkeypatch):
    inv, entries = source_frontier
    # Accepted lifts outside inventory retain their existing extent fallback.
    entries.append(SimpleNamespace(va=0x403000, size=110, record={}))
    with monkeypatch.context() as default_only:
        default_only.setattr(queue.image, "primary", lambda: pytest.fail("default queue loaded game image"))
        default_only.setattr(ledger, "blocked", lambda: pytest.fail("default queue changed blocked policy"))
        assert [i.va for i in queue.c_frontier(30)] == [0x401000, 0x403000]
    items = queue.c_frontier(30, include_unlifted=True)
    assert [i.va for i in items] == list(inv.functions) + [0x403000]
    assert items[0].name == "test_401000" and items[0].body_bytes == 100
    assert items[-1].name == "" and items[-1].body_bytes == 110


@pytest.mark.parametrize("no_static_callees", [False, True])
def test_c_frontier_unlifted_respects_exclusions_before_limit(source_frontier, monkeypatch, no_static_callees):
    inv, entries = source_frontier
    (env.C_DIR / "fn_00401000.c").write_text("old lifted C source")
    (env.C_DIR / "fn_00401100.c").write_text("old unlifted C source")
    entries.append(SimpleNamespace(va=0x401200, size=100, record={"c": {"source": "accepted"}}))
    monkeypatch.setattr(ledger, "legacy_entries", lambda: [SimpleNamespace(va=0x401300)])
    work.update(0x401400, "harness-blocker", "old ABI hold", "ABI supported")
    work.update(0x401500, "evidence-needed", "caller evidence", "caller recovered")
    work.claim([0x401600], "other")
    monkeypatch.setattr(ledger, "blocked", lambda: {0x401700: {"reason": "unsupported"}})
    monkeypatch.setattr(queue, "compiler_generated", lambda va: "import-thunk" if va == 0x401800 else None)
    inv.functions[0x402000] = SimpleNamespace(name="outside_text", body_bytes=1)
    filters = dict(include_unlifted=True, no_static_callees=no_static_callees)
    assert [i.va for i in queue.c_frontier(1, **filters)] == [0x401900]
    assert [i.va for i in queue.c_frontier(1, owner="other", **filters)] == [0x401600]
    assert [i.va for i in queue.c_frontier(1, include_generated=True, **filters)] == [0x401800]
    # Existing triage override keeps its established meaning; accepted/blocked stay excluded.
    vas = {i.va for i in queue.c_frontier(30, include_held=True, **filters)}
    assert vas == set(inv.functions) - {0x401200, 0x401300, 0x401700, 0x401800, 0x402000}


@pytest.mark.parametrize("no_static_callees", [False, True])
def test_c_frontier_unlifted_filters_and_callee_ordering(source_frontier, monkeypatch, no_static_callees):
    inv, entries = source_frontier
    inv.functions = {va: SimpleNamespace(name="test", body_bytes=size) for va, size in (
        (0x401500, 20), (0x401400, 40), (0x401300, 40), (0x401200, 80),
        (0x401100, 10), (0x401000, 100))}
    inv.modules = {va: [r"C:\code\MATCH\unit.cpp"] for va in inv.functions}
    inv.modules[0x401000] = [r"C:\code\elsewhere.cpp"]
    inv.callees = {0x401500: [0x401400], 0x401300: [0x400000]}
    entries.append(SimpleNamespace(va=0x401400, size=40, record={}))
    monkeypatch.setattr(ledger, "legacy_entries", lambda: [SimpleNamespace(va=0x400000)])
    filters = dict(include_unlifted=True, min_bytes=20, max_bytes=80, module="match",
                   no_static_callees=no_static_callees)
    expected = [0x401400, 0x401200] if no_static_callees else [0x401300, 0x401400, 0x401200, 0x401500]
    items = queue.c_frontier(2, **filters)
    assert [(i.va, i.ready, i.pending_callees) for i in items] == [(va, True, 0) for va in expected[:2]]
    assert [i.va for i in queue.c_frontier(30, **filters)] == expected


@pytest.mark.parametrize("no_static_callees", [False, True])
def test_c_frontier_unlifted_shards_and_claims(source_frontier, no_static_callees):
    inv, _ = source_frontier
    filters = dict(include_unlifted=True, no_static_callees=no_static_callees)
    shards = [{i.va for i in queue.c_frontier(30, shard=(k, 3), **filters)}
              for k in (1, 2, 3)]
    assert set().union(*shards) == set(inv.functions)
    assert sum(map(len, shards)) == len(inv.functions)
    assert [i.va for i in queue.claim_next(2, "coordinator", **filters)] == [0x401000, 0x401100]
    assert [i.va for i in queue.c_frontier(1, **filters)] == [0x401200]


def test_c_frontier_no_static_callees_is_opt_in_before_limit(workspace, monkeypatch):
    inv = inventory.load()
    monkeypatch.setattr(inventory, "load", lambda: inv)
    inv.callees = {0x401000: [0x400000], 0x401100: [0x401200]}
    monkeypatch.setattr(ledger, "legacy_entries", lambda: [SimpleNamespace(va=0x400000)])
    expected = [0x401000, 0x401200, 0x401100]
    assert [i.va for i in queue.c_frontier(3)] == expected
    assert [i.va for i in queue.c_frontier(3, no_static_callees=False)] == expected
    # A ready function can still have an accepted static callee. Filter before limiting.
    assert [i.va for i in queue.c_frontier(1, no_static_callees=True)] == [0x401200]


def test_c_frontier_no_static_callees_with_unlifted(source_frontier):
    inv, _ = source_frontier
    inv.callees = {0x401000: [0x400000], 0x401100: [0x400000], 0x401200: []}
    assert queue.c_frontier(2, no_static_callees=True) == []
    # Explicit empty and absent metadata both pass this selection-only filter.
    assert [i.va for i in queue.c_frontier(2, include_unlifted=True, no_static_callees=True)] == [
        0x401200, 0x401300]


def test_claim_next_forwards_no_static_callees(workspace, monkeypatch):
    inv = inventory.load()
    inv.callees = {0x401000: [0x401100]}
    monkeypatch.setattr(inventory, "load", lambda: inv)
    assert [i.va for i in queue.claim_next(1, "coordinator", no_static_callees=True)] == [0x401100]
    assert set(work.claims()) == {"fn_00401100"}


def test_changed_retry_requires_a_real_change_and_preserves_attempt_history(workspace):
    source = env.C_DIR / "fn_00401000.c"
    source.write_text("first")
    work.record_test(0x401000, {"status": "fail", "problems": ["case 4"]})
    with pytest.raises(ValueError, match="unchanged"):
        work.retry(0x401000, "retry", changed=True)
    source.write_text("second")
    work.retry(0x401000, "source corrected", changed=True)
    state = work.read()["functions"]["fn_00401000"]
    assert state["failures"] == state["attempts"] == 1
    assert state["state"] == "ready"


def test_failed_attempt_is_not_reset_by_passing_test_or_manual_classification(workspace):
    for _ in range(5):
        work.record_test(0x401000, {"status": "fail"})
    work.update(0x401000, "evidence-needed", "entry register", "caller contract established")
    work.record_test(0x401000, {"status": "pass"})
    state = work.read()["functions"]["fn_00401000"]
    assert (state["attempts"], state["failures"], state["state"]) == (6, 5, "evidence-needed")
    assert state['retry_when'] == 'caller contract established'
    work.retry(0x401000, 'Caller contract independently established')
    state = work.read()["functions"]["fn_00401000"]
    assert (state['attempts'], state['failures'], state['state']) == (6, 5, 'ready')


def test_retry_does_not_clear_a_newer_hold(workspace, monkeypatch):
    va = 0x401000
    work.update(va, 'evidence-needed', 'Old hold', 'Old condition')
    original = work.identity
    injected = False
    def identity_with_new_hold(value):
        nonlocal injected
        if not injected:
            injected = True
            work.update(va, 'harness-blocker', 'New hold', 'New condition')
        return {'different': 'inputs'} if injected else original(value)
    monkeypatch.setattr(work, 'identity', identity_with_new_hold)
    with pytest.raises(ValueError, match='changed|hold|retry'):
        work.retry(va, 'Old condition resolved', changed=True)
    record = work.read()['functions']['fn_00401000']
    assert record['reason'] == 'Old hold'
    assert record['additional_holds'][0]['reason'] == 'New hold'


def test_shared_claim_respects_hard_hold(workspace):
    va = 0x401000
    work.update(va, 'harness-blocker', 'Fault', 'Investigate fault')
    with pytest.raises(ValueError, match='hold|retry'):
        work.claim([va], 'worker')
    assert not work.claims()


def test_shared_claim_respects_combined_five_attempt_budget(workspace):
    va = 0x401000
    for _ in range(5):
        work.update(va, 'translation-defect', 'failed test', report={'status': 'fail'})
    with pytest.raises(ValueError, match='five|budget|exhaust'):
        work.claim([va], 'worker')
    assert not work.claims()


def test_direct_cli_c_accept_respects_hard_hold(workspace, monkeypatch, capsys):
    from cmre import recover
    va = 0x401000
    work.update(va, 'harness-blocker', 'Fault', 'Investigate fault')
    monkeypatch.setattr(recover, 'accept', lambda *a, **k: pytest.fail('held function was accepted'))
    assert cli.main(['accept', 'fn_00401000', '--c']) == 2
    assert 'hold' in capsys.readouterr().err


def test_new_evidence_does_not_erase_an_existing_hold(workspace):
    va = 0x401000
    work.update(va, 'harness-blocker', 'Original boundary', 'Resolve original boundary', 'original')
    (env.C_DIR / 'fn_00401000.c').write_text('changed')
    result = work.record_test(va, {'status': 'pass', 'source_sha256': 'old'})
    assert result['reason'] == 'Original boundary'
    assert len(result['additional_holds']) == 1
    assert result['additional_holds'][0]['reason'] == 'Source changed after the reported test'
    result = work.retry(va, 'Original boundary independently resolved')
    assert result['state'] == 'evidence-needed'
    assert result['reason'] == 'Source changed after the reported test'
    result = work.retry(va, 'Changed source independently retested')
    assert result['state'] == 'ready'
    assert (result['attempts'], result['failures']) == (1, 0)


def test_import_wip_is_idempotent_and_never_infers_test_success(workspace):
    (env.C_DIR / "fn_00401000.c").write_text("/* WIP: case 12 */\nint x;")
    (env.C_DIR / "fn_00401100.c").write_text("int y;")
    assert work.import_wip() == {"needs-review": 1, "evidence-needed": 1}
    assert work.import_wip() == {"needs-review": 0, "evidence-needed": 0}
    assert "unknown" in work.read()["functions"]["fn_00401100"]["reason"]


def test_cli_requires_a_retry_condition_for_blockers(workspace, capsys):
    assert cli.main(["work", "set", "fn_00401000", "--state", "harness-blocker", "--reason", "bad fixture"]) == 2
    assert "retry condition" in capsys.readouterr().err


def test_recording_report_does_not_mutate_it(workspace):
    report = {"status": "fail", "problems": [], "builds": {"-O0": {"detail": "compiler error"}}}
    work.record_test(0x401000, report)
    assert report["problems"] == []


def test_pass_for_changed_source_does_not_become_pending_review(workspace):
    (env.C_DIR / "fn_00401000.c").write_text("changed after run")
    result = work.record_test(0x401000, {"status": "pass", "source_sha256": "old"})
    assert result["state"] == "evidence-needed"


def test_budget_only_failure_is_unresolved_evidence_not_a_translation_defect(workspace):
    report = {"status": "fail", "builds": {"-O0": {"difftest": {
        "mismatches": [{"case": 1, "kind": "budget-asymmetry", "original": "return", "recovered": "budget"}]}}}}
    result = work.record_test(0x401000, report)
    assert result["state"] == "evidence-needed"
    assert result["blocker"] == "execution-budget"
    assert result["failures"] == 1


def test_live_claim_hides_a_function_from_other_agents_until_it_expires(workspace, monkeypatch):
    work.claim([0x401000], "codex-1")
    assert [i.va for i in queue.c_frontier(1)] == [0x401100]
    assert [i.va for i in queue.c_frontier(1, owner="claude-1")] == [0x401100]
    assert [i.va for i in queue.c_frontier(1, owner="codex-1")] == [0x401000]
    assert len(queue.c_frontier(10, include_held=True)) == 3
    later = work._now() + work.timedelta(hours=work.CLAIM_HOURS + 1)
    monkeypatch.setattr(work, "_now", lambda: later)
    assert [i.va for i in queue.c_frontier(1)] == [0x401000]
    assert work.claims() == {}


def test_claim_of_another_owners_live_function_is_refused(workspace):
    work.claim([0x401000], "codex-1")
    with pytest.raises(ValueError, match="fn_00401000 is claimed by codex-1"):
        work.claim([0x401000, 0x401100], "claude-1")
    assert set(work.claims()) == {"fn_00401000"}  # all or nothing
    work.claim([0x401000], "codex-1")  # renewing your own claim is allowed


def test_claim_next_gives_independent_agents_disjoint_functions(workspace):
    assert [i.va for i in queue.claim_next(2, "a")] == [0x401000, 0x401100]
    assert [i.va for i in queue.claim_next(2, "b")] == [0x401200]
    assert [i.va for i in queue.claim_next(2, "a")] == [0x401000, 0x401100]  # own claims come back
    assert {k: v["owner"] for k, v in work.claims().items()} == {
        "fn_00401000": "a", "fn_00401100": "a", "fn_00401200": "b"}


def test_release_returns_an_owners_functions_to_the_queue(workspace):
    queue.claim_next(3, "a")
    work.release("a", [0x401100])
    assert [i.va for i in queue.c_frontier(3, owner="b")] == [0x401100]
    work.release("a")
    assert work.claims() == {}


def test_shards_partition_the_queue_without_coordination(workspace):
    shards = [{i.va for i in queue.c_frontier(10, shard=(k, 3))} for k in (1, 2, 3)]
    assert set().union(*shards) == {0x401000, 0x401100, 0x401200}
    assert sum(map(len, shards)) == 3
    with pytest.raises(ValueError, match="shard"):
        queue.c_frontier(10, shard=(0, 3))


def test_cli_next_claims_and_work_lists_claims(workspace, capsys):
    assert cli.main(["next", "--c", "-n", "1", "--claim", "codex-1"]) == 0
    assert capsys.readouterr().out.startswith("fn_00401000")
    assert cli.main(["next", "--c", "-n", "1", "--shard", "1/1", "--claim", "claude-1"]) == 0
    assert capsys.readouterr().out.startswith("fn_00401100")
    assert cli.main(["work", "claims"]) == 0
    out = capsys.readouterr().out
    assert "fn_00401000 codex-1" in out and "fn_00401100 claude-1" in out
    assert cli.main(["work", "release", "--owner", "codex-1"]) == 0
    assert set(work.claims()) == {"fn_00401100"}


def test_concurrent_completion_retries_lock_without_duplicating_attempts(workspace, monkeypatch):
    from contextlib import contextmanager
    calls = []
    @contextmanager
    def contended(path):
        if path.name == 'work.lock':
            calls.append(path)
            if len(calls) < 3:
                raise work.coordination.BusyError("another worker is recording")
        yield
    monkeypatch.setattr(work.coordination, "lock", contended)
    monkeypatch.setattr(work.time, "sleep", lambda _: None)
    record = work.record_test(0x401000, {"status": "pass"})
    assert record["attempts"] == 1 and len(calls) == 3
