#include "cmre.h"

u32 CM_CDECL cm_fn_00838de0(void)
{
    u32 index = (u32)(s32)CM_GLOBAL(volatile s16, 0x00b77c7a);
    u32 pointer = *(volatile u32 *)(0x00b74c26u + index * 0x300u);
    if (pointer != 0 && *(volatile u8 *)(pointer + 0x5fu) == 5)
        return 1;
    return 0;
}
