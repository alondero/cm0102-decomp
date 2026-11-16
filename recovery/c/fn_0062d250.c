/* fn_0062d250 (preferred-image VA 0x0062d250, module injury.cpp)
   Plain C reconstruction of the 2626-byte original routine.
   Entry: ECX = this (the injury-list object); 8 stack arguments (ret 0x20). */

#include "cmre.h"

/* ------------------------------------------------------------------ */
/* original callees                                                    */
/* ------------------------------------------------------------------ */
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
extern long long CM_CDECL cm_fn_00544410(volatile u32 record, volatile u32 adjustment);
extern u32 CM_CDECL cm_fn_005444a0(u32 person, u32 input_club);
/* the original forwards this call's EAX to cm_fn_00417c10 as its third
   stack argument, so the result is observable at the boundary */
extern u32 * CM_THISCALL cm_fn_0054c8e0(short *param_1, u32 *param_2, int param_3);
extern void CM_THISCALL cm_fn_0058e340(u32 *param_1, int *param_2, int *param_3, u32 param_4);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag, u32 person, u32 force);
extern void CM_THISCALL cm_fn_0062c680(int *param_1, int *param_2);
extern u32 CM_STDCALL cm_fn_0062dca0(u32 source_arg, u32 details_arg, u32 value_arg);
extern void CM_THISCALL cm_fn_0062e500(u32 self, u32 first, u32 second);
extern void CM_THISCALL cm_fn_0062e820(u32 param_1, u32 param_2, char param_3, int param_4, u32 *param_5);
extern void CM_THISCALL cm_fn_00775cd0(int param_1, int *param_2, short param_3);
extern void CM_THISCALL cm_fn_007a7f20(u8 *self, u32 *param_2, s16 param_3);
extern unsigned long long CM_CDECL cm_fn_007e4750(void *first, void *second, void *third);
extern void CM_THISCALL cm_fn_00842b00(int param_1, int *param_2, char param_3);
extern void CM_THISCALL cm_fn_008b1750(int param_1, int *param_2, short param_3);
extern unsigned long long CM_CDECL cm_fn_0090cfc0(u32 entry_eax, u32 entry_edx, u32 bound);
extern char * CM_CDECL cm_fn_0090d130(void *first, void *second, u32 line);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_00945667(u32 arg8, u32 argC);

/* ------------------------------------------------------------------ */
/* original globals                                                     */
/* ------------------------------------------------------------------ */
#define DAT_00ae2364 CM_GLOBAL(int,    0x00ae2364)
#define DAT_00ae24d0 CM_GLOBAL(u8,     0x00ae24d0)
#define DAT_00ae2d0c CM_GLOBAL(u32,    0x00ae2d0c)
#define DAT_00b67a34 CM_GLOBAL(u32,    0x00b67a34)
#define DAT_00b74318 CM_GLOBAL(int,    0x00b74318)
#define DAT_00b7434c CM_GLOBAL(int,    0x00b7434c)
#define DAT_00de1f44 CM_GLOBAL(int,    0x00de1f44)
#define DAT_00de1f54 CM_GLOBAL(int,    0x00de1f54)
#define DAT_00de1f64 CM_GLOBAL(u32,    0x00de1f64)
#define DBL_00967898 CM_GLOBAL(double, 0x00967898)

/* 0x00ae2c90 is a global object; the routine passes its address */
#define AEC90_ADDR ((u32)0x00ae2c90)

/* the 13-byte injury table based at 0x009e7adb */
#define INJ_BASE 0x009e7adb
#define TBL_W0(i) (*(volatile s16 *)(INJ_BASE + (i)))
#define TBL_W2(i) (*(volatile s16 *)(INJ_BASE + 2 + (i)))
#define TBL_B4(i) (*(volatile u8  *)(INJ_BASE + 4 + (i)))
#define TBL_D5(i) (*(volatile u32 *)(INJ_BASE + 5 + (i)))

