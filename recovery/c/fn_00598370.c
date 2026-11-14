/* fn_00598370 -- faithful C translation of the CM 3.9.68 routine at preferred
   image VA 0x00598370 (image base 0x00400000).  Every statement was checked
   against the instruction listing printed by `cmre brief fn_00598370`.

   Structure of the original:
     - "this" arrives in ECX (no stack arguments: `ret 0`).
     - zeroes [this+0x10] and [this+0xc], then walks a 581-byte record
       (DAT_00ae23b4 + index*0x245, index = [this+8]).
     - 50 entries at rec+0xd7 are resolved through cm_fn_004cf5f0 (people) and
       cm_fn_004cf6d0 (clubs); 5 entries at rec+0x19f, 7 at rec+0x1b3 and 3 at
       rec+0x1cf are resolved through cm_fn_004cf5a0.
     - [this+0x10] keeps the largest value seen, [this+0xc] the running sum.
     - a bonus term is added to [this+0xc] and the sum is finally recomputed
       with an x87 expression; the result is stored back into [this+0xc].
     - cm_fn_005968d0(this, 0, -1) result is stored into [this+0x14]. */

#include "cmre.h"

/* Original globals (preferred-image VAs, 4 bytes each). */
#define G_TABLE_BASE CM_GLOBAL(u32, 0x00ae23b4) /* record table base   */
#define G_CLUB       CM_GLOBAL(u32, 0x00ae1050) /* receiver of lookups */
#define G_ID_A       CM_GLOBAL(u32, 0x009cf2e4)
#define G_ID_B       CM_GLOBAL(u32, 0x009cf360)

/* Original x87 constants (64-bit doubles stored in the image). */
#define K_968968     CM_GLOBAL(double, 0x00968968)
#define K_96a620     CM_GLOBAL(double, 0x0096a620)
#define K_9689e0     CM_GLOBAL(double, 0x009689e0)
#define K_96a350     CM_GLOBAL(double, 0x0096a350)
#define K_96a618     CM_GLOBAL(double, 0x0096a618)
#define K_96a610     CM_GLOBAL(double, 0x0096a610)
#define K_96a608     CM_GLOBAL(double, 0x0096a608)
#define K_96a600     CM_GLOBAL(double, 0x0096a600)
#define K_96a5c0     CM_GLOBAL(double, 0x0096a5c0)

extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, volatile u32 person, u32 club);
extern u32 CM_THISCALL cm_fn_004cf6d0(u32 self, u32 argument);
extern u32 CM_THISCALL cm_fn_005968d0(u32 self, u32 person, u32 flags);
/* The original callee at 0x00864550 has no `this` receiver: its first
   instruction (00864550) overwrites ECX with [0xae1050] before reading
   anything.  It takes six callee-clean stack arguments (it ends with
   `ret 0x18`, six dwords) and it returns its result on the x87 stack:
   the exit path (008665a9 fld dword ptr [esp+0x4c] / 008665b4 ret 0x18)
   leaves the value in ST0 and never sets EAX to it.  The caller converts
   that ST0 value with the MSVC __ftol helper at 0x00945640 and uses the
   low half of EDX:EAX (005983f8 mov ebp, eax).  Declare the real ABI:
   stdcall, six stack arguments, long double (ST0) result.  This matches
   recovery/c/fn_00844780.c, so both translation units agree. */
extern long double CM_STDCALL cm_fn_00864550(u32 p, u32 d, u32 zero,
                                              u32 minus1, u32 mode, u32 zero2);

