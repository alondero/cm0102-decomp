#include "cmre.h"

extern u32 CM_CDECL cm_fn_00540520(u32 *pointer);

u8 CM_CDECL cm_fn_00540490(u32 *first, u32 *second)
{
    u8 left = (u8)cm_fn_00540520(first);
    u8 right = (u8)cm_fn_00540520(second);
    s32 key;

    if (left == right)
        return 1;
    if ((left == 8 && right == 9) || (right == 8 && left == 9))
        return 1;
    if ((left == 8 && right == 7) || (right == 8 && left == 7))
        return 1;

    key = CM_GLOBAL(volatile s32, 0x009cf1dc);
    if (left == 6 && *(volatile s32 *)second == key)
        return 1;
    if (right == 6 && *(volatile s32 *)first == key)
        return 1;
    if ((left == 6 && right == 7) || (right == 6 && left == 7))
        return 1;
    return 0;
}
