#include "cmre.h"

/* Preferred-image VA 008abe30, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch110: counter/index L0, count BYTE L4, attributes L8..L3f,
   date L40, accepted-person pointers L48..L10f. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u8 CM_THISCALL cm_fn_0062c400(u32, u32);
extern u32 CM_THISCALL cm_fn_0055d270(u32, u32, u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *, u32, u32);
extern u32 CM_THISCALL cm_fn_007757d0(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00896660(u32, u32, u32);
extern u16 CM_THISCALL cm_fn_005b2ae0(u32, u8, u32);
extern u32 CM_CDECL cm_fn_0053b900(u32 *);
extern u32 CM_CDECL cm_fn_006e2000(u32, s8, u32, s8, s8, u32, u32, u32,
                                  u32, u32, u32, u32, u32, u32, u32);

u32 CM_THISCALL cm_fn_008abe30(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x110];
    u32 l = (u32)scratch, eax, i, base, person, owner, value, remaining;
    u32 offset, year, day, club, options;
    u8 count, index, flag;
    frame.state = 0xffffffffu;
    frame.handler = 0x0096378b;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    W(l + 8) = 1; W(l + 0x1e) = 1;
    W(l + 0x22) = 0x10; W(l + 0x24) = 0x40; W(l + 0x26) = 0x40;
    W(l + 0x2a) = 0x10; W(l + 0x2c) = 0x40; W(l + 0x2e) = 0x40;
    W(l + 0x32) = 0x10; W(l + 0x34) = 0x40; W(l + 0x36) = 0x40;
    W(l + 0x3a) = 0x10; W(l + 0x3c) = 0x40; W(l + 0x3e) = 0x40;
    for (i = 0; i < 20; ++i) D(self + 0xdf0 + i * 4) = 0;
    eax = D(self + 0xde5);
    W(l + 0xa) = 0x84; W(l + 0xc) = 0x104; W(l + 0xe) = 0x404;
    W(l + 0x10) = 0x804; W(l + 0x12) = 0x90; W(l + 0x14) = 0x110;
    W(l + 0x16) = 0x410; W(l + 0x18) = 0x810;
    W(l + 0x1a) = 0x140; W(l + 0x1c) = 0x440;
    W(l + 0x20) = 4; W(l + 0x28) = 4; W(l + 0x30) = 4; W(l + 0x38) = 4;
    if (eax == 0) { eax = 0; goto done; }
    eax = D(self + 0xde1);
    if (eax == 0) goto done;
    count = 0;
    offset = 0;
    B(l + 4) = 0;
    D(l) = 50;
    do {
        base = D(self + 0xde5);
        person = D(base + offset + 0xd7);
        if (person != 0) {
            value = D(person + 0x61);
            if (value != 0) {
                owner = D(0x00b63d28);
                flag = cm_fn_0062c400(owner, person);
                if (flag != 2) {
                    value = D(self + 0xda6);
                    owner = D(0x00ae28f0);
                    eax = cm_fn_0055d270(owner, person, value, 1, 0);
                    if (eax == 0) {
                        year = W(self + 0xdba);
                        day = W(self + 0xdbc);
                        cm_fn_00549ef0((void *)(l + 0x40), day, year);
                        club = D(self + 0xde5);
                        owner = D(0x00b74318);
                        frame.state = 0;
                        eax = cm_fn_007757d0(owner, person, l + 0x40, club, 0, 0);
                        if (eax == 0) {
                            index = count;
                            ++count;
                            D(l + 0x48 + (u32)(s32)(s8)index * 4u) = person;
                        }
                        frame.state = 0xffffffffu;
                    }
                }
            }
        }
        remaining = D(l);
        offset += 4;
        --remaining;
        D(l) = remaining;
    } while (remaining != 0);
    B(l + 4) = count;
    if (count == 0) { eax = 0; goto done; }
    club = D(self + 0xde5);
    owner = D(0x00de1d08);
    base = cm_fn_00896660(owner, club, 0);
    if (base != 0) {
        index = 0;
        B(l) = 0;
        i = 0;
        do {
            value = D(l);
            W(l + 8 + i * 2) = cm_fn_005b2ae0(base, (u8)value, 0);
            ++index;
            ++i;
            B(l) = index;
        } while ((s8)index < 11);
    }
    club = D(self + 0xde5);
    eax = cm_fn_0053b900((u32 *)club);
    club = D(self + 0xde5);
    flag = B(self + 0xddf);
    count = (u8)D(l + 4);
    options = eax != 0 ? 0x2014 : 0x2042;
    cm_fn_006e2000(l + 0x48, (s8)count, l + 8, 11, (s8)(flag & 0xf),
                   self + 0xdf0, options, club, 0, 0, 0, 0, 0, 0, 0);
    eax = 1;
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return eax;
}
