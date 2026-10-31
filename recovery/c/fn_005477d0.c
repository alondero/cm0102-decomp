#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
u32 CM_CDECL cm_fn_005477d0(u32 first, u32 second)
{
    if (!first || !second) return 0;
    if (D(second + 0xa7) == first || D(second + 0xab) == first ||
        D(second + 0xaf) == first) return 1;
    return 0;
}

