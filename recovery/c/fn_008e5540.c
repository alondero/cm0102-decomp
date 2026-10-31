#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00456740(u32 self, u32 value, u32 unused);
extern u32 CM_THISCALL cm_fn_00456770(u32 self, u32 value, u32 unused);

u32 CM_CDECL cm_fn_008e5540(u32 left, u32 right)
{
    u32 r = *(volatile u32 *)right;
    u32 high = *(volatile u32 *)(r + 0x21);
    u32 l = *(volatile u32 *)left;
    u32 low = *(volatile u32 *)(r + 0x1d);
    if (cm_fn_00456770(l + 0x1d, low, high) != 0)
        return 0xffffffffu;
    r = *(volatile u32 *)right;
    high = *(volatile u32 *)(r + 0x21);
    l = *(volatile u32 *)left;
    low = *(volatile u32 *)(r + 0x1d);
    return cm_fn_00456740(l + 0x1d, low, high) != 0;
}
