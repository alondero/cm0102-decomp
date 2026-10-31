#include "cmre.h"

u32 CM_THISCALL cm_fn_007a7ed0(u32 self, u32 key, u32 flag)
{
    u32 entry = *(volatile u32 *)(self + 4);
    s32 count = *(volatile s32 *)(self + 8);
    u32 index = 0;
    u32 sought;
    if (count <= 0)
        return 0;
    sought = *(volatile u32 *)key;
    while (*(volatile u32 *)entry != sought) {
        ++index;
        entry += 0x1a;
        if ((s32)index >= count)
            return 0;
    }
    if (flag != 0)
        return *(volatile u8 *)(entry + 0x18) == 8 ? entry : 0;
    return *(volatile u8 *)(entry + 0x19) != 0 ? entry : 0;
}
