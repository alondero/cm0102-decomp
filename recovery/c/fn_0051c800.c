#include "cmre.h"
/* Preferred-image VA0051c800/base00400000, pinned 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4/full EAX (1 or -1), SEH9576d9. Scratch21c:
   L0 count/allocation/primary date, L8 group offset, Lc owner/secondary
   date, L14 counter/date output, diagnosticsL1c,L11c.
   Original549ef0 consumes only lowWORDs;54c5b0(+1) writes8 output bytes. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_0051c410(u32,u32);
extern u32 CM_THISCALL cm_fn_0051cb90(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0051ebe0(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944c94(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_0051c800(u32 self, volatile u32 round_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x21c];
    u32 l = (u32)scratch, pointer, owner, flag, round, group, records;
    u32 offset, count, match, index, table, object, allocation;
    u32 first, second, version, result = 1, byte_offset;
    u16 word, year, other_year;
    u8 byte, latest;
    frame.state = 0xffffffffu;
    frame.handler = 0x009576d9;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    pointer = D(self + 4);
    owner = D(pointer);
    flag = D(self + 0x45);
    D(l + 0xc) = owner;
    if (!flag) goto done;
    count = (u32)(s32)(s16)W(self + 0x3c);
    round = round_arg;
    if ((s32)round > (s32)count - 1 || (s32)round < 0) goto diagnostic;
    table = D(self + 0xad);
    pointer = D(table + round * 4u);
    byte = B(pointer + 0x34);
    if (!byte) cm_fn_0051c410(self, round);
    group = (u32)(s32)(s8)B(self + 0xab);
    D(l + 0x14) = 0;
    records = D(self + 0xa3);
    offset = group * 104u;
    D(l + 8) = offset;
    count = (u32)(s32)(s16)W(records + offset + 0x1a);
    D(l) = count;
    if ((s32)count > 0) {
        table = D(self + 0xad);
        match = D(table + group * 4u);
        do {
            word = W(match);
            if (word != 0xffffu) {
                index = (u32)(s32)(s16)word;
                table = D(self + 0xa7);
                if (D(table + index * 6u) == 0) goto reschedule;
            }
            word = W(match + 2);
            if (word != 0xffffu) {
                index = (u32)(s32)(s16)word;
                table = D(self + 0xa7);
                if (D(table + index * 6u) == 0) goto reschedule;
            }
            index = D(l + 0x14);
            count = D(l);
            index++;
            match += 0x35;
            D(l + 0x14) = index;
        } while ((s32)index < (s32)count);
    }
    index = 0;
    if ((s32)count > 0) {
        byte_offset = 0;
        do {
            table = D(self + 0xad);
            pointer = D(table + group * 4u);
            if (B(pointer + byte_offset + 0x34) == 1)
                cm_fn_0051cb90(self, group, index, 1);
            table = D(self + 0xad);
            pointer = D(table + group * 4u);
            if (B(pointer + byte_offset + 0x34) == 2) {
                cm_fn_0051cb90(self, group, index, 0);
                table = D(0x00adadfc);
                owner = D(l + 0xc);
                if (D(table + owner * 4u) == 0) goto done;
            }
            records = D(self + 0xa3);
            offset = D(l + 8);
            index++;
            byte_offset += 0x35;
            count = (u32)(s32)(s16)W(records + offset + 0x1a);
        } while ((s32)index < (s32)count);
    }
    allocation = cm_fn_00944cf1(8);
    D(l) = allocation;
    frame.state = 1;
    if (allocation) {
        records = D(self + 0xa3);
        offset = D(l + 8);
        pointer = records + offset;
        year = (u16)((s16)(s8)B(pointer + 5) + W(self + 0x40));
        word = W(pointer + 3);
        object = (u32)cm_fn_00549ef0(allocation, word, year);
    } else object = 0;
    frame.state = 0xffffffffu;
    cm_fn_0051ebe0(self, object, group);
    if (object) cm_fn_00944c94(object);
    table = D(0x00adadfc);
    owner = D(l + 0xc);
    if (D(table + owner * 4u) == 0) goto done;
    records = D(self + 0xa3);
    offset = D(l + 8);
    if (W(records + offset + 7) != 0x8c) {
        byte = B(self + 0xab);
        B(self + 0xab) = (u8)(byte + 1u);
    }
    byte = B(self + 0xab);
    latest = B(self + 0xac);
    if ((s8)latest < (s8)byte) B(self + 0xac) = byte;
    goto done;
reschedule:
    offset = round * 104u;
    year = (u16)((s16)(s8)B(records + offset + 2) + W(self + 0x40));
    word = W(records + offset);
    cm_fn_00549ef0(l, word, year);
    frame.state = 0;
    pointer = cm_fn_0054c5b0(l, l + 0xc, 1);
    cm_fn_00417c10(l, l + 0x14, pointer);
    records = D(self + 0xa3);
    word = W(l);
    W(records + offset) = word;
    byte = B(l + 2);
    latest = B(self + 0x40);
    records = D(self + 0xa3);
    round++;
    B(records + offset + 2) = (u8)(byte - latest);
    if ((s16)round >= (s16)W(self + 0x3c)) goto done;
    do {
        first = (u32)(s32)(s16)round;
        records = D(self + 0xa3);
        offset = first * 104u;
        year = (u16)((s16)(s8)B(records + offset + 2) + W(self + 0x40));
        word = W(records + offset);
        cm_fn_00549ef0(l + 0xc, word, year);
        year = W(l + 2);
        other_year = W(l + 0xe);
        second = D(l);
        if (year > other_year ||
            (year == other_year && (s16)second > (s16)W(l + 0xc))) {
            records = D(self + 0xa3);
            W(records + offset) = (u16)second;
            byte = B(l + 2);
            latest = B(self + 0x40);
            records = D(self + 0xa3);
            B(records + offset + 2) = (u8)(byte - latest);
        }
        round++;
    } while ((s16)round < (s16)W(self + 0x3c));
    goto done;
diagnostic:
    cm_fn_00944cff(0x009b55ac, 0, 0, l + 0x11c, l + 0x1c);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, l + 0x11c, l + 0x1c, 0xa64);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    result = 0xffffffffu;
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return result;
}
