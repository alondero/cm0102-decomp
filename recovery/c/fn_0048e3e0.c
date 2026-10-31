#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern u32 CM_STDCALL cm_fn_007875e0(u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(u32, u32, u32);

u32 CM_THISCALL cm_fn_0048e3e0(u32 self, u32 club, u32 amount_low, u32 amount_high,
                              u32 kind, u32 year, u32 fixture, u32 option)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0xe0];
    u32 previous, result, threshold, threshold_high, pointer, value, other, won, first, second, context;
    previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095606b;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    /* 007875e0 ignores incoming ECX; retain the original global read. */
    (void)D(0x00b7431c);
    result = cm_fn_007875e0(club);
    if (result == 0)
        goto done;
    threshold = (u32)(s32)(s16)W(club + 0x80) * 50u;
    threshold_high = (threshold & 0x80000000u) != 0 ? 0xffffffffu : 0;
    result = threshold;
    if ((s32)amount_high < (s32)threshold_high ||
        (amount_high == threshold_high && amount_low < threshold))
        goto done;
    cm_fn_0077bbe0((u32)scratch, 0x1a, 0);
    pointer = D(self + 4);
    frame.state = 0;
    value = D(pointer);
    cm_fn_00785790((u32)scratch, 0, value);
    cm_fn_00785790((u32)scratch, 1, amount_low);
    value = D(club);
    cm_fn_00785790((u32)scratch, 2, value);
    cm_fn_00785790((u32)scratch, 3, (u32)(s32)(s8)kind);
    cm_fn_00785790((u32)scratch, 4, (u32)(s32)(s16)year);
    cm_fn_00785790((u32)scratch, 7, (u32)(s32)(s8)option);
    won = 0;
    if (fixture != 0) {
        pointer = D(fixture + 0x1c);
        if (club == pointer) {
            other = D(fixture + 0x20);
            value = D(other);
            cm_fn_00785790((u32)scratch, 5, value);
            if (B(fixture + 0x3d) != 0) {
                won = 1;
            } else {
                first = B(fixture + 0x4b);
                second = B(fixture + 0x4c);
                if ((s8)first > (s8)second) {
                    won = 1;
                } else {
                    first = B(fixture + 0x43);
                    second = B(fixture + 0x44);
                    if ((s8)first > (s8)second)
                        won = 1;
                }
            }
        } else {
            value = D(pointer);
            cm_fn_00785790((u32)scratch, 5, value);
            if (B(fixture + 0x3e) != 0) {
                won = 1;
            } else {
                first = B(fixture + 0x4b);
                second = B(fixture + 0x4c);
                if ((s8)first < (s8)second) {
                    won = 1;
                } else {
                    first = B(fixture + 0x43);
                    second = B(fixture + 0x44);
                    if ((s8)first < (s8)second)
                        won = 1;
                }
            }
        }
    } else {
        cm_fn_00785790((u32)scratch, 5, 0xffffffffu);
    }
    cm_fn_00785790((u32)scratch, 6, won);
    context = D(0x00b7431c);
    result = (u32)cm_fn_007861e0(context, (u32)scratch, club);
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
