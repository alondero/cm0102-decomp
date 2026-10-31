#include "cmre.h"

extern void CM_THISCALL cm_fn_00417c10(u32 *self, u32 *output, u16 *source);

u32 CM_THISCALL cm_fn_00448490(volatile u8 *self, volatile u8 *source)
{
    u32 temporary[2];
    *(volatile u32 *)(self + 0x00) = *(volatile u32 *)(source + 0x00);
    *(volatile u32 *)(self + 0x04) = *(volatile u32 *)(source + 0x04);
    *(volatile u32 *)(self + 0x08) = *(volatile u32 *)(source + 0x08);
    *(volatile u32 *)(self + 0x0c) = *(volatile u32 *)(source + 0x0c);
    cm_fn_00417c10((u32 *)(self + 0x10), temporary, (u16 *)(source + 0x10));
    return (u32)self;
}
