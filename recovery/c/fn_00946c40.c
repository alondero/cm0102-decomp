#include "cmre.h"
/* WIP: -O0 passes, -O2 differs on page 0xad9000 (C runtime globals; cases 7/11/13). Likely the randomized-CRT-state harness gap; unresolved. */

extern void CM_CDECL cm_fn_0094c9ae(void);
extern u32 CM_CDECL cm_fn_0094cc21(const volatile u8 *record);
extern volatile s32 *CM_CDECL cm_fn_0094cf0d(volatile s32 *input);

/* Cdecl, one stack argument (the record pointer P at arg_04), `ret 0`.
   Returns the record pointer returned by 0x94cf0d (EAX). The stack argument
   slot is used as a local (x in the listing) and is modelled as a local.
   The record is 4-byte fields: [0] seconds, [1] minutes, [2] hours, [3] a
   running value, [4] an 11 month field, [5] a counter, [6] a day-of-week
   field (mod 7), [7] a day-count field, [8] a flag. The base values come from
   globals 0x00ad91f8 (subtracted), 0x00ad91fc (gate), 0x00ad9200 (subtracted
   when the 0x94cc21 test is non-zero).
   Negative first field: returns 0. Values in (0x3f480, 0x7ffc0b7f) go through
   0x94cf0d twice (see listing). Otherwise the routine splits into seconds,
   minutes, hours and days with C truncating division and the negative-remainder
   corrections as the listing has them. */
u32 CM_CDECL cm_fn_00946c40(u32 arg_04)
{
    volatile s32 *esi;
    s32 x;
    s32 t;
    s32 flag;
    s32 q;
    s32 rem;
    s32 dq;

    esi = (volatile s32 *)arg_04;
    if (*esi < 0)
        return 0;

    cm_fn_0094c9ae();
    t = *esi;
    if (t > 0x3f480 && t < 0x7ffc0b7f) {
        x = t - *(volatile s32 *)0x00ad91f8;
        esi = cm_fn_0094cf0d(&x);
        if (*(volatile s32 *)0x00ad91fc == 0)
            return (u32)esi;
        if (cm_fn_0094cc21((const volatile u8 *)esi) == 0)
            return (u32)esi;
        x -= *(volatile s32 *)0x00ad9200;
        esi = cm_fn_0094cf0d(&x);
        esi[8] = 1;
        return (u32)esi;
    }

    esi = cm_fn_0094cf0d((volatile s32 *)arg_04);
    flag = (s32)cm_fn_0094cc21((const volatile u8 *)esi);
    t = esi[0];
    if (flag != 0)
        t -= *(volatile s32 *)0x00ad9200;
    t -= *(volatile s32 *)0x00ad91f8;

    x = t;
    rem = x % 60;
    esi[0] = rem;
    if (rem < 0) {
        esi[0] = rem + 60;
        x -= 60;
    }

    q = x / 60;
    x = q + esi[1];
    rem = x % 60;
    esi[1] = rem;
    if (rem < 0) {
        esi[1] = rem + 60;
        x -= 60;
    }

    q = x / 60;
    x = q + esi[2];
    rem = x % 24;
    esi[2] = rem;
    if (rem < 0) {
        esi[2] = rem + 24;
        x -= 24;
    }

    dq = x / 24;
    x = dq;

    if (dq > 0) {
        rem = (esi[6] + dq) % 7;
        esi[6] = rem;
        esi[3] += x;
        esi[7] += x;
        return (u32)esi;
    }
    if (dq == 0)
        return (u32)esi;

    rem = (esi[6] + dq + 7) % 7;
    esi[6] = rem;
    esi[3] += x;
    if (esi[3] > 0) {
        esi[7] += x;
        return (u32)esi;
    }
    esi[3] = esi[3] + 0x1f;
    esi[5] -= 1;
    esi[7] = 0x16c;
    esi[4] = 0x0b;
    return (u32)esi;
}
