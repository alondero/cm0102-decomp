#include "cmre.h"

extern void CM_FASTCALL cm_fn_0049f450(u32 self);
extern void CM_THISCALL cm_fn_00518690(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 value);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 value);

void CM_THISCALL cm_fn_00410840(u8 *self)
{
    volatile struct { u32 prev, handler, state; } frame;
    frame.state = 0xffffffffu;
    frame.handler = 0x009539a8u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    *(volatile u32 *)self = 0x009676c8u;
    u32 value = *(volatile u32 *)(self + 0xa7);
    frame.state = 0;
    if (value != 0)
        (void)cm_fn_009452ca(value);

    value = *(volatile u32 *)(self + 0xa3);
    if (value != 0)
        (void)cm_fn_009452ca(value);

    u32 array = *(volatile u32 *)(self + 0xad);
    if (array != 0) {
        u32 index = 0;
        s16 initial_count = *(volatile s16 *)(self + 0x3c);
        if (initial_count > 0) {
            do {
                array = *(volatile u32 *)(self + 0xad);
                value = *(volatile u32 *)(array + index * 4u);
                if (value != 0)
                    (void)cm_fn_009452ca(value);
                s16 count = *(volatile s16 *)(self + 0x3c);
                index++;
                if ((s32)index >= count)
                    break;
            } while (1);
        }
        array = *(volatile u32 *)(self + 0xad);
        (void)cm_fn_009452ca(array);
    }

    u32 child = *(volatile u32 *)(self + 8);
    if (child != 0) {
        cm_fn_0049f450(child);
        (void)cm_fn_00944c94(child);
    }

    frame.state = 0xffffffffu;
    cm_fn_00518690((u32)self);
    CM_FS(u32, 0) = frame.prev;
}
