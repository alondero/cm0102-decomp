#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
typedef u16 word_7757d0 __attribute__((may_alias));
#define W(a) CM_GLOBAL(volatile word_7757d0, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern u32 CM_THISCALL cm_fn_0068d040(u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00549ef0(u32, u16, u16);
extern u32 CM_THISCALL cm_fn_005acbe0(u32, u32, u32, u16, u16,
                                     u32, u32, u32, u8, u8);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);

/* Preferred-image VA007757d0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS5/RET20/fullEAX. Scratch2c: selfL0, datesL4/Lc/L14,
   listL1c (constructor writes14 of16 bytes). No scratch zero-fill.
   549ef0 observes only argument WORDs; 5acbe0 arg9 observes BYTE only.
   The two all-star paths retain cached indices/base unless formatting runs. */
u32 CM_THISCALL cm_fn_007757d0(u32 self, volatile u32 person_argument,
                              volatile u32 date_argument,
                              volatile u32 club_filter_argument,
                              volatile u32 text_argument,
                              volatile u32 match_argument)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 scratch[0x2c / 4];
    u32 l = (u32)scratch, value, other, date, person, owner, entry, match;
    u32 first_day, first_year, last_day, last_year, receiver, club_id;
    u32 result, row_day, row_extra, competition, schema, year, day;
    u32 base, index_a8, index_a4, mapped, offset;
    u16 compare_year;
    u8 status, side, slot;

    frame.state = 0xffffffff;
    frame.handler = 0x0095def0;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    D(l) = self;
    cm_fn_0068d020(l + 0x1c);
    value = club_filter_argument;
    frame.state = 0;
    if (value != 0) {
        if (cm_fn_0053b900(value) != 0) goto fail;
        value = D(value);
        other = D(0x009d00a4);
        if (value == other) goto fail;
        if (value == D(0x009d00a8)) goto fail;
    }
    date = date_argument;
    cm_fn_0054c8e0(date, l + 4, 1);
    frame.state = (frame.state & 0xffffff00u) | 1u;
    cm_fn_0054c5b0(date, l + 0xc, 1);
    person = person_argument;
    frame.state = (frame.state & 0xffffff00u) | 2u;
    owner = D(person + 0x24);
    if (owner != 0) {
        status = B(person + 0x5e);
        club_id = D(owner);
        side = (u8)(((u8)~status & 4u) >> 2);
        last_year = D(l + 0xe);
        last_day = D(l + 0xc);
        first_year = D(l + 6);
        first_day = D(l + 4);
        receiver = D(0x00ae2a58);
        value = cm_fn_005acbe0(receiver, first_day, first_year,
                               (u16)last_day, (u16)last_year, club_id,
                               l + 0x1c, 0xffffffff, 0xfe, (u8)(side == 0));
        match = match_argument;
        if ((u16)value != 0) {
            if (match == 0) goto international;
            value = D(l + 0x1e);
            D(l + 0x26) = value;
            entry = cm_fn_0068d1b0(l + 0x1c);
            while (entry != 0) {
                if (D(entry + 0x14) == match) goto international;
                entry = cm_fn_0068d1b0(l + 0x1c);
            }
        }
        receiver = D(l);
        value = D(receiver);
        if (value != 0 && side == 0) {
            other = D(value + 2);
            D(value + 0xa) = other;
            receiver = D(receiver);
            entry = cm_fn_0068d1b0(receiver);
            while (entry != 0) {
                owner = D(person + 0x24);
                value = D(entry);
                if (value != D(owner)) goto next_entry;
                if (match != 0) {
                    value = D(entry + 4);
                    other = D(match);
                    if (value != other) goto next_entry;
                }
                row_day = D(entry + 8);
                row_extra = D(entry + 0xc);
                compare_year = W(date + 2);
                D(l + 0x14) = row_day;
                D(l + 0x18) = row_extra;
                year = W(l + 0x16);
                if ((u16)year < compare_year) goto international_before;
                if ((u16)year > compare_year) goto next_entry;
                other = W(date);
                if ((s16)(u16)row_day < (s16)(u16)other)
                    goto international_before;
next_entry:
                value = D(l);
                receiver = D(value);
                entry = cm_fn_0068d1b0(receiver);
            }
        }
    }

    base = D(0x00adadfc);
    value = D(0x009cf690);
    competition = D(base + value * 4u);
    if (competition == 0) goto fail;
    schema = D(competition + 0xa3);
    year = (u32)(s32)(s8)B(schema + 5);
    year = (u16)((u16)year + W(competition + 0x40));
    day = W(schema + 3);
    cm_fn_00549ef0(l + 0x14, (u16)day, (u16)year);
    value = D(person + 0x39);
    frame.state = (frame.state & 0xffffff00u) | 3u;
    if (value == 0) goto fail;
    value = D(value + 0x57);
    if (value == 0) goto fail;
    other = D(value);
    value = D(0x009cf590);
    if (other != value) goto fail;
    first_year = D(l + 6);
    compare_year = W(l + 0x16);
    if ((u16)first_year > compare_year) goto fail;
    day = D(l + 0x14);
    if ((u16)first_year == compare_year &&
        (s16)W(l + 4) > (s16)(u16)day) goto fail;
    last_year = D(l + 0xe);
    if ((u16)last_year < compare_year) goto fail;
    if ((u16)last_year == compare_year &&
        (s16)W(l + 0xc) < (s16)(u16)day) goto fail;

    index_a8 = D(0x009d00a8);
    base = D(0x00ae23b4);
    club_filter_argument = base + index_a8 * 581u;
    index_a4 = D(0x009d00a4);
    date_argument = base + index_a4 * 581u;
    slot = 0;
    do {
        mapped = club_filter_argument;
        offset = (u32)(s32)(s8)slot * 4u;
        if (D(mapped + offset + 0xd7) == person) goto found_a8;
        mapped = date_argument;
        if (D(mapped + offset + 0xd7) == person) goto found_a4;
        ++slot;
    } while ((s8)slot < 50);
    goto fail;

international:
    value = text_argument;
    if (value != 0)
        cm_fn_0066f4e0(value, 0x00a62ff4, 0x00ad9c64);
    result = D(person + 0x24);
    goto finish;
international_before:
    value = text_argument;
    if (value != 0)
        cm_fn_0066f4e0(value, 0x00a62fc8, 0x00ad9c64);
    result = D(person + 0x24);
    goto finish;
found_a8:
    value = text_argument;
    if (value != 0) {
        cm_fn_0066f4e0(0x00de1f64, 0x00a62fa4, 0x00ad9c64);
        index_a8 = D(0x009d00a8);
        base = D(0x00ae23b4);
    }
    result = base + index_a8 * 581u;
    goto finish;
found_a4:
    value = text_argument;
    if (value != 0) {
        cm_fn_0066f4e0(0x00de1f64, 0x00a62fa4, 0x00ad9c64);
        index_a4 = D(0x009d00a4);
        base = D(0x00ae23b4);
    }
    result = base + index_a4 * 581u;
    goto finish;
fail:
    result = 0;
finish:
    frame.state = 0xffffffff;
    cm_fn_0068d040(l + 0x1c);
    CM_FS(u32, 0) = frame.prev;
    return result;
}
