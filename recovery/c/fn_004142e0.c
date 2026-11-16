#include "cmre.h"

/* Pinned 3.9.68 SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86:
   preferred-image VAs 004142e0-e8 read both stack words and [ecx],
   004142ea-ec push the words and call, and 004142f1 returns with ret 8. */
extern u32 CM_THISCALL cm_fn_0058f120(u32 *self, u32 arg1, u32 arg2);

u32 CM_THISCALL cm_fn_004142e0(volatile u32 *self, u32 arg1, u32 arg2)
{
    u32 receiver = *self;
    return cm_fn_0058f120((u32 *)receiver, arg1, arg2);
}
