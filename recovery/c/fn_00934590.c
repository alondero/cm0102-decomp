#include "cmre.h"

u32 CM_CDECL cm_fn_00934590(u32 unused, u32 object, u32 first, u32 dest, u32 last)
{
    u32 xs = *(volatile u32 *)(object + 0x10);
    u32 ys = *(volatile u32 *)(object + 0x1c);
    s32 x0 = *(volatile s16 *)(xs + first * 2);
    s32 x1 = *(volatile s16 *)(xs + last * 2);
    s32 xd = *(volatile s16 *)(xs + dest * 2);
    u32 y0 = *(volatile u32 *)(ys + first * 4);
    s32 dx = x1 - x0;
    u32 dy = *(volatile u32 *)(ys + last * 4) - y0;
    u32 numerator = (u32)(xd - x0) * dy + (u32)(dx >> 1);
    u32 result = (u32)((s32)numerator / dx) + y0;
    (void)unused;
    *(volatile u32 *)(ys + dest * 4) = result;
    return result;
}
