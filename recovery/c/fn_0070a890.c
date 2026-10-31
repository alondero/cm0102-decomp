#include "cmre.h"

void CM_THISCALL cm_fn_0070a890(u32 self, s16 value)
{
    *(volatile s16 *)(self + 0x19cu) = value;
    if (value > 200) *(volatile s16 *)(self + 0x19cu) = 200;
    else if (value < -200) *(volatile s16 *)(self + 0x19cu) = -200;
}
