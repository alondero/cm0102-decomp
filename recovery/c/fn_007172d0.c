/* Preferred-image VA 0x007172d0, base 0x00400000; CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Physical EAX/EDX/ECX prefix preserves the incidental full EAX result.
   EDX is positional only; the native routine does not consume it. */
#include "cmre.h"

/* Original boundary: only EAX is consumed; bound is at native ESP+4. */
extern u32 CM_CDECL cm_fn_0090cfc0(u32 bound);

__attribute__((regparm(3), target("no-sse3")))
u32 CM_STDCALL cm_fn_007172d0(u32 incoming_eax, u32 incoming_edx,
                             u32 self, u32 delta_arg)
{
    u32 eax = incoming_eax;
    s8 delta = (s8)delta_arg;
    s8 before, value;
    s32 sum;
    s16 word;
    (void)incoming_edx;
    if (delta == 0) {
        eax = *(volatile u32 *)(self + 0x19eu);
        if (*(volatile s32 *)(eax + 0x477au) == -1) {
            word = *(volatile s16 *)(self + 0x35u);
            if (word <= 7500 || *(volatile s8 *)(self + 0x17u) >= 10)
                return eax & 0xffffff00u;
        }
    }
    before = *(volatile s8 *)(self + 0x17u);
    eax = (eax & 0xffffff00u) | (u8)before;
    value = before;
    if (delta > 0) {
        sum = (s32)before + (s32)delta;
        if (sum > 10) {
            eax = cm_fn_0090cfc0(21u);
            if ((s32)eax < sum) {
                eax = cm_fn_0090cfc0((u32)((s32)delta + 1));
                value = (s8)((u8)eax + (u8)before);
                goto changed;
            }
        }
    } else if (delta < 0) {
        sum = (s32)before + (s32)delta;
        if (sum < 10) {
            eax = cm_fn_0090cfc0(11u);
            if ((s32)eax > sum) {
                eax = cm_fn_0090cfc0((u32)((s32)delta - 1));
                value = (s8)((u8)eax + (u8)before);
                goto changed;
            }
        }
    }
    value = (s8)((u8)before + (u8)delta);
changed:
    word = *(volatile s16 *)(self + 0x35u);
    if (word >= 7500) {
        long double live = (long double)(s32)value;
        long double cap = (long double)(s32)word;
        volatile double rounded;
        cap *= CM_GLOBAL(volatile double, 0x00968d60);
        cap -= CM_GLOBAL(volatile double, 0x0096d868);
        rounded = (double)cap; /* Native FSTP m64 at VA007173a8. */
        if (!(live > (long double)rounded)) live = (long double)rounded;
        eax = (u32)(long long)live; /* Native __ftol CALL VA007173bd. */
        value = (s8)eax;
    }
    if (value < 1) value = 1;
    else if (value > 20) value = 20;
    *(volatile s8 *)(self + 0x17u) = value;
    return (eax & 0xffffff00u) | (u32)(value != before);
}
