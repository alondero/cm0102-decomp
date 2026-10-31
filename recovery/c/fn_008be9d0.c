#include "cmre.h"
/* Preferred-image VA008be9d0/base00400000, pinned 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS3/RET12/full incidental EAX, SEH963a9c; scratch20c:
   allocationL0/date outputL4/diagnosticsLc,L10c. Packed fields retain
   old heap bits; allocation failures and missing slots are not repaired. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_0084b580(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00945667(u32,u32);
extern u32 CM_CDECL cm_fn_00945501(u32,u32);
extern u32 CM_FASTCALL cm_fn_008e5710(u32);
extern u32 CM_CDECL cm_fn_00944c94(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_FASTCALL cm_fn_008e5680(u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_FASTCALL cm_fn_0068d020(u32);
extern u32 CM_THISCALL cm_fn_0068d2f0(u32,u32);
extern u32 CM_STDCALL cm_fn_007875e0(u32);
extern u32 CM_STDCALL cm_fn_008e4610(u32,u32);
/* RET8; explicitly transport the ECX value set at original 008bed1f. */
extern u32 CM_THISCALL cm_fn_008d6bf0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008c6220(u32,u32,s8,u8);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_008be9d0(u32 self, volatile u32 source_arg,
                              volatile u32 replacement_arg, volatile u32 flag_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x20c];
    u32 l = (u32)scratch, previous, flag, source, replacement, table, row;
    u32 eax, id, club_id, club, person_id, person, base, receiver, index;
    u32 capacity, used, old, new_capacity, pointer, scan, allocation, value;
    u32 offset, old_word, version;
    u8 byte_value, old_byte, packed;
    previous = CM_FS(volatile u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00963a9c;
    frame.prev = previous;
    flag = flag_arg;
    CM_FS(volatile u32, 0) = (u32)&frame;
    replacement = replacement_arg;
    source = source_arg;
    if (flag) {
        B(source + 0x2d) = 1;
        B(replacement + 0x2d) = 1;
        id = D(source);
        base = D(0x00ae23b4);
        club_id = D(source + 8);
        club = base + club_id * 581u;
        person_id = D(source + 0xc);
        base = D(0x00ae23bc);
        person = base + person_id * 110u;
        receiver = D(self + 0x856);
        cm_fn_0084b580(receiver, person, club, id);
    }
    index = D(source + 0x28);
    if (index == 0xffffffffu) {
        capacity = D(self + 0x10);
        used = D(self + 0x14);
        if (used == capacity) {
            old = D(self + 0xc);
            new_capacity = capacity + 50u;
            D(self + 0x10) = new_capacity;
            if (old) {
                eax = cm_fn_00945667(old, new_capacity * 4u);
                capacity = D(self + 0x10);
                pointer = eax + capacity * 4u - 200u;
                D(self + 0xc) = eax;
                for (scan = 0; scan < 50; ++scan) {
                    D(pointer) = 0;
                    pointer += 4;
                }
            } else {
                eax = cm_fn_00945501(new_capacity, 4);
                D(self + 0xc) = eax;
            }
        }
        capacity = D(self + 0x10);
        pointer = D(self + 0xc);
        index = 0;
        if ((s32)capacity <= 0) goto diagnostic;
        for (;;) {
            if (D(pointer) == 0) break;
            index++;
            pointer += 4;
            if ((s32)index >= (s32)capacity) goto diagnostic;
        }
        if (index == 0xffffffffu) goto diagnostic;
        D(source + 0x28) = index;
        B(source + 0x2c) = 1;
    }
    table = D(self + 0xc);
    row = D(table + index * 4u);
    if (row) {
        cm_fn_008e5710(row);
        cm_fn_00944c94(row);
    } else {
        value = D(self + 0x14);
        D(self + 0x14) = value + 1u;
    }
    allocation = cm_fn_00944cf1(0x41);
    D(l) = allocation;
    frame.state = 0;
    if (allocation) eax = cm_fn_008e5680(allocation);
    else eax = 0;
    table = D(self + 0xc);
    frame.state = 0xffffffffu;
    D(table + index * 4u) = eax;
    table = D(self + 0xc);
    value = D(replacement);
    row = D(table + index * 4u);
    D(row) = value;
    for (offset = 4; offset <= 0xc; offset += 4) {
        value = D(replacement + offset);
        D(row + offset) = value;
    }
    byte_value = B(replacement + 0x10);
    B(row + 0x10) = byte_value;
    byte_value = B(replacement + 0x11);
    B(row + 0x11) = byte_value;
    cm_fn_00417c10(row + 0x12, l + 4, replacement + 0x12);
    byte_value = B(replacement + 0x1a);
    B(row + 0x1a) = byte_value;
    value = D(replacement + 0x1b);
    D(row + 0x1b) = value;
    byte_value = B(replacement + 0x1f);
    B(row + 0x1f) = byte_value;
    for (offset = 0x20; offset <= 0x28; offset += 4) {
        value = D(replacement + offset);
        D(row + offset) = value;
    }
    for (offset = 0x2c; offset <= 0x2f; ++offset) {
        byte_value = B(replacement + offset);
        B(row + offset) = byte_value;
    }
    byte_value = B(replacement + 0x30);
    old_byte = B(row + 0x30);
    packed = ((byte_value ^ old_byte) & 0xfu) ^ old_byte;
    old_word = D(row + 0x31);
    B(row + 0x30) = packed;
    value = D(replacement + 0x31);
    D(row + 0x31) = ((value ^ old_word) & 0xfu) ^ old_word;
    value = D(replacement + 0x35);
    D(row + 0x35) = value;
    byte_value = B(replacement + 0x39);
    B(row + 0x39) = byte_value;
    value = D(replacement + 0x3a);
    D(row + 0x3a) = value;
    byte_value = B(replacement + 0x3e);
    B(row + 0x3e) = byte_value;
    byte_value = B(replacement + 0x3f);
    B(row + 0x3f) = byte_value;
    byte_value = B(replacement + 0x40);
    B(row + 0x40) = byte_value;
    table = D(self + 0xc);
    source = source_arg;
    row = D(table + index * 4u);
    value = D(source + 0x20);
    D(row + 0x20) = value;
    table = D(self + 0xc);
    value = D(self + 0x85a);
    row = D(table + index * 4u);
    D(row + 0x24) = value;
    value = D(self + 0x85a);
    D(self + 0x85a) = value + 1u;
    pointer = D(replacement + 4);
    if (pointer) {
        allocation = cm_fn_00944cf1(0xe);
        D(l) = allocation;
        frame.state = 1;
        if (allocation) eax = cm_fn_0068d020(allocation);
        else eax = 0;
        table = D(self + 0xc);
        frame.state = 0xffffffffu;
        row = D(table + index * 4u);
        D(row + 4) = eax;
        table = D(self + 0xc);
        value = D(replacement + 4);
        row = D(table + index * 4u);
        receiver = D(row + 4);
        cm_fn_0068d2f0(receiver, value);
    }
    club_id = D(replacement + 8);
    base = D(0x00ae23b4);
    club = base + club_id * 581u;
    (void)D(0x00b7431c); /* Original read; 7875e0 is STD1/RET4. */
    eax = cm_fn_007875e0(club);
    if (eax) {
        byte_value = B(source + 0x1f);
        table = D(self + 0xc);
        row = D(table + index * 4u);
        if (byte_value == 1) cm_fn_008e4610(row, source);
        else cm_fn_008d6bf0(self, row, source);
        eax = cm_fn_008c6220(self, source, 0xe, 0);
    } else eax = cm_fn_008c6220(self, source, 2, 0);
    goto done;
diagnostic:
    cm_fn_00944cff(0x00a8d514, 0, 0, l + 0x10c, l + 0xc);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, l + 0x10c, l + 0xc, 0x60f);
    eax = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return eax;
}