u32 CM_FASTCALL cm_fn_00598370(u32 self)
{
    u32 idx;
    u32 rec;
    u32 i;
    u8 counter; /* byte counter at the original stack slot L+3 */

    *(volatile u32 *)(self + 0x10) = 0;
    *(volatile u32 *)(self + 0xc) = 0;

    counter = 0;
    idx = *(volatile u32 *)(self + 8);
    /* lea ecx,[eax+eax*8]; shl ecx,4; add ecx,eax; add edx,eax; lea ebp,[edx+ecx*4] */
    rec = G_TABLE_BASE + idx + idx * 580u;

    /* ---- 50 entries at rec+0xd7 -------------------------------------- */
    for (i = 0; i < 50u; i++) {
        u32 val = *(volatile u32 *)(rec + 0xd7u + 4u * i);
        u32 person;
        u32 club;
        s32 v;

        if (val == 0)
            continue;

        counter = (u8)(counter + 1);

        person = cm_fn_004cf5f0(G_CLUB, val, rec);
        if (person == 0)
            continue;

        v = *(volatile s32 *)(person + 0xc);
        if (v == -1) {
            /* 005983ee call 0x864550 leaves its result in ST0 (it never
               sets EAX); 005983f3 call 0x945640 is the MSVC __ftol of that
               ST0 value into EDX:EAX; 005983f8 mov ebp, eax keeps the low
               half.  The callee takes val, rec, 0, -1, 0xb, 0 as stack
               arguments; the ECX = person written at 005983ec is dead (the
               callee overwrites ECX on entry).  The __ftol conversion is
               inlined here exactly as at the other four __ftol sites below
               (00598564, 0059857e, 0059868a). */
            long double r = cm_fn_00864550(val, rec, 0, 0xffffffffu, 0xb, 0);
            v = (s32)(long long)r;
        }

        club = cm_fn_004cf6d0(G_CLUB, val);
        if (club != 0) {
            u8 kind = *(volatile u8 *)(person + 0x35);
            if ((kind & 0x3f) != 6) {
                u32 club2 = cm_fn_004cf6d0(G_CLUB, val);
                v = (s32)((u32)v - *(volatile u32 *)(club2 + 0xc));
            }
        }

        if (v > *(volatile s32 *)(self + 0x10))
            *(volatile u32 *)(self + 0x10) = (u32)v;
        *(volatile u32 *)(self + 0xc) =
            *(volatile u32 *)(self + 0xc) + (u32)v;
    }

    /* ---- 5 entries at rec+0x19f -------------------------------------- */
    for (i = 0; i < 5u; i++) {
        u32 val = *(volatile u32 *)(rec + 0x19fu + 4u * i);
        u32 obj;
        s32 c;

        if (val == 0)
            continue;
        obj = cm_fn_004cf5a0(G_CLUB, val);
        if (obj == 0)
            continue;

        c = *(volatile s32 *)(obj + 0xc);
        if (c > *(volatile s32 *)(self + 0x10))
            *(volatile u32 *)(self + 0x10) = (u32)c;
        *(volatile u32 *)(self + 0xc) =
            *(volatile u32 *)(self + 0xc) +
            (u32)*(volatile u32 *)(obj + 0xc);
    }

    /* ---- 7 entries at rec+0x1b3 -------------------------------------- */
    for (i = 0; i < 7u; i++) {
        u32 val = *(volatile u32 *)(rec + 0x1b3u + 4u * i);
        u32 obj;
        s32 c;

        if (val == 0)
            continue;
        obj = cm_fn_004cf5a0(G_CLUB, val);
        if (obj == 0)
            continue;

        c = *(volatile s32 *)(obj + 0xc);
        if (c > *(volatile s32 *)(self + 0x10))
            *(volatile u32 *)(self + 0x10) = (u32)c;
        *(volatile u32 *)(self + 0xc) =
            *(volatile u32 *)(self + 0xc) +
            (u32)*(volatile u32 *)(obj + 0xc);
    }

    /* ---- 3 entries at rec+0x1cf -------------------------------------- */
    for (i = 0; i < 3u; i++) {
        u32 val = *(volatile u32 *)(rec + 0x1cfu + 4u * i);
        u32 obj;
        s32 c;

        if (val == 0)
            continue;
        obj = cm_fn_004cf5a0(G_CLUB, val);
        if (obj == 0)
            continue;

        c = *(volatile s32 *)(obj + 0xc);
        if (c > *(volatile s32 *)(self + 0x10))
            *(volatile u32 *)(self + 0x10) = (u32)c;
        *(volatile u32 *)(self + 0xc) =
            *(volatile u32 *)(self + 0xc) +
            (u32)*(volatile u32 *)(obj + 0xc);
    }

    /* ---- bonus term, only while the counter is below 15 -------------- */
    if ((s8)counter < 15) {
        s32 sw = (s16)*(volatile u16 *)(rec + 0x80);
        s32 value;

        if (sw > 0x128e) {
            /* movsx eax,ax; sub eax,0x79e */
            value = sw - 0x79e;
        } else if (sw > 0x8ca) {
            /* fild (ax-0x8ca); fmul [0x968968]; fadd [0x96a620]; __ftol */
            long double t = (long double)(sw - 0x8ca);
            t = t * (long double)K_968968;
            t = t + (long double)K_96a620;
            value = (s32)(long long)t;
        } else {
            /* fild ax; fmul [0x9689e0]; __ftol */
            long double t = (long double)sw;
            t = t * (long double)K_9689e0;
            value = (s32)(long long)t;
        }

        if (*(volatile u8 *)(rec + 0x64) != 1) {
            /* fild value; fmul [0x968968]; __ftol */
            long double t = (long double)value;
            t = t * (long double)K_968968;
            value = (s32)(long long)t;
        }

        {
            u32 p = *(volatile u32 *)(rec + 0x53);
            if (p != 0) {
                /* mov cl,byte [ecx+0x7e]; cmp cl,bl; jge -> the compares are
                   signed byte compares: a byte with the top bit set is
                   negative, so the "cl < 1" path (mov cl,bl) is taken and the
                   idiv is skipped entirely. */
                s8 b = (s8)*(volatile u8 *)(p + 0x7e);

                if (b >= 1) {
                    s32 div = (b > 3) ? (s32)3 : (s32)b;

                    value = value / div; /* idiv: truncation toward zero */
                }
            }
        }

        {
            /* movsx edx,counter; mov ecx,0xf; sub ecx,edx; imul ecx,eax */
            u32 delta = (u32)(0xf - (s32)(s8)counter) * (u32)value;
            *(volatile u32 *)(self + 0xc) =
                *(volatile u32 *)(self + 0xc) + delta;
        }
    }

    /* ---- final recomputation of [this+0xc] ---------------------------- */
    {
        u8 f64 = *(volatile u8 *)(rec + 0x64);
        long double t;

        if (f64 == 1) {
            u32 p = *(volatile u32 *)(rec + 0x53);
            int special = 0;

            if (p != 0) {
                u32 id = *(volatile u32 *)p;
                if (id == G_ID_A)
                    special = 1;
                else if (id == G_ID_B)
                    special = 1;
            }

            if (special) {
                s32 sw = (s16)*(volatile u16 *)(rec + 0x80);
                s32 sq = (s32)((u32)sw * (u32)sw);
                if (sw > 0x1482)
                    t = (long double)sq * (long double)K_96a350;
                else if (sw > 0xcb2)
                    t = (long double)sq * (long double)K_96a618;
                else
                    t = (long double)sq * (long double)K_96a610;
            } else {
                s32 sw = (s16)*(volatile u16 *)(rec + 0x80);
                s32 sq = (s32)((u32)sw * (u32)sw);
                t = (long double)sq * (long double)K_96a608;
            }
        } else {
            s32 sw = (s16)*(volatile u16 *)(rec + 0x80);
            s32 sq = (s32)((u32)sw * (u32)sw);
            if (sw > 0xcb2)
                t = (long double)sq * (long double)K_96a600;
            else
                t = (long double)sq * (long double)K_96a5c0;
        }

        /* fiadd dword ptr [esi+0xc]; __ftol */
        t = t + (long double)*(volatile s32 *)(self + 0xc);
        *(volatile u32 *)(self + 0xc) = (u32)(long long)t;
    }

    /* ---- final call --------------------------------------------------- */
    {
        u32 r = cm_fn_005968d0(self, 0, 0xffffffffu);
        *(volatile u32 *)(self + 0x14) = r;
        return r;
    }
}
