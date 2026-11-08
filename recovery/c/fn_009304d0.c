#include "cmre.h"

extern u32 CM_THISCALL cm_fn_004142e0(u32 self, u32 first, u32 second);

u32 CM_THISCALL cm_fn_009304d0(u32 self)
{
    u32 high;
    u32 low;
    u32 target;

    high = *(volatile u32 *)(self + 0x27);
    low = *(volatile u32 *)(self + 0x23);
    target = CM_GLOBAL(volatile u32, 0x00ad9c60);
    return cm_fn_004142e0(target, low, high);
}
