/* fn_006421c0 - preferred-image VA 0x006421c0 (image base 0x00400000).
 *
 * Binary: CM0102 3.9.68, cm0102.exe,
 *   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * Source of truth: original instructions 0x006421c0 .. 0x0064240a.
 *
 * Entry: ECX is read before written (0x006421cd "mov ebp, ecx") and the value
 * is kept in EBP and reloaded into ECX for the calls at 0x0064230c and
 * 0x00642367, so the routine is __thiscall; the original pops no stack
 * argument ("ret" with no immediate at 0x0064240a).
 */
#include "cmre.h"

/* Globals at their preferred-image VAs (widths and signedness from the
 * cmre brief: 0x009cf584/0x009cf588/0x00b67a34 are undefined4, the rest are
 * int, 0x00ae24d0 is a single byte that the original uses as a buffer
 * address). */
#define DAT_009cf360 CM_GLOBAL(volatile int, 0x009cf360)
#define DAT_009cf580 CM_GLOBAL(volatile int, 0x009cf580)
#define DAT_009cf584 CM_GLOBAL(volatile u32, 0x009cf584)
#define DAT_009cf588 CM_GLOBAL(volatile u32, 0x009cf588)
#define DAT_009cf58c CM_GLOBAL(volatile int, 0x009cf58c)
#define DAT_00adadfc CM_GLOBAL(volatile int, 0x00adadfc)
#define DAT_00ae235c CM_GLOBAL(volatile int, 0x00ae235c)
#define DAT_00ae23b4 CM_GLOBAL(volatile int, 0x00ae23b4)
#define DAT_00ae23d0 CM_GLOBAL(volatile int, 0x00ae23d0)
#define DAT_00ae24d0 CM_GLOBAL(volatile u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(volatile u32, 0x00b67a34)

/* String globals: 0x009870e0, 0x009870e8 and 0x009ea2a8. */
#define STR_009870e0 ((const char *)0x009870e0)
#define STR_009870e8 ((const char *)0x009870e8)
#define STR_009ea2a8 ((const char *)0x009ea2a8)

/* Callees stay explicit boundaries.
 *
 * cm_fn_004afbd0: the original call site is
 *   0x006422da "push 9"
 *   0x006422dc "push esi"
 *   0x006422dd "push ebx"
 *   0x006422de "call 0x4afbd0"
 * so the callee reads its first three stack arguments as the array base
 * (EBX), the element count (ESI) and the wanted count (9).  EAX and EDX are
 * still live at that call (EAX = the record array end, EDX = the index of the
 * last collected record) and the recovered callee takes them as two extra
 * trailing parameters, so they are passed after the three stack arguments.
 * The cmre brief renders this callee with the two entry registers first,
 * which contradicts the push order above.
 */
extern u32 CM_CDECL cm_fn_004afbd0(u32 base, u32 count, u32 wanted, u32 entry_eax, u32 entry_edx);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_006826d0(void *self, u32 arg);
extern u32 CM_STDCALL cm_fn_006830b0(u32 arg1, u32 arg2, u32 arg3);
extern void CM_STDCALL cm_fn_006831a0(u8 *self, u32 *param_2, u32 param_3);
extern char *cm_fn_0090d130(...);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

