/* Preferred-image VA 0x00662d50, image base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ret 4 with ECX read first: thiscall with one stack argument. Only the low
   byte of that argument is tested (00662d58). 00662d70 and 00944c94 remain
   original boundaries; EAX is the incoming ECX on both paths. */
#include "cmre.h"

extern void CM_THISCALL cm_fn_00662d70(u32 self);
extern void CM_CDECL cm_fn_00944c94(u32 block);

u32 CM_THISCALL cm_fn_00662d50(u32 self, u32 flags)
{
    cm_fn_00662d70(self);
    if ((u8)flags & 1u)
        cm_fn_00944c94(self);
    return self;
}
