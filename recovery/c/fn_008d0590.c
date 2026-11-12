/* fn_008d0590 - recovered plain C; acceptance is recorded by cmre.
 *
 * Original: CM0102/Game/cm0102.exe, Championship Manager 01/02 3.9.68,
 *   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86
 *   preferred image base 0x00400000, function VA 0x008d0590, body 489 bytes.
 * Evidence: navigation packet
 *   CM0102/Analysis/cmre/evidence/fn_008d0590-99faa619b093dc2675dc2907d5bcecb0fcb06d58d55eab46740f98e5e8bb1305.txt
 *   plus the pinned listings of the direct callees 0x00549F70, 0x0054A110 and
 *   0x0054CF70 read from the same image.
 *
 * ABI: thiscall (ECX = this), seven stack dwords (ret 0x1c), result in EAX.
 * Every callee below stays an explicit original-code boundary: the build
 * binds each undefined cm_fn_<va> to its preferred-image address.
 */
#include "cmre.h"

/* Original callees (preferred-image VAs). */
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern u32 CM_THISCALL cm_fn_0054cf70(u32 self);
extern volatile u32 *CM_THISCALL cm_fn_0054a110(volatile u32 *self);
extern volatile u32 *CM_THISCALL cm_fn_00549f70(volatile u32 *self, u32 day,
                                                u32 month, u32 year, u32 target);

/* The slot at vtable + 0x28. The original pushes seven dwords in the order
   a3, a7, a6, a5, a4, a2, a1 (a1 last) and the callee returns with
   ret 0x1c, so the slot takes the object in ECX plus seven stack dwords. */
typedef void (CM_THISCALL *cm_slot_008d0590)(u32 self, u32 a1, u32 a2, u32 a4,
                                              u32 a5, u32 a6, u32 a7, u32 a3);

/* Original globals (preferred-image VAs). 0x00ae2c90 is the eight-byte
   Date value {u16 day-of-year, u16 year, u32 leap}. */
#define G_DAY_YEAR CM_GLOBAL(volatile u32, 0x00ae2c90)
#define G_LEAP     CM_GLOBAL(volatile u32, 0x00ae2c94)
#define G_YEAR     CM_GLOBAL(volatile u16, 0x00ae2c92)
#define G_TABLE    CM_GLOBAL(volatile u32, 0x00ae1050)

/* The original's 16-byte frame area: the copied global date at +0 and the
   constructed date at +8. One allocation keeps the original offsets. */
typedef union { u32 d[4]; u16 w[8]; } cm_area;

