#include "cmre.h"

u32 CM_CDECL cm_fn_007be640(u32 first, u32 second)
{
    u32 left, right;
    if (first == 0)
        return 1;
    if (second == 0)
        return 0xffffffffu;
    left = *(volatile u32 *)second;
    right = *(volatile u32 *)first;
    if (left == right) {
        left = *(volatile u32 *)(second + 4u);
        right = *(volatile u32 *)(first + 4u);
    }
    return left - right;
}
