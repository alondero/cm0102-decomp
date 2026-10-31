#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00448490(volatile u8 *self, volatile u8 *source);

u32 CM_THISCALL cm_fn_00448530(volatile u8 *self, volatile u8 *source)
{
    *(volatile u16 *)self = *(volatile u16 *)source;
    cm_fn_00448490(self + 2, source + 2);
    return (u32)self;
}
