#include "cmre.h"

extern u32 CM_STDCALL cm_fn_00944c3a(u32 socket);

u32 CM_THISCALL cm_fn_008ae340(volatile u32 *self, u32 index_arg)
{
    u32 base = *self;
    u32 offset, socket;
    if (!base)
        return 0;
    offset = (u32)(s32)(s16)index_arg * 4u;
    socket = *(volatile u32 *)(base + offset);
    if (socket == 0xffffffffu)
        return socket;
    cm_fn_00944c3a(socket);
    base = *self;
    *(volatile u32 *)(base + offset) = 0xffffffffu;
    return base;
}
