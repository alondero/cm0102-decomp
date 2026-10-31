#include "cmre.h"

s32 CM_CDECL cm_fn_00952e9c(s32 code)
{
    volatile s32 original_code = code;

    if (original_code == 1)
        CM_GLOBAL(volatile u32, 0x00de2f8cu) = 0x21u;
    else if (original_code > 1 && original_code <= 3)
        CM_GLOBAL(volatile u32, 0x00de2f8cu) = 0x22u;
    return original_code;
}
