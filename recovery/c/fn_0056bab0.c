/* WIP: bad-target argument mismatch, case 1 O0/O2. The virtual call's
   inferred fifth argument is entry EBX saved by PUSH EBX at0056bb3c;
   four explicit arguments match. No entry-EBX transport is represented.
   Preferred-image VA 0056bab0, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_00522e00(u32, u32, s16, u32, s16, u32, u32,
    u16, u8, u8, u16, u8, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0056bab0(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x40c];
    u32 previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095815e;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    u32 count = (u32)(s32)(s16)W(self + 0x3e);
    u32 clubs = (u32)cm_fn_00944e46(count * 4);
    u32 result, line;
    u8 *name = scratch + 0x20c;
    u8 *extension = scratch + 0xc;
    if (!clubs) { line = 0x2e9; goto error; }
    u32 copied = 0;
    if ((s16)W(self + 0x3e) > 0) {
        do {
            u32 table = D(self + 0xb2);
            ++copied;
            u32 club = D(table + copied * 4u - 4);
            D(clubs + copied * 4u - 4) = club;
        } while ((s32)copied < (s16)W(self + 0x3e));
    }
    u32 vtable = D(self);
    u32 target = D(vtable + 0x3c);
    u32 schedule = ((u32 (CM_THISCALL *)(u32, u32, void *, void *, u32))target)(
        self, 0, scratch, scratch + 4, 0);
    u32 allocation = (u32)cm_fn_00944cf1(0xb2);
    D((u32)scratch + 8) = allocation;
    frame.state = 0;
    u32 child = 0;
    if (allocation) {
        u8 flag = B(self + 0x51);
        u32 round_flag = D((u32)scratch + 4);
        u16 year = W(self + 0x40);
        u32 competition = D(self + 4);
        u32 rounds = D((u32)scratch);
        s16 size = (s16)W(self + 0x3e);
        child = cm_fn_00522e00(allocation, self, size, clubs, (s16)rounds,
            competition, schedule, year, 0, 1, (u16)round_flag, flag, 0, 0, 0, 0);
    }
    u32 holder = D(self + 0xc);
    frame.state = 0xffffffffu;
    D(holder) = child;
    cm_fn_009452ca(clubs);
    cm_fn_009452ca(schedule);
    holder = D(self + 0xc);
    result = D(holder);
    if (result) {
        D(self + 0x30) = 0;
        goto done;
    }
    name = scratch + 0x30c;
    extension = scratch + 0x10c;
    line = 0x300;
error:
    cm_fn_00944cff(0x009c4180, 0, 0, name, extension);
    u32 text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
