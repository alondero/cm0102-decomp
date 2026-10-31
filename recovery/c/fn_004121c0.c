/* WIP: canonical case0 virtual boundary infers a fifth argument from saved
   incoming EBX, though the original pushes four. Coverage .5038/inc0;
   one failed invocation. Do not invent a fifth source argument. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
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

u32 CM_THISCALL cm_fn_004121c0(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x40c];
    u32 previous, clubs, table, parent, row, value, target, schedule, allocated;
    u32 flag, year, competition, rounds, child, holder, result, name, extension, line, text;
    previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00953a3e;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    clubs = (u32)cm_fn_00944e46(8);
    if (clubs == 0) {
        name = (u32)scratch + 0x20c;
        extension = (u32)scratch + 0xc;
        line = 0x348;
        goto error;
    }
    holder = D(self + 0xc);
    parent = D(holder);
    table = D(parent + 0xb1);
    value = D(table);
    D(clubs) = value;
    holder = D(self + 0xc);
    parent = D(holder);
    table = D(parent + 0xb1);
    row = D(table + 0x3b);
    D(clubs + 4) = row;
    table = D(self);
    target = D(table + 0x3c);
    schedule = ((u32 (CM_THISCALL *)(u32, u32, u32, u32, u32))target)
        (self, 1, (u32)scratch, (u32)scratch + 4, 0);
    allocated = (u32)cm_fn_00944cf1(0xb2);
    D((u32)scratch + 8) = allocated;
    frame.state = 0;
    child = 0;
    if (allocated != 0) {
        flag = D((u32)scratch + 4);
        year = W(self + 0x40);
        competition = D(self + 4);
        rounds = D((u32)scratch);
        child = cm_fn_00522e00(allocated, self, 2, clubs, (s16)rounds,
            competition, schedule, (u16)year, 1, 1, (u16)flag, 0x14, 0, 0, 0, 0);
    }
    holder = D(self + 0xc);
    frame.state = 0xffffffffu;
    D(holder + 4) = child;
    result = (u32)cm_fn_009452ca(schedule);
    holder = D(self + 0xc);
    child = D(holder + 4);
    if (child == 0) {
        name = (u32)scratch + 0x30c;
        extension = (u32)scratch + 0x10c;
        line = 0x35c;
        goto error;
    }
    cm_fn_0051c800(child, 0);
    result = (u32)cm_fn_009452ca(clubs);
    goto done;
error:
    cm_fn_00944cff(0x00987c20, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
