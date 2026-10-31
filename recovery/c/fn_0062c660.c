#include "cmre.h"

u32 CM_THISCALL cm_fn_0062c660(u32 self, u32 entry, u32 value)
{
    u32 index = *(volatile u32 *)entry;
    u32 base = *(volatile u32 *)self;
    *(volatile u8 *)(base + index * 31u + 0x1eu) = (u8)value;
    return base;
}
