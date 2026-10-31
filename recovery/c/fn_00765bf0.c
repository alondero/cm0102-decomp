#include "cmre.h"

/* Preferred-image VA 0x00765bf0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_00518640(u32);
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00766290(u32, u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0049ee70(u32, u32);
extern u32 CM_THISCALL cm_fn_00518790(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00765bf0(u32 self, volatile u32 sources,
    volatile u32 competition, volatile u32 date, volatile u32 year,
    volatile u32 flag, volatile u32 last)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x20c];
    u32 local = (u32)scratch, prev, date_field, selector, comp, date_arg;
    u32 source, club, person, value, vtable, target, name, ext, line, text;
    u16 year_word, last_word;
    u8 flag_byte;
    prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095da5a;
    frame.prev = prev;
    CM_FS(u32, 0) = (u32)&frame;
    D(local + 8) = self;
    cm_fn_00518640(self);
    date_field = self + 0xb2;
    frame.state = 0;
    cm_fn_0054a110(date_field);
    year_word = (u16)year;
    comp = competition;
    flag_byte = (u8)flag;
    W(self + 0x40) = year_word;
    date_arg = date;
    D(self + 4) = comp;
    selector = self + 0x3a;
    *(volatile u8 *)&frame.state = 1;
    D(self) = 0x0096d8ac;
    D(self + 0x30) = 0xffffffffu;
    D(self + 0x2c) = 0;
    B(self + 0x44) = 0xff;
    B(self + 0x43) = 1;
    B(self + 0xb1) = 0;
    B(self + 0xab) = 0;
    B(self + 0x42) = flag_byte;
    W(selector) = 0xffff;
    cm_fn_00417c10(date_field, local, date_arg);
    last_word = (u16)last;
    source = sources;
    W(self + 0xba) = last_word;
    B(self + 0x49) = 9;
    B(self + 0x4a) = 9;
    B(self + 0xac) = 0;
    club = D(source);
    person = D(club + 0x69);
    if (person != 0) {
        value = D(person);
        D(self + 0x24) = value;
    } else {
        D(self + 0x24) = 0xffffffffu;
    }
    value = cm_fn_00766290(self, source);
    if (value == 0) {
        name = local + 0x10c; ext = local + 0x0c; line = 0x3c;
        goto error;
    }
    value = cm_fn_00944cf1(0x5ce);
    D(local) = value;
    *(volatile u8 *)&frame.state = 2;
    if (value != 0) value = cm_fn_0049ee70(value, self);
    *(volatile u8 *)&frame.state = 1;
    D(self + 8) = value;
    if (value == 0) {
        name = local + 0x0c; ext = local + 0x10c; line = 0x44;
        goto error;
    }
    vtable = D(self);
    target = D(vtable + 0x3c);
    value = ((u32 (CM_THISCALL *)(u32, u32, u32, u32, u32))target)
        (self, 0xffffffffu, self + 0x3c, selector, 0);
    D(self + 0xa3) = value;
    value = cm_fn_00518790(self);
    if ((s32)value >= 0) goto done;
    name = local + 0x0c; ext = local + 0x10c; line = 0x4e;
error:
    cm_fn_00944cff(0x00a5ab00, 0, 0, name, ext);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, ext, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return self;
}
