#include "cmre.h"

extern void CM_FASTCALL cm_fn_0049f450(int *arg);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 arg);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg);
extern void CM_THISCALL cm_fn_00518690(void *self);

void CM_THISCALL cm_fn_0092b6a0(void *self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 pointer;
    u32 count;
    u32 index;
    u32 cleanup_self = (u32)self;

    frame.state = 0xffffffffu;
    frame.handler = 0x00965f58u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    *(volatile u32 *)((u32)self + 0u) = 0x00971250u;

    pointer = *(volatile u32 *)((u32)self + 0xa7u);
    frame.state = 0u;
    if (pointer != 0u) cm_fn_009452ca(pointer);
    pointer = *(volatile u32 *)((u32)self + 0xa3u);
    if (pointer != 0u) cm_fn_009452ca(pointer);
    pointer = *(volatile u32 *)((u32)self + 0xadu);
    if (pointer != 0u) {
        count = *(volatile u16 *)((u32)self + 0x3cu);
        if ((s16)count > 0) {
            index = 0u;
            do {
                pointer = *(volatile u32 *)((u32)self + 0xadu);
                pointer = *(volatile u32 *)(pointer + index * 4u);
                if (pointer != 0u) cm_fn_009452ca(pointer);
                count = *(volatile u16 *)((u32)self + 0x3cu);
                ++index;
            } while ((s32)index < (s16)count);
        }
        pointer = *(volatile u32 *)((u32)self + 0xadu);
        cm_fn_009452ca(pointer);
    }

    pointer = *(volatile u32 *)((u32)self + 8u);
    if (pointer != 0u) {
        cm_fn_0049f450((int *)pointer);
        cm_fn_00944c94(pointer);
    }
    frame.state = 0xffffffffu;
    cm_fn_00518690((void *)cleanup_self);
    CM_FS(u32, 0) = frame.prev;
}
