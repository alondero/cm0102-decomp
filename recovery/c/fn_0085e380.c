#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00456740(u32 self, u32 value, u32 unused);
extern u32 CM_THISCALL cm_fn_00456770(u32 self, u32 value, u32 unused);

u32 CM_CDECL cm_fn_0085e380(u32 left, u32 right)
{
    u32 high = *(volatile u32 *)(right + 8);
    u32 low = *(volatile u32 *)(right + 4);
    u32 l, r;
    if (cm_fn_00456770(left + 4, low, high) != 0)
        return 0xffffffffu;
    high = *(volatile u32 *)(right + 8);
    low = *(volatile u32 *)(right + 4);
    if (cm_fn_00456740(left + 4, low, high) != 0)
        return 1;
    l = *(volatile u32 *)left;
    r = *(volatile u32 *)right;
    return l - r;
}
