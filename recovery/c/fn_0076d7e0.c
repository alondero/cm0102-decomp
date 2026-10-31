#include "cmre.h"

u32 CM_THISCALL cm_fn_0076d7e0(void *self, u32 *entry, u32 selector)
{
    u32 index = *(volatile u32 *)entry;
    u32 offset = index * 110u + (u32)(s32)(s8)selector * 20u;
    u32 base = *(volatile u32 *)((u32)self + 4u);
    *(volatile u8 *)(base + offset + 0x45u) = 0;
    return base;
}
