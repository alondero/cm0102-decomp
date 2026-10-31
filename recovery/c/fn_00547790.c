#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
u32 CM_CDECL cm_fn_00547790(u32 first, u32 second)
{
    if (!first || !second) return 0;
    if (D(second + 0x9b) == first || D(second + 0x9f) == first ||
        D(second + 0xa3) == first) return 1;
    return 0;
}
