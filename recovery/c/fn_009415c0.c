#include "cmre.h"

u32 CM_CDECL cm_fn_009415c0(u32 self)
{
    u32 offset = *(volatile u32 *)(self + 4u);
    u32 base = *(volatile u32 *)self;
    u32 address = base + offset;
    u32 high, low;
    *(volatile u32 *)(self + 4u) = offset + 2u;
    high = *(volatile u8 *)address;
    low = *(volatile u8 *)(address + 1u);
    return (address & 0xffff0000u) | (high << 8) | low;
}
