/* Preferred-image VA 005a5930, base 00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver, one DWORD stack slot, RET4. Its direct wrapper forwards EAX. */
#include "cmre.h"

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_005a5930(u32 self, volatile u32 input)
{
    u32 base, index, address, result;
    long double value;
    if (!input)
        return input;
    base = CM_GLOBAL(volatile u32, self + 8u);
    index = CM_GLOBAL(volatile u32, input);
    address = base + index;
    input = (u32)(s32)CM_GLOBAL(volatile s8, address);
    value = (long double)(s32)input;
    value *= CM_GLOBAL(volatile double, 0x00968968u);
    /* Original registered __ftol call 005a5953; signed FISTP64 (ADR0006). */
    result = (u32)(long long)value;
    CM_GLOBAL(volatile u8, address) = (u8)result;
    return result;
}
