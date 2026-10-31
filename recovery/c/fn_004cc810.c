#include "cmre.h"

extern void CM_THISCALL cm_fn_00417c10(void *self, u32 *local_out, u16 *source);

u8 * CM_THISCALL cm_fn_004cc810(u8 *self, const u8 *source)
{
    volatile u8 *destination = (volatile u8 *)self;
    const volatile u8 *input = (const volatile u8 *)source;
    u8 local_buffer[8];
    u32 count;
    u8 src_byte;
    u8 dst_byte;
    u8 value;

    *(volatile u32 *)(destination + 0x00) = *(const volatile u32 *)(input + 0x00);
    *(volatile u32 *)(destination + 0x04) = *(const volatile u32 *)(input + 0x04);
    *(volatile u32 *)(destination + 0x08) = *(const volatile u32 *)(input + 0x08);
    *(volatile u32 *)(destination + 0x0c) = *(const volatile u32 *)(input + 0x0c);

    count = 3;
    do {
        u32 copied = *(const volatile u32 *)(input + 0x10 + (3 - count) * 4);
        *(volatile u32 *)(destination + 0x10 + (3 - count) * 4) = copied;
        --count;
    } while (count != 0);

    count = 0;
    do {
        src_byte = input[0x1c + count];
        destination[0x1c + count] = src_byte;
        ++count;
    } while (count < 5);

    {
        u32 word = *(const volatile u32 *)(input + 0x21);
        *(volatile u32 *)(destination + 0x21) = word;
    }
    cm_fn_00417c10((void *)(destination + 0x25), (u32 *)local_buffer, (u16 *)(input + 0x25));
    cm_fn_00417c10((void *)(destination + 0x2d), (u32 *)local_buffer, (u16 *)(input + 0x2d));

    src_byte = input[0x35];
    dst_byte = destination[0x35];
    value = (u8)((((s8)(src_byte << 2) >> 2) ^ dst_byte) & 0x3f) ^ dst_byte;
    destination[0x35] = value;
    src_byte = input[0x35];
    value = (u8)((((s8)(src_byte << 1) >> 1) ^ value) & 0x40) ^ value;
    destination[0x35] = value;
    src_byte = input[0x35];
    value = (u8)((value ^ src_byte) & 0x7f) ^ src_byte;
    destination[0x35] = value;

    {
        u32 dword = *(const volatile u32 *)(input + 0x36);
        *(volatile u32 *)(destination + 0x36) = dword;
    }
    destination[0x3a] = input[0x3a];
    *(volatile u16 *)(destination + 0x3b) = *(const volatile u16 *)(input + 0x3b);
    destination[0x3d] = input[0x3d];
    destination[0x3e] = input[0x3e];
    destination[0x3f] = input[0x3f];
    {
        u32 dword = *(const volatile u32 *)(input + 0x40);
        *(volatile u32 *)(destination + 0x40) = dword;
    }
    destination[0x44] = input[0x44];
    *(volatile u32 *)(destination + 0x45) = *(const volatile u32 *)(input + 0x45);
    destination[0x49] = input[0x49];
    *(volatile u32 *)(destination + 0x4a) = *(const volatile u32 *)(input + 0x4a);
    destination[0x4e] = input[0x4e];

    dst_byte = destination[0x4f];
    src_byte = input[0x4f];
    value = (u8)((src_byte ^ dst_byte) & 0x0f) ^ dst_byte;
    destination[0x4f] = value;
    src_byte = input[0x4f];
    value = (u8)((value ^ src_byte) & 0x0f) ^ src_byte;
    destination[0x4f] = value;

    return self;
}
