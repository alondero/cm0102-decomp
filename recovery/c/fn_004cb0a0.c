#include "cmre.h"
extern u32 CM_THISCALL cm_fn_00867240(u32, volatile u32);
extern void CM_THISCALL cm_fn_008674e0(void *, u32);

void CM_STDCALL cm_fn_004cb0a0(u32 a, u32 b, u32 c)
{
    u32 p, flags;
    s32 d, x, sum;
    s8 result, limit, old;
    long double t;
    if (!c || !b || !a) return;
    p = *(volatile u32 *)(a + 0x61);
    if (!p) return;
    d = *(volatile s16 *)(p + 0xb);
    d -= *(volatile s16 *)(b + 0x80);
    x = *(volatile s8 *)(a + 0x18);
    t = (long double)x * CM_GLOBAL(double, 0x968d90);
    t = (long double)d + t;
    t -= CM_GLOBAL(double, 0x968d88);
    if (t < CM_GLOBAL(double, 0x9689a0))
        t *= CM_GLOBAL(double, 0x968d80);
    else
        t *= CM_GLOBAL(double, 0x968d78);
    t += CM_GLOBAL(double, 0x967890);
    if (t < CM_GLOBAL(double, 0x967898))
        t = CM_GLOBAL(double, 0x967898);
    else if (t > CM_GLOBAL(double, 0x968950))
        t = CM_GLOBAL(double, 0x968950);
    x = *(volatile s8 *)(c + 0x3f);
    /* Pinned 3.9.68 SHA-256:
       0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
       Preferred-image VAs 004cb15b/15f/161: FILD(x+5), FXCH,
       FDIVP ST(1) compute (x+5)/t, not t/(x+5). */
    t = (long double)(x + 5) / t;
    /* Original preferred-image VA 0x004cb163: x87-to-i64 helper.
       The C cast supplies the computed value rather than calling a false
       no-argument C extern. Both semantic gates pass with the corrected
       division. This does NOT establish preservation of live x87 depth
       across the faultable 004cb150 field load: plain C permits spills.
       Exact depth/fault-order equivalence remains an evidence boundary. */
    result = (s8)(long long)t;
    if (result > 10) result = 10;
    flags = *(volatile u32 *)(c + 0x45);
    limit = (flags & 2) ? 50 : 100;
    if (flags & 1) limit = (s8)(limit - 50);
    if (limit >= -50 && (flags & 8)) limit = (s8)(limit - 50);
    if (limit >= -50 && (flags & 0x20)) limit = (s8)(limit - 50);
    if (limit >= -50 && (flags & 0x40)) limit = (s8)(limit - 50);
    if (limit >= -50 && (flags & 0x80)) limit = (s8)(limit - 50);
    if (limit >= -50 && (flags & 0x100)) limit = (s8)(limit - 50);
    old = *(volatile s8 *)(c + 0x44);
    sum = (s32)old + result;
    if (sum > 100) sum = 100;
    else if (sum < -100) sum = -100;
    *(volatile s8 *)(c + 0x44) = (s8)sum;
    old = *(volatile s8 *)(c + 0x44);
    if (old > limit) *(volatile s8 *)(c + 0x44) = limit;
    if (!*(volatile u8 *)(c + 0x49)) {
        if ((result > 5 && (s32)*(volatile s8 *)(c + 0x44) + result > 0) ||
            *(volatile s8 *)(c + 0x3d) >= 75 - (s32)result) {
            if (flags & 4) cm_fn_008674e0((void *)c, 4);
            if (*(volatile u8 *)(c + 0x45) & 0x10)
                cm_fn_008674e0((void *)c, 0x10);
        }
    }
    cm_fn_00867240(c, 0);
}
