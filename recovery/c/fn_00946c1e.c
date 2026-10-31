#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_0094c827(volatile u8 *stream);

s32 CM_CDECL cm_fn_00946c1e(volatile u8 *stream, u32 output)
{
    unsigned long long result = cm_fn_0094c827(stream);
    u32 low;
    *(volatile u32 *)output = (u32)result;
    *(volatile u32 *)(output + 4u) = (u32)(result >> 32);
    low = *(volatile u32 *)output;
    return (low & *(volatile u32 *)(output + 4u)) == 0xffffffffu ? -1 : 0;
}
