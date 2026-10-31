#include "cmre.h"
/* Preferred-image VA0055e300, base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS3/RET12/fullEAX0or1; SEH957ff8/scratch2c: world L0, datesL8/L10,
   20B record L18 (embedded dateL1c), date-copy outputL0.
   Record flag L2b starts partially unknown: root sets bits1/2. 556940's
   mandatory fourth index defines bit0 on returning paths. 556c70 copies
   bits0..2 only; 557280 masks each of these bits. Upper bits stay unknown.
   556d00 reads only lowWORD of the partially updated EAX argument. */
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define G(a) CM_GLOBAL(volatile u32, a)
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_007e82e0(u32, u32, u32);
extern void CM_THISCALL cm_fn_00556d00(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00669bd0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054cdb0(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern void CM_THISCALL cm_fn_00556940(u32, u32, u32);
extern void CM_STDCALL cm_fn_00557280(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_005487b0(u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern void CM_THISCALL cm_fn_00758de0(u32, u32, u32, u32, u32, u32, u32);

u32 CM_THISCALL cm_fn_0055e300(u32 self, volatile u32 person_arg,
                              volatile u32 other_arg,
                              u32 unused_arg __attribute__((unused)))
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x2c];
    u32 l = (u32)scratch, person, id, base, world, cursor, a, b, p, q;
    u32 count, target, club, map, amount, kind;
    u8 byte, flag, status, trait, sum;
    frame.state = 0xffffffffu; frame.handler = 0x00957ff8;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    cm_fn_0054a110(l + 0x10);
    frame.state = 0;
    cm_fn_0054a110(l + 8);
    person = person_arg;
    B((u32)&frame.state) = 1;
    id = D(person); base = G(0x00ae23bc);
    D(l) = base + id * 110u;
    byte = B(person + 0x2e);
    if (byte != 12 && B(person + 0x2f) != 12) {
        CM_FS(volatile u32, 0) = frame.prev;
        return 0;
    }
    person_arg = 4;
    base = D(self + 0x14); cursor = base + id * 29u;
    do {
        byte = B(cursor);
        if (byte != 0xff) {
            base = D(self);
            target = D(base + (u32)(s32)(s8)byte * 4u);
            id = D(person);
            a = cm_fn_007e82e0(target, id, 0);
            if (a != 0) {
                a = (a & 0xffff0000u) | W(a + 6);
                if ((s16)(u16)a > -1) {
                    id = D(person);
                    cm_fn_00556d00(self, id, a);
                }
            }
        }
        count = person_arg; cursor += 5; --count; person_arg = count;
    } while (count != 0);
    cm_fn_0054a110(l + 0x1c);
    world = D(l);
    B(l + 0x18) = 0xff;
    W(l + 0x1a) = 90;
    p = D(world + 0x39);
    B((u32)&frame.state) = 2;
    cm_fn_00669bd0(p, l + 0x10, l + 8);
    b = G(0x00ae2c94); a = G(0x00ae2c90);
    a = cm_fn_0054cdb0(l + 8, a, b);
    if ((s32)a < 80) W(l + 0x1a) = 180;
    B(l + 0x19) = 0;
    cm_fn_00417c10(l + 0x1c, l, 0x00ae2c90);
    flag = B(l + 0x2b); byte = B(person + 0x48);
    flag |= 2;
    id = D(person);
    flag = (flag & 0xfbu) | (byte == 0 ? 4u : 0u);
    B(l + 0x28) = 3;
    base = D(self);
    W(l + 0x29) = 0;
    B(l + 0x24) = 0xff; B(l + 0x25) = 0xff;
    B(l + 0x26) = 0xff; B(l + 0x27) = 0;
    B(l + 0x2b) = flag;
    target = D(base);
    cm_fn_007e82e0(target, id, 1);
    id = D(person); target = G(0x00ae28f0);
    cm_fn_00556940(target, id, l + 0x18);
    id = D(person); q = other_arg;
    base = G(0x00ae23bc); p = base + id * 110u;
    target = G(0x00ae28f0);
    /* 557280 overwrites incoming ECX before any use; RET12, three stackargs. */
    cm_fn_00557280(p, l + 0x18, q);
    club = D(world + 0x39);
    if (club == 0) goto done;
    if ((s16)W(club + 0x80) <= 0x1482) goto done;
    if (D(club + 0xcf) == 0) goto done;
    if (cm_fn_00600a50(club, 0, 1, 0, 0) != 0) goto done;
    if ((u8)cm_fn_005487b0(club) == 0) goto done;
    id = D(club); map = G(0x00ae2d14);
    if ((s8)B(map + id * 6u + 4) >= -3) goto done;
    p = G(0x00b67a44); base = D(p);
    if ((s16)W(base + id * 73u + 6) >= 0x109a) goto done;
    if (cm_fn_0090cfc0(20) == 0) {
        amount = D(l + 0x1a); kind = 1; goto notify;
    }
    id = D(club); p = D(club + 0xcf); map = G(0x00ae2d14);
    trait = B(p + 0x5a); status = B(map + id * 6u + 4);
    if ((s32)(s8)status + (s32)(s8)trait < 10) {
        sum = (u8)(B(p + 0x5b) + trait);
        if (cm_fn_0090cfc0((u32)(s32)(s8)sum) == 0) {
            amount = 0xffffffffu; kind = 3; goto notify;
        }
    }
    p = D(club + 0xcf);
    if ((s8)B(p + 0x5a) <= 10) goto done;
    if ((s8)B(p + 0x57) <= 10) goto done;
    if (cm_fn_0090cfc0(20) != 0) goto done;
    amount = 0xffffffffu; kind = 2;
notify:
    target = G(0x00b6b7c8);
    cm_fn_00758de0(target, club, world, kind, 0xffffffffu, amount, 0xffffffffu);
done:
    a = frame.prev;
    CM_FS(volatile u32, 0) = a;
    return 1;
}
