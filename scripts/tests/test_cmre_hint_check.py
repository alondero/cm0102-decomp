"""Hint preflight is asset-free and reuses the existing strict schema."""
import importlib.util
import json
from pathlib import Path

import pytest

MODULE = Path(__file__).resolve().parents[1] / "cmre_hint_check.py"
spec = importlib.util.spec_from_file_location("cmre_hint_check", MODULE)
check = importlib.util.module_from_spec(spec)
spec.loader.exec_module(check)


def fixture(count=10):
    return {"regs": {name: 0 for name in
                     ("eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "eflags")},
            "stack": [0] * count, "arena_template": 0,
            "arena_writes": {}, "globals": {}, "import_seed": 0}


def write(tmp_path, hints):
    path = tmp_path / "hints.json"
    path.write_text(json.dumps(hints), encoding="utf-8")
    return path


def test_valid_c_only_fixture_and_metadata_are_read_only(tmp_path, capsys):
    path = write(tmp_path, {"captures": ["a" * 64], "c_hints": {
        "arg_count": 18, "fixtures": [fixture(18)],
        "fixture_provenance": {"source": "original instructions"}}})
    before = path.read_bytes()
    assert check.main([str(path)]) == 0
    out = json.loads(capsys.readouterr().out)
    assert out["validation"] == "schema-only"
    assert (out["c_arg_count"], out["c_fixtures"], out["c_captures"]) == (18, 1, 1)
    assert out["acceptance"] == "not-checked" and not out["recovery_changed"]
    assert path.read_bytes() == before


@pytest.mark.parametrize("hints,reason", [
    ({"provenance": {}}, "supported fields"),
    ({"fixtures": [{**fixture(), "regs": {"eax": 0}}]}, "every initial register"),
    ({"fixtures": [fixture(18)]}, "stack length differs"),
    ({"c_hints": {"cases": 1}}, "not acceptance policy"),
    ([], "supported fields"),
])
def test_rejects_invalid_hints_without_building(tmp_path, capsys, monkeypatch, hints, reason):
    def forbidden(*args, **kwargs):
        raise AssertionError("schema preflight executed a differential test")
    monkeypatch.setattr(check.difftest, "run", forbidden)
    path = write(tmp_path, hints)
    assert check.main([str(path)]) == 2
    result = json.loads(capsys.readouterr().out)
    assert result["status"] == "fail" and reason in result["reason"]


def test_empty_hints_and_missing_file(tmp_path, capsys):
    assert check.main([str(write(tmp_path, {}))]) == 0
    capsys.readouterr()
    assert check.main([str(tmp_path / "absent.json")]) == 2
    assert json.loads(capsys.readouterr().out)["validation"] == "schema-only"


def test_malformed_json(tmp_path, capsys):
    path = tmp_path / "broken.json"
    path.write_text("{", encoding="utf-8")
    assert check.main([str(path)]) == 2
    assert json.loads(capsys.readouterr().out)["status"] == "fail"


def test_error_output_is_bounded(tmp_path, capsys):
    path = write(tmp_path, {"arg_count": [0] * 2000})
    assert check.main([str(path)]) == 2
    result = json.loads(capsys.readouterr().out)
    assert result["reason_truncated"] and len(result["reason"]) == 600
