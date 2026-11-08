/* Preferred-image VA 0043f4c0, base 00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver, signed QWORD stack argument, RET8; retain full EAX. */
#include "cmre.h"

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_0043f4c0(u32 self, long long input)
{
    long double value = (long double)input;
    u32 result;
    value /= CM_GLOBAL(volatile double, 0x00ad9cb8u);
    /* Original registered __ftol call 0043f4cd; signed FISTP64 (ADR0006). */
    result = (u32)(long long)value;
    CM_GLOBAL(volatile u32, self) = result;
    return result;
}
