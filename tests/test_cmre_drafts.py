"""Ghidra draft conversion and the translation brief (inputs only, never evidence)."""
import pytest

from cmre import drafts, env, recover

GHIDRA = """/* UNVALIDATED GHIDRA 12.1.4 PSEUDOCODE for preferred-image VA 0x006869a0. */

undefined4 __thiscall FUN_006869a0(int param_1,uint param_2)

{
  undefined2 uVar1;
  uVar1 = CONCAT11((char)param_2,DAT_00ae2364);
  FUN_00785d40(param_1,param_2,0);
  return CONCAT22(uVar1,*(ushort *)s_Error_009870e0);
}
"""


def test_types_conventions_functions_and_helpers_are_converted():
    source, problems = drafts.convert(0x006869A0, GHIDRA)
    assert problems == []
    assert "u32 CM_THISCALL cm_fn_006869a0(int param_1,u32 param_2)" in source
    assert "cm_fn_00785d40(param_1,param_2,0);" in source
    assert "#define CONCAT11(a, b)" in source and "#define CONCAT22(a, b)" in source
    assert "#define s_Error_009870e0 ((char *)0x009870e0)" in source
    assert "#define DAT_00ae2364 CM_GLOBAL(" in source
    assert "extern " in source and "cm_fn_00785d40(" in source.split("\n\n")[0]


def test_concat_with_a_three_byte_piece_masks_it_instead_of_crashing():
    text = GHIDRA.replace("CONCAT11((char)param_2,DAT_00ae2364)", "CONCAT13((char)param_2,param_1)")
    text = text.replace("CONCAT22(uVar1,", "CONCAT31(param_1,")
    source, _ = drafts.convert(0x006869A0, text)
    assert "#define CONCAT13(a, b) ((u32)(((u32)(u8)(a) << 24) | ((u32)(b) & 0xffffffu)))" in source
    assert "#define CONCAT31(a, b) ((u32)(((u32)(u32)(a) << 8) | (u32)(u8)(b)))" in source


def test_inexpressible_registers_are_reported():
    _, problems = drafts.convert(0x006869A0, GHIDRA.replace("param_1,param_2,0", "unaff_ESI,extraout_ECX,0"))
    assert problems == ["extraout_ECX", "unaff_ESI"]


@pytest.mark.skipif(not (drafts.DRAFTS / "fn_006869a0.c").exists(), reason="local Ghidra drafts not exported")
def test_converted_draft_passes_semantic_gate(tmp_path, monkeypatch):
    # This function's recorded input hints supply the coverage its acceptance uses.
    monkeypatch.setattr(env, "HINTS_DIR", env.ROOT / "recovery/hints")
    path, problems = drafts.write(0x006869A0, tmp_path / "fn_006869a0.c")
    assert problems == []
    assert recover.test_c(0x006869A0, source=path, profile="semantic")["status"] == "pass"


def test_accepted_prototype_strips_nested_attributes():
    proto = drafts.accepted_prototype(0x00785D40)
    assert proto.startswith("unsigned long long CM_THISCALL cm_fn_00785d40(")
    assert "__attribute__" not in proto and ") cm_fn" not in proto


@pytest.mark.parametrize("source, expected", [
    # Legacy abi records define asm-statement macros before the definition (fn_009452ca).
    ("#define STATE \\\n    \"+a\"(eax), \"+c\"(ecx)\n#define STEP(text) \\\n"
     "    __asm__ volatile(text : STATE : : \"memory\", \"cc\")\n\n"
     "unsigned long long CM_CDECL\n__attribute__((naked, optimize(\"O2\", \"align-loops=1\")))\n"
     "cm_fn_00450a70(u32 arg4 __attribute__((unused)))\n{\n}\n",
     "unsigned long long CM_CDECL cm_fn_00450a70(u32 arg4)"),
    ("#define COPY(d, s) \\\n    (*(u32 *)(d) = *(u32 *)(s))\nvoid CM_THISCALL cm_fn_00450a70(void *self) {\n}\n",
     "void CM_THISCALL cm_fn_00450a70(void *self)"),
    ("#define ONE 1\n#define TWO \\\n  2\nu32 *cm_fn_00450a70(void)\n{\n}\n", "u32 *cm_fn_00450a70(void)"),
])
def test_accepted_prototype_skips_continued_macros(tmp_path, monkeypatch, source, expected):
    monkeypatch.setattr(env, "C_DIR", tmp_path)
    (tmp_path / "fn_00450a70.c").write_text(source)
    assert drafts.accepted_prototype(0x00450A70) == expected


def test_legacy_abi_callee_prototype_is_a_declaration():
    assert drafts.prototype(0x009452CA) == "extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);"


@pytest.mark.skipif(not (drafts.DRAFTS / "fn_007862d0.c").exists(), reason="local Ghidra drafts not exported")
def test_brief_is_compact_and_complete():
    text = drafts.brief(0x007862D0)
    assert "evidence: ret 8" in text and "registers read before written: ecx" in text
    assert "callee C        extern unsigned long long CM_THISCALL cm_fn_00785d40(" in text
    assert "global 0x00ae2364 int (4 bytes)" in text
    assert "007862d0  sub esp, 0x200" in text


