#include "cmre.h"

u32 CM_CDECL cm_fn_0094f833(u32 address)
{
    u32 count = 0;
    do {
        if (*(volatile u32 *)address != 0)
            return 0;
        ++count;
        address += 4;
    } while ((s32)count < 3);
    return 1;
}
