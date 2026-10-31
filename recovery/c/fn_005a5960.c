#include "cmre.h"

u32 CM_THISCALL cm_fn_005a5960(u32 self, u32 entry)
{
    if (entry != 0) {
        u32 base = *(volatile u32 *)(self + 8u);
        u32 index = *(volatile u32 *)entry;
        *(volatile u8 *)(base + index) = 0;
    }
    return entry;
}
