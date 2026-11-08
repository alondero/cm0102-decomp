#include "cmre.h"

extern long double CM_THISCALL cm_fn_004a2e10(u32 self, u32 input, u32 selector);

/* Preferred-image VA 0x0092fb40; cm0102.exe SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver, one stack DWORD, RET 4; preserve partial-register EAX results. */
__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_0092fb40(u32 self, u32 input)
{
    volatile u32 cached_input = input;
    u32 context = *(volatile u32 *)(self + 8u);
    long double first_value = cm_fn_004a2e10(context, cached_input, 19u);
    /* __ftol at VA 0x0092fb54: signed 64-bit chop (ADR 0006).
       Materialize before the original receiver-field reload. */
    volatile long long first_integer = (long long)first_value;
    context = *(volatile u32 *)(self + 8u);
    s8 first = (s8)(u8)first_integer;
    long double second_value = cm_fn_004a2e10(context, cached_input, 18u);
    /* __ftol at VA 0x0092fb66: preserve the full low DWORD. */
    u32 eax = (u32)(long long)second_value;
    s8 second = (s8)(u8)eax;
    u32 high = eax & 0xffffff00u;
    if (second < 3) {
        if (first < 5) return high;
        if (first < 9) return high | 255u;
        return (high | (u32)(first < 17)) - 3u;
    }
    if (second < 9) {
        if (first < 5) return high | 2u;
        if (first < 9) return high;
        return (high | (u32)(first < 17)) - 2u;
    }
    if (second < 17) {
        if (first < 5) return high | 3u;
        if (first < 9) return high | 2u;
        return (high | (u32)(first < 17)) - 1u;
    }
    if (first < 9) return high | 3u;
    return ((high | (u32)(first >= 17)) - 1u) & 2u;
}
