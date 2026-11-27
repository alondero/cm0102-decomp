"""Opt-in regression against the retained, hash-matched fn_005c8a00 candidate."""

import json
import os
from pathlib import Path
from types import SimpleNamespace

import pytest

from cmre import emu, env, localize, recover


def test_localize_known_bad_fn005c8a00(monkeypatch):
    fixture_dir = os.environ.get("CMRE_LOCALIZE_REPRO")
    if fixture_dir is None:
        pytest.skip("set CMRE_LOCALIZE_REPRO to the local fn_005c8a00 regression bundle")
    bundle = Path(fixture_dir)
    monkeypatch.setattr(env, "HINTS_DIR", bundle / "hints")
    source = bundle / "fn_005c8a00.c"
    gate = bundle / "gate.json"
    retained = json.loads(gate.read_bytes())
    expected = retained["builds"]["-O2"]["difftest"]
    assert retained["source_sha256"] == env.digest(source)
    assert expected["coverage"] == 0.7567
    assert len(expected["mismatches"]) == 6
    first = expected["mismatches"][0]
    assert (first["case"], first["kind"], first["original"], first["recovered"]) == (
        1, "budget-asymmetry", "budget", "return",
    )

    control = recover.explain_c(0x005C8A00, first["case"], source=source, profile="semantic")
    assert control["difference"] == {
        "field": "outcome", "original": first["original"], "recovered": first["recovered"],
    }

    report = localize.run(0x005C8A00, gate, source=source, opt="-O2")
    assert report["gate_mismatch"] == first
    assert report["classification"] == "candidate-source-defect"
    assert report["original_stop"]["outcome"] == "budget"
    assert report["recovered_stop"]["outcome"] == "return"
    assert report["divergence"] == {
        "kind": "call",
        "original": {
            "kind": "call", "site": 0x0095002D, "target": "KERNEL32.dll!CreateFileA",
            "args": (0x00DE279C, 0x80000000, 3, 0x0203EBB0, 3, 128, 0), "import_index": 0,
        },
        "recovered": None,
    }


def test_final_write_uses_differing_byte_and_ignores_equivalent_store_widths():
    original = SimpleNamespace(writes=[
        (0, {"kind": "write", "address": 0x1000, "size": 4, "value": 1, "site": 0x1230}),
        (1, {"kind": "write", "address": 0x1004, "size": 4, "value": 9, "site": 0x1234}),
    ], writes_truncated=False)
    recovered = SimpleNamespace(writes=[
        (0, {"kind": "write", "address": 0x1000, "size": 1, "value": 1, "site": 0x2340}),
        (1, {"kind": "write", "address": 0x1004, "size": 1, "value": 7, "site": 0x2344}),
    ], writes_truncated=False)
    point = localize._point(localize._first_write(original, recovered, {0x1004: (9, 7)}))
    assert (point["original"]["site"], point["original"]["final_byte"]) == (0x1234, 9)
    assert (point["recovered"]["site"], point["recovered"]["final_byte"]) == (0x2344, 7)


def test_pure_helper_inlining_cannot_replace_the_import_witness():
    call = {"kind": "call", "target": "import", "import_index": 0}
    original = SimpleNamespace(calls=[(3, call)], transfers=[{"target": 0x401000}])
    recovered = SimpleNamespace(calls=[], transfers=[])
    na = ("budget", (), (), [("c", "import", 0, ())], {})
    nb = ("return", (), (), [], {})
    assert localize._point(localize._first_call(original, recovered, na, nb, None)) == {
        "kind": "call", "original": call, "recovered": None,
    }

def test_write_locations_exclude_an_equivalent_unaligned_relocation_on_a_mixed_page():
    amap = emu.AddressMap({"v_00401000": 0x6E5D1000}, {}, 0x6E5D0000, 0x6E5E0000)
    original = b"\x00" + (0x00401000).to_bytes(4, "little") + b"\x00\x00\x01"
    recovered = b"\x00" + (0x6E5D1000).to_bytes(4, "little") + b"\x00\x00\x02"
    assert localize._changed_bytes({0x1000: original}, {0x1000: recovered}, None, None, amap, "abi") == {
        0x1007: (1, 2),
    }
