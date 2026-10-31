#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_009452ca(u32 allocation);

u32 CM_THISCALL cm_fn_00785d00(u32 self)
{
    u32 value = *(volatile u32 *)self;
    if (value != 0) {
        cm_fn_009452ca(value);
        *(volatile u32 *)self = 0;
        *(volatile u32 *)(self + 4) = 0;
    }
    value = *(volatile u32 *)(self + 8);
    if (value != 0) {
        value = (u32)cm_fn_009452ca(value);
        *(volatile u32 *)(self + 8) = 0;
        *(volatile u32 *)(self + 0xc) = 0;
    }
    return value;
}
