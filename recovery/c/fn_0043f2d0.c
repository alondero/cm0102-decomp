/* WIP: asymmetric execution budget, case 29. */
#include "cmre.h"

u32 CM_CDECL cm_fn_0043f2d0(u32 address, u32 count)
{
    u32 result = count - 1u;
    u32 remaining = count;
    while (remaining != 0) {
        u16 value = *(volatile u16 *)address;
        u16 swapped = (u16)((value >> 8) | (value << 8));
        result = (result & 0xffff0000u) | value;
        address += 2u;
        --remaining;
        *(volatile u16 *)(address - 2u) = swapped;
    }
    return result;
}
