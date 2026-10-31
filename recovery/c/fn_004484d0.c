#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00448490(volatile u8 *self, volatile u8 *source);

u32 CM_THISCALL cm_fn_004484d0(volatile u8 *self, volatile u8 *source)
{
    *self = *source;
    cm_fn_00448490(self + 1, source + 1);
    *(volatile u16 *)(self + 0x19u) = *(volatile u16 *)(source + 0x19u);
    return (u32)self;
}
