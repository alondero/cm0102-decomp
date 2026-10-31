#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_THISCALL cm_fn_0068d020(u32);
extern void CM_THISCALL cm_fn_0068d040(u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(u32, u32, u32);
extern void CM_THISCALL cm_fn_005acee0(u32, u32, u32, u32, u32, u32, u32,
    u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32);

u32 CM_THISCALL cm_fn_0088c9f0(u32 self, u32 first, u32 second, u32 start,
                              u32 cutoff, u32 filter, u32 selector, u32 list)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x28];
    u32 local = (u32)scratch;
    u32 year, day, extra, a, b, context, value, node, year_arg, day_arg;
    u32 cutoff_year, local_year, local_day, cutoff_day, count, index, table, home;
    u32 target;
    u8 found_first;
    volatile u8 found_second;
    frame.state = 0xffffffffu;
    frame.handler = 0x00963038;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    D(local + 0xc) = self;
    cm_fn_0068d020(local + 0x18);
    frame.state = 0;
    B(local + 3) = 0;
    if (list == 0) {
        /* 0068d020 returns with full ECX=0; MOV CX preserves those zero
           upper bits. MOV AX retains the start pointer's upper half. */
        year = W(start + 2);
        day = (start & 0xffff0000u) | W(start);
        D(local + 4) = day;
        D(local + 8) = year;
        if (first == 0xffffffffu) {
            a = 0xffffffffu;
            b = 0xffffffffu;
            extra = 0;
        } else {
            a = first;
            b = second;
            extra = 0xffffffffu;
        }
        context = D(0x00ae2a58);
        cm_fn_005acee0(context, local + 8, local + 4, local + 3,
            1, filter, a, b, 0xffffffffu, 0xffffffffu, extra, extra, extra,
            selector, local + 0x18, 1, 0xfffffffeu, 0xffffffffu, 0);
        list = local + 0x18;
    }
    value = D(list + 2);
    D(list + 0xa) = value;
    node = cm_fn_0068d1b0(list);
    while (node != 0) {
        if (cutoff != 0) {
            day_arg = (node & 0xffff0000u) | W(node + 0x28);
            /* The original iterator leaves ECX equal to its list receiver. */
            year_arg = (list & 0xffff0000u) | W(node + 0x2a);
            cm_fn_00549ef0(local + 0x10, year_arg, day_arg);
            cutoff_year = W(cutoff + 2);
            local_year = W(local + 0x12);
            if (local_year > cutoff_year)
                goto next;
            if (local_year == cutoff_year) {
                local_day = W(local + 0x10);
                cutoff_day = W(cutoff);
                if ((s16)local_day > (s16)cutoff_day)
                    goto next;
            }
        }
        if (B(node + 0x43) != 0xff) {
            count = (u32)(s32)(s16)W(self + 0x3e);
            found_first = 0;
            index = 0;
            found_second = 0;
            if ((s32)count > 0) {
                table = D(self + 0xb1);
                do {
                    value = D(table);
                    home = D(node + 0x1c);
                    if (value == home)
                        found_first = 1;
                    if (value == D(node + 0x20))
                        found_second = 1;
                    if (found_first != 0 && found_second != 0)
                        break;
                    ++index;
                    table += 59;
                } while ((s32)index < (s32)count);
                if (found_first != 0 && found_second != 0) {
                    self = D(local + 0xc);
                    table = D(self);
                    target = D(table + 4);
                    ((void (CM_THISCALL *)(u32, u32, u32))target)(self, node, 0);
                } else {
                    self = D(local + 0xc);
                }
            }
        }
next:
        node = cm_fn_0068d1b0(list);
    }
    frame.state = 0xffffffffu;
    cm_fn_0068d040(local + 0x18);
    CM_FS(u32, 0) = frame.prev;
    return 1;
}
