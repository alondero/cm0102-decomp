#include "cmre.h"

u32 CM_CDECL cm_fn_006695c0(void)
{
    u32 pointer = CM_GLOBAL(volatile u32, 0x00ae2d08);
    u32 address;
    if (pointer != 0) {
        u32 value = *(volatile u32 *)(pointer + 0x71u);
        if (value != 0)
            return value;
    }
    for (address = 0x00b63d60u; address < 0x00b64738u; address += 0x48u) {
        pointer = *(volatile u32 *)address;
        if (pointer != 0 && (*(volatile u8 *)(pointer + 0x11cu) & 2u) != 0)
            return *(volatile u32 *)(pointer + 0x71u);
    }
    return 0;
}
