"""Tier-2 C path: faithful C passes at O0 and O2; altered C fails."""
import pytest

from cmre import env, recover

VA = 0x004671B0  # lea eax, [ecx + 0x20]; ret   (thiscall, returns this + 0x20)
FAITHFUL = """#include "cmre.h"
/* 0x004671B0: address of the member at offset 0x20. */
CM_THISCALL void *cm_fn_004671b0(void *self) { return (u8 *)self + 0x20; }
"""


@pytest.fixture
def c_dir(tmp_path, monkeypatch):
    monkeypatch.setattr(env, "C_DIR", tmp_path)
    return tmp_path


def test_faithful_c_passes_at_both_optimisation_levels(c_dir):
    (c_dir / f"fn_{VA:08x}.c").write_text(FAITHFUL)
    r = recover.test_c(VA, {"cases": 100}, fresh=True)
    assert r["status"] == "pass", r
    assert set(r["builds"]) == {"-O0", "-O2"}


def test_altered_c_fails(c_dir):
    (c_dir / f"fn_{VA:08x}.c").write_text(FAITHFUL.replace("0x20", "0x24"))
    r = recover.test_c(VA, {"cases": 100}, fresh=True)
    assert r["status"] == "fail"


def test_clobbering_a_callee_saved_register_fails(c_dir):
    bad = FAITHFUL.replace("{ return", '{ __asm__ volatile("xor %%esi, %%esi" ::: ); return')
    (c_dir / f"fn_{VA:08x}.c").write_text(bad)
    r = recover.test_c(VA, {"cases": 100}, fresh=True)
    assert r["status"] == "fail"


def test_decorated_stdcall_and_fastcall_symbols_link(tmp_path):
    # MinGW decorates stdcall as _name@N and fastcall as @name@N; callees must bind to the original VA.
    from cmre import build

    src = tmp_path / "fn_00450a70.c"
    src.write_text('#include "cmre.h"\n'
                   "extern CM_STDCALL u32 cm_fn_00412ef0(u32 a, u32 b);\n"
                   "extern CM_FASTCALL u32 cm_fn_004671b0(u32 a);\n"
                   "CM_STDCALL u32 cm_fn_00450a70(u32 x) { return cm_fn_00412ef0(x, 1) + cm_fn_004671b0(x); }\n")
    built = build.build(src, "_cm_fn_00450a70", tmp_path, "-O0")
    assert built.entry == built.symbols["_cm_fn_00450a70@4"]
    assert built.symbols["_cm_fn_00412ef0@8"] == 0x00412EF0
    assert built.symbols["@cm_fn_004671b0@4"] == 0x004671B0
