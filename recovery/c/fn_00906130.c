#include "cmre.h"

#define DAT_009cf6e8   CM_GLOBAL(int, 0x009cf6e8)
#define DAT_009cf6f0   CM_GLOBAL(int, 0x009cf6f0)
#define DAT_009cf6f8   CM_GLOBAL(int, 0x009cf6f8)
#define DAT_00ae23b4   CM_GLOBAL(int, 0x00ae23b4)
#define DAT_00ae23d0   CM_GLOBAL(int, 0x00ae23d0)
#define DAT_00de1f64   CM_GLOBAL(u32, 0x00de1f64)

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae770(u32 self, u32 addition);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, const char *name, u32 key_a, u32 key_b);
extern char * cm_fn_0066f4e0(int param_1, char *param_2, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);

void CM_STDCALL cm_fn_00906130(u32 param_1, u32 param_2, u32 param_3)
{
    volatile u8 *p1 = (volatile u8 *)&param_1;
    volatile u8 *p2 = (volatile u8 *)&param_2;
    volatile u8 *p3 = (volatile u8 *)&param_3;
    u32 self_lookup = param_1;
    u32 self_report = param_3;
    u32 r, v, rem;
    u32 vals[3];
    u32 *pl;
    volatile u8 *pc;
    int count;

    if (param_2 == 0) {
        r = (u32)cm_fn_00785830((void *)self_lookup, 0x1eu);
        v = r & 0xffffu;
        rem = (v + 1u) % 100u;
        cm_fn_0066f4e0((int)(u32)&DAT_00de1f64, (char *)0x00aa1b50, (int)v, (int)rem);
        cm_fn_004ae660((void *)self_report, (u32)&DAT_00de1f64);
        return;
    }

    vals[0] = (u32)DAT_009cf6e8;
    vals[1] = (u32)DAT_009cf6f0;
    vals[2] = (u32)DAT_009cf6f8;
    p2[0] = 0;
    p2[1] = 10;
    p2[2] = 20;

    cm_fn_0066f4e0((int)(u32)&DAT_00de1f64, (char *)0x00aa1b04);
    cm_fn_004ae660((void *)self_report, (u32)&DAT_00de1f64);

    pl = vals;
    pc = p2;
    count = 3;
    do {
        u8 c = *pc;
        u32 idx = (u32)cm_fn_00785830((void *)self_lookup, (u32)c);
        if (idx != 0xffffffffu) {
            u32 comp = *pl;
            u32 base2 = (u32)DAT_00ae23d0;
            u32 off = comp * 107u;
            s8 bv = *(volatile s8 *)(u32)(base2 + off + 0x52u);
            u32 ptr = base2 + off + 0x38u;
            u32 kb, nm;
            int upper;
            u8 nxt, cc;

            cm_fn_0066f4e0((int)(u32)&DAT_00de1f64, (char *)0x00aa1ae4,
                           (int)bv, (int)bv, (int)ptr);
            cm_fn_004ae770(self_report, (u32)&DAT_00de1f64);
            base2 = (u32)DAT_00ae23d0;
            kb = base2 + off;
            nm = base2 + off + 0x38u;
            cm_fn_004ae8a0(self_report, (const char *)nm, 0x7d0u, kb);

            upper = (int)(s8)c + 10;
            p1[0] = c;
            nxt = (u8)(c + 1u);
            p3[0] = nxt;

            for (;;) {
                u32 t1, t2, r0, r1, r2, r3, r4;
                u32 base1;
                char *fmt;
                s8 by, bx;
                u32 pt, pt2, nm2;

                cc = p1[0];
                nxt = p3[0];
                t1 = (u32)cm_fn_00785830((void *)self_lookup, (u32)cc);
                if (t1 == 0xffffffffu)
                    break;
                t2 = (u32)cm_fn_00785830((void *)self_lookup, (u32)nxt);
                fmt = (t2 == 0xffffffffu) ? (char *)0x00aa1acc : (char *)0x00aa1ab4;

                r0 = (u32)cm_fn_00785830((void *)self_lookup, (u32)cc);
                base1 = (u32)DAT_00ae23b4;
                pt = base1 + r0 * 581u + 0x38u;
                r1 = (u32)cm_fn_00785830((void *)self_lookup, (u32)cc);
                base1 = (u32)DAT_00ae23b4;
                by = *(volatile s8 *)(u32)(base1 + r1 * 581u + 0x52u);
                r2 = (u32)cm_fn_00785830((void *)self_lookup, (u32)cc);
                base1 = (u32)DAT_00ae23b4;
                bx = *(volatile s8 *)(u32)(base1 + r2 * 581u + 0x52u);

                cm_fn_0066f4e0((int)(u32)&DAT_00de1f64, fmt,
                               (int)bx, (int)by, (int)pt);
                cm_fn_004ae770(self_report, (u32)&DAT_00de1f64);

                r3 = (u32)cm_fn_00785830((void *)self_lookup, (u32)cc);
                base1 = (u32)DAT_00ae23b4;
                pt2 = base1 + r3 * 581u;
                r4 = (u32)cm_fn_00785830((void *)self_lookup, (u32)cc);
                base1 = (u32)DAT_00ae23b4;
                nm2 = base1 + r4 * 581u + 0x38u;
                cm_fn_004ae8a0(self_report, (const char *)nm2, 0x7d5u, pt2);

                nxt = (u8)(nxt + 1u);
                cc = (u8)(cc + 1u);
                p3[0] = nxt;
                p1[0] = cc;
                if ((int)(s8)cc >= upper)
                    break;
            }
        }
        pl++;
        pc++;
        count--;
    } while (count != 0);
}
