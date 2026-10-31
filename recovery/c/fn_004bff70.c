#include "cmre.h"

extern u32 CM_CDECL cm_fn_004ba8f0(u32 value, u32 flag);

u32 CM_CDECL cm_fn_004bff70(u32 object, u32 first, u32 second, u32 selector)
{
    u8 value;
    if ((u16)second != 0)
        return cm_fn_004ba8f0(second, 0);
    if ((u16)first != 0)
        return cm_fn_004ba8f0(first, 0);
    if (object == 0)
        return 1;
    if ((u8)selector == 1)
        value = *(volatile u8 *)(object + 0x37);
    else
        value = *(volatile u8 *)(object + 0x52);
    return (object & 0xffffff00u) | value;
}
