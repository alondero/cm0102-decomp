#include "cmre.h"

u32 CM_CDECL cm_fn_004b0850(s32 value)
{
    if (value >= 1000)
        return (u32)value & 0xffffff00u;
    return ((u32)(value / 10) & 0xffffff00u) | (u8)(value % 10);
}
