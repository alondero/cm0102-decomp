/* fn_00733000 faithful translation of the 3.9.68 original (VA 0x00733000). */
#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, const char *name, u32 key_a, u32 key_b);
extern u32 CM_CDECL cm_fn_0053e140(u32 first, u32 second, u32 third, u32 output, u32 size, u32 kind, u32 person);
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third, u32 date);
extern char * CM_CDECL cm_fn_0066f4e0(int param_1, char *param_2, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);

#define G_AD9C64 CM_GLOBAL(u8, 0x00ad9c64)
#define G_AE23B4 CM_GLOBAL(int, 0x00ae23b4)
#define G_AE23BC CM_GLOBAL(volatile u32, 0x00ae23bc)

#define BM_MAIN ((volatile u8  *)0x733920)
#define TT_MAIN ((volatile u32 *)0x7338d8)
#define BM_ALT  ((volatile u8  *)0x7339d0)
#define TT_ALT  ((volatile u32 *)0x7339b4)
#define TT_NEST ((volatile u32 *)0x7339a4)

/* One scratch allocation covering the whole L frame (L = ESP after sub esp,0x808). */
union CMFrame {
    u8  b[0x900];
    u32 w[0x240];
};
#define W(off) (S.w[(off) / 4])

#define CALL66(fmt, buf) cm_fn_0066f4e0((int)0xde1f64, (char *)(fmt), (char *)(buf))

