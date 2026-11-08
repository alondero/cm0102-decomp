#include "cmre.h"

extern long double CM_THISCALL cm_fn_004a2e10(u32 self, u32 input, u32 selector);

/* Preferred-image VA 0x0091b710; cm0102.exe SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver, one stack DWORD, RET 4. Partial AL writes retain EAX's high bits. */
__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_0091b710(u32 self, u32 input)
{
    volatile u32 cached_input = input;
    u32 context = *(volatile u32 *)(self + 8u);
    long double first_value = cm_fn_004a2e10(context, cached_input, 19u);
    /* __ftol at VA 0x0091b724: chop ST0 to signed 64 bits (ADR 0006).
       Volatile local keeps this conversion before the receiver-field reload. */
    volatile long long first_integer = (long long)first_value;
    context = *(volatile u32 *)(self + 8u);
    s8 first = (s8)(u8)first_integer;
    long double second_value = cm_fn_004a2e10(context, cached_input, 18u);
    /* __ftol at VA 0x0091b736: retain its complete low DWORD. */
    u32 eax = (u32)(long long)second_value;
    s8 second = (s8)(u8)eax;
    u32 result = (eax & 0xffffff00u) | (u32)(first < 12);
    return second < 12 ? result - 1u : result;
}
