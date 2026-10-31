#include "cmre.h"

extern u32 CM_CDECL cm_fn_00934b60(u32, u32, u32, u32, u32, u32, u32, u32, u32);

u32 CM_CDECL cm_fn_00934b10(u32 object, u32 a2, u32 a3, u32 a4,
                            u32 a5, u32 a6, u32 a7, u32 a8)
{
    if (object == 0 || *(volatile u32 *)object != 0xa5a0f5a5u ||
        *(volatile u32 *)(object + 0x44c) != 0x0fa55af0u)
        return 0xffffffffu;
    return cm_fn_00934b60(object, a2, a3, 2, a4, a5, a6, a7, a8);
}
