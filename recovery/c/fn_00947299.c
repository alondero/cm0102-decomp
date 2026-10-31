/* Preferred-image VA 0x00947299, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
typedef void (CM_STDCALL *file_time_fn)(void *out);
extern unsigned long long CM_STDCALL cm_fn_00945e40(u32, u32, u32, u32);
extern unsigned long long CM_STDCALL cm_fn_0094dac0(u32, u32, u32, u32);
unsigned long long CM_CDECL __attribute__((regparm(3))) cm_fn_00947299(
    u32 incoming_eax, u32 incoming_edx, u32 incoming_ecx)
{
    volatile u32 filetime[2];
    unsigned long long product;
    u32 before, low, high, subtract, filelow;
    (void)incoming_eax;
    (void)incoming_edx;
    filetime[1] = incoming_ecx;
    filetime[0] = incoming_ecx;
    ((file_time_fn)CM_GLOBAL(volatile u32, 0x00967144u))((void *)filetime);
    product = cm_fn_00945e40(filetime[1], 0, 0, 1);
    before = (u32)product;
    subtract = CM_GLOBAL(volatile u32, 0x00de2ff0u);
    low = before - subtract;
    filelow = filetime[0];
    high = (u32)(product >> 32) - CM_GLOBAL(volatile u32, 0x00de2ff4u) - (before < subtract);
    before = low;
    low += filelow;
    high += low < before;
    return cm_fn_0094dac0(low, high, 0x2710u, 0);
}
