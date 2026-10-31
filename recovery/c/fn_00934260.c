#include "cmre.h"

u32 CM_CDECL cm_fn_00934260(u32 first, u32 self, u32 mode, u32 i, u32 j, u32 k)
{
    u32 base, p, q, a, b;
    s32 diff;
    (void)first;
    if ((u16)mode)
        base = *(volatile u32 *)(self + 0x18);
    else
        base = *(volatile u32 *)(self + 0x1c);
    p = base + (u32)(s32)(s16)i * 4;
    a = *(volatile u32 *)p;
    q = base + (u32)(s32)(s16)j * 4;
    b = *(volatile u32 *)q;
    if ((s32)b <= (s32)a)
        return b;
    diff = (s16)((u16)b - *(volatile u16 *)p);
    b -= (u32)diff;
    *(volatile u32 *)q = b;
    q = base + (u32)(s32)(s16)k * 4;
    b = *(volatile u32 *)q;
    *(volatile u32 *)q = b - (u32)diff;
    return q;
}

