#include "cmre.h"

u32 CM_CDECL cm_fn_00547bd0(u32 object, u32 value)
{
    if (object == 0 || value == 0)
        return object;
    if (*(volatile u32 *)(object + 0x9bu) == value)
        *(volatile u32 *)(object + 0x9bu) = 0;
    else if (*(volatile u32 *)(object + 0x9fu) == value)
        *(volatile u32 *)(object + 0x9fu) = 0;
    else if (*(volatile u32 *)(object + 0xa3u) == value)
        *(volatile u32 *)(object + 0xa3u) = 0;
    return object;
}
