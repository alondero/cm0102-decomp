/* Preferred-image VA 0x004d71b0, image base 0x00400000.
   Binary: CM0102 3.9.68 SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86
   Faithful translation checked against the listing at preferred-image VAs. */
#include "cmre.h"

#define DAT_00ae23b4 CM_GLOBAL(s32, 0x00ae23b4)
#define DAT_00de1f54 CM_GLOBAL(u32, 0x00de1f54)
#define DAT_00ae2a38 CM_GLOBAL(u32, 0x00ae2a38)
#define DAT_00b7431c CM_GLOBAL(u32, 0x00b7431c)
#define FS0 CM_FS(void *, 0)

extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, u32 person, u32 club);
/* ABI: at preferred-image VA 0x004d738e (pinned binary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86) the
   listing loads ECX from the saved incoming receiver before `call 0x4d1600`
   at 0x004d739c with six stack arguments; the callee consumes that ECX, so
   the boundary is thiscall with a leading self parameter. */
extern u8 * CM_THISCALL cm_fn_004d1600(u32 self,int *a1,int *a2,int *a3,u16 *a4,char a5,int a6);
extern u32 CM_CDECL cm_fn_005444a0(u32 person, u32 input_club);
extern long long CM_THISCALL cm_fn_00598800(u32 self,u32 person,u32 include,u32 output,u32 adjust,u32 extra);
extern u32 CM_THISCALL cm_fn_005a0590(u32 self, u32 arg1);
extern unsigned long long CM_CDECL cm_fn_005b7d20(u32 person);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag, u32 person, u32 force);
extern u32 CM_THISCALL cm_fn_0068d020(u32 self);
extern u32 CM_THISCALL cm_fn_0068d040(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self,u32 arg1,u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self,u32 index,u32 value);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self,u32 arg1,u32 arg2);
extern u32 CM_THISCALL cm_fn_00842180(u32 self, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5);
extern char CM_THISCALL cm_fn_008d1da0(u32 self, u32 p1, u32 p2, u32 *p3, u32 p4);
extern unsigned long long CM_CDECL cm_fn_0090cfc0(u32 bound);

/* Scratch mirrors original ESP0-relative stack slots: slot ESP0-o maps to sc[SCOFF-o]. */
#define SCOFF 0x1c0
#define S(o) (sc + SCOFF + (o))
#define SB(o) (*S(o))
static u32 ld32(u8 *p){ u32 v; __builtin_memcpy(&v,p,4); return v; }
static void st32(u8 *p,u32 v){ __builtin_memcpy(p,&v,4); }
#define LD(o) ld32(S(o))
#define ST(o,v) st32(S(o),(u32)(v))

#define O_THIS  (-0x184)
#define O_176   (-0x176)
#define O_16E   (-0x16e)
#define O_S168  (-0x168)
#define O_BUF178 (-0x178)
#define O_BUFA  (-0x164)
#define O_CNT   (-0x186)
#define O_N     (-0x180)
#define O_ARR(n) (-0x84 + 4*(n))

