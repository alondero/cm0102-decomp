#include "cmre.h"
u32 CM_CDECL cm_fn_00934ae0(u32 object)
{
    u32 result = 0, i;
    for (i = 0; i < 12; ++i) {
        u16 word = CM_GLOBAL(volatile u16, object + 0x3a2u + i * 2u);
        if ((s16)word > (s16)result)
            result = (object & 0xffff0000u) | word;
    }
    return result;
}

