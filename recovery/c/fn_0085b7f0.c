#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_009452ca(u32 allocation);

u32 CM_THISCALL cm_fn_0085b7f0(u32 self)
{
    u32 result = *(volatile u32 *)self;
    u32 second;
    if (result != 0)
        result = (u32)cm_fn_009452ca(result);
    second = *(volatile u32 *)(self + 0xc);
    if (second != 0)
        result = (u32)cm_fn_009452ca(second);
    return result;
}
