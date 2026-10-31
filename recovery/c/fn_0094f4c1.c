#include "cmre.h"

u32 CM_CDECL cm_fn_0094f4c1(u32 address)
{
    u32 target = CM_GLOBAL(volatile u32, 0x0096706c);
    u32 (CM_STDCALL *is_bad)(u32) = (u32 (CM_STDCALL *)(u32))target;
    return is_bad(address) == 0;
}
