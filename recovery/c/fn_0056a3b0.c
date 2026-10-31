#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 a, u32 b);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_007863f0(void *self, u32 a, u32 b);
extern unsigned long long CM_THISCALL cm_fn_00786780(void *self, u32 a);

void CM_STDCALL cm_fn_0056a3b0(volatile u8 *object, volatile u8 *other,
                              u32 third, u32 fourth, u32 fifth)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 local[224];
    u32 previous = CM_FS(u32, 0);
    u32 pointer;
    u32 value;
    u32 global;

    frame.state = 0xffffffffu;
    frame.handler = 0x009580eb;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;

    if (object != 0 && *(volatile u32 *)(object + 0x39) != 0 &&
        *(volatile u32 *)(object + 0x61) != 0 && other != 0) {
        (void)cm_fn_0077bbe0((void *)local, 0xbe9, 0);
        value = *(volatile u32 *)object;
        frame.state = 0;
        (void)cm_fn_00785790((void *)local, 0, value);

        pointer = *(volatile u32 *)(object + 4);
        value = *(volatile u32 *)(pointer + 0x33);
        (void)cm_fn_00785790((void *)local, 1, value);
        pointer = *(volatile u32 *)(object + 8);
        value = *(volatile u32 *)(pointer + 0x33);
        (void)cm_fn_00785790((void *)local, 2, value);
        pointer = *(volatile u32 *)(object + 0x0c);
        value = *(volatile u32 *)(pointer + 0x33);
        (void)cm_fn_00785790((void *)local, 3, value);

        value = *(volatile u32 *)other;
        (void)cm_fn_00785790((void *)local, 4, value);
        pointer = *(volatile u32 *)(object + 0x39);
        value = pointer != 0 ? *(volatile u32 *)pointer : 0xffffffffu;
        (void)cm_fn_00785790((void *)local, 5, value);
        (void)cm_fn_00785790((void *)local, 6, (u32)(s32)(s8)fourth);
        (void)cm_fn_00785790((void *)local, 7, (u32)(s32)(s8)fifth);

        pointer = *(volatile u32 *)(object + 0x39);
        if (pointer != 0)
            pointer = *(volatile u32 *)(pointer + 0x53);
        value = pointer != 0 ? *(volatile u32 *)pointer : 0xffffffffu;
        (void)cm_fn_00785790((void *)local, 8, value);
        (void)cm_fn_00785790((void *)local, 9, (u32)(s32)(s8)third);

        pointer = *(volatile u32 *)(object + 0x61);
        if (*(volatile s16 *)(pointer + 0x0d) > 0x1d4c) {
            global = CM_GLOBAL(volatile u32, 0x00b7431c);
            (void)cm_fn_00786780((void *)global, (u32)local);
        } else {
            pointer = *(volatile u32 *)(object + 0x39);
            if (pointer != 0) {
                pointer = *(volatile u32 *)(pointer + 0x53);
                if (pointer != 0) {
                    global = CM_GLOBAL(volatile u32, 0x00b7431c);
                    (void)cm_fn_007863f0((void *)global, (u32)local, pointer);
                }
            }
            pointer = *(volatile u32 *)(object + 0x39);
            value = *(volatile u32 *)(object + 0x1a);
            pointer = *(volatile u32 *)(pointer + 0x53);
            if (pointer != value) {
                global = CM_GLOBAL(volatile u32, 0x00b7431c);
                (void)cm_fn_007863f0((void *)global, (u32)local, value);
            }
        }
    }
    CM_FS(u32, 0) = frame.prev;
}
