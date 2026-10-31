#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007e82e0(u32 self, u32 key, u32 create);

u32 CM_THISCALL cm_fn_007e8590(u32 self, u32 key)
{
    u32 result = cm_fn_007e82e0(self, key, 0);
    if (result != 0)
        *(volatile u16 *)(result + 6) = 0xffffu;
    return result;
}
