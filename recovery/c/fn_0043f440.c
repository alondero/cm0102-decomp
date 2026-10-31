#include "cmre.h"

u32 CM_CDECL cm_fn_0043f440(void)
{
    u32 address = 0x00ad9ca8;
    u32 remaining = 0xffffffffu;
    do {
        u8 value = *(volatile u8 *)address;
        ++address;
        --remaining;
        if (value == 0)
            break;
    } while (remaining != 0);
    return ~remaining - 1u;
}
