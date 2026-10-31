#include "cmre.h"

u32 CM_CDECL cm_fn_00540780(u32 left, u32 right)
{
    u32 a = *(volatile u32 *)(left + 4);
    u32 b, x, y;
    u16 l, r;
    if (!a) {
        b = *(volatile u32 *)(right + 4);
        return b != 0;
    }
    b = *(volatile u32 *)(right + 4);
    if (!b) return 0xffffffff;
    x = *(volatile u32 *)a;
    y = *(volatile u32 *)b;
    x -= y;
    if (x) return x;
    l = *(volatile u16 *)(left + 8);
    if (l == 0xffff) return 0;
    r = *(volatile u16 *)(right + 8);
    if (r == 0xffff) return 0;
    return (u32)((s32)(s16)r - (s32)(s16)l);
}
