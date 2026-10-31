#include "cmre.h"

u32 CM_THISCALL cm_fn_004a7f20(u32 self, u32 first, u32 unused)
{
    u16 high = *(volatile u16 *)(self + 2u);
    (void)unused;
    if ((u16)(first >> 16) != high)
        return 0;
    return (u16)first == *(volatile u16 *)self;
}
