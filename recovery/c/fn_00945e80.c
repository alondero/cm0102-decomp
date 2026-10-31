/* Preferred-image VA 0x00945e80, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: CRT signed 64-bit division helper (__alldiv). Stdcall with four
   dword stack arguments (ret 0x10): dividend low, dividend high, divisor low,
   divisor high. Returns the signed 64-bit quotient in EDX:EAX.
   Algorithm, as the listing does it:
   - each negative operand is negated (64-bit) and counted; the result is
     negated again only when exactly one operand was negative;
   - if the divisor high dword is zero: quotient high = dividend high / divisor
     low (a 32-bit DIV, which traps on a zero divisor), then quotient low =
     (remainder:dividend low) / divisor low with the same 64/32 DIV;
   - otherwise both operands are shifted right together until the divisor high
     dword is zero, the 32-bit estimate q = (shifted dividend) / (shifted divisor
     low) is formed, then q is decremented once when q * divisor (full 64-bit
     product, with overflow) is greater than the dividend. The high quotient
     dword is zero on this path.
   The 64/32 DIV instructions are written as a restoring shift-subtract loop
   (`div64by32`) so only 32-bit operations are used; the division-by-zero trap
   comes from the 32-bit `/` on the first step, as in the listing. Field
   meanings are not applicable; this is arithmetic only. */
#include "cmre.h"

/* Quotient of (hi:lo) / d for hi < d, as one x86 32-bit DIV would give it. */
static u32 cm_div64by32(u32 hi, u32 lo, u32 d)
{
    u32 rem = hi;
    u32 q = 0;
    int i;

    for (i = 0; i < 32; i++) {
        u32 top = rem >> 31;

        rem = (rem << 1) | (lo >> 31);
        lo <<= 1;
        q <<= 1;
        if (top != 0 || rem >= d) {
            rem -= d;
            q |= 1;
        }
    }
    return q;
}

unsigned long long CM_STDCALL cm_fn_00945e80(u32 a_lo, u32 a_hi, u32 b_lo, u32 b_hi)
{
    u32 signs = 0;
    u32 q_lo;
    u32 q_hi;
    u32 sh_a_hi;
    u32 sh_a_lo;
    u32 sh_b_hi;
    u32 sh_b_lo;
    u32 est;
    unsigned long long t;
    unsigned long long prod_lo;
    unsigned long long prod_hi;
    u32 lo_part;
    u32 hi_part;
    u32 sum;
    int carry;

    if ((s32)a_hi < 0) {
        signs++;
        t = 0ull - (((unsigned long long)a_hi << 32) | a_lo);
        a_lo = (u32)t;
        a_hi = (u32)(t >> 32);
    }
    if ((s32)b_hi < 0) {
        signs++;
        t = 0ull - (((unsigned long long)b_hi << 32) | b_lo);
        b_lo = (u32)t;
        b_hi = (u32)(t >> 32);
    }

    if (b_hi == 0) {
        q_hi = a_hi / b_lo;
        q_lo = cm_div64by32(a_hi % b_lo, a_lo, b_lo);
    } else {
        sh_b_hi = b_hi;
        sh_b_lo = b_lo;
        sh_a_hi = a_hi;
        sh_a_lo = a_lo;
        do {
            sh_b_lo = (sh_b_lo >> 1) | (sh_b_hi << 31);
            sh_b_hi >>= 1;
            sh_a_lo = (sh_a_lo >> 1) | (sh_a_hi << 31);
            sh_a_hi >>= 1;
        } while (sh_b_hi != 0);

        est = cm_div64by32(sh_a_hi, sh_a_lo, sh_b_lo);

        prod_hi = (unsigned long long)est * b_hi;
        lo_part = (u32)prod_hi;
        prod_lo = (unsigned long long)b_lo * est;
        hi_part = (u32)(prod_lo >> 32);
        sum = hi_part + lo_part;
        carry = sum < hi_part;
        if (carry) {
            est--;
        } else if (sum > a_hi) {
            est--;
        } else if (sum == a_hi && (u32)prod_lo > a_lo) {
            est--;
        }
        q_hi = 0;
        q_lo = est;
    }

    t = ((unsigned long long)q_hi << 32) | q_lo;
    if (signs == 1) {
        t = 0ull - t;
    }
    return t;
}
