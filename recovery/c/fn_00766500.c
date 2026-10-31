#include "cmre.h"

/* Preferred-image VA00766500, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS11/fullEAX self; SEH95dafa, scratch20c: date/cached allocationL0,
   savedselfL8, diagnosticsLc/L10c. VT96d94c slot96d988 ->766970, THIS4
   RET16/fullEAX, writes WORD output fields. Keep actual indirect call. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_THISCALL cm_fn_00682200(u32);
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00766bf0(u32, u32);
extern u32 CM_THISCALL cm_fn_006835c0(u32);
extern u32 CM_THISCALL cm_fn_006827d0(u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0049ee70(u32, u32);
extern u32 CM_THISCALL cm_fn_0068a850(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00766500(u32 self, volatile u16 count_arg,
    volatile u32 clubs_arg, volatile u32 league_arg, volatile u32 date_arg,
    volatile u8 first_arg, volatile u8 second_arg, volatile u32 first_four_arg,
    volatile u32 second_four_arg, volatile u16 year_arg, volatile u8 mode_arg,
    volatile u16 last_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x20c];
    u32 l = (u32)scratch, count, league, first, second, source, i;
    u32 year, mode, last, date, clubs, pointer, target, vtable, value;
    u32 name, extension, line;
    frame.state = 0xffffffffu; frame.handler = 0x0095dafa;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    D(l + 8) = self;
    cm_fn_00682200(self);
    frame.state = 0;
    cm_fn_0054a110(self + 0xee);
    count = count_arg;
    league = league_arg;
    first = first_arg;
    source = first_four_arg;
    W(self + 0x3e) = (u16)count;
    second = second_arg;
    B(self + 0xc3) = (u8)second;
    *(volatile u8 *)&frame.state = 1;
    D(self) = 0x0096d94c;
    D(self + 4) = league;
    B(self + 0xc2) = (u8)first;
    B(self + 0xc4) = 1;
    for (i = 0; i < 4; ++i) B(self + i + 0xc5) = B(source + i);
    source = second_four_arg;
    for (i = 0; i < 4; ++i) B(self + i + 0xbe) = B(source + i);
    year = year_arg;
    mode = mode_arg;
    last = last_arg;
    W(self + 0x40) = (u16)year;
    B(self + 0x42) = (u8)mode;
    D(self + 0x30) = 0xffffffffu;
    D(self + 0x20) = 0xffffffffu;
    D(self + 0x1c) = 0xffffffffu;
    date = date_arg;
    B(self + 0x44) = 0xff;
    D(self + 0x2c) = 0;
    B(self + 0x43) = 2;
    W(self + 0x3a) = 0;
    W(self + 0xab) = (u16)last;
    cm_fn_00417c10(self + 0xee, l, date);
    clubs = clubs_arg;
    W(self + 0xd9) = 0;
    pointer = D(clubs);
    pointer = D(pointer + 0x69);
    if (pointer != 0) {
        value = D(pointer);
        D(self + 0x24) = value;
    } else D(self + 0x24) = 0xffffffffu;
    vtable = D(self);
    B(self + 0x49) = 9;
    B(self + 0x4a) = 9;
    W(self + 0x3c) = 1;
    W(self + 0xdb) = 0;
    B(self + 0xbe) = 0; B(self + 0xbf) = 0;
    B(self + 0xc0) = 0; B(self + 0xc1) = 0;
    target = D(vtable + 0x3c);
    value = ((u32 (CM_THISCALL *)(u32, u32, u32, u32, u32))target)
        (self, 0xffffffffu, self + 0xa9, self + 0x3a, 0);
    D(self + 0xba) = value;
    value = cm_fn_00766bf0(self, clubs);
    if (value == 0) {
        name = l + 0x10c; extension = l + 0xc; line = 0x56;
        goto diagnostic;
    }
    value = cm_fn_006835c0(self);
    if (value == 0) {
        name = l + 0xc; extension = l + 0x10c; line = 0x5c;
        goto diagnostic;
    }
    value = cm_fn_006827d0(self, 0);
    if (value == 0) {
        name = l + 0xc; extension = l + 0x10c; line = 0x63;
        goto diagnostic;
    }
    pointer = (u32)cm_fn_00944cf1(0x5ce);
    D(l) = pointer;
    *(volatile u8 *)&frame.state = 2;
    if (pointer != 0) value = cm_fn_0049ee70(pointer, self);
    else value = 0;
    *(volatile u8 *)&frame.state = 1;
    D(self + 8) = value;
    if (value == 0) {
        name = l + 0xc; extension = l + 0x10c; line = 0x6a;
        goto diagnostic;
    }
    cm_fn_0068a850(self);
    goto done;
diagnostic:
    cm_fn_00944cff(0x00a5ab3c, 0, 0, name, extension);
    value = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, value, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return self;
}
