#include "cmre.h"

extern u32 CM_CDECL cm_fn_005409b0(u32 *input);

u32 CM_STDCALL cm_fn_0076e070(u32 self)
{
    u32 pointer = *(volatile u32 *)(self + 0x24u);
    u32 result;
    if (pointer == 0)
        return 0xffu;
    result = cm_fn_005409b0((u32 *)pointer);
    if (result != 0)
        return (result & 0xffffff00u) | 2u;
    return (*(volatile u8 *)(self + 0x5eu) >> 3) & 1u;
}
