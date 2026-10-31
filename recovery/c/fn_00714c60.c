#include "cmre.h"

u8 CM_THISCALL cm_fn_00714c60(u32 self, u32 value)
{
    s8 count = *(volatile s8 *)(self + 0x101u);
    s32 index;
    for (index = 0; index < count; ++index) {
        if (*(volatile u32 *)(self + 0x1aeu + (u32)index * 4u) == value)
            return 1;
    }
    return 0;
}
