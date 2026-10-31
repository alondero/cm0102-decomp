/* Preferred-image VA 0x00450c70, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0054a110(u32 self);

u32 CM_THISCALL cm_fn_00450c70(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 predecessor;

    frame.state = 0xffffffffu;
    frame.handler = 0x00955743u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    (void)cm_fn_0054a110(self);
    frame.state = 0;
    (void)cm_fn_0054a110(self + 0x18u);
    *(volatile u8 *)&frame.state = 1;
    (void)cm_fn_0054a110(self + 0x20u);
    predecessor = frame.prev;
    CM_FS(u32, 0) = predecessor;
    return self;
}
