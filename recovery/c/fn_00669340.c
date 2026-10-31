#include "cmre.h"

u8 CM_CDECL cm_fn_00669340(u32 value)
{
    u32 index;
    for (index = 0; index < 35u; ++index) {
        if (*(volatile u32 *)(0x00b63d60u + index * 72u) == value)
            return (u8)index;
    }
    return 4;
}
