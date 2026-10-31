#include "cmre.h"

extern u32 CM_CDECL cm_fn_0094c322(u32 value, u32 mask);

u32 CM_CDECL cm_fn_00947090(u32 value)
{
    if (CM_GLOBAL(volatile s32, 0x00ad91dcu) > 1)
        return cm_fn_0094c322(value, 8);
    return *(volatile u8 *)(CM_GLOBAL(volatile u32, 0x00ad8fd0u) + value * 2u) & 8u;
}
