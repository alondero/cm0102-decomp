#include "cmre.h"

extern u32 CM_CDECL cm_fn_004841d0(u32 a1, u32 a2, u32 a3, u32 a4,
                                   u32 a5, u32 a6, u32 a7, u32 a8,
                                   u32 a9, u32 a10, u32 a11, u32 a12,
                                   u32 a13, u32 a14);

u32 CM_CDECL cm_fn_00484980(u32 first, u32 second, u32 third)
{
    return cm_fn_004841d0(first, 0, 0x80000, 4, 0xffffffffu,
                         0xffffffffu, 0xffffffffu, 0x164c21, second,
                         0, 0, third, 0, 0);
}
