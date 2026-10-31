#include "cmre.h"

u32 CM_THISCALL cm_fn_007e9180(u32 self, u32 filter)
{
    u32 count = *(volatile u32 *)(self + 0x13);
    u32 i = 0, offset = 0;
    if ((s32)count > 0) {
        do {
            u32 row = *(volatile u32 *)(self + 0xb) + offset;
            u32 index = *(volatile u32 *)row;
            if (index != 0xffffffff) {
                if (!filter) {
                    *(volatile u8 *)(row + 4) = 0;
                } else {
                    u32 base = CM_GLOBAL(volatile u32, 0x00ae23bc);
                    if (*(volatile u32 *)(base + index * 110 + 0x39) == filter)
                        *(volatile u8 *)(row + 4) = 0;
                }
            }
            count = *(volatile u32 *)(self + 0x13);
            ++i;
            offset += 14;
        } while ((s32)i < (s32)count);
    }
    return count;
}
