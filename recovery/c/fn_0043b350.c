#include "cmre.h"

extern void CM_THISCALL cm_fn_0043b370(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);

/* Preferred-image VA 0x0043b350: destructor followed by optional free. */
u32 CM_THISCALL cm_fn_0043b350(u32 self, u8 flags)
{
    cm_fn_0043b370(self);
    if (flags & 1)
        cm_fn_00944c94((void *)self);
    return self;
}
