/* Preferred-image VA 0x008e56a0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern void CM_FASTCALL cm_fn_0054a110(u32 self);
extern u32 CM_THISCALL cm_fn_008e5d60(u32 self, u32 a, u32 b, u32 c,
                                      u32 d, u32 e, u32 f);

u32 CM_THISCALL cm_fn_008e56a0(u32 self, u32 a, u32 b, u32 c, u32 d, u32 e)
{
    volatile struct { u32 prev, handler, state; } frame;
    frame.state = 0xffffffffu;
    frame.handler = 0x009648ebu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0054a110(self + 0x12u);
    frame.state = 0;
    cm_fn_008e5d60(self, a, b, c, d, e, 0);
    CM_FS(u32, 0) = frame.prev;
    return self;
}
