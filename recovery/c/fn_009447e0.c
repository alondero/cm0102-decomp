#include "cmre.h"

u32 CM_CDECL cm_fn_009447e0(u32 first, u32 second)
{
    u32 sign = 1, low, high;
    if ((s32)first < 0) {
        first = 0u - first;
        sign = 0xffffffff;
    }
    if ((s16)second < 0) {
        second = 0u - second;
        sign = 0u - sign;
    }
    second &= 0xffff;
    low = (first & 0xffff) * second;
    high = ((u32)((s32)first >> 16) & 0xffff) * second;
    return ((low >> 14) + high * 4) * sign;
}
