#include "cmre.h"
extern u8 CM_THISCALL cm_fn_005b8960(u32);
u8 CM_THISCALL cm_fn_006d5af0(u32 self, u32 flag)
{
    return cm_fn_005b8960(self + ((u8)flag ? 0xaac5u : 0x91e2u));
}
