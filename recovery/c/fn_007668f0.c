/* Preferred-image VA 0x007668f0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern void CM_THISCALL cm_fn_00687970(u32 self, u32 buffer);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 value);
extern void CM_FASTCALL cm_fn_0049f450(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 value);
extern u32 CM_FASTCALL cm_fn_00682300(u32 self);

u32 CM_THISCALL cm_fn_007668f0(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 value, result;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095db56u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    D(self) = 0x0096d94cu;
    frame.state = 1;
    cm_fn_00687970(self, 0);
    value = D(self + 0xba);
    if (value) cm_fn_009452ca(value);
    value = D(self + 8);
    if (value) {
        cm_fn_0049f450(value);
        cm_fn_00944c94(value);
    }
    frame.state = 0xffffffffu;
    result = cm_fn_00682300(self);
    CM_FS(u32, 0) = frame.prev;
    return result;
}
