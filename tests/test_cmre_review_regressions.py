"""Concrete worker-pass defects retained as original-vs-candidate experiments.

Target: pinned 3.9.68 SHA-256 from env.TARGET_SHA256; addresses are preferred VAs.
Synthetic inputs exercise the evidenced object offsets, not invented game rules.
"""
import struct
from pathlib import Path

import pytest

from cmre import difftest, emu, env, recover


@pytest.mark.parametrize("va,t_offset,w_offset,flag", [
    (0x009345E0, 0x3C8, 0x18, 0x10),
    (0x009347A0, 0x3CC, 0x1C, 0x20),
])
def test_two_node_cycle_requires_saved_start_node(tmp_path, monkeypatch, va, t_offset, w_offset, flag):
    # Two flagged nodes link 0 -> 1 -> 0. Both original routines finish when
    # the walk returns to the saved start, not to the current node.
    arena = bytearray(emu.ARENA_SIZE)
    p, q = emu.ARENA + 0x100, emu.ARENA + 0x600
    links, flags, t, w, lo, hi = [emu.ARENA + n for n in (0x1000, 0x1100, 0x1200, 0x1300, 0x1400, 0x1500)]
    values = {p + 0x1C: links, p + 0x28: flags, p + t_offset: t,
              q: 1, q + 4: lo, q + 8: hi, q + w_offset: w,
              links: 1, flags: flag | (flag << 16), t: 100, t + 4: 200,
              w: 150, w + 4: 250, lo: 0, hi: 1}
    for address, value in values.items():
        struct.pack_into("<I", arena, address - emu.ARENA, value)
    regs = dict.fromkeys(emu.REG_NAMES[:8], 0)
    regs["eflags"] = 0x202
    case = emu.Case(regs, [p, q] + [0] * 14, bytes(arena), {}, 0)
    monkeypatch.setattr(difftest, "make_case", lambda *args: case)
    source = (env.C_DIR / f"fn_{va:08x}.c").read_text(encoding="utf-8")
    assert "while (r != jstart)" in source
    candidate = tmp_path / f"fn_{va:08x}.c"
    altered = (source.replace("while (r != jstart)", "while (r != jcur)")
               .replace("    s32 jstart;\n", "").replace("            jstart = j;\n", ""))
    for text, broken in ((source, False), (altered, True)):
        candidate.write_text(text, encoding="utf-8")
        report = recover.test_c(va, {"cases": 2, "budget": 2000}, fresh=True, source=candidate, profile="semantic")
        for build in report["builds"].values():
            assert "difftest" in build, build
            result = build["difftest"]
            assert result["outcomes"] == {"return": 2}
            assert bool(result["mismatches"]) == broken
            if broken:
                assert result["mismatches"][0]["kind"] == "budget-asymmetry"


def test_record_move_reloads_base_when_records_alias_object(tmp_path):
    # Pinned original, deterministic case 0: the six-byte records overlap the
    # object's base field. The original reloads that field at VA 0x007c6c50.
    # A pointer-typed, nonvolatile field read can be hoisted past DWORD stores
    # under C aliasing rules, causing a different eventual fault under -O2.
    va = 0x007C6BF0
    fixture = Path(__file__).parent / "fixtures/record_move_alias.json"
    (env.HINTS_DIR / f"fn_{va:08x}.json").write_bytes(fixture.read_bytes())
    source = (env.C_DIR / f"fn_{va:08x}.c").read_text(encoding="utf-8")
    candidate = tmp_path / f"fn_{va:08x}.c"
    candidate.write_text(source, encoding="utf-8")
    options = {"cases": 1, "budget": 800000}
    report = recover.test_c(va, options, fresh=True, source=candidate, profile="semantic")
    assert report["status"] == "pass", {
        "problems": report["problems"],
        "builds": {opt: {k: build.get("difftest", {}).get(k)
                         for k in ("mismatches", "coverage", "inconclusive")}
                   for opt, build in report["builds"].items()},
    }
    assert all(b["difftest"]["inconclusive"] == 0 for b in report["builds"].values())

    broken = source.replace("volatile ", "").replace(
        "*(u32 *)(object + 0x3cu)", "(u32)*(u8 **)(object + 0x3cu)")
    assert broken != source
    candidate.write_text(broken, encoding="utf-8")
    report = recover.test_c(va, options, fresh=True, source=candidate, profile="semantic")
    assert report["builds"]["-O0"]["status"] == "pass"
    mismatches = report["builds"]["-O2"]["difftest"]["mismatches"]
    assert any(m.get("case") == 0 and m.get("field") == "detail" for m in mismatches)
