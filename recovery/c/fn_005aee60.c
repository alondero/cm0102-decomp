#include "cmre.h"

u32 CM_STDCALL cm_fn_005aee60(u32 arg)
{
    u32 table = CM_GLOBAL(volatile u32, 0x00ae2a5c);
    u32 index = *(volatile u32 *)arg;
    u16 value = *(volatile u16 *)(table + index * 2u);
    if ((value & 0xfffeu) != 0)
        value >>= 1;
    else
        value = CM_GLOBAL(volatile u16, 0x00ae2c92);
    return (arg & 0xffff0000u) | value;
}
