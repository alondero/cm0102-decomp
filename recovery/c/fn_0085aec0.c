#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);

u32 CM_STDCALL cm_fn_0085aec0(u32 input)
{
    volatile struct { u32 prev, handler; s32 state; } frame;
    u8 scratch[0xe0];
    u32 value, result, self;
    frame.state = -1;
    frame.handler = 0x0096297b;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0077bbe0(scratch, 0x18, 0);
    frame.state = 0;
    value = *(volatile u32 *)input;
    cm_fn_00785790(scratch, 0, value);
    cm_fn_00785790(scratch, 1, 0);
    self = CM_GLOBAL(volatile u32, 0x00b7431c);
    result = (u32)cm_fn_007861e0((void *)self, (u32)scratch, input);
    CM_FS(u32, 0) = frame.prev;
    return result;
}
