#include "cmre.h"

extern u32 CM_CDECL cm_fn_0053b900(u32 *input);

u32 CM_STDCALL cm_fn_00775790(u32 input)
{
    u32 value;
    if (cm_fn_0053b900((u32 *)input) != 0)
        return 1;
    value = *(volatile u32 *)input;
    if (value == CM_GLOBAL(volatile u32, 0x009d00a4))
        return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d00a8))
        return 1;
    return 0;
}
