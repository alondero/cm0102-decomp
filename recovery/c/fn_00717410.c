/* Preferred-image VA 0x00717410; CM0102 3.9.68, base 0x00400000.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   EAX/EDX prefix positions retain the physical entry register contract.
   Original callee receives self in EAX and as its sole native stack word. */
#include "cmre.h"
extern u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_006c38c0(
    u32 incoming_eax, u32 incoming_edx, u32 self, u32 object);
u32 CM_CDECL __attribute__((regparm(3))) cm_fn_00717410(
    u32 incoming_eax, u32 incoming_edx, u32 self)
{
    u32 receiver = CM_GLOBAL(volatile u32, self + 0x19eu);
    (void)incoming_eax; /* MOV EAX,ECX kills it before its first use. */
    if (!receiver) return 0;
    return cm_fn_006c38c0(self, incoming_edx, receiver, self);
}
