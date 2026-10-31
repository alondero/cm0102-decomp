/* Preferred-image VA 0x0077a7d0; image base 0x00400000; pinned target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

typedef struct {
    u32 prev;
    u32 handler;
    s32 state;
} cm_seh_0077a7d0;

extern u32 CM_THISCALL cm_fn_008ae180(u32 self, u16 count_arg);
extern u32 CM_FASTCALL cm_fn_008ae260(u32 self);
extern u32 CM_THISCALL cm_fn_008aea80(u32 self, u16 code);

u32 CM_THISCALL cm_fn_0077a7d0(u32 self, u32 param_2, u32 param_3, u32 param_4)
{
    volatile cm_seh_0077a7d0 frame;
    u32 i;
    u32 status;

    frame.state = -1;
    frame.handler = 0x0095e068u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    cm_fn_008ae180(self, 0x10);
    *(volatile u16 *)(self + 0xc3a6au) = 0;
    *(volatile u32 *)(self + 0xc3a5eu) = 0;
    *(volatile u32 *)(self + 0xc3a5au) = 0;
    frame.state = 0;
    for (i = 0; i < 8; i++)
        *(volatile u32 *)(self + 0x4bau + i * 4u) = 0xffffffffu;
    *(volatile u32 *)(self + 0xc3a66u) = param_4;
    *(volatile u32 *)(self + 0xc3a62u) = param_3;
    *(volatile u32 *)param_2 = 1;
    status = cm_fn_008ae260(self);
    if (status == 0)
        *(volatile u32 *)param_2 = 0;
    else
        cm_fn_008aea80(self, 0x1a0bu);
    CM_FS(u32, 0) = frame.prev;
    return self;
}
