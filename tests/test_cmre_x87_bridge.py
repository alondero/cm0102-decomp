"""ADR 0010: x87 code generation in C builds and the _CIpow operand bridge."""
import capstone
import pytest

from cmre import build, lift, recover


def _instructions(built):
    text = built.image.text
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    return list(md.disasm(built.image.read(text.va, text.end - text.va), text.va))


@pytest.mark.parametrize("opt", recover.C_OPT_LEVELS)
def test_float_to_integer_conversion_avoids_fisttp(tmp_path, opt):
    # Unicorn stores 0x7fffffffffffffff for an out-of-range FISTTP where the CPU
    # (and the original __ftol's FISTP) stores the indefinite 0x8000000000000000:
    # fn_007164b0 case 62 compared 0xffffffff against 0 for C / 0.0.
    source = tmp_path / "fn_00401000.c"
    source.write_text('''#include "cmre.h"
u32 CM_CDECL cm_fn_00401000(u32 bits)
{
    long double value = (long double)CM_GLOBAL(volatile double, 0x00968950) / (long double)(s32)bits;
    return (u32)(long long)value;
}
''', encoding="utf-8")
    built = build.build(source, lift.symbol(0x00401000), tmp_path, opt)
    mnemonics = {i.mnemonic for i in _instructions(built)}
    assert "fisttp" not in mnemonics
    assert "fistp" in mnemonics


@pytest.mark.parametrize("opt", recover.C_OPT_LEVELS)
def test_cipow_bridge_loads_both_operands_and_calls_the_original_helper(tmp_path, opt):
    source = tmp_path / "fn_00401000.c"
    source.write_text('''#include "cmre.h"
#include "cmre_x87.h"
u32 CM_CDECL cm_fn_00401000(u32 bits)
{
    long double base = *(volatile float *)(bits + 0x1a2u);
    long double power = cm_x87_CIpow(base, CM_GLOBAL(volatile double, 0x00968950));
    return (u32)(long long)power;
}
''', encoding="utf-8")
    built = build.build(source, lift.symbol(0x00401000), tmp_path, opt)
    code = [(i.mnemonic, i.op_str) for i in _instructions(built)]
    bridge = ("fld", "xword ptr [esp + 4]"), ("fld", "xword ptr [esp + 0x10]"), ("call", "0x945ff0"), ("ret", "")
    starts = [n for n in range(len(code) - 3) if tuple(code[n:n + 4]) == bridge]
    assert len(starts) == 1, code
