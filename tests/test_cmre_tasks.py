"""Task ownership, frozen input evidence and publication recovery contracts."""
import json
from types import SimpleNamespace

import pytest

from cmre import env, tasks, work

VA = 0x401000


@pytest.fixture
def workspace(tmp_path, monkeypatch):
    monkeypatch.setenv("CMRE_COORDINATION_PATH", str(tmp_path / "command.lock"))
    monkeypatch.delenv("CMRE_WRITER_TOKEN", raising=False)
    for name, relative in (("ROOT", "."), ("ANALYSIS", "analysis"), ("RECOVERY", "recovery"),
                           ("HINTS_DIR", "recovery/hints"), ("C_DIR", "recovery/c"),
                           ("LIFTED_DIR", "recovery/lifted"), ("LEDGER_DIR", "recovery/ledger")):
        path = tmp_path / relative
        path.mkdir(parents=True, exist_ok=True)
        monkeypatch.setattr(env, name, path)
    (env.LIFTED_DIR / "fn_00401000.S").write_text("ret\n")
    (env.ROOT / "recovery/include").mkdir(parents=True)
    (env.ROOT / "recovery/include/cmre.h").write_text("typedef unsigned int u32;\n")
    (env.LEDGER_DIR / "fn_00401000.json").write_text(json.dumps({"lifted": {}}))
    fingerprint = {"target": "pinned", "harness": "first"}
    monkeypatch.setattr(tasks, "_inputs", lambda va: {**fingerprint, "abi_contract": tasks._optional_hash(tasks._contract_path(va))})
    function = SimpleNamespace(name="FUN_00401000", body_bytes=1, signature="undefined FUN_00401000(void)")
    monkeypatch.setattr(tasks.inventory, "load", lambda: SimpleNamespace(functions={VA: function}))
    instruction = SimpleNamespace(va=VA, raw=b"\xc3", mnemonic="ret", op_str="", kind="ret")
    lifted = SimpleNamespace(problems=[], region_end=VA + 1, insns={VA: instruction}, callees=[], imports=[])
    monkeypatch.setattr(tasks.lift, "discover", lambda va: lifted)
    monkeypatch.setattr(tasks.recover, "test_c", lambda va, *, source: {"status": "pass", "source": str(source)})
    return fingerprint


def leased():
    packet = tasks.prepare(VA)
    claimed = tasks.claim(packet["task_id"], "worker")
    candidate = env.ROOT / claimed["candidate_path"]
    candidate.write_text("#include \"cmre.h\"\nvoid cm_fn_00401000(void) {}\n")
    return claimed, candidate


def submitted():
    manifest, candidate = leased()
    return tasks.submit(manifest["task_id"], "worker", manifest["lease"]["token"]), candidate


def test_packet_is_deduplicated_complete_and_does_not_disclose_lease(workspace):
    manifest, candidate = leased()
    packet = tasks.prepare(VA)
    assert packet["task_id"] == manifest["task_id"]
    assert "token" not in packet["lease"]
    assert packet["evidence"]["instructions"][0]["bytes"] == "c3"
    assert packet["evidence"]["assembly"] == "ret\n"
    assert packet["allowed_worker_files"] == [env.rel(candidate)]
    assert packet["abi"]["unknown"]
    assert not (env.C_DIR / "fn_00401000.c").exists()


def test_claim_rejects_live_lease_and_wrong_token(workspace):
    manifest, _ = leased()
    with pytest.raises(ValueError, match="live lease"):
        tasks.claim(manifest["task_id"], "other")
    with pytest.raises(ValueError, match="owner/token"):
        tasks.submit(manifest["task_id"], "worker", "incorrect")
    assert "token" not in tasks.status(manifest["task_id"])["lease"]


@pytest.mark.parametrize('state', ['harness-blocker', 'evidence-needed'])
def test_direct_task_claim_respects_existing_hold(workspace, state):
    packet = tasks.prepare(VA)
    work.update(VA, state, 'Retained discrepancy', 'Resolve named boundary')
    with pytest.raises(ValueError, match='hold|retry'):
        tasks.claim(packet['task_id'], 'worker')
    assert tasks.status(packet['task_id'])['status'] == 'prepared'
    work.retry(VA, 'Named boundary independently resolved')
    assert tasks.claim(packet['task_id'], 'worker')['status'] == 'claimed'


