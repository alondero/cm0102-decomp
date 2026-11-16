#include "cmre.h"

extern u32 CM_THISCALL cm_fn_006cb9b0(u32 self, u32 object, u32 x, u32 y);
extern u8 CM_THISCALL cm_fn_006cbc00(u32 self, u32 x0, u32 y0, u32 x1, u32 y1);
extern unsigned long long CM_CDECL cm_fn_0090cfc0(u32 entry_eax, u32 entry_edx, u32 bound);

/* Original preferred-image VAs below refer to CM0102 3.9.68, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   006fa039/006fa094 lower x87 ST0 conversions as long-long casts, not
   ordinary no-argument C calls. Emitted precision/fault order still needs
   inspection. 006f9fd2/006fa128/006fa131 change AL only: preserve the
   other EAX bits explicitly, including when wrappers forward all of EAX. */
u32 CM_THISCALL cm_fn_006f9f90(u32 self, u32 x, u32 y, u32 mode,
                                 float a, float b, u8 *out)
{
    u8 ox = *(volatile u8 *)(self + 0x102);
    u8 oy = *(volatile u8 *)(self + 0x103);
    s32 index = (s8)((u8)x - ox) * 24 + (s8)((u8)y - oy);
    s16 target = *(volatile s16 *)(0xb67c30u + (u32)(index * 2));
    s32 delta;
    s32 limit;
    if (*(volatile s16 *)(self + 0x19a) == target)
        return (y & 0xffffff00u) | 1u;
    if ((u8)mode) {
        u32 receiver = *(volatile u32 *)(self + 0x19e);
        u32 r = cm_fn_006cb9b0(receiver, self, x, y);
        s32 skill = *(volatile s8 *)(self + 0x10b);
        long double t;
        s32 first, second, dx, dy;
        if ((u8)r)
            t = (long double)(skill * 4) + (long double)a * CM_GLOBAL(float, 0x968a78) + CM_GLOBAL(float, 0x96d7b0);
        else
            t = (long double)(skill * 5) + (long double)a * CM_GLOBAL(float, 0x968a78) + CM_GLOBAL(float, 0x96d664);
        first = (s32)(long long)t;
        oy = *(volatile u8 *)(self + 0x103);
        dy = (s8)oy - (s8)y;
        if (dy < 0) dy = -dy;
        ox = *(volatile u8 *)(self + 0x102);
        dx = (s8)ox - (s8)x;
        if (dx < 0) dx = -dx;
        index = (s8)dy * 9 + (s8)dx;
        t = ((long double)*(volatile float *)(0xa15e44u + (u32)(index * 4)) - (long double)b) * CM_GLOBAL(float, 0x96afb0) - CM_GLOBAL(float, 0x969560);
        second = (s32)(long long)t;
        limit = (s32)((u32)first - (u32)second);
        if ((s16)limit < 30) limit = 30;
        else if ((s16)limit > 135) limit = 135;
    } else {
        limit = 75;
    }
    delta = *(volatile s16 *)(self + 0x19a) - (s32)target;
    if (delta < 0) delta = -delta;
    if ((s16)delta > 180) delta = 360 - delta;
    if ((s16)delta >= (s16)limit) return (u32)delta & 0xffffff00u;
    if (out) {
        u32 eax = (y & 0xffffff00u) | *(volatile u8 *)(self + 0x102);
        u32 receiver = *(volatile u32 *)(self + 0x19e);
        u8 result = cm_fn_006cbc00(receiver, eax, oy, x, y);
        s32 skill, v;
        *out = result;
        skill = *(volatile s8 *)(self + 0x10b);
        v = (s8)result;
        u32 random = (u32)cm_fn_0090cfc0((u32)v, (u32)(v * v), (u32)(v * v));
        if (skill < (s32)random) return random & 0xffffff00u;
        return (random & 0xffffff00u) | 1u;
    }
    return ((u32)delta & 0xffffff00u) | 1u;
}
