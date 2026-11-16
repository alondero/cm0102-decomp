#include "cmre.h"

extern void CM_THISCALL cm_fn_0078b250(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);

/* The destructor remains an original-code boundary. Fixture initialization,
   not extra writes here, must make its 0x00687970 callee return. */
u32 CM_THISCALL cm_fn_0078b230(u32 self, u8 flags)
{
    cm_fn_0078b250(self);
    if ((flags & 1) != 0)
        cm_fn_00944c94((void *)self);
    return self;
}
