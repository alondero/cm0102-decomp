#include "cmre.h"

u32 CM_STDCALL cm_fn_007e2930(u8 *data, u8 *out, u8 *other, u32 unused)
{
    (void)unused;
    if (out) {
        u8 a = *(volatile u8 *)(data + 10);
        u8 b = *(volatile u8 *)(data + 11);
        *(volatile u8 *)out = (u8)(((a >> 3) & 7) + (b & 7) + (a & 7) + 1);
    }
    if (other)
        *(volatile u8 *)other = 0xff;
    return (u32)*(volatile u16 *)(data + 8) + 2 >= 16;
}

