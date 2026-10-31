#include "cmre.h"

typedef void (CM_THISCALL *dispose_element_fn)(u32 self, u32 mode);
extern void CM_THISCALL cm_fn_00687970(void *self, void *buffer);
extern void CM_FASTCALL cm_fn_0049f450(int *arg);
extern void CM_FASTCALL cm_fn_00682300(u32 *arg);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg);

void CM_THISCALL cm_fn_005f3600(void *self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 self_address = (u32)self;
    u32 value;
    u32 index;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095acb8u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    *(volatile u32 *)self_address = 0x0096b7b8u;
    frame.state = 0u;
    cm_fn_00687970(self, (void *)0);

    value = *(volatile u32 *)(self_address + 0xbau);
    if (value != 0u) {
        cm_fn_009452ca(value);
        *(volatile u32 *)(self_address + 0xbau) = 0u;
    }
    value = *(volatile u32 *)(self_address + 0xeeu);
    if (value != 0u)
        cm_fn_009452ca(value);

    value = *(volatile u32 *)(self_address + 0x30u);
    index = 0u;
    if ((s32)value >= 0) {
        do {
            u32 array = *(volatile u32 *)(self_address + 0x0cu);
            u32 element = *(volatile u32 *)(array + index * 4u);
            if (element != 0u) {
                u32 vtable = *(volatile u32 *)element;
                dispose_element_fn slot = (dispose_element_fn)*(volatile u32 *)vtable;
                slot(element, 1u);
            }
            value = *(volatile u32 *)(self_address + 0x30u);
            ++index;
        } while ((s32)index <= (s32)value);
    }

    value = *(volatile u32 *)(self_address + 0x0cu);
    if (value != 0u) {
        cm_fn_009452ca(value);
        *(volatile u32 *)(self_address + 0x0cu) = 0u;
    }
    value = *(volatile u32 *)(self_address + 8u);
    if (value != 0u) {
        cm_fn_0049f450((int *)value);
        cm_fn_00944c94((void *)value);
    }
    frame.state = 0xffffffffu;
    cm_fn_00682300((u32 *)self_address);
    CM_FS(u32, 0) = frame.prev;
}
