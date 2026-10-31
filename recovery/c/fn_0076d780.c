#include "cmre.h"

extern u32 CM_CDECL cm_fn_005409b0(u32 *input);

u32 CM_STDCALL cm_fn_0076d780(u32 input, u32 second)
{
    u32 found = cm_fn_005409b0((u32 *)input);
    u8 low;
    if (found == 0)
        low = second != 0;
    else
        low = second != 0 ? 1 : 2;
    return (second & 0xffffff00u) | low;
}
