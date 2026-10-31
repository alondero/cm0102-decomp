#include "cmre.h"

u32 CM_CDECL cm_fn_007fc270(u32 first, u32 second)
{
    u32 left, right;
    if (first == 0)
        return 0xffffffffu;
    if (second == 0)
        return 1;
    left = *(volatile u32 *)(first + 4u);
    right = *(volatile u32 *)(second + 4u);
    if (left == right) {
        left = *(volatile u32 *)first;
        right = *(volatile u32 *)second;
    }
    return left - right;
}
