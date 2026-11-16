#include "cmre.h"

extern u8 CM_THISCALL cm_fn_00714d30(volatile u8 *self);

/* The comparison below is provisional: C does not guarantee the x87
   FLD/FCOMP/FNSTSW status and fault behavior at 006cbaa5-006cbaba.
   Pinned image SHA-256: 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
u8 CM_STDCALL cm_fn_006cb9b0(volatile u8 *self, s32 arg2, s32 arg3)
{
    u8 mode = cm_fn_00714d30(self);
    s8 a = (s8)arg2, b = (s8)arg3;
    s8 c, d;
    int x, y;
    float value;

    if (a < 2 || a > 6) return 0;
    if (!mode) {
        if (b < 10 || b > 11) return 0;
    } else {
        if (b < 0 || b > 1) return 0;
    }
    c = (s8)self[0x102];
    if (c == 4) return 0;
    if (!mode) {
        if (c < 6 && c > 2 && b < (s8)self[0x103]) return 0;
        d = (s8)self[0x103];
        if (d < 9) return 0;
    } else {
        if (c < 6 && c > 2 && b > (s8)self[0x103]) return 0;
        d = (s8)self[0x103];
        if (d > 2) return 0;
    }
    if (a <= 2 || a >= 6) return 0;
    x = (int)c - (int)a;
    if (x < 0) x = -x;
    if (x <= 1) return 0;
    y = (int)b - (int)d;
    if (y < 0) y = -y;
    x = (s8)x;
    y = (s8)y;
    if (x < 0 || x > 8 || y < 0 || y > 11)
        value = CM_GLOBAL(volatile float, 0x0096d2dc);
    else
        value = *(volatile float *)(0x00a15e44u + (u32)(y * 9 + x) * 4u);
    return value > CM_GLOBAL(volatile double, 0x00968940);
}
