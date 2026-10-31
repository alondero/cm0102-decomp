/* Preferred-image VA 0x009342C0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl (ret 0, caller pops). Ten dword stack arguments, read at
   entry+4 (a1), +8 (a2), +0x0c (a3, word), +0x14 (a5, word), +0x24 (a9),
   +0x28 (a10); a4, a6, a7 and a8 are not read. Returns the dword that was at
   a2->[0x18] + a9*4 before the store, and stores at a2->[0x18] + a10*4.
   Arithmetic on the words at a1->[0x12] and a1->[0x29c] follows the listing
   (imul, shl 6, sar 1, cdq/idiv); the 0x448 flag and the 0x5c table select the
   clamp. Object and record accesses are volatile so -O2 keeps the listing's
   order at a fault. Field meanings remain unknown. */
#include "cmre.h"

u32 CM_CDECL cm_fn_009342c0(u8 *a1, u8 *a2, u32 a3, u32 a4, u32 a5,
                            u32 a6, u32 a7, u32 a8, u32 a9, u32 a10)
{
    volatile u8 *self = (volatile u8 *)a1;
    u8 *base;
    u8 *table;
    s32 scale;
    s32 eax;
    s32 edx;
    s32 ecx_val;
    s32 t;

    (void)a4;
    (void)a6;
    (void)a7;
    (void)a8;

    base = *(u8 *volatile *)(self + 0x12);
    scale = (s32)*(volatile s16 *)(self + 0x29c);
    edx = (s32)*(volatile s16 *)(base + a9 * 2u);
    eax = (s32)*(volatile s16 *)(base + a10 * 2u);
    eax = (s32)((u32)eax - (u32)edx);
    edx = scale;
    eax = (s32)((u32)eax * (u32)*(volatile s32 *)(self + 0x29e));
    eax = (s32)((u32)eax << 6);
    edx = scale >> 1;
    eax = (s32)((u32)eax + (u32)edx);
    eax = eax / scale;

    scale = 1;
    if (eax < 0) {
        eax = (s32)(0u - (u32)eax);
        scale = -1;
    }

    edx = (s32)*(volatile s32 *)(self + 0x448);
    if (edx != 0) {
        if (eax < 0x20 && (s16)a5 != 0) {
            eax = (s32)((u32)eax + (u32)eax);
            if (eax > 0x20) {
                eax = 0x20;
            }
        }
    } else {
        if ((s16)a3 >= 0) {
            ecx_val = (s32)*(volatile s32 *)(self + 0x5c + (s32)(s16)a3 * 4);
            if (eax > ecx_val) {
                t = (s32)((u32)eax - 0x2au);
                eax = (ecx_val > t) ? ecx_val : t;
            } else {
                t = (s32)((u32)eax + 0x2au);
                eax = (ecx_val >= t) ? t : ecx_val;
            }
        }
        eax = (s32)((u32)eax + 0x20u);
        if (((u32)eax & 0xffffffc0u) == 0) {
            if ((s16)a5 != 0) {
                eax = 0x40;
            }
        }
    }

    edx = (s32)(s16)scale;
    table = *(u8 *volatile *)(a2 + 0x18);
    edx = (s32)((u32)edx * (u32)eax);
    eax = *(volatile s32 *)(table + a9 * 4u);
    edx = (s32)((u32)edx + (u32)eax);
    *(volatile u32 *)(table + a10 * 4u) = (u32)edx;
    return (u32)eax;
}
