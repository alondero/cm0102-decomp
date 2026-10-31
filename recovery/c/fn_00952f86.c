#include "cmre.h"

/* Preferred-image VA 0x00952f86, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The original implements the arithmetic with integer words. In particular,
   ADD 0xffff updates the full exponent slot, and the product's carry words
   share the original scratch layout with its loop counters. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00951f87(u32, u32, u32);
extern void CM_CDECL cm_fn_00952006(u32);
extern void CM_CDECL cm_fn_00952034(u32);

u32 CM_CDECL cm_fn_00952f86(u32 left, u32 right)
{
    u8 scratch[0x24];
    u32 local = (u32)scratch, result, sign, a_exp, b_exp;
    volatile u32 exponent, remaining;
    u32 value, lhs, rhs, product, out, carry, shifts;
    u16 a_word, b_word, word;
    b_word = W(right + 10);
    D(local + 0x10) = 0;
    D(local) = 0;
    D(local + 4) = 0;
    D(local + 8) = 0;
    a_word = W(left + 10);
    sign = (b_word ^ a_word) & 0x8000u;
    a_exp = a_word & 0x7fffu;
    b_exp = b_word & 0x7fffu;
    result = a_exp;
    exponent = a_exp + b_exp;
    if (a_exp >= 0x7fff || b_exp >= 0x7fff || (u16)exponent > 0xbffd)
        goto overflow;
    if ((u16)exponent <= 0x3fbf) goto zero;
    if (a_exp == 0) {
        ++exponent;
        value = D(left + 8);
        if ((value & 0x7fffffffu) == 0) {
            value = D(left + 4);
            if (value == 0) {
                value = D(left);
                if (value == 0) {
                    W(left + 10) = 0;
                    return 0;
                }
            }
        }
    }
    result = 0;
    if (b_exp == 0) {
        ++exponent;
        value = D(right + 8);
        if ((value & 0x7fffffffu) == 0) {
            value = D(right + 4);
            if (value == 0) {
                value = D(right);
                if (value == 0) goto zero;
            }
        }
    }
    D(local + 0x14) = result;
    D(local + 0x20) = local + 4;
    remaining = 5;
    do {
        value = D(local + 0x14);
        value += value;
        if ((s32)remaining > 0) {
            value += left;
            D(local + 0x1c) = value;
            value = remaining;
            D(local + 0x18) = right + 8;
            D(local + 0x0c) = value;
            do {
                lhs = D(local + 0x1c);
                rhs = D(local + 0x18);
                a_word = W(lhs);
                b_word = W(rhs);
                product = (u32)a_word * b_word;
                out = D(local + 0x20) - 4u;
                value = D(out);
                carry = cm_fn_00951f87(value, product, out);
                if (carry != 0) {
                    out = D(local + 0x20);
                    word = W(out);
                    W(out) = (u16)(word + 1u);
                }
                value = D(local + 0x1c);
                D(local + 0x1c) = value + 2u;
                value = D(local + 0x18);
                D(local + 0x18) = value - 2u;
                value = D(local + 0x0c) - 1u;
                D(local + 0x0c) = value;
            } while (value != 0);
        }
        value = D(local + 0x20);
        D(local + 0x20) = value + 2u;
        value = D(local + 0x14);
        D(local + 0x14) = value + 1u;
        --remaining;
    } while ((s32)remaining > 0);
    exponent += 0xc002u;
    if ((s16)(u16)exponent > 0) {
        while ((B(local + 0x0b) & 0x80) == 0) {
            cm_fn_00952006(local);
            exponent += 0xffffu;
            if ((s16)(u16)exponent <= 0) break;
        }
        if ((s16)(u16)exponent > 0) goto round;
    }
    exponent += 0xffffu;
    if ((s16)(u16)exponent < 0) {
        shifts = 0u - (u32)(s32)(s16)(u16)exponent;
        exponent += shifts;
        do {
            if ((B(local) & 1) != 0) {
                value = D(local + 0x10);
                D(local + 0x10) = value + 1u;
            }
            cm_fn_00952034(local);
            --shifts;
        } while (shifts != 0);
        if (D(local + 0x10) != 0) {
            value = B(local);
            B(local) = (u8)(value | 1u);
        }
    }
round:
    word = W(local);
    if (word > 0x8000) goto increment;
    value = D(local) & 0x1ffffu;
    if (value != 0x18000) goto finish;
increment:
    if (D(local + 2) == 0xffffffffu) {
        value = D(local + 2);
        D(local + 2) = value & 0u;
        if (D(local + 6) == 0xffffffffu) {
            value = D(local + 6);
            D(local + 6) = value & 0u;
            if (W(local + 10) == 0xffff) {
                ++exponent;
                W(local + 10) = 0x8000;
            } else {
                word = W(local + 10);
                W(local + 10) = (u16)(word + 1u);
            }
        } else {
            value = D(local + 6);
            D(local + 6) = value + 1u;
        }
    } else {
        value = D(local + 2);
        D(local + 2) = value + 1u;
    }
finish:
    result = exponent;
    if ((u16)result >= 0x7fff) goto overflow;
    word = W(local + 2);
    result |= sign;
    W(left) = word;
    value = D(local + 4);
    D(left + 2) = value;
    value = D(local + 8);
    D(left + 6) = value;
    W(left + 10) = (u16)result;
    return result;
zero:
    D(left + 8) = 0;
    D(left + 4) = 0;
    D(left) = 0;
    return 0;
overflow:
    value = D(left + 4);
    D(left + 4) = value & 0u;
    sign = (sign != 0 ? 0x80000000u : 0u) + 0x7fff8000u;
    value = D(left);
    D(left) = value & 0u;
    D(left + 8) = sign;
    return result;
}
