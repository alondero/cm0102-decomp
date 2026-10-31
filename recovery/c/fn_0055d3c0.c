#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007e88d0(u32 self, u32 record, u32 third, u32 fourth);

u32 CM_THISCALL cm_fn_0055d3c0(volatile u8 *self, volatile u32 *record,
                               u32 position, u32 third, u32 fourth)
{
    u32 index, base;
    s32 slot;
    if (record && (s32)*record >= CM_GLOBAL(volatile s32, 0x00ae2364))
        return 0;
    index = *record;
    base = *(volatile u32 *)(self + 0x14);
    slot = *(volatile s8 *)(base + index * 29u + (u32)((s32)(s8)position * 5));
    if (slot == -1)
        return 0;
    base = *(volatile u32 *)self;
    return cm_fn_007e88d0(*(volatile u32 *)(base + (u32)slot * 4u),
                          (u32)record, third, fourth);
}
