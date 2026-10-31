#include "cmre.h"

u32 CM_THISCALL cm_fn_0076c0f0(void *self, u32 *entry, u32 selector)
{
    u32 index = * (volatile u32 *)entry;
    u32 offset = index * 110u + (u32)(s32)(s8)selector * 20u;
    u32 base = *(volatile u32 *)((u32)self + 4u);
    *(volatile u8 *)(base + offset + 0x44u) = 1;
    return base;
}
