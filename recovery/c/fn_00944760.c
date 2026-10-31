#include "cmre.h"

/* cdecl, two stack arguments (a at entry [esp+4], b at entry [esp+8]), no memory
   access, 32-bit result in EAX. Both operands are taken as magnitudes. The sign
   factor is -1 when exactly one operand is negative, otherwise +1. Each magnitude
   is split into its low and high 16-bit halves; the product is formed from the
   halves with the listing's 32-bit wrapping arithmetic (shifts, multiplies and
   adds), then shifted left 10 and added to the low-half product shifted right 6
   (logical). The final value is multiplied by the sign factor. */
u32 CM_CDECL cm_fn_00944760(s32 a, s32 b)
{
    u32 abs_a;
    u32 abs_b;
    u32 lo_a;
    u32 hi_a;
    u32 lo_b;
    u32 hi_b;
    u32 sign;
    u32 acc;

    abs_a = (a < 0) ? (0u - (u32)a) : (u32)a;
    abs_b = (b < 0) ? (0u - (u32)b) : (u32)b;

    sign = 1u;
    if ((a < 0) != (b < 0)) {
        sign = 0xffffffffu;
    }

    lo_a = abs_a & 0xffffu;
    hi_a = (u32)((s32)abs_a >> 16) & 0xffffu;
    lo_b = abs_b & 0xffffu;
    hi_b = (u32)((s32)abs_b >> 16) & 0xffffu;

    acc = ((hi_a << 16) + lo_a) * hi_b;
    acc = acc + lo_b * hi_a;
    acc = acc << 10;
    acc = acc + ((lo_a * lo_b) >> 6);
    acc = acc * sign;

    return acc;
}
