#include "cmre.h"

u32 CM_CDECL cm_fn_00549ea0(u32 arg)
{
    u32 value = arg;
    u32 matched = 0;
    if (value != 0) {
        value = *(volatile u32 *)value;
        if (value == CM_GLOBAL(volatile u32, 0x009cf2e4) ||
            value == CM_GLOBAL(volatile u32, 0x009cf300) ||
            value == CM_GLOBAL(volatile u32, 0x009cf314) ||
            value == CM_GLOBAL(volatile u32, 0x009cf360) ||
            value == CM_GLOBAL(volatile u32, 0x009cf488) ||
            value == CM_GLOBAL(volatile u32, 0x009cf338))
            matched = 1;
    }
    return (value & 0xffffff00u) | matched;
}
