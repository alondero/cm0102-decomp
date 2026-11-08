/* Preferred-image VA 0x00922ee0, base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Physical EAX result: null data pointer, or the last loaded signed count.
   RNG remains original code: only its EAX is consumed at these call sites. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_0090cfc0(u32 bound);
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile s8, (a))

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_00922ee0(u32 self)
{
    u32 result, index = 0, address, pointer, sample;
    s32 sum;
    s8 value;
    long double live;
    volatile long long converted;

    result = D(self + 4u);
    if (!result) return result;
    result = D(0x00ae2354);
    if ((s32)result <= 0) return result;
    do {
        pointer = D(self + 4u);
        address = pointer + index * 4u;
        sample = cm_fn_0090cfc0(100);
        value = B(address);
        sum = (s32)(s16)sample + (s32)value;
        live = (long double)sum;
        live *= CM_GLOBAL(volatile double, 0x00968968);
        converted = (long long)live; /* Native __ftol CALL VA 0x00922f26. */
        B(address) = (s8)(u8)(u32)converted;
        pointer = D(self + 4u);
        address = pointer + index * 4u;
        value = B(address);
        if (value < 0) B(address) = 0;
        else if (value > 100) B(address) = 100;

        pointer = D(self + 4u);
        address = pointer + index * 4u + 1u;
        sample = cm_fn_0090cfc0(100);
        value = B(address);
        sum = (s32)(s16)sample + (s32)value;
        live = (long double)sum;
        live *= CM_GLOBAL(volatile double, 0x00968968);
        converted = (long long)live; /* Native __ftol CALL VA 0x00922f6e. */
        B(address) = (s8)(u8)(u32)converted;
        pointer = D(self + 4u);
        address = pointer + index * 4u + 1u;
        value = B(address);
        if (value < 0) B(address) = 0;
        else if (value > 100) B(address) = 100;

        pointer = D(self + 4u);
        address = pointer + index * 4u + 2u;
        sample = cm_fn_0090cfc0(100);
        value = B(address);
        sum = (s32)(s16)sample + (s32)value;
        live = (long double)sum;
        live *= CM_GLOBAL(volatile double, 0x00968968);
        converted = (long long)live; /* Native __ftol CALL VA 0x00922fb8. */
        B(address) = (s8)(u8)(u32)converted;
        pointer = D(self + 4u);
        address = pointer + index * 4u + 2u;
        value = B(address);
        if (value < 0) B(address) = 0;
        else if (value > 100) B(address) = 100;

        sample = cm_fn_0090cfc0(10);
        if (!sample) {
            sample = cm_fn_0090cfc0(3);
            pointer = D(self + 4u);
            B(pointer + index * 4u + 3u) = (s8)(u8)sample;
        }
        result = D(0x00ae2354);
        index += 1u;
    } while ((s32)index < (s32)result);
    return result;
}
