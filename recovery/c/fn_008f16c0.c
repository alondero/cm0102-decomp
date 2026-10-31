#include "cmre.h"
extern u32 CM_CDECL cm_fn_007615d0(u32);
extern u32 CM_CDECL cm_fn_007643d0(u32);

u32 CM_CDECL cm_fn_008f16c0(u32 selection)
{
    u32 value = 0;
    u32 result;
    if ((u16)selection != 0xffff) {
        u32 index = (u32)(s32)(s16)CM_GLOBAL(volatile u16, 0x00b77c7a);
        u32 slot = (u32)(s32)(s16)selection;
        u32 table = CM_GLOBAL(volatile u32, 0x00b74c4c + index * 0x300u);
        value = CM_GLOBAL(volatile u32, table + slot * 0x18cu + 0xba966u);
    }
    if ((u16)value == 0)
        value = (value & 0xffff0000u) | CM_GLOBAL(volatile u16, 0x00dd6bde);
    if ((u16)value == 1)
        return 0xfffffff7u;
    result = cm_fn_007615d0(selection);
    if (result == 0)
        result = cm_fn_007643d0(selection);
    return result;
}

