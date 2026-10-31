/* WIP: first canonical test matches all compared cases but coverage .3835.
   Deeper paths need evidenced virtual schedule/advance callees and tied rows. */
#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_0051c800(u32, u32);
extern u32 CM_THISCALL cm_fn_00522e00(u32, u32, s16, u32, s16, u32, u32, u16, u8, u8, u16, u8, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_007eba90(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x214];
    u32 previous, table, value, count, vtable, result, schedule, allocated;
    u32 round_flag, year, competition, rounds, child, holder, text;
    previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x009609de;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    table = D(self + 0xb1);
    count = 0;
    value = W(table + 0x19);
    if (value == W(table + 0x54)) {
        value = D(table);
        count = 2;
        D((u32)scratch + 8) = value;
        value = D(table + 0x3b);
        D((u32)scratch + 12) = value;
        B(table + 0x3a) = 2;
        table = D(self + 0xb1);
        B(table + 0x74) = 1;
        table = D(self + 0xb1);
        B(table + 0x37) = 1;
        table = D(self + 0xb1);
        B(table + 0x72) = 1;
    }
    B(self + 0xee) = (u8)(count / 2u);
    if (count == 0) {
        vtable = D(self);
        D(self + 0x2c) = 0;
        D(self + 0x30) = 0xffffffffu;
        D(self + 0xe6) = 0;
        result = ((u32 (CM_THISCALL *)(u32))D(vtable + 0x94))(self);
        goto done;
    }
    vtable = D(self);
    schedule = ((u32 (CM_THISCALL *)(u32, u32, u32, u32, u32))D(vtable + 0x3c))
        (self, 0, (u32)scratch, (u32)scratch + 4, 0);
    allocated = (u32)cm_fn_00944cf1(0xb2);
    D((u32)scratch + 16) = allocated;
    frame.state = 0;
    child = 0;
    if (allocated != 0) {
        round_flag = D((u32)scratch + 4);
        year = W(self + 0x40);
        competition = D(self + 4);
        rounds = D((u32)scratch);
        child = cm_fn_00522e00(allocated, self, (s16)count, (u32)scratch + 8,
            (s16)rounds, competition, schedule, (u16)year, 0, 1, (u16)round_flag, 0x14, 0, 0, 0, 0);
    }
    holder = D(self + 0xc);
    frame.state = 0xffffffffu;
    D(holder) = child;
    holder = D(self + 0xc);
    if (D(holder) == 0) {
        cm_fn_00944cff(0x00a7acc8, 0, 0, (u32)scratch + 0x114, (u32)scratch + 0x14);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, (u32)scratch + 0x114, (u32)scratch + 0x14, 0x22e);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
    } else {
        cm_fn_009452ca(schedule);
        holder = D(self + 0xc);
        child = D(holder);
        result = cm_fn_0051c800(child, 0);
        D(self + 0x30) = 0;
        B(self + 0xc5) = 6;
    }
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
