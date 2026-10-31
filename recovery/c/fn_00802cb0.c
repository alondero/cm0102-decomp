#include "cmre.h"

u32 CM_THISCALL cm_fn_00802cb0(u32 self, u32 selector, u32 destination)
{
    u8 byte = (u8)selector;
    *(volatile u8 *)destination = byte;
    if (byte != 0xff) {
        s32 row = *(volatile s16 *)(self + 0x382a);
        u32 index = (u32)(row * 10 + (s32)(s8)byte);
        *(volatile u32 *)(self + index * 4u + 0x26250a) = 1;
        return index;
    }
    return destination;
}
