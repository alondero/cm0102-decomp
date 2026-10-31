#include "cmre.h"
/* Preferred-image VA005ef5e0/base00400000, pinned 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS3/RET12/full boolean EAX, SEH95ac1b. Scratch4dc:
   baseL0/date outputL4/namesLc,L3e/timeL70/scoreL74/yearL78/dateL7c;
   stringsL84,L14c,L214 and diagnosticsL2dc,L3dc. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_FASTCALL cm_fn_0054a110(u32);
extern u32 CM_CDECL cm_fn_0053dba0(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_009466d0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00542100(u32,u32,u32,s32,s8,u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32);
extern u32 CM_CDECL cm_fn_00946ebb(u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern void CM_CDECL cm_fn_00945333(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

static __attribute__((always_inline)) inline void copy_string(u32 destination, u32 source)
{
    u32 p = source, left = 0xffffffffu, length, words, tail;
    do {
        u8 c = B(p);
        p++;
        left--;
        if (!c) break;
    } while (left != 0);
    length = ~left;
    source = p - length;
    words = length >> 2;
    tail = length & 3u;
    while (words) {
        u32 value = D(source);
        D(destination) = value;
        source += 4;
        destination += 4;
        words--;
    }
    while (tail) {
        u8 value = B(source);
        B(destination) = value;
        source++;
        destination++;
        tail--;
    }
}

u32 CM_THISCALL cm_fn_005ef5e0(u32 self, volatile s8 kind_arg,
                              volatile u32 record_arg, volatile u32 score_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x4dc];
    u32 l = (u32)scratch, record, score, base, slot, pointer, time_value;
    u32 year, saved_score, cursor, row, p, q, i, version, result = 0;
    u16 date_word;
    u8 c, other, nation_first, club_first;
    s8 kind;
    int equal;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095ac1b;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    cm_fn_0054a110(l + 0x7c);
    kind = kind_arg;
    frame.state = 0;
    if (kind < 0 || kind > 0x1b) goto diagnostic;
    record = record_arg;
    if (!record) goto diagnostic;
    score = score_arg;
    base = self + (u32)(s32)kind * 12000u;
    D(l) = base;
    slot = base + 0x2e68;
    if (!score) goto done;
    if (score <= D(slot + 0x68)) goto done;
    cm_fn_0053dba0(record, l + 0x84, 0xc8, 1);
    cm_fn_009466d0(l + 0xc, l + 0x84, 0x32);
    pointer = D(record + 0x39);
    B(l + 0x3d) = 0;
    if (pointer) cm_fn_00542100(pointer, 0, l + 0x214, 0xc8, 1, 1);
    else B(l + 0x214) = 0;
    pointer = D(record + 0x24);
    if (pointer) {
        cm_fn_00542100(pointer, 0, l + 0x14c, 0xc8, 1, 1);
        nation_first = B(l + 0x14c);
    } else {
        nation_first = 0;
        B(l + 0x14c) = 0;
    }
    club_first = B(l + 0x214);
    if (club_first) {
        if (nation_first)
            cm_fn_00944c9f(l + 0x84, 0x009ce448, l + 0x214, l + 0x14c);
        else copy_string(l + 0x84, l + 0x214);
    } else if (nation_first) copy_string(l + 0x84, l + 0x14c);
    else cm_fn_0066f4e0(l + 0x84, 0x009ce438);
    cm_fn_009466d0(l + 0x3e, l + 0x84, 0x32);
    B(l + 0x6f) = 0;
    time_value = cm_fn_00946ebb(0);
    year = D(0x00dd7120);
    D(l + 0x78) = year;
    D(l + 0x70) = time_value;
    D(l + 0x74) = score;
    cm_fn_00417c10(l + 0x7c, l + 4, 0x00ae2c90);
    base = D(l);
    cursor = base + 0x6c;
    for (row = 0; row < 99; ++row) {
        p = cursor - 0x6c;
        q = l + 0xc;
        for (;;) {
            c = B(p);
            other = B(q);
            if (c != other) { equal = 0; break; }
            if (!c) { equal = 1; break; }
            c = B(p + 1);
            other = B(q + 1);
            if (c != other) { equal = 0; break; }
            p += 2;
            q += 2;
            if (!c) { equal = 1; break; }
        }
        if (equal) {
            year = D(cursor);
            if (year == D(l + 0x78)) {
                date_word = W(l + 0x7e);
                if (date_word == W(cursor + 6)) {
                    date_word = W(l + 0x7c);
                    if (date_word == W(cursor + 4)) goto done;
                }
            }
        }
        cursor += 0x78;
    }
    time_value = D(l + 0x70);
    for (i = 0; i < 12; ++i) {
        u32 value = D(l + 0xc + i * 4u);
        D(slot + i * 4u) = value;
    }
    date_word = W(l + 0x3c);
    W(slot + 0x30) = date_word;
    year = D(l + 0x78);
    for (i = 0; i < 12; ++i) {
        u32 value = D(l + 0x3e + i * 4u);
        D(slot + 0x32 + i * 4u) = value;
    }
    saved_score = D(l + 0x74);
    date_word = W(l + 0x6e);
    W(slot + 0x62) = date_word;
    D(slot + 0x64) = time_value;
    D(slot + 0x68) = saved_score;
    D(slot + 0x6c) = year;
    cm_fn_00417c10(slot + 0x70, l + 4, l + 0x7c);
    base = D(l);
    cm_fn_00945333(base, 100, 0x78, 0x005f0620);
    result = 1;
    goto done;
diagnostic:
    cm_fn_00944cff(0x009ce408, 0, 0, l + 0x3dc, l + 0x2dc);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, l + 0x3dc, l + 0x2dc, 0x13b);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return result;
}
