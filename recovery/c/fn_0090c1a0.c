#include "cmre.h"

extern u32 CM_CDECL cm_fn_00945ce6(u32 destination, u32 size, u32 count, u32 stream);
extern s32 CM_CDECL cm_fn_00946a22(volatile u8 *file, u32 second, u32 mode);

u32 CM_CDECL cm_fn_0090c1a0(u32 stream, u32 output)
{
    cm_fn_00946a22((volatile u8 *)stream, 0, 2);
    return cm_fn_00945ce6((u32)&output, 4, 1, stream);
}
