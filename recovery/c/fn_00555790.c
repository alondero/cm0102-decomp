#include "cmre.h"

extern void CM_THISCALL cm_fn_0058e330(u32 self);
extern void CM_THISCALL cm_fn_0068d040(u32 self);
extern void CM_THISCALL cm_fn_007e7860(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 value);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 value);

void CM_THISCALL cm_fn_00555790(u8 *self)
{
    volatile struct {
        u32 prev;
        u32 handler;
        u32 state;
    } frame;

    frame.state = 0xffffffffu;
    frame.handler = 0x00957cabu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    u32 value = *(volatile u32 *)(self + 4);
    frame.state = 0;
    if (value != 0)
        (void)cm_fn_009452ca(value);

    value = *(volatile u32 *)(self + 0x0c);
    if (value != 0)
        (void)cm_fn_009452ca(value);

    value = *(volatile u32 *)(self + 0x14);
    if (value != 0)
        (void)cm_fn_009452ca(value);

    for (u32 offset = 0; offset != 0x2b * 4u; offset += 4u) {
        u32 array = *(volatile u32 *)self;
        u32 item = *(volatile u32 *)(array + offset);
        if (item != 0) {
            cm_fn_007e7860(item);
            (void)cm_fn_00944c94(item);
        }
    }

    value = *(volatile u32 *)self;
    if (value != 0)
        (void)cm_fn_009452ca(value);

    value = *(volatile u32 *)(self + 0x18);
    if (value != 0) {
        cm_fn_0058e330(value);
        (void)cm_fn_00944c94(value);
    }

    frame.state = 0xffffffffu;
    cm_fn_0068d040((u32)(self + 0x1c));
    CM_FS(u32, 0) = frame.prev;
}
