/* WIP: case4 import events diverge in original CRT file enumeration;
   seeded imports leave output buffers unwritten, so stack bytes feed
   00946985's FILETIME guard. One failed invocation; four remain. */
#include "cmre.h"

/* Preferred-image VA00453D60/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL5/RET0; all five direct callers discard EAX. L is the original
   120-byte scratch allocation, distinct from the linked SEH frame. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
#define SET(i, v) cm_fn_007ff270(0xb74450, (i), (u32)(v), 0)
extern u32 CM_CDECL cm_fn_00542740(u32);
extern u32 CM_CDECL cm_fn_00600c90(u32, u32, u8);
extern u32 CM_THISCALL cm_fn_005fe950(u32, u32);
extern u32 CM_THISCALL cm_fn_007fe600(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007ff010(u32, u32);
extern void CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);

void CM_CDECL cm_fn_00453d60(volatile u32 club_arg, volatile u32 value_arg,
    volatile u32 restore_arg, volatile u32 mode_arg, volatile u32 flag_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x120];
    u32 L = (u32)scratch, club, profile, i, value, mode, record, stats;
    u32 second, scan_club;
    u16 score, factor, best_score, second_score, third_score;
    u8 found;
    frame.state = 0xffffffff;
    frame.handler = 0x9557bb;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    D(L + 0x20) = 0x41;
    D(L + 0x24) = 8;
    B(L + 0x2c) = 6;
    D(L + 0x28) = 0x154a21;
    B(L + 0x2d) = 0xff;
    B(L + 0x2e) = 0xff;
    club = club_arg;
    frame.state = 0;
    if (club != 0 && cm_fn_00542740(club) == 0) value_arg = 0;
    if (restore_arg != 0) {
        for (i = 0; i != 60; ++i) {
            value = cm_fn_007ff010(0xb74450, i);
            D(L + 0x30 + i * 4) = value;
        }
    }
    if (club != 0 && cm_fn_00600c90(club, 0, 1) != 0) {
        value = D(0xb63c98);
        profile = cm_fn_005fe950(value, 0);
    } else profile = L + 0x20;
    value = cm_fn_007fe600(0xb74450, 0x454900, 0x4671f0, 0, 0x469da0, 0);
    if (value == 0) goto done;
    SET(0, club);
    value = value_arg;
    SET(1, value);
    SET(2, 0);
    SET(3, 0);
    mode = mode_arg;
    SET(4, mode != 0 ? 5 : 1);
    SET(5, 0);
    SET(6, 0);
    value = D(profile); SET(7, value);
    value = D(profile + 4); SET(8, value);
    value = (u32)(s32)(s8)B(profile + 0xc); SET(9, value);
    SET(10, 0);
    SET(11, 0);
    value = W(0xae2c92); SET(12, value);
    SET(13, 0);
    SET(14, 0);
    SET(15, 1);
    SET(16, 0);
    SET(17, 0);
    SET(18, 1);
    SET(19, 0);
    SET(20, 0);
    SET(21, 0);
    SET(22, 0);
    value = W(0xae2c92); SET(23, value);
    value = D(profile + 8); SET(24, value);
    value = (u32)(s32)(s8)B(profile + 0xd); SET(25, value);
    value = (u32)(s32)(s8)B(profile + 0xe); SET(26, value);
    SET(27, mode != 0 ? 2 : 1);
    SET(28, 0);
    SET(29, 0);
    SET(30, 0);
    SET(31, 0);
    if (restore_arg != 0) {
        value = D(L + 0x40); SET(4, value);
        value = D(L + 0x4c); SET(7, value);
        value = D(L + 0x50); SET(8, value);
        value = D(L + 0x54); SET(9, value);
        value = D(L + 0x70); SET(16, value);
        value = D(L + 0x74); SET(17, value);
        value = D(L + 0x78); SET(18, value);
        value = D(L + 0x90); SET(24, value);
        value = D(L + 0x94); SET(25, value);
        value = D(L + 0x98); SET(26, value);
        value = D(L + 0x9c); SET(27, value);
    }
    if (flag_arg != 0) {
        value = D(profile);
        SET(7, value | 0x800u);
    }
    scan_club = club_arg;
    third_score = (u16)-10000;
    second = 0;
    B(L + 3) = 0;
    W(L + 0x18) = third_score;
    second_score = third_score;
    D(L + 0xc) = 0;
    D(L + 0x14) = 0;
    if (scan_club != 0) {
        D(L + 8) = 50;
        D(L + 4) = scan_club + 0xd7;
        for (;;) {
            value = D(L + 4);
            record = D(value);
            if (record != 0) {
                stats = D(record + 0x61);
                if (stats != 0) {
                    value = cm_fn_0053b900(scan_club);
                    stats = D(record + 0x61);
                    if (value != 0) score = W(stats + 0xd);
                    else score = W(stats + 0xb);
                    factor = W(stats + 5);
                    factor = (u16)((u32)factor * 10u);
                    score = (u16)((u32)score + factor);
                    found = B(L + 3);
                    if ((s8)found < 3) { ++found; B(L + 3) = found; }
                    best_score = W(L + 0x18);
                    if ((s16)score > (s16)best_score) {
                        D(L + 0x14) = second;
                        second = D(L + 0xc);
                        third_score = second_score;
                        second_score = best_score;
                        W(L + 0x18) = score;
                        D(L + 0xc) = record;
                    } else if ((s16)score > (s16)second_score) {
                        third_score = second_score;
                        D(L + 0x14) = second;
                        second_score = score;
                        second = record;
                    } else if ((s16)score > (s16)third_score) {
                        third_score = score;
                        D(L + 0x14) = record;
                    }
                }
            }
            value = D(L + 4);
            i = D(L + 8);
            value += 4;
            --i;
            D(L + 4) = value;
            D(L + 8) = i;
            if (i == 0) break;
            scan_club = club_arg;
        }
        found = B(L + 3);
        if ((s8)found > 0) {
            value = D(L + 0xc);
            value = D(value);
        } else value = 0xffffffff;
    } else value = 0xffffffff;
    SET(32, value);
    found = B(L + 3);
    value = (s8)found > 1 ? D(second) : 0xffffffff;
    SET(33, value);
    if ((s8)found > 2) {
        value = D(L + 0x14);
        value = D(value);
    } else value = 0xffffffff;
    SET(34, value);
done:
    value = frame.prev;
    CM_FS(volatile u32, 0) = value;
}
