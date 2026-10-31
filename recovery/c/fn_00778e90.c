#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00456740(u32 self, u32 value, u32 unused);
extern u32 CM_THISCALL cm_fn_00456770(u32 self, u32 value, u32 unused);

u32 CM_CDECL cm_fn_00778e90(u32 left, u32 right)
{
    u32 high = *(volatile u32 *)(right + 4);
    u32 low = *(volatile u32 *)right;
    if (cm_fn_00456770(left, low, high) != 0)
        return 0xffffffffu;
    high = *(volatile u32 *)(right + 4);
    low = *(volatile u32 *)right;
    return cm_fn_00456740(left, low, high) != 0;
}
