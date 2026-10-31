/* WIP: coverage .2846; actual vtable slot009677a4 targets004111a0,
   which is absent from the inventory and stops as interior. Four failed
   tests include two fixture-schema rejections; no comparison mismatches. */
#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_0068b100(u32, u32, u32, u32, u32, u32,
    u32, u32, u32, u32, u32, u32, u32, u32, u32, u32,
    u32, u32, u32, u32, u32, u32, u32);
extern void CM_THISCALL cm_fn_00684230(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* 0068b100 consumes the partial-register arguments at their stored widths:
   BYTE flags, WORD year/options/count. See original VA0068b12a/151/15f/176/
   1ce/1d6; the upper bits are overwritten before any wider use. */
u32 CM_THISCALL cm_fn_00412010(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x228];
    u32 previous, table, value, vtable, schedule, allocated, child, nineteenth, flag51, option, year, c4, c3, c2, aux, competition, holder, result, text;
    previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00953a1e;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    B((u32)scratch) = 2;
    B((u32)scratch + 1) = 0;
    table = D(self + 0xb1);
    B((u32)scratch + 2) = 0;
    B((u32)scratch + 3) = 0;
    value = D(table);
    D((u32)scratch + 12) = value;
    value = D(table + 0x3b);
    D((u32)scratch + 16) = value;
    value = D(table + 0x76);
    D((u32)scratch + 20) = value;
    value = D(table + 0xb1);
    D((u32)scratch + 24) = value;
    value = D(table + 0xec);
    D((u32)scratch + 28) = value;
    value = D(table + 0x127);
    vtable = D(self);
    D((u32)scratch + 32) = value;
    schedule = ((u32 (CM_THISCALL *)(u32, u32, u32, u32, u32))D(vtable + 0x3c))
        (self, 0, (u32)scratch + 8, (u32)scratch + 4, 0);
    allocated = (u32)cm_fn_00944cf1(0xee);
    D((u32)scratch + 0x24) = allocated;
    frame.state = 0;
    child = 0;
    if (allocated != 0) {
        nineteenth = W(self + 0xd9);
        flag51 = B(self + 0x51);
        option = D((u32)scratch + 4);
        year = W(self + 0x40);
        c4 = B(self + 0xc4);
        c3 = B(self + 0xc3);
        c2 = B(self + 0xc2);
        aux = D((u32)scratch + 8);
        competition = D(self + 4);
        child = cm_fn_0068b100(allocated, self, 6, (u32)scratch + 12, 1,
            competition, schedule, aux, c2, c3, c4, self + 0xc5, (u32)scratch,
            year, 0, option, flag51, 1, 0, nineteenth, 0xffffffffu, 0, 2);
    }
    holder = D(self + 0xc);
    frame.state = 0xffffffffu;
    D(holder) = child;
    cm_fn_009452ca(schedule);
    holder = D(self + 0xc);
    child = D(holder);
    if (child == 0) {
        cm_fn_00944cff(0x00987c20, 0, 0, (u32)scratch + 0x128, (u32)scratch + 0x28);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, (u32)scratch + 0x128, (u32)scratch + 0x28, 0x328);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        result = 0;
    } else {
        cm_fn_00684230(child);
        D(self + 0x30) = 0;
        result = 1;
    }
    CM_FS(u32, 0) = frame.prev;
    return result;
}
