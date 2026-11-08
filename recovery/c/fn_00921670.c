#include "cmre.h"

extern long double CM_THISCALL cm_fn_004a2e10(u32 self, u32 input, u32 selector);

/* Preferred-image VA 0x00921670; cm0102.exe SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver, one stack DWORD, RET 4; preserve partial-register EAX results. */
__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_00921670(u32 self, u32 input)
{
    volatile u32 cached_input = input;
    u32 context = *(volatile u32 *)(self + 8u);
    long double first_value = cm_fn_004a2e10(context, cached_input, 19u);
    /* __ftol at VA 0x00921684: signed 64-bit chop (ADR 0006).
       Materialize before the original receiver-field reload. */
    volatile long long first_integer = (long long)first_value;
    context = *(volatile u32 *)(self + 8u);
    s8 first = (s8)(u8)first_integer;
    long double second_value = cm_fn_004a2e10(context, cached_input, 18u);
    /* __ftol at VA 0x00921696: preserve the full low DWORD. */
    volatile long long second_integer = (long long)second_value;
    u32 eax = (u32)second_integer;
    s32 adjustment = *(volatile s8 *)(self + 0xeeu);
    s32 threshold = 6 - adjustment;
    s32 second = (s8)(u8)eax;
    if (second < threshold) {
        eax = ((u32)second & 0xffffff00u) | (u32)(first < threshold);
        return eax - 1u;
    }
    return ((u32)(s32)first & 0xffffff00u) | (u32)(first < threshold);
}
