#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 a, u32 b);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_00786090(void *self, u32 a, u32 b,
                                                     u32 c, u32 d, u32 e, u32 f);

void CM_STDCALL cm_fn_006a5a60(u32 *param_1, u32 *param_2, s8 param_3)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 local[0xe8];
    volatile u32 saved0;
    volatile u32 saved1;
    u32 global0;
    u32 global1;
    u16 threshold;
    u16 high;
    u32 global_self;

    frame.state = 0xffffffff;
    frame.handler = 0x0095cecb;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    global0 = CM_GLOBAL(volatile u32, 0x00dd6ec8);
    global1 = CM_GLOBAL(volatile u32, 0x00dd6ecc);
    threshold = CM_GLOBAL(volatile u16, 0x00ae2c92);
    saved0 = global0;
    saved1 = global1;
    (void)saved1;
    high = *(volatile u16 *)((volatile u8 *)&saved0 + 2);
    if (high > threshold ||
        (high == threshold && (s16)(u16)global0 >
         (s16)CM_GLOBAL(volatile u16, 0x00ae2c90))) {
        goto cleanup;
    }
    (void)cm_fn_0077bbe0((void *)(local + 8), 0x1781, 0);
    frame.state = 0;
    (void)cm_fn_00785790((void *)(local + 8), 0, *(volatile u32 *)param_2);
    (void)cm_fn_00785790((void *)(local + 8), 1, *(volatile u32 *)param_1);
    (void)cm_fn_00785790((void *)(local + 8), 2, (u32)(s32)param_3);
    global_self = CM_GLOBAL(volatile u32, 0x00b7431c);
    (void)cm_fn_00786090((void *)global_self, (u32)(local + 8),
                         (u32)param_2, 0, 0, 0, 0);
cleanup:
    CM_FS(u32, 0) = frame.prev;
}
