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
    assert (state["attempts"], state["failures"], state["state"]) == (6, 5, "needs-review")


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
        calls.append(path)
        if len(calls) < 3:
            raise work.coordination.BusyError("another worker is recording")
        yield
    monkeypatch.setattr(work.coordination, "lock", contended)
    monkeypatch.setattr(work.time, "sleep", lambda _: None)
    record = work.record_test(0x401000, {"status": "pass"})
    assert record["attempts"] == 1 and len(calls) == 3
