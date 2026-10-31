/* Preferred-image VA 00667100, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0054a110(u32 self);

u32 CM_THISCALL cm_fn_00667100(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 predecessor;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095c32bu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    (void)cm_fn_0054a110(self + 0x2au);
    frame.state = 0;
    (void)cm_fn_0054a110(self + 0x39u);
    predecessor = frame.prev;
    CM_FS(u32, 0) = predecessor;
    return self;
}
