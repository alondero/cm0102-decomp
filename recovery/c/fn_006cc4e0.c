#include "cmre.h"

float CM_STDCALL cm_fn_006cc4e0(u32 a, u32 b, u32 c, u32 d)
{
    s32 x = (s32)(s8)a - (s32)(s8)c;
    s32 y = (s32)(s8)b - (s32)(s8)d;
    s8 dx = (s8)(x < 0 ? -x : x);
    s8 dy = (s8)(y < 0 ? -y : y);
    if (dx < 0 || dx > 8 || dy < 0 || dy > 11)
        return CM_GLOBAL(volatile float, 0x0096d2dc);
    return *(volatile float *)(0x00a15e44u + ((u32)dy * 9 + (u32)dx) * 4);
}