def test_task_claim_cannot_collide_with_shared_claim(workspace):
    packet = tasks.prepare(VA)
    work.claim([VA], 'shared-worker')
    with pytest.raises(ValueError, match='claim|owner'):
        tasks.claim(packet['task_id'], 'direct-worker')
    assert tasks.status(packet['task_id'])['status'] == 'prepared'
    work.release('shared-worker')
    assert tasks.claim(packet['task_id'], 'direct-worker')['status'] == 'claimed'


def test_shared_claim_cannot_collide_with_live_task_lease(workspace, monkeypatch):
    packet = tasks.prepare(VA)
    lease = tasks.claim(packet['task_id'], 'direct-worker')
    with pytest.raises(ValueError, match='claim|lease'):
        work.claim([VA], 'shared-worker')
    assert not work.claims()
    monkeypatch.setattr(work.time, 'time', lambda: lease['lease']['expires_at'] + 1)
    assert work.claim([VA], 'shared-worker')['fn_00401000']['owner'] == 'shared-worker'


def test_expired_lease_can_be_reclaimed_but_old_token_cannot_submit(workspace, monkeypatch):
    manifest, _ = leased()
    monkeypatch.setattr(tasks.time, "time", lambda: manifest["lease"]["expires_at"] + 1)
    replacement = tasks.claim(manifest["task_id"], "worker")
    assert replacement["lease"]["token"] != manifest["lease"]["token"]
    with pytest.raises(ValueError, match="owner/token"):
        tasks.submit(manifest["task_id"], "worker", manifest["lease"]["token"])


def test_stale_inputs_reject_submission_and_make_new_packet(workspace):
    manifest, _ = leased()
    workspace["harness"] = "second"
    with pytest.raises(ValueError, match="Stale"):
        tasks.submit(manifest["task_id"], "worker", manifest["lease"]["token"])
    assert tasks.prepare(VA)["task_id"] != manifest["task_id"]


def test_submission_runs_staged_gate_without_accepting_and_records_unknown_metrics(workspace, monkeypatch):
    monkeypatch.setattr(tasks.recover, "accept", lambda *a: pytest.fail("worker must not accept"))
    result, candidate = submitted()
    assert result["status"] == "submitted"
    attempt = result["attempts"][-1]
    assert attempt["metrics"] == {key: None for key in tasks.METRICS}
    assert attempt["candidate_sha256"] == env.digest(candidate)
    assert (env.ROOT / attempt["report_path"]).exists()
    assert not (env.C_DIR / "fn_00401000.c").exists()


def test_revise_preserves_submission_and_requires_a_new_lease(workspace):
    manifest, candidate = submitted()
    old_attempt = dict(manifest['attempts'][0])
    report = env.ROOT / old_attempt['report_path']
    old_report = report.read_bytes()
    candidate.write_text('unsigned long long cm_fn_00401000(unsigned int arg) {}\n')
    revised = tasks.revise(manifest['task_id'], 'Match the reviewed callee declaration for composition')
    assert revised['status'] == 'prepared' and revised['lease'] is None
    assert revised['attempts'] == [old_attempt] and report.read_bytes() == old_report
    assert revised['journal'][-1]['event'] == 'revision-requested'
    assert revised['journal'][-1]['previous_candidate_sha256'] == old_attempt['candidate_sha256']
    assert revised['journal'][-1]['candidate_sha256'] == env.digest(candidate)
    assert not (env.C_DIR / 'fn_00401000.c').exists()
    with pytest.raises(ValueError, match='owner/token'):
        tasks.submit(manifest['task_id'], 'worker', 'old-token')
    claimed = tasks.claim(manifest['task_id'], 'worker')
    result = tasks.submit(manifest['task_id'], 'worker', claimed['lease']['token'])
    assert result['status'] == 'submitted' and len(result['attempts']) == 2
    assert result['attempts'][0] == old_attempt


def test_revise_rejects_invalid_state_stale_inputs_and_missing_reason(workspace):
    manifest, _ = leased()
    with pytest.raises(ValueError, match='submitted'):
        tasks.revise(manifest['task_id'], 'Not submitted yet')
    manifest = tasks.submit(manifest['task_id'], 'worker', manifest['lease']['token'])
    with pytest.raises(ValueError, match='reason'):
        tasks.revise(manifest['task_id'], ' ')
    workspace['harness'] = 'changed'
    with pytest.raises(ValueError, match='Stale'):
        tasks.revise(manifest['task_id'], 'Shared inputs changed')


