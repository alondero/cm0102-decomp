/* Preferred-image VA 0x00951fa8, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_00951f87(u32 left, u32 right, u32 output);

u32 CM_CDECL cm_fn_00951fa8(u32 left, u32 right)
{
    u32 a, b, carry;
    b = D(right);
    a = D(left);
    carry = cm_fn_00951f87(a, b, left);
    if (carry) {
        a = D(left + 4u);
        if (cm_fn_00951f87(a, 1, left + 4u))
            D(left + 8u) = D(left + 8u) + 1u;
    }
    b = D(right + 4u);
    a = D(left + 4u);
    if (cm_fn_00951f87(a, b, left + 4u))
        D(left + 8u) = D(left + 8u) + 1u;
    b = D(right + 8u);
    a = D(left + 8u);
    return cm_fn_00951f87(a, b, left + 8u);
}
