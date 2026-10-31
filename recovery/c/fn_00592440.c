/* WIP: one official test invocation; O2 mismatch on case 196 HeapFree events. No evidence-backed source correction identified. */
#include "cmre.h"

extern void CM_FASTCALL cm_fn_0049f450(u32 self);
extern void CM_THISCALL cm_fn_00518690(void *self_arg);
extern void CM_CDECL cm_fn_00944c94(void *arg4);
extern void CM_CDECL cm_fn_009452ca(u32 arg4);

void CM_THISCALL cm_fn_00592440(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    frame.state = 0xffffffffu;
    frame.handler = 0x00958ed8u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    CM_GLOBAL(volatile u32, self) = 0x0096a360u;
    u32 value = CM_GLOBAL(volatile u32, self + 0xa7u);
    frame.state = 0;
    if (value != 0)
        cm_fn_009452ca(value);

    value = CM_GLOBAL(volatile u32, self + 0xa3u);
    if (value != 0)
        cm_fn_009452ca(value);

    u32 array = CM_GLOBAL(volatile u32, self + 0xadu);
    if (array != 0) {
        u32 index = 0;
        s16 initial_count = CM_GLOBAL(volatile s16, self + 0x3cu);
        if (initial_count > 0) {
            do {
                array = CM_GLOBAL(volatile u32, self + 0xadu);
                value = CM_GLOBAL(volatile u32, array + index * 4u);
                if (value != 0)
                    cm_fn_009452ca(value);
                s16 count = CM_GLOBAL(volatile s16, self + 0x3cu);
                index++;
                if ((s32)index >= count)
                    break;
            } while (1);
        }
        array = CM_GLOBAL(volatile u32, self + 0xadu);
        cm_fn_009452ca(array);
    }

    u32 child = CM_GLOBAL(volatile u32, self + 8u);
    if (child != 0) {
        cm_fn_0049f450(child);
        cm_fn_00944c94((void *)child);
    }

    frame.state = 0xffffffffu;
    cm_fn_00518690((void *)self);
    u32 restore_prev = frame.prev;
    CM_FS(u32, 0) = restore_prev;
}