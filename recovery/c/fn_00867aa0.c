#include "cmre.h"

u32 CM_CDECL cm_fn_00867aa0(u32 arg, u32 selector)
{
    u32 pointer = *(volatile u32 *)(arg + 0x61u);
    u8 value;
    if (pointer == 0)
        return 0;
    value = *(volatile u8 *)(pointer + 0x0fu);
    if (value == 0x14u)
        return (u8)selector == 2;
    return (u8)selector != 2;
}
