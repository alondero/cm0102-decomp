#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_FASTCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_0054cdb0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00669bd0(u32, u32, u32);

u32 CM_FASTCALL cm_fn_0062a920(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x28];
    u32 local = (u32)scratch;
    u32 value, base, slot, cursor, person, id, related, score;
    u32 first, second, table, state, index, limit, doubled;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095b090;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    D(local + 0x14) = self;
    cm_fn_0054a110(local + 0x18);
    frame.state = 0;
    cm_fn_0054a110(local + 0x20);
    doubled = D(0x00ae2350);
    cursor = D(0x00ae23b4);
    *(volatile u8 *)&frame.state = 1;
    D(local + 8) = 0;
    doubled += doubled;
    value = D(0x00ae235c);
    value -= doubled;
    D(local) = cursor;
    if ((s32)value <= 0)
        goto done;
    slot = cursor + 0xd7;
    D(local + 0xc) = slot;
    do {
        value = cm_fn_00669bd0(cursor, local + 0x18, local + 0x20);
        if (value == 0) {
            second = D(0x00ae2c94);
            first = D(0x00ae2c90);
            base = cm_fn_0054cdb0(local + 0x18, first, second);
            if ((s32)base > 0x50)
                base = 0x28;
            else if ((s32)base > 0x28)
                base = (u32)(s32)(s8)(0x50 - (u8)base);
        } else {
            base = 0;
        }
        cursor = slot;
        D(local + 4) = 0x32;
        do {
            person = D(cursor);
            if (person != 0) {
                id = D(person);
                limit = D(0x00ae2364);
                if ((s32)id < (s32)limit) {
                    related = D(person + 0x61);
                    if (related != 0) {
                        first = (u32)((s32)(s8)B(related + 0x34) / 2);
                        second = (u32)((s32)(s8)B(person + 0x5b) / 4);
                        score = base - second - first;
                        if ((s32)score < 1)
                            score = 1;
                        else if ((s32)score > 0x28)
                            score = 0x28;
                        score = 0x2710 - score * 75;
                        table = D(local + 0x14);
                        table = D(table);
                        W(id * 31 + table + 8) = (u16)score;
                    } else {
                        person = D(slot);
                        first = D(local);
                        if (D(person + 0x39) == first) {
                            state = B(person + 0x3d);
                            if (state == 0xb) {
                                D(person + 0x39) = 0;
                                person = D(slot);
                                B(person + 0x3d) = 8;
                            } else if (state == 0xf) {
                                B(person + 0x3d) = 8;
                            } else if (state == 0xd) {
                                B(person + 0x3d) = 6;
                            }
                        }
                        D(slot) = 0;
                    }
                }
            }
            value = D(local + 4);
            slot += 4;
            cursor += 4;
            --value;
            D(local + 4) = value;
        } while (value != 0);
        doubled = D(0x00ae2350);
        value = D(local + 8);
        limit = D(0x00ae235c);
        cursor = D(local);
        slot = D(local + 0xc);
        doubled += doubled;
        ++value;
        limit -= doubled;
        cursor += 0x245;
        slot += 0x245;
        index = value;
        D(local + 8) = index;
        D(local) = cursor;
        D(local + 0xc) = slot;
    } while ((s32)value < (s32)limit);
done:
    CM_FS(u32, 0) = frame.prev;
    return value;
}
