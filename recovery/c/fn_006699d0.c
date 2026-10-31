#include "cmre.h"

u32 CM_CDECL cm_fn_006699d0(u32 key)
{
    u32 address;
    for (address = 0x00b63d60u; address < 0x00b64738u; address += 0x48u) {
        u32 pointer = *(volatile u32 *)address;
        if (pointer != 0 && *(volatile u32 *)(pointer + 0x71u) == key &&
            *(volatile u8 *)(pointer + 0x11cu) != 0)
            return 1;
    }
    return 0;
}
