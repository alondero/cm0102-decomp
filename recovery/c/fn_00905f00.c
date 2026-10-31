#include "cmre.h"

/* Preferred-image VA 0x00905f00, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The original 00785790 reads its index with MOV AL,[entry ESP+4] and
   consumes only its signed low BYTE. Forwarded incoming EBX high bits and
   unwritten counter high bytes therefore do not affect that callee.
   Scratch retains both BYTE counter writes and original DWORD reads. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0077bbe0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00785790(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00786520(u32, u32, u32);

u32 CM_CDECL cm_fn_00905f00(void)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0xa4c];
    u32 local = (u32)scratch, prev, doubled, club, remaining, selected = 0;
    u32 nation, country, wanted, value, key, category, owner = 0, table;
    u32 message = local + 0x0c, result, manager, counter, count;
    u8 index, third = 0;
    prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0096528b;
    frame.prev = prev;
    CM_FS(u32, 0) = (u32)&frame;
    value = D(0x00ae2350);
    club = D(0x00ae23b4);
    doubled = value + value;
    remaining = D(0x00ae235c) - doubled;
    B(local + 4) = 0;
    B(local + 3) = 0;
    if ((s32)remaining > 0) {
        wanted = D(0x009cfa10);
        do {
            nation = D(club + 0x53);
            if (nation == 0) goto scan_next;
            if (D(club + 0x1db) == 0xffffffffu) goto scan_next;
            if (B(nation + 0x11c) == 0) goto scan_next;
            country = D(nation + 0x71);
            if (country == 0) goto scan_next;
            if (D(country) == wanted) {
                counter = (u32)(s32)(s16)(u16)selected;
                ++selected;
                D(local + 0xec + counter * 4u) = club;
            }
scan_next:
            club += 581;
            --remaining;
        } while (remaining != 0);
    }
    count = (u32)(s32)(s16)(u16)selected;
    cm_fn_00945333(local + 0xec, count, 4, 0x00906410);
    cm_fn_0077bbe0(message, 0x2f, 0);
    frame.state = 0;
    value = W(0x00ae2c92);
    result = cm_fn_00785790(message, 30, value);
    if ((s32)count <= 0) goto done;
    table = local + 0xec;
    D(local + 8) = count;
    do {
        club = D(table);
        nation = D(club + 0x53);
        if (nation != owner) {
            if (owner != 0) {
                value = D(local + 4);
                cm_fn_00785790(message, value, 0xffffffffu);
                index = B(local + 3);
                cm_fn_00785790(message, (u8)(index + 10u), 0xffffffffu);
                cm_fn_00785790(message, (u8)(third + 20u), 0xffffffffu);
                manager = D(0x00b7431c);
                cm_fn_00786520(manager, message, owner);
                B(local + 4) = 0;
                B(local + 3) = 0;
                third = 0;
            }
            owner = D(club + 0x53);
            value = D(owner);
            cm_fn_00785790(message, 31, value);
        }
        key = D(club + 0x1db);
        category = D(0x009cf6e8);
        if (key == category) {
            index = B(local + 4);
            value = D(club);
            B(local + 4) = (u8)(index + 1u);
            cm_fn_00785790(message, index, value);
        } else if (key == D(0x009cf6f0)) {
            index = B(local + 3);
            value = D(club);
            cm_fn_00785790(message, (u8)(index + 10u), value);
            index = B(local + 3);
            B(local + 3) = (u8)(index + 1u);
        } else if (key == D(0x009cf6f8)) {
            value = D(club);
            cm_fn_00785790(message, (u8)(third + 20u), value);
            third = (u8)(third + 1u);
        }
        result = D(local + 8);
        table += 4;
        --result;
        D(local + 8) = result;
    } while (result != 0);
    if (owner != 0) {
        value = D(local + 4);
        cm_fn_00785790(message, value, 0xffffffffu);
        index = B(local + 3);
        cm_fn_00785790(message, (u8)(index + 10u), 0xffffffffu);
        third = (u8)(third + 20u);
        cm_fn_00785790(message, third, 0xffffffffu);
        manager = D(0x00b7431c);
        result = cm_fn_00786520(manager, message, owner);
    }
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
