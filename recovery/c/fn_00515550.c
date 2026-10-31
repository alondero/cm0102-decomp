/* Faithful C recovery at preferred-image VA 00515550. */
#include "cmre.h"

extern void CM_FASTCALL cm_fn_0049f450(u32 *self);
extern void CM_FASTCALL cm_fn_00682300(u32 *self);
extern void CM_THISCALL cm_fn_00687970(u8 *self, u8 *buffer);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg);

void CM_FASTCALL cm_fn_00515550(u32 *self)
{
    volatile struct seh_registration {
        u32 prev;
        u32 handler;
        u32 state;
    } frame;

    frame.state = 0xffffffffu;
    frame.handler = 0x00957408u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    *(volatile u32 *)self = 0x00969178u;
    frame.state = 0;
    cm_fn_00687970((u8 *)self, (u8 *)0);

    u32 value = *(volatile u32 *)((u32)self + 0xbau);
    if (value != 0) {
        cm_fn_009452ca(value);
        *(volatile u32 *)((u32)self + 0xbau) = 0;
    }

    u32 owned = *(volatile u32 *)((u32)self + 8u);
    if (owned != 0) {
        cm_fn_0049f450((u32 *)owned);
        cm_fn_00944c94((void *)owned);
    }

    frame.state = 0xffffffffu;
    cm_fn_00682300(self);
    u32 previous = frame.prev;
    CM_FS(u32, 0) = previous;
}
