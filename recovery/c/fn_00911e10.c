#include "cmre.h"

extern void CM_FASTCALL cm_fn_0049f450(u32 object);
extern void CM_FASTCALL cm_fn_00682300(u32 self);
extern void CM_THISCALL cm_fn_00687970(u32 self, u32 argument);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 argument);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 argument);

typedef void (CM_THISCALL *cm_slot0_fn)(u32 self, u32 argument);

void CM_THISCALL cm_fn_00911e10(u8 *self)
{
    volatile struct {
        void *prev;
        void *handler;
        s32 state;
    } frame;
    frame.state = -1;
    frame.handler = (void *)0x00965728u;
    void *previous = CM_FS(void *, 0);
    frame.prev = previous;
    CM_FS(void *, 0) = (void *)&frame;

    *(volatile u32 *)self = 0x00970b70u;
    frame.state = 0;
    cm_fn_00687970((u32)self, 0);

    u32 value = *(volatile u32 *)(self + 0xba);
    if (value != 0) {
        (void)cm_fn_009452ca(value);
        *(volatile u32 *)(self + 0xba) = 0;
    }

    value = *(volatile u32 *)(self + 0xef);
    if (value != 0)
        (void)cm_fn_009452ca(value);

    s32 index = 0;
    s32 limit = *(volatile s32 *)(self + 0x30);
    if (limit >= 0) {
        do {
            u32 list = *(volatile u32 *)(self + 0x0c);
            u32 element = *(volatile u32 *)(list + (u32)index * 4u);
            if (element != 0) {
                u32 vtable = *(volatile u32 *)element;
                u32 target = *(volatile u32 *)(vtable + 0);
                ((cm_slot0_fn)target)(element, 1);
            }
            limit = *(volatile s32 *)(self + 0x30);
            index = (s32)((u32)index + 1u);
        } while (index <= limit);
    }

    value = *(volatile u32 *)(self + 0x0c);
    if (value != 0) {
        (void)cm_fn_009452ca(value);
        *(volatile u32 *)(self + 0x0c) = 0;
    }

    u32 object = *(volatile u32 *)(self + 8);
    if (object != 0) {
        cm_fn_0049f450(object);
        (void)cm_fn_00944c94(object);
    }

    value = *(volatile u32 *)(self + 0xf7);
    if (value != 0)
        (void)cm_fn_009452ca(value);

    frame.state = -1;
    cm_fn_00682300((u32)self);
    CM_FS(void *, 0) = frame.prev;
}
