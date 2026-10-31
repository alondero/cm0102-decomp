#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0058e340(u32, u32, u32, u32);

u32 CM_THISCALL cm_fn_00414300(u32 self, u32 first, u32 second, u32 third)
{
    u32 receiver = *(volatile u32 *)(self + 4u);
    return cm_fn_0058e340(receiver, second, third, first);
}
