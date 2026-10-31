#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_00944c94(void *allocation);

u32 CM_THISCALL cm_fn_007293c0(u32 self)
{
    u32 value = *(volatile u32 *)self;
    if (value != 0) {
        value = (u32)cm_fn_00944c94((void *)value);
        *(volatile u32 *)self = 0;
        *(volatile u32 *)(self + 4) = 0;
    }
    return value;
}
