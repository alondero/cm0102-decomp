#include "cmre.h"

float CM_THISCALL cm_fn_0079d3e0(u32 self, u32 key)
{
    u32 index, base;
    if (key == 0) return CM_GLOBAL(volatile float, 0x00969084u);
    index = *(volatile u32 *)key;
    base = *(volatile u32 *)self;
    return *(volatile float *)(base + index * 17u + 4u);
}
