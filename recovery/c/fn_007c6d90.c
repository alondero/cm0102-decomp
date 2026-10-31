#include "cmre.h"

s32 CM_CDECL cm_fn_007c6d90(u32 left, u32 right)
{
    u32 a, b;
    if (!left) return 1;
    if (!right) return -1;
    a = *(volatile u32 *)(left + 10);
    b = *(volatile u32 *)(right + 10);
    if (a > b) return 1;
    if (a < b) return -1;
    a = *(volatile u32 *)(left + 4);
    b = *(volatile u32 *)(right + 4);
    if (a > b) return 1;
    return a < b ? -1 : 0;
}
