#include "cmre.h"

u8 CM_CDECL cm_fn_0040cba0(u32 object, u32 values, s8 count)
{
    u32 key = 0xffffffffu;
    s32 index;
    if (object != 0) {
        u32 nested = *(volatile u32 *)(object + 0x53u);
        if (nested != 0) key = *(volatile u32 *)nested;
    }
    for (index = 0; index < count; ++index)
        if (*(volatile u32 *)(values + (u32)index * 4u) == key) return (u8)index;
    return 0xffu;
}
