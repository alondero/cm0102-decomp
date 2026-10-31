#include "cmre.h"

u32 CM_STDCALL cm_fn_007e6f20(u32 input)
{
    u32 value;
    if (!input) return 0;
    value = *(volatile u32 *)input;
    if (value == CM_GLOBAL(volatile u32, 0x009cf794)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009cf684)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009cf798)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009cf6b0)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009cf5d8)) return 1;
    return value == CM_GLOBAL(volatile u32, 0x009cf548);
}
