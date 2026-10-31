#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
u32 CM_THISCALL cm_fn_005999a0(u32 self, u32 value)
{
    u32 first = D(self + 0x148);
    u32 second = D(self + 0xa8);
    u32 low, sum, high;
    D(self + 0x148) = first + value;
    D(self + 0xa8) = second + value;
    low = D(self);
    sum = low + value;
    D(self) = sum;
    high = D(self + 4) + ((s32)value < 0 ? 0xffffffffu : 0u) + (sum < low);
    D(self + 4) = high;
    return high;
}

