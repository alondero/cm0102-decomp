/* WIP: fault address differs at case 0 after five failed test iterations. */
#include "cmre.h"

extern void CM_THISCALL cm_fn_00417c10(u32 *self, u32 *arg1, u16 *arg2);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *self, u32 first, u32 second);
extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg1);
typedef u16 *(CM_THISCALL *cm_slot_3c)(void *self, u32 arg1, u32 arg2, u32 arg3, u32 arg4);

u32 CM_THISCALL cm_fn_0048f340(void *self, volatile u32 *out)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 local_area[40];
    volatile u8 *p = (volatile u8 *)self;
    u8 *vtable;
    cm_slot_3c slot;
    u16 *found;
    u32 result_value;
    u32 out0;
    u32 out1;

    frame.state = 0xffffffff;
    frame.handler = 0x00956108;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    *(volatile u32 *)(local_area + 12) = 0;
    cm_fn_0054a110((u16 *)(local_area + 16));
    vtable = *(u8 * volatile *)p;
    frame.state = 0;
    slot = *(cm_slot_3c volatile *)(vtable + 0x3c);
    found = slot(self, 0xffffffff, (u32)(local_area + 10),
                 (u32)(local_area + 8), (u32)(local_area + 12));
    if (found != (u16 *)0) {
        if (*(volatile u32 *)(local_area + 12) != 0) {
            u32 second_word = *(volatile u16 *)((volatile u8 *)found + 2);
            u32 first = *(volatile u16 *)found;
            u16 base = *(volatile u16 *)(p + 0x40);
            result_value = (u32)cm_fn_00549ef0((void *)(local_area + 24), first,
                                               (u16)(second_word + base));
            cm_fn_00417c10((u32 *)(local_area + 16),
                           (u32 *)(local_area + 32), (u16 *)result_value);
        } else {
            s16 delta = *(volatile s8 *)((volatile u8 *)found + 5);
            u16 base = *(volatile u16 *)(p + 0x40);
            u32 first = *(volatile u16 *)((volatile u8 *)found + 3);
            result_value = (u32)cm_fn_00549ef0((void *)(local_area + 32), first,
                                               (u16)(delta + base));
            cm_fn_00417c10((u32 *)(local_area + 16),
                           (u32 *)(local_area + 24), (u16 *)result_value);
        }
        cm_fn_009452ca((u32)found);
    }
    out0 = *(volatile u32 *)(local_area + 16);
    out1 = *(volatile u32 *)(local_area + 20);
    out[0] = out0;
    out[1] = out1;
    CM_FS(u32, 0) = frame.prev;
    return (u32)out;
}
