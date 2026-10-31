#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32, u32, u32);
extern void CM_THISCALL cm_fn_005abb10(u32, u32, u32);
extern void CM_THISCALL cm_fn_007775c0(u32, u32);

u32 CM_THISCALL cm_fn_007777e0(u32 self, u32 date)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x30];
    u32 local = (u32)scratch;
    u32 nations, limit, value, index, row, records, record, row_offset;
    u32 club_offset, club, context;
    u16 first_year, record_year, last_year, day;
    u8 record_index, count, status;
    int more;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095df98;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    nations = D(0x00ae2350);
    limit = D(0x00ae235c) - 2u * nations;
    D(local + 0x14) = limit;
    cm_fn_0054c8e0(date, local + 0x20, 1);
    frame.state = 0;
    cm_fn_0054c5b0(date, local + 0x28, 1);
    value = D(self + 0x24);
    *(volatile u8 *)&frame.state = 1;
    if (value == 0) {
        cm_fn_007775c0(self, date);
        cm_fn_0054c8e0(date, local + 0x18, 1);
        *(volatile u8 *)&frame.state = 2;
        cm_fn_007775c0(self, local + 0x18);
        value = cm_fn_0054c5b0(local + 0x18, local + 0xc, 2);
        /* LEA [ESP+14] at 0077787b precedes PUSH EAX: L+4. */
        cm_fn_00417c10(local + 0x18, local + 4, value);
        cm_fn_007775c0(self, local + 0x18);
        *(volatile u8 *)&frame.state = 1;
    }
    index = 0;
    D(local + 0x18) = 0;
    if ((s32)limit > 0) {
        D(local + 0xc) = 0;
        D(local + 4) = 0;
        do {
            value = D(self + 0x24);
            status = B(value + index);
            if (status == 1 || status == 2) {
                row = D(self + 0x20);
                row_offset = D(local + 4);
                row += row_offset;
                record_index = 0;
                B(local + 3) = 0;
                count = B(row + 5);
                if ((s8)count > 0) {
                    records = D(row);
                    do {
                        first_year = W(local + 0x22);
                        record = records + (u32)(s32)(s8)record_index * 9u;
                        record_year = W(record + 2);
                        if (first_year > record_year)
                            goto next_record;
                        if (first_year == record_year) {
                            day = W(local + 0x20);
                            if ((s16)day > (s16)W(record))
                                goto next_record;
                        }
                        record_year = W(record + 2);
                        last_year = W(local + 0x2a);
                        if (last_year < record_year)
                            break;
                        if (last_year == record_year) {
                            day = W(local + 0x28);
                            if ((s16)day < (s16)W(record))
                                break;
                        }
                        value = B(record + 8);
                        status = B(local + 3);
                        B(local + 3) = (u8)(status + value);
next_record:
                        ++record_index;
                    } while ((s8)record_index < (s8)count);
                    if ((s8)B(local + 3) > 2) {
                        club = D(0x00ae23b4);
                        club_offset = D(local + 0xc);
                        club += club_offset;
                        context = D(0x00ae2a58);
                        cm_fn_005abb10(context, club, date);
                    }
                    /* All BP date comparisons precede this full EBP reload. */
                    limit = D(local + 0x14);
                }
            }
            index = D(local + 0x18);
            row_offset = D(local + 4);
            club_offset = D(local + 0xc);
            ++index;
            row_offset += 6;
            club_offset += 581;
            more = (s32)index < (s32)limit;
            D(local + 0x18) = index;
            D(local + 4) = row_offset;
            D(local + 0xc) = club_offset;
        } while (more);
    }
    CM_FS(u32, 0) = frame.prev;
    return index;
}
