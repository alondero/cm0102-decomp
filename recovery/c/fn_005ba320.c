/* fn_005ba320 (VA 0x005ba320, image base 0x00400000, 3.9.68 primary image).
   Destructor-shaped routine recovered from the listing in the job bundle:
     - SEH frame { prev, handler = 0x00959918, state } linked in push order,
     - vtable store 0x0096a864 at [this],
     - frees of [this+0xa7], [this+0xa3], the [this+0xad] pointer table
       (count word at [this+0x3c]) and the table itself,
     - 0x49f450 / 0x944c94 on [this+8],
     - 0x518690 on this, then the SEH predecessor is restored.
   Every reload of [this+0x3c] and [this+0xad] is kept where the listing makes
   it (0x005ba384, 0x005ba39a, 0x005ba3a3) via volatile u32/s16 accesses. */
#include "cmre.h"

extern void CM_FASTCALL cm_fn_0049f450(u8 *param_1);
extern void CM_THISCALL cm_fn_00518690(void *self_arg);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

void CM_THISCALL cm_fn_005ba320(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;

    frame.state = 0xffffffffu;
    frame.handler = 0x00959918u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    CM_GLOBAL(volatile u32, self) = 0x0096a864u;
    u32 value = CM_GLOBAL(volatile u32, self + 0xa7u);
    frame.state = 0;
    if (value != 0)
        cm_fn_009452ca(value);

    value = CM_GLOBAL(volatile u32, self + 0xa3u);
    if (value != 0)
        cm_fn_009452ca(value);

    if (CM_GLOBAL(volatile u32, self + 0xadu) != 0) {
        s32 index = 0;
        while (index < (s32)CM_GLOBAL(volatile s16, self + 0x3cu)) {
            value = CM_GLOBAL(volatile u32,
                              CM_GLOBAL(volatile u32, self + 0xadu) + (u32)index * 4u);
            if (value != 0)
                cm_fn_009452ca(value);
            index = index + 1;
        }
        cm_fn_009452ca(CM_GLOBAL(volatile u32, self + 0xadu));
    }

    value = CM_GLOBAL(volatile u32, self + 8u);
    if (value != 0) {
        cm_fn_0049f450((u8 *)value);
        cm_fn_00944c94((void *)value);
    }

    frame.state = 0xffffffffu;
    cm_fn_00518690((void *)self);
    CM_FS(u32, 0) = frame.prev;
}
