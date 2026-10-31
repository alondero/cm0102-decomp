#include "cmre.h"

/* WIP: O0/O2 events mismatch, case1/index41: original cleanup calls
   DeleteFileA, C omits it after unwritten Find-output handling. One failure.
   Keep3fixtures/600/VAseed/200000; require original-derived output proof. */
/* Preferred-image VA004542F0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL5/RET0/full EAX, including the original final7FF130 result.
   Scratch234: default15B L24, saved60DWORDs L34, errorsL34/L134.
   SEH frame is separately linked. Signed WORD rankings and sentinel bugs stay. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_00765480(u32);
extern u32 CM_CDECL cm_fn_00600c90(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_005fe950(u32, u32);
extern u32 CM_THISCALL cm_fn_007ff010(u32, u32);
extern u32 CM_THISCALL cm_fn_007fe4a0(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007ff130(u32, u32, u32, u32, u32);
extern void CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

static u32 set_value(u32 record, u32 index, u32 value)
{
    return cm_fn_007ff130(0xb74450, record, index, value, 0);
}

u32 CM_CDECL cm_fn_004542f0(volatile u32 record_arg, volatile u32 second_arg,
                            volatile u32 restore_arg, volatile u32 mode_arg,
                            volatile u32 style_arg)
{
    static const u8 restored[11] = {4,7,8,9,16,17,18,24,25,26,27};
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x234];
    u32 L = (u32)scratch, record, pointer, defaults, value, index, mode;
    u32 club, person, stats, current, remaining, second_player, result, version;
    u16 score, best_score, second_score;
    u8 ranked;

    frame.state = 0xffffffffu;
    frame.handler = 0x9557db;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    D(L + 0x24) = 0x41;
    D(L + 0x28) = 8;
    B(L + 0x30) = 6;
    D(L + 0x2c) = 0x154a21;
    B(L + 0x31) = 0xff;
    B(L + 0x32) = 0xff;
    record = record_arg;
    frame.state = 0;
    if (record == 0) {
        cm_fn_00944cff(0x98e598, 0, 0, (void *)(L + 0x134), (void *)(L + 0x34));
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, version, L + 0x134, L + 0x34, 0x32b);
        result = cm_fn_005e8290(0x9870e0, 0xae24d0, 0);
        D(0xb67a34) = 0;
        goto done;
    }
    pointer = cm_fn_00765480(record);
    if (pointer != 0 && D(pointer + 0xe2) != 0) {
        value = D(pointer + 0xe6);
        D(L + 4) = value;
    } else {
        value = D(record + 0x39);
        if (value == 0) value = D(record + 0x24);
        D(L + 4) = value;
    }
    if (restore_arg != 0) {
        for (index = 0; index != 60; ++index) {
            value = cm_fn_007ff010(0xb74450, index);
            D(L + 0x34 + index * 4u) = value;
        }
    }
    club = D(L + 4);
    if (club != 0 && cm_fn_00600c90(club, 0, 1) != 0) {
        pointer = D(0xb63c98);
        defaults = cm_fn_005fe950(pointer, 0);
    } else defaults = L + 0x24;
    result = cm_fn_007fe4a0(0xb74450, record, 0x454900, 0x4671f0, 0, 0x469da0, 0);
    if (result == 0) goto done;
    value = D(L + 4);
    set_value(record, 0, value);
    value = second_arg;
    set_value(record, 1, value);
    set_value(record, 2, 0);
    set_value(record, 3, 0);
    mode = mode_arg;
    set_value(record, 4, mode != 0 ? 5 : 1);
    set_value(record, 5, 0);
    set_value(record, 6, 0);
    value = D(defaults);
    set_value(record, 7, value);
    value = D(defaults + 4);
    set_value(record, 8, value);
    value = (u32)(s32)(s8)B(defaults + 0xc);
    set_value(record, 9, value);
    set_value(record, 10, 0);
    set_value(record, 11, 0);
    value = W(0xae2c92);
    set_value(record, 12, value);
    set_value(record, 13, 0);
    set_value(record, 14, 0);
    set_value(record, 15, 1);
    set_value(record, 16, 0);
    set_value(record, 17, 0);
    set_value(record, 18, 1);
    set_value(record, 19, 0);
    set_value(record, 20, 0);
    set_value(record, 21, 0);
    set_value(record, 22, 0);
    value = W(0xae2c92);
    set_value(record, 23, value);
    value = D(defaults + 8);
    set_value(record, 24, value);
    value = (u32)(s32)(s8)B(defaults + 0xd);
    set_value(record, 25, value);
    value = (u32)(s32)(s8)B(defaults + 0xe);
    set_value(record, 26, value);
    set_value(record, 27, mode != 0 ? 2 : 1);
    set_value(record, 28, 0);
    set_value(record, 29, 0);
    set_value(record, 30, 0);
    set_value(record, 31, 0);
    if (restore_arg != 0) {
        for (index = 0; index != 11; ++index) {
            current = restored[index];
            value = D(L + 0x34 + current * 4u);
            set_value(record, current, value);
        }
    }
    if (style_arg != 0) {
        value = D(defaults);
        cm_fn_007ff270(0xb74450, 7, value | 0x800u, 0);
    }
    club = D(L + 4);
    second_score = 0xd8f0;
    B(L + 3) = 0;
    W(L + 8) = second_score;
    W(L + 0xc) = second_score;
    D(L + 0x14) = 0;
    D(L + 0x1c) = 0;
    second_player = 0;
    if (club != 0) {
        current = club + 0xd7u;
        D(L + 0x20) = 50;
        D(L + 0x10) = current;
scan_player:
        current = D(L + 0x10);
        person = D(current);
        if (person != 0) {
            stats = D(person + 0x61);
            if (stats != 0) {
                value = cm_fn_0053b900(club);
                stats = D(person + 0x61);
                if (value != 0) score = W(stats + 0xd);
                else score = W(stats + 0xb);
                ranked = B(L + 3);
                if ((s8)ranked < 3) B(L + 3) = (u8)(ranked + 1u);
                best_score = W(L + 8);
                if ((s16)score > (s16)best_score) {
                    D(L + 0x1c) = second_player;
                    second_player = D(L + 0x14);
                    W(L + 0xc) = second_score;
                    second_score = best_score;
                    W(L + 8) = score;
                    D(L + 0x14) = person;
                } else if ((s16)score > (s16)second_score) {
                    W(L + 0xc) = second_score;
                    D(L + 0x1c) = second_player;
                    second_score = score;
                    second_player = person;
                } else if ((s16)score > (s16)W(L + 0xc)) {
                    W(L + 0xc) = score;
                    D(L + 0x1c) = person;
                }
            }
        }
        current = D(L + 0x10);
        remaining = D(L + 0x20);
        current += 4u;
        --remaining;
        D(L + 0x10) = current;
        D(L + 0x20) = remaining;
        if (remaining != 0) {
            club = D(L + 4);
            goto scan_player;
        }
    }
    ranked = B(L + 3);
    if ((s8)ranked > 0) {
        pointer = D(L + 0x14);
        value = D(pointer);
    } else value = 0xffffffffu;
    set_value(record, 32, value);
    ranked = B(L + 3);
    value = (s8)ranked > 1 ? D(second_player) : 0xffffffffu;
    set_value(record, 33, value);
    ranked = B(L + 3);
    if ((s8)ranked > 2) {
        pointer = D(L + 0x1c);
        value = D(pointer);
    } else value = 0xffffffffu;
    result = set_value(record, 34, value);
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
