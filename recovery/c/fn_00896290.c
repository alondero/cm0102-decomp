#include "cmre.h"

/* Preferred-image VA00896290, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/fullEAX, SEH96334b. Scratch24c: integer/doubleL0, loopLc,
   doubleL10/L18/L20, recordL2c (dateL2c/scoresL45,L46), diagnosticsL4c/L14c.
   Keep x87 extended intermediates alongside explicit original FST64 spills.
   7e4750 writes both score bytes on every returning nonnull-input path. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
#define F(a) (*(volatile double *)(a))
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u8, u32, u32);
extern unsigned long long CM_CDECL cm_fn_007e4750(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00896290(u32 self, volatile u32 input_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x24c];
    u32 l = (u32)scratch, input, club, value, id, map, base, row, index;
    u32 name, extension, line, result, early;
    u16 games, goals_for, goals_against, points, word;
    u8 first, second;
    long double a, b, c, d, p, p2, g, r;
    frame.state = 0xffffffffu; frame.handler = 0x0096334b;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    cm_fn_0054a110(l + 0x2c);
    input = input_arg;
    frame.state = 0;
    if (input == 0) {
        name = l + 0x14c; extension = l + 0x4c; line = 0x1f9; early = 1;
        goto diagnostic;
    }
    club = D(input + 0x1c);
    if (club != 0 && cm_fn_00600a50(club, 0, 1, 0, 0) != 0) {
        result = 0;
        goto done;
    }
    club = D(input + 0x20);
    if (club != 0 && cm_fn_00600a50(club, 0, 1, 0, 0) != 0) {
        result = 0;
        goto done;
    }
    index = 0;
    D(l + 0xc) = index;
next:
    if (index == 0) club = D(input + 0x1c);
    else club = D(input + 0x20);
    if (club == 0) {
        name = l + 0x4c; extension = l + 0x14c; line = 0x20a; early = 0;
        goto diagnostic;
    }
    id = D(club);
    map = D(self + 0x601);
    id = (u32)(s32)(s16)W(map + id * 145u);
    base = D(self);
    row = base + id * 40u;
    cm_fn_007e4750(input, club, l + 0x2c);
    W(row + 2) = (u16)(W(row + 2) + 1u);
    first = B(l + 0x45);
    second = B(l + 0x46);
    games = W(row + 2);
    if ((s8)first > (s8)second) {
        W(row + 4) = (u16)(W(row + 4) + 1u);
        W(row + 0xe) = (u16)(W(row + 0xe) + 3u);
    } else if (first == second) {
        W(row + 6) = (u16)(W(row + 6) + 1u);
        W(row + 0xe) = (u16)(W(row + 0xe) + 1u);
    } else W(row + 8) = (u16)(W(row + 8) + 1u);
    word = (u16)(s16)(s8)B(l + 0x45);
    W(row + 0xa) = (u16)(W(row + 0xa) + word);
    goals_for = W(row + 0xa);
    word = (u16)(s16)(s8)B(l + 0x46);
    W(row + 0xc) = (u16)(W(row + 0xc) + word);
    goals_against = W(row + 0xc);

    if ((s16)goals_for < 1) D(l) = 1;
    else D(l) = (u32)(s32)(s16)goals_for;
    a = (long double)(s32)D(l);
    if ((s16)goals_against < 1) D(l) = 1;
    else D(l) = (u32)(s32)(s16)goals_against;
    points = W(row + 0xe);
    b = (long double)(s32)D(l);
    if ((s16)points < 1) D(l) = 1;
    else D(l) = (u32)(s32)(s16)points;
    c = (long double)(s32)D(l);
    F(l + 0x10) = (double)c;
    if ((s16)games < 1) D(l) = 1;
    else D(l) = (u32)(s32)(s16)games;
    d = (long double)(s32)D(l);
    c = (long double)F(l + 0x10);
    p = c / d;
    F(l + 0x18) = (double)p;
    p2 = p * p;
    F(l + 0x20) = (double)p2;
    g = a / d;
    F(l) = (double)g;
    F(l + 0x10) = (double)(g * p2);
    r = d / b;
    F(row + 0x10) = (double)((r * (long double)F(l + 0x10)) * (long double)F(l));
    F(row + 0x18) = (double)(((r * (long double)F(l)) *
                                  (long double)F(l + 0x20)) * (long double)F(l + 0x18));
    F(row + 0x20) = (double)((r * r) * (long double)F(l + 0x10));
    index = D(l + 0xc);
advance:
    ++index;
    D(l + 0xc) = index;
    if ((s32)index < 2) goto next;
    result = 1;
    goto done;
diagnostic:
    cm_fn_00944cff(0x00a8b908, 0, 0, name, extension);
    value = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, value, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    if (early == 0) goto advance;
    result = 0;
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return result;
}
