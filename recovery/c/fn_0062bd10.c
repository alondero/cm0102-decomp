/* Preferred-image VA 0x0062bd10, image base 0x00400000; 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Three stack arguments, RET12, ECX receiver. Retain full EAX, including the
   index-derived high half added after the low-half SI field replacement. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0090cfc0(u32 bound);

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_0062bd10(u32 self, u32 record,
                              volatile u32 field, volatile u32 increment)
{
    u32 index, offset, base, address, saved, other, result;
    s32 first, second, third, threshold;
    s16 current;
    long double value, multiplier;
    if (!record)
        return record;
    index = CM_GLOBAL(volatile u32, record);
    base = CM_GLOBAL(volatile u32, 0x00ae2364u);
    if ((s32)index >= (s32)base)
        return record;
    offset = index * 31u;
    base = CM_GLOBAL(volatile u32, self);
    CM_GLOBAL(volatile u16, offset + base + 10u) = (u16)field;
    index = CM_GLOBAL(volatile u32, record);
    offset = index * 31u;
    base = CM_GLOBAL(volatile u32, self);
    address = offset + base + 12u;
    current = (s16)CM_GLOBAL(volatile u16, address);
    saved = (offset & 0xffff0000u) | (u16)current;
    if (current < -1000) {
        field = (u32)(s32)current;
        value = (long double)(s32)field;
        field = (u32)(s32)(s16)increment;
        value *= CM_GLOBAL(volatile double, 0x0096bac0u);
        value -= CM_GLOBAL(volatile double, 0x00968e18u);
        multiplier = (long double)(s32)field;
        value *= multiplier;
        /* Registered __ftol call 0x0062bd82, FISTP64 truncation (ADR0006). */
        result = (u32)(long long)value;
        result += saved;
        CM_GLOBAL(volatile u16, address) = (u16)result;
        return result;
    }
    other = CM_GLOBAL(volatile u32, record + 0x61u);
    first = (s8)CM_GLOBAL(volatile u8, other + 0x44u);
    second = (s8)CM_GLOBAL(volatile u8, other + 0x3cu);
    third = (s8)CM_GLOBAL(volatile u8, record + 0x5bu);
    threshold = 1200 - (s16)(first + second + third);
    if ((s32)current < threshold) {
        result = increment;
        saved += result;
        CM_GLOBAL(volatile u16, address) = (u16)saved;
        return result;
    }
    if (current >= 1250)
        return record;
    index = CM_GLOBAL(volatile u32, record);
    offset = index * 31u;
    base = CM_GLOBAL(volatile u32, self);
    address = offset + base + 12u;
    result = cm_fn_0090cfc0((u32)(s32)(s16)increment);
    result += 1u;
    CM_GLOBAL(volatile u16, address) =
        (u16)(CM_GLOBAL(volatile u16, address) + (u16)result);
    return result;
}
