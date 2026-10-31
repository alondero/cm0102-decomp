#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
typedef u16 word_4c7560 __attribute__((may_alias));
#define W(a) CM_GLOBAL(volatile word_4c7560, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_FASTCALL cm_fn_0085e410(u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004d0c60(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00549ef0(u32, u32, u32);
extern u32 CM_FASTCALL cm_fn_0054a110(u32);
extern u32 CM_CDECL cm_fn_00669bd0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32, u32, u32);
extern u32 CM_FASTCALL cm_fn_0054cfb0(u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_THISCALL cm_fn_00549f70(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054c770(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004c79a0(u32, u32);
extern u32 CM_THISCALL cm_fn_0085e960(u32, u32, u32);

/* Preferred-image VA004c7560/base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver/RET0, full incidental EAX. Single scratch2d4, linked SEH.
   549f70 uses only WORD day/year and BYTE month. The root's partial
   year/weekday slots are therefore read at those proven widths; the
   unwritten upper bytes are neither initialized nor consumed in C. */
u32 CM_FASTCALL cm_fn_004c7560(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 scratch[0x2d4 / 4];
    u32 l = (u32)scratch, previous, eax, count, person, offset, base, club;
    u32 pointer, first, second, date, index, value, year, month, day, version;
    u32 next_count, next_offset, bound, slot, contract;
    u16 compare_year, field_year;
    u8 flag;
    s32 level;

    frame.state = 0xffffffffu;
    frame.handler = 0x00956c71;
    previous = CM_FS(u32, 0);
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0085e410(l + 0x84);
    count = D(0x00ae2364);
    frame.state = 0;
    D(self + 4) = count;
    eax = cm_fn_00944e46(count * 80u);
    D(self) = eax;
    if (eax == 0) {
        cm_fn_00944cff(0x009a8190, 0, 0, l + 0x1d4, l + 0xd4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009a8120, version,
                       l + 0x1d4, l + 0xd4, 0xd7, 0x009a8134);
        cm_fn_0066f4e0(0x00ae28bc, 0x009a8108);
        cm_fn_0066f4e0(0x00ae26c4, 0x009a7ff8, 0x00ae24d0);
        eax = cm_fn_005e8290(0x00ae28bc, 0x00ae26c4, 0);
        goto done;
    }
    D(self + 8) = 0;
    eax = D(0x00ae2364);
    person = D(0x00ae23bc);
    D(l) = 0;
    if ((s32)eax <= 0) goto done;
    offset = 0;
    D(l + 4) = offset;
    do {
        base = D(0x00ae2d0c);
        flag = B(base + offset + 0xb);
        B(base + offset + 0xb) = flag & 0x7fu;
        pointer = D(person + 0x39);
        eax = cm_fn_004d0c60(self, person, pointer, 1, 0xffffffffu, 0);
        club = D(person + 0x39);
        D(person + 0x52) = eax;
        if (club != 0) {
            pointer = cm_fn_00549ef0(l + 0x6c, 0x1f, 0x76c);
            first = D(pointer);
            second = D(pointer + 4);
            D(l + 0x14) = first;
            compare_year = W(l + 0x16);
            field_year = W(person + 0x48);
            date = person + 0x46;
            D(l + 0x18) = second;
            if (compare_year == field_year) {
                value = W(date);
                if ((u16)first == value) {
                    value = W(0x00ae2c92);
                    W(l + 0x10) = (u16)value;
                    cm_fn_0054a110(l + 0x24);
                    B((u32)&frame.state) = 1;
                    cm_fn_00669bd0(club, 0, l + 0x24);
                    pointer = cm_fn_0054c5b0(l + 0x24, l + 0x34, 10);
                    B((u32)&frame.state) = 2;
                    value = cm_fn_0054cfb0(pointer);
                    B(l + 0xc) = (u8)value;
                    B((u32)&frame.state) = 0;
                    value = cm_fn_0090cfc0(6);
                    year = W(l + 0x10);
                    year = value + year + 1u;
                    month = B(l + 0xc);
                    value = cm_fn_0090cfc0(0x1c);
                    day = (u32)(s32)(s8)(u8)(value + 1u);
                    pointer = cm_fn_00549f70(l + 0x4c, day, month,
                                             (u16)year, 0xffffffffu);
                    cm_fn_00417c10(date, l + 0x7c, pointer);
                }
            }
            for (;;) {
                pointer = cm_fn_0054c770(0x00dd6ec8, l + 0x5c, 8);
                first = D(pointer);
                second = D(pointer + 4);
                field_year = W(date + 2);
                D(l + 0x1c) = first;
                D(l + 0x20) = second;
                compare_year = W(l + 0x1e);
                if (compare_year < field_year) break;
                if (compare_year == field_year) {
                    value = W(date);
                    if ((s16)first <= (s16)value) break;
                }
                pointer = cm_fn_0054c770(date, l + 0x74, 0x34);
                cm_fn_00417c10(date, l + 0x3c, pointer);
            }
        }
        value = D(person + 0x24);
        if (value != 0) {
            pointer = cm_fn_00549ef0(l + 0x44, 0x1f, 0x76c);
            first = D(pointer);
            second = D(pointer + 4);
            D(l + 0x2c) = first;
            compare_year = W(l + 0x2e);
            field_year = W(person + 0x33);
            date = person + 0x31;
            D(l + 0x30) = second;
            if (compare_year == field_year) {
                value = W(date);
                if ((u16)first == value) {
                    value = W(0x00ae2c92);
                    W(l + 8) = (u16)value;
                    value = cm_fn_0090cfc0(5);
                    year = W(l + 8);
                    year = value + year + 1u;
                    pointer = cm_fn_00549f70(l + 0x54, 10, 5,
                                             (u16)year, 0xffffffffu);
                    cm_fn_00417c10(date, l + 0x64, pointer);
                }
            }
        }
        pointer = D(person + 0x39);
        if (pointer != 0) {
            flag = B(person + 0x3d);
            if (flag == 0 || flag == 6 || flag == 7 || flag == 8 || flag == 9 ||
                flag == 10 || flag == 11 || flag == 12 || flag == 13 || flag == 15) {
                contract = D(person + 0x52);
                slot = cm_fn_004c79a0(self, 1);
                if ((s32)slot >= 0) {
                    index = D(person);
                    offset = index * 79u;
                    base = D(0x00ae2d0c);
                    D(base + offset) = slot;
                    base = D(self);
                    index = D(person);
                    D(base + slot * 80u) = index;
                    base = D(self);
                    cm_fn_0085e960(base + slot * 80u, person, contract);
                }
                pointer = D(person + 0x61);
                if (pointer != 0) {
                    flag = B(pointer + 4);
                    if ((s8)flag < 1) level = 1;
                    else if ((s8)flag > 50) level = 50;
                    else level = (s8)flag;
                    pointer = 0;
                    if (person != 0) {
                        index = D(person);
                        bound = D(0x00ae2364);
                        if ((s32)index < (s32)bound) {
                            base = D(0x00ae2d0c);
                            index = D(base + index * 79u);
                            if ((s32)index >= 0) {
                                base = D(self);
                                pointer = base + index * 80u;
                            }
                        }
                    }
                    B(pointer + 0x3a) = (u8)(0u - (u32)level);
                }
            }
        }
        next_count = D(l);
        next_offset = D(l + 4);
        bound = D(0x00ae2364);
        eax = next_count + 1u;
        offset = next_offset + 79u;
        person += 110u;
        D(l) = eax;
        D(l + 4) = offset;
    } while ((s32)eax < (s32)bound);
done:
    previous = frame.prev;
    CM_FS(u32, 0) = previous;
    return eax;
}
