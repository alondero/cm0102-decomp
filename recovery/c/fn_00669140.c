#include "cmre.h"

u32 CM_CDECL cm_fn_00669140(void)
{
    u32 address;
    u8 count = 0;
    CM_GLOBAL(volatile u8, 0x00b64738) = 0;
    for (address = 0x00b63d74u; address < 0x00b6474cu; address += 0x48u) {
        u32 pointer = *(volatile u32 *)(address - 0x14u);
        if (pointer == 0 || *(volatile u8 *)(pointer + 0x11cu) != 0) {
            *(volatile u8 *)address = count;
            ++count;
            CM_GLOBAL(volatile u8, 0x00b64738) = count;
        }
    }
    return 1;
}

