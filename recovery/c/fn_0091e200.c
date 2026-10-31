#include "cmre.h"

typedef void (CM_THISCALL *dispose_element_fn)(u32 self, u32 mode);
extern void CM_THISCALL cm_fn_00687970(void *self, void *buffer);
extern void CM_FASTCALL cm_fn_0049f450(int *arg);
extern void CM_FASTCALL cm_fn_00682300(u32 *arg);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg);

void CM_THISCALL cm_fn_0091e200(void *self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 pointer;
    u32 count;
    u32 index;
    u32 cleanup_self = (u32)self;

    frame.state = 0xffffffffu;
    frame.handler = 0x00965ad8u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    *(volatile u32 *)((u32)self + 0u) = 0x00970d78u;
    frame.state = 0u;
    cm_fn_00687970(self, (void *)0);

    pointer = *(volatile u32 *)((u32)self + 0xbau);
    if (pointer != 0u) {
        cm_fn_009452ca(pointer);
        *(volatile u32 *)((u32)self + 0xbau) = 0u;
    }
    pointer = *(volatile u32 *)((u32)self + 0xefu);
    if (pointer != 0u) cm_fn_009452ca(pointer);

    count = *(volatile u32 *)((u32)self + 0x30u);
    index = 0u;
    if ((s32)count >= 0) {
        do {
            pointer = *(volatile u32 *)((u32)self + 0x0cu);
            pointer = *(volatile u32 *)(pointer + index * 4u);
            if (pointer != 0u) {
                u32 vtable = *(volatile u32 *)pointer;
                ((dispose_element_fn)*(volatile u32 *)vtable)(pointer, 1u);
            }
            count = *(volatile u32 *)((u32)self + 0x30u);
            ++index;
        } while ((s32)index <= (s32)count);
    }

    pointer = *(volatile u32 *)((u32)self + 0x0cu);
    if (pointer != 0u) {
        cm_fn_009452ca(pointer);
        *(volatile u32 *)((u32)self + 0x0cu) = 0u;
    }
    pointer = *(volatile u32 *)((u32)self + 8u);
    if (pointer != 0u) {
        cm_fn_0049f450((int *)pointer);
        cm_fn_00944c94((void *)pointer);
    }
    frame.state = 0xffffffffu;
    cm_fn_00682300((u32 *)cleanup_self);
    CM_FS(u32, 0) = frame.prev;
}
