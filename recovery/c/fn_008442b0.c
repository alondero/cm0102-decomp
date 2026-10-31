#include "cmre.h"

extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);

u32 CM_STDCALL cm_fn_008442b0(u32 argument)
{
    u32 self = CM_GLOBAL(volatile u32, 0x00ae1050);
    u32 result = cm_fn_004cf5a0(self, argument);
    if (result != 0) {
        u8 value = *(volatile u8 *)(result + 0x4eu);
        *(volatile u8 *)(result + 0x4eu) = value | 2u;
    }
    return result;
}
