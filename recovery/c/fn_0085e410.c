#include "cmre.h"
extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);

u32 CM_THISCALL cm_fn_0085e410(u32 self)
{
    volatile struct { u32 prev, handler; s32 state; } frame;
    frame.state = -1;
    frame.handler = 0x009629eb;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0054a110((u16 *)(self + 0x25));
    frame.state = 0;
    cm_fn_0054a110((u16 *)(self + 0x2d));
    *(volatile u32 *)(self + 4) = 0xffffffff;
    *(volatile u32 *)(self + 0x4a) = 0xffffffff;
    *(volatile u8 *)(self + 0x49) = 0;
    *(volatile u16 *)(self + 0x3b) = 0;
    *(volatile u8 *)(self + 0x4e) = 0;
    *(volatile u32 *)(self + 0x40) = 0;
    *(volatile u32 *)(self + 0x45) = 0;
    *(volatile u32 *)(self + 8) = 0;
    CM_FS(u32, 0) = frame.prev;
    return self;
}
