#include "cmre.h"

u32 CM_CDECL cm_fn_0041bf80(u32 arg)
{
    u32 result = arg == 0;
    CM_GLOBAL(volatile u32, 0x00dd6bda) = result;
    return result;
}
