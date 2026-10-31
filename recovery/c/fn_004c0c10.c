#include "cmre.h"

extern u32 CM_CDECL cm_fn_0094695d(u32 handle);

u32 CM_CDECL cm_fn_004c0c10(u32 object)
{
    u32 handle = *(volatile u32 *)object;
    if (handle == 0xffffffffu) return 0xffffffffu;
    cm_fn_0094695d(handle);
    *(volatile u32 *)object = 0xffffffffu;
    return 0;
}
