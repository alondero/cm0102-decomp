/* Original-instruction authority: CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   All addresses below are preferred-image VAs.
   00439731 supplies the receiver; 004398c9 returns without stack cleanup.
   0043978f and 004397c5..004397c8 push four virtual arguments;
   004397c8 supplies ECX=self and 004397ca calls the unmodified +3c slot.
   The previous hold concerned slot 00968590 -> 00438bf0, missing from
   the target inventory. This translation retains that actual memory dispatch:
   no replacement target, direct-call binding, or invented return is used.
   Target inventory admission remains external evidence, not a C fix.
   Current canonical retest: O0/O2 coverage 0.5612, inc=0; case 0
   explain reports difference={}. Thus the coverage hold is NOT resolved.
   No private hint change is proposed: neither the supplied parent listing
   nor this matching case proves the missing target's function entry.
   004397f3..00439810 push the fifteen constructor arguments, with ECX
   from allocation at 00439811. 0043982f loads the receiver for 0043989a.
   00439862..00439879 retain the error-format arguments across the no-arg
   00439869 call; the combined cleanup is at 0043988e. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_0051c800(u32, u32);
extern u32 CM_THISCALL cm_fn_00522e00(u32, u32, s16, u32, s16, u32, u32,
    u16, u8, u8, u16, u8, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00439710(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x410];
    u32 clubs, table, value, target, schedule, allocated, flag, year, competition;
    u32 rounds, child, holder, result, name, extension, line, text;
    frame.state = 0xffffffffu;
    frame.handler = 0x009549ee;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    B((u32)scratch) = 0;
    B((u32)scratch + 1) = 3;
    B((u32)scratch + 2) = 1;
    B((u32)scratch + 3) = 2;
    clubs = (u32)cm_fn_00944e46(16);
    if (clubs == 0) {
        name = (u32)scratch + 0x10;
        extension = (u32)scratch + 0x210;
        line = 0x21b;
        goto error;
    }
    table = D(self + 0xb1);
    value = D(table);
    D(clubs) = value;
    table = D(self + 0xb1);
    value = D(table + 0xb1);
    D(clubs + 4) = value;
    table = D(self + 0xb1);
    value = D(table + 0x3b);
    D(clubs + 8) = value;
    table = D(self + 0xb1);
    value = D(table + 0x76);
    D(clubs + 0xc) = value;
    table = D(self);
    target = D(table + 0x3c);
    schedule = ((u32 (CM_THISCALL *)(u32, u32, u32, u32, u32))target)
        (self, 0, (u32)scratch + 8, (u32)scratch + 4, 0);
    allocated = (u32)cm_fn_00944cf1(0xb2);
    D((u32)scratch + 0xc) = allocated;
    frame.state = 0;
    child = 0;
    if (allocated != 0) {
        flag = D((u32)scratch + 4);
        year = W(self + 0x40);
        competition = D(self + 4);
        rounds = D((u32)scratch + 8);
        child = cm_fn_00522e00(allocated, self, 4, clubs, (s16)rounds,
            competition, schedule, (u16)year, 0, 1, (u16)flag, 0x14,
            0, 1, 0, (u32)scratch);
    }
    holder = D(self + 0xc);
    frame.state = 0xffffffffu;
    D(holder) = child;
    holder = D(self + 0xc);
    child = D(holder);
    if (child == 0) {
        name = (u32)scratch + 0x310;
        extension = (u32)scratch + 0x110;
        line = 0x22f;
        goto error;
    }
    cm_fn_0051c800(child, 0);
    cm_fn_009452ca(schedule);
    result = (u32)cm_fn_009452ca(clubs);
    D(self + 0x30) = 0;
    goto done;
error:
    cm_fn_00944cff(0x0098afd8, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