u32 CM_THISCALL cm_fn_008d0590(u32 self, volatile u32 a1,
                               volatile u32 a2, volatile u32 a3,
                               volatile u32 a4, volatile u32 a5,
                               volatile u32 a6, volatile u32 a7)
{
    volatile cm_area area;
    u32 key, slot, found, entry, vtable, record, day_word;
    u32 global, leap, record_year, record_day;
    u32 limit, month_arg, built0, built1, other_year, this_year, def_ptr;
    u32 candidate, cached_a1, cached_a2, cached_a3, cached_a4;
    u32 cached_a5, cached_a6, cached_a7, method;
    u32 year_destination, month_destination, limit_year;
    u8 other_month, this_month;
    volatile u32 *built, *def;
    u16 year;
    u8 seen, month;

    /* 0x008d0593 xor dl,dl / 0x008d0595 add ecx,0x8ac
       0x008d05a5 mov esi,dword ptr [ebp+0x53] : the search key. */
    cached_a1 = a1;
    key = *(volatile u32 *)(cached_a1 + 0x53);
    slot = self + 0x8ac;
    found = 0;
    seen = 0;
    for (;;) {
        candidate = *(volatile u32 *)slot;                    /* 0x008d05a8 */
        if (*(volatile u32 *)(candidate + 0xa) == key) {       /* 0x008d05aa */
            found = 1;
            break;                                            /* 0x008d05ad */
        }
        seen++;                                               /* 0x008d05b3 */
        slot += 4;                                             /* 0x008d05b5 */
        if (seen >= 0x1c) break;                               /* 0x008d05b8 */
    }
    if (found) {
        entry = *(volatile u32 *)slot;                         /* 0x008d065f */
        if (entry == 0)                                        /* 0x008d0661 */
            entry = *(volatile u32 *)(self + 0x8ac);           /* 0x008d05cd */
    } else {
        entry = *(volatile u32 *)(self + 0x8ac);               /* 0x008d05cd */
    }

    /* 0x008d05d2 / 0x008d067b: the object's vtable, then the slot call.
       The original pushes a3, a7, a6, a5, a4, a2, a1 (a1 last). */
    cached_a3 = a3;
    cached_a7 = a7;
    cached_a6 = a6;
    cached_a5 = a5;
    vtable = *(volatile u32 *)entry;
    cached_a4 = a4;
    cached_a2 = a2;
    method = *(volatile u32 *)(vtable + 0x28);
    ((cm_slot_008d0590)method)(entry, cached_a1, cached_a2,
        cached_a4, cached_a5, cached_a6, cached_a7, cached_a3);

    /* 0x008d05e3-0x008d05f6: the day of month of the global date and the
       record that carries the compared date. */
    day_word = cm_fn_0054cf70(0x00ae2c90u);                   /* 0x008d05e8 */
    record = cm_fn_004cf5a0(G_TABLE, cached_a3);               /* 0x008d05f6 */

    /* 0x008d05fd-0x008d060e: no record, or a record whose +0x35 field is
       not 2, gives the default year. */
    if (record == 0 || ((*(volatile u8 *)(record + 0x35)) & 0x3f) != 2)
        goto default_at_8;                                    /* 0x008d075f */

    global = G_DAY_YEAR;                                      /* 0x008d0614 */
    leap = G_LEAP;                                            /* 0x008d061a */
    record_year = *(volatile u16 *)(record + 0x2f);            /* 0x008d0620 */
    area.d[0] = global;                                       /* 0x008d0624 */
    area.d[1] = leap;                                         /* 0x008d0628 */
    /* 0x008d062c mov dx,word ptr [esp+0x12] : the global year. */
    if (area.w[1] > record_year)                              /* 0x008d0634 ja */
        goto default_at_0;                                    /* 0x008d0645 */
    if (area.w[1] < record_year)                              /* 0x008d0636 jb */
        goto compare;                                         /* 0x008d0691 */
    record_day = *(volatile u16 *)(record + 0x2d);             /* 0x008d063a */
    if ((s16)(u16)global <= (s16)(u16)record_day)             /* 0x008d063e setg */
        goto compare;                                         /* 0x008d0643 je */
    goto default_at_0;                                        /* 0x008d0645 */

compare:
    /* 0x008d0691-0x008d06aa: the working year against the global year. */
    this_year = *(volatile u16 *)cached_a7;                    /* 0x008d0691 */
    limit = (u32)G_YEAR + 5u;                /* 0x008d0694 xor ecx,ecx ... 0x008d069f */
    if (this_year > limit)                                    /* 0x008d06aa jle */
        goto default_at_0;                                    /* 0x008d06ac */
    month_arg = limit & 0xffffff00u;             /* ECX at 0x008d06c6 */
    for (;;) {
        /* 0x008d06c6-0x008d06d1: construct the date in the frame area at
           +8. The pushed dwords are the day of month, the month byte, the
           year word and the 0xffffffff weekday target. */
        month = *(volatile u8 *)cached_a6;                    /* 0x008d06c6 */
        built = cm_fn_00549f70(&area.d[2], day_word,
                               month_arg | (u32)month,
                                this_year, 0xffffffffu);
        built0 = *(volatile u32 *)built;                      /* 0x008d06d6 */
        built1 = *(volatile u32 *)((u32)(unsigned long)built + 4u); /* 0x008d06d8 */
        record_year = *(volatile u16 *)(record + 0x2f);       /* 0x008d06db */
        area.d[0] = built0;                                   /* 0x008d06df */
        area.d[1] = built1;                                   /* 0x008d06e3 */
        /* 0x008d06e7 mov dx,word ptr [esp+0x12] : the built year. */
        if (area.w[1] < record_year)                          /* 0x008d06f1 jb */
            return ((u32)(unsigned long)built & 0xffff0000u)
                   | record_year;                            /* 0x008d06db/076f */
        if (area.w[1] == record_year) {
            record_day = *(volatile u16 *)(record + 0x2d);    /* 0x008d06f5 */
            if (!((s16)(u16)built0 > (s16)(u16)record_day))   /* 0x008d06f9 setg */
                return 0;                                     /* 0x008d076f */
        }
        /* 0x008d0700-0x008d070b: step the month back; a borrow takes the
           year down and wraps the month to 11. */
        month = (u8)(*(volatile u8 *)cached_a6 - 1u);          /* 0x008d0700/2 */
        *(volatile u8 *)cached_a6 = month;                    /* 0x008d0704 */
        if ((s8)month < 0) {                                  /* 0x008d0706 jns */
            *(volatile u16 *)cached_a7 = (u16)(*(volatile u16 *)cached_a7 - 1u); /* 0x008d0708 */
            *(volatile u8 *)cached_a6 = 0x0b;                 /* 0x008d070b */
        }
        /* 0x008d070e-0x008d0736: keep the earlier (year, month) pair in
           (a5, a4); the year compare is unsigned, the month compare signed. */
        year_destination = a5;                               /* 0x008d070e */
        this_year = *(volatile u16 *)cached_a7;               /* 0x008d0712 */
        other_year = *(volatile u16 *)year_destination;       /* 0x008d0715 */
        if (other_year == this_year) {
            month_destination = a4;                          /* 0x008d071f */
            other_month = *(volatile u8 *)month_destination;
            this_month = *(volatile u8 *)cached_a6;
            if ((s8)other_month <= (s8)this_month)
                goto check_year;
        } else if (other_year < this_year) {
            goto check_year;
        }
        month_destination = a4;                              /* 0x008d072b */
        this_month = *(volatile u8 *)cached_a6;
        *(volatile u8 *)month_destination = this_month;        /* 0x008d0731 */
        this_year = *(volatile u16 *)cached_a7;
        *(volatile u16 *)year_destination = (u16)this_year;    /* 0x008d0736 */
check_year:
        /* 0x008d0739-0x008d0743 */
        this_year = *(volatile u16 *)cached_a7;               /* 0x008d0739 */
        limit_year = G_YEAR;                                  /* 0x008d073c */
        if (this_year >= limit_year)                          /* 0x008d0743 jae */
            continue;                                         /* 0x008d06c6 */
        /* 0x008d0745-0x008d0752: the default year; EAX keeps the pointer. */
        def = cm_fn_0054a110(&area.d[2]);
        *(volatile u16 *)cached_a7 = *(volatile u16 *)((u32)(unsigned long)def + 2u);
        return (u32)(unsigned long)def;                       /* 0x008d075c */
    }

default_at_0:
    /* 0x008d0645 / 0x008d06ac: the default date's year. EAX keeps the
       default pointer with AX replaced by the year (mov ax,[eax+2]). */
    def = cm_fn_0054a110(&area.d[0]);
    def_ptr = (u32)(unsigned long)def;
    year = *(volatile u16 *)(def_ptr + 2u);                    /* 0x008d064e */
    *(volatile u16 *)cached_a7 = year;                        /* 0x008d0652 */
    return (def_ptr & 0xffff0000u) | (u32)year;               /* 0x008d065c */

default_at_8:
    /* 0x008d075f: the default date's year; EAX keeps the pointer. */
    def = cm_fn_0054a110(&area.d[2]);
    *(volatile u16 *)cached_a7 = *(volatile u16 *)((u32)(unsigned long)def + 2u);
    return (u32)(unsigned long)def;                           /* 0x008d0776 */
}
