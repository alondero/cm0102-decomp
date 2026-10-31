#include "cmre.h"

extern void CM_THISCALL cm_fn_00417c10(void *self, u32 *local_out, u16 *source);

u8 * CM_THISCALL cm_fn_0071df00(u8 *self, const u8 *source)
{
    volatile u8 *destination = (volatile u8 *)self;
    const volatile u8 *input = (const volatile u8 *)source;
    u8 local_buffer[8];
    u32 copied;
    u32 offset;

    *(volatile u32 *)(destination + 0x00) = *(const volatile u32 *)(input + 0x00);
    *(volatile u32 *)(destination + 0x04) = *(const volatile u32 *)(input + 0x04);
    *(volatile u32 *)(destination + 0x08) = *(const volatile u32 *)(input + 0x08);
    *(volatile u32 *)(destination + 0x0c) = *(const volatile u32 *)(input + 0x0c);
    copied = *(const volatile u32 *)(input + 0x10);
    *(volatile u32 *)(destination + 0x10) = copied;

    cm_fn_00417c10((void *)(destination + 0x14), (u32 *)local_buffer,
                   (u16 *)(input + 0x14));

    for (offset = 0x1c; offset <= 0x35; ++offset) {
        u8 byte = input[offset];
        destination[offset] = byte;
    }

    *(volatile u16 *)(destination + 0x36) = *(const volatile u16 *)(input + 0x36);
    *(volatile u16 *)(destination + 0x38) = *(const volatile u16 *)(input + 0x38);
    *(volatile u16 *)(destination + 0x3a) = *(const volatile u16 *)(input + 0x3a);
    *(volatile u16 *)(destination + 0x3c) = *(const volatile u16 *)(input + 0x3c);

    for (offset = 0x3e; offset <= 0x48; ++offset) {
        u8 byte = input[offset];
        destination[offset] = byte;
    }

    return self;
}
