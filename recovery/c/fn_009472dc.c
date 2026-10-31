/* Preferred-image VA 0x009472dc, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
typedef void (CM_STDCALL *file_time_fn_9472dc)(void *out);
extern unsigned long long CM_STDCALL cm_fn_00945e40(u32, u32, u32, u32);
unsigned long long CM_CDECL __attribute__((regparm(3))) cm_fn_009472dc(
    u32 incoming_eax, u32 incoming_edx, u32 incoming_ecx)
{
    volatile u32 filetime[2];
    unsigned long long product;
    u32 low, high, old;
    (void)incoming_eax; (void)incoming_edx;
    filetime[1] = incoming_ecx;
    filetime[0] = incoming_ecx;
    ((file_time_fn_9472dc)CM_GLOBAL(volatile u32, 0x00967144u))((void *)filetime);
    product = cm_fn_00945e40(filetime[1], 0, 0, 1);
    old = (u32)product;
    low = old + filetime[0];
    high = (u32)(product >> 32) + (low < old);
    CM_GLOBAL(volatile u32, 0x00de2ff0u) = low;
    CM_GLOBAL(volatile u32, 0x00de2ff4u) = high;
    return ((unsigned long long)high << 32) | low;
}
