#include "cmre.h"

/* Preferred-image VA005AB510/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4/full EAX. L is the original 2F4-byte scratch base.
   Dates L4/LC/L1C/L5C/L24, table L48; SEH is a separate linked frame.
   Original 549EF0 reads only the low WORD of both stack arguments. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern void CM_FASTCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_00549ef0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern u32 CM_CDECL cm_fn_004b0850(u32);
extern void CM_FASTCALL cm_fn_0068d0d0(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054c770(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32, u32, u32);
extern void CM_THISCALL cm_fn_0054a2d0(u32, u32, u32);
extern u32 CM_STDCALL cm_fn_005af2f0(u32, u32);
extern u32 CM_THISCALL cm_fn_005ad780(u32, u32, u32, u32);
extern void CM_FASTCALL cm_fn_0068d040(u32);
extern void CM_CDECL cm_fn_00944c94(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_005ab510(u32 self, u32 fixture)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x2f4];
    u32 L = (u32)scratch, p, value, eax, index, target;
    u32 filename, extension, version, output, first, second, row, extra;
    u16 year, other_year, weekday, day;
    u8 column, counter;

    frame.state = 0xffffffffu;
    frame.handler = 0x00959707;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0054a110(L + 0x1c);
    frame.state = 0;
    cm_fn_0054a110(L + 0xc);
    frame.state = (frame.state & 0xffffff00u) | 1u;
    cm_fn_0054a110(L + 4);
    frame.state = (frame.state & 0xffffff00u) | 2u;
    cm_fn_0054a110(L + 0x5c);
    frame.state = (frame.state & 0xffffff00u) | 3u;
    cm_fn_0054a110(L + 0x24);
    frame.state = (frame.state & 0xffffff00u) | 4u;
    W(L + 0x48) = 2;
    W(L + 0x4a) = 1;
    W(L + 0x4c) = 0xffff;
    W(L + 0x4e) = 5;
    W(L + 0x50) = 6;
    W(L + 0x52) = 0xffff;
    W(L + 0x54) = 0;
    W(L + 0x56) = 3;
    W(L + 0x58) = 4;
    if (fixture == 0) {
        filename = L + 0x1f4;
        extension = L + 0xf4;
        cm_fn_00944cff(0x9cc208, 0, 0, (void *)filename, (void *)extension);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, version, filename, extension, 0x4d7);
        eax = cm_fn_005e8290(0x9870e0, 0xae24d0, 0);
        D(0xb67a34) = 0;
        goto done;
    }
    first = W(fixture + 0x28);
    second = W(fixture + 0x2a);
    p = cm_fn_00549ef0(L + 0x14, second, first);
    cm_fn_00417c10(L + 4, L + 0x2c, p);
    value = cm_fn_004b0850((u32)(s32)(s16)W(fixture + 0x30));
    if ((u8)value == 3) goto queue_record;
    value = cm_fn_004b0850((u32)(s32)(s16)W(fixture + 0x30));
    if ((u8)value == 4) {
queue_record:
        counter = B(fixture + 0x42);
        weekday = W(fixture + 0x34);
        first = D(fixture);
        B(L + 0x3c) = counter;
        second = D(fixture + 0x10);
        W(L + 0x3d) = weekday;
        D(L + 0x38) = first;
        first = D(fixture + 0xc);
        D(L + 0x43) = second;
        D(L + 0x3f) = first;
        cm_fn_0068d0d0(0xae2a48, L + 0x38, L + 0x38, 15);
    }
    index = (u32)(s32)(s8)B(fixture + 0x3a) - 2u;
    if (index <= 8u) {
        target = D(0x005abae4u + index * 4u);
        (void)target;
        switch (index) {
        case 0:
            first = W(fixture + 0x24);
            if (first != 0) {
                second = W(fixture + 0x26);
                p = cm_fn_00549ef0(L + 0x14, second, first);
                output = L + 0xdc;
            } else {
                filename = L + 0xf4;
                extension = L + 0x1f4;
                cm_fn_00944cff(0x9cc208, 0, 0, (void *)filename, (void *)extension);
                version = cm_fn_0090d130();
                cm_fn_00944c9f(0xae24d0, 0x9870e8, version, filename, extension, 0x4f9);
                cm_fn_005e8290(0x9870e0, 0xae24d0, 0);
                D(0xb67a34) = 0;
                p = cm_fn_0054c770(L + 4, L + 0x9c, 2);
                output = L + 0xac;
            }
            cm_fn_00417c10(L + 0xc, output, p);
            first = D(L + 0xc);
            second = D(L + 0x10);
            D(L + 0x2c) = first;
            D(L + 0x30) = second;
            p = cm_fn_0054c770(L + 4, L + 0xbc, 20);
            year = W(p + 2);
            other_year = W(L + 0x2e);
            if (other_year > year ||
                (other_year == year && (s16)first > (s16)W(p))) {
                p = cm_fn_0054c770(L + 4, L + 0xcc, 4);
                cm_fn_00417c10(L + 0x1c, L + 0x7c, p);
            } else {
                p = cm_fn_0054c5b0(L + 4, L + 0xc4, 3);
                cm_fn_00417c10(L + 0x1c, L + 0x94, p);
            }
            break;
        case 3:
            p = cm_fn_0054c770(L + 4, L + 0xe4, 1);
            cm_fn_00417c10(L + 0xc, L + 0xa4, p);
            cm_fn_00417c10(L + 0x1c, L + 0xd4, L + 4);
            break;
        case 2:
        case 4:
            p = cm_fn_0054c770(L + 4, L + 0xb4, 2);
            cm_fn_00417c10(L + 0xc, L + 0xec, p);
            p = cm_fn_0054c5b0(L + 4, L + 0x84, 3);
            cm_fn_00417c10(L + 0x1c, L + 0x8c, p);
            break;
        case 5:
        case 6:
        case 8:
            p = cm_fn_0054c770(L + 4, L + 0x6c, 1);
            cm_fn_00417c10(L + 0xc, L + 0x74, p);
            p = cm_fn_0054c8e0(L + 4, L + 0x64, 3);
            cm_fn_00417c10(L + 0x1c, L + 0x38, p);
            break;
        default:
            break;
        }
    }
    B(L + 1) = 0;
outer_pass:
    B(L + 2) = 0;
table_row:
    value = (u32)(s32)(s8)B(L + 2);
    column = 0;
    D(L + 0x2c) = value * 3u;
    B(L + 3) = column;
table_column:
    row = D(L + 0x2c);
    value = (u32)(s32)(s8)column + row;
    weekday = W(L + 0x48 + value * 2u);
    if (weekday == 0xffff) goto column_next;
    cm_fn_00417c10(L + 0x24, L + 0x38, L + 0x1c);
compare_dates:
    year = W(L + 0xe);
    other_year = W(L + 0x26);
    if (year < other_year ||
        (year == other_year && (s16)W(L + 0xc) <= (s16)W(L + 0x24)))
        goto column_reload;
    cm_fn_0054a2d0(L + 0x24, (u32)(s32)(s16)weekday, L + 0x5c);
    p = cm_fn_0054c5b0(0xae2c90, L + 0x64, 2);
    first = D(p);
    second = D(p + 4);
    year = W(L + 0x5e);
    D(L + 0x14) = first;
    other_year = W(L + 0x16);
    D(L + 0x18) = second;
    if (other_year > year) goto retry_date;
    value = D(L + 0x5c);
    if (other_year == year && (s16)first > (s16)value) goto retry_date;
    other_year = W(L + 0xe);
    if (other_year < year) goto retry_date;
    if (other_year == year && (s16)W(L + 0xc) < (s16)value) goto retry_date;
    W(fixture + 0x28) = year;
    p = D(fixture + 0x14);
    W(fixture + 0x2a) = (u16)value;
    value = cm_fn_005af2f0(p, (u32)(s32)(s16)weekday);
    counter = B(L + 1);
    extra = counter == 1;
    B(fixture + 0x3f) = (u8)value;
    p = cm_fn_005ad780(self, fixture, L + 0x34, extra);
    if (p == 0) {
        eax = D(L + 0x34);
        if (eax == 0) goto done;
    } else {
        cm_fn_0068d040(p);
        cm_fn_00944c94(p);
        eax = D(L + 0x34);
    }
    if (eax == 1) goto done;
    value = (u32)(s32)(s8)B(fixture + 0x3a);
    if ((s32)value > (s32)eax) goto done;
retry_date:
    p = cm_fn_0054c5b0(L + 0x24, L + 0x74, 4);
    cm_fn_00417c10(L + 0x24, L + 0x6c, p);
    goto compare_dates;
column_reload:
    column = B(L + 3);
column_next:
    ++column;
    B(L + 3) = column;
    if ((s8)column < 3) goto table_column;
    counter = B(L + 2);
    ++counter;
    B(L + 2) = counter;
    if ((s8)counter < 3) goto table_row;
    counter = B(L + 1);
    ++counter;
    B(L + 1) = counter;
    if ((s8)counter < 2) goto outer_pass;
    first = D(0xae2c90);
    second = D(0xae2c94);
    year = W(L + 6);
    D(L + 0x14) = first;
    D(L + 0x18) = second;
    other_year = W(L + 0x16);
    if (other_year > year ||
        (other_year == year && (s16)first > (s16)W(L + 4)))
        cm_fn_00417c10(L + 4, L + 0x38, 0xae2c90);
    p = cm_fn_0054c5b0(L + 4, L + 0x38, 2);
    eax = cm_fn_00417c10(L + 4, L + 0x64, p);
    day = W(L + 4);
    year = W(L + 6);
    W(fixture + 0x2a) = day;
    W(fixture + 0x28) = year;
    eax = (eax & 0xffff0000u) | day;
done:
    CM_FS(u32, 0) = frame.prev;
    return eax;
}
