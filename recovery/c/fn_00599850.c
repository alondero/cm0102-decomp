#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))

u32 CM_THISCALL cm_fn_00599850(u32 self, u32 value)
{
    u32 first = D(self + 0x128);
    u32 second = D(self + 0x88);
    u32 low, high, borrow;
    D(self + 0x128) = first + value;
    D(self + 0x88) = second + value;
    low = D(self);
    borrow = low < value;
    D(self) = low - value;
    high = D(self + 4) - ((s32)value < 0 ? 0xffffffffu : 0u) - borrow;
    D(self + 4) = high;
    return high;
}
