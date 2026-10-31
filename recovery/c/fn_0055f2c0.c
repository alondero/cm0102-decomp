#include "cmre.h"

extern void CM_CDECL cm_fn_005e42e0(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_005e2eb0(volatile u32, volatile u32, volatile u32, volatile u32);

u32 CM_THISCALL cm_fn_0055f2c0(u32 self)
{
    u32 value = *(volatile u32 *)(self + 0x12f4fau);
    if (value != 0)
        return value;
    *(volatile u32 *)(self + 0x12f4fau) = 1;
    cm_fn_005e42e0(0, 0, 0x31fu, 0x257u);
    return cm_fn_005e2eb0(0, 0, 0x31fu, 0x257u);
}