def test_revise_keeps_attempt_limit_and_report_integrity(workspace):
    manifest, _ = submitted()
    with tasks.coordination.lock(tasks.task_dir(manifest['task_id']) / '.lock'):
        manifest['attempts'] *= tasks.MAX_ATTEMPTS
        tasks.save(manifest)
    with pytest.raises(ValueError, match='exhausted'):
        tasks.revise(manifest['task_id'], 'Would exceed the attempt limit')
    with tasks.coordination.lock(tasks.task_dir(manifest['task_id']) / '.lock'):
        manifest['attempts'] = manifest['attempts'][:1]
        tasks.save(manifest)
    (env.ROOT / manifest['attempts'][0]['report_path']).write_text('{}')
    with pytest.raises(ValueError, match='report changed'):
        tasks.revise(manifest['task_id'], 'Report was tampered with')


def test_failure_budget_and_evidence_survive(workspace, monkeypatch):
    manifest, _ = leased()
    monkeypatch.setattr(tasks.recover, "test_c", lambda *a, **k: {"status": "fail", "mismatch": "eax"})
    for _ in range(tasks.MAX_ATTEMPTS):
        result = tasks.submit(manifest["task_id"], "worker", manifest["lease"]["token"])
    assert result["status"] == "exhausted"
    assert len(result["attempts"]) == tasks.MAX_ATTEMPTS
    assert all((env.ROOT / a["report_path"]).exists() for a in result["attempts"])
    with pytest.raises(ValueError, match="exhausted"):
        tasks.claim(manifest["task_id"], "other")


def test_validation_exception_is_journaled(workspace, monkeypatch):
    manifest, _ = leased()
    def fail(*args, **kwargs):
        raise RuntimeError("compiler unavailable")
    monkeypatch.setattr(tasks.recover, "test_c", fail)
    with pytest.raises(RuntimeError):
        tasks.submit(manifest["task_id"], "worker", manifest["lease"]["token"])
    result = tasks.status(manifest["task_id"])
    assert result["attempts"][-1]["status"] == "failed"
    assert "compiler unavailable" in result["attempts"][-1]["error"]


@pytest.mark.parametrize("task_id", ["../outside", "c-00401000-../../bad", "C-00401000-0123456789abcdef"])
def test_task_id_rejects_path_escape(task_id):
    with pytest.raises(ValueError):
        tasks.task_dir(task_id)


@pytest.mark.parametrize("metrics", [{"policy": 1}, {"input_tokens": -1}, {"worker_seconds": float("nan")}, {"output_tokens": 1.5}])
def test_invalid_metrics_do_not_consume_attempt(workspace, metrics):
    manifest, _ = leased()
    with pytest.raises(ValueError):
        tasks.submit(manifest["task_id"], "worker", manifest["lease"]["token"], metrics=metrics)
    assert not tasks.status(manifest["task_id"])["attempts"]


def test_publication_rejects_changed_candidate(workspace):
    manifest, candidate = submitted()
    candidate.write_text("changed")
    with pytest.raises(ValueError, match="candidate changed"):
        tasks.publish(manifest["task_id"])
    assert not (env.C_DIR / "fn_00401000.c").exists()


@pytest.mark.parametrize("previous", [None, "existing work"])
def test_rejected_publication_restores_previous_source(workspace, monkeypatch, previous):
    manifest, _ = submitted()
    destination = env.C_DIR / "fn_00401000.c"
    if previous is not None:
        destination.write_text(previous)
    monkeypatch.setattr(tasks.recover, "accept", lambda *a: (False, {"status": "nondeterministic"}))
    result = tasks.publish(manifest["task_id"])
    assert result["status"] == "publication-failed"
    assert destination.read_text() == previous if previous is not None else not destination.exists()


def test_accepted_publication_is_durable_and_idempotent(workspace, monkeypatch):
    manifest, candidate = submitted()
    def accept(va, tier):
        assert tier == "c"
        path = tasks.ledger.record_path(va)
        path.write_text(json.dumps({"c": {"source_sha256": env.digest(env.C_DIR / "fn_00401000.c")}}))
        return True, {"status": "accepted"}
    monkeypatch.setattr(tasks.recover, "accept", accept)
    result = tasks.publish(manifest["task_id"])
    assert result["status"] == "published"
    assert env.digest(env.C_DIR / "fn_00401000.c") == env.digest(candidate)
    assert tasks.publish(manifest["task_id"])["status"] == "published"


