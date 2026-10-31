#include "cmre.h"

float CM_THISCALL cm_fn_006fa1e0(u32 self)
{
    u8 flag = *(volatile u8 *)(self + 0x27);
    s32 a = *(volatile s8 *)(self + 0x103);
    s32 b;
    s32 index;
    if (flag == 0)
        a -= 11;
    if (a < 0)
        a = -a;
    a = (s8)a;
    b = *(volatile s8 *)(self + 0x102) - 4;
    if (b < 0)
        b = -b;
    b = (s8)b;
    index = a * 9 + b;
    return *(volatile float *)(0x00a15e44u + (u32)index * 4u);
}
