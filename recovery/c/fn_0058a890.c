#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_00786780(void *self, u32 arg1);

u32 CM_CDECL cm_fn_0058a890(u32 argument)
{
    volatile struct { u32 prev, handler; s32 state; } frame;
    u8 scratch[0xe0];
    u32 value, service, result;
    frame.state = -1;
    frame.handler = 0x00958cdb;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0077bbe0(scratch, 0xfc1, 0);
    value = (u32)(s32)(s8)argument;
    frame.state = 0;
    cm_fn_00785790(scratch, 0, value);
    service = CM_GLOBAL(volatile u32, 0x00b7431c);
    result = (u32)cm_fn_00786780((void *)service, (u32)scratch);
    CM_FS(u32, 0) = frame.prev;
    return result;
}
