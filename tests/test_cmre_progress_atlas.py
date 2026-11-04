"""Asset-free progress accounting, offline exports and automatic refresh."""
import json
import re

import pytest

from cmre import cli, env, inventory, queue, shards
from scripts import cmre_progress as progress


def facts():
    return {"schema": shards.FACTS_SCHEMA, "input_sha256": "pinned-target",
            "inventory": {"sha256": {"functions.tsv": "pinned-inventory"},
                          "functions": {"fn_00401000": 200, "fn_00401010": 50, "fn_00401020": 30,
                                        "fn_00401030": 20, "fn_00401040": 0}},
            "source": ["fn_00401000"], "lifted": ["fn_00401000", "fn_00401010"],
            "lift_blocked": ["fn_00401020"], "held": {}, "generated": {},
            "accepted_holds": {}, "c_ready": [], "composed": ["fn_00401000"],
            "integrated": [], "outside": {"fn_00999999": ["accepted-source"]}}


def snapshot(raw=None, **kwargs):
    return progress.snapshot(raw or facts(), {"fn_00401000": [r"C:\private\code\date\date.cpp"]},
                             generated_at=kwargs.get("generated_at", "2026-11-04T12:00:00+00:00"),
                             revision=kwargs.get("revision", "deadbeef"),
                             source_kinds={"fn_00401000": "c"})


def test_area_and_percentages_use_inventory_bytes_with_no_double_counting():
    data = snapshot()
    assert data["totals"]["inventory"] == {"functions": 5, "body_bytes": 300}
    assert sum(r["bytes"] for r in data["functions"]) == 300
    assert data["totals"]["source"] == {"functions": 1, "body_bytes": 200}
    assert data["totals"]["c"] == data["totals"]["source"]
    assert data["totals"]["legacy"] == {"functions": 0, "body_bytes": 0}
    assert data["totals"]["recovered"] == {"functions": 2, "body_bytes": 250}
    assert data["totals"]["composed"] == {"functions": 1, "body_bytes": 200}
    assert data["totals"]["integrated"] == {"functions": 0, "body_bytes": 0}
    assert len({r["id"] for r in data["functions"]}) == 5
    assert data["functions"][0]["state"] == "source"
    assert data["functions"][1]["state"] == "c-queue"
    assert data["functions"][2]["state"] == "lift-blocked"
    assert data["outside_inventory"] == {"fn_00999999": ["accepted-source"]}


def test_module_candidates_are_sanitized_and_ambiguity_is_explicit():
    assert progress.module_label([r"C:\private\code\match\match.cpp"]) == "match/match.cpp"
    assert progress.module_label(["/private/user/match.cpp"]) == "match.cpp"
    assert progress.module_label(["one.cpp", "two.cpp"]) == "Ambiguous module"
    assert progress.module_label([r"C:\a\code\date.cpp", "/b/code/date.cpp"]) == "date.cpp"
    assert progress.module_label([]) == "Unknown module"
    assert progress.module_label(["C:/private/code/../secret.cpp"]) == "Unknown module"


def test_history_deduplicates_unchanged_state_but_retains_real_changes_and_reversals():
    first = snapshot()
    history = progress.history_with([], first)
    assert len(history) == 1
    assert progress.history_with(history, snapshot(generated_at="later", revision="another")) == history
    changed = facts(); changed["source"].append("fn_00401010")
    second = snapshot(changed)
    assert second["digest"] != first["digest"]
    history = progress.history_with(history, second)
    history = progress.history_with(history, first)
    assert len(history) == 3
    assert history[-1]["totals"]["source"]["body_bytes"] == 200
    changed["inventory"]["sha256"] = {"functions.tsv": "new"}
    assert snapshot(changed)["digest"] != second["digest"]


