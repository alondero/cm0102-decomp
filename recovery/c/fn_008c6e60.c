#include "cmre.h"

/* Preferred-image VA 0x008c6e60, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The sole direct caller overwrites EAX at008bbe05. The original inner loop
   advances its counter by6 and its record pointer by0x102; retain both. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_FASTCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_008c7560(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007c3e40(u32, u32, u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054c770(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008c7070(u32, u32, u32, u32, u32, u32);
extern void CM_CDECL cm_fn_00945333(u32, u32, u32, u32);

void CM_THISCALL cm_fn_008c6e60(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x3c];
    u32 local = (u32)scratch, total, person, index = 0, table, club, receiver;
    u32 year_bits, entries, length, row, scan, id, base, country, other;
    u32 candidate, value, low, high, input, output, selected_self;
    u16 target_year, now_year;
    u8 category;

    frame.state = 0xffffffffu;
    frame.handler = 0x00963d68;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    D(local + 4) = self;
    cm_fn_0054a110(local + 0x14);
    total = D(0x00ae2364);
    person = D(0x00ae23bc);
    frame.state = 0;
    D(local + 8) = 0;
    if ((s32)total <= 0) goto sort;
    do {
        category = B(person + 0x22);
        if (category > 13) {
            table = D(self + 0x86e);
            W(table + index * 8u + 3) = 0x3fff;
        } else if (category > 5) {
            table = D(self + 0x86e);
            W(table + index * 8u + 3) = 0x1fff;
        }
        club = D(person + 0x39);
        if (club == 0 || B(person + 0x3d) != 11) goto next;
        if (cm_fn_008c7560(self, person, club, 0) == 0) goto next;
        year_bits = (self & 0xffff0000u) | W(person + 0x40);
        receiver = D(0x00b74350);
        D(local) = year_bits;
        entries = cm_fn_007c3e40(receiver, person, local + 0xc);
        length = D(local + 0xc);
        scan = 0;
        D(local + 0x10) = entries;
        if ((s32)length > 0) {
            row = entries + 2;
            do {
                id = D(row);
                if ((s32)id >= 0 && (s32)id < (s32)D(0x00ae235c)) {
                    base = D(0x00ae23b4);
                    club = D(person + 0x39);
                    country = D(base + id * 581u + 0x53);
                    other = D(club + 0x53);
                    if (country != other) {
                        year_bits = D(local);
                        break;
                    }
                    candidate = (club & 0xffff0000u) | W(row - 2);
                    year_bits = D(local);
                    if ((u16)candidate < (u16)year_bits) {
                        year_bits = candidate;
                        D(local) = year_bits;
                    }
                }
                scan += 6;
                row += 0x102;
            } while ((s32)scan < (s32)length);
        }
        entries = D(local + 0x10);
        if (entries != 0) cm_fn_009452ca(entries);
        value = W(0x00ae2c92);
        if ((s32)(value - (u32)(u16)year_bits) >= 5) {
            club = D(person + 0x39);
            country = D(club + 0x53);
            D(person + 0x1e) = country;
        } else {
            input = person + 0x3e;
            output = local + 0x24;
            for (;;) {
                cm_fn_00417c10(local + 0x14, output, input);
                low = D(0x00dd6ec8);
                high = D(0x00dd6ecc);
                target_year = W(local + 0x16);
                D(local + 0x1c) = low;
                D(local + 0x20) = high;
                now_year = W(local + 0x1e);
                if (now_year < target_year ||
                    (now_year == target_year && (s16)low <= (s16)W(local + 0x14))) break;
                input = cm_fn_0054c770(local + 0x14, local + 0x2c, 0x34);
                output = local + 0x34;
            }
            club = D(person + 0x39);
            selected_self = D(local + 4);
            country = D(club + 0x53);
            cm_fn_008c7070(selected_self, person, country, year_bits, 1, 0);
        }
        self = D(local + 4);
        index = D(local + 8);
next:
        total = D(0x00ae2364);
        ++index;
        person += 110;
        D(local + 8) = index;
    } while ((s32)index < (s32)total);
sort:
    length = D(self + 0x93e);
    entries = D(self + 0x93a);
    cm_fn_00945333(entries, length, 14, 0x008e55c0);
    value = frame.prev;
    CM_FS(u32, 0) = value;
}
