/* Preferred-image VA 0x0058cea0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern void CM_FASTCALL cm_fn_0054a110(u32 self);
extern u32 CM_FASTCALL cm_fn_0054cfb0(u32 self);
extern void CM_THISCALL cm_fn_00417c10(u32 self, u32 local, u32 source);
extern u32 CM_THISCALL cm_fn_0058d7a0(u32 self, u32 argument);
extern void CM_FASTCALL cm_fn_0058dd50(u32 self);

u32 CM_THISCALL cm_fn_0058cea0(u32 self, u32 argument)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[8];
    u32 result;
    frame.state = 0xffffffffu;
    frame.handler = 0x00958e0bu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0054a110(self + 5u);
    frame.state = 0;
    result = cm_fn_0054cfb0(0x00dd6ec8u);
    CM_GLOBAL(volatile u8, self + 4u) = (u8)result;
    cm_fn_00417c10(self + 5u, (u32)scratch, 0x00dd6ec8u);
    if (!argument || !cm_fn_0058d7a0(self, argument))
        cm_fn_0058dd50(self);
    CM_FS(u32, 0) = frame.prev;
    return self;
}
