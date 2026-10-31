/* Preferred-image VA 0x0092ff80, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_THISCALL cm_fn_0058e330(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *pointer);
extern void CM_THISCALL cm_fn_00412ef0(u32 *self);
void CM_THISCALL cm_fn_0092ff80(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 member;
    frame.state = 0xffffffffu;
    frame.handler = 0x00966098u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    CM_GLOBAL(volatile u32, self) = 0x009713b8u;
    member = CM_GLOBAL(volatile u32, self + 0x1fu);
    frame.state = 0;
    if (member) {
        cm_fn_0058e330(member);
        cm_fn_00944c94((void *)member);
    }
    frame.state = 0xffffffffu;
    cm_fn_00412ef0((u32 *)self);
    CM_FS(u32, 0) = frame.prev;
}
