#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_00684cd0(u32);
extern u32 CM_THISCALL cm_fn_0091a7e0(u32);
u32 CM_THISCALL cm_fn_0091a7b0(u32 self)
{
    u32 value, table;
    cm_fn_00684cd0(self);
    value = D(self + 0xfb);
    if (value) return value;
    table = D(self);
    value = ((u32 (CM_THISCALL *)(u32, u32, u32))D(table + 0x10))(self, 1, 0);
    if ((u8)value) value = cm_fn_0091a7e0(self);
    return value;
}

