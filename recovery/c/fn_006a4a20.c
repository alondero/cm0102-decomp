#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);

void CM_STDCALL cm_fn_006a4a20(u32 *param_1, s16 param_2)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 local_ec[224];
    u32 value_a;
    u32 value_b;
    u16 limit_high;
    u16 value_high;
    s16 limit_low;

    frame.state = 0xffffffff;
    frame.handler = 0x0095ce4b;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    value_a = CM_GLOBAL(volatile u32, 0x00dd6ec8);
    value_b = CM_GLOBAL(volatile u32, 0x00dd6ecc);
    limit_high = CM_GLOBAL(volatile u16, 0x00ae2c92);
    (void)value_b;
    value_high = (u16)(value_a >> 16);
    if (value_high < limit_high ||
        (value_high == limit_high &&
         (limit_low = CM_GLOBAL(volatile s16, 0x00ae2c90),
          (s16)value_a <= limit_low))) {
        cm_fn_0077bbe0((void *)local_ec, 0x1771, 0);
        frame.state = 0;
        cm_fn_00785790((void *)local_ec, 0, *param_1);
        cm_fn_00785790((void *)local_ec, 1, (u32)(s32)param_2);
        cm_fn_007861e0(CM_GLOBAL(void *, 0x00b7431c), (u32)local_ec, (u32)param_1);
    }
    CM_FS(u32, 0) = frame.prev;
}
