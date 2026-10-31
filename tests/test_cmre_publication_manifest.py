"""The status manifest lists only published recovered C, with its status and exact-byte hash."""
import hashlib
import json

import pytest

from cmre import publication as pub

LISTING = "".join(f"  {0x401000 + 2 * i:x}:\t8b c1\tmov    %ecx,%eax\n" for i in range(8))
BODY = "int fn(int a) {\n    return a + 1;\n}\n"


@pytest.fixture
def tree(tmp_path):
    supplied = tmp_path / "supplied"
    supplied.mkdir()
    (supplied / "game.exe").write_bytes(b"MZ" + bytes(range(256)))
    c_dir = tmp_path / "c"
    ledger_dir = tmp_path / "ledger"
    c_dir.mkdir()
    ledger_dir.mkdir()
    return {"c": c_dir, "ledger": ledger_dir, "index": pub.OriginalIndex.build([supplied])}


def _write(tree, fid, text, accepted_text=None, ledger=True):
    data = text.encode()
    (tree["c"] / f"{fid}.c").write_bytes(data)
    if ledger:
        record = {"name": "FUN_secret", "module_candidates": ["e:\\dev\\private\\x.cpp"]}
        if accepted_text is not None:
            record["c"] = {"source_sha256": hashlib.sha256(accepted_text.encode()).hexdigest()}
        (tree["ledger"] / f"{fid}.json").write_text(json.dumps(record), encoding="utf-8")
    return data


def test_manifest_statuses_and_exclusions(tree):
    _write(tree, "fn_00000001", BODY, accepted_text=BODY)
    _write(tree, "fn_00000002", BODY + "/* WIP */\n", accepted_text=None)
    _write(tree, "fn_00000003", LISTING, accepted_text=LISTING)
    _write(tree, "fn_00000004", BODY, accepted_text="a different accepted source\n")
    _write(tree, "fn_00000005", BODY, ledger=False)
    rows = pub.manifest_rows(tree["c"], tree["ledger"], tree["index"])
    assert rows == [
        ("fn_00000001", "accepted", hashlib.sha256(BODY.encode()).hexdigest()),
        ("fn_00000002", "wip", hashlib.sha256((BODY + "/* WIP */\n").encode()).hexdigest()),
        ("fn_00000005", "wip", hashlib.sha256(BODY.encode()).hexdigest()),
    ]


def test_manifest_file_holds_only_the_three_columns(tree, tmp_path):
    _write(tree, "fn_00000001", BODY, accepted_text=BODY)
    out = tmp_path / "STATUS.tsv"
    pub.write_manifest(out, pub.manifest_rows(tree["c"], tree["ledger"], tree["index"]))
    lines = out.read_text(encoding="utf-8").splitlines()
    assert lines[0] == "function\tstatus\tsha256"
    assert len(lines) == 2 and lines[1].split("\t")[:2] == ["fn_00000001", "accepted"]
    text = out.read_text(encoding="utf-8")
    assert "FUN_secret" not in text and "e:\\dev" not in text
