#include "cmre.h"

u32 CM_CDECL cm_fn_007ae240(u32 left, u32 right)
{
    u16 a, b;
    u32 x, y;
    if (!left) return 0xffffffff;
    if (!right) return 1;
    a = *(volatile u16 *)(left + 6);
    b = *(volatile u16 *)(right + 6);
    if (a > b) return 1;
    if (a < b) return 0xffffffff;
    a = *(volatile u16 *)(left + 4);
    b = *(volatile u16 *)(right + 4);
    if (a != b)
        return (u32)((s32)(s16)a - (s32)(s16)b);
    x = *(volatile u32 *)left;
    y = *(volatile u32 *)right;
    return x - y;
}
