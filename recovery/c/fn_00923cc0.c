#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);

struct cm_seh_00923cc0 { void *prev; u32 handler; u32 state; };

void CM_STDCALL cm_fn_00923cc0(void *object, s8 param)
{
    volatile u8 local[232];
    volatile struct cm_seh_00923cc0 frame;
    u32 first;
    u32 second;
    u16 threshold;
    u16 high;
    u32 address;
    u32 target;

    frame.prev = CM_FS(void *, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00965c4bu;
    CM_FS(void *, 0) = (void *)&frame;
    if (object != 0) {
        first = CM_GLOBAL(volatile u32, 0x00dd6ec8);
        second = CM_GLOBAL(volatile u32, 0x00dd6ecc);
        threshold = CM_GLOBAL(volatile u16, 0x00ae2c92);
        *(volatile u32 *)(local + 0) = first;
        *(volatile u32 *)(local + 4) = second;
        high = *(volatile u16 *)(local + 2);
        if (high <= threshold &&
            (high < threshold ||
             (s16)(u16)first <= CM_GLOBAL(volatile s16, 0x00ae2c90))) {
            cm_fn_0077bbe0((void *)(local + 8), 2, 0);
            address = *(volatile u32 *)((volatile u8 *)object + 0x1c);
            frame.state = 0;
            cm_fn_00785790((void *)(local + 8), 0,
                           *(volatile u32 *)address);
            address = *(volatile u32 *)((volatile u8 *)object + 0x20);
            cm_fn_00785790((void *)(local + 8), 1,
                           *(volatile u32 *)address);
            address = *(volatile u32 *)((volatile u8 *)object + 0x14);
            cm_fn_00785790((void *)(local + 8), 2,
                           *(volatile u32 *)address);
            cm_fn_00785790((void *)(local + 8), 3, (u32)(s32)param);
            address = *(volatile u32 *)((volatile u8 *)object + 0x1c);
            target = CM_GLOBAL(volatile u32, 0x00b7431c);
            cm_fn_007861e0((void *)target, (u32)(local + 8), address);
            address = *(volatile u32 *)((volatile u8 *)object + 0x20);
            target = CM_GLOBAL(volatile u32, 0x00b7431c);
            cm_fn_007861e0((void *)target, (u32)(local + 8), address);
        }
    }
    CM_FS(void *, 0) = frame.prev;
}
