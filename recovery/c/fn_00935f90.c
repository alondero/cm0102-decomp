/* Preferred-image VA 00935f90, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))

u32 CM_CDECL cm_fn_00935f90(u32 state, volatile u32 first_index,
                            u32 second_index, u32 arg4, u32 arg5)
{
    u32 first = first_index;
    u32 x, y, dx, dy, value, cross, a, b, va, vb;
    x = D(state + 0x12u);
    y = D(state + 0x16u);
    dx = (u32)(s32)(s16)W(x + second_index * 2u);
    value = (u32)(s32)(s16)W(x + first * 2u);
    dx -= value;
    dy = (u32)(s32)(s16)W(y + second_index * 2u);
    value = (u32)(s32)(s16)W(y + first * 2u);
    dy -= value;
    cross = dx * arg5 - dy * arg4;
    if ((s32)cross <= 0) return 0;

    a = D(state + 0x40u);
    b = D(state + 0x44u);
    va = D(a + second_index * 4u);
    vb = D(b + second_index * 4u);
    /* Native 00935fe0 reassigns the first-index argument slot. */
    first_index = vb;
    cross = vb * dx - va * dy;
    if ((s32)cross < 0) {
        value = first_index;
        value = (va * arg4 + value * arg5) & 0xffffc000u;
        if ((s32)value <= (s32)0xf07b0000u) return 1;
    }

    a = D(state + 0x48u);
    b = D(state + 0x4cu);
    va = D(a + second_index * 4u);
    vb = D(b + second_index * 4u);
    cross = vb * dx - va * dy;
    if ((s32)cross < 0) {
        value = (va * arg4 + vb * arg5) & 0xffffc000u;
        if ((s32)value <= (s32)0xf07b0000u) return 1;
    }
    return 0;
}