/* fields of the "this" object (esi) and of the injury record (edi) */
#define THIS_W(off) (*(volatile u32 *)((u8 *)param_1 + (off)))
#define P4_W(off)   (*(volatile u32 *)((u8 *)param_4 + (off)))
#define P4_B(off)   (*(volatile u8  *)((u8 *)param_4 + (off)))

/* 0x0090cfc0 takes the incoming EAX/EDX pair first and the range bound
   last; the original call sites put the operand in EAX/EDX and push the
   bound.  Only the bound is reconstructed here. */
#define RND(bound) ((u32)cm_fn_0090cfc0((u32)(bound), (u32)(bound), (u32)(bound)))

/* signed division by 10 / 20 as the original lowers it:
   mov eax,0x66666667 ; imul ecx ; sar edx,shift ; mov ecx,edx ;
   shr ecx,0x1f ; add edx,ecx                                          */
static s32 cmre_magic_div(s32 n, int shift)
{
    long long p = (long long)n * (long long)(s32)0x66666667;
    s32 hi = (s32)(u32)((unsigned long long)p >> 32);
    s32 q = hi >> shift;
    return q + (s32)((u32)q >> 31);
}

int CM_THISCALL
cm_fn_0062d250(int *param_1, u32 param_2, int param_3, int *param_4, int param_5,
                int param_6, u32 param_7, char param_8, int param_9)
{
    /* two 256 byte scratch buffers: bufA at E-0x200, bufB at E-0x100 */
    u8 scratch[0x220];
    u8 *bufA = scratch + 0x20;
    u8 *bufB = scratch + 0x120;

    u32 saved;       /* [esp+0x28] */
    u32 slot_index;  /* [esp+0x24] */
    u32 tmp_c;       /* [esp+0x1c] */
    u32 idx13;       /* [esp+0x18] */
    s32 total;       /* [esp+0x14] */
    u32 tmp10;       /* [esp+0x10], one rand result kept across a call */
    u8  age_b;       /* [esp+0x13] */
    u32 it;          /* (u8)param_2 : injury type */
    u8  sev;         /* (u8)param_8 : severity */
    char *msg;
    u32 err_x;       /* first buffer vararg left on the stack by 90d130 */
    u32 err_y;       /* second buffer vararg */
    u32 err_line;    /* source line left on the stack by 90d130 */
    u32 offset;      /* index * 0x30 */
    u32 index;
    u32 ebx;

    saved      = 0;
    slot_index = 0;
    tmp_c      = 0;
    idx13      = 0;
    total      = 0;
    tmp10      = 0;
    age_b      = 0;
    it         = param_2 & 0xffu;
    sev        = (u8)param_8;
    msg        = (char *)0;
    err_x      = 0;
    err_y      = 0;
    err_line   = 0;
    offset     = 0;
    index      = 0;
    ebx        = 0;

    if (param_4 == (int *)0) {
        cm_fn_00944cff(0x009e7f28u, 0u, 0u, (void *)bufB, (void *)bufA);
        msg = cm_fn_0090d130((void *)bufB, (void *)bufA, 0x889u);
        err_x    = (u32)bufB;
        err_y    = (u32)bufA;
        err_line = 0x889u;
        goto report_error;
    }

    if ((s32)P4_W(0) >= (s32)DAT_00ae2364) {
        return -1;
    }

    if (P4_W(0x61) == 0) {
        cm_fn_00944cff(0x009e7f28u, 0u, 0u, (void *)bufA, (void *)bufB);
        msg = cm_fn_0090d130((void *)bufA, (void *)bufB, 0x896u);
        err_x    = (u32)bufA;
        err_y    = (u32)bufB;
        err_line = 0x896u;
        goto report_error;
    }

    /* signed byte range check on the injury type */
    if ((s8)it < 0 || it >= 0x52u) {
        cm_fn_00944c9f(0x00de1f64u, 0x009e7fbcu, (u32)(s32)(s8)it);
        cm_fn_00944cff(0x009e7f28u, 0u, 0u, (void *)bufA, (void *)bufB);
        msg = cm_fn_0090d130((void *)bufA, (void *)bufB, 0x89eu);
        err_x    = (u32)bufA;
        err_y    = (u32)bufB;
        err_line = 0x89eu;
        goto report_error;
    }

    cm_fn_0062c680(param_1, param_4);
    if (DAT_00de1f44 != 0) {
        cm_fn_008b1750(DAT_00de1f44, param_4, 0);
    }

    if (param_9 != 0) {
        if (sev == 0) {
            return -1;
        }
        idx13 = (u32)((s32)(s8)it * 13);
        if (TBL_D5(idx13) == 0) {
            return -1;
        }
        if ((s8)sev < 0) {
            u32 q = (u32)cmre_magic_div((s32)(s8)sev, 2);
            if (RND(q) == 0) {
                u32 rb = (u32)(P4_W(0) * 31u) + (u32)THIS_W(0);
                *(volatile u8 *)(rb + 0x12) = (u8)it;
                *(volatile u8 *)(rb + 0x13) = 1;
                return -1;
            }
        }
        if ((s8)sev <= 0) {
            return -1;
        }
        {
            u32 rb = (u32)(P4_W(0) * 31u) + (u32)THIS_W(0);
            *(volatile u8 *)(rb + 0x12) = (u8)it;
            *(volatile u8 *)(rb + 0x13) = sev;
        }
        return -1;
    }

    /* ---- 0x62d3e3: the previous injury slot ---- */
    {
        u32 rb = (u32)(P4_W(0) * 31u) + (u32)THIS_W(0);
        u32 prev = *(volatile u32 *)(rb + 0xe);
        if (prev != 0xffffffffu) {
            u32 oldrec;
            if (it != 0x4du && it != 0x4eu && it != 0x4fu && it != 0x51u) {
                return -1;
            }
            oldrec = prev * 3u * 16u + (u32)THIS_W(4);
            saved = (u32)(*(volatile u16 *)(oldrec + 0x2c));
            cm_fn_0058e340((u32 *)THIS_W(0xc), (int *)(rb + 0x14),
                           (int *)(rb + 0x18), oldrec);
            *(volatile u8 *)oldrec = 0xff;
            rb = (u32)(P4_W(0) * 31u) + (u32)THIS_W(0);
            *(volatile u32 *)(rb + 0xe) = 0xffffffffu;
        }
    }

    /* ---- 0x62d450: pick a free slot ---- */
    {
        u32 rb = (u32)(P4_W(0) * 31u) + (u32)THIS_W(0);
        u32 listbase;
        u32 count;
        u32 i;
        int found;

        *(volatile u8 *)(rb + 0x1d) = (u8)it;

        listbase = (u32)THIS_W(4);
        count    = 0;
        found    = 0;
        i        = 0;
        if (listbase != 0) {
            count = (u32)THIS_W(8);
            if ((s32)count > 0) {
                while ((s32)i < (s32)count) {
                    if (*(volatile u8 *)(listbase + i * 0x30u) == 0xff) {
                        found = 1;
                        break;
                    }
                    i++;
                }
            }
        }

        if (found) {
            slot_index = i;
            index      = i;
        } else {
            /* 0x62d48d: the slot count is re-read here on both paths */
            u32 newcount = (u32)THIS_W(8) + 0x64u;
            u32 newbase;
            if (listbase != 0) {
                newbase = (u32)cm_fn_00945667(listbase, newcount * 0x30u);
            } else {
                newbase = (u32)cm_fn_00944e46(newcount * 0x30u);
            }
            THIS_W(4) = newbase;
            if (newbase == 0) {
                cm_fn_00944cff(0x009e7f28u, 0u, 0u, (void *)bufA, (void *)bufB);
                msg = cm_fn_0090d130((void *)bufA, (void *)bufB, 0x909u);
                err_x    = (u32)bufA;
                err_y    = (u32)bufB;
                err_line = 0x909u;
                goto report_error;
            }
            {
                /* 0x62d534: the fill start is a fresh read of [esi+8],
                   not the scan count from 0x62d466 (skipped when the
                   base was null). */
                u32 fillcount = (u32)THIS_W(8);
                if ((s32)fillcount < (s32)newcount) {
                    u32 k   = newcount - fillcount;
                    u32 off = fillcount * 0x30u;
                    do {
                        *(volatile u8 *)(newbase + off) = 0xff;
                        off += 0x30u;
                        k--;
                    } while (k != 0);
                }
            }
            index      = (u32)THIS_W(8);
            slot_index = index;
            THIS_W(8)  = newcount;
        }
    }

    /* ---- 0x62d55d ---- */
    offset = index * 0x30u;
    tmp_c  = offset;
    *(volatile u8 *)((u32)THIS_W(4) + offset) = (u8)it;

    /* ---- 0x62d576: owner records ---- */
    if (param_5 != 0) {
        u8 d = 0;
        for (;;) {
            u32 eb1 = *(volatile u32 *)((u8 *)param_5 + 0x1c);
            if (*(volatile u32 *)(eb1 + (u32)d * 4u + 0xd7u) == (u32)param_4) {
                if (eb1 != 0) {
                    cm_fn_007e4750((void *)param_5, (void *)eb1,
                                   (void *)((u32)THIS_W(4) + offset + 1u));
                    goto owner_done;
                }
                goto store_ff;
            }
            eb1 = *(volatile u32 *)((u8 *)param_5 + 0x20);
            if (*(volatile u32 *)(eb1 + (u32)d * 4u + 0xd7u) == (u32)param_4) {
                if (eb1 != 0) {
                    cm_fn_007e4750((void *)param_5, (void *)eb1,
                                   (void *)((u32)THIS_W(4) + offset + 1u));
                    goto owner_done;
                }
                goto store_ff;
            }
            d = (u8)(d + 1u);
            if ((s8)d >= 0x32) {
                break;
            }
        }
    }
store_ff:
    *(volatile u8 *)((u32)THIS_W(4) + offset + 0x1a) = 0xff;
owner_done:;

    /* ---- 0x62d5af: worst club condition ---- */
    {
        u8 best = 0;
        if (P4_W(0x39) != 0) {
            s32 k = 0;
            int n = 3;
            do {
                u32 p = *(volatile u32 *)((u32)P4_W(0x39) + (u32)k + 0x1cfu);
                if (p != 0) {
                    u32 p2 = *(volatile u32 *)(p + 0x69);
                    if (p2 != 0) {
                        s8 r = (s8)(u8)cm_fn_00544410(p2, *(volatile u8 *)(p2 + 0x1e));
                        if (r > (s8)best) {
                            u32 p3 = *(volatile u32 *)((u32)P4_W(0x39) + (u32)k + 0x1cfu);
                            u32 p4 = *(volatile u32 *)(p3 + 0x69);
                            best = (u8)cm_fn_00544410(p4, *(volatile u8 *)(p4 + 0x1e));
                        }
                    }
                }
                k += 4;
                n--;
            } while (n != 0);
        }

        /* ---- 0x62d612: severity table ---- */
        {
            s32 b    = (s32)(s8)best;
            s32 prod;
            u32 r1;
            u32 r2;

            idx13 = (u32)((s32)(s8)it * 13);
            prod  = (s32)((u32)TBL_W2(idx13) * (u32)(0x23 - b));
            ebx   = (u32)cmre_magic_div(prod, 3);

            r1 = RND((u32)(b + 5));
            r2 = RND(0x14u);
            if ((s32)r1 > (s32)r2) {
                ebx -= RND((u32)(s32)(s16)ebx);
            }
            if (it == 0x51u) {
                ebx = 0;
            }
            ebx += saved;
            age_b = P4_B(0x18);

            if (it != 0x51u) {
                if ((s32)TBL_W0(idx13) > 10) {
                    if ((s8)age_b > 30) {
                        s32 cx  = (s32)(s8)*(volatile u8 *)((u32)P4_W(0x61) + 0x2d);
                        s32 dx  = (s32)(s8)P4_B(0x18);
                        s32 axv = cx + dx - 0x23;
                        if ((s16)axv > 0) {
                            ebx += RND((u32)(s32)(s16)axv);
                        }
                    }
                    {
                        u8 pb = *(volatile u8 *)((u32)P4_W(0x61) + 0x2d);
                        if ((s8)pb > 10) {
                            ebx += RND((u32)(s32)(s16)((s32)(s8)pb - 10));
                        }
                    }
                }
            }
        }
    }

    /* ---- 0x62d6e6: the 16 bit total ---- */
    {
        u16 bp = (u16)((u16)TBL_W0(idx13) + (u16)ebx);
        saved = (u32)bp;
        total = (s32)(s16)bp;
    }

    /* ---- 0x62d705: games missed counter ---- */
    if (total > 0) {
        s32 i = 0;
        do {
            u32 addr = (u32)DAT_00ae2d0c + (u32)(P4_W(0) * 79u) + 0xau;
            if (*(volatile u8 *)addr >= 100u) {
                break;
            }
            {
                s32 a2 = (s32)(s8)*(volatile u8 *)((u32)P4_W(0x61) + 0x34);
                if (RND((u32)(a2 * 5 + 12)) == 0) {
                    u32 a3 = (u32)DAT_00ae2d0c + (u32)(P4_W(0) * 79u) + 0xau;
                    *(volatile u8 *)a3 = (u8)(*(volatile u8 *)a3 + 1u);
                }
            }
            i++;
        } while (i < total);
    }

    /* ---- 0x62d758 ---- */
    if (param_3 == 0) {
        u32 club = (u32)P4_W(0x39);
        if (club != 0) {
            if ((u32)cm_fn_00600a50(club, 0u, 1u, 0u, 0u) == 0) {
                if (total >= 100) {
                    if ((u32)cm_fn_005444a0((u32)param_4, 0u) == 0) {
                        u32 g = (u32)DAT_00de1f54;
                        if (g != 0) {
                            u32 e   = *(volatile u32 *)(g + 0x856);
                            u32 cid = *(volatile u32 *)((u32)P4_W(0x39));
                            u32 dst = *(volatile u32 *)(e + 0xdc);
                            *(volatile u8 *)(dst + cid * 9u + 8u) = 100;
                        }
                    }
                }
            }
        }
        {
            u32 r = RND(0x32u);
            if (total >= (s32)(r + 0x1eu)) {
                u32 g = (u32)DAT_00de1f54;
                if (g != 0) {
                    u32 e = *(volatile u32 *)(g + 0x856);
                    if (e != 0) {
                        cm_fn_00842b00((int)e, param_4, 0);
                    }
                }
            }
        }
    }

    /* ---- 0x62d7e9 ---- */
    *(volatile u16 *)((u32)THIS_W(4) + offset + 0x28) = (u16)saved;
    *(volatile u16 *)((u32)THIS_W(4) + offset + 0x2c) = (u16)saved;

    /* ---- 0x62d805: the scaled severity ---- */
    tmp_c = (u32)(s32)(s8)TBL_B4(idx13);
    {
        long double t;
        long long fv;
        t  = (long double)(s32)(s8)TBL_B4(idx13);
        t  = t * (long double)DBL_00967898;
        t  = t * (long double)total;
        fv = (long long)t;                 /* original call 0x00945640 (__ftol) */
        *(volatile u16 *)((u32)THIS_W(4) + offset + 0x2a) = (u16)(u32)fv;
    }

    /* ---- 0x62d82b ---- */
    {
        u32 lb = (u32)THIS_W(4);
        s32 s28 = (s32)(s16)(*(volatile u16 *)(lb + offset + 0x28));
        s32 s2a = (s32)(s16)(*(volatile u16 *)(lb + offset + 0x2a));
        if (s2a + 0x3c < s28) {
            *(volatile u16 *)(lb + offset + 0x2a) = (u16)(s28 - 0x3c);
        }
    }
    {
        u32 lb = (u32)THIS_W(4);
        *(volatile u16 *)(lb + offset + 0x2e) =
            *(volatile u16 *)(lb + offset + 0x2a);
    }

    /* ---- 0x62d858 ---- */
    if (param_3 != 0) {
        u32 r = RND((u32)total);
        u32 *res = cm_fn_0054c8e0((short *)AEC90_ADDR, &tmp_c, (int)(r + 1u));
        cm_fn_00417c10((u32 *)((u32)THIS_W(4) + offset + 0x20), &saved,
                       (u16 *)res);
    } else {
        cm_fn_00417c10((u32 *)((u32)THIS_W(4) + offset + 0x20), &tmp_c,
                       (u16 *)AEC90_ADDR);
    }

    cm_fn_0062c680(param_1, param_4);
    cm_fn_0062dca0((u32)param_4, (u32)THIS_W(4) + offset, param_7);

    /* ---- 0x62d8e6 ---- */
    {
        u32 rb = (u32)(P4_W(0) * 31u) + (u32)THIS_W(0);
        *(volatile u32 *)(rb + 0xe) = slot_index;
    }
    if (sev != 0 && TBL_D5(idx13) != 0) {
        if ((s8)sev < 0) {
            u32 q = (u32)cmre_magic_div((s32)(s8)sev, 2);
            if (RND(q) == 0) {
                u32 rb = (u32)(P4_W(0) * 31u) + (u32)THIS_W(0);
                *(volatile u8 *)(rb + 0x12) = (u8)it;
                if (RND((u32)(s32)(s8)sev) == 0) {
                    rb = (u32)(P4_W(0) * 31u) + (u32)THIS_W(0);
                    *(volatile u8 *)(rb + 0x13) = 1;
                }
            }
        } else if ((s8)sev > 0) {
            u32 rb = (u32)(P4_W(0) * 31u) + (u32)THIS_W(0);
            *(volatile u8 *)(rb + 0x12) = (u8)it;
            *(volatile u8 *)(rb + 0x13) = sev;
        }
    }

    /* ---- 0x62d979 ---- */
    if (P4_W(0x24) != 0 && DAT_00b74318 != 0) {
        u16 w = *(volatile u16 *)((u32)THIS_W(4) + offset + 0x28);
        cm_fn_00775cd0(DAT_00b74318, param_4, (short)w);
    }
    {
        u32 rb = (u32)(P4_W(0) * 31u) + (u32)THIS_W(0);
        *(volatile u8 *)(rb + 0x1c) = 0;
    }
    {
        u32 rb = (u32)(P4_W(0) * 31u) + (u32)THIS_W(0);
        cm_fn_0062e500((u32)param_1, rb, (u32)THIS_W(4) + offset);
    }

    /* ---- 0x62d9c4: the same injury again ---- */
    {
        u32 rb = (u32)(P4_W(0) * 31u) + (u32)THIS_W(0);
        if (*(volatile u8 *)(rb + 0x12) == (u8)it && (s8)sev >= 0) {
            u8 c13;
            *(volatile u8 *)(rb + 0x13) = (u8)(*(volatile u8 *)(rb + 0x13) + 1u);
            rb  = (u32)(P4_W(0) * 31u) + (u32)THIS_W(0);
            c13 = *(volatile u8 *)(rb + 0x13);
            if ((s8)c13 > 1) {
                s32 lim = 200 - (s32)TBL_W0(idx13);
                s32 div;
                u32 r1;
                u32 r2;
                if (lim > 5) {
                    div = lim;
                } else {
                    div = 5;
                }
                if ((s8)sev > 0) {
                    div = div / (s32)(s8)sev;
                }
                r1 = RND((u32)(s32)(s8)c13);
                tmp10 = r1;
                r2 = RND((u32)div);
                if ((s32)tmp10 > (s32)r2 && DAT_00b7434c != 0) {
                    u16 w = *(volatile u16 *)((u32)THIS_W(4) + offset + 0x2a);
                    cm_fn_007a7f20((u8 *)(u32)DAT_00b7434c, (u32 *)param_4, (s16)w);
                }
            }
        }
    }

    /* ---- 0x62da75: age related reporting ----
       Failure of an age block rejoins the chain (jle 0x62db06 / 0x62db4f /
       0x62db8f), but a successful roll leaves it for 0x62dbcb: the original
       takes je 0x62dbcb when DAT_00b7434c is null, else jmp 0x62dbbc (report
       call) and the return at 0x62dbc6 falls into 0x62dbcb.  So each block
       short-circuits the later age checks and the >500 block. */
    if ((s8)age_b > 36) {
        s32 w2a = (s32)(s16)(*(volatile u16 *)((u32)THIS_W(4) + offset + 0x2a));
        s32 lim = 120 - (s32)(s8)*(volatile u8 *)((u32)P4_W(0x61) + 0x2d)
                      - (s32)(s8)P4_B(0x18);
        if (w2a > lim) {
            u32 r = RND((u32)w2a);
            s32 lim2 = 90 - (s32)(s8)P4_B(0x18);
            if ((s32)r > lim2) {
                if (DAT_00b7434c != 0) {
                    u16 w = *(volatile u16 *)((u32)THIS_W(4) + offset + 0x2a);
                    cm_fn_007a7f20((u8 *)(u32)DAT_00b7434c, (u32 *)param_4, (s16)w);
                }
                goto age_chain_done;                  /* 0x62dbcb */
            }
        }
    }
    if ((s8)age_b > 33) {
        s32 w2a = (s32)(s16)(*(volatile u16 *)((u32)THIS_W(4) + offset + 0x2a));
        s32 lim = 150 - (s32)(s8)*(volatile u8 *)((u32)P4_W(0x61) + 0x2d)
                      - (s32)(s8)P4_B(0x18);
        if (w2a > lim) {
            u32 r = RND((u32)w2a);
            s32 lim2 = 90 - (s32)(s8)P4_B(0x18);
            if ((s32)r > lim2) {
                if (DAT_00b7434c != 0) {
                    u16 w = *(volatile u16 *)((u32)THIS_W(4) + offset + 0x2a);
                    cm_fn_007a7f20((u8 *)(u32)DAT_00b7434c, (u32 *)param_4, (s16)w);
                }
                goto age_chain_done;                  /* 0x62dbcb */
            }
        }
    }
    if ((s8)age_b > 31) {
        s32 w2a = (s32)(s16)(*(volatile u16 *)((u32)THIS_W(4) + offset + 0x2a));
        s32 lim = 300 - (s32)(s8)P4_B(0x18);
        if (w2a > lim) {
            u32 r = RND((u32)w2a);
            s32 lim2 = 200 - (s32)(s8)P4_B(0x18);
            if ((s32)r > lim2) {
                if (DAT_00b7434c != 0) {
                    u16 w = *(volatile u16 *)((u32)THIS_W(4) + offset + 0x2a);
                    cm_fn_007a7f20((u8 *)(u32)DAT_00b7434c, (u32 *)param_4, (s16)w);
                }
                goto age_chain_done;                  /* 0x62dbcb */
            }
        }
    }
    if ((s32)(s16)(*(volatile u16 *)((u32)THIS_W(4) + offset + 0x2a)) > 500) {
        s32 a2 = (s32)(s8)*(volatile u8 *)((u32)P4_W(0x61) + 0x34);
        if (RND((u32)(a2 + 5)) == 0 && DAT_00b7434c != 0) {
            u16 w = *(volatile u16 *)((u32)THIS_W(4) + offset + 0x2a);
            cm_fn_007a7f20((u8 *)(u32)DAT_00b7434c, (u32 *)param_4, (s16)w);
        }
    }
age_chain_done:;

    /* ---- 0x62dbcb ---- */
    if (param_6 != 0) {
        u32 a1 = param_2;
        if ((u8)a1 == 1u || (u8)a1 == 0u || (u8)a1 == 2u) {
            u32 club = (u32)P4_W(0x39);
            if (club != 0) {
                u32 c0 = *(volatile u32 *)club;
                cm_fn_0062e820((u32)param_1, AEC90_ADDR, (char)param_2, (int)c0,
                               (u32 *)0);
            }
        }
    }

    return (int)slot_index;

report_error:
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)msg, err_x, err_y, err_line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    DAT_00b67a34 = 0;
    return -1;
}
