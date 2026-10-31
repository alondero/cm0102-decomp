#include "cmre.h"

extern u8 CM_THISCALL cm_fn_006e9820(u32 self, u32 a1, u32 a2, u32 a3,
                                      u32 a4, u32 a5, u32 a6, u32 a7);

u8 CM_THISCALL cm_fn_006cfc60(u32 self, u32 a1, u32 a2, u32 a3,
                              u32 a4, u32 a5, u32 a6, u32 a7)
{
    return cm_fn_006e9820(self + 0xf5b2u, a1, a2, a3, a4, a5, a6, a7);
}
