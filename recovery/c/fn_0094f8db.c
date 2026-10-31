/* WIP: fault data address mismatch, case 35 (callee 0x0094F780 at 0x0094F7BF, test [eax], edx with eax = &dw6 + 4*q; the address follows where this frame places dw6, which C does not control) */
/* Preferred-image VA 0x0094F8DB, image base 0x00400000, cdecl, ret 0.
   Arg0 src (u16 *): reads word at +0xa, dwords at +6 and +2, word at +0.
   Arg1 out (u32 *): stores at out[1] and out[0] (cfg[4]==0x40) or out[0] (0x20).
   Arg2 cfg (s32 *): dwords cfg[0] through cfg[5] are read in listing order.
   Frame [ebp-0x18 .. ebp-1] is one 0x18-byte block: a 12-byte scratch buffer
   that is passed to 0x0094F80C, then dwords at -0x0c, -0x08 and -0x04. */
#include "cmre.h"

extern u32 cm_fn_0094f780(int param_1,int param_2);
extern void cm_fn_0094f80c(int param_1,u32 *param_2);
extern void cm_fn_0094f827(u32 *param_1);
extern u32 cm_fn_0094f833(int *param_1);
extern void CM_CDECL cm_fn_0094f84e(u32 *words, s32 value);

u32 cm_fn_0094f8db(u16 *param_1,u32 *param_2,int *param_3)
{
    struct {
        u32 scratch[3];   /* [ebp-0x18]: 12 bytes, passed to 0x0094F80C */
        u32 dw6;          /* [ebp-0x0c]: dword at src+6, passed by address */
        u32 dw2;          /* [ebp-0x08]: dword at src+2 */
        u32 spare;        /* [ebp-0x04]: word at src+0 shifted to bits 16..31; never read */
    } f;
    u16 *src;
    s32 *cfg;
    u16 w_a;
    u16 w_0;
    u32 sign;
    s32 e;
    s32 c1;
    s32 lim;
    s32 cfg4;
    u32 shift;
    u32 esi;
    u32 ret;

    src = param_1;
    cfg = param_3;

    w_a = src[5];                                   /* movzx ecx, word [eax+0xa] */
    sign = (u32)w_a & 0x8000u;                      /* and ecx, 0x8000; stored over arg0 slot */
    f.dw6 = *(u32 *)(src + 3);                      /* [eax+6] */
    f.dw2 = *(u32 *)(src + 1);                      /* [eax+2] */
    w_0 = src[0];                                   /* movzx eax, word [eax] */
    f.spare = (u32)w_0 << 16;
    e = (s32)((u32)((u32)w_a & 0x7fffu) - 0x3fffu);

    if ((u32)e == 0xffffc001u) {
        esi = 0;
        if (cm_fn_0094f833((int *)&f.dw6) != 0) {
            ret = 0;
        } else {
            cm_fn_0094f827(&f.dw6);
            ret = 2;
        }
    } else {
        cm_fn_0094f80c((int)(u32)f.scratch, &f.dw6);
        if (cm_fn_0094f780((int)(u32)&f.dw6, cfg[2]) != 0) {
            e = (s32)((u32)e + 1u);
        }
        c1 = cfg[1];
        lim = (s32)((u32)c1 - (u32)cfg[2]);
        if (e < lim) {
            cm_fn_0094f827(&f.dw6);
            esi = 0;
            ret = 2;
        } else if (e <= c1) {
            esi = (u32)c1 - (u32)e;
            cm_fn_0094f80c((int)(u32)&f.dw6, f.scratch);
            cm_fn_0094f84e(&f.dw6, (s32)esi);
            cm_fn_0094f780((int)(u32)&f.dw6, cfg[2]);
            cm_fn_0094f84e(&f.dw6, (s32)((u32)cfg[3] + 1u));
            esi = 0;
            ret = 2;
        } else if (e < cfg[0]) {
            s32 c3;
            c3 = cfg[3];
            esi = (u32)cfg[5];
            f.dw6 &= 0x7fffffffu;                   /* and byte [ebp-9], 0x7f */
            esi = esi + (u32)e;
            cm_fn_0094f84e(&f.dw6, c3);
            ret = 0;
        } else {
            cm_fn_0094f827(&f.dw6);
            f.dw6 |= 0x80000000u;                   /* or byte [ebp-9], 0x80 */
            cm_fn_0094f84e(&f.dw6, cfg[3]);
            esi = (u32)cfg[5] + (u32)cfg[0];
            ret = 1;
        }
    }

    /* 0x0094FA09: merge the mantissa, sign and shifted exponent term. */
    shift = 31u - (u32)cfg[3];                      /* push 0x1f; pop ecx; sub ecx,[edi+0xc] */
    cfg4 = cfg[4];                                  /* mov edi,[edi+0x10] */
    esi = esi << (shift & 0x1fu);                   /* shl esi, cl */
    esi |= (sign != 0) ? 0x80000000u : 0u;          /* neg/sbb/and 0x80000000 */
    esi |= f.dw6;

    if (cfg4 == 0x40) {
        param_2[1] = esi;
        param_2[0] = f.dw2;
    } else if (cfg4 == 0x20) {
        param_2[0] = esi;
    }
    return ret;
}
