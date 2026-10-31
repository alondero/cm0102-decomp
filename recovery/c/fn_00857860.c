/* WIP: coverage .2881; actual vtable slot0096fcc0 targets00856260,
   which is absent from the inventory and stops as interior. Four failed
   tests include two fixture-schema rejections; no comparison mismatches. */
#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))

extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_00522e00(u32, u32, s16, u32, s16, u32, u32, u16, u8, u8, u16, u8, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0051c410(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00857860(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x21c];
    u32 previous, table, holder, object, value, vtable, schedule, allocated, child, round_flag, year, competition, rounds, result, text;
    previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0096282e;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    table = D(self + 0xb1);
    value = D(table + 0x375);
    holder = D(self + 0xc);
    D((u32)scratch + 8) = value;
    object = D(holder);
    table = D(object + 0xb1);
    value = D(table + 0x375);
    D((u32)scratch + 12) = value;
    object = D(holder + 4);
    table = D(object + 0xb1);
    value = D(table + 0x375);
    D((u32)scratch + 16) = value;
    object = D(holder + 8);
    table = D(object + 0xb1);
    vtable = D(self);
    value = D(table + 0x375);
    D((u32)scratch + 20) = value;
    schedule = ((u32 (CM_THISCALL *)(u32, u32, u32, u32, u32))D(vtable + 0x3c))
        (self, 7, (u32)scratch, (u32)scratch + 4, 0);
    allocated = (u32)cm_fn_00944cf1(0xb2);
    D((u32)scratch + 0x18) = allocated;
    frame.state = 0;
    child = 0;
    if (allocated != 0) {
        round_flag = D((u32)scratch + 4);
        year = W(self + 0x40);
        competition = D(self + 4);
        rounds = D((u32)scratch);
        child = cm_fn_00522e00(allocated, self, 4, (u32)scratch + 8, (s16)rounds,
            competition, schedule, (u16)year, 7, 1, (u16)round_flag, 0x14, 0, 0, 0, 0);
    }
    holder = D(self + 0xc);
    frame.state = 0xffffffffu;
    D(holder + 0x1c) = child;
    holder = D(self + 0xc);
    child = D(holder + 0x1c);
    if (child == 0) {
        cm_fn_00944cff(0x00a828f8, 0, 0, (u32)scratch + 0x11c, (u32)scratch + 0x1c);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, (u32)scratch + 0x11c, (u32)scratch + 0x1c, 0x33d);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
    } else {
        cm_fn_009452ca(schedule);
        holder = D(self + 0xc);
        child = D(holder + 0x1c);
        result = cm_fn_0051c410(child, 0);
        previous = frame.prev;
        D(self + 0x30) = 7;
        CM_FS(u32, 0) = previous;
        return result;
    }
    CM_FS(u32, 0) = frame.prev;
    return result;
}
