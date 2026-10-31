#include "cmre.h"

/* Signed fixed-point quotient: the result is sign * (((|a| / |b|) << 16) +
   (((|a| % |b|) shifted by 16 after halving both operands while the remainder
   exceeds 0xffff) / |b|)). Arguments are on the stack (cdecl); the caller pops
   them, so the routine itself uses ret with no stack bytes.

   The sign is positive when the operands have equal signs (a zero first
   operand counts as non-negative) and negative otherwise. The halving loop
   uses an arithmetic shift on the divisor, as the original does with sar.
   A zero divisor reaches the original's divide instruction and faults there. */

u32 CM_CDECL cm_fn_00944840(s32 a, s32 b)
{
    u32 sign;
    u32 mag_a;
    u32 mag_b;
    u32 hi;
    u32 rem;
    u32 lo;

    sign = 1u;
    mag_a = (u32)a;
    if (a < 0) {
        sign = 0xffffffffu;
        mag_a = 0u - (u32)a;
    }

    mag_b = (u32)b;
    if (b < 0) {
        sign = 0u - sign;
        mag_b = 0u - (u32)b;
    }

    hi = (mag_a / mag_b) << 16;
    rem = mag_a % mag_b;

    while (rem > 0xffffu) {
        rem = rem >> 1;
        mag_b = (u32)(((s32)mag_b) >> 1);
    }

    lo = (rem << 16) / mag_b;
    return (lo + hi) * sign;
}
