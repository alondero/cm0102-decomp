#include "cmre.h"

u32 CM_THISCALL cm_fn_00456740(u32 self, u32 value, u32 unused)
{
    u16 high = *(volatile u16 *)(self + 2);
    u16 other = (u16)(value >> 16);
    (void)unused;
    if (other < high)
        return 1;
    if (other > high)
        return 0;
    return (s16)value < *(volatile s16 *)self;
}
