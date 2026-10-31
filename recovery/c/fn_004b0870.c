#include "cmre.h"

u32 CM_CDECL cm_fn_004b0870(u32 arg)
{
    s32 remainder = 0;
    s32 low = (s16)arg;
    if (low < 1000)
        remainder = low % 10;
    return arg - (u32)remainder;
}
