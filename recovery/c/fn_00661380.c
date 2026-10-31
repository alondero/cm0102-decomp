/* Preferred-image VA 00661380, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

extern u8 *CM_THISCALL cm_fn_008e84a0(u8 *self, u32 arg1, s32 arg2,
                                      s32 arg3, u32 arg4, u32 arg5);
extern u32 CM_THISCALL cm_fn_006613f0(u32 self);

u32 CM_THISCALL cm_fn_00661380(u32 self, u32 arg1, s32 arg2,
                              s32 arg3, u32 arg4, u32 arg5)
{
    volatile struct {
        u32 saved_object;
        struct { u32 prev, handler, state; } seh;
    } frame;
    u32 previous;

    frame.seh.state = 0xffffffffu;
    frame.seh.handler = 0x0095c058u;
    frame.seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame.seh;
    frame.saved_object = self;
    frame.saved_object = self;
    cm_fn_008e84a0((u8 *)self, arg1, arg2, arg3, arg4, arg5);
    frame.seh.state = 0;
    CM_GLOBAL(volatile u32, self) = 0x0096c96cu;
    (void)cm_fn_006613f0(self);
    previous = frame.seh.prev;
    CM_GLOBAL(volatile u8, self + 0x14u) = 40;
    CM_FS(u32, 0) = previous;
    return self;
}
