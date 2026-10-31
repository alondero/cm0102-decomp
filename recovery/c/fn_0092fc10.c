#include "cmre.h"
extern u32 CM_THISCALL cm_fn_00768290(u32 self);
extern u32 CM_THISCALL cm_fn_0076a280(u32 self, u32 stream);
extern u32 CM_THISCALL cm_fn_0092fc80(u32 self, volatile u32 argument);

u32 CM_THISCALL cm_fn_0092fc10(u32 self, u32 stream)
{
    volatile struct { u32 prev, handler; s32 state; } frame;
    frame.state = -1;
    frame.handler = 0x00966028;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_00768290(self);
    frame.state = 0;
    *(volatile u32 *)self = 0x009713a4;
    *(volatile u32 *)(self + 0xe) = 0;
    *(volatile u32 *)(self + 0x13) = 0;
    *(volatile u8 *)(self + 0x12) = 4;
    if (!stream || !cm_fn_0076a280(self, stream))
        cm_fn_0092fc80(self, stream);
    CM_FS(u32, 0) = frame.prev;
    return self;
}
