#include "cmre.h"

extern u8 *CM_THISCALL cm_fn_0071df00(u8 *self, const u8 *source);

u8 *CM_THISCALL cm_fn_008a5010(u8 *self, const u8 *source)
{
    volatile u8 *d = (volatile u8 *)self;
    const volatile u8 *s = (const volatile u8 *)source;
    u32 i, block;

    *(volatile u32 *)(d + 0x00) = *(const volatile u32 *)(s + 0x00);
    *(volatile u32 *)(d + 0x04) = *(const volatile u32 *)(s + 0x04);
    *(volatile u16 *)(d + 0x08) = *(const volatile u16 *)(s + 0x08);
    *(volatile u16 *)(d + 0x0a) = *(const volatile u16 *)(s + 0x0a);
    for (i = 0; i < 5; ++i)
        *(volatile u16 *)(d + 0x0c + i * 2) = *(const volatile u16 *)(s + 0x0c + i * 2);
    for (i = 0; i < 5; ++i)
        *(volatile u16 *)(d + 0x16 + i * 2) = *(const volatile u16 *)(s + 0x16 + i * 2);
    for (i = 0; i < 5; ++i)
        *(volatile u16 *)(d + 0x20 + i * 2) = *(const volatile u16 *)(s + 0x20 + i * 2);
    for (i = 0; i < 5; ++i)
        *(volatile u16 *)(d + 0x2a + i * 2) = *(const volatile u16 *)(s + 0x2a + i * 2);
    *(volatile u32 *)(d + 0x34) = *(const volatile u32 *)(s + 0x34);
    *(volatile u32 *)(d + 0x38) = *(const volatile u32 *)(s + 0x38);
    *(volatile u32 *)(d + 0x3c) = *(const volatile u32 *)(s + 0x3c);
    for (i = 0; i < 3; ++i)
        *(volatile u32 *)(d + 0x40 + i * 4) = *(const volatile u32 *)(s + 0x40 + i * 4);
    *(volatile u8 *)(d + 0x4c) = *(const volatile u8 *)(s + 0x4c);
    *(volatile u16 *)(d + 0x4d) = *(const volatile u16 *)(s + 0x4d);
    *(volatile u16 *)(d + 0x4f) = *(const volatile u16 *)(s + 0x4f);
    *(volatile u16 *)(d + 0x51) = *(const volatile u16 *)(s + 0x51);
    *(volatile u32 *)(d + 0x53) = *(const volatile u32 *)(s + 0x53);
    for (i = 0; i < 0x32; ++i)
        *(volatile u32 *)(d + 0x57 + i * 4) = *(const volatile u32 *)(s + 0x57 + i * 4);

    for (block = 0; block < 2; ++block) {
        u32 b = block * 0x625;
        d[0x11f + b] = s[0x11f + b];
        d[0x120 + b] = s[0x120 + b];
        for (i = 0; i < 15; ++i)
            d[0x121 + b + i] = s[0x121 + b + i];
        for (i = 0; i < 7; ++i)
            *(volatile u16 *)(d + 0x130 + b + i * 2) = *(const volatile u16 *)(s + 0x130 + b + i * 2);
        for (i = 0; i < 4; ++i)
            for (u32 j = 0; j < 5; ++j)
                d[0x13e + b + i * 5 + j] = s[0x13e + b + i * 5 + j];
        d[0x152 + b] = s[0x152 + b];
        d[0x153 + b] = s[0x153 + b];
        for (i = 0; i < 11; ++i)
            *(volatile u16 *)(d + 0x154 + b + i * 2) = *(const volatile u16 *)(s + 0x154 + b + i * 2);
        for (i = 0; i < 11; ++i)
            *(volatile u16 *)(d + 0x16a + b + i * 2) = *(const volatile u16 *)(s + 0x16a + b + i * 2);
        *(volatile u32 *)(d + 0x180 + b) = *(const volatile u32 *)(s + 0x180 + b);
        *(volatile u32 *)(d + 0x184 + b) = *(const volatile u32 *)(s + 0x184 + b);
        *(volatile u16 *)(d + 0x188 + b) = *(const volatile u16 *)(s + 0x188 + b);
        *(volatile u16 *)(d + 0x18a + b) = *(const volatile u16 *)(s + 0x18a + b);
        *(volatile u16 *)(d + 0x18c + b) = *(const volatile u16 *)(s + 0x18c + b);
        *(volatile u16 *)(d + 0x18e + b) = *(const volatile u16 *)(s + 0x18e + b);
        for (i = 0; i < 0x14; ++i) {
            u8 *row_d = (u8 *)(d + 0x190 + b + i * 0x49);
            const u8 *row_s = (const u8 *)(s + 0x190 + b + i * 0x49);
            (void)cm_fn_0071df00(row_d, row_s);
        }
    }

    *(volatile u32 *)(d + 0xd69) = *(const volatile u32 *)(s + 0xd69);
    *(volatile u32 *)(d + 0xd6d) = *(const volatile u32 *)(s + 0xd6d);
    *(volatile u32 *)(d + 0xd71) = *(const volatile u32 *)(s + 0xd71);
    d[0xd75] = s[0xd75];
    *(volatile u32 *)(d + 0xd76) = *(const volatile u32 *)(s + 0xd76);
    return self;
}

