#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* Preferred-image VA008ad780/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL8/full boolean EAX. Every subtraction and scaled numerator wraps
   at 32 bits before signed division, as in the original IMUL/SAR sequences.
   Formal slots and output pointers retain the original late read order. */
u32 CM_CDECL cm_fn_008ad780(u32 flags, volatile u32 left, volatile u32 top,
                           volatile u32 right, volatile u32 bottom,
                           volatile u32 x_out, volatile u32 y_out,
                           volatile u32 alternate)
{
    u32 scratch[0x200 / 4];
    u32 l = (u32)scratch, a, b, numerator, value, pointer, mode;
    u32 filename, extension, line, version;
    if ((u8)flags & 1u) {
        a = right;
        b = left;
        value = (u32)((s32)(a - b) / 2) + b;
        a = bottom;
        pointer = x_out;
        a -= 10u;
        D(pointer) = value;
        pointer = y_out;
        D(pointer) = a;
        return 1;
    }
    if ((u8)flags & 2u) {
        a = bottom;
        b = top;
        value = a - (u32)((s32)(a - b) / 12) - 15u;
    } else if ((u8)flags & 4u) {
        a = bottom;
        b = top;
        value = a - (u32)((s32)(a - b) / 6) - 20u;
    } else if ((u8)flags & 8u) {
        a = top;
        b = bottom;
        numerator = (a - b) * 2u;
        value = (u32)((s32)numerator / 6) + b - 10u;
    } else if ((u8)flags & 0x10u) {
        a = top;
        b = bottom;
        numerator = (a - b) * 3u;
        value = (u32)((s32)numerator / 6) + b;
    } else if ((u8)flags & 0x20u) {
        a = top;
        b = bottom;
        numerator = (a - b) * 4u;
        value = (u32)((s32)numerator / 6) + b;
    } else if ((u8)flags & 0x40u) {
        a = top;
        b = bottom;
        numerator = (a - b) * 5u;
        value = (u32)((s32)numerator / 6) + b;
    } else {
        filename = l; extension = l + 0x100; line = 0x21a8;
        goto diagnostic;
    }
    pointer = y_out;
    D(pointer) = value;
    if ((u8)flags & 0x80u) {
        mode = alternate;
        if (mode != 0) {
            a = right;
            b = left;
            value = (u32)((s32)(a - b) / 10) + b;
        } else {
            a = right;
            b = left;
            value = (u32)((s32)(a - b) / 8) + b;
        }
    } else if (flags & 0x100u) {
        mode = alternate;
        b = left;
        a = right;
        numerator = (a - b) * 3u;
        value = (u32)((s32)numerator / (mode != 0 ? 10 : 8)) + b;
    } else if (flags & 0x200u) {
        a = right;
        b = left;
        numerator = (a - b) * 5u;
        value = (u32)((s32)numerator / 10) + b;
    } else if (flags & 0x400u) {
        mode = alternate;
        a = right;
        b = left;
        numerator = (a - b) * (mode != 0 ? 7u : 5u);
        value = (u32)((s32)numerator / (mode != 0 ? 10 : 8)) + b;
    } else if (flags & 0x800u) {
        mode = alternate;
        a = right;
        b = left;
        numerator = (a - b) * (mode != 0 ? 9u : 7u);
        value = (u32)((s32)numerator / (mode != 0 ? 10 : 8)) + b;
    } else {
        filename = l + 0x100; extension = l; line = 0x21dd;
        goto diagnostic;
    }
    pointer = x_out;
    D(pointer) = value;
    return 1;
diagnostic:
    cm_fn_00944cff(0x00a8b94c, 0, 0, filename, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, filename, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
