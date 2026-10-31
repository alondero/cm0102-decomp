#include "cmre.h"

u32 CM_CDECL cm_fn_00765480(u32 input)
{
    u32 index, value, limit;
    if (!input) {
        index = (u32)(s32)CM_GLOBAL(volatile s16, 0x00b77c7a);
        input = *(volatile u32 *)(0x00b74c26u + index * 768);
        if (!input) return 0;
    }
    value = *(volatile u32 *)input;
    limit = CM_GLOBAL(volatile u32, 0x00ae2364);
    index = value - limit + 16;
    if ((s32)index < 0 || (s32)index >= 16) return 0;
    return 0x00b73390u + index * 246;
}
