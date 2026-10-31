#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00448490(volatile u8 *self, volatile u8 *source);

u32 CM_THISCALL cm_fn_004485b0(volatile u8 *self, volatile u8 *source)
{
    cm_fn_00448490(self, source);
    *(volatile u16 *)(self + 0x18u) = *(volatile u16 *)(source + 0x18u);
    return (u32)self;
}
