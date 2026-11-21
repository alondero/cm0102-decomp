/* fn_0056a560 - preferred-image VA 0x0056a560, image base 0x00400000.
   Pinned CM0102 3.9.68 executable SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   Entry: three stack arguments (arg1 = [esp+4], arg2 = [esp+8],
   arg3 = [esp+0xc]); `ret 0xc` pops the three argument slots, so the
   entry convention is callee-cleanup (stdcall).

   Frame (ESP0 = entry esp, F = ESP0 - 0x1a0 = frame bottom):
     F+0x00  u32  branch A: r3 (result of the index-3 lookup)
                  branch B: r2 (result of the index-2 lookup)
     F+0x04  u32  branch A: r2   branch B: r3
     F+0x08  u32  branch A: r6 (index-6 lookup)
     F+0x0c  u32  branch A: r1 (index-1 lookup)
     F+0x10  u8[0x190]  message buffer written by cm_fn_0053e140
   The four words and the buffer live in one scratch allocation so the
   original addresses keep their relative offsets.

   Lookup helper cm_fn_00785830 is called with the receiver in ECX
   (arg1) and the index on the stack; it cleans its own stack argument
   (frame offsets above were derived from that clean-up). */

#include "cmre.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

#define DAT_00ae235c CM_GLOBAL(volatile u32, 0x00ae235c)
#define DAT_00ae23b4 CM_GLOBAL(int, 0x00ae23b4)
#define DAT_00ae23bc CM_GLOBAL(int, 0x00ae23bc)
#define DAT_00de1f64 CM_GLOBAL(u32, 0x00de1f64)

/* address of the global word pushed as the first message argument */
#define ADDR_00de1f64 ((u32)(unsigned long)&DAT_00de1f64)

#define STR_009c3884 ((char *)0x009c3884)
#define STR_009c38c8 ((char *)0x009c38c8)
#define STR_009c3924 ((char *)0x009c3924)
#define STR_009c3988 ((char *)0x009c3988)
#define STR_009c39e0 ((char *)0x009c39e0)
#define STR_009c3b18 ((char *)0x009c3b18)
#define STR_009c3c5c ((char *)0x009c3c5c)
#define STR_009c3d84 ((char *)0x009c3d84)
#define STR_009c3ec4 ((char *)0x009c3ec4)
#define STR_009c3fa8 ((char *)0x009c3fa8)
#define STR_009c40a0 ((char *)0x009c40a0)

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, const char *name, u32 key_a, u32 key_b);
extern u32 CM_CDECL cm_fn_0053e140(u32 first, u32 second, u32 third, u32 output, u32 size, u32 kind, u32 person);
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third, u32 date);
/* The original pushes 7, 6 or 3 arguments at the three call sites and
   removes them itself, so the tail is variadic. */
extern char * CM_CDECL cm_fn_0066f4e0(int param_1, char *param_2, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);

typedef struct {
    u32 slot_00;
    u32 slot_04;
    u32 slot_08;
    u32 slot_0c;
    u8  buf[0x190];
} cm_frame_0056a560;

