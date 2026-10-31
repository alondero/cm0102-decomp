#include "cmre.h"

void CM_THISCALL cm_fn_00905c60(u32 self, u32 key, u32 first, u32 second)
{
    u32 cursor;
    s32 index = 0;
    *(volatile u8 *)second = 1;
    *(volatile u8 *)first = 1;
    if (*(volatile s8 *)(self + 0x438u) <= 0) return;
    cursor = self + 0xcu;
    do {
        if (*(volatile u32 *)(cursor - 8u) == key) {
            *(volatile u8 *)first = *(volatile u8 *)cursor;
            *(volatile u8 *)second = *(volatile u8 *)(cursor + 1u);
        }
        ++index;
        cursor += 0x12u;
    } while (index < *(volatile s8 *)(self + 0x438u));
}
