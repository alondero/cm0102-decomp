#include "cmre.h"

/* 0058e340 returns whatever remains in EAX; forward that full register. */
extern u32 CM_THISCALL cm_fn_0058e340(u32, u32, u32, u32);

u32 CM_THISCALL cm_fn_004142c0(u32 self, u32 first, u32 second, u32 third)
{
    u32 receiver = *(volatile u32 *)self;
    return cm_fn_0058e340(receiver, second, third, first);
}
