/* Preferred-image VA 0x0094FBA8, image base 0x00400000; target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Cdecl (RET 0) with two stack arguments: [EBP+8] is the output record and
   [EBP+0xC] is the input record. The input is read as an 8-byte value whose
   low dword is at +0, high dword at +4 and the sign/11-bit field word at +6.
   The output is a 10-byte record: 64-bit mantissa (low dword at +0, high dword
   at +4) and a word at +8. The code converts the input's 11-bit field into a
   biased word by adding 0x3C00 (normal) or 0x3C01 (subnormal) and sets the
   explicit bit 0x80000000 for every value except subnormals. A zero input
   stores a zero word and does not apply the sign (positive zero). Subnormals
   are shifted left until bit 31 of the high dword is set, decrementing the
   word once per shift, and each shift stores the record. The original store
   order is kept with volatile accesses. ECX is pushed as a local slot and never
   read; no receiver. Field and value meanings remain unknown beyond these
   operations. */
#include "cmre.h"

void CM_CDECL cm_fn_0094fba8(u8 *out, const u8 *in)
{
    volatile u8 *src = (volatile u8 *)in;
    volatile u8 *dst = (volatile u8 *)out;
    u32 top;
    u32 sign;
    u32 expBits;
    u32 hi20;
    u32 lo32;
    u32 hidden;
    u32 exp;
    u32 hi;
    u32 lo;
    u32 carry;

    top = *(volatile u16 *)(src + 6);
    sign = top & 0x8000u;
    expBits = (top >> 4) & 0x7ffu;
    hi20 = *(volatile u32 *)(src + 4) & 0xfffffu;
    lo32 = *(volatile u32 *)(src + 0);

    if (expBits == 0) {
        if (hi20 == 0 && lo32 == 0) {
            *(volatile u32 *)(dst + 4) = 0;
            *(volatile u32 *)(dst + 0) = 0;
            *(volatile u16 *)(dst + 8) = 0;
            return;
        }
        exp = expBits + 0x3c01u;
        hidden = 0;
    } else if (expBits == 0x7ffu) {
        exp = 0x7fffu;
        hidden = 0x80000000u;
    } else {
        exp = expBits + 0x3c00u;
        hidden = 0x80000000u;
    }

    hi = (hi20 << 11) | (lo32 >> 21) | hidden;
    lo = lo32 << 11;
    *(volatile u32 *)(dst + 4) = hi;
    *(volatile u32 *)(dst + 0) = lo;
    while ((hi & 0x80000000u) == 0) {
        carry = lo >> 31;
        hi = (hi << 1) | carry;
        lo = lo << 1;
        *(volatile u32 *)(dst + 0) = lo;
        *(volatile u32 *)(dst + 4) = hi;
        exp += 0xffffu;
    }
    *(volatile u16 *)(dst + 8) = (u16)(sign | exp);
}
