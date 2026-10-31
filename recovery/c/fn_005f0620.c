#include "cmre.h"

s32 CM_CDECL cm_fn_005f0620(u32 left, u32 right)
{
    u32 a, b;
    s32 x, y;
    if (!left) return 1;
    if (!right) return -1;
    b = *(volatile u32 *)(right + 0x68);
    a = *(volatile u32 *)(left + 0x68);
    if (a < b) return 1;
    if (a > b) return -1;
    y = *(volatile s32 *)(right + 0x64);
    x = *(volatile s32 *)(left + 0x64);
    if (y < x) return 1;
    return x < y ? -1 : 0;
}