def test_interrupted_publication_reconciles_accepted_ledger(workspace):
    manifest, candidate = submitted()
    destination = env.C_DIR / "fn_00401000.c"
    destination.write_bytes(candidate.read_bytes())
    tasks.ledger.record_path(VA).write_text(json.dumps({"c": {"source_sha256": env.digest(candidate)}}))
    manifest.update(status="publishing", publication={"previous_sha256": None, "candidate_sha256": env.digest(candidate)})
    tasks.save(manifest)
    assert tasks.publish(manifest["task_id"])["status"] == "published"
    assert destination.exists()


def test_interrupted_unaccepted_publication_restores_source(workspace):
    manifest, candidate = submitted()
    destination = env.C_DIR / "fn_00401000.c"
    destination.write_bytes(candidate.read_bytes())
    manifest.update(status="publishing", publication={"previous_sha256": None, "candidate_sha256": env.digest(candidate)})
    tasks.save(manifest)
    assert tasks.publish(manifest["task_id"])["status"] == "publication-failed"
    assert not destination.exists()


def test_evidence_needed_escalates_without_running_validation(workspace, monkeypatch):
    manifest, _ = leased()
    monkeypatch.setattr(tasks.recover, "test_c", lambda *a, **k: pytest.fail("no candidate gate for escalation"))
    result = tasks.submit(manifest["task_id"], "worker", manifest["lease"]["token"],
                          outcome="evidence-needed", summary="Cannot establish caller cleanup ABI")
    assert result["status"] == "escalated"
    assert result["lease"] is None


def test_compact_summary_omits_assembly_and_bearer_token(workspace):
    manifest, _ = leased()
    result = tasks.summary(manifest)
    assert "evidence" not in result
    assert "token" not in result["lease"]
    assert tasks.summary(manifest, include_token=True)["lease"]["token"] == manifest["lease"]["token"]


def test_staged_submission_does_not_take_global_writer(workspace, monkeypatch):
    manifest, _ = leased()
    monkeypatch.setattr(tasks.coordination, "writer", lambda: pytest.fail("staged validation must permit independent tasks"))
    assert tasks.submit(manifest["task_id"], "worker", manifest["lease"]["token"])["status"] == "submitted"


def test_metrics_keep_missing_cost_unknown(workspace):
    manifest, _ = leased()
    tasks.submit(manifest["task_id"], "worker", manifest["lease"]["token"], outcome="failed",
                 metrics={"model": "small", "input_tokens": 100, "output_tokens": 20})
    result = tasks.metrics_summary(manifest["task_id"])
    assert result["costs"]["all_tokens"]["known_subtotal"] == 120
    assert result["costs"]["all_tokens"]["total"] is None
    assert result["costs"]["coordinator_tokens"]["known_subtotal"] is None
    assert result["costs"]["coordinator_tokens"]["missing_attempts"] == 1
    assert result["by_model"][0]["model"] == "small"
    assert result["useful_published"] == 0


def test_metrics_attribute_failed_attempt_cost_and_published_credit(workspace):
    manifest, _ = leased()
    token = manifest["lease"]["token"]
    costs = {"input_tokens": 100, "output_tokens": 20, "coordinator_tokens": 10,
             "reviewer_tokens": 5, "worker_seconds": 4}
    tasks.submit(manifest["task_id"], "worker", token, outcome="failed", metrics={**costs, "model": "small"})
    result = tasks.submit(manifest["task_id"], "worker", token, metrics={**costs, "model": "large"})
    result["status"] = "published"  # Isolate aggregation from publication mechanics.
    tasks.save(result)
    metrics = tasks.metrics_summary()
    assert metrics["costs"]["all_tokens"]["total"] == 270
    assert metrics["retries"] == 1
    assert metrics["useful_published"] == 1
    assert metrics["first_pass_published"] == 0
    by_model = {row["model"]: row for row in metrics["by_model"]}
    assert by_model["small"]["useful_published"] == 0
    assert by_model["small"]["costs"]["all_tokens"]["total"] == 135
    assert by_model["large"]["useful_published"] == 1


def test_metrics_empty_and_escalated_tasks_do_not_claim_free_success(workspace):
    assert tasks.metrics_summary()["costs"]["all_tokens"]["total"] is None
    manifest, _ = leased()
    tasks.submit(manifest["task_id"], "worker", manifest["lease"]["token"],
                 outcome="evidence-needed", summary="Need original caller evidence")
    result = tasks.metrics_summary()
    assert result["escalations"] == 1
    assert result["by_model"][0]["model"] is None
    assert result["costs"]["all_tokens"]["total"] is None


