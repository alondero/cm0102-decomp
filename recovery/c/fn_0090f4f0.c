/* Preferred-image VA 0x0090F4F0; image base 0x00400000; pinned target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

typedef struct {
    void *prev;
    void *handler;
    s32 state;
} cm_seh_0090f4f0;

typedef void (CM_THISCALL *cm_child_callback_0090f4f0)(u32 self, u32 arg);

extern void CM_FASTCALL cm_fn_0049f450(u32 *buffer);
extern void CM_THISCALL cm_fn_00518690(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 buffer);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 value);

void CM_THISCALL cm_fn_0090f4f0(u32 self)
{
    volatile cm_seh_0090f4f0 frame;
    u32 value;

    frame.state = -1;
    frame.handler = (void *)0x00965698u;
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;

    /* Save-self local, saved ESI/EDI, then install the original vtable. */
    value = self;
    *(volatile u32 *)(self + 0) = 0x00970ad0u;
    value = *(volatile u32 *)(self + 0xa7);
    frame.state = 0;
    if (value != 0)
        (void)cm_fn_009452ca(value);

    value = *(volatile u32 *)(self + 0xa3);
    if (value != 0)
        (void)cm_fn_009452ca(value);

    value = *(volatile u32 *)(self + 0xad);
    if (value != 0) {
        u32 index = 0;
        s16 limit = *(volatile s16 *)(self + 0x3c);
        if (limit > 0) {
            for (;;) {
                u32 base = *(volatile u32 *)(self + 0xad);
                u32 item = *(volatile u32 *)(base + index * 4u);
                if (item != 0)
                    (void)cm_fn_009452ca(item);
                limit = *(volatile s16 *)(self + 0x3c);
                ++index;
                if ((s32)index >= (s32)limit)
                    break;
            }
        }
        (void)cm_fn_009452ca(*(volatile u32 *)(self + 0xad));
    }

    {
        s32 count = *(volatile s32 *)(self + 0x30);
        u32 index = 0;
        if (count >= 0) {
            for (;;) {
                u32 table = *(volatile u32 *)(self + 0x0c);
                u32 child = *(volatile u32 *)(table + index * 4u);
                if (child != 0) {
                    u32 vtable = *(volatile u32 *)child;
                    u32 target = *(volatile u32 *)vtable;
                    ((cm_child_callback_0090f4f0)target)(child, 1);
                }
                count = *(volatile s32 *)(self + 0x30);
                ++index;
                if ((s32)index > count)
                    break;
            }
        }
    }

    value = *(volatile u32 *)(self + 0x0c);
    if (value != 0) {
        (void)cm_fn_009452ca(value);
        *(volatile u32 *)(self + 0x0c) = 0;
    }

    value = *(volatile u32 *)(self + 8);
    if (value != 0) {
        cm_fn_0049f450((u32 *)value);
        (void)cm_fn_00944c94(value);
    }

    (void)cm_fn_009452ca(*(volatile u32 *)(self + 0xb2));
    frame.state = -1;
    cm_fn_00518690(self);
    value = (u32)frame.prev;
    CM_FS(void *, 0) = (void *)value;
}
