#include "cmre.h"
extern u32 CM_THISCALL cm_fn_006ea760(u32, u32, u32, u32, u32);
u32 CM_THISCALL cm_fn_006cfd70(u32 self, u32 a, u32 b, u32 c, u32 d)
{
    return cm_fn_006ea760(self + 0xf5b2u, a, b, c, d);
}

