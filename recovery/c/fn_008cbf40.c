#include "cmre.h"

extern void CM_THISCALL cm_fn_00417c10(u32 *self, u32 *output, u16 *source);

u32 CM_THISCALL cm_fn_008cbf40(volatile u8 *self, volatile u8 *source)
{
    u32 temporary[2];
    cm_fn_00417c10((u32 *)self, temporary, (u16 *)source);
    *(volatile u32 *)(self + 8u) = *(volatile u32 *)(source + 8u);
    return (u32)self;
}
