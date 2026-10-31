#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00448170(volatile u8 *self, volatile u8 *source);

u32 CM_THISCALL cm_fn_004ab5d0(volatile u8 *self, volatile u8 *source)
{
    cm_fn_00448170(self, source);
    *(volatile u32 *)(self + 0x14u) = *(volatile u32 *)(source + 0x14u);
    return (u32)self;
}
