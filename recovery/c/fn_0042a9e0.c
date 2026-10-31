/* WIP: semantic indirect shape counts delayed EBX save as fifth argument;
   case7 at original VA0042aab5 compares saved EBX against compiled stack tail. */
#include "cmre.h"

/* Preferred-image VA 0x0042a9e0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The virtual builder takes four stack arguments; its last zero was pushed
   before filling the club array. Scratch outputs and the allocation slot stay adjacent. */
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_00522e00(u32, u32, s16, u32, s16, u32, u32, u16,
                                    u8, u8, u16, u8, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0051c800(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))

u32 CM_FASTCALL cm_fn_0042a9e0(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x40c];
    u32 work = (u32)scratch, eax, clubs, source, value, table, target;
    u32 schedule, object, receiver, slot, rounds, competition, round_flag;
    u16 year;
    char *version;

    eax = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095446e;
    frame.prev = eax;
    CM_FS(u32, 0) = (u32)&frame;
    clubs = (u32)cm_fn_00944e46(16);
    if (!clubs) {
        cm_fn_00944cff(0x0098aa30, 0, 0, scratch + 0x20c, scratch + 0xc);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                      scratch + 0x20c, scratch + 0xc, 0x286);
        eax = cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        goto done;
    }
    source = D(self + 0xb1);
    value = D(source);
    D(clubs) = value;
    source = D(self + 0xb1);
    value = D(source + 0xb1);
    D(clubs + 4) = value;
    source = D(self + 0xb1);
    value = D(source + 0x3b);
    D(clubs + 8) = value;
    source = D(self + 0xb1);
    value = D(source + 0x76);
    D(clubs + 12) = value;
    table = D(self);
    target = D(table + 0x3c);
    schedule = ((u32 (CM_THISCALL *)(u32, u32, u32, u32, u32))target)
                   (self, 0, work, work + 4, 0);
    object = (u32)cm_fn_00944cf1(0xb2);
    D(work + 8) = object;
    frame.state = 0;
    if (object) {
        round_flag = D(work + 4);
        year = W(self + 0x40);
        competition = D(self + 4);
        rounds = D(work);
        eax = cm_fn_00522e00(object, self, 4, clubs, (s16)rounds, competition,
                            schedule, year, 0, 1, (u16)round_flag, 20, 0, 0, 0, 0);
    } else {
        eax = 0;
    }
    slot = D(self + 12);
    frame.state = 0xffffffffu;
    D(slot) = eax;
    eax = (u32)cm_fn_009452ca(schedule);
    slot = D(self + 12);
    receiver = D(slot);
    if (!receiver) {
        cm_fn_00944cff(0x0098aa30, 0, 0, scratch + 0x30c, scratch + 0x10c);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                      scratch + 0x30c, scratch + 0x10c, 0x29d);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        eax = (u32)cm_fn_009452ca(clubs);
    } else {
        cm_fn_0051c800(receiver, 0);
        eax = (u32)cm_fn_009452ca(clubs);
        D(self + 0x30) = 0;
    }
done:
    CM_FS(u32, 0) = frame.prev;
    return eax;
}
