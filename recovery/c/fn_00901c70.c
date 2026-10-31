#include "cmre.h"

extern void CM_FASTCALL cm_fn_00768290(u32 *self);
extern u32 CM_THISCALL cm_fn_0076a280(u32 *self, u32 arg);
extern void CM_THISCALL cm_fn_00901d00(u32 *self, u32 arg);

u32 *CM_THISCALL cm_fn_00901c70(u32 *self, u32 arg)
{
    volatile struct { u32 prev; u32 handler; u32 state; } frame;
    u32 *result;
    u32 index;
    u32 table;
    u32 saved_prev;

    frame.state = 0xffffffffu;
    frame.handler = 0x00965128u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    cm_fn_00768290(self);
    *(volatile u32 *)self = 0x00970748u;
    index = (u32)CM_GLOBAL(volatile s32, 0x009cf4dc);
    table = (u32)CM_GLOBAL(volatile u32, 0x00ae23a8);
    frame.state = 0;
    *(volatile u32 *)((u8 *)self + 0x13) = 0x1a;
    *(volatile u8 *)((u8 *)self + 0x12) = 1;
    *(volatile u32 *)((u8 *)self + 0x0e) = table + index * 0x122u;
    if (arg == 0 || cm_fn_0076a280(self, arg) == 0)
        cm_fn_00901d00(self, arg);
    saved_prev = frame.prev;
    result = self;
    CM_FS(u32, 0) = saved_prev;
    return result;
}
