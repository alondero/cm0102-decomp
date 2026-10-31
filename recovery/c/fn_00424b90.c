#include "cmre.h"
extern u32 CM_THISCALL cm_fn_00424c00(u32 self);
extern u8 *CM_THISCALL cm_fn_008e84a0(u8 *object, u32 arg1, s32 arg2, s32 arg3, u32 arg4, u32 arg5);

u32 CM_THISCALL cm_fn_00424b90(u32 self, u32 a, s32 b, s32 c, u32 d, u32 e)
{
    volatile struct { u32 prev, handler; s32 state; } frame;
    frame.state = -1;
    frame.handler = 0x009542c8;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_008e84a0((u8 *)self, a, b, c, d, e);
    frame.state = 0;
    *(volatile u32 *)self = 0x00967c10;
    cm_fn_00424c00(self);
    *(volatile u8 *)(self + 0x13) = 0xff;
    *(volatile u8 *)(self + 0x14) = 0xff;
    CM_FS(u32, 0) = frame.prev;
    return self;
}
