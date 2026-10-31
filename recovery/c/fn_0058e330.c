#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
u32 CM_THISCALL cm_fn_0058e330(u32 self)
{
    u32 value = CM_GLOBAL(volatile u32, self);
    if (value) return (u32)cm_fn_009452ca(value);
    return 0;
}
