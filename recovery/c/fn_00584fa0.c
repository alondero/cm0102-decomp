#include "cmre.h"

/* Preferred-image VA 0x00584fa0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Four fixed six-byte record groups retain their distinct limits, offsets,
   flags, counter slots and late table reloads. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00584fa0(u32 self)
{
    u8 scratch[0x214];
    u32 local = (u32)scratch, old, count, allocated, total, club, key, wanted;
    u32 off1, off2 = 0, off3, off4, offset, counter, flag, base, value;
    u32 scan, a, b, c, d, text;
    u8 kind;
    W(self + 0x36) = 0x47;
    W(self + 0x38) = 0x47;
    old = D(self + 0x14);
    D(local) = 0;
    D(local + 8) = 0;
    D(local + 12) = 0;
    D(local + 4) = 0;
    if (old != 0) cm_fn_009452ca(old);
    count = (u32)(s32)(s16)W(self + 0x36);
    allocated = cm_fn_00944e46(count * 6u);
    D(self + 0x14) = allocated;
    if (allocated == 0) {
        cm_fn_00944cff(0x009c4900, 0, 0, local + 0x114, local + 0x14);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x114, local + 0x14, 0x44e);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    total = D(0x00ae235c);
    club = D(0x00ae23b4);
    D(local + 0x10) = 0;
    if ((s32)total > 0) {
        off1 = off3 = off4 = 0;
        do {
            key = D(club + 0x1db);
            wanted = D(0x009cf6e8);
            if (key != wanted) goto next;
            kind = B(club + 0x1df);
            if (kind == 1) {
                if ((s32)off1 >= 0x60) goto next;
                offset = off1; flag = 7; counter = local + 4;
            } else if (kind == 2) {
                if ((s32)off2 >= 0x6c) goto next;
                offset = off2 + 0x60; flag = 8; counter = local + 12;
            } else if (kind == 3) {
                if ((s32)off3 >= 0x72) goto next;
                offset = off3 + 0xcc; flag = 13; counter = local + 8;
            } else if (kind == 4) {
                if ((s32)off4 >= 0x6c) goto next;
                offset = off4 + 0x13e; flag = 0; counter = local;
            } else {
                goto next;
            }
            base = D(self + 0x14);
            D(base + offset) = club;
            base = D(self + 0x14);
            B(base + offset + 5) = 0;
            base = D(self + 0x14);
            B(base + offset + 4) = (u8)flag;
            value = D(counter);
            ++value;
            if (kind == 1) off1 += 6;
            else if (kind == 2) off2 += 6;
            else if (kind == 3) off3 += 6;
            else off4 += 6;
            D(counter) = value;
next:
            scan = D(local + 0x10);
            club += 581;
            ++scan;
            D(local + 0x10) = scan;
            total = D(0x00ae235c);
        } while ((s32)(s16)(u16)scan < (s32)total);
        off2 = 0;
    }
    a = D(local + 4);
    b = D(local + 12);
    c = D(local + 8);
    d = D(local);
    total = a + b;
    total += c;
    total += d;
    count = (u32)(s32)(s16)W(self + 0x36);
    if (total == count) return 1;
    W(self + 0x36) = (u16)off2;
    return 0;
}
