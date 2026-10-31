#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00448110(u32 self, u32 src);

u32 CM_THISCALL cm_fn_004ab5b0(u32 self, u32 src)
{
    u32 value = *(volatile u32 *)src;
    *(volatile u32 *)self = value;
    cm_fn_00448110(self + 4u, src + 4u);
    return self;
}
