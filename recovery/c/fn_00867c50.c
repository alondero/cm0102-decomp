/* Preferred-image VA 00867c50, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole indexed caller004E25A2 next calls007FF010, whose first instruction
   overwrites full EAX before use. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern void CM_THISCALL cm_fn_0043f580(u32 value, u32 flags);

void CM_THISCALL cm_fn_00867c50(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u32 value;
    u32 global, flags, prior, result;
    frame.state = 0xffffffffu;
    frame.handler = 0x00962ae8u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    value = 0;
    frame.state = 0;
    global = D(0x00ae3138u);
    flags = D(global + 4u);
    if (flags & 0x800000u) {
        value = D(self + 0xcu) * 52u;
        cm_fn_0043f580((u32)&value, 4);
        result = value;
        D(self + 0xcu) = (u32)((s32)result / 52);
        prior = frame.prev;
    } else if (flags & 0x400000u) {
        value = D(self + 0xcu) * 4u;
        cm_fn_0043f580((u32)&value, 8);
        result = value;
        D(self + 0xcu) = (u32)((s32)result / 4);
        prior = frame.prev;
    } else {
        value = D(self + 0xcu);
        cm_fn_0043f580((u32)&value, 8);
        result = value;
        prior = frame.prev;
        D(self + 0xcu) = result;
    }
    CM_FS(u32, 0) = prior;
}
