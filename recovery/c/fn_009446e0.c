#include "cmre.h"

/* cdecl signed multiply of two stack arguments (a at [esp+4] on entry, b next),
   returning the 32-bit product in EAX. Each operand is split into magnitude
   and sign: the magnitude is a wrapping negation when negative, and the result
   sign is negative exactly when the operand signs differ. The magnitudes are
   multiplied in 16-bit halves (the original's imul/shr/add sequence), so
   16.16-style rounding and wrap-around follow the original. The original's
   `or ebx, 0xffffffff` on a negative first operand reads the saved EBX but
   produces -1 regardless, so no input value leaks into the result. */
s32 CM_CDECL cm_fn_009446e0(s32 a, s32 b)
{
    u32 ua;
    u32 ub;
    u32 al;
    u32 ah;
    u32 bl;
    u32 bh;
    u32 sum;
    u32 sign;

    if (a < 0) {
        ua = 0u - (u32)a;
    } else {
        ua = (u32)a;
    }
    if (b < 0) {
        ub = 0u - (u32)b;
    } else {
        ub = (u32)b;
    }

    /* 0xffffffff when exactly one operand is negative, otherwise 1. */
    if ((a < 0) != (b < 0)) {
        sign = 0xffffffffu;
    } else {
        sign = 1u;
    }

    al = ua & 0xffffu;
    ah = ua >> 16;
    bl = ub & 0xffffu;
    bh = ub >> 16;

    sum = ((al * bl) >> 16);
    sum = sum + bl * ah;
    sum = sum + al * bh;
    sum = sum + ((ah * bh) << 16);

    return (s32)(sum * sign);
}
