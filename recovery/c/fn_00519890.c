#include "cmre.h"

/* Preferred-image VA 0x00519890, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Retain upper halves when MOV CX/DI/DX/AX supplies a pushed DWORD. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_0086a800(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0086b0b0(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0086b710(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0086bdd0(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0086c000(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_005c6710(u32, u32);
extern u32 CM_CDECL cm_fn_00669470(u32);

u32 CM_THISCALL cm_fn_00519890(u32 self, volatile u32 first, volatile u32 mode,
                              u32 unused1, u32 unused2, volatile u32 fifth,
                              volatile u32 sixth)
{
    u32 record = fifth, club, value, other, manager, person, base, id;
    u32 word_arg, amount, percent, table, round_bits, count_bits;
    s32 round, count, numerator, quotient, remainder;
    u8 kind;
    (void)unused1; (void)unused2;
    if (B(record + 0x3a) == 1) return 0;
    if (D(self + 0x2c) != 0) return 0;
    if (B(self + 0x44) != 0xff) return 0;
    kind = *(volatile u8 *)&mode;
    if (kind == 1) {
        value = sixth;
        club = first;
        manager = D(0x00dd7f00);
        cm_fn_0086a800(manager, club, record, value);
        person = D(self + 4);
        other = D(0x009cf750);
        id = D(person);
        if (id != other) {
            value = D(person + 0x5d);
            if (value == 0) return 0;
            value = D(value + 0x71);
            if (value == 0) return 0;
            id = D(value);
            other = D(0x009cfa10);
            if (id != other) return 0;
            manager = D(0x00ae2c74);
            if (cm_fn_005c6710(manager, person) != 0) return 0;
            id = D(club);
            base = D(0x00ae2d14);
            if (D(base + id * 6u) != 0xffffffffu) return 0;
            person = D(self + 4);
            value = cm_fn_00669470(person);
            if ((u8)value == 0xff) return 0;
            person = D(self + 4);
            value = cm_fn_00669470(person);
            id = (u32)(s32)(s8)value;
            person = D(self + 4);
            other = D(0x00b63d7e + id * 72u);
            if (other != person) return 0;
        }
        id = D(club);
        base = D(0x00ae2d14);
        other = D(0x009cf6f0);
        D(base + id * 6u) = other;
        return 0;
    }
    if (kind == 2) {
        value = sixth;
        club = first;
        manager = D(0x00dd7f00);
        cm_fn_0086b0b0(manager, club, record, value);
        return 0;
    }
    if (kind == 3) {
        value = sixth;
        club = first;
        manager = D(0x00dd7f00);
        cm_fn_0086b710(manager, club, record, value);
        return 0;
    }
    if (kind == 0) {
        round_bits = W(record + 0x34);
        count_bits = W(self + 0x3c);
        other = (u32)(s32)(s16)(round_bits + 1u);
        if ((s32)other >= (s32)(s16)count_bits) return 0;
        table = D(self + 0xa3);
        amount = (table & 0xffff0000u) | W(table + other * 104u + 7);
        if ((s16)amount == 0x8c) return 0;
        round = (s16)round_bits;
        word_arg = (record & 0xffff0000u) | W(record + 0x32);
        count = (s16)count_bits;
        numerator = round * 99;
        quotient = numerator / (count - 1);
        person = D(self + 4);
        percent = (u32)quotient;
        percent = (percent & 0xffffff00u) | (u8)(percent + 1u);
        club = first;
        manager = D(0x00dd7f00);
        cm_fn_0086bdd0(manager, club, person, word_arg, amount, percent);
        return 0;
    }
    if (kind == 0xff) {
        round = CM_GLOBAL(volatile s16, record + 0x34);
        count = CM_GLOBAL(volatile s16, self + 0x3c);
        numerator = round * 99;
        quotient = numerator / count;
        remainder = numerator % count;
        amount = ((u32)remainder & 0xffff0000u) | W(record + 0x30);
        percent = (u32)quotient;
        percent = (percent & 0xffffff00u) | (u8)(percent + 1u);
        word_arg = (percent & 0xffff0000u) | W(record + 0x32);
        person = D(self + 4);
        club = first;
        manager = D(0x00dd7f00);
        cm_fn_0086c000(manager, club, person, word_arg, amount, percent);
    }
    return 0;
}
