#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
u32 CM_CDECL cm_fn_005446d0(u32 argument)
{
    u32 first = D(0x009cf2e4);
    u32 value = D(argument);
    if (value == first || value == D(0x009cf45c) || value == D(0x009cf2dc) ||
        value == D(0x009cf510) || value == D(0x009cf3e4))
        return 1;
    return 0;
}

