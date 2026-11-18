"""Actual native function entry -> OS imports -> function endpoint (no game)."""
import json
from pathlib import Path
import uuid

import pytest

from native_import_results import ROOT, Unavailable, build_native_results
from native_entry_imports import observe_entry_imports


@pytest.fixture(scope="module")
def child():
    try:
        return build_native_results(ROOT / "local/native-entry-imports" / uuid.uuid4().hex)
    except Unavailable as exc:
        pytest.skip(str(exc))


def test_actual_function_scope_encloses_native_import_outputs(child):
    result = observe_entry_imports(child)
    assert result["child_returncode"] == 0
    assert result["cleanup"]["detached"]
    assert result["cleanup"]["all_debugger_bytes_restored"]
    scopes = result["function_scopes"]
    assert scopes["cleanup_complete"]
    assert not scopes["replayable"] and not scopes["effects_complete"]
    assert not scopes["game_recorder_wired"]
    scope, = scopes["scopes"]
    assert scope["state"] == "paired-endpoints"
    records = result["protocol"]["invocations"]
    root, heap, read = records
    assert scope["imports"] == [heap["invocation"], read["invocation"]]
    assert root["import_name"].startswith("function-entry!")
    assert heap["import_name"] == "KERNEL32.dll!HeapAlloc"
    assert read["import_name"] == "KERNEL32.dll!ReadFile"
    assert heap["parent"] == read["parent"] == root["invocation"] == scope["invocation"]
    assert {r["scope_id"] for r in records} == {scope["scope_id"]}
    assert root["entry_sequence"] < heap["entry_sequence"] < read["entry_sequence"]
    assert root["return_sequence"] > read["return_sequence"]
    child_result = json.loads(result["stdout"].strip())
    output, = heap["return"]["memory"]
    assert output["data_hex"] == child_result["heap_hex"]
    assert heap["return"]["registers"]["eax"] == child_result["heap_pointer"]
    assert all(r["replayable"] is False for r in records)
    assert "import_transcript" not in result
    assert result["entry_snapshot"]["registers"] == root["entry"]["registers"]


@pytest.mark.parametrize("point", ["entry-hit", "step-prepared", "step-stop"])
def test_interrupted_function_scope_retained_without_false_completion(child, point):
    result = observe_entry_imports(child, detach_at=point)
    assert result["child_returncode"] == 0
    assert result["cleanup"]["detached"]
    assert result["cleanup"]["all_debugger_bytes_restored"]
    scope, = result["function_scopes"]["scopes"]
    assert scope["state"] == "incomplete"
    assert not result["function_scopes"]["replayable"]
    assert result["protocol"]["cleanup_complete"]
    if point != "entry-hit":
        assert result["cleanup"]["interrupted_step"]["trap_flag_after"] == 0


@pytest.mark.parametrize("point", ["step-prepared", "step-stop"])
def test_backend_failure_keeps_scope_evidence_and_restores_child(child, point):
    result = {}
    with pytest.raises(RuntimeError, match="injected failure"):
        observe_entry_imports(child, fail_at=point, report=result)
    assert result["cleanup"]["detached"]
    assert result["cleanup"]["all_debugger_bytes_restored"]
    assert result["cleanup"]["interrupted_step"]["trap_flag_after"] == 0
    scope, = result["function_scopes"]["scopes"]
    assert scope["state"] == "incomplete"
    retained = json.loads(Path(result["evidence_path"]).read_text(encoding="utf-8"))
    assert retained["function_scopes"] == result["function_scopes"]
    assert retained["entry_snapshot"] == result["entry_snapshot"]
