#include "cmre.h"

u32 CM_CDECL cm_fn_00939860(volatile u8 *self)
{
    u32 owner = *(volatile u32 *)(self + 8);
    s32 count = *(volatile s16 *)(self + 0x3e);
    u32 base = *(volatile u32 *)(owner + 0x18);
    u32 cursor = *(volatile u32 *)(self + 0x20);

    if (count >= 0) {
        do {
            u32 offset = *(volatile u32 *)(cursor - 4);
            cursor -= 4;
            *(volatile u8 *)(base + offset) ^= 1;
        } while (count-- != 0);
    }
    *(volatile u16 *)(self + 0x3e) = 0;
    *(volatile u32 *)(self + 0x20) = cursor;
    return cursor;
}
