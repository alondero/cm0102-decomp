#include "cmre.h"

u32 CM_THISCALL cm_fn_008c6200(void *self, u32 index)
{
    u32 address = (u32)self;
    u32 count = *(volatile u32 *)(address + 0x10u);
    u32 base = *(volatile u32 *)(address + 0xcu);
    if ((s32)index < (s32)count)
        return base + index * 4u;
    return base;
}
