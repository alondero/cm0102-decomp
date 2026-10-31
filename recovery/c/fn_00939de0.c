#include "cmre.h"

u32 CM_CDECL cm_fn_00939de0(u32 self)
{
    u32 input = *(volatile u32 *)self;
    u32 output = *(volatile u32 *)(self + 0x20);
    u32 count = *(volatile u8 *)input;
    ++input;
    *(volatile u32 *)self = input;
    while (count) {
        u32 high = *(volatile u8 *)input;
        u32 low = *(volatile u8 *)(input + 1);
        input += 2;
        *(volatile u32 *)output = (u32)(s32)(s16)((high << 8) + low);
        output += 4;
        --count;
    }
    *(volatile u32 *)(self + 0x20) = output;
    *(volatile u32 *)self = input;
    return input;
}
