#include "cmre.h"

u32 CM_CDECL cm_fn_00943d30(u32 arg)
{
    u32 pointer = *(volatile u32 *)(arg + 12u);
    if (pointer != 0)
        return *(volatile u16 *)(pointer + 0x16u);
    return 0x800u;
}
