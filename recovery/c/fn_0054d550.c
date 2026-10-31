#include "cmre.h"

u32 CM_CDECL cm_fn_0054d550(u32 first, u32 second)
{
    u32 value;
    u32 low;
    if (first == 9u) {
        s32 signed_value = (s16)second;
        value = (u32)signed_value;
        low = signed_value < 5 || signed_value > 6;
    } else {
        value = first;
        low = first == 3u;
    }
    return (value & 0xffffff00u) | low;
}
