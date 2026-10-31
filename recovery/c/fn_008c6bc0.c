#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_FASTCALL cm_fn_0054a110(u32);
extern u32 CM_FASTCALL cm_fn_0054cfb0(u32);
extern u32 CM_THISCALL cm_fn_0054c770(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0040a7d0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern void CM_THISCALL cm_fn_008c4e10(u32, u32);

/* EAX is overwritten by the sole direct continuation at008c5914. */
void CM_THISCALL cm_fn_008c6bc0(u32 self, u32 date)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x3c];
    u8 months[3];
    u32 local = (u32)scratch;
    u32 value, total, cursor, index, record, month, year, sum, converted;
    u32 first, second, third, day, table, object, target, mode, flag, same_year;
    frame.state = 0xffffffffu;
    frame.handler = 0x00963d48;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    W(local + 4) = 0;
    W(local + 6) = 0;
    W(local + 8) = 0;
    cm_fn_0054a110(local + 0xc);
    frame.state = 0;
    value = cm_fn_0054cfb0(date);
    months[0] = (u8)(value - 1);
    if ((s8)months[0] < 0) {
        months[0] = 0xb;
        W(local + 4) = 0xffff;
    }
    months[1] = (u8)cm_fn_0054cfb0(date);
    value = cm_fn_0054cfb0(date);
    months[2] = (u8)(value + 1);
    if ((s8)months[2] > 0xb) {
        months[2] = 0;
        W(local + 8) = 1;
    }
    cm_fn_0054c770(date, local + 0x24, 1);
    *(volatile u8 *)&frame.state = 1;
    cm_fn_0054c8e0(date, local + 0x1c, 1);
    total = D(self + 0x8a8);
    D(local) = 0;
    cursor = D(self + 0x8a4);
    *(volatile u8 *)&frame.state = 2;
    if ((s32)total <= 0)
        goto done;
    do {
        month = 0;
        do {
            record = D(cursor);
            value = B(record + 4);
            if (value == months[month]) {
                year = W(date + 2);
                sum = W(local + 4 + month * 2);
                sum = (u16)(sum + year);
                converted = cm_fn_0040a7d0(record, local + 0x2c, sum);
                cm_fn_00417c10(local + 0xc, local + 0x34, converted);
                first = D(date);
                second = D(date + 4);
                third = D(local + 0xe);
                D(local + 0x14) = first;
                same_year = W(local + 0x16) == (u16)third;
                D(local + 0x18) = second;
                day = D(local + 0xc);
                if (same_year && (u16)first == (u16)day) {
                    record = D(cursor);
                    flag = B(record + 5);
                    index = (u32)(s32)(s8)B(record);
                    object = D(self + index * 4 + 0x8ac);
                    table = D(object);
                    mode = (third & 0xffffff00u) | (flag == 0 ? 1 : 3);
                    target = D(table + 0x2c);
                    ((u32 (CM_THISCALL *)(u32, u32, u32, u32))target)
                        (object, record, mode, third);
                    break;
                }
                if (W(local + 0x1e) == (u16)third && W(local + 0x1c) == (u16)day) {
                    record = D(cursor);
                    if (B(record + 5) == 0) {
                        record = D(cursor);
                        index = (u32)(s32)(s8)B(record);
                        object = D(self + index * 4 + 0x8ac);
                        table = D(object);
                        target = D(table + 0x2c);
                        ((u32 (CM_THISCALL *)(u32, u32, u32, u32))target)
                            (object, record, 0, third);
                        record = D(cursor);
                        index = (u32)(s32)(s8)B(record);
                        object = D(self + index * 4 + 0x8ac);
                        value = D(object + 0xa);
                        if (value != 0)
                            cm_fn_008c4e10(self, value);
                        break;
                    }
                }
                if (W(local + 0x26) == (u16)third && W(local + 0x24) == (u16)day) {
                    record = D(cursor);
                    if (B(record + 5) == 0) {
                        record = D(cursor);
                        index = (u32)(s32)(s8)B(record);
                        object = D(self + index * 4 + 0x8ac);
                        table = D(object);
                        target = D(table + 0x2c);
                        ((u32 (CM_THISCALL *)(u32, u32, u32, u32))target)
                            (object, record, 2, third);
                        break;
                    }
                }
            }
            ++month;
        } while (month < 3);
        value = D(local);
        total = D(self + 0x8a8);
        ++value;
        cursor += 4;
        D(local) = value;
    } while ((s32)value < (s32)total);
done:
    CM_FS(u32, 0) = frame.prev;
}
