#include "cmre.h"

u32 CM_CDECL cm_fn_0094a6aa(void)
{
    volatile double divisor = CM_GLOBAL(volatile double, 0x00971578u);
    volatile double dividend = CM_GLOBAL(volatile double, 0x00971570u);
    volatile double residual = (double)((long double)dividend -
        ((long double)dividend / (long double)divisor) * (long double)divisor);
    double threshold = CM_GLOBAL(volatile double, 0x00967890u);

    return residual > threshold;
}
