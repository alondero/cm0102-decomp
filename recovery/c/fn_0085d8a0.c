/* WIP: listing's x87 FSQRT has no faithful plain-C expression in the current
   O0 lane; builtin sqrt links unsuccessfully, and O2 differs in FPU state. */
#include "cmre.h"

long double CM_STDCALL cm_fn_0085d8a0(
    volatile u8 *object, volatile u8 *point, volatile u8 *other,
    double incoming)
{
    s32 scale;
    double dx;
    double dy;
    double distance;
    long double value;
    long double temporary;

    (void)*(volatile u16 *)(object + 0x30);
    scale = (s32)*(volatile s8 *)(*(volatile u32 *)(*(volatile u32 *)(object + 0x20) + 0x53) + 0x7e);
    dx = *(volatile double *)(point + 0x2b) - *(volatile double *)(other + 0x2b);
    dy = *(volatile double *)(point + 0x23) - *(volatile double *)(other + 0x23);
    distance = __builtin_sqrt(dx * dx + dy * dy);

    if (*(volatile u16 *)(object + 0x30) == 0xa0 &&
        *(volatile u8 *)(object + 0x3a) == 5)
        distance *= 0.5;

    value = 1.0L - (long double)distance * 0.13333333333333333L;
    if (*(volatile u8 *)(object + 0x3c) == 0)
        value *= value;

    if (0.0 < distance && 0 < scale) {
        temporary = (long double)scale + 1.0L;
        value /= temporary * temporary;
    }

    if (14.0 < incoming) {
        temporary = 1.0L - (20.0L - incoming) * 0.15L;
        if (value < temporary)
            return temporary;
    }
    if (*(volatile u8 *)(object + 0x3a) == 10 && value < 0.2L)
        return 0.2L;
    if (value < 0.05L &&
        *(volatile u32 *)(*(volatile u32 *)(object + 0x1c) + 0x53) ==
            *(volatile u32 *)(*(volatile u32 *)(object + 0x20) + 0x53))
        return 0.05L;
    if (value < 0.03L)
        value = 0.03L;
    return value;
}
