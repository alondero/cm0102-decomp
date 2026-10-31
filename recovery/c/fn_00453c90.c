#include "cmre.h"
extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern u32 CM_THISCALL cm_fn_0068d020(u32 self);

u32 CM_THISCALL cm_fn_00453c90(u32 self)
{
    volatile struct { u32 prev, handler; s32 state; } frame;
    u32 i;
    frame.state = -1;
    frame.handler = 0x00955778;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0054a110((u16 *)self);
    frame.state = 0;
    cm_fn_0068d020(self + 0x18);
    for (i = 0; i < 10; ++i)
        *(volatile u32 *)(self + i * 4) = 0;
    *(volatile u16 *)(self + 40) = 0;
    CM_FS(u32, 0) = frame.prev;
    return self;
}
