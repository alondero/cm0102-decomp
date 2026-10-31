/* Preferred-image VA 0x00765e30, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_FASTCALL cm_fn_00518640(u32 self);
extern void CM_FASTCALL cm_fn_0054a110(u32 self);
extern u32 CM_THISCALL cm_fn_0051fc00(u32 self, u32 flag);

u32 CM_THISCALL cm_fn_00765e30(u32 self, u32 argument)
{
    volatile struct { u32 prev, handler, state; } frame;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095da86u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_00518640(self);
    frame.state = 0;
    cm_fn_0054a110(self + 0xb2u);
    *(volatile u8 *)&frame.state = 1;
    CM_GLOBAL(volatile u32, self) = 0x0096d8acu;
    CM_GLOBAL(volatile u32, self + 4u) = argument;
    cm_fn_0051fc00(self, 1);
    CM_FS(u32, 0) = frame.prev;
    return self;
}
