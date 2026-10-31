#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0077bbe0(void *self, u32 a, u32 b);
extern u32 CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern u32 CM_THISCALL cm_fn_007863f0(void *self, u32 a, u32 b);
extern u32 CM_THISCALL cm_fn_00786780(void *self, u32 a);

void CM_STDCALL cm_fn_00419ad0(volatile u8 *object, volatile u8 *other,
                             s8 arg3, s16 arg4)
{
    volatile struct { u32 prev; u32 handler; s32 state; } frame;
    volatile u8 local[224];
    u32 previous = CM_FS(u32, 0);

    frame.state = -1;
    frame.handler = 0x00953ecb;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;

    if (object == 0 || *(volatile u32 *)(object + 0x39) == 0 ||
        *(volatile u32 *)(object + 0x61) == 0 || other == 0)
        goto done;

    cm_fn_0077bbe0((void *)local, 0xbc5, 0);
    u32 v = *(volatile u32 *)(object + 0);
    frame.state = 0;
    cm_fn_00785790((void *)local, 0, v);
    u32 p = *(volatile u32 *)(object + 4);
    v = *(volatile u32 *)(p + 0x33);
    cm_fn_00785790((void *)local, 1, v);
    p = *(volatile u32 *)(object + 8);
    v = *(volatile u32 *)(p + 0x33);
    cm_fn_00785790((void *)local, 2, v);
    p = *(volatile u32 *)(object + 0xc);
    v = *(volatile u32 *)(p + 0x33);
    cm_fn_00785790((void *)local, 3, v);
    v = *(volatile u32 *)other;
    cm_fn_00785790((void *)local, 4, v);
    p = *(volatile u32 *)(object + 0x39);
    v = p ? *(volatile u32 *)p : 0xffffffffu;
    cm_fn_00785790((void *)local, 5, v);
    cm_fn_00785790((void *)local, 6, (u32)(s32)arg3);
    cm_fn_00785790((void *)local, 7, (u32)(s32)arg4);
    p = *(volatile u32 *)(object + 0x39);
    if (p != 0) p = *(volatile u32 *)(p + 0x53);
    v = p ? *(volatile u32 *)p : 0xffffffffu;
    cm_fn_00785790((void *)local, 8, v);

    p = *(volatile u32 *)(object + 0x61);
    if (*(volatile s16 *)(p + 0xd) > 0x1d4c) {
        u32 global = CM_GLOBAL(volatile u32, 0x00b7431c);
        cm_fn_00786780((void *)global, (u32)local);
        goto done;
    }
    p = *(volatile u32 *)(object + 0x39);
    if (p != 0) {
        v = *(volatile u32 *)(p + 0x53);
        if (v != 0) {
            u32 global = CM_GLOBAL(volatile u32, 0x00b7431c);
            cm_fn_007863f0((void *)global, (u32)local, v);
        }
    }
    p = *(volatile u32 *)(object + 0x39);
    v = *(volatile u32 *)(object + 0x1a);
    if (*(volatile u32 *)(p + 0x53) != v) {
        u32 global = CM_GLOBAL(volatile u32, 0x00b7431c);
        cm_fn_007863f0((void *)global, (u32)local, v);
    }

done:
    CM_FS(u32, 0) = frame.prev;
}
