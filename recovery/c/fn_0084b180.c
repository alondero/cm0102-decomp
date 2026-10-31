/* Preferred-image VA 0x0084b180, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0053d740(u8 *first, u32 *second);
extern u32 CM_THISCALL cm_fn_008c7950(u32 self, u32 first, u32 second, u32 third, u32 fourth, u32 fifth);
u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_0084b180(
    u32 incoming_eax, u32 incoming_edx, u32 incoming_ecx, u32 first, u32 unused, u32 third)
{
    u32 primary = *(volatile u32 *)(first + 0x1au);
    u32 candidate, result, field;
    (void)incoming_edx; (void)incoming_ecx; (void)unused;
    if (!primary) return incoming_eax & 0xffffff00u;
    candidate = *(volatile u32 *)(third + 0x53u);
    if (*(volatile u8 *)(candidate + 0x76u) != 0x0eu) return (candidate & 0xffffff00u) | 1u;
    if (primary == candidate) return (candidate & 0xffffff00u) | 1u;
    if (*(volatile u32 *)(first + 0x1eu) == candidate) return (candidate & 0xffffff00u) | 1u;
    result = cm_fn_0053d740((u8 *)first, (u32 *)candidate);
    if (result) return (result & 0xffffff00u) | 1u;
    field = *(volatile u32 *)(first + 0x24u);
    if (!field && !*(volatile u8 *)(first + 0x22u)) return 0;
    candidate = *(volatile u32 *)(third + 0x53u);
    return cm_fn_008c7950(CM_GLOBAL(volatile u32, 0x00de1f54u), first, candidate, 0, 0, 0);
}
