/* WIP: partial AX return; upper EAX half is the caller's entry value in the original
   (mov ax, imm and fnstsw ax leave bits 16-31 alone) and the caller at 0x004A4F7A reads
   EAX. Plain C cannot preserve the entry EAX upper half. Declared u32 so the width check
   passes; the first mismatch is case 0 regs EAX (original 0xc6090040, recovered 0x40), and
   the only difference is the upper 16 bits.

   Preferred-image VA 0x004AE4F0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl (ret 0), one stack argument (a record pointer), result in AX.
   Byte compares use the 0x14 value in DL; +0x15, +0x17, +0x18 and +0x19 are
   signed. The x87 compare multiplies the signed byte +0x11 by the double at
   0x00968940 (draft value 1.5, not yet confirmed from the bytes). Object reads are
   volatile so the read order matches the listing. Field meanings remain unknown. */
#include "cmre.h"

u32 cm_fn_004ae4f0(u8 *record)
{
    volatile s8 *r = (volatile s8 *)record;
    s8 al;
    s32 p11;
    s32 p12;
    s32 p14;
    s32 sum;

    if (r[0x0f] == 0x14) {
        return 1;
    }
    if (r[0x10] == 0x14 || r[0x11] == 0x14 || r[0x16] == 0x14) {
        al = r[0x13];
        if (al != 0x14 && r[0x12] != 0x14 && r[0x14] != 0x14) {
            return 4;
        }
        p11 = r[0x11];
        p12 = r[0x12];
        p14 = r[0x14];
        sum = p14 + p12;
        sum = sum + (s32)al;
        if ((double)sum < (double)p11 * 1.5) {
            return 4;
        }
    }
    if (r[0x13] != 0x14 && r[0x12] != 0x14 && r[0x14] != 0x14) {
        return 0x40;
    }
    if (r[0x15] < 0x14) {
        return 0x10;
    }
    if (r[0x19] < r[0x18]) {
        return 0x10;
    }
    if (r[0x19] < r[0x17]) {
        return 0x10;
    }
    return 0x40;
}
