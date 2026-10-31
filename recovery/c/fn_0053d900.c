#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
u32 CM_CDECL cm_fn_0053d900(u32 argument)
{
    u32 first = D(0x009cf41c);
    u32 value = D(argument);
    if (value == first || value == D(0x009cf478) || value == D(0x009cf530) ||
        value == D(0x009cf2f8) || value == D(0x009cf500))
        return 1;
    return 0;
}
