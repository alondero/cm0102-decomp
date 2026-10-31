#include "cmre.h"

u32 CM_CDECL cm_fn_007c6de0(u32 left, u32 right)
{
    u32 a, b;
    s16 x, y;
    if (!left) return 1;
    if (!right) return 0xffffffff;
    a = *(volatile u32 *)(left + 4);
    b = *(volatile u32 *)(right + 4);
    if (a > b) return 1;
    if (a < b) return 0xffffffff;
    x = *(volatile s16 *)(left + 8);
    y = *(volatile s16 *)(right + 8);
    if (x > y) return 1;
    if (x != y) return 0xffffffff;
    a = *(volatile u32 *)left;
    b = *(volatile u32 *)right;
    return a - b;
}
