#include "cmre.h"

extern u32 CM_CDECL cm_fn_009478d8(u32 path, u32 mode);

u32 CM_CDECL cm_fn_0090c120(u32 path)
{
    return cm_fn_009478d8(path, 0) == 0;
}
