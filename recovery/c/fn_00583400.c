#include "cmre.h"
extern u32 CM_THISCALL cm_fn_00583480(u32 self, u32 argument);
extern u32 CM_THISCALL cm_fn_00768290(u32 self);
extern u32 CM_THISCALL cm_fn_0076a280(u32 self, u32 stream);

u32 CM_THISCALL cm_fn_00583400(u32 self, u32 stream)
{
    volatile struct { u32 prev, handler; s32 state; } frame;
    frame.state = -1;
    frame.handler = 0x009589d8;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_00768290(self);
    frame.state = 0;
    *(volatile u32 *)self = 0x0096a1bc;
    *(volatile u32 *)(self + 0xe) = 0;
    *(volatile u32 *)(self + 0x13) = 2;
    *(volatile u8 *)(self + 0x12) = 2;
    if (!stream || !cm_fn_0076a280(self, stream))
        cm_fn_00583480(self, stream);
    CM_FS(u32, 0) = frame.prev;
    return self;
}
