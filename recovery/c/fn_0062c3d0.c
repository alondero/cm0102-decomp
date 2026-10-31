#include "cmre.h"

u32 CM_THISCALL cm_fn_0062c3d0(u32 self, u32 index)
{
    u32 count = CM_GLOBAL(volatile u32, 0x00ae2364);
    if ((s32)index >= (s32)count)
        return 0;
    {
        u32 base = *(volatile u32 *)self;
        return (u32)(s32)*(volatile s16 *)(base + index * 31u + 12u);
    }
}
