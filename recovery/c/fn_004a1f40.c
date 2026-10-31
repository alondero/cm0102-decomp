#include "cmre.h"
extern u32 CM_THISCALL cm_fn_00448490(volatile u8 *self, volatile u8 *source);
u32 CM_THISCALL cm_fn_004a1f40(volatile u8 *self, volatile u8 *source)
{
    self[0] = source[0];
    cm_fn_00448490(self + 1, source + 1);
    *(volatile u32 *)(self + 0x19) = *(volatile u32 *)(source + 0x19);
    *(volatile u16 *)(self + 0x1d) = *(volatile u16 *)(source + 0x1d);
    return (u32)self;
}
