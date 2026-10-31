#include "cmre.h"
extern u32 CM_CDECL cm_fn_00806260(u32 left, u32 right, u32 words);

u32 CM_CDECL cm_fn_00806030(u32 self, u32 text)
{
    u32 p;
    if (!self || !text)
        return 0;
    p = self + 4;
    if (p && cm_fn_00806260(p, text, 0))
        return 1;
    p = self + 0x38;
    if (p && cm_fn_00806260(p, text, 0))
        return 1;
    return 0;
}
