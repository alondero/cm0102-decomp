#include "cmre.h"

extern void CM_THISCALL cm_fn_00687970(u8 *self, u8 *buffer);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 pointer);
extern void CM_FASTCALL cm_fn_0049f450(u32 *pointer);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *pointer);
extern void CM_FASTCALL cm_fn_00682300(u32 *self);

void CM_THISCALL cm_fn_007ce950(u8 *self)
{
    volatile struct { u32 prev; u32 handler; u32 state; } frame;
    u32 saved_prev;
    u32 pointer;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095fa48u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    *(volatile u32 *)self = 0x0096e6f0u;
    frame.state = 0;
    cm_fn_00687970(self, 0);

    pointer = *(volatile u32 *)(self + 0xba);
    if (pointer != 0) {
        cm_fn_009452ca(pointer);
        *(volatile u32 *)(self + 0xba) = 0;
    }

    pointer = *(volatile u32 *)(self + 8);
    if (pointer != 0) {
        cm_fn_0049f450((u32 *)pointer);
        cm_fn_00944c94((void *)pointer);
    }

    frame.state = 0xffffffffu;
    cm_fn_00682300((u32 *)self);
    saved_prev = frame.prev;
    CM_FS(u32, 0) = saved_prev;
}
