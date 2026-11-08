/* Preferred-image VA 0x006e86d0, base 0x00400000; CM 01/02 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The initial PUSH ECX allocates a local overwritten at 0x006e86e3.
   Five cdecl arguments; callers retain the complete physical EAX result. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_005407e0(u32 first, u32 second, u32 mode);
extern long long CM_CDECL cm_fn_00544410(volatile u32 record, volatile u32 adjustment);

/* Original __ftol changes/restores CW and uses FISTP qword, not FISTTP. */
__attribute__((target("no-sse3")))
u32 CM_CDECL cm_fn_006e86d0(u32 optional, u32 primary, u32 fallback,
                           u32 mode_arg, u32 rating_arg)
{
    u32 seed = CM_GLOBAL(volatile u32, 0x00dd7120);
    u32 dynamic = 0;
    s32 rating;
    s8 age = 0;
    s8 adjustment = 1;
    u8 mode = (u8)mode_arg;
    u32 detail;
    u32 other;
    s32 remainder;
    volatile float factor;
    long double value;
    s32 displaced;
    s32 weight;
    u32 weighted;
    u32 product;

    seed += CM_GLOBAL(volatile u32, primary);
    if ((s16)rating_arg > -1) {
        rating = (s16)rating_arg / 50;
    } else {
        other = optional;
        if (!other)
            other = CM_GLOBAL(volatile u32, fallback + 0xcfu);
        rating = (s16)cm_fn_005407e0(primary, other, 0) / 50;
    }

    if (mode != 2) {
        u8 kind;
        u8 byte_age;
        detail = CM_GLOBAL(volatile u32, primary + 0x61u);
        kind = CM_GLOBAL(volatile u8, detail + 0x0fu);
        byte_age = CM_GLOBAL(volatile u8, primary + 0x18u);
        age = (s8)(u8)(byte_age - (kind == 20 ? 26u : 23u));
    }

    if (optional) {
        dynamic = CM_GLOBAL(volatile u32, optional + 0x69u);
        seed += CM_GLOBAL(volatile u32, optional);
        if (dynamic && mode == 2 && (s16)rating > 50) {
            u32 arg = (dynamic & 0xffffff00u) |
                      CM_GLOBAL(volatile u8, dynamic + 0x17u);
            s32 threshold = (s16)rating +
                ((s8)(u32)cm_fn_00544410(dynamic, arg) - 10) * 10;
            if (threshold > (s32)seed % 200) {
                u16 result;
                detail = CM_GLOBAL(volatile u32, primary + 0x61u);
                result = CM_GLOBAL(volatile u16, detail + 5u);
                return (detail & 0xffff0000u) | result;
            }
        }
    }

    remainder = (s32)seed % 101;
    factor = (long double)remainder *
             (long double)CM_GLOBAL(volatile double, 0x00967898);
    if (dynamic) {
        remainder = (s32)seed % 10;
        if (!remainder) {
            value = (long double)factor *
                    (long double)CM_GLOBAL(volatile float, 0x0096afb0);
        } else {
            u32 arg = ((u32)remainder & 0xffffff00u) |
                      CM_GLOBAL(volatile u8, dynamic + (mode == 2 ? 0x17u : 0x18u));
            s32 helper = (s8)(u32)cm_fn_00544410(dynamic, arg);
            value = (long double)helper * (long double)factor;
        }
        value += (long double)CM_GLOBAL(volatile double, 0x00968968);
        /* Original 0x006e884c calls 0x00945640 (__ftol), ADR 0006. */
        adjustment = (s8)(u32)(long long)value;
        if (adjustment > 20)
            adjustment = 20;
    }

    if (mode != 2 && age > 0)
        rating = (s32)((u32)rating - (u32)((s32)age * (s32)age));
    remainder = (s32)seed % ((21 - (s32)adjustment) * 2);
    displaced = (s16)(u16)((u32)remainder + (u32)(s32)adjustment - 21u);
    detail = CM_GLOBAL(volatile u32, primary + 0x61u);
    other = (u32)(s32)CM_GLOBAL(volatile s16, detail + (mode == 2 ? 5u : 7u));
    weight = (s8)(u8)(adjustment / 2 + 10);
    weighted = (other + (u32)displaced) * (u32)weight;
    weighted += (u32)(20 - weight) * (u32)(s32)(s16)rating;
    product = weighted * 0x66666667u;
    /* IMUL leaves its low DWORD in EAX; MOV AX,DX replaces only the WORD. */
    return (product & 0xffff0000u) | (u16)((s32)weighted / 20);
}