void CM_STDCALL cm_fn_0056a560(u32 param_1, u32 param_2, u32 param_3)
{
    cm_frame_0056a560 fr;
    u32 r0, r1, r2, r3, r4, r6, r9;
    u32 rec, base, str_addr;
    int v;

    if (param_2 != 0) {
        /* ---- 0x0056a578 .. 0x0056a5e8: ten index lookups ---- */
        r0 = (u32)cm_fn_00785830((void *)param_1, 0);
        r1 = (u32)cm_fn_00785830((void *)param_1, 1);
        fr.slot_0c = r1;                                  /* 0x0056a597 */
        r2 = (u32)cm_fn_00785830((void *)param_1, 2);
        fr.slot_04 = r2;                                  /* 0x0056a5a4 */
        r3 = (u32)cm_fn_00785830((void *)param_1, 3);
        fr.slot_00 = r3;                                  /* 0x0056a5b1 */
        r4 = (u32)cm_fn_00785830((void *)param_1, 4);
        (void)cm_fn_00785830((void *)param_1, 5);
        r6 = (u32)cm_fn_00785830((void *)param_1, 6);
        fr.slot_08 = r6;                                  /* 0x0056a5d2 */
        (void)cm_fn_00785830((void *)param_1, 7);
        (void)cm_fn_00785830((void *)param_1, 8);
        r9 = (u32)cm_fn_00785830((void *)param_1, 9);

        /* 0x0056a5ed..0x0056a607: kind 1 */
        (void)cm_fn_0053e140(r1, fr.slot_04, fr.slot_00,
                             (u32)(unsigned long)fr.buf, 0x190, 1, r0);

        /* ---- 0x0056a611..0x0056a790: switch on r9 ---- */
        if (r9 == 0) {
            /* 0x0056a6d3: second switch on r6 */
            u32 r6v = fr.slot_08;
            if (r6v == 1) {
                /* 0x0056a75f */
                base = (u32)DAT_00ae23b4;
                rec = r4 * 581u;
                v = *(s8 *)(base + rec + 0x52);
                str_addr = base + rec + 0x38;
                (void)cm_fn_0066f4e0((int)ADDR_00de1f64, STR_009c39e0,
                                     (u32)v, (u32)v,
                                     (u32)(unsigned long)fr.buf, str_addr);
            } else if (r6v == 2) {
                /* 0x0056a736 */
                base = (u32)DAT_00ae23b4;
                rec = r4 * 581u;
                v = *(s8 *)(base + rec + 0x52);
                str_addr = base + rec + 0x38;
                (void)cm_fn_0066f4e0((int)ADDR_00de1f64, STR_009c3b18,
                                     (u32)v, (u32)v,
                                     (u32)(unsigned long)fr.buf, str_addr);
            } else if (r6v == 3) {
                /* 0x0056a70d */
                base = (u32)DAT_00ae23b4;
                rec = r4 * 581u;
                v = *(s8 *)(base + rec + 0x52);
                str_addr = base + rec + 0x38;
                (void)cm_fn_0066f4e0((int)ADDR_00de1f64, STR_009c3c5c,
                                     (u32)v, (u32)v,
                                     (u32)(unsigned long)fr.buf, str_addr);
            } else {
                /* 0x0056a6e4 */
                base = (u32)DAT_00ae23b4;
                rec = r4 * 581u;
                v = *(s8 *)(base + rec + 0x52);
                str_addr = base + rec + 0x38;
                (void)cm_fn_0066f4e0((int)ADDR_00de1f64, STR_009c3d84,
                                     (u32)v, (u32)v,
                                     (u32)(unsigned long)fr.buf, str_addr);
            }
        } else if (r9 == 1) {
            /* 0x0056a698: seven arguments, string then buffer */
            base = (u32)DAT_00ae23b4;
            rec = r4 * 581u;
            v = *(s8 *)(base + rec + 0x52);
            str_addr = base + rec + 0x38;
            (void)cm_fn_0066f4e0((int)ADDR_00de1f64, STR_009c3ec4,
                                 (u32)v, (u32)v, 100u, str_addr,
                                 (u32)(unsigned long)fr.buf);
        } else if (r9 == 3) {
            /* 0x0056a65d: seven arguments; the push order here is
               string, buffer, value, value, 100, format, handle, so 100
               lands in the third argument slot and the buffer precedes
               the string. */
            base = (u32)DAT_00ae23b4;
            rec = r4 * 581u;
            v = *(s8 *)(base + rec + 0x52);
            str_addr = base + rec + 0x38;
            (void)cm_fn_0066f4e0((int)ADDR_00de1f64, STR_009c3fa8,
                                 100u, (u32)v, (u32)v,
                                 (u32)(unsigned long)fr.buf, str_addr);
        } else {
            /* 0x0056a622: seven arguments, string then buffer */
            base = (u32)DAT_00ae23b4;
            rec = r4 * 581u;
            v = *(s8 *)(base + rec + 0x52);
            str_addr = base + rec + 0x38;
            (void)cm_fn_0066f4e0((int)ADDR_00de1f64, STR_009c40a0,
                                 (u32)v, (u32)v, 100u, str_addr,
                                 (u32)(unsigned long)fr.buf);
        }

        /* ---- 0x0056a793 ---- */
        cm_fn_004ae660((void *)param_3, ADDR_00de1f64);

        /* ---- 0x0056a7a6..0x0056a7ec ---- */
        {
            u32 idx = r0 * 110u;                       /* 0x0056a7c5 */
            u32 q = (u32)DAT_00ae23bc;                 /* 0x0056a7b7 */
            u32 res = cm_fn_0053e990(q + idx, fr.slot_0c, fr.slot_04,
                                     fr.slot_00, 0);
            if (res != 0) {
                u32 key = idx + (u32)DAT_00ae23bc;     /* 0x0056a7d8 */
                cm_fn_004ae8a0(param_3, (const char *)fr.buf, 0x7da, key);
            }
        }

        /* ---- 0x0056a7f1..0x0056a81c ---- */
        if ((s32)r4 > -1) {
            u32 limit = (u32)DAT_00ae235c - 1u;        /* 0x0056a7fa */
            if ((s32)r4 < (s32)limit) {
                u32 p = (u32)DAT_00ae23b4 + rec;       /* 0x0056a809 */
                cm_fn_004ae8a0(param_3, (const char *)(p + 4), 0x7d5, p);
            }
        }
    } else {
        /* ---- 0x0056a82e .. 0x0056a88d: nine index lookups ---- */
        r0 = (u32)cm_fn_00785830((void *)param_1, 0);
        r1 = (u32)cm_fn_00785830((void *)param_1, 1);
        r2 = (u32)cm_fn_00785830((void *)param_1, 2);
        fr.slot_00 = r2;                               /* 0x0056a856 */
        r3 = (u32)cm_fn_00785830((void *)param_1, 3);
        fr.slot_04 = r3;                               /* 0x0056a863 */
        r4 = (u32)cm_fn_00785830((void *)param_1, 4);
        (void)cm_fn_00785830((void *)param_1, 5);
        (void)cm_fn_00785830((void *)param_1, 6);
        (void)cm_fn_00785830((void *)param_1, 6);
        r9 = (u32)cm_fn_00785830((void *)param_1, 9);

        /* 0x0056a892..0x0056a8ac: kind 5 */
        (void)cm_fn_0053e140(r1, fr.slot_00, fr.slot_04,
                             (u32)(unsigned long)fr.buf, 0x190, 5, r0);

        /* ---- 0x0056a8b6..0x0056a980: switch on r9 ---- */
        if (r9 == 0) {
            /* 0x0056a96c: three arguments */
            (void)cm_fn_0066f4e0((int)ADDR_00de1f64, STR_009c3884,
                                 (u32)(unsigned long)fr.buf);
        } else if (r9 == 1) {
            /* 0x0056a935 */
            base = (u32)DAT_00ae23b4;
            rec = r4 * 581u;
            v = *(s8 *)(base + rec + 0x52);
            str_addr = base + rec + 0x38;
            (void)cm_fn_0066f4e0((int)ADDR_00de1f64, STR_009c38c8,
                                 (u32)v, (u32)v,
                                 (u32)(unsigned long)fr.buf, str_addr);
        } else if (r9 == 3) {
            /* 0x0056a901 */
            base = (u32)DAT_00ae23b4;
            rec = r4 * 581u;
            v = *(s8 *)(base + rec + 0x52);
            str_addr = base + rec + 0x38;
            (void)cm_fn_0066f4e0((int)ADDR_00de1f64, STR_009c3924,
                                 (u32)v, (u32)v,
                                 (u32)(unsigned long)fr.buf, str_addr);
        } else {
            /* 0x0056a8ca */
            base = (u32)DAT_00ae23b4;
            rec = r4 * 581u;
            v = *(s8 *)(base + rec + 0x52);
            str_addr = base + rec + 0x38;
            (void)cm_fn_0066f4e0((int)ADDR_00de1f64, STR_009c3988,
                                 (u32)v, (u32)v,
                                 (u32)(unsigned long)fr.buf, str_addr);
        }

        /* ---- 0x0056a983 ---- */
        cm_fn_004ae660((void *)param_3, ADDR_00de1f64);

        /* ---- 0x0056a994..0x0056a9dc ---- */
        {
            u32 idx = r0 * 110u;                       /* 0x0056a9ad */
            u32 q = (u32)DAT_00ae23bc;                 /* 0x0056a9a0 */
            u32 res = cm_fn_0053e990(q + idx, r1, fr.slot_00,
                                     fr.slot_04, 0);
            if (res != 0) {
                u32 key = idx + (u32)DAT_00ae23bc;     /* 0x0056a9c2 */
                cm_fn_004ae8a0(param_3, (const char *)fr.buf, 0x7da, key);
            }
        }

        /* ---- 0x0056a9e1..0x0056aa14 ---- */
        if ((s32)r4 > -1) {
            u32 limit = (u32)DAT_00ae235c - 1u;        /* 0x0056a9e6 */
            if ((s32)r4 < (s32)limit) {
                u32 p = (u32)DAT_00ae23b4 + r4 * 581u; /* 0x0056a9f0 */
                cm_fn_004ae8a0(param_3, (const char *)(p + 4), 0x7d5, p);
            }
        }
    }
}
