/* Preferred-image VA 0x007683a0, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_THISCALL cm_fn_00767ee0(u32 self, u32 first, u32 second);
u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_007683a0(
    u32 incoming_eax, u32 incoming_edx, u32 self, u32 first, u32 second)
{
    u32 target = *(volatile u32 *)(self + 0xau);
    (void)incoming_edx;
    if (!target) return incoming_eax;
    return cm_fn_00767ee0(target, first, second);
}
