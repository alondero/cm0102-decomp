#include "cmre.h"

extern s32 CM_CDECL cm_fn_00946a22(volatile u8 *file, u32 second, u32 mode);
extern u32 CM_CDECL cm_fn_00945bfe(u32 buffer, u32 count, u32 element_size, u32 state_address);

u32 CM_CDECL cm_fn_005b2770(u32 object, volatile u32 offset, volatile u32 buffer, volatile u32 size)
{
    u32 stream = *(volatile u32 *)object;
    u32 result, length, dest;
    if (stream == 0)
        return 0;
    result = (u32)cm_fn_00946a22((volatile u8 *)stream, offset, 0);
    if (result != 0)
        return 0;
    stream = *(volatile u32 *)object;
    length = size;
    dest = buffer;
    result = cm_fn_00945bfe(dest, length, 1, stream);
    return result == 1;
}