void CM_STDCALL cm_fn_00733000(u32 arg1, u32 arg2, u32 arg3)
{
    union CMFrame S;
    void *self = (void *)arg1;
    u32 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15, r16;
    u32 p4, person1, person2;
    u32 t;
    int i;

    if (arg2 != 0) {
        r0  = (u32)cm_fn_00785830(self, 0);
        r1  = (u32)cm_fn_00785830(self, 1);
        r2  = (u32)cm_fn_00785830(self, 2);
        r3  = (u32)cm_fn_00785830(self, 3);
        r4  = (u32)cm_fn_00785830(self, 4);
        r5  = (u32)cm_fn_00785830(self, 5);
        r6  = (u32)cm_fn_00785830(self, 6);
        r7  = (u32)cm_fn_00785830(self, 7);
        r8  = (u32)cm_fn_00785830(self, 8);
        r9  = (u32)cm_fn_00785830(self, 9);
        r10 = (u32)cm_fn_00785830(self, 0xa);
        r11 = (u32)cm_fn_00785830(self, 0xb);
        r12 = (u32)cm_fn_00785830(self, 0xc);
        if (r12 == 0x420e9)
            r13 = (u32)cm_fn_00785830(self, 0xd);
        else
            r13 = 0xffffffffu;
        r14 = (u32)cm_fn_00785830(self, 0xe);
        r15 = (u32)cm_fn_00785830(self, 0xf);
        r16 = (u32)cm_fn_00785830(self, 0x10);

        /* store results to their L-relative scratch homes */
        W(0x34) = r0;  W(0x24) = r1;  W(0x18) = r2;  W(0x1c) = r3;
        W(0x10) = r5;  W(0x20) = r6;  W(0x00) = r7;  W(0x08) = r8;
        W(0x04) = r9;  W(0x0c) = r10; W(0x14) = r11; W(0x28) = r13;
        W(0x30) = r15; W(0x2c) = r16;

        /* DAT_00ad9c64-prefixed 400-byte buffer at L+0x358 */
        S.b[0x358] = G_AD9C64;
        for (i = 0; i < 99; i++) *(volatile u32 *)(S.b + 0x359 + 4*i) = 0;
        *(volatile u16 *)(S.b + 0x4e5) = 0;
        S.b[0x4e7] = 0;

        cm_fn_0053e140(r9, r10, r11, (u32)(S.b + 0x1c8), 400, 1, r8);
        cm_fn_0053e140(r9, r10, r11, (u32)(S.b + 0x678), 400, 5, r8);
        if ((int)r13 > -1)
            cm_fn_0053e140(r14, r15, r16, (u32)(S.b + 0x358), 400, 1, r13);

        p4 = (u32)G_AE23B4 + r4 * 0x245u;
        cm_fn_0053e140(r1, r2, r3, (u32)(S.b + 0x38), 400, 1, r0);
        cm_fn_0053e140(r1, r2, r3, (u32)(S.b + 0x4e8), 400, 5, r0);

        /* dispatch on r5 */
        if (r5 > 0x82)
            goto L_733639;
        t = TT_MAIN[BM_MAIN[r5]];
        switch (t) {
        case 0x733212: /* idx16 */
            CALL66(0xa25de8, S.b + 0x38); goto L_733650;
        case 0x733221: /* idx9 */
            CALL66(0xa25d30, S.b + 0x38); goto L_733650;
        case 0x733230: /* idx5 */
            if (r7 == 0) {
                cm_fn_0066f4e0((int)0xde1f64, (char *)0xa2596c,
                               (char *)(S.b + 0x38), (char *)(S.b + 0x1c8));
                goto L_733650;
            } else if (r7 == 1) {
                s8 v = *(volatile s8 *)(p4 + 0x52);
                cm_fn_0066f4e0((int)0xde1f64, (char *)0xa25a78,
                               v, v,
                               (char *)(p4 + 0x38), (char *)(S.b + 0x38),
                               (char *)(S.b + 0x1c8));
                goto L_733650;
            } else if (r7 == 2) {
                s8 v = *(volatile s8 *)(p4 + 0x52);
                cm_fn_0066f4e0((int)0xde1f64, (char *)0xa25b40,
                               v, v, (char *)(S.b + 0x38),
                               (char *)(p4 + 0x38), (char *)(S.b + 0x1c8));
                goto L_733650;
            } else {
                s8 v = *(volatile s8 *)(p4 + 0x52);
                cm_fn_0066f4e0((int)0xde1f64, (char *)0xa25c58,
                               v, v, 100, (char *)(p4 + 0x38),
                               (char *)(S.b + 0x38), (char *)(S.b + 0x1c8));
                goto L_733650;
            }
        case 0x7332ef: /* idx7 */
            if (r7 == 0)      CALL66(0xa25730, S.b + 0x38);
            else if (r7 == 1) CALL66(0xa257cc, S.b + 0x38);
            else              CALL66(0xa258a0, S.b + 0x38);
            goto L_733650;
        case 0x733328: /* idx2 */
            {
                s8 v = *(volatile s8 *)(p4 + 0x52);
                if (r7 == 0)
                    cm_fn_0066f4e0((int)0xde1f64, (char *)0xa2558c, 100, v, v,
                                   (char *)(S.b + 0x38), (char *)(S.b + 0x1c8),
                                   (char *)(p4 + 0x38));
                else
                    cm_fn_0066f4e0((int)0xde1f64, (char *)0xa2568c, v, v,
                                   (char *)(S.b + 0x38), (char *)(p4 + 0x38));
                goto L_733650;
            }
        case 0x73337e: /* idx4 */
            if (r6 == 1) { CALL66(0xa254cc, S.b + 0x38); goto L_733650; }
            if (r7 == 0) {
                cm_fn_0066f4e0((int)0xde1f64, (char *)0xa251e0, 100, 100,
                               (char *)(S.b + 0x1c8), (char *)(S.b + 0x38),
                               (char *)(S.b + 0x678));
                goto L_733650;
            } else if (r7 == 1) {
                cm_fn_0066f4e0((int)0xde1f64, (char *)0xa252f4,
                               (char *)(S.b + 0x38), (char *)(S.b + 0x1c8));
                goto L_733650;
            } else {
                cm_fn_0066f4e0((int)0xde1f64, (char *)0xa253b0, 100,
                               (char *)(S.b + 0x38), (char *)(S.b + 0x1c8),
                               (char *)(S.b + 0x678));
                goto L_733650;
            }
        case 0x733422: /* idx13 */
            CALL66(0xa25120, S.b + 0x38); goto L_733650;
        case 0x733431: /* idx0 */
            if (r6 == 1) { CALL66(0xa25060, S.b + 0x38); goto L_733650; }
            if (r7 > 3) goto L_73350f;
            t = TT_NEST[r7];
            switch (t) {
            case 0x73345b:
                if ((int)r13 > -1) {
                    cm_fn_0066f4e0((int)0xde1f64, (char *)0xa24f80,
                                   (char *)(S.b + 0x358), (char *)(S.b + 0x38));
                    goto L_733650;
                }
                CALL66(0xa24eb4, S.b + 0x38); goto L_733650;
            case 0x733493:
                if ((int)r13 > -1) {
                    cm_fn_0066f4e0((int)0xde1f64, (char *)0xa24ddc,
                                   (char *)(S.b + 0x358), (char *)(S.b + 0x38));
                    goto L_733650;
                }
                CALL66(0xa24d1c, S.b + 0x38); goto L_733650;
            case 0x7334cb:
                cm_fn_0066f4e0((int)0xde1f64, (char *)0xa24c5c, 100,
                               (char *)(S.b + 0x38));
                goto L_733650;
            case 0x7334e9:
                cm_fn_0066f4e0((int)0xde1f64, (char *)0xa24b90, 100,
                               (char *)(S.b + 0x38), (char *)(S.b + 0x1c8));
                goto L_733650;
            default: goto L_73350f;
            }
        L_73350f:
        case 0x73350f:
            CALL66(0xa24ad0, S.b + 0x38); goto L_733650;
        case 0x73351e: /* idx12 */
            if (r7 == 0) {
                cm_fn_0066f4e0((int)0xde1f64, (char *)0xa248c0, 100,
                               (char *)(S.b + 0x38), (char *)(S.b + 0x1c8));
                goto L_733650;
            } else if (r7 == 1) {
                cm_fn_0066f4e0((int)0xde1f64, (char *)0xa24950, 100,
                               (char *)(S.b + 0x1c8), (char *)(S.b + 0x38));
                goto L_733650;
            } else {
                CALL66(0xa24a60, S.b + 0x38); goto L_733650;
            }
        case 0x733585: /* idx8 */
            CALL66(0xa24a60, S.b + 0x38); goto L_733650;
        case 0x733594: /* idx3 */
            CALL66(0xa247ec, S.b + 0x38); goto L_733650;
        case 0x7335a3: /* idx6 */
            CALL66(0xa243f8, S.b + 0x38); goto L_733650;
        case 0x7335ad: /* idx14 */
            CALL66(0xa24750, S.b + 0x38); goto L_733650;
        case 0x7335bc: /* idx10 */
            if (r7 == 0) {
                s8 v = *(volatile s8 *)(p4 + 0x52);
                cm_fn_0066f4e0((int)0xde1f64, (char *)0xa244d8, 100, v, v,
                               (char *)(S.b + 0x1c8), (char *)(S.b + 0x38),
                               (char *)(p4 + 0x38), (char *)(S.b + 0x4e8));
                goto L_733650;
            } else if (r7 == 1) {
                cm_fn_0066f4e0((int)0xde1f64, (char *)0xa24638,
                               (char *)(S.b + 0x38), (char *)(S.b + 0x1c8),
                               (char *)(S.b + 0x4e8));
                goto L_733650;
            } else goto L_733639;
        case 0x733626: /* idx11 */
            CALL66(0xa243f8, S.b + 0x38); goto L_733650;
        case 0x73362d: /* idx15 */
            CALL66(0xa24474, S.b + 0x38); goto L_733650;
        L_733639:
        case 0x733639: /* idx1, idx17, default */
            CALL66(0xa243f8, S.b + 0x38);
            /* fall through to shared tail */
        }
        L_733650:
        /* shared tail (main path) */
        cm_fn_004ae660((void *)arg3, 0xde1f64);
        person1 = r0 * 0x6e + (u32)G_AE23BC;
        if (cm_fn_0053e990(person1, r1, r2, r3, 0) != 0) {
            person1 = r0 * 0x6e + G_AE23BC;
            cm_fn_004ae8a0(arg3, (char *)(S.b + 0x38),   0x7d7, person1);
            person1 = r0 * 0x6e + G_AE23BC;
            cm_fn_004ae8a0(arg3, (char *)(S.b + 0x4e8), 0x7d7, person1);
        }
        person2 = r8 * 0x6e + (u32)G_AE23BC;
        if (cm_fn_0053e990(person2, r9, r10, r11, 0) != 0) {
            person2 = r8 * 0x6e + G_AE23BC;
            cm_fn_004ae8a0(arg3, (char *)(S.b + 0x1c8), 0x7d7, person2);
            person2 = r8 * 0x6e + G_AE23BC;
            cm_fn_004ae8a0(arg3, (char *)(S.b + 0x678), 0x7d7, person2);
        }
        if (p4 != 0) {
            cm_fn_004ae8a0(arg3, (char *)(p4 + 0x38), 0x7d5, p4);
            cm_fn_004ae8a0(arg3, (char *)(p4 + 4),    0x7d5, p4);
        }
        return;
    }
    /* alt path: arg2 == 0 */
    {
        u32 a0, a1, a2, a3, a5, a6, a7, palt;
        a0 = (u32)cm_fn_00785830(self, 0);
        a1 = (u32)cm_fn_00785830(self, 1);
        a2 = (u32)cm_fn_00785830(self, 2);
        a3 = (u32)cm_fn_00785830(self, 3);
        (void)cm_fn_00785830(self, 4);
        a5 = (u32)cm_fn_00785830(self, 5);
        a6 = (u32)cm_fn_00785830(self, 6);
        a7 = (u32)cm_fn_00785830(self, 7);
        W(0x08) = a2; W(0x04) = a3; W(0x10) = a6;
        cm_fn_0053e140(a1, a2, a3, (u32)(S.b + 0x38), 400, 5, a0);

        if (a5 > 0x82) goto L_ALT_DEFAULT;
        t = TT_ALT[BM_ALT[a5]];
        switch (t) {
        case 0x73380f: /* idx0 */
            if (a6 == 1) { CALL66(0xa24384, S.b + 0x38); break; }
            if (a7 == 0 || a7 == 1) {
                cm_fn_0066f4e0((int)0xde1f64, (char *)0xa24364);
                goto L_ALT_TAIL;
            }
            goto L_ALT_DEFAULT;
        case 0x73383f: /* idx4 */
            CALL66(0xa24328, S.b + 0x38); break;
        case 0x73384b: /* idx2 */
            CALL66(0xa242f0, S.b + 0x38); break;
        case 0x733852: /* idx1 */
            CALL66(0xa242f0, S.b + 0x38); break;
        case 0x733803: /* idx5 */
            CALL66(0xa243bc, S.b + 0x38); break;
        case 0x733859: /* idx3, idx6, default */
        default:
        L_ALT_DEFAULT:
            CALL66(0xa242f0, S.b + 0x38); break;
        }
        L_ALT_TAIL:
        cm_fn_004ae660((void *)arg3, 0xde1f64);
        palt = a0 * 0x6e + (u32)G_AE23BC;
        if (cm_fn_0053e990(palt, a1, a2, a3, 0) != 0) {
            palt = a0 * 0x6e + G_AE23BC;
            cm_fn_004ae8a0(arg3, (char *)(S.b + 0x38), 0x7d7, palt);
        }
        return;
    }
}
