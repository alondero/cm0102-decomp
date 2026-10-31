#include "cmre.h"

u32 CM_CDECL cm_fn_00942290(u32 arg, u32 key)
{
    u32 object = *(volatile u32 *)arg;
    s32 count = *(volatile s16 *)(object + 8u);
    u32 entries, index;
    if (count <= 0)
        return 0;
    entries = *(volatile u32 *)(object + 0x10u);
    for (index = 0; index < (u32)count; ++index) {
        u32 item = *(volatile u32 *)(entries + index * 4u);
        if (*(volatile u32 *)(item + 4u) == key)
            return *(volatile u32 *)(entries + index * 4u);
    }
    return 0;
}
