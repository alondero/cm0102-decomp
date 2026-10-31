#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0062ed70(u32 self, u32 arg);

u32 CM_STDCALL cm_fn_00631550(u32 arg)
{
    u32 self = CM_GLOBAL(volatile u32, 0x00b63d28);
    u32 result = cm_fn_0062ed70(self, arg);
    u32 low = 0;
    if (result != 0 && *(volatile u8 *)result == 0x51u)
        low = 1;
    return (result & 0xffffff00u) | low;
}
