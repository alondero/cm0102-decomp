#include "cmre.h"

extern void CM_THISCALL cm_fn_00417c10(u32 *self, u32 *local, u16 *value);

u32 *CM_THISCALL cm_fn_005c6e00(void *self, u32 arg2, u8 arg3, u32 arg4,
                                 u8 arg5, u8 arg6, u16 arg7, u16 arg8,
                                 u16 arg9, u32 *arg10, char *arg11)
{
    u16 index = 0;
    u32 candidate;
    u32 arg2_value;
    u8 arg3_value;
    u8 arg5_value;
    u32 arg4_value;
    u8 arg6_value;
    u16 arg8_value;
    u16 arg9_value;
    u16 arg7_value;
    u32 *arg11_words;
    volatile u8 local_area[8];
    volatile u8 *src;
    volatile u8 *dst;
    u32 length = 0;
    u32 i;

    for (;;) {
        candidate = (u32)self + (u32)index * 0x69u + 0xb1u;
        if (*(volatile u32 *)candidate == 0)
            break;
        index = (u16)(index + 1u);
        if ((s16)index >= 0x7f)
            return (u32 *)0;
    }

    if (candidate == 0)
        return (u32 *)0;

    arg3_value = *(volatile u8 *)&arg3;
    arg5_value = *(volatile u8 *)&arg5;
    arg2_value = *(volatile u32 *)&arg2;
    *(volatile u8 *)(candidate + 0x0c) = arg3_value;
    arg4_value = *(volatile u32 *)&arg4;
    *(volatile u8 *)(candidate + 0x0d) = arg5_value;
    *(volatile u32 *)candidate = arg2_value;
    arg6_value = *(volatile u8 *)&arg6;
    *(volatile u8 *)(candidate + 0x0e) = arg6_value;

    cm_fn_00417c10((u32 *)(candidate + 4), (u32 *)local_area,
                   (u16 *)arg4_value);

    arg8_value = *(volatile u16 *)&arg8;
    arg9_value = *(volatile u16 *)&arg9;
    arg7_value = *(volatile u16 *)&arg7;
    *(volatile u16 *)(candidate + 0x11) = arg8_value;
    *(volatile u16 *)(candidate + 0x13) = arg9_value;
    *(volatile u32 *)(candidate + 0x65) = 0;
    *(volatile u32 *)(candidate + 0x29) = 0;
    *(volatile u32 *)(candidate + 0x2d) = 0;
    arg11_words = (u32 *)arg11;
    *(volatile u16 *)(candidate + 0x0f) = arg7_value;

    src = (volatile u8 *)arg11_words;
    do {
        u8 ch = src[length];
        ++length;
        if (ch == 0)
            break;
    } while (length != 0);

    src = (volatile u8 *)arg11_words;
    dst = (volatile u8 *)(candidate + 0x31);
    for (i = 0; i < (length >> 2); ++i) {
        u32 word = *(volatile u32 *)(src + i * 4u);
        *(volatile u32 *)(dst + i * 4u) = word;
    }
    for (i = (length >> 2) * 4u; i < length; ++i)
        dst[i] = src[i];

    for (i = 0; i < 4; ++i) {
        u32 value = *(volatile u32 *)((volatile u8 *)arg10 + i * 4u);
        *(volatile u32 *)(candidate + 0x15 + i * 4u) = value;
        *(volatile u8 *)(candidate + 0x25 + i) = 1;
    }
    return (u32 *)candidate;
}