@pytest.mark.parametrize("metrics", [{"model": ""}, {"model": 4}, {"coordinator_tokens": -1}, {"reviewer_tokens": 1.5}])
def test_coordinator_and_model_metrics_validate(workspace, metrics):
    manifest, _ = leased()
    with pytest.raises(ValueError):
        tasks.submit(manifest["task_id"], "worker", manifest["lease"]["token"], metrics=metrics)


def write_contract(monkeypatch, **changes):
    contract = {"schema": "cmre-abi-contract/1", "target_sha256": env.TARGET_SHA256,
                "address_convention": env.ADDRESS_CONVENTION, "va": f"{VA:#010x}",
                "declaration": "void CM_CDECL cm_fn_00401000(void);", "calling_convention": "cdecl",
                "evidence": [{"va": f"{VA:#010x}", "bytes": "c3", "supports": "Entry immediately returns without callee stack cleanup"}],
                "unknowns": ["Meaning of caller context remains unknown"], **changes}
    path = tasks._contract_path(VA)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(contract))
    monkeypatch.setattr(tasks.image, "primary", lambda: SimpleNamespace(read=lambda va, size: b"\xc3" if va == VA and size == 1 else b"\x00" * size))
    return path


def test_analyst_abi_contract_embeds_byte_checked_claims_and_preserves_unknowns(workspace, monkeypatch):
    write_contract(monkeypatch)
    packet = tasks.prepare(VA)
    contract = packet["abi"]["analyst_contract"]
    assert contract["claims"]["declaration"] == "void CM_CDECL cm_fn_00401000(void);"
    assert "not machine-proven" in contract["verification"]
    assert packet["abi"]["unknown"] == ["Meaning of caller context remains unknown"]
    assert packet["inputs"]["abi_contract"] == env.digest(tasks._contract_path(VA))


def test_abi_contract_accepts_variadic_function_declaration(workspace, monkeypatch):
    declaration = 'int CM_CDECL cm_fn_00401000(const char *format, ...);'
    write_contract(monkeypatch, declaration=declaration)
    assert tasks.prepare(VA)['abi']['analyst_contract']['claims']['declaration'] == declaration


@pytest.mark.parametrize('declaration', [
    'int cm_fn_00401000(...);',
    'int cm_fn_00401000(void, ...);',
    'int cm_fn_00401000(int x, ..);',
    'int cm_fn_00401000(int x, ....);',
    'int cm_fn_00401000(int x, ..., int y);',
    'int cm_fn_00401000(int x, ..., ...);',
    'int cm_fn_00401000(int x,, ...);',
    'int cm_fn_00401000(void name, ...);',
    'int cm_fn_00401000(int x, void, ...);',
    'int CM_STDCALL cm_fn_00401000(int x, ...);',
    'int __fastcall cm_fn_00401000(int x, ...);',
    'int cm_fn_00401000(int x, ...){ return 0; }',
    '#define X 1\nint cm_fn_00401000(int x, ...);',
])
def test_abi_contract_rejects_malformed_variadic_declarations(workspace, monkeypatch, declaration):
    write_contract(monkeypatch, declaration=declaration)
    with pytest.raises(ValueError, match='declaration|variadic'):
        tasks.prepare(VA)


@pytest.mark.parametrize('convention', ['stdcall', 'fastcall', 'thiscall'])
def test_variadic_abi_requires_cdecl(workspace, monkeypatch, convention):
    write_contract(monkeypatch, declaration='int cm_fn_00401000(int value, ...);',
                   calling_convention=convention)
    with pytest.raises(ValueError, match='variadic|declaration'):
        tasks.prepare(VA)


def test_new_contract_invalidates_existing_task_and_changes_packet_id(workspace, monkeypatch):
    manifest, _ = leased()
    write_contract(monkeypatch)
    with pytest.raises(ValueError, match="Stale"):
        tasks.submit(manifest["task_id"], "worker", manifest["lease"]["token"])
    assert tasks.prepare(VA)["task_id"] != manifest["task_id"]


def test_changed_contract_invalidates_claimed_packet(workspace, monkeypatch):
    path = write_contract(monkeypatch)
    manifest, _ = leased()
    contract = json.loads(path.read_text())
    contract["unknowns"].append("Additional caller register use unresolved")
    path.write_text(json.dumps(contract))
    with pytest.raises(ValueError, match="Stale"):
        tasks.validate_current(manifest)


