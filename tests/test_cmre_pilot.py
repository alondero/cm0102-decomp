import pytest
from types import SimpleNamespace

from cmre import pilot


def test_unknown_usage_cannot_be_reported_as_token_efficiency(monkeypatch):
    monkeypatch.setattr(pilot, "accepted", lambda: {})
    monkeypatch.setattr(pilot.work, "read", lambda: {"functions": {}})
    data = {"started": 10, "ended": 20, "functions": {}, "calls": {}, "usage_complete": False,
            "harness": pilot.recover.harness_digest(), "target": pilot.env.TARGET_SHA256}
    report = pilot.report(data)
    assert not report["usage_complete"]
    assert all(value is None for value in report["tokens_per_accepted_byte"].values())


def test_model_usage_is_deduplicated_by_provider_call_id():
    data = {"ended": None, "calls": {}}
    usage = dict.fromkeys(pilot.TOKEN_FIELDS, 5)
    pilot.record_usage(data, "call1", "observed-model", usage, 2)
    with pytest.raises(ValueError, match="unique"):
        pilot.record_usage(data, "call1", "observed-model", usage, 2)
    assert len(data["calls"]) == 1


@pytest.mark.parametrize("name", ["../escape", "C:/file", "a/b", "", "a\\b"])
def test_pilot_paths_stay_under_analysis(name):
    with pytest.raises(ValueError):
        pilot.path(name)


def test_rates_require_matching_accepted_source_and_complete_usage(tmp_path, monkeypatch):
    monkeypatch.setattr(pilot.env, "C_DIR", tmp_path)
    source = tmp_path / "fn_00401000.c"
    source.write_text("accepted source")
    entry = SimpleNamespace(record={"c": {"source_sha256": pilot.env.digest(source)}})
    monkeypatch.setattr(pilot, "accepted", lambda: {0x401000: entry})
    monkeypatch.setattr(pilot.work, "read", lambda: {"functions": {}})
    data = {"started": 0, "ended": 3600,
            "functions": {"fn_00401000": {"already_accepted": False, "bytes": 100, "attempts": 0, "failures": 0}},
            "calls": {"call": {"input_tokens": 200, "output_tokens": 100, "cache_read_tokens": 0,
                                "cache_write_tokens": 0, "review_corrections": 1}}, "usage_complete": True,
            "harness": pilot.recover.harness_digest(), "target": pilot.env.TARGET_SHA256}
    result = pilot.report(data)
    assert result["accepted_bytes_per_hour"] == 100
    assert result["tokens_per_accepted_byte"]["output_tokens"] == 1
    source.write_text("changed without acceptance")
    assert pilot.report(data)["accepted_bytes"] == 0


def test_finished_pilot_never_credits_later_acceptance(monkeypatch):
    frozen = {"accepted_bytes": 100, "finished": True}
    def unexpected_read():
        raise AssertionError("finished measurement must not query later ledgers")
    monkeypatch.setattr(pilot, "accepted", unexpected_read)
    assert pilot.report({"result": frozen}) == frozen


def test_finish_command_freezes_before_later_acceptance(tmp_path, monkeypatch, capsys):
    from cmre import cli
    monkeypatch.setattr(pilot.env, "ANALYSIS", tmp_path)
    monkeypatch.setattr(pilot.env, "C_DIR", tmp_path)
    monkeypatch.setattr(pilot.inventory, "load", lambda: SimpleNamespace(
        functions={0x401000: SimpleNamespace(body_bytes=100)}))
    monkeypatch.setattr(pilot.ledger, "legacy_entries", lambda: [])
    current, states = {}, {}
    monkeypatch.setattr(pilot, "accepted", lambda: current)
    monkeypatch.setattr(pilot.work, "read", lambda: {"functions": states})
    assert cli.main(["pilot", "start", "test", "--functions", "fn_00401000"]) == 0
    assert cli.main(["pilot", "finish", "test"]) == 0
    capsys.readouterr()
    source = tmp_path / "fn_00401000.c"
    source.write_text("later accepted")
    current[0x401000] = SimpleNamespace(record={"c": {"source_sha256": pilot.env.digest(source)}})
    states["fn_00401000"] = {"attempts": 9, "failures": 5}
    assert cli.main(["pilot", "report", "test"]) == 0
    import json
    observed = json.loads(capsys.readouterr().out)
    assert observed["accepted_bytes"] == 0
    assert observed["test_invocations"] == observed["failed_invocations"] == 0
