#include "cmre.h"

u32 CM_CDECL cm_fn_0085e2f0(u32 first, u32 second)
{
    u32 left = *(volatile u32 *)first;
    u32 right = *(volatile u32 *)second;
    u32 result = left - right;
    u32 left_ptr, right_ptr;
    if (result != 0)
        return result;
    left_ptr = *(volatile u32 *)(first + 12u);
    if (left_ptr == 0)
        return 0;
    right_ptr = *(volatile u32 *)(second + 12u);
    if (right_ptr == 0)
        return 0;
    right = *(volatile u32 *)(right_ptr + 0x3cu);
    left = *(volatile u32 *)(left_ptr + 0x3cu);
    return right - left;
}
