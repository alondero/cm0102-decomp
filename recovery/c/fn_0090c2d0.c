#include "cmre.h"

u32 CM_CDECL cm_fn_0090c2d0(u32 arg)
{
    u32 index = (u8)arg;
    if (index < 0x20u)
        return arg;
    return *(volatile u8 *)(0x00aa2155u + index * 3u);
}
