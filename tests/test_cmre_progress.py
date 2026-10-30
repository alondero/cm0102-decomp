"""Progress distinguishes source acceptance from lifts and excludes unmapped extents."""
from types import SimpleNamespace

from cmre import cli, env, ledger, queue


def test_source_coverage_uses_inventory_bytes_and_unique_accepted_entries(monkeypatch, capsys, tmp_path):
    def entry(va, *, legacy=False, c=False):
        return SimpleNamespace(va=va, legacy=legacy, size=99999, record={"c": {} if not c else {"source": "c"}})

    recovered = {1: entry(1, c=True), 2: entry(2, legacy=True),
                 3: entry(3), 4: entry(4, legacy=True)}
    inv = SimpleNamespace(functions={va: SimpleNamespace(body_bytes=n) for va, n in ((1, 30), (2, 20), (3, 50))})
    monkeypatch.setattr(queue.inventory, "load", lambda: inv)
    monkeypatch.setattr(ledger, "recovered", lambda: recovered)
    monkeypatch.setattr(ledger, "blocked", lambda: {})
    summary = queue.coverage_summary()
    assert summary["recovered_body_bytes"] == 100
    assert summary["source_body_bytes"] == 50
    assert summary["source_percent"] == 50
    assert summary["c_tier_body_bytes"] == 30
    assert summary["legacy_body_bytes"] == 20
    assert summary["source_functions"] == 3
    assert summary["source_unmapped_functions"] == 1
    assert summary["lifted_functions"] == 2  # C-tier remains a subset of lifted records.

    (tmp_path / "docs").mkdir()
    monkeypatch.setattr(env, "ROOT", tmp_path)
    assert cli.cmd_status(SimpleNamespace(json=False, write=True)) == 0
    output = capsys.readouterr().out
    assert output.index("accepted C/legacy") < output.index("recovered")
    assert "50/100" in output
    document = (tmp_path / "docs/STATUS.md").read_text(encoding="utf-8")
    assert "| source body bytes | 50 |" in document
    assert "portable" in document


def test_empty_inventory_reports_zero_without_division_or_extent_fallback(monkeypatch):
    monkeypatch.setattr(queue.inventory, "load", lambda: SimpleNamespace(functions={}))
    monkeypatch.setattr(ledger, "recovered", lambda: {})
    monkeypatch.setattr(ledger, "blocked", lambda: {})
    summary = queue.coverage_summary()
    assert summary["source_percent"] == summary["recovered_percent"] == 0
