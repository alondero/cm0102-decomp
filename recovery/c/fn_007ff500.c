/* Preferred-image VA 007ff500, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern void CM_THISCALL cm_fn_007fc690(u32 self);
extern void CM_THISCALL cm_fn_0077a890(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *pointer);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_THISCALL cm_fn_0077a7d0(u32 self, u32 status,
                                    u32 create_callback, u32 destroy_callback);

u32 CM_THISCALL cm_fn_007ff500(u32 self, volatile u32 status,
                              volatile u32 allocation)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 previous, first, second, old, object, result;
    previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0096110bu;
    frame.prev = previous;
    first = status;
    CM_FS(u32, 0) = (u32)&frame;
    second = allocation;
    D(self + 0x2627a0u) = first;
    old = D(self + 0x3840u);
    D(self + 0x2627a4u) = second;
    if (old != 0) {
        cm_fn_007fc690(self);
        old = D(self + 0x3840u);
        if (old != 0) {
            cm_fn_0077a890(old);
            cm_fn_00944c94((void *)old);
        }
        D(self + 0x3840u) = 0;
    }
    allocation = (u32)cm_fn_00944cf1(0xc3e54u);
    object = allocation;
    frame.state = 0;
    if (object != 0)
        object = cm_fn_0077a7d0(object, (u32)&status, 0x007ff630u, 0x007ff7b0u);
    result = status;
    frame.state = 0xffffffffu;
    D(self + 0x3840u) = object;
    if (result == 0) {
        if (object != 0) {
            cm_fn_0077a890(object);
            cm_fn_00944c94((void *)object);
            result = status;
        }
        D(self + 0x3840u) = 0;
    }
    CM_FS(u32, 0) = frame.prev;
    return result;
}
