#include "cmre.h"

u32 CM_CDECL cm_fn_00941580(u32 object)
{
    u32 offset = *(volatile u32 *)(object + 4);
    u32 base = *(volatile u32 *)object;
    u32 address = base + offset;
    u32 result;
    *(volatile u32 *)(object + 4) = offset + 4;
    result = *(volatile u8 *)address;
    result = (result << 8) | *(volatile u8 *)(address + 1);
    result = (result << 8) | *(volatile u8 *)(address + 2);
    result = (result << 8) | *(volatile u8 *)(address + 3);
    return result;
}
