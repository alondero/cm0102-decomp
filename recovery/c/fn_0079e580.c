#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_THISCALL cm_fn_00768290(u32);
extern u32 CM_THISCALL cm_fn_0076a280(u32, u32);
extern u32 CM_THISCALL cm_fn_0079e5f0(u32, u32);

u32 CM_THISCALL cm_fn_0079e580(u32 self, u32 stream)
{
    volatile struct { u32 prev, handler, state; } frame;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095ee08;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_00768290(self);
    frame.state = 0;
    D(self) = 0x0096e1a8;
    D(self + 0xe) = 0;
    D(self + 0x13) = 1;
    B(self + 0x12) = 4;
    if (!stream || !cm_fn_0076a280(self, stream))
        cm_fn_0079e5f0(self, stream);
    CM_FS(u32, 0) = frame.prev;
    return self;
}

