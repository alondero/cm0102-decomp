/* Preferred-image VA 0x00934380, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl (ret 0, caller pops). Ten dword stack arguments, read at
   entry+4 (a1, object), +8 (a2, record), +0x0c (a3, word), +0x14 (a5, word),
   +0x24 (a9), +0x28 (a10); a4, a6, a7 and a8 are not read. Returns the dword that
   was at a2->[0x1c] + a9*4 before the store, and stores at a2->[0x1c] + a10*4.
   Arithmetic on the words at a1->[0x16] and a1->[0x29c] and the dword at
   a1->[0x2a2] follows the listing (imul, shl 6, sar 1, cdq/idiv). The clamp reads
   the dword table at a1->[0x5c + a3*4]; the 0x448 flag and the 0x2a6 value select
   the final value. Object and record accesses are volatile so -O2 keeps the
   listing's order at a fault. Field meanings remain unknown. */
#include "cmre.h"

u32 CM_CDECL cm_fn_00934380(u8 *a1, u8 *a2, u32 a3, u32 a4, u32 a5,
                            u32 a6, u32 a7, u32 a8, u32 a9, u32 a10)
{
    volatile u8 *self = (volatile u8 *)a1;
    u8 *base;
    u8 *table;
    s32 scale;
    s32 sign;
    s32 clamped;
    s32 eax;
    s32 edx;
    s32 t;

    (void)a4;
    (void)a6;
    (void)a7;
    (void)a8;

    base = *(u8 *volatile *)(self + 0x16);
    scale = (s32)*(volatile s16 *)(self + 0x29c);
    edx = (s32)*(volatile s16 *)(base + a9 * 2u);
    eax = (s32)*(volatile s16 *)(base + a10 * 2u);
    eax = (s32)((u32)eax - (u32)edx);
    edx = scale;
    eax = (s32)((u32)eax * (u32)*(volatile s32 *)(self + 0x2a2));
    eax = (s32)((u32)eax << 6);
    edx = scale >> 1;
    eax = (s32)((u32)eax + (u32)edx);
    eax = eax / scale;

    sign = 1;
    if (eax < 0) {
        eax = (s32)(0u - (u32)eax);
        sign = -1;
    }

    table = *(u8 *volatile *)(a2 + 0x1c);
    clamped = eax;
    if ((s16)a3 >= 0) {
        edx = (s32)*(volatile s32 *)(self + 0x5c + (s32)(s16)a3 * 4);
        if (eax <= edx) {
            t = (s32)((u32)eax + 0x2au);
            clamped = (edx >= t) ? t : edx;
        } else {
            t = (s32)((u32)eax - 0x2au);
            clamped = (edx > t) ? edx : t;
        }
    }

    edx = (s32)*(volatile s32 *)(self + 0x448);
    if (edx != 0) {
        if (clamped < 0x24 || *(volatile s32 *)(self + 0x2a6) == 2) {
            /* eax keeps the quotient */
        } else {
            eax = (s32)(((u32)clamped + 0x20u) & 0xffffffc0u);
        }
    } else {
        eax = (s32)(((u32)clamped + 0x20u) & 0xffffffc0u);
        if (eax == 0 && (s16)a5 != 0) {
            eax = 0x40;
        }
    }

    scale = (s32)(s16)sign;
    edx = (s32)((u32)scale * (u32)eax);
    eax = *(volatile s32 *)(table + a9 * 4u);
    edx = (s32)((u32)edx + (u32)eax);
    *(volatile u32 *)(table + a10 * 4u) = (u32)edx;
    return (u32)eax;
}
