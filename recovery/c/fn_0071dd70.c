#include "cmre.h"

extern u8 * CM_THISCALL cm_fn_0071df00(u8 *self, const u8 *source);

u8 * CM_THISCALL cm_fn_0071dd70(u8 *self, const u8 *source)
{
    volatile u8 *destination = (volatile u8 *)self;
    const volatile u8 *input = (const volatile u8 *)source;
    u32 i;
    u32 row;

    for (i = 0; i <= 0x10; ++i)
        destination[i] = input[i];
    *(volatile u16 *)(destination + 0x11) = *(const volatile u16 *)(input + 0x11);
    *(volatile u16 *)(destination + 0x13) = *(const volatile u16 *)(input + 0x13);
    *(volatile u16 *)(destination + 0x15) = *(const volatile u16 *)(input + 0x15);
    *(volatile u16 *)(destination + 0x17) = *(const volatile u16 *)(input + 0x17);
    *(volatile u16 *)(destination + 0x19) = *(const volatile u16 *)(input + 0x19);
    *(volatile u16 *)(destination + 0x1b) = *(const volatile u16 *)(input + 0x1b);
    *(volatile u16 *)(destination + 0x1d) = *(const volatile u16 *)(input + 0x1d);

    for (i = 0x1f; i < 0x24; ++i)
        destination[i] = input[i];
    for (i = 0x24; i < 0x29; ++i)
        destination[i] = input[i];
    for (i = 0x29; i < 0x2e; ++i)
        destination[i] = input[i];
    for (i = 0x2e; i < 0x33; ++i)
        destination[i] = input[i];
    destination[0x33] = input[0x33];
    destination[0x34] = input[0x34];

    for (i = 0; i < 0x0b; ++i)
        *(volatile u16 *)(destination + 0x35 + i * 2) =
            *(const volatile u16 *)(input + 0x35 + i * 2);
    for (i = 0; i < 0x0b; ++i)
        *(volatile u16 *)(destination + 0x4b + i * 2) =
            *(const volatile u16 *)(input + 0x4b + i * 2);

    *(volatile u32 *)(destination + 0x61) = *(const volatile u32 *)(input + 0x61);
    *(volatile u32 *)(destination + 0x65) = *(const volatile u32 *)(input + 0x65);
    *(volatile u16 *)(destination + 0x69) = *(const volatile u16 *)(input + 0x69);
    *(volatile u16 *)(destination + 0x6b) = *(const volatile u16 *)(input + 0x6b);
    *(volatile u16 *)(destination + 0x6d) = *(const volatile u16 *)(input + 0x6d);
    *(volatile u16 *)(destination + 0x6f) = *(const volatile u16 *)(input + 0x6f);

    for (row = 0; row < 0x14; ++row)
        (void)cm_fn_0071df00((u8 *)(destination + 0x71 + row * 0x49),
                             (const u8 *)(input + 0x71 + row * 0x49));
    return self;
}
