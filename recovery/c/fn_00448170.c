#include "cmre.h"

extern void CM_THISCALL cm_fn_00417c10(u32 *self, u32 *output, u16 *source);

u32 CM_THISCALL cm_fn_00448170(volatile u8 *self, volatile u8 *source)
{
    u32 temporary[2];
    *(volatile u32 *)self = *(volatile u32 *)source;
    cm_fn_00417c10((u32 *)(self + 4), temporary, (u16 *)(source + 4));
    cm_fn_00417c10((u32 *)(self + 0xc), temporary, (u16 *)(source + 0xc));
    return (u32)self;
}
