/* WIP: outcome mismatch (original fault, source return), case 1 */
#include "cmre.h"

extern short *CM_THISCALL cm_fn_0054c8e0(void *self, short *value, u32 mode);
extern short *CM_THISCALL cm_fn_0054c5b0(void *self, short *value, u32 mode);

int CM_THISCALL cm_fn_005c29b0(volatile u8 *self, u32 arg,
    volatile u32 *second)
{
    struct seh_frame { u32 prev, handler, state; } volatile frame;
    frame.prev = CM_FS(u32, 0);
    frame.state = (u32)-1;
    frame.handler = 0x00959de8u;
    CM_FS(u32, 0) = (u32)(unsigned long)&frame;

    if (arg != *(volatile u32 *)(self + 4)) {
        u32 global = *(volatile u32 *)0x00ae2378u;
        s32 value = *(volatile s32 *)arg;
        if (value >= (s32)(global - 0x7fu)) {
            CM_FS(u32, 0) = frame.prev;
            return 1;
        }
    }

    s8 a = *(volatile s8 *)arg;
    s8 b = *(volatile s8 *)(self + 0xaf);
    s8 index = (s8)((u8)a - (u8)b);
    volatile u8 *record = self + (s32)index * 105;
    if (*(volatile u32 *)(record + 0xb1) == 0) {
        CM_FS(u32, 0) = frame.prev;
        return 0;
    }

    volatile u32 pair0 = second[0];
    volatile u32 pair1 = second[1];
    volatile u32 locals[6];
    volatile u32 *lower = &locals[2];
    lower[0] = pair0;
    lower[1] = pair1;
    short *got = cm_fn_0054c8e0((void *)(record + 0xb5), (short *)lower, 7);
    u16 got_hi = *(volatile u16 *)((u8 *)got + 2);
    u16 asked_hi = *(volatile u16 *)((u8 *)lower + 2);
    frame.state = 0;
    int lower_match = asked_hi > got_hi;
    if (asked_hi == got_hi) {
        s16 got_lo = *(volatile s16 *)got;
        lower_match = (s16)pair0 > got_lo;
    }

    if (lower_match) {
        pair0 = second[0];
        pair1 = second[1];
        volatile u32 *upper = &locals[4];
        upper[0] = pair0;
        upper[1] = pair1;
        got = cm_fn_0054c5b0((void *)(record + 0xb5), (short *)upper, 7);
        got_hi = *(volatile u16 *)((u8 *)got + 2);
        asked_hi = *(volatile u16 *)((u8 *)upper + 2);
        int upper_match = asked_hi < got_hi;
        if (asked_hi == got_hi) {
            s16 got_lo = *(volatile s16 *)got;
            upper_match = (s16)pair0 < got_lo;
        }
        if (upper_match) {
            CM_FS(u32, 0) = frame.prev;
            return 1;
        }
    }
    CM_FS(u32, 0) = frame.prev;
    return 0;
}
