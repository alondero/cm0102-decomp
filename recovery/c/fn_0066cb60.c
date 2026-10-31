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
extern void CM_FASTCALL cm_fn_00684230(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0066cb60(u32 self, u32 unused __attribute__((unused)))
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x410];
    u32 local = (u32)scratch;
    u32 table, target, schedule, clubs, value, i, pointer, child, holder;
    u32 d9, flag, rounds, year, c4, c3, c2, count, competition;
    u32 name, extension, line, text, result;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095c76e;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    table = D(self);
    B(local) = 2;
    B(local + 1) = 0;
    B(local + 2) = 0;
    B(local + 3) = 0;
    target = D(table + 0x3c);
    schedule = ((u32 (CM_THISCALL *)(u32, u32, u32, u32, u32))target)
        (self, 0, local + 8, local + 4, 0);
    clubs = (u32)cm_fn_00944e46(0x14);
    if (clubs == 0) {
        name = local + 0x10;
        extension = local + 0x210;
        line = 0x245;
        goto error;
    }
    i = 0;
    pointer = clubs;
    do {
        table = D(self + 0xee);
        i += 8;
        pointer += 4;
        value = D(table + i - 4);
        D(pointer - 4) = value;
        table = D(self + 0xee);
        D(table + i - 4) = 0;
    } while ((s32)i < 0x28);
    value = (u32)cm_fn_00944cf1(0xee);
    D(local + 0xc) = value;
    frame.state = 0;
    child = 0;
    if (value != 0) {
        d9 = W(self + 0xd9);
        flag = B(self + 0x51);
        rounds = D(local + 4);
        year = W(self + 0x40);
        c4 = B(self + 0xc4);
        c3 = B(self + 0xc3);
        c2 = B(self + 0xc2);
        count = D(local + 8);
        competition = D(self + 4);
        /* 0068b100 consumes partial-register fields at WORD/BYTE widths;
           see the original loads0068b12a..0068b267 and004204d0. */
        child = cm_fn_0068b100(value, self, 5, clubs, 2, competition,
            schedule, count, c2, c3, c4, self + 0xc5, local, year, 0,
            rounds, flag, 1, 0x82, d9, 0xffffffffu, 0, 2);
    }
    holder = D(self + 0xc);
    frame.state = 0xffffffffu;
    D(holder) = child;
    cm_fn_009452ca(clubs);
    cm_fn_009452ca(schedule);
    holder = D(self + 0xc);
    child = D(holder);
    if (child == 0) {
        name = local + 0x310;
        extension = local + 0x110;
        line = 0x25b;
        goto error;
    }
    cm_fn_00684230(child);
    D(self + 0x30) = 0;
    result = 1;
    goto done;
error:
    cm_fn_00944cff(0x009eb15c, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    result = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
