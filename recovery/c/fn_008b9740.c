/* Preferred-image VA 0x008b9740, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern void CM_FASTCALL cm_fn_0068d040(u32 self);
extern unsigned long long CM_STDCALL cm_fn_009462bb(u32 base, u32 size, u32 count, u32 destructor);

unsigned long long CM_THISCALL cm_fn_008b9740(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    unsigned long long result;
    frame.state = 0xffffffffu;
    frame.handler = 0x009638e8u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    frame.state = 0;
    cm_fn_0068d040(self + 0x80au);
    frame.state = 0xffffffffu;
    result = cm_fn_009462bb(self + 0xfau, 0x9bu, 11, 0x005a53c0u);
    CM_FS(u32, 0) = frame.prev;
    return result;
}
