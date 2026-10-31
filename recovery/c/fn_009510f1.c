#include "cmre.h"

extern u32 CM_CDECL cm_fn_00950fc4(double value);

u32 CM_CDECL cm_fn_009510f1(double value)
{
    union {
        double value;
        unsigned long long bits;
    } representation;
    representation.value = value;

    u32 low = (u32)representation.bits;
    u32 high = (u32)(representation.bits >> 32);
    u16 exponent_sign = (u16)(high >> 16);
    u16 exponent = exponent_sign & 0x7ff0u;

    if (exponent == 0x7ff0u) {
        u32 kind = cm_fn_00950fc4(value);
        if (--kind == 0)
            return 0x200u;
        if (--kind == 0)
            return 4u;
        if (--kind == 0)
            return 2u;
        return 1u;
    }

    u32 sign = (high & 0x80000000u) != 0 ? 0x8000u : 0u;
    if (exponent == 0 &&
        (((high & 0x000fffffu) != 0) || low != 0)) {
        return sign != 0 ? 0x10u : 0x80u;
    }

    volatile double comparison = CM_GLOBAL(volatile double, 0x009689a0u);
    if (value == comparison || value != value || comparison != comparison)
        return sign != 0 ? 0x20u : 0x40u;
    return sign != 0 ? 0x08u : 0x100u;
}
