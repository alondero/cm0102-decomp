#include "cmre.h"

extern void CM_THISCALL cm_fn_0077bbe0(void *self, u32 a, u32 b);
extern void CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern void CM_THISCALL cm_fn_007861e0(void *self, u32 a, u32 b);

void CM_STDCALL cm_fn_006a0540(volatile u32 *object, volatile u32 *other)
{
    struct seh_frame { u32 prev, handler, state; } volatile frame;
    volatile u8 local[0xe8];
    u32 entry_prev = CM_FS(u32, 0);
    frame.state = (u32)-1;
    frame.handler = 0x0095cd8bu;
    frame.prev = entry_prev;
    CM_FS(u32, 0) = (u32)(unsigned long)&frame;

    if (object != 0) {
        u32 nested = *(volatile u32 *)((volatile u8 *)object + 0x39);
        if (nested != 0 && other != 0) {
            u32 date_low = CM_GLOBAL(volatile u32, 0x00dd6ec8u);
            u32 date_high = CM_GLOBAL(volatile u32, 0x00dd6eccu);
            u16 limit_high = CM_GLOBAL(volatile u16, 0x00ae2c92u);
            *(volatile u32 *)local = date_low;
            *(volatile u32 *)(local + 4) = date_high;
            u16 value_high = *(volatile u16 *)(local + 2);
            u16 limit_high_again = limit_high;
            if (value_high < limit_high_again ||
                (value_high == limit_high_again &&
                 (s16)date_low <= (s16)CM_GLOBAL(volatile u16, 0x00ae2c90u))) {
                cm_fn_0077bbe0((void *)(local + 8), 0x1775, 0);
                u32 first = object[0];
                frame.state = 0;
                cm_fn_00785790((void *)(local + 8), 0, first);
                u32 p = object[1];
                cm_fn_00785790((void *)(local + 8), 1,
                    *(volatile u32 *)(p + 0x33));
                p = object[2];
                cm_fn_00785790((void *)(local + 8), 2,
                    *(volatile u32 *)(p + 0x33));
                p = object[3];
                cm_fn_00785790((void *)(local + 8), 3,
                    *(volatile u32 *)(p + 0x33));
                nested = *(volatile u32 *)((volatile u8 *)object + 0x39);
                cm_fn_00785790((void *)(local + 8), 4, *(volatile u32 *)nested);
                cm_fn_00785790((void *)(local + 8), 5, other[0]);
                cm_fn_00785790((void *)(local + 8), 6,
                    (u32)(s32)*(volatile s8 *)((volatile u8 *)object + 0x3d));
                nested = *(volatile u32 *)((volatile u8 *)object + 0x39);
                u32 global = CM_GLOBAL(volatile u32, 0x00b7431cu);
                cm_fn_007861e0((void *)global, (u32)(unsigned long)(local + 8), nested);
            }
        }
    }
    CM_FS(u32, 0) = frame.prev;
}
