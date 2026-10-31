/* Faithful C recovery at preferred-image VA 0043d2b0. */
#include "cmre.h"

extern void CM_THISCALL cm_fn_0043d340(void *self, u32 arg);
extern void CM_FASTCALL cm_fn_00768290(u32 *self);
extern u32 CM_THISCALL cm_fn_0076a280(u32 *self, u32 arg);

u32 *CM_THISCALL cm_fn_0043d2b0(u32 *self, u32 arg)
{
    volatile struct seh_registration {
        u32 prev;
        u32 handler;
        u32 state;
    } frame;

    frame.state = 0xffffffffu;
    frame.handler = 0x00954b78u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    cm_fn_00768290(self);
    *(volatile u32 *)self = 0x00968824u;
    u32 index = CM_GLOBAL(volatile u32, 0x009cf260);
    u32 base = CM_GLOBAL(volatile u32, 0x00ae23a8);
    u32 input = arg;
    u32 scaled = index * 0x122u;
    frame.state = 0;
    *(volatile u32 *)((u32)self + 0x13u) = 5;
    u32 object_address = base + scaled;
    *(volatile u8 *)((u32)self + 0x12u) = 1;
    *(volatile u32 *)((u32)self + 0x0eu) = object_address;
    if (input != 0 && cm_fn_0076a280(self, input) != 0) {
        /* Original returns directly after the successful predicate. */
    } else {
        cm_fn_0043d340(self, input);
    }

    u32 previous = frame.prev;
    CM_FS(u32, 0) = previous;
    return self;
}
