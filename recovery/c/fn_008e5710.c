#include "cmre.h"

extern void CM_THISCALL cm_fn_0068d040(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1f0(u8 *self, const u8 *target);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg);

void CM_THISCALL cm_fn_008e5710(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 list;
    frame.state = 0xffffffffu;
    frame.handler = 0x0096490bu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    list = *(volatile u32 *)self;
    frame.state = 0;
    if (list != 0) {
        if (*(volatile u16 *)list == 1) {
            cm_fn_0068d040(list);
            cm_fn_00944c94((void *)list);
        } else {
            cm_fn_0068d1f0((u8 *)list, (const u8 *)(self + 0x20u));
        }
    }
    list = *(volatile u32 *)(self + 4u);
    if (list != 0) {
        cm_fn_0068d040(list);
        cm_fn_00944c94((void *)list);
    }
    CM_FS(u32, 0) = frame.prev;
}
