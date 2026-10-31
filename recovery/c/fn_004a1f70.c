#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00448490(u32 self, u32 source);

u32 CM_THISCALL cm_fn_004a1f70(volatile u8 *self, volatile u8 *source)
{
    self[0] = source[0];
    self[1] = source[1];
    cm_fn_00448490((u32)self + 2, (u32)source + 2);
    *(volatile u32 *)(self + 0x1a) = *(volatile u32 *)(source + 0x1a);
    *(volatile u16 *)(self + 0x1e) = *(volatile u16 *)(source + 0x1e);
    return (u32)self;
}
