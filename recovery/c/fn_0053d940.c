#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
u32 CM_CDECL cm_fn_0053d940(u32 argument)
{
    u32 first = D(0x009cf4d0);
    u32 value = D(argument);
    if (value == first || value == D(0x009cf52c) || value == D(0x009cf4b4) ||
        value == D(0x009cf2a4) || value == D(0x009cf514))
        return 1;
    return 0;
}

