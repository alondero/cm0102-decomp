#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00448490(u32 self, u32 source);

u32 CM_THISCALL cm_fn_004a2050(volatile u8 *self, volatile u8 *source)
{
    *(volatile u32 *)self = *(volatile u32 *)source;
    cm_fn_00448490((u32)self + 4, (u32)source + 4);
    self[0x1c] = source[0x1c];
    *(volatile u32 *)(self + 0x1d) = *(volatile u32 *)(source + 0x1d);
    *(volatile u16 *)(self + 0x21) = *(volatile u16 *)(source + 0x21);
    return (u32)self;
}
