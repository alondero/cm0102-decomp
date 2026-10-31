#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);

void CM_STDCALL cm_fn_005a0410(u32 param_1, u32 param_2, s8 param_3)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 local_ec[224];
    u32 index = param_1;
    u32 base;

    frame.state = 0xffffffff;
    frame.handler = 0x009591ab;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0077bbe0((void *)local_ec, 0x1780, 0);
    frame.state = 0;
    cm_fn_00785790((void *)local_ec, 0, index);
    cm_fn_00785790((void *)local_ec, 1, param_2);
    cm_fn_00785790((void *)local_ec, 2, (u32)(s32)param_3);
    base = (u32)CM_GLOBAL(s32, 0x00ae23b4);
    cm_fn_007861e0(CM_GLOBAL(void *, 0x00b7431c), (u32)local_ec,
                   base + index * 0x245u);
    CM_FS(u32, 0) = frame.prev;
}
