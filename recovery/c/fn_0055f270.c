#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_009452ca(u32 allocation);

u32 CM_CDECL cm_fn_0055f270(void)
{
    u32 value = CM_GLOBAL(volatile u32, 0x00ae2918);
    if (value != 0) {
        value = (u32)cm_fn_009452ca(value);
        CM_GLOBAL(volatile u32, 0x00ae2918) = 0;
    }
    return value;
}
