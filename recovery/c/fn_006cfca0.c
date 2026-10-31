#include "cmre.h"
extern u8 CM_THISCALL cm_fn_006e9540(u32, u32, u32, u32, u32);
u8 CM_THISCALL cm_fn_006cfca0(u32 self, u32 a, u32 b, u32 c, u32 d)
{
    return cm_fn_006e9540(self + 0xf5b2u, a, b, c, d);
}
