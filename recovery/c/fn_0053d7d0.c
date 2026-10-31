#include "cmre.h"

u32 CM_CDECL cm_fn_0053d7d0(u32 arg)
{
    u32 pointer = *(volatile u32 *)(arg + 0x53u);
    if (pointer != 0 && *(volatile u8 *)(pointer + 0x7fu) == 2)
        return 1;
    return 0;
}
