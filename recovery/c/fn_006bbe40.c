/* Preferred-image VA 006bbe40, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole caller006b9f50 overwrites AL at006b9f55, then kills full EAX on both
   branches:006b9fa3 reloads it;0090cfc0(bound10) overwrites it at0090cfd5. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern s32 CM_CDECL cm_fn_0090cfc0(s32 bound);
extern u8 CM_THISCALL cm_fn_006cd490(u32 self, u8 flag);

void CM_THISCALL cm_fn_006bbe40(u32 self)
{
    s32 first, second;
    u8 state, other, x, y, flag;
    s16 angle;
    if (D(self + 0xf582u) != 0)
        return;
    if (D(self + 0xf57au) != 0)
        return;
    state = B(self + 0x8eaau);
    if (state == 0)
        return;
    if (B(self + 0x8eb2u) != 0) {
        B(self + 0x8eaau) = 0;
        B(self + 0x8ea9u) = 0;
        return;
    }
    if ((s8)state < 2) {
        if ((s8)state <= 0)
            goto check;
        first = cm_fn_0090cfc0((s32)(s8)state * (s32)(s8)state);
        second = cm_fn_0090cfc0(2);
        if (first < second)
            goto check;
    }
    state = B(self + 0x8eaau);
    other = B(self + 0x8ea9u);
    B(self + 0x8eaau) = (u8)(state - 1u);
    if (other != 0) {
        other = (u8)(other - 1u);
        B(self + 0x8ea9u) = other;
    }
    angle = (s16)W(self + 0x8ed8u);
    if (angle > 220 && angle < 320)
        B(self + 0x8ea7u) = (u8)(B(self + 0x8ea7u) - 1u);
    else if (angle > 40 && angle < 140)
        B(self + 0x8ea7u) = (u8)(B(self + 0x8ea7u) + 1u);
    else if (angle > 130 && angle < 230) {
        other = B(self + 0x8ea8u);
        other = (u8)(other - 1u);
        B(self + 0x8ea8u) = other;
    } else if (angle < 50 || angle > 310) {
        other = B(self + 0x8ea8u);
        other = (u8)(other + 1u);
        B(self + 0x8ea8u) = other;
    }
check:
    x = B(self + 0x8ea7u);
    if ((s8)x < 0)
        goto outside;
    y = B(self + 0x8ea8u);
    if ((s8)y < 0 || (s8)x >= 9 || (s8)y >= 12)
        goto outside;
    return;
outside:
    flag = B(self + 0x8eabu);
    cm_fn_006cd490(self, flag);
}
