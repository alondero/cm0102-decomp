#include "cmre.h"

extern s32 CM_CDECL cm_fn_0090cfc0(s32 bound);

u32 CM_CDECL cm_fn_00757f30(u32 object)
{
    s32 random;
    if (*(volatile s16 *)(object + 0x80) > 0x186a) {
        s8 low;
        random = cm_fn_0090cfc0(100);
        low = (s8)random;
        if (low == 0)
            return 6;
        if (low < 8)
            return 3;
        if (low < 15)
            return 5;
        if (low < 40)
            return 2;
        return low >= 60;
    }
    random = cm_fn_0090cfc0(4);
    return random >= 0 && random <= 2 ? 2 : 4;
}
