#include "cmre.h"

u32 CM_CDECL cm_fn_00540980(u32 address, u32 flags)
{
    u8 value = *(volatile u8 *)(address + 0x5eu);
    u8 mask = ((u8)flags & 3u) ? 0x0cu : 3u;
    value &= mask;
    *(volatile u8 *)(address + 0x5eu) = value;
    value |= (u8)flags;
    *(volatile u8 *)(address + 0x5eu) = value;
    return address;
}
