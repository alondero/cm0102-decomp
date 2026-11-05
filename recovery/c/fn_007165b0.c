#include "cmre.h"

/* Original preferred-image VA 007165b0, image base 00400000;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
__attribute__((target("no-sse3")))
unsigned long long CM_THISCALL cm_fn_007165b0(void *self, volatile u32 arg)
{
    u32 base = (u32)self;
    long double value;
    s32 factor;
    u32 incoming = arg;
    int selected = incoming != 0 && incoming == *(volatile u32 *)(base + 0x1a6u);

    value = *(volatile float *)(base + 0x1a2u);
    factor = *(volatile s8 *)(base + 0xffu);
    value *= CM_GLOBAL(volatile double, 0x00967898);
    if (selected)
        factor += 5;
    arg = (u32)factor; /* Original DWORD spill to the incoming stack slot. */
    value += CM_GLOBAL(volatile double, 0x00969040);
    value *= (long double)(s32)arg;
    if (selected) {
        value *= CM_GLOBAL(volatile double, 0x00968e18);
        /* Original __ftol call at preferred-image VA 007165ec. */
        return (unsigned long long)(long long)value;
    }
    value += CM_GLOBAL(volatile double, 0x009688a0);
    value *= CM_GLOBAL(volatile double, 0x00968968);
    /* Original __ftol call at preferred-image VA 00716623. */
    return (unsigned long long)(long long)value;
}
