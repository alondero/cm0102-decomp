"""Extraction seam: real authored child, never a game or arbitrary process."""
import uuid

import pytest

from native_debug_fixture import ROOT, Unavailable, build_fixture
from native_import_observer import observe_calls


def test_backend_rejects_nonowned_target_before_any_process_access():
    from cmre.debug_observe import run_session

    with pytest.raises(ValueError, match="owned authored child"):
        run_session(1234)


def test_existing_fixture_client_runs_through_extracted_physical_backend(monkeypatch):
    from cmre import debug_observe

    directory = ROOT / "local/debug-observe-extraction" / uuid.uuid4().hex
    try:
        exe = build_fixture(directory, external_breakpoints=True)
    except Unavailable as exc:
        pytest.skip(str(exc))
    calls = []
    original = debug_observe.run_session

    def run(client, **options):
        result = original(client, **options)
        calls.append((client.pid, client.ownership))
        return result

    monkeypatch.setattr(debug_observe, "run_session", run)
    result = observe_calls(exe, "ordinary")
    assert calls == [(result["protocol"]["source"]["pid"], "owned-authored-child")]
    assert result["child_returncode"] == 0
    assert result["cleanup"]["all_debugger_bytes_restored"]
    assert result["cleanup"]["detached"]
    assert result["protocol"]["cleanup_complete"]
    assert result["protocol"]["replayable"] is False
    assert len(result["steps"]) == 2
    assert all(step["trap_flag_after"] == 0 for step in result["steps"])


def test_step_ownership_distinguishes_reused_threads():
    from cmre.debug_observe import _owns_step
    from cmre.import_observe import ThreadInstance

    owner = ThreadInstance(41, 2)
    assert _owns_step(owner, ThreadInstance(41, 2))
    assert not _owns_step(owner, ThreadInstance(41, 1))
    assert not _owns_step(owner, ThreadInstance(42, 2))
    assert not _owns_step(owner, None)
    with pytest.raises(TypeError, match="thread instance"):
        _owns_step(41, 41)
