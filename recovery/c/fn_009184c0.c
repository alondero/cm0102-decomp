#include "cmre.h"

extern void CM_THISCALL cm_fn_00687970(u8 *self, u8 *buffer);
extern void CM_FASTCALL cm_fn_0068d040(u32 *value);
extern void CM_FASTCALL cm_fn_0049f450(u32 *value);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 pointer);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 pointer);
extern void CM_FASTCALL cm_fn_00682300(u32 *self);

void CM_THISCALL cm_fn_009184c0(u32 *self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 index;
    u32 *children;
    u32 pointer;

    frame.state = 0xffffffffu;
    frame.handler = 0x00965978u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)(unsigned long)&frame;
    *(volatile u32 *)self = 0x00970cc4u;
    frame.state = 0u;
    (void)cm_fn_00687970((u8 *)self, (u8 *)0);

    pointer = *(volatile u32 *)((u8 *)self + 0xbau);
    if (pointer != 0u) {
        (void)cm_fn_009452ca(pointer);
        *(volatile u32 *)((u8 *)self + 0xbau) = 0u;
    }

    {
        s32 limit = *(volatile s32 *)((u8 *)self + 0x30u);
        index = 0u;
        if (limit >= 0) {
        do {
            children = *(u32 * volatile *)((u8 *)self + 0x0cu);
            pointer = *(volatile u32 *)&children[index];
            if (pointer != 0u) {
                u32 *vtable = (u32 *)(unsigned long)
                    *(volatile u32 *)(unsigned long)pointer;
                u32 target = *(volatile u32 *)vtable;
                ((void (CM_THISCALL *)(u32, u32))(unsigned long)target)(
                    pointer, 1u);
            }
            limit = *(volatile s32 *)((u8 *)self + 0x30u);
            index++;
        } while ((s32)index <= limit);
        }
    }

    pointer = *(volatile u32 *)((u8 *)self + 0x0cu);
    if (pointer != 0u) {
        (void)cm_fn_009452ca(pointer);
        *(volatile u32 *)((u8 *)self + 0x0cu) = 0u;
    }

    pointer = *(volatile u32 *)((u8 *)self + 0x10u);
    if (pointer != 0u) {
        cm_fn_0068d040((u32 *)(unsigned long)pointer);
        (void)cm_fn_00944c94(pointer);
    }
    pointer = *(volatile u32 *)((u8 *)self + 8u);
    if (pointer != 0u) {
        cm_fn_0049f450((u32 *)(unsigned long)pointer);
        (void)cm_fn_00944c94(pointer);
    }
    pointer = *(volatile u32 *)((u8 *)self + 0xf7u);
    if (pointer != 0u)
        (void)cm_fn_009452ca(pointer);
    pointer = *(volatile u32 *)((u8 *)self + 0xeeu);
    if (pointer != 0u)
        (void)cm_fn_009452ca(pointer);

    frame.state = 0xffffffffu;
    cm_fn_00682300(self);
    CM_FS(u32, 0) = frame.prev;
}
