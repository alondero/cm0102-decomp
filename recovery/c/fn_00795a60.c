#include "cmre.h"

u32 CM_THISCALL cm_fn_00795a60(u32 self, u32 entry)
{
    u32 count = CM_GLOBAL(volatile u32, 0x00ae2364);
    u32 index = *(volatile u32 *)entry;
    u32 result = count - index - 1u;
    if ((s32)result >= 0 && (s32)result < 16)
        *(volatile u8 *)(self + result * 255u + 0x80u) = 0;
    return result;
}
