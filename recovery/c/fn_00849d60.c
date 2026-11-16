#include "cmre.h"

extern u32 CM_CDECL cm_fn_0053ca90(volatile u8 *);
extern u32 CM_CDECL cm_fn_005407e0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007f7490(u32, u32, u32);

u8 CM_STDCALL cm_fn_00849d60(u32 first, u32 second)
{
    u32 v, receiver;
    s16 b, d;
    s8 age;
    long double x, y;
    double stored;
    float ratio;

    v = cm_fn_0053ca90((volatile u8 *)second);
    v = cm_fn_005407e0(first, v, 0);
    if ((s16)v > 1) {
        v = cm_fn_0053ca90((volatile u8 *)second);
        v = cm_fn_005407e0(first, v, 0);
        b = (s16)v;
    } else {
        b = 1;
    }
    d = *(volatile s16 *)(second + 0x80);
    if (d <= 1) d = 1;
    v = cm_fn_0053ca90((volatile u8 *)second);
    receiver = CM_GLOBAL(volatile u32, 0xb74448);
    if (cm_fn_007f7490(receiver, first, v)) {
        x = (long double)b * CM_GLOBAL(volatile double, 0x968940);
        y = (long double)d * CM_GLOBAL(volatile double, 0x969040);
        stored = (double)y;
        if (!(x > stored)) x = stored;
        /* VA 00849e0b: MSVC __ftol consumes ST0, not stack arguments. */
        b = (s16)(long long)x;
    }
    age = *(volatile s8 *)(first + 0x18);
    x = (long double)b / (long double)d;
    /* Preferred-image VAs 00849e2d-00849e42: C0 selects the lower
       reload for less-than OR unordered (CM0102 3.9.68, SHA-256
       0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86). */
    if (!(x >= CM_GLOBAL(volatile double, 0x967880)))
        x = CM_GLOBAL(volatile double, 0x967880);
    else if (x > CM_GLOBAL(volatile double, 0x968950))
        x = CM_GLOBAL(volatile double, 0x968950);
    ratio = (float)x;
    if (x > CM_GLOBAL(volatile double, 0x968940)) return 1;
    if (ratio > CM_GLOBAL(volatile double, 0x968978) && d < 0x2134) return 2;
    if (!(ratio > CM_GLOBAL(volatile double, 0x968978)) && b <= 0x1964) {
        if (ratio > CM_GLOBAL(volatile double, 0x968e48) && age < 0x17) return 5;
        return age >= 0x18 ? 4 : 6;
    }
    return 3;
}
