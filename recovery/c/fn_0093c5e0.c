#include "cmre.h"

extern void CM_CDECL cm_fn_009445d0(u32 object, u32 value);

void CM_CDECL cm_fn_0093c5e0(volatile u32 *self)
{
    if (self == 0)
        return;

    u32 object = *self;
    u32 value = *(volatile u32 *)((u8 *)self + 0x4cu);
    if (value == *(volatile u32 *)(object + 0x54u))
        *(volatile u32 *)(object + 0x84u) = 1u;
    else
        cm_fn_009445d0(object, value);

    value = *(volatile u32 *)((u8 *)self + 0x38u);
    if (value != (u32)self + 0x12u) {
        object = *self;
        cm_fn_009445d0(object, value);
    }

    object = *self;
    value = *(volatile u32 *)((u8 *)self + 0x60u);
    if (value == *(volatile u32 *)(object + 0x5cu))
        *(volatile u32 *)(object + 0x8cu) = 1u;
    else
        cm_fn_009445d0(object, value);

    value = *(volatile u32 *)((u8 *)self + 0x54u);
    object = *self;
    cm_fn_009445d0(object, value);

    object = *self;
    if ((u32)self == *(volatile u32 *)(object + 0x50u))
        *(volatile u32 *)(object + 0x80u) = 1u;
    else
        cm_fn_009445d0(object, (u32)self);
}
