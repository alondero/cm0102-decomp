#include "cmre.h"

u32 CM_CDECL cm_fn_0094f80c(u32 destination, u32 source)
{
    u32 delta = destination - source;
    u32 count = 3;
    do {
        u32 value = *(volatile u32 *)source;
        *(volatile u32 *)(delta + source) = value;
        source += 4;
        --count;
    } while (count != 0);
    return source;
}
