#include "cmre.h"

extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 text);

void CM_STDCALL cm_fn_006a9e20(s8 kind, u32 destination, s16 capacity)
{
    if (destination == 0 || capacity < 15) return;
    if (kind == 1) cm_fn_0066f4e0(destination, 0x00a1153cu);
    else if (kind == 2) cm_fn_0066f4e0(destination, 0x00a11558u);
}
