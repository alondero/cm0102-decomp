#include "cmre.h"
extern u8 CM_THISCALL cm_fn_005b8dc0(u32);
u8 CM_THISCALL cm_fn_006d5b20(u32 self, u32 flag)
{
    return cm_fn_005b8dc0(self + ((u8)flag ? 0xaac5u : 0x91e2u));
}

