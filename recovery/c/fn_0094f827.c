#include "cmre.h"

u32 CM_CDECL cm_fn_0094f827(u32 destination)
{
    *(volatile u32 *)destination = 0;
    *(volatile u32 *)(destination + 4u) = 0;
    *(volatile u32 *)(destination + 8u) = 0;
    return 0;
}
