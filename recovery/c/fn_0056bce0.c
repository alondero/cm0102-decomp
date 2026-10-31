#include "cmre.h"

/* Preferred-image VA 0x0056bce0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Unmatched scans leave the allocated club slots untouched. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_0051c410(u32, u32);
extern u32 CM_THISCALL cm_fn_00522e00(u32, u32, s16, u32, s16, u32, u32,
    u16, u8, u8, u16, u8, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
typedef u32 (CM_THISCALL *ScheduleCall)(u32, u32, u32, u32, u32);

u32 CM_THISCALL cm_fn_0056bce0(u32 self)
{
    u32 previous = CM_FS(u32, 0);
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x40c];
    u32 local = (u32)scratch, clubs, table, row, index, value, schedule, object;
    u32 target, flags, rounds, owner, result, name, extension, line, text;
    s32 count;
    u16 year;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095817e;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    clubs = cm_fn_00944e46(8);
    if (clubs == 0) {
        name = local + 0x20c; extension = local + 0xc; line = 0x337;
        goto error;
    }
    count = CM_GLOBAL(volatile s16, self + 0x3e);
    if (count > 0) {
        table = D(self + 0xa7);
        row = table + 5;
        for (index = 0; (s32)index < count; ++index, row += 6) {
            if (B(row) == 1) {
                value = D(table + index * 6u);
                D(clubs) = value;
                break;
            }
        }
    }
    count = CM_GLOBAL(volatile s16, self + 0x3e);
    if (count > 0) {
        value = D(self + 0xc);
        value = D(value);
        table = D(value + 0xa7);
        row = table + 5;
        for (index = 0; (s32)index < count; ++index, row += 6) {
            if (B(row) == 1) {
                value = D(table + index * 6u);
                D(clubs + 4) = value;
                break;
            }
        }
    }
    table = D(self);
    target = D(table + 0x3c);
    schedule = ((ScheduleCall)target)(self, 1, local, local + 4, 0);
    object = cm_fn_00944cf1(0xb2);
    D(local + 8) = object;
    frame.state = 0;
    if (object != 0) {
        flags = D(local + 4);
        year = W(self + 0x40);
        owner = D(self + 4);
        rounds = D(local);
        result = cm_fn_00522e00(object, self, 2, clubs, (s16)rounds, owner,
                    schedule, year, 1, 1, (u16)flags, 0x14, 0, 0, 0, 0);
    } else result = 0;
    table = D(self + 0xc);
    frame.state = 0xffffffffu;
    D(table + 4) = result;
    cm_fn_009452ca(schedule);
    table = D(self + 0xc);
    object = D(table + 4);
    if (object != 0) {
        cm_fn_0051c410(object, 0);
        result = cm_fn_009452ca(clubs);
        goto done;
    }
    name = local + 0x30c; extension = local + 0x10c; line = 0x35a;
error:
    cm_fn_00944cff(0x009c4180, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
done:
    previous = frame.prev;
    CM_FS(u32, 0) = previous;
    return result;
}
