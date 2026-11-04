#include "cmre.h"

/* Preferred-image VAs, base 0x00400000; original SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Normal-return SEH chain is explicit; exception dispatch remains a boundary. */
extern u32 CM_THISCALL cm_fn_0068d040(void *self);
typedef u32 (CM_THISCALL *virtual_0)(void *self, u32 flags);

u32 CM_THISCALL cm_fn_00453cf0(u8 *self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 child, result;
    frame.state = 0xffffffffu;
    frame.handler = 0x009557a3u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    child = *(u32 *)(self + 0x26);
    frame.state = 1;
    if (child) {
        u32 table = *(u32 *)child;
        ((virtual_0)*(u32 *)table)((void *)child, 1);
    }
    *(volatile u8 *)&frame.state = 0;
    result = cm_fn_0068d040(self + 0x18);
    CM_FS(u32, 0) = frame.prev;
    return result;
}
