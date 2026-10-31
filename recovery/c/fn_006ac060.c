#include "cmre.h"

extern void CM_THISCALL cm_fn_0077bbe0(void *self, u32 a, u32 b);
extern void CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern void CM_THISCALL cm_fn_007861e0(void *self, u32 a, u32 b);

void CM_STDCALL cm_fn_006ac060(volatile u32 *object, s8 first, s8 second,
    s16 third, s8 fourth)
{
    struct seh_frame { u32 prev, handler, state; } volatile frame;
    volatile u8 local[0xe0];
    u32 entry_prev = CM_FS(u32, 0);
    frame.state = (u32)-1;
    frame.handler = 0x0095d05bu;
    frame.prev = entry_prev;
    CM_FS(u32, 0) = (u32)(unsigned long)&frame;

    if (object != 0) {
        u32 nested = *(volatile u32 *)((volatile u8 *)object + 0x39);
        if (nested != 0) {
            u32 target = *(volatile u32 *)(nested + 0x53);
            if (target != 0) {
                cm_fn_0077bbe0((void *)local, 0x178a, 0);
                u32 first_value = object[0];
                frame.state = 0;
                cm_fn_00785790((void *)local, 0, first_value);
                u32 p = object[1];
                cm_fn_00785790((void *)local, 1,
                    *(volatile u32 *)(p + 0x33));
                p = object[2];
                cm_fn_00785790((void *)local, 2,
                    *(volatile u32 *)(p + 0x33));
                p = object[3];
                cm_fn_00785790((void *)local, 3,
                    *(volatile u32 *)(p + 0x33));
                cm_fn_00785790((void *)local, 4, (u32)(s32)first);
                nested = *(volatile u32 *)((volatile u8 *)object + 0x39);
                target = *(volatile u32 *)(nested + 0x53);
                cm_fn_00785790((void *)local, 5,
                    *(volatile u32 *)target);
                cm_fn_00785790((void *)local, 6, (u32)(s32)second);
                cm_fn_00785790((void *)local, 7, (u32)(s32)third);
                cm_fn_00785790((void *)local, 8, (u32)(s32)fourth);
                nested = *(volatile u32 *)((volatile u8 *)object + 0x39);
                u32 global = CM_GLOBAL(volatile u32, 0x00b7431cu);
                cm_fn_007861e0((void *)global, (u32)(unsigned long)local, nested);
            }
        }
    }
    u32 previous = frame.prev;
    CM_FS(u32, 0) = previous;
}
