/* Preferred-image VA 0x00713d90, base 0x00400000; primary executable SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Callers at VAs 0x0070057f and 0x00711cee test AL only. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_006cb6c0(u8 *self, u8 first, u8 second,
                                     u8 third, u8 fourth);
/* Original ret 0x10; ECX is overwritten before its first read. */
extern long double CM_STDCALL cm_fn_006cc4e0(u32 first, u32 second,
                                            u32 third, u32 fourth);

u8 CM_THISCALL cm_fn_00713d90(u32 self, u32 first, u32 second)
{
    u32 original_first = first, receiver, mode, before, after;
    u8 old_first, old_second;
    s32 difference, limit, first_distance, second_distance;
    volatile float threshold;
    long double candidate;

    old_first = CM_GLOBAL(volatile u8, self + 0x102u);
    if ((u8)first == old_first &&
        (u8)second == CM_GLOBAL(volatile u8, self + 0x103u))
        return 1;
    mode = CM_GLOBAL(volatile u8, self + 0x27u) == 0 ? 0u : 0xbu;
    first = (first & 0xffffff00u) | mode;
    receiver = CM_GLOBAL(volatile u32, self + 0x19eu);
    before = cm_fn_006cb6c0((u8 *)receiver, (u8)original_first,
                           (u8)second, 4, (u8)mode);
    old_first = CM_GLOBAL(volatile u8, self + 0x102u);
    old_second = CM_GLOBAL(volatile u8, self + 0x103u);
    receiver = CM_GLOBAL(volatile u32, self + 0x19eu);
    after = cm_fn_006cb6c0((u8 *)receiver, (u8)original_first,
                          (u8)second, old_first, old_second);
    difference = (s32)(s16)before - (s32)(s16)after;
    if (difference < 0)
        difference = -difference;
    limit = (s32)CM_GLOBAL(volatile s8, self + 0x10cu) + 10;
    if (difference >= limit)
        return 0;

    first_distance = (s32)CM_GLOBAL(volatile s8, self + 0x102u)
                   - (s32)(s8)original_first;
    if (first_distance < 0)
        first_distance = -first_distance;
    second_distance = (s32)CM_GLOBAL(volatile s8, self + 0x103u)
                    - (s32)(s8)second;
    if (second_distance < 0)
        second_distance = -second_distance;
    /* Keep the otherwise unused original ECX load before the table access. */
    receiver = CM_GLOBAL(volatile u32, self + 0x19eu);
    (void)receiver;
    threshold = CM_GLOBAL(volatile float, 0x00a15e44u +
        ((u32)(s32)(s8)second_distance * 9u +
         (u32)(s32)(s8)first_distance) * 4u);
    candidate = cm_fn_006cc4e0(original_first, second, 4, first);
    return candidate >= (long double)threshold;
}
