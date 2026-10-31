/* Preferred-image VA 0x004143f0, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
typedef unsigned long long (__attribute__((stdcall, regparm(3))) *callback_4143f0)(u32, u32, u32, u32, u32, u32);
unsigned long long CM_THISCALL cm_fn_004143f0(u32 self, u32 outer, u32 inner, u32 a, u32 b, u32 c)
{
    u32 table = *(volatile u32 *)(self + 8u);
    u32 first = *(volatile u32 *)(table + outer * 4u);
    u32 nested = *(volatile u32 *)(first + 4u);
    u32 receiver = *(volatile u32 *)(nested + inner * 4u);
    u32 vtable = *(volatile u32 *)receiver;
    return ((callback_4143f0)*(volatile u32 *)(vtable + 0xcu))(
        a, vtable, receiver, a, b, c);
}
