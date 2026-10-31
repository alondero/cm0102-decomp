#include "cmre.h"

u32 CM_CDECL cm_fn_005e7f00(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h)
{
    u32 width = (u32)(c < g ? c : g) - (u32)(a > e ? a : e);
    u32 height = (u32)(d < h ? d : h) - (u32)(b > f ? b : f);
    if ((s32)width <= 0) width = 0;
    if ((s32)height <= 0) height = 0;
    return width * height;
}
