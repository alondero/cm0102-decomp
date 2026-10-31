#include "cmre.h"
extern u32 CM_THISCALL cm_fn_00448490(volatile u8 *self, volatile u8 *source);
u32 CM_THISCALL cm_fn_00448500(volatile u8 *self, volatile u8 *source)
{
    self[0] = source[0];
    self[1] = source[1];
    cm_fn_00448490(self + 2, source + 2);
    *(volatile u16 *)(self + 0x1a) = *(volatile u16 *)(source + 0x1a);
    return (u32)self;
}