void CM_THISCALL cm_fn_004d71b0(u32 self, u32 *param_1, u32 *param_2)
{
    u8 sc[0x200];
    volatile struct { u32 prev, handler, state; } frame;
    u32 iv5, r600, ebx, r, A, B, v, esi_v, r6002, r5a;
    s32 v32, idx, iVar7;
    long double t, lhs, rhs;
    long long ll, ll2;
    s32 n, i, j, rem, m, cnt;
    u32 ai, aj, p;
    s16 bi, bj;
    u8 ct, dl, bl2;
    char cr;

    frame.state = 0xffffffffu;
    frame.handler = 0x00956f16u;
    frame.prev = (u32)FS0;
    FS0 = (void *)&frame;

    if (!param_1) goto light_epi;
    if (!*(u32 *)((u8 *)param_1 + 0x61)) goto light_epi;
    if (!param_2) goto light_epi;
    if (!*(u32 *)((u8 *)param_2 + 0xcf)) goto light_epi;

    SB(O_CNT) = 0;
    ST(O_THIS, self);

    iv5 = cm_fn_004cf5f0(self, (u32)param_1, (u32)param_2);
    cm_fn_0068d020((u32)S(O_BUF178));
    frame.state = 0;                             /* 0x004d7223 */
    if (!iv5) goto full_epi;

    cm_fn_0077bbe0(S(O_BUFA), 0xbea, 0);
    ((u8 *)&frame.state)[0] = 1;                 /* 0x004d724d: byte write to frame.state */

    cm_fn_00785790(S(O_BUFA), 0,  *(u32 *)((u8 *)param_2 + 0));
    cm_fn_00785790(S(O_BUFA), 1,  *(u32 *)((u8 *)param_1 + 0));
    cm_fn_00785790(S(O_BUFA), 2,  *(u32 *)(*(u32 *)((u8 *)param_1 + 4) + 0x33));
    cm_fn_00785790(S(O_BUFA), 3,  *(u32 *)(*(u32 *)((u8 *)param_1 + 8) + 0x33));
    cm_fn_00785790(S(O_BUFA), 4,  *(u32 *)(*(u32 *)((u8 *)param_1 + 0xc) + 0x33));
    {
        u32 P = *(volatile u32 *)((u8 *)param_2 + 0xcf);
        cm_fn_00785790(S(O_BUFA), 5, *(u32 *)(P + 0));
        P = *(volatile u32 *)((u8 *)param_2 + 0xcf);
        cm_fn_00785790(S(O_BUFA), 6, *(u32 *)(*(u32 *)(P + 4) + 0x33));
        P = *(volatile u32 *)((u8 *)param_2 + 0xcf);
        cm_fn_00785790(S(O_BUFA), 7, *(u32 *)(*(u32 *)(P + 8) + 0x33));
        P = *(volatile u32 *)((u8 *)param_2 + 0xcf);
        cm_fn_00785790(S(O_BUFA), 8, *(u32 *)(*(u32 *)(P + 0xc) + 0x33));
    }
    cm_fn_00785790(S(O_BUFA), 9,  *(u32 *)((u8 *)iv5 + 0x45));
    cm_fn_00785790(S(O_BUFA), 10, (u32)(s32)(s8)(*(u8 *)((u8 *)iv5 + 0x4f) >> 4));
    r = cm_fn_005b7d20(*(u32 *)((u8 *)param_1 + 0x61));   /* movsx ecx, ax */
    cm_fn_00785790(S(O_BUFA), 11, (u32)(s32)(s16)(u16)(u32)r);
    v = (u32)cm_fn_0090cfc0(2);                            /* push eax */
    cm_fn_00785790(S(O_BUFA), 12, v);

    iVar7 = (s32)cm_fn_005444a0((u32)param_1, (u32)param_2);
    ebx = (u32)(iVar7 == 0);
    r600 = cm_fn_00600a50((u32)param_2, 0, 1, 0, 0);
    ST(O_S168, ebx);                                      /* 0x004d7367 */

    if (r600 == 0 || (ebx != 0 && ((*(u8 *)((u8 *)iv5 + 0x4f) & 0xf0) != 0x10))) {
        /* x87: fild [B]; fmul c1; fadd c2; fimul [A]; call 0x00945640 (__ftol) */
        A = (u32)cm_fn_004d1600((u32)LD(O_THIS), (int *)param_2, 0, (int *)param_1, 0, 1, 1);
        B = (u32)cm_fn_0090cfc0(0x29);
        t = (long double)(s32)B;
        t = t * (long double)CM_GLOBAL(double, 0x967898);
        t = t + (long double)CM_GLOBAL(double, 0x9689b0);
        t = t * (long double)(s32)A;
        ll = (long long)t;                                /* cm_fn_00945640 */
        v32 = (s32)(u32)ll;                               /* mov ecx, eax */
        if (v32 > 10000 && v32 < 200000) {
            esi_v = (u32)((v32 / 10000) * 10000);
        } else if (v32 < 1000000) {
            esi_v = (u32)((v32 / 100000) * 100000);
        } else if (v32 < 5000000) {
            esi_v = (u32)((v32 / 250000) * 250000);
        } else {
            esi_v = (u32)((v32 / 1000000) * 1000000);
        }
    } else {
        esi_v = *(u32 *)((u8 *)param_1 + 0x52);
    }
    ST(O_THIS, esi_v);                                    /* 0x004d7485 */
    cm_fn_00785790(S(O_BUFA), 13, esi_v);
    if ((s32)esi_v < 500000) goto full_epi;               /* cmp esi,0x7a120; jl */

    {
        u32 gde = CM_GLOBAL(u32, 0x00de1f54);
        u32 recv = *(u32 *)((u8 *)gde + 0x856);
        cm_fn_00842180(recv, (u32)param_1, (u32)S(O_BUF178), 1, 1, 0);
    }
    ST(O_16E, LD(O_176));                                 /* 0x004d74b7/bb */

    ct = SB(O_CNT);
    if (ct < 30) {
        u32 it = cm_fn_0068d1b0((u32)S(O_BUF178));
        while (it != 0) {
            ct = SB(O_CNT);
            if (ct >= 30) break;
            idx = *(volatile s32 *)((u8 *)it + 4);
            p = CM_GLOBAL(volatile u32, 0x00ae23b4) + (u32)idx * 581u;
            r6002 = cm_fn_00600a50(p, 0, 1, 0, 0);
            if (r6002 == 0) {
                /* accepted */
            } else {
                idx = *(volatile s32 *)((u8 *)it + 4);
                p = CM_GLOBAL(volatile u32, 0x00ae23b4) + (u32)idx * 581u;
                r5a = cm_fn_005a0590(CM_GLOBAL(u32, 0x00ae2a38), p);
                if (r5a == 0) goto next_it;
                ll2 = cm_fn_00598800(r5a, 0, 0, 0, 1, 0);
                lhs = (long double)(s32)esi_v * (long double)CM_GLOBAL(double, 0x9689c0);
                rhs = (long double)ll2;
                if (!(lhs <= rhs)) goto next_it;           /* fcompp; je skip */
            }
            idx = *(volatile s32 *)((u8 *)it + 4);
            if (idx <= -1) goto next_it;
            p = CM_GLOBAL(volatile u32, 0x00ae23b4) + (u32)idx * 581u;
            ST(O_ARR((s32)(s8)ct), p);
            SB(O_CNT) = (u8)(ct + 1);
        next_it:
            it = cm_fn_0068d1b0((u32)S(O_BUF178));
        }
        ct = SB(O_CNT);
    }

    n = (s32)(s8)ct;
    ST(O_N, (u32)n);
    if (n > 1) {
        for (i = 0; i < n - 1; i++) {
            j = i + 1;
            rem = n - j;
            while (rem != 0) {
                ai = LD(O_ARR(i));
                aj = LD(O_ARR(j));
                bi = *(s16 *)((u8 *)ai + 0x80);
                bj = *(s16 *)((u8 *)aj + 0x80);
                if (bi < bj) {
                    ST(O_ARR(i), aj);
                    ST(O_ARR(j), ai);
                }
                j++;
                rem--;
            }
        }
    }

    m = (n < 3) ? n : 3;
    SB(O_THIS) = 0x0f;                                    /* 0x004d7624 */
    cm_fn_00785790(S(O_BUFA), 14, (u32)m);

    dl = 0;
    bl2 = 0xf;
    for (;;) {
        m = (n < 3) ? n : 3;
        if ((s32)(s8)dl >= m) break;                      /* 0x004d7647 */
        if (dl == 1) {
            if (LD(O_ARR(1)) == LD(O_ARR(0))) {
                SB(O_THIS) = bl2;
                cm_fn_00785790(S(O_BUFA), (u32)(bl2 - 2), 1);
                goto final_call;
            }
        } else if (dl == 2) {
            if (LD(O_ARR(2)) == LD(O_ARR(0)) || LD(O_ARR(2)) == LD(O_ARR(1))) {
                SB(O_THIS) = bl2;
                cm_fn_00785790(S(O_BUFA), (u32)(bl2 - 3), 2);
                goto final_call;
            }
        }
        p = LD(O_ARR((s32)(s8)dl));
        cm_fn_00785790(S(O_BUFA), (u32)bl2, *(u32 *)p);
        bl2++;
        dl++;
    }
    SB(O_THIS) = bl2;                                     /* 0x004d76d6 */

final_call:
    cm_fn_00785790(S(O_BUFA), LD(O_THIS), LD(O_S168));

    {
        u32 gde = CM_GLOBAL(u32, 0x00de1f54);
        if (gde != 0 && *(u32 *)((u8 *)param_1 + 0x61) != 0)
            cr = cm_fn_008d1da0(gde, (u32)param_1, 0x21a, (u32 *)S(O_ARR(0)), 0x1e);
        else
            cr = 0;
    }
    if ((s32)(s8)cr > 0) {
        cnt = (s32)(s8)cr;
        {
            u32 *pp = (u32 *)S(O_ARR(0));
            while (cnt-- > 0) {
                cm_fn_007861e0((void *)CM_GLOBAL(u32, 0x00b7431c), (u32)S(O_BUFA), *pp);
                pp++;
            }
        }
    }

full_epi:
    frame.state = 0xffffffffu;                            /* 0x004d7742 */
    cm_fn_0068d040((u32)S(O_BUF178));
light_epi:
    FS0 = (void *)frame.prev;                             /* 0x004d775d */
}
