#include "cmre.h"

/*
 * Original register/stack boundary, preferred-image VAs, CM0102 3.9.68
 * SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86:
 * 006d5d68 loads argument 2 into full EAX; 006d5d6d loads argument 1
 * into EDX; ECX remains the receiver. 006d5d58..006d5d72 push all
 * seven stack arguments; 006d5d73 calls 006d6f70 and 006d5d78
 * forwards its full EAX. Model these live register inputs explicitly:
 * regparm(3) assigns EAX, EDX, ECX, and stdcall retains stack cleanup.
 * This is an original-code boundary, not callee composition.
 */
extern u32 __attribute__((stdcall, regparm(3))) cm_fn_006d6f70(
    u32 eax_in, u32 edx_in, u32 ecx_in,
    u32, u32, u32, u32, u32, u32, u32);

u32 CM_THISCALL cm_fn_006d5d00(u32 self, u32 a, u32 b, u32 c,
                                u32 d, u32 e, u32 f, u32 g)
{
    volatile u16 *month = (volatile u16 *)(self + 8);
    u16 m = *month;
    s16 count;
    if (m == 0) m = 11;
    else m = (u16)(m - 1);
    *month = m;
    count = *(volatile s16 *)(self + 6);
    if (count > 0) {
        u16 x = *(volatile u16 *)(self + 0x712 + (u32)((s32)count * 14));
        u16 y = *(volatile u16 *)(self + 0x30 + (u32)((s32)(s16)m * 14));
        if (x == y) *(volatile u16 *)(self + 6) = (u16)(count - 1);
    }
    return cm_fn_006d6f70(b, a, self, a, b, c, d, e, f, g);
}
