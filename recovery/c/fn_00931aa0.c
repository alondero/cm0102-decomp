#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_009452ca(u32 pointer);

u32 CM_THISCALL cm_fn_00931aa0(volatile u8 *self, volatile s16 *key)
{
    u32 value = *(volatile u16 *)(self + 0xc);
    u32 result;
    if ((s32)*key != (s32)value)
        return value;
    result = *(volatile u32 *)self;
    if (result) {
        result = (u32)cm_fn_009452ca(result);
        *(volatile u32 *)self = 0;
    }
    *(volatile u32 *)(self + 4) = 0;
    *(volatile u32 *)(self + 8) = 0;
    return result;
}
