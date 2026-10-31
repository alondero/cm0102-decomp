#include "cmre.h"

u32 CM_CDECL cm_fn_00951d5b(u32 first, volatile u32 mask, u32 flag)
{
    u32 index = (u8)first;
    u8 entry = *(volatile u8 *)(0x00de4521u + index);
    if ((entry & (u8)flag) != 0)
        return 1;
    if (mask != 0) {
        u32 value = *(volatile u16 *)(0x00ad8fdau + index * 2u);
        if ((value & mask) != 0)
            return 1;
    }
    return 0;
}
