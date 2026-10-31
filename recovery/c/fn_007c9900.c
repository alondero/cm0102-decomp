/* WIP: one canonical failure, coverage0.1391/inc0. Original constructor
   vtable0096e360 slot0096e39c targets007c8df0, absent from inventory. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_0068b100(u32, u32, u32, u32, u32, u32,
    u32, u32, u32, u32, u32, u32, u32, u32, u32, u32,
    u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_FASTCALL cm_fn_007c9900(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x40c];
    u32 local = (u32)scratch;
    u32 table, target, schedule, count, clubs, index, cursor, base, value;
    u32 allocated, d9, flag, enumeration, year, c4, c3, c2, rounds;
    u32 competition, holder, result, name, extension, line, text;
    frame.prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095f77e;
    CM_FS(u32, 0) = (u32)&frame;
    table = D(self);
    target = D(table + 0x3c);
    schedule = ((u32 (CM_THISCALL *)(u32, u32, u32, u32, u32))target)
        (self, 0, local, local + 4, 0);
    count = (u32)(s32)(s16)W(self + 0x3e);
    clubs = (u32)cm_fn_00944e46(count * 4u);
    if (clubs == 0) {
        name = local + 0x20c;
        extension = local + 0xc;
        line = 0x226;
        goto error;
    }
    index = 0;
    if ((s16)W(self + 0x3e) > 0) {
        cursor = clubs;
        do {
            base = D(self + 0xee);
            value = D(base + 0x20u + (cursor - clubs));
            ++index;
            D(cursor) = value;
            cursor += 4;
            count = (u32)(s32)(s16)W(self + 0x3e);
        } while ((s32)index < (s32)count);
    }
    allocated = (u32)cm_fn_00944cf1(0xee);
    D(local + 8) = allocated;
    frame.state = 0;
    value = 0;
    if (allocated != 0) {
        d9 = W(self + 0xd9);
        flag = B(self + 0x51);
        enumeration = D(local + 4);
        year = W(self + 0x40);
        c4 = B(self + 0xc4);
        c3 = B(self + 0xc3);
        c2 = B(self + 0xc2);
        rounds = D(local);
        competition = D(self + 4);
        count = W(self + 0x3e);
        /* Original0068b100 consumes the partial-register arguments only
           at its BYTE/WORD field widths. Keep the original DWORD locals. */
        value = cm_fn_0068b100(allocated, self, count, clubs, 2, competition,
            schedule, rounds, c2, c3, c4, self + 0xc5, self + 0xbe,
            year, 0, enumeration, flag, 1, 0, d9, 0xffffu, 0, 2);
    }
    holder = D(self + 0xc);
    frame.state = 0xffffffffu;
    D(holder) = value;
    holder = D(self + 0xc);
    if (D(holder) != 0) {
        cm_fn_009452ca(clubs);
        result = (u32)cm_fn_009452ca(schedule);
        D(self + 0x30) = 0;
        goto done;
    }
    name = local + 0x30c;
    extension = local + 0x10c;
    line = 0x238;
error:
    cm_fn_00944cff(0x00a78598, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
