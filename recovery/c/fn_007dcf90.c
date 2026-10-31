#include "cmre.h"
extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern u32 CM_THISCALL cm_fn_007e7760(u32 self, u32 input);

u32 CM_THISCALL cm_fn_007dcf90(u32 self, u32 input)
{
    volatile struct { u32 prev, handler; s32 state; } frame;
    u32 value;
    frame.state = -1;
    frame.handler = 0x0095ff58;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_007e7760(self, input);
    frame.state = 0;
    cm_fn_0054a110((u16 *)(self + 0x23));
    *(volatile u32 *)self = 0x0096eb14;
    *(volatile u8 *)(self + 4) = 0;
    *(volatile u8 *)(self + 5) = 9;
    *(volatile u8 *)(self + 6) = 5;
    value = CM_GLOBAL(volatile u32, 0x009cf2e4);
    *(volatile u32 *)(self + 0x17) = value;
    value = CM_GLOBAL(volatile u32, 0x009cfa10);
    *(volatile u8 *)(self + 0x1f) = 9;
    *(volatile u32 *)(self + 0x1b) = value;
    *(volatile u8 *)(self + 0x20) = 0xff;
    *(volatile u8 *)(self + 0x21) = 1;
    *(volatile u8 *)(self + 0x22) = 0;
    CM_FS(u32, 0) = frame.prev;
    return self;
}
