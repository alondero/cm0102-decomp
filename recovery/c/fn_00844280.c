#include "cmre.h"
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
u32 CM_STDCALL cm_fn_00844280(u32 argument)
{
    u32 self = CM_GLOBAL(volatile u32, 0x00ae1050);
    u32 result = cm_fn_004cf5a0(self, argument);
    if (!result) return 0;
    CM_GLOBAL(volatile u8, result + 0x4e) &= 0xfe;
    return (result & 0xffffff00u) | 1u;
}
