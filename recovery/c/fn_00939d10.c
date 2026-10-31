#include "cmre.h"

u32 CM_CDECL cm_fn_00939d10(u32 self)
{
    u16 raw = *(volatile u16 *)(self + 4);
    u32 output = *(volatile u32 *)(self + 0x20);
    u32 input = *(volatile u32 *)self;
    s32 count = (s16)(raw - 0xb0);
    if (count >= 0) {
        ++count;
        do {
            u32 byte = *(volatile u8 *)input;
            output += 4;
            ++input;
            *(volatile u32 *)(output - 4) = byte;
            --count;
        } while (count != 0);
    }
    *(volatile u32 *)(self + 0x20) = output;
    *(volatile u32 *)self = input;
    return output;
}
