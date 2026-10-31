/* Preferred-image VA 0x00682300, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern void CM_STDCALL cm_fn_009462bb(u32 base, u32 size, u32 count, u32 destructor);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 address);
extern unsigned long long CM_THISCALL cm_fn_0048c650(u32 self);

unsigned long long CM_THISCALL cm_fn_00682300(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 value, count;
    unsigned long long result;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095c954u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    D(self) = 0x0096d020u;
    value = D(self + 0xeau);
    frame.state = 2;
    if (value) {
        count = D(value - 4u);
        cm_fn_009462bb(value, 16, count, 0x005a53c0u);
        cm_fn_00944c94(value - 4u);
    }
    frame.state = 0xffffffffu;
    result = cm_fn_0048c650(self);
    CM_FS(u32, 0) = frame.prev;
    return result;
}