def test_offline_html_escapes_script_breakouts_and_excludes_raw_analysis():
    data = snapshot()
    data["functions"][0]["module"] = '</script><script>alert("bad")</script>&'
    html = progress.render(data, [])
    embedded = re.search(r'<script id="progress-data" type="application/json">(.*?)</script>', html, re.S)[1]
    assert "<" not in embedded and "&" not in embedded
    assert json.loads(embedded)["snapshot"] == data
    assert 'src="http' not in html and 'href="http' not in html
    assert "/* PROGRESS_" not in html
    assert "C:\\private" not in html
    assert not {"signature", "disassembly", "instructions", "name", "source_path"} & data["functions"][0].keys()


def test_export_retains_history_and_does_not_replace_good_html_on_input_failure(tmp_path, monkeypatch):
    data = snapshot()
    monkeypatch.setattr(progress, "capture", lambda: data)
    index = progress.write_dashboard(tmp_path)
    original = index.read_bytes()
    progress.write_dashboard(tmp_path)
    assert len(json.loads((tmp_path / "history.json").read_text())) == 1
    assert not list(tmp_path.glob("*.tmp"))
    def fail():
        raise ValueError("Missing pinned inventory")
    monkeypatch.setattr(progress, "capture", fail)
    with pytest.raises(ValueError, match="Missing pinned"):
        progress.write_dashboard(tmp_path)
    assert index.read_bytes() == original


def test_capture_rejects_missing_inputs_even_when_inventory_cache_exists(tmp_path, monkeypatch):
    monkeypatch.setattr(env, "INVENTORY_SHA256", {tmp_path / "functions.tsv": "missing"})
    monkeypatch.setattr(inventory, "load", lambda: pytest.fail("must reject missing pins before cached load"))
    with pytest.raises(ValueError, match="Missing or changed pinned inventory"):
        progress.capture()


def test_capture_uses_the_captured_source_set_without_rereading_acceptance(tmp_path, monkeypatch):
    from types import SimpleNamespace
    from cmre import ledger

    pin = tmp_path / "inventory.tsv"
    pin.write_text("pinned")
    monkeypatch.setattr(env, "INVENTORY_SHA256", {pin: env.digest(pin)})
    monkeypatch.setattr(env, "REGISTRY", pin)
    raw = facts()
    raw["source"].append("fn_00401010")
    monkeypatch.setattr(shards, "gather", lambda: raw)
    monkeypatch.setattr(ledger, "legacy_entries", lambda: [SimpleNamespace(va=0x401000)])
    monkeypatch.setattr(ledger, "recovered", lambda: pytest.fail("mutable acceptance must not be reread"))
    monkeypatch.setattr(inventory, "load", lambda: SimpleNamespace(modules={}))
    monkeypatch.setattr(progress.subprocess, "run", lambda *a, **kw: SimpleNamespace(returncode=0, stdout="abcd\n"))
    data = progress.capture()
    assert data["totals"]["source"]["body_bytes"] == 250
    assert data["totals"]["c"]["body_bytes"] == 50
    assert data["totals"]["legacy"]["body_bytes"] == 200


def test_empty_inventory_has_no_fabricated_source_or_byte_coverage():
    raw = facts()
    raw["inventory"]["functions"] = {}
    data = snapshot(raw)
    assert data["functions"] == []
    assert all(value == {"functions": 0, "body_bytes": 0} for value in data["totals"].values())


def test_status_write_refreshes_atlas_but_plain_status_does_not(tmp_path, monkeypatch):
    calls = []
    monkeypatch.setattr(queue, "coverage_summary", lambda: {"sample": 1})
    monkeypatch.setattr(cli, "_write_status", lambda summary: calls.append("status"))
    monkeypatch.setattr(progress, "write_dashboard", lambda output=None: calls.append(output) or tmp_path / "index.html")
    assert cli.main(["status", "--json"]) == 0
    assert calls == []
    # Calling the handler avoids acquiring a command lease in the real workspace.
    from types import SimpleNamespace
    assert cli.cmd_status(SimpleNamespace(json=True, write=True)) == 0
    assert calls == ["status", None]
    assert cli.main(["progress", "--output", str(tmp_path)]) == 0
    assert calls[-1] == tmp_path
