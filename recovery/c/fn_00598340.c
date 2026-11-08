/* Preferred-image VA 00598340, base 00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver, one DWORD stack slot, RET4; preserve full EAX forwarding. */
#include "cmre.h"

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_00598340(u32 self, volatile u32 adjustment)
{
    long double value;
    u32 result;
    adjustment = (u32)((s32)(s8)adjustment + 100);
    value = (long double)(s32)adjustment;
    value *= CM_GLOBAL(volatile double, 0x00967898u);
    value *= (long double)CM_GLOBAL(volatile s32, self + 0x14u);
    /* Original registered __ftol call 0059835c; signed FISTP64 (ADR0006). */
    result = (u32)(long long)value;
    CM_GLOBAL(volatile u32, self + 0x14u) = result;
    return result;
}
