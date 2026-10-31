#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007862d0(void *self, u32 arg1, u32 arg2);

void CM_THISCALL cm_fn_00420ca0(void *self, u32 *param_1, s8 param_2)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 local_ec[224];
    u32 *esi = (u32 *)self;
    u32 *arg = param_1;

    frame.state = 0xffffffff;
    frame.handler = 0x0095407b;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0077bbe0((void *)local_ec, 0x11, 0);
    frame.state = 0;
    cm_fn_00785790((void *)local_ec, 0, *arg);
    cm_fn_00785790((void *)local_ec, 1, (u32)(s32)param_2);
    cm_fn_00785790((void *)local_ec, 2, **(u32 **)((u8 *)esi + 4));
    cm_fn_007862d0(CM_GLOBAL(void *, 0x00b7431c), (u32)local_ec, *(u32 *)((u8 *)esi + 4));
    CM_FS(u32, 0) = frame.prev;
}
