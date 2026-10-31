#include "cmre.h"

extern u32 CM_THISCALL cm_fn_006f9f90(u32 self, u32 a, u32 b, u32 c,
                                      u32 d, u32 e, u32 f);

u8 CM_THISCALL cm_fn_006f4650(volatile u8 *self)
{
    u8 side = self[0x27];
    u8 state = self[0x103];
    u32 target = side ? 0 : 11;
    if (state == target) {
        s8 value = (s8)self[0x102];
        if (value >= 3 && value <= 5)
            return 1;
    }
    return (u8)cm_fn_006f9f90((u32)self, 4, target, 0, 0, 0, 0);
}
