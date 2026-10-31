#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_00786090(void *self, u32 arg1, u32 arg2,
                                                       u32 arg3, u32 arg4, u32 arg5,
                                                       u32 arg6);

void CM_STDCALL cm_fn_006a4c40(u32 *param_1)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 local_area[232];
    volatile u8 *buffer = local_area + 8;
    u32 original_high;
    u32 original_low;
    u16 limit_high;
    u16 input_high;
    u32 restore_prev;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095ce6bu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    original_high = CM_GLOBAL(volatile u32, 0x00dd6ec8);
    original_low = CM_GLOBAL(volatile u32, 0x00dd6ecc);
    limit_high = CM_GLOBAL(volatile u16, 0x00ae2c92);
    *(volatile u32 *)(local_area + 0) = original_high;
    *(volatile u32 *)(local_area + 4) = original_low;
    input_high = *(volatile u16 *)(local_area + 2);
    if (input_high <= limit_high &&
        (input_high < limit_high ||
         (s16)original_high <= CM_GLOBAL(volatile s16, 0x00ae2c90))) {
        u32 value;
        u32 child;

        cm_fn_0077bbe0((void *)buffer, 0x1772u, 0);
        frame.state = 0;
        child = *(volatile u32 *)param_1;
        cm_fn_00785790((void *)buffer, 0, child);

        child = *(volatile u32 *)((volatile u8 *)param_1 + 0xcf);
        if (child == 0) {
            value = 0x64;
        } else {
            u32 child_month = *(volatile u16 *)(child + 0x40);
            u32 current_month = CM_GLOBAL(volatile u16, 0x00ae2c92);
            s32 child_day = *(volatile s16 *)(child + 0x3e);
            s32 current_day = CM_GLOBAL(volatile s16, 0x00ae2c90);
            u32 delta = current_month - child_month;
            value = delta * 0x16du - (u32)child_day + (u32)current_day;
        }
        cm_fn_00785790((void *)buffer, 1, value);
        child = CM_GLOBAL(volatile u32, 0x00b7431c);
        cm_fn_00786090((void *)child, (u32)buffer, (u32)param_1,
                       0, 0, 0, 0);
    }

    restore_prev = frame.prev;
    CM_FS(u32, 0) = restore_prev;
}
