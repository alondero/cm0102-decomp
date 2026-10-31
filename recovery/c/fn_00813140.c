/* Preferred-image VA 0x00813140, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern void CM_FASTCALL cm_fn_0068d040(u32 self);
extern unsigned long long CM_STDCALL cm_fn_009462bb(u32 base, u32 size, u32 count, u32 destructor);

unsigned long long CM_THISCALL cm_fn_00813140(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    unsigned long long result;
    frame.state = 0xffffffffu;
    frame.handler = 0x00961246u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    frame.state = 1;
    cm_fn_0068d040(self + 0x361eu);
    *(volatile u8 *)&frame.state = 0;
    cm_fn_009462bb(self + 0x1b0eu, 0x2b0u, 10, 0x007ae4f0u);
    frame.state = 0xffffffffu;
    result = cm_fn_009462bb(self + 1u, 0x2b0u, 10, 0x007ae4f0u);
    CM_FS(u32, 0) = frame.prev;
    return result;
}
