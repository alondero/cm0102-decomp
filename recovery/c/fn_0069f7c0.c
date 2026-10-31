#include "cmre.h"

u32 CM_THISCALL cm_fn_0069f7c0(u32 *self, u32 *index, u32 value)
{
    u32 base = *(volatile u32 *)self;
    u32 n = *(volatile u32 *)index;
    *(volatile u16 *)(base + n * 73 + 6) = (u16)value;
    n = *(volatile u32 *)index;
    base = *(volatile u32 *)self;
    *(volatile u16 *)(base + n * 73 + 8) = (u16)value;
    n = *(volatile u32 *)index;
    base = *(volatile u32 *)self;
    *(volatile u16 *)(base + n * 73 + 12) = (u16)value;
    n = *(volatile u32 *)index;
    base = *(volatile u32 *)self;
    *(volatile u16 *)(base + n * 73 + 10) = (u16)value;
    return n * 73;
}
