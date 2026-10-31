#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 a, u32 b);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 a, u32 b);
extern unsigned long long CM_THISCALL cm_fn_007863f0(void *self, u32 a, u32 b);

void CM_STDCALL cm_fn_00758de0(volatile u8 *first, volatile u8 *second,
                               s8 arg3, s8 arg4, s16 arg5, s8 arg6)
{
    volatile struct { u32 prev; u32 handler; s32 state; } frame;
    volatile u8 local[224];
    u32 previous = CM_FS(u32, 0);
    frame.state = -1;
    frame.handler = 0x0095d93b;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    if (first == 0 || *(volatile u32 *)(first + 0x53) == 0 ||
        *(volatile u32 *)(first + 0xcf) == 0 || second == 0)
        goto done;

    cm_fn_0077bbe0((void *)local, 0x178e, 0);
    u32 v = *(volatile u32 *)first;
    frame.state = 0;
    cm_fn_00785790((void *)local, 0, v);
    u32 list = *(volatile u32 *)(first + 0xcf);
    if (list != 0) {
        v = *(volatile u32 *)list;
        cm_fn_00785790((void *)local, 1, v);
        u32 p = *(volatile u32 *)(list + 4);
        v = *(volatile u32 *)(p + 0x33);
        cm_fn_00785790((void *)local, 2, v);
        p = *(volatile u32 *)(list + 8);
        v = *(volatile u32 *)(p + 0x33);
        cm_fn_00785790((void *)local, 3, v);
        p = *(volatile u32 *)(list + 0xc);
        v = *(volatile u32 *)(p + 0x33);
    } else {
        cm_fn_00785790((void *)local, 1, 0xffffffffu);
        cm_fn_00785790((void *)local, 2, 0xffffffffu);
        cm_fn_00785790((void *)local, 3, 0xffffffffu);
        v = 0xffffffffu;
    }
    cm_fn_00785790((void *)local, 4, v);
    cm_fn_00785790((void *)local, 5, (u32)(s32)arg3);
    v = *(volatile u32 *)second;
    cm_fn_00785790((void *)local, 6, v);
    u32 p = *(volatile u32 *)(second + 4);
    v = *(volatile u32 *)(p + 0x33);
    cm_fn_00785790((void *)local, 7, v);
    p = *(volatile u32 *)(second + 8);
    v = *(volatile u32 *)(p + 0x33);
    cm_fn_00785790((void *)local, 8, v);
    p = *(volatile u32 *)(second + 0xc);
    v = *(volatile u32 *)(p + 0x33);
    cm_fn_00785790((void *)local, 9, v);
    cm_fn_00785790((void *)local, 10, (u32)(s32)arg4);
    cm_fn_00785790((void *)local, 11, (u32)(s32)arg5);
    p = *(volatile u32 *)(first + 0x53);
    v = p ? *(volatile u32 *)p : 0xffffffffu;
    cm_fn_00785790((void *)local, 12, v);
    cm_fn_00785790((void *)local, 13, (u32)(s32)arg6);

    p = *(volatile u32 *)(first + 0x53);
    if (p != 0 && *(volatile s16 *)(first + 0x80) > 0x128e) {
        u32 global = CM_GLOBAL(volatile u32, 0x00b7431c);
        cm_fn_007863f0((void *)global, (u32)local, p);
    } else {
        u32 global = CM_GLOBAL(volatile u32, 0x00b7431c);
        cm_fn_007861e0((void *)global, (u32)local, (u32)first);
    }
done:
    CM_FS(u32, 0) = frame.prev;
}
