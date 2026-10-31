#include "cmre.h"

extern s32 CM_CDECL cm_fn_00951756(u32 locale, u32 flags, u8 *first,
                                  s32 first_length, u8 *second,
                                  s32 second_length, u32 codepage);

u32 CM_CDECL cm_fn_0094dede(u8 *first, u8 *second, u32 count)
{
    u32 codepage, locale, result;
    if (!count)
        return 0;
    codepage = CM_GLOBAL(volatile u32, 0x00de4408);
    locale = CM_GLOBAL(volatile u32, 0x00de4624);
    result = (u32)cm_fn_00951756(locale, 1, first, (s32)count, second,
                                (s32)count, codepage);
    return result ? result - 2u : 0x7fffffffu;
}
