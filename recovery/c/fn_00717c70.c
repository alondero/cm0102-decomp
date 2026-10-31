/* Preferred-image VA 0x00717c70, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_006c38c0(u32, u32, u32, u32);
extern u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_006c38f0(u32, u32, u32, u32);
u32 CM_CDECL __attribute__((regparm(3))) cm_fn_00717c70(
    u32 incoming_eax, u32 incoming_edx, u32 self)
{
    u32 target = *(volatile u32 *)(self + 0x19eu);
    u32 result = target ? cm_fn_006c38c0(incoming_eax, incoming_edx, target, self) : 0;
    result &= 0x880u;
    if (!result) return 0;
    target = *(volatile u32 *)(self + 0x19eu);
    /* The second original callee writes EDX before reading it. */
    result = target ? cm_fn_006c38f0(result, incoming_edx, target, self) : 0;
    result &= 0x880u;
    if (!result) return 0;
    return (result & 0xffffff00u) | 1u;
}