def test_wrong_original_evidence_bytes_reject_abi_contract(workspace, monkeypatch):
    write_contract(monkeypatch, evidence=[{"va": f"{VA:#010x}", "bytes": "90", "supports": "Claim"}])
    with pytest.raises(ValueError, match="bytes differ"):
        tasks.prepare(VA)


@pytest.mark.parametrize("changes", [
    {"target_sha256": "wrong"}, {"va": "0x00401001"}, {"address_convention": "RVA"},
    {"declaration": "#include <file>\nvoid cm_fn_00401000(void);"},
    {"declaration": "void cm_fn_00401000(void) {}"},
    {"declaration": "void cm_fn_00401000(void); void other(void);"},
    {"declaration": "void other(void);"}, {"calling_convention": "unknown"},
    {"unknowns": None}, {"evidence": []},
])
def test_invalid_contract_schema_or_declaration_rejected(workspace, monkeypatch, changes):
    write_contract(monkeypatch, **changes)
    with pytest.raises(ValueError):
        tasks.prepare(VA)


def test_contract_requires_explicit_unknowns_even_if_empty(workspace, monkeypatch):
    path = write_contract(monkeypatch)
    contract = json.loads(path.read_text())
    del contract["unknowns"]
    path.write_text(json.dumps(contract))
    with pytest.raises(ValueError, match="schema fields"):
        tasks.prepare(VA)


def test_usage_correction_preserves_results_and_identity_and_journals_change(workspace):
    manifest, _ = leased()
    result = tasks.submit(manifest["task_id"], "worker", manifest["lease"]["token"],
                          metrics={"model": "small", "input_tokens": 100, "output_tokens": 20})
    before = json.loads(json.dumps(result))
    updated = tasks.record_usage(manifest["task_id"], 1, {"reviewer_tokens": 30, "coordinator_tokens": 10})
    assert updated["attempts"][0]["metrics"]["input_tokens"] == 100
    assert updated["attempts"][0]["metrics"]["model"] == "small"
    assert {k: v for k, v in updated.items() if k not in ("attempts", "journal")} == {
        k: v for k, v in before.items() if k not in ("attempts", "journal")}
    assert {k: v for k, v in updated["attempts"][0].items() if k != "metrics"} == {
        k: v for k, v in before["attempts"][0].items() if k != "metrics"}
    event = updated["journal"][-1]
    assert event["event"] == "usage-corrected"
    assert event["before"]["reviewer_tokens"] is None
    assert event["after"]["reviewer_tokens"] == 30
    assert tasks.metrics_summary(manifest["task_id"])["costs"]["all_tokens"]["total"] == 160


def test_usage_can_restore_unknown_cost_without_erasing_other_measurements(workspace):
    result, _ = submitted()
    tasks.record_usage(result["task_id"], 1, {"reviewer_tokens": 20, "coordinator_tokens": 10})
    updated = tasks.record_usage(result["task_id"], 1, {"reviewer_tokens": None})
    assert updated["attempts"][0]["metrics"]["reviewer_tokens"] is None
    assert updated["attempts"][0]["metrics"]["coordinator_tokens"] == 10


@pytest.mark.parametrize("attempt,metrics", [
    (0, {"reviewer_tokens": 2}), (True, {"reviewer_tokens": 2}), (2, {"reviewer_tokens": 2}),
    (1, {}), (1, {"source_sha256": "new"}), (1, {"reviewer_tokens": -1}),
])
def test_invalid_usage_correction_does_not_change_receipt(workspace, attempt, metrics):
    result, _ = submitted()
    path = tasks.task_dir(result["task_id"]) / "task.json"
    before = path.read_bytes()
    with pytest.raises(ValueError):
        tasks.record_usage(result["task_id"], attempt, metrics)
    assert path.read_bytes() == before


def test_usage_cli_routes_correction_and_returns_compact_success(workspace, capsys):
    from cmre import cli

    result, _ = submitted()
    assert cli.main(["task", "usage", result["task_id"], "--attempt", "1", "--metrics",
                     '{"reviewer_tokens": 7}']) == 0
    response = json.loads(capsys.readouterr().out)
    assert response["latest_attempt"]["metrics"]["reviewer_tokens"] == 7
    assert "evidence" not in response
