"""C frame allocation and the effect of changing stack-probe build inputs."""
import pytest

from cmre import build, emu, image, lift, recover


@pytest.mark.parametrize("opt", recover.C_OPT_LEVELS)
def test_large_stack_frame_links_and_runs(tmp_path, opt):
    source = tmp_path / "fn_00401000.c"
    source.write_text('''#include "cmre.h"
u32 CM_CDECL cm_fn_00401000(void)
{
    volatile u8 buffer[5000];
    buffer[0] = 17;
    buffer[4999] = 25;
    return buffer[0] + buffer[4999];
}
''', encoding="utf-8")
    built = build.build(source, lift.symbol(0x00401000), tmp_path, opt)
    machine = emu.Machine(image.primary(), built.image)
    result = machine.run(built.entry, emu.Case({}, [], bytes(emu.ARENA_SIZE)), 1000)
    assert result.outcome == "return", result
    assert result.regs[emu.REG_NAMES.index("eax")] == 42
