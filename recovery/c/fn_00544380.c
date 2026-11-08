/* Preferred-image VA 0x00544380, image base 0x00400000; CM 01/02 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Cdecl, two stack words. Original tail JMP 0x00544402 -> 0x00945640
   is the registered MSVC __ftol lowering; ADR 0006 permits its 64-bit cast.
   Keep the original argument-slot assignments and repeated constant reads. */
#include "cmre.h"

__attribute__((target("no-sse3")))
long long CM_CDECL cm_fn_00544380(volatile u32 record, volatile u32 adjustment)
{
    u32 pointer = record;
    s32 delta = (s8)*(volatile u8 *)&adjustment;
    s32 field = (s16)CM_GLOBAL(volatile u16, pointer + 5u);
    long double value, doubled, square;

    record = (u32)field;
    adjustment = (u32)delta;
    value = (long double)(s32)record;
    doubled = (long double)(s32)adjustment;
    doubled += doubled;
    value += doubled;
    value *= CM_GLOBAL(volatile double, 0x00967888u);
    value += CM_GLOBAL(volatile double, 0x009688b0u);
    square = value;
    square *= value;
    square *= CM_GLOBAL(volatile double, 0x00967880u);
    square += value;
    value = square;
    value *= CM_GLOBAL(volatile double, 0x00968a80u);
    value += CM_GLOBAL(volatile double, 0x00968968u);

    if (value > CM_GLOBAL(volatile double, 0x00967890u)) {
        if (value >= CM_GLOBAL(volatile double, 0x00968ea8u)) {
            value = CM_GLOBAL(volatile double, 0x00968ea8u);
            goto convert;
        }
    }
    if (!(value > CM_GLOBAL(volatile double, 0x00967890u)))
        value = CM_GLOBAL(volatile double, 0x00967890u);
convert:
    return (long long)value; /* Original __ftol tail at 0x00544402. */
}
