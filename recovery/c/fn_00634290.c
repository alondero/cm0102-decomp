#include "cmre.h"

extern void CM_FASTCALL cm_fn_0049f450(int *param_1);
extern void CM_THISCALL cm_fn_00518690(void *self_arg);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

void CM_THISCALL cm_fn_00634290(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095b358u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    *(volatile u32 *)self = 0x0096bc2cu;
    u32 pointer = *(volatile u32 *)(self + 0xa7u);
    frame.state = 0u;
    if (pointer != 0)
        cm_fn_009452ca(pointer);

    pointer = *(volatile u32 *)(self + 0xa3u);
    if (pointer != 0)
        cm_fn_009452ca(pointer);

    u32 array = *(volatile u32 *)(self + 0xadu);
    if (array != 0) {
        u32 index = 0;
        s32 count = *(volatile s16 *)(self + 0x3cu);
        if (count > 0) {
            do {
                u32 current_array = *(volatile u32 *)(self + 0xadu);
                pointer = *(volatile u32 *)(current_array + index * 4u);
                if (pointer != 0)
                    cm_fn_009452ca(pointer);
                count = *(volatile s16 *)(self + 0x3cu);
                ++index;
            } while ((s32)index < count);
        }
        cm_fn_009452ca(*(volatile u32 *)(self + 0xadu));
    }

    u32 child = *(volatile u32 *)(self + 8u);
    if (child != 0) {
        cm_fn_0049f450((int *)child);
        cm_fn_00944c94(child);
    }

    frame.state = 0xffffffffu;
    cm_fn_00518690((void *)self);
    CM_FS(u32, 0) = frame.prev;
}

