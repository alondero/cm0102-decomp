/* Preferred-image VA 0x006BAB90, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: ECX is the object (read before written), no stack arguments, ret 0.
   Returns a byte in AL: 0 or 1; the reset path returns 0.
   Logic, in listing order:
   - byte +0x475a == 0 -> return 0.
   - dword p = +0x4761; p == 0 -> return 0.
   - dl = byte p+0x27; is_zero = (dl == 0). The jle at 0x6babb5 is never taken
     (AL is 0 or 1 against -1).
   - dword q = +0xf582; q == 0 -> return 1.
   - (signed byte q+0x27) != is_zero -> return 1.
   - a = signed byte +0x8ea8, b = signed byte +0x475c.
     reset when (a < b and dl == 0) or (a > b and dl == 1); otherwise return 1
     (a == b always returns 1).
   - reset: dword +0x4761 = 0, word +0x475f = 0xffff, dword +0x4765 = 0,
     byte +0x4769 = 0, byte +0x475a = 0, bytes +0x475b, +0x475c, +0x475d,
     +0x475e = 0xff; return 0.
   Field meanings remain unknown. Object accesses are volatile so -O2 keeps the
   listing's store order if a fault occurs. */
#include "cmre.h"

u8 CM_THISCALL cm_fn_006bab90(u8 *object)
{
    volatile u8 *self = (volatile u8 *)object;
    u8 *p;
    u8 *q;
    u8 dl;
    s32 is_zero;
    s8 a;
    s8 b;

    if (self[0x475a] == 0) {
        return 0;
    }
    p = *(u8 *volatile *)(self + 0x4761);
    if (p == 0) {
        return 0;
    }
    dl = p[0x27];
    is_zero = (dl == 0);
    q = *(u8 *volatile *)(self + 0xf582);
    if (q == 0) {
        return 1;
    }
    if ((s32)(s8)q[0x27] != is_zero) {
        return 1;
    }
    a = (s8)self[0x8ea8];
    b = (s8)self[0x475c];
    if ((a < b && dl == 0) || (a > b && dl == 1)) {
        *(volatile u32 *)(self + 0x4761) = 0;
        *(volatile u16 *)(self + 0x475f) = 0xffff;
        *(volatile u32 *)(self + 0x4765) = 0;
        self[0x4769] = 0;
        self[0x475a] = 0;
        self[0x475b] = 0xff;
        self[0x475c] = 0xff;
        self[0x475d] = 0xff;
        self[0x475e] = 0xff;
        return 0;
    }
    return 1;
}
