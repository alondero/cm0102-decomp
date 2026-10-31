#include "cmre.h"

float CM_THISCALL cm_fn_006f09b0(u32 self, u32 first, u32 second)
{
    s32 a = *(volatile s8 *)(self + 0x103) - (s32)(s8)second;
    s32 b;
    s32 index;
    if (a < 0)
        a = -a;
    a = (s8)a;
    b = *(volatile s8 *)(self + 0x102) - (s32)(s8)first;
    if (b < 0)
        b = -b;
    b = (s8)b;
    index = a * 9 + b;
    return *(volatile float *)(0x00a15e44u + (u32)index * 4u);
}
