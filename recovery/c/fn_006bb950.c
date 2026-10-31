#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00716450(u32 self, u32 factor_bits);

u32 CM_THISCALL cm_fn_006bb950(u32 self, u32 selector, u32 factor_bits)
{
    u32 cursor = self + 0x4796u + (u32)(s32)(s8)selector * 0x22d8u;
    u32 result = 0;
    u32 i;
    for (i = 0; i != 20; ++i) {
        result = *(volatile u32 *)(cursor + 0x69);
        if (result)
            result = cm_fn_00716450(cursor, factor_bits);
        cursor += 0x1beu;
    }
    return result;
}
