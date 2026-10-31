#include "cmre.h"
extern u32 CM_THISCALL cm_fn_00448490(volatile u8 *self, volatile u8 *source);
u32 CM_THISCALL cm_fn_00448230(volatile u8 *self, volatile u8 *source)
{
    *(volatile u32 *)self = *(volatile u32 *)source;
    cm_fn_00448490(self + 4, source + 4);
    self[0x1c] = source[0x1c];
    *(volatile u16 *)(self + 0x1d) = *(volatile u16 *)(source + 0x1d);
    return (u32)self;
}
