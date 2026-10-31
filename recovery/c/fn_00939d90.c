#include "cmre.h"

u32 CM_CDECL cm_fn_00939d90(u32 self)
{
    u16 raw = *(volatile u16 *)(self + 4);
    u32 output = *(volatile u32 *)(self + 0x20);
    u32 input = *(volatile u32 *)self;
    s32 count = (s16)(raw - 0xb8);
    if (count >= 0) {
        ++count;
        do {
            u32 high = *(volatile u8 *)input;
            u32 low = *(volatile u8 *)(input + 1);
            input += 2;
            *(volatile u32 *)output = (u32)(s32)(s16)((high << 8) + low);
            output += 4;
            --count;
        } while (count);
    }
    *(volatile u32 *)(self + 0x20) = output;
    *(volatile u32 *)self = input;
    return input;
}
