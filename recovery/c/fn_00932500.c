/* Preferred-image VA 0x00932500, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl (ret 0), two stack arguments, A = [esp+4], B = [esp+8].
   Same comparison shape as VA 0x00768200 with different field offsets:
   the signed divisor byte is at +0x0e and the signed word is at +0x13.
   - A.divisor < 3: return 1 if B.divisor >= 3, else 0.
   - Otherwise, B.divisor < 3: return -1.
   - Otherwise compare a = A.word / A.divisor with b = B.word / B.divisor in
     x87 extended precision: -1 if a > b, 1 if a < b, 0 if equal. */
#include "cmre.h"

s32 CM_CDECL cm_fn_00932500(void *lhs, void *rhs)
{
    u8 *a = (u8 *)lhs;
    u8 *b = (u8 *)rhs;
    s8 a_divisor;
    s8 b_divisor;
    long double a_value;
    long double b_value;

    a_divisor = *(s8 *)(a + 0x0e);
    b_divisor = *(s8 *)(b + 0x0e);
    if (a_divisor < 3) {
        return (b_divisor >= 3) ? 1 : 0;
    }
    if (b_divisor < 3) {
        return -1;
    }
    a_value = (long double)*(s16 *)(a + 0x13) / (long double)a_divisor;
    b_value = (long double)*(s16 *)(b + 0x13) / (long double)b_divisor;
    if (a_value > b_value) {
        return -1;
    }
    if (a_value < b_value) {
        return 1;
    }
    return 0;
}
