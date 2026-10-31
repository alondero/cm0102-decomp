#include "cmre.h"

u32 CM_CDECL cm_fn_00941740(u32 address, u32 offset)
{
    u32 base = *(volatile u32 *)(address + 12u);
    *(volatile u32 *)(address + 4u) = base + offset;
    return address;
}
