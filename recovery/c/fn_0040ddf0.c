#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_00522e00(u32, u32, s16, u32, s16, u32, u32, u16, u8, u8, u16, u8, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0040ddf0(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x40c];
    u32 previous, count, clubs, index, table, value, vtable, schedule, allocated;
    u32 flag, round_flag, year, competition, rounds, child, holder, result, name, extension, line, text;
    previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095385e;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    count = (u32)(s32)(s16)W(self + 0x3e);
    clubs = (u32)cm_fn_00944e46(count * 4u);
    if (clubs == 0) {
        name = (u32)scratch + 0x20c;
        extension = (u32)scratch + 0xc;
        line = 0x2fd;
        goto error;
    }
    index = 0;
    if ((s16)W(self + 0x3e) > 0) {
        do {
            table = D(self + 0xb2);
            value = D(table + 0x40 + index * 4u);
            D(clubs + index * 4u) = value;
            ++index;
            count = (u32)(s32)(s16)W(self + 0x3e);
        } while ((s32)index < (s32)count);
    }
    vtable = D(self);
    schedule = ((u32 (CM_THISCALL *)(u32, u32, u32, u32, u32))D(vtable + 0x3c))
        (self, 0, (u32)scratch, (u32)scratch + 4, 0);
    allocated = (u32)cm_fn_00944cf1(0xb2);
    D((u32)scratch + 8) = allocated;
    frame.state = 0;
    child = 0;
    if (allocated != 0) {
        flag = B(self + 0x51);
        round_flag = D((u32)scratch + 4);
        year = W(self + 0x40);
        competition = D(self + 4);
        rounds = D((u32)scratch);
        count = W(self + 0x3e);
        child = cm_fn_00522e00(allocated, self, (s16)count, clubs, (s16)rounds,
            competition, schedule, (u16)year, 0, 2, (u16)round_flag, (u8)flag, 0, 0, 0, 0);
    }
    holder = D(self + 0xc);
    frame.state = 0xffffffffu;
    D(holder) = child;
    cm_fn_009452ca(clubs);
    cm_fn_009452ca(schedule);
    holder = D(self + 0xc);
    child = D(holder);
    if (child != 0) {
        D(self + 0x30) = 0;
        result = holder;
        goto done;
    }
    name = (u32)scratch + 0x30c;
    extension = (u32)scratch + 0x10c;
    line = 0x314;
error:
    cm_fn_00944cff(0x00987b60, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
