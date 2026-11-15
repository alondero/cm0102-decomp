/* Preferred-image VA0094A698, base00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL0; original PUSH 30000 / PUSH10000 / CALL0094F5B0 / POP / POP.
   That original helper owns the control-word effects. Caller009455C1
   forwards EAX through FNCLEX/RET. ECX has no live callee input. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0094f5b0(u32,u32);
u32 CM_CDECL cm_fn_0094a698(void)
{
    volatile u32 result=cm_fn_0094f5b0(0x10000u,0x30000u);
    return result;
}
