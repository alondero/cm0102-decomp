#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0077bbe0(void *self, u32 a, u32 b);
extern u32 CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern u32 CM_THISCALL cm_fn_007861e0(void *self, u32 a, u32 b);

void CM_CDECL cm_fn_0058a070(volatile u8 *first, volatile u8 *second,
                             volatile u8 *third)
{
    volatile struct { u32 prev; u32 handler; s32 state; } frame;
    volatile u8 local[224];
    u32 previous = CM_FS(u32, 0);
    frame.state = -1;
    frame.handler = 0x00958c7b;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;

    if (first == 0 || *(volatile u32 *)(first + 0x61) == 0 ||
        *(volatile u32 *)(first + 0x1a) == 0 || second == 0 ||
        *(volatile u32 *)(second + 0x53) == 0)
        goto done;
    u32 p = *(volatile u32 *)(second + 0x69);
    if (p == 0 || *(volatile u32 *)(p + 0x38) == 0 ||
        *(volatile u32 *)(second + 0xcf) == 0 || third == 0)
        goto done;

    cm_fn_0077bbe0((void *)local, 0xbe3, 0);
    u32 v = *(volatile u32 *)first;
    frame.state = 0;
    cm_fn_00785790((void *)local, 0, v);
    p = *(volatile u32 *)(first + 4);
    v = *(volatile u32 *)(p + 0x33);
    cm_fn_00785790((void *)local, 1, v);
    p = *(volatile u32 *)(first + 8);
    v = *(volatile u32 *)(p + 0x33);
    cm_fn_00785790((void *)local, 2, v);
    p = *(volatile u32 *)(first + 0xc);
    v = *(volatile u32 *)(p + 0x33);
    cm_fn_00785790((void *)local, 3, v);
    v = *(volatile u32 *)second;
    cm_fn_00785790((void *)local, 4, v);
    p = *(volatile u32 *)(first + 0x61);
    s32 index_value = *(volatile s16 *)(p + 0xb) > 0x1c52 ? 2 : 1;
    cm_fn_00785790((void *)local, 5, (u32)index_value);
    u32 q = *(volatile u32 *)(second + 0xcf);
    p = q;
    v = *(volatile u32 *)p;
    cm_fn_00785790((void *)local, 6, v);
    p = *(volatile u32 *)(q + 4);
    v = *(volatile u32 *)(p + 0x33);
    cm_fn_00785790((void *)local, 7, v);
    p = *(volatile u32 *)(q + 8);
    v = *(volatile u32 *)(p + 0x33);
    cm_fn_00785790((void *)local, 8, v);
    p = *(volatile u32 *)(q + 0xc);
    v = *(volatile u32 *)(p + 0x33);
    cm_fn_00785790((void *)local, 9, v);
    v = *(volatile u32 *)third;
    cm_fn_00785790((void *)local, 10, v);
    u32 global = CM_GLOBAL(volatile u32, 0x00b7431c);
    cm_fn_007861e0((void *)global, (u32)local,
                   (u32)second);

done:
    CM_FS(u32, 0) = frame.prev;
}