void CM_THISCALL cm_fn_006421c0(void *self)
{
    /* Original frame: "sub esp, 0x214" then four saved registers, so the
     * frame base L is 0x10 below the current stack pointer.  The two scratch
     * buffers live at L+0x14 and L+0x114, i.e. 0x100 bytes apart inside one
     * allocation; the original leaves them uninitialised. */
    u8 scratch[0x200];
    u8 *const buf_lo = scratch;          /* original L+0x014 */
    u8 *const buf_hi = scratch + 0x100;  /* original L+0x114 */

    u32 arr_base;      /* eax at 0x006421c6 */
    u32 team_a;        /* stack slot L+0x00 */
    u32 team_b;        /* stack slot L+0x10 */
    u32 team_c;        /* stack slot L+0x0c */
    u32 n_teams;       /* eax at 0x006421f8 */
    u32 bufp;          /* ebx, stack slot L+0x08 */
    u32 *buf;
    u32 count;         /* esi */
    u32 idx;           /* edi */
    u32 rec;           /* eax, the team record being scanned */
    u32 edx_val;       /* edx at the 0x006422de call */
    u32 i;
    u32 limit;

    /* The original keeps the entry ECX in EBP and hands it to 0x6830b0 and
     * 0x6831a0 as ECX; neither callee takes a receiver in its recovered C
     * signature, so the value has no observable effect here. */
    (void)self;

    arr_base = (u32)DAT_00adadfc;                                   /* 0x006421c6 */
    team_a = *(volatile u32 *)(arr_base + (u32)DAT_009cf580 * 4);    /* 0x006421d9 */
    team_b = *(volatile u32 *)(arr_base + (u32)DAT_009cf584 * 4);    /* 0x006421e8 */
    team_c = *(volatile u32 *)(arr_base + (u32)DAT_009cf588 * 4);    /* 0x006421f5 */
    n_teams = (u32)DAT_00ae235c;                                    /* 0x006421f8 */

    bufp = (u32)cm_fn_00944e46(n_teams * 4);                         /* 0x00642209 */
    if (bufp == 0) {                                                /* 0x00642213 */
        /* 0x0064221b..0x0064222f: five arguments, last two are the buffers
         * at L+0x114 and L+0x14 in that order. */
        cm_fn_00944cff((u32)STR_009ea2a8, 0, 0, buf_hi, buf_lo);
        {
            /* 0x00642237..0x00642249: cdecl, three arguments.  The three
             * values pushed for 0x90d130 stay on the stack (the original
             * clears them only with the final "add esp, 0x24"), so they are
             * the extra arguments the format at 0x009870e8 consumes. */
            char *msg = cm_fn_0090d130(buf_hi, buf_lo, 0x46b);
            /* 0x0064224e..0x00642259: the format "v%s %s.%s %d" consumes the
             * pushed message plus the three values left by 0x90d130. */
            cm_fn_00944c9f((u32)&DAT_00ae24d0, (u32)STR_009870e8, (u32)msg,
                           (u32)buf_hi, (u32)buf_lo, 0x46b);
            /* 0x0064225e..0x00642269 */
            cm_fn_005e8290(STR_009870e0, (const char *)&DAT_00ae24d0, 0);
        }
        DAT_00b67a34 = 0;                                            /* 0x00642271 */
        return;
    }
    buf = (u32 *)bufp;

    /* 0x00642282..0x006422ce: scan the team records, stride 0x245. */
    count = 0;
    idx = 0;
    n_teams = (u32)DAT_00ae235c;
    rec = (u32)DAT_00ae23b4;
    edx_val = 0;
    if ((s32)n_teams > 0) {                                          /* 0x0064228d */
        do {
            u32 p57 = *(volatile u32 *)(rec + 0x57);                 /* 0x00642295 */
            u32 p53;
            edx_val = p57;
            if (p57 != 0) {                                         /* 0x00642298 */
                p53 = *(volatile u32 *)(rec + 0x53);                 /* 0x0064229c */
                if (p53 != 0) {                                     /* 0x0064229f */
                    edx_val = *(volatile u32 *)p57;                   /* 0x006422a3 */
                    if (edx_val == (u32)DAT_009cf58c) {              /* 0x006422a5 */
                        u32 v53 = *(volatile u32 *)p53;               /* 0x006422ad */
                        edx_val = (u32)DAT_009cf360;                  /* 0x006422af */
                        if (v53 == edx_val) {                         /* 0x006422b5 */
                            u32 slot = (u32)(s16)count;              /* 0x006422b9 */
                            edx_val = slot;
                            count++;                                 /* 0x006422bc */
                            buf[slot] = rec;                         /* 0x006422bd */
                        }
                    }
                }
            }
            n_teams = (u32)DAT_00ae235c;                             /* 0x006422c0 */
            idx++;                                                  /* 0x006422c6 */
            rec += 0x245;                                            /* 0x006422c7 */
        } while ((s32)idx < (s32)n_teams);                            /* 0x006422cc */
    }

    if ((s32)(s16)count >= 9) {                                     /* 0x006422d0 */
        /* 0x006422da..0x006422de: base, count, wanted; EAX and EDX still
         * hold the values the original had on entry to this call. */
        cm_fn_004afbd0((u32)buf, count, 9, rec, edx_val);
        /* 0x006422e6..0x00642315: nine entries of the sorted array. */
        for (i = 0; i < 9; i++) {
            u32 a = (u32)DAT_009cf580;                              /* 0x006422ed */
            u32 val = ((volatile u32 *)buf)[i];                      /* 0x006422f2 */
            u32 addr = a * 107u + (u32)DAT_00ae23d0;                /* 0x006422f6 */
            cm_fn_006830b0(val, addr, 1);                            /* 0x0064230c */
        }

        /* 0x00642317..0x00642327: still inside the "si >= 9" branch, the
         * "jl 0x006423f7" at 0x006422d4 skips this block as well. */
        limit = (u32)(s16)(*(volatile u16 *)(team_a + 0x3e));
        if ((s32)limit > 0) {
            for (i = 0; i < limit; i++) {                            /* 0x006423ea */
                u8 *p;
                /* 0x0064232f..0x00642367: team_a */
                p = (u8 *)cm_fn_006826d0((void *)team_a, i);         /* 0x00642336 */
                if (p[0x37] == 3) {                                 /* 0x0064233b */
                    u32 b = (u32)DAT_009cf58c;                      /* 0x00642340 */
                    u32 addr = b * 107u + (u32)DAT_00ae23d0;        /* 0x00642347 */
                    u32 *q = (u32 *)cm_fn_006826d0((void *)team_a, i);   /* 0x0064235d */
                    cm_fn_006831a0((u8 *)(*q), (u32 *)addr, 1);      /* 0x00642367 */
                }
                /* 0x0064236c..0x006423a4: team_b */
                p = (u8 *)cm_fn_006826d0((void *)team_b, i);         /* 0x00642373 */
                if (p[0x37] == 3) {                                 /* 0x00642378 */
                    u32 b = (u32)DAT_009cf58c;                      /* 0x0064237d */
                    u32 addr = b * 107u + (u32)DAT_00ae23d0;        /* 0x00642384 */
                    u32 *q = (u32 *)cm_fn_006826d0((void *)team_b, i);   /* 0x0064239a */
                    cm_fn_006831a0((u8 *)(*q), (u32 *)addr, 1);      /* 0x006423a4 */
                }
                /* 0x006423a9..0x006423e1: team_c */
                p = (u8 *)cm_fn_006826d0((void *)team_c, i);         /* 0x006423b0 */
                if (p[0x37] == 3) {                                 /* 0x006423b5 */
                    u32 b = (u32)DAT_009cf58c;                      /* 0x006423ba */
                    u32 addr = b * 107u + (u32)DAT_00ae23d0;        /* 0x006423c1 */
                    u32 *q = (u32 *)cm_fn_006826d0((void *)team_c, i);   /* 0x006423d7 */
                    cm_fn_006831a0((u8 *)(*q), (u32 *)addr, 1);      /* 0x006423e1 */
                }
            }
        }
    }

    /* 0x006423f3..0x006423fd */
    cm_fn_009452ca(bufp);
}
