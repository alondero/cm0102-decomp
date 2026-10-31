#include "cmre.h"

extern u32 CM_CDECL cm_fn_0066f4d0(void);

u8 CM_CDECL cm_fn_00617780(s8 value)
{
    u32 index, target;
    if (cm_fn_0066f4d0() != 9u) {
        cm_fn_0066f4d0();
        return 0;
    }
    index = (u32)((s32)value - 1);
    if (index > 13u) return 1;
    target = *(volatile u32 *)(0x006177b0u + index * 4u);
    if (target == 0x0061779cu) return 4;
    if (target == 0x006177a2u) return 3;
    if (target == 0x006177aau) return 0;
    return 1;
}
