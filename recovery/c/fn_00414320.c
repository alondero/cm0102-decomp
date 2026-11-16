#include "cmre.h"

extern int CM_THISCALL cm_fn_0058f120(u32 *self, u32 arg1, u32 arg2);

/* Preferred-image VAs 00414320-00414332, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86:
   ECX supplies the receiver, two stack arguments are forwarded, and RET 8
   cleans them. The CALL result remains in EAX. */
int CM_THISCALL cm_fn_00414320(u32 *self, u32 arg1, u32 arg2)
{
    /* Unresolved entry boundary, same pinned binary/hash cited above:
       VA 00414320 reads [entry ESP+8], 00414324 reads [entry ESP+4],
       and 00414328 reads [ECX+4] BEFORE the first stack write,
       PUSH EAX at 0041432b. If self+4 aliases entry ESP-4, a generated
       prologue can destroy this input before the volatile read below.
       Volatile does not protect against that prologue. This plain-C body
       requires an independently validated entry binding; a gate pass does
       not resolve the alias. No such binding is asserted here. */
    u32 receiver = *(volatile u32 *)((u32)self + 4);
    return cm_fn_0058f120((u32 *)receiver, arg1, arg2);
}