SEH = """undefined4 __cdecl FUN_006869a0(int param_1)

{
  undefined4 local_c;
  undefined *puStack_8;
  puStack_8 = &LAB_00953819;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  if (param_1 == 0) goto LAB_006869b0;
  ExceptionList = local_c;
  return 1;
LAB_006869b0:
  return 0;
}
"""


def test_ghidra_seh_identifiers_become_fs_and_address_constants():
    source, _ = drafts.convert(0x006869A0, SEH)
    assert "#define ExceptionList CM_FS(void *, 0)" in source
    assert "#define LAB_00953819 CM_GLOBAL(u8, 0x00953819)" in source
    assert "#define LAB_006869b0" not in source  # a goto label, not a value


REPAIRABLE = """#include "cmre.h"
extern void CM_CDECL cm_fn_00944c94(u8 *p);
extern u32 CM_CDECL cm_fn_00944c9f(u32 a, u32 b);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 a);
u32 CM_CDECL cm_fn_006869a0(u32 x, s8 *name)
{
  void *p;
  u32 unused;
  u32 written;
  written = 1;
  cm_fn_00944c94(x + 1);
  x = cm_fn_00944c9f(name, x);
  p = cm_fn_00944c9f(x, 2);
  p = cm_fn_009452ca(x);
  return x + (p != 0);
}
"""


def test_repair_makes_type_level_errors_compile_without_changing_expressions():
    source, notes = drafts.repair(0x006869A0, REPAIRABLE)
    assert drafts._diagnostics(source) == [], source
    assert "cm_fn_00944c94((u8 *)(x + 1));" in source
    assert "x = cm_fn_00944c9f((u32)(name), x);" in source
    assert "p = (void *)(cm_fn_00944c9f(x, 2));" in source
    assert "p = (void *)(u32)(cm_fn_009452ca(x));" in source  # low dword, as a 32-bit caller reads EAX
    assert "u32 unused;" not in source
    assert "u32 written __attribute__((unused));" in source
    assert len(notes) == 6 and all(n.startswith("line ") for n in notes)


def test_repair_leaves_compiling_source_unchanged():
    clean = "#include \"cmre.h\"\nu32 CM_CDECL cm_fn_006869a0(u32 x)\n{\n  return x + 1;\n}\n"
    assert drafts.repair(0x006869A0, clean) == (clean, [])


def test_repair_never_casts_between_different_widths():
    narrow = REPAIRABLE.replace("cm_fn_00944c94(x + 1);", "cm_fn_00944c94((u16)x);")
    source, _ = drafts.repair(0x006869A0, narrow)
    assert "cm_fn_00944c94((u16)x);" in source


@pytest.mark.skipif(not (drafts.DRAFTS / "fn_005b9440.c").exists(), reason="local Ghidra drafts not exported")
def test_forwarded_entry_ecx_becomes_the_missing_thiscall_receiver():
    # 005b9440: push esi; mov esi, ecx; call 0x5b9460 -- straight line from entry, ECX untouched.
    source, notes = drafts.repair(0x005B9440, drafts.convert(0x005B9440)[0])
    assert "cm_fn_005b9460((u8 *)(param_1));" in source  # then cast to the accepted prototype
    assert any("forwarded entry ECX" in n for n in notes)
    assert drafts._diagnostics(source) == [], drafts._diagnostics(source)


@pytest.mark.skipif(not (drafts.DRAFTS / "fn_005b9440.c").exists(), reason="local Ghidra drafts not exported")
def test_receiver_without_an_ecx_parameter_is_only_annotated():
    draft = drafts.convert(0x005B9440)[0].replace("CM_THISCALL cm_fn_005b9440", "CM_CDECL cm_fn_005b9440")
    source, _ = drafts.repair(0x005B9440, draft)
    assert "NEEDS-REVIEW: receiver missing for CM_THISCALL cm_fn_005b9460" in source
    assert "ECX before call at 0x005b9443: unchanged since entry" in source
    assert "cm_fn_005b9460();" in source


@pytest.mark.parametrize("proto,expected", [
    ("extern char * cm_fn_0090d130(void);", "extern char * cm_fn_0090d130(...);"),
    ("extern void CM_STDCALL cm_fn_0090d130(void);", "extern void CM_STDCALL cm_fn_0090d130(u32, u32, u32);")])
def test_void_callee_called_with_arguments_gets_a_compilable_prototype(tmp_path, monkeypatch, proto, expected):
    # GCC 15 defaults to C23, where `f()` means no parameters rather than unspecified ones.
    text = ("u32 __cdecl FUN_006869a0(int param_1)\n\n{\n"
            "  FUN_0090d130(param_1,(char *)param_1,FUN_00785d40(1,2));\n  return 0;\n}\n")
    monkeypatch.setattr(drafts, "prototype", lambda va: proto if va == 0x0090D130
                        else "extern u32 cm_fn_00785d40(u32 a, u32 b);")
    source, _ = drafts.convert(0x006869A0, text)
    assert expected in source and "pushed for the next call" in source
    path = tmp_path / "fn_006869a0.c"
    path.write_text(source)
    cc = [env.tool("gcc"), "-m32", "-c", str(path), "-o", str(tmp_path / "x.o"), "-Wall", "-Wextra",
          "-Werror", "-Wno-int-conversion", "-ffreestanding", f"-I{env.ROOT / 'recovery/include'}"]
    result = env.run_tool(cc)
    assert "too many arguments" not in result.stderr and "expected 0" not in result.stderr
