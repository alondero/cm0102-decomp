#include "cmre.h"

extern u32 CM_CDECL cm_fn_009466d0(u32 destination, u32 source, u32 count);

u32 CM_THISCALL cm_fn_00795a10(u32 self, u32 entry, volatile u32 source)
{
    u32 count = CM_GLOBAL(volatile u32, 0x00ae2364);
    u32 index = *(volatile u32 *)entry;
    u32 result = count - index - 1u;
    u32 base;
    if ((s32)result < 0 || (s32)result >= 16)
        return result;
    base = self + result * 255u;
    result = cm_fn_009466d0(base + 0x80u, source, 0xffu);
    *(volatile u8 *)(base + 0x17eu) = 0;
    return result;
}
