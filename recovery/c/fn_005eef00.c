/* Preferred-image VA 0x005eef00, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_CDECL cm_fn_005e42b0(u32 value);

u32 CM_THISCALL cm_fn_005eef00(u32 self)
{
    u32 result = D(self + 0x4c), value, left, right;
    u8 a, b;
    if (result) {
        result = cm_fn_005e42b0(result);
        D(self + 0x4c) = 0;
    }
    value = D(self + 0x5c);
    if (!value) return result;
    left = 0x00ae2920u;
    right = self + 0x80u;
    for (;;) {
        a = B(left);
        b = B(right);
        if (a != b) break;
        if (!a) goto equal;
        a = B(left + 1);
        b = B(right + 1);
        if (a != b) break;
        left += 2;
        right += 2;
        if (!a) goto equal;
    }
    result = cm_fn_005e42b0(value);
    D(self + 0x5c) = 0;
    return result;
equal:
    W(0x00ae291c) = (u16)(W(0x00ae291c) - 1u);
    D(self + 0x5c) = 0;
    return 0;
}
