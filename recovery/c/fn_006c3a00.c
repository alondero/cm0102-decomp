#include "cmre.h"

extern u32 CM_FASTCALL cm_fn_00717410(u8 *record);
extern long double CM_THISCALL cm_fn_006f09b0(u8 *record, s32 value, s8 mode);

u8 CM_THISCALL cm_fn_006c3a00(u8 *self, s8 index, s32 value, s8 mode, u8 enabled)
{
    u8 score = 0;
    u32 x = (u32)(s32)index;
    u32 scaled = (x * 8u - x) * 32u - x;
    u32 scaled5 = scaled + scaled * 4u;
    u32 record = (u32)self + scaled5 * 8u + 0x4796u;

    for (u32 i = 0; i < 20u; ++i, record += 0x1beu) {
        s8 active = *(volatile s8 *)(record + 0x19u);
        if (active >= 0) {
            u16 check = (u16)cm_fn_00717410((u8 *)record);
            if (check != 1u) {
                if (enabled != 0 || *(volatile u8 *)(record + 0x2bu) == 0) {
                    long double result = cm_fn_006f09b0((u8 *)record, value, mode);
                    double limit_a = CM_GLOBAL(volatile double, 0x009689a0u);
                    if (!(result < (long double)limit_a) &&
                        !(result > (long double)limit_a)) {
                        score = (u8)(score + 3u);
                    } else {
                        double limit_b = CM_GLOBAL(volatile double, 0x00968940u);
                        if (!(result > (long double)limit_b))
                            score = (u8)(score + 1u);
                    }
                }
            }
        }
    }
    return score;
}
