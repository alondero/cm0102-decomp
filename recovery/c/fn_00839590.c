#include "cmre.h"

u32 CM_CDECL cm_fn_00839590(u32 arg)
{
    u16 value = CM_GLOBAL(volatile u16, 0x00ae3184);
    if (arg == 0)
        value = CM_GLOBAL(volatile u16, 0x00ae31a8);
    return (arg & 0xffff0000u) | value;
}
