/* Faithful C recovery at preferred-image VA 008508c0. */
#include "cmre.h"

extern u32 *CM_THISCALL cm_fn_00851020(u32 *self, u16 value);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);

u32 CM_THISCALL cm_fn_008508c0(void *self, u32 unused_arg)
{
    (void)unused_arg;
    volatile struct seh_registration {
        u32 prev;
        u32 handler;
        u32 state;
    } frame;

    frame.state = 0xffffffffu;
    frame.handler = 0x009626ebu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    u32 base = (u32)self;
    u32 link = *(volatile u32 *)(base + 4u);
    u32 object = *(volatile u32 *)(link + 0x5du);
    u8 flags = *(volatile u8 *)(object + 0x11cu);
    if ((flags & 4u) == 0) {
        u32 allocation = (u32)cm_fn_00944cf1(0xeeu);
        frame.state = 0;
        if (allocation != 0) {
            u16 value = *(volatile u16 *)(base + 0x40u);
            u32 output = (u32)cm_fn_00851020((u32 *)allocation, value);
            *(volatile u32 *)(base + 0xeeu) = output;
        } else {
            *(volatile u32 *)(base + 0xeeu) = 0;
        }
    } else {
        *(volatile u32 *)(base + 0xeeu) = 0;
    }

    u32 previous = frame.prev;
    CM_FS(u32, 0) = previous;
    return 1;
}
