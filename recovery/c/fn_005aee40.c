#include "cmre.h"

u32 CM_STDCALL cm_fn_005aee40(u32 argument)
{
    u32 table = CM_GLOBAL(volatile u32, 0x00ae2a5c);
    u32 index = *(volatile u32 *)argument;
    u8 value = *(volatile u8 *)(table + index * 2u);
    return (value & 1u) + 0x7d1u;
}
