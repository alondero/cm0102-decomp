#include "cmre.h"

long double CM_THISCALL cm_fn_005d5a60(u32 self, u32 selector)
{
    u32 denominator = self + ((u8)selector ? 0x28u : 0x18u);
    u32 numerator = denominator - 8u;
    double value = *(volatile double *)denominator;
    double zero = CM_GLOBAL(volatile double, 0x009689a0u);

    if (value < zero || value > zero)
        return (long double)*(volatile double *)numerator /
               (long double)*(volatile double *)denominator;
    return (long double)CM_GLOBAL(volatile double, 0x00968998u);
}
