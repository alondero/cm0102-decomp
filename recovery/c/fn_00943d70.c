#include "cmre.h"

static void scale_record(volatile u8 *out, volatile u8 *input, u32 source_offset)
{
    volatile u32 count = 0;
    volatile u32 src = *(volatile u32 *)(input + source_offset);
    *(volatile u16 *)(out + 4) = *(volatile u16 *)(src + 8);
    src = *(volatile u32 *)(input + source_offset);
    *(volatile u16 *)(out + 6) = *(volatile u16 *)(src + 0xa);
    src = *(volatile u32 *)(input + source_offset);
    *(volatile u16 *)(out + 8) = *(volatile u16 *)(src + 0xc);
    src = *(volatile u32 *)(input + source_offset);
    *(volatile u16 *)(out + 0xa) = *(volatile u16 *)(src + 0xe);
    src = *(volatile u32 *)(input + source_offset);
    *(volatile u32 *)(out + 0xc) = (u32)(s32)*(volatile s16 *)(src + 0x18);
    src = *(volatile u32 *)(input + source_offset);
    *(volatile u32 *)(out + 0x10) = (u32)(s32)*(volatile s16 *)(src + 0x16);
    for (;;) {
        s32 a = *(volatile s32 *)(out + 0xc);
        if (a >= 0x10000 || a <= -0x10000) break;
        s32 b = *(volatile s32 *)(out + 0x10);
        if (b >= 0x10000 || b <= -0x10000) break;
        count = count + 1;
        *(volatile s32 *)(out + 0xc) = a * 2;
        *(volatile s32 *)(out + 0x10) = b * 2;
        if (count >= 0x10) break;
    }
}

void CM_CDECL cm_fn_00943d70(volatile u8 *input, volatile u8 *one,
    volatile u8 *two)
{
    *(volatile u32 *)two = 0;
    *(volatile u32 *)one = 0;
    *(volatile u16 *)(one + 0x14) = 0;
    *(volatile u16 *)(one + 0x16) = 0;
    *(volatile u16 *)(two + 0x14) = 0;
    *(volatile u16 *)(two + 0x16) = 0;
    *(volatile u16 *)(one + 0x14) = *(volatile u16 *)(input + 0x34);
    *(volatile u16 *)(one + 0x16) = *(volatile u16 *)(input + 0x36);

    volatile u32 source = *(volatile u32 *)(input + 0x18);
    if (source != 0) {
        *(volatile u32 *)one = 1;
        scale_record(one, input, 0x18);
    }

    source = *(volatile u32 *)(input + 0x1c);
    if (source != 0) {
        *(volatile u32 *)two = 1;
        scale_record(two, input, 0x1c);
    }
}
