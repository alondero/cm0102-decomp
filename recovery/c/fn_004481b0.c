#include "cmre.h"
extern void cm_fn_005a53c0(void);

u32 CM_THISCALL cm_fn_004481b0(u32 self)
{
    volatile struct { u32 prev, handler; s32 state; } frame;
    u32 prev;
    (void)self;
    frame.state = -1;
    frame.handler = 0x0095529e;
    prev = CM_FS(u32, 0);
    frame.prev = prev;
    CM_FS(u32, 0) = (u32)&frame;
    frame.state = 10;
    cm_fn_005a53c0();
    *(volatile u8 *)&frame.state = 9;
    cm_fn_005a53c0();
    *(volatile u8 *)&frame.state = 8;
    cm_fn_005a53c0();
    *(volatile u8 *)&frame.state = 7;
    cm_fn_005a53c0();
    CM_FS(u32, 0) = frame.prev;
    return prev;
}

