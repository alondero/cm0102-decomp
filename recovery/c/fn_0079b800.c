#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg);

u32 CM_THISCALL cm_fn_0079b800(u32 self)
{
    u32 value = *(volatile u32 *)(self + 8u);
    u32 result;
    if (value != 0) {
        cm_fn_009452ca(value);
        *(volatile u32 *)(self + 8u) = 0;
    }
    value = *(volatile u32 *)(self + 12u);
    if (value != 0) {
        cm_fn_009452ca(value);
        *(volatile u32 *)(self + 12u) = 0;
    }
    value = *(volatile u32 *)self;
    if (value == 0)
        return 0;
    result = (u32)cm_fn_009452ca(value);
    *(volatile u32 *)self = 0;
    return result;
}
