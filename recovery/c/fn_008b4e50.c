/* Preferred-image VA 0x008B4E50, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object) with two byte stack arguments, ret 8.
   The first argument is sign-extended and stored as a dword at +0x00; the
   second is stored as a byte at +0x15 and, sign-extended, selects one of eight
   jump-table entries at 0x008B4F08 after an unsigned compare with 7 (ja), so
   negative or larger values fall through with no further stores. Table:
   0 -> zero fill, 1 -> bytes +4..+7 = 0x19 and +8 = 0, 2 -> +4 = 0x32, +5..+7 =
   0x0A, +8 = 0, 3 -> +6 = 0x32 and the others as case 2, 4 -> +7 = 0x32,
   5 -> +5 = 0x32, 6 -> +6 = 0, +8 = 0x32 and +4, +5, +7 = 0x0A, 7 -> zero fill.
   Zero fill stores bytes +4..+8 = 0. Stores are volatile so -O2 keeps the
   listing's order at a fault. Field meanings remain unknown. */
#include "cmre.h"

void CM_THISCALL cm_fn_008b4e50(u8 *self, s8 arg1, s8 arg2)
{
    volatile u8 *base = (volatile u8 *)self;

    *(volatile s32 *)(base + 0x00) = (s32)arg1;
    base[0x15] = (u8)arg2;
    switch ((u32)(s32)arg2) {
    case 0:
    case 7:
        base[0x04] = 0;
        base[0x05] = 0;
        base[0x06] = 0;
        base[0x07] = 0;
        base[0x08] = 0;
        break;
    case 1:
        base[0x08] = 0;
        base[0x04] = 0x19;
        base[0x05] = 0x19;
        base[0x06] = 0x19;
        base[0x07] = 0x19;
        break;
    case 2:
        base[0x04] = 0x32;
        base[0x05] = 0x0a;
        base[0x06] = 0x0a;
        base[0x07] = 0x0a;
        base[0x08] = 0;
        break;
    case 3:
        base[0x06] = 0x32;
        base[0x04] = 0x0a;
        base[0x05] = 0x0a;
        base[0x07] = 0x0a;
        base[0x08] = 0;
        break;
    case 4:
        base[0x07] = 0x32;
        base[0x04] = 0x0a;
        base[0x05] = 0x0a;
        base[0x06] = 0x0a;
        base[0x08] = 0;
        break;
    case 5:
        base[0x05] = 0x32;
        base[0x04] = 0x0a;
        base[0x06] = 0x0a;
        base[0x07] = 0x0a;
        base[0x08] = 0;
        break;
    case 6:
        base[0x06] = 0;
        base[0x04] = 0x0a;
        base[0x05] = 0x0a;
        base[0x07] = 0x0a;
        base[0x08] = 0x32;
        break;
    default:
        break;
    }
}
