#include "cmre.h"

/* Listing frame: the two 0x00542100 / 0x0053e140 output buffers are adjacent.
   scratch[0..199] is the 200-byte buffer at the frame offset used as the third
   argument of 0x00542100 (buf1f4); scratch[200..499] is the 300-byte buffer passed
   to 0x0053e140 (buf12c). Object fields: 0x52 signed byte in a 581-byte (0x245)
   row based at VA 0x00ae23b4, 0x38 pointer into the same row. Output VA 0x00de1f64. */
#define REC_S8(addr, off) (*(volatile s8 *)((u32)(addr) + (u32)(off)))
#define OUT_BUF 0x00de1f64u
#define DAT_AE23B4 CM_GLOBAL(u32, 0x00ae23b4)
#define DAT_AE23BC CM_GLOBAL(u32, 0x00ae23bc)

extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern unsigned long long CM_THISCALL cm_fn_004ae660(u32 self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, u32 p2, u32 p3, u32 p4);
extern u32 cm_fn_0053e140(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6, u32 p7);
extern u32 cm_fn_0053e990(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5);
extern u32 cm_fn_00542100(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6);
extern char *cm_fn_0066f4e0(u32 dst, u32 fmt, ...);

void CM_STDCALL cm_fn_0076ff40(u32 param_1, u32 param_2, u32 param_3)
{
    u8 scratch[500];
    u32 buf1f4;
    u32 buf12c;
    u32 r0;
    u32 r1;
    u32 r2;
    u32 r3;
    u32 r4;
    u32 r5;
    u32 edi;
    u32 iVar7;

    buf1f4 = (u32)&scratch[0];
    buf12c = (u32)&scratch[200];

    r0 = (u32)cm_fn_00785830((void *)param_1, 0u);
    r1 = (u32)cm_fn_00785830((void *)param_1, 1u);
    r2 = (u32)cm_fn_00785830((void *)param_1, 2u);
    r3 = (u32)cm_fn_00785830((void *)param_1, 3u);
    r4 = (u32)cm_fn_00785830((void *)param_1, 4u);

    if (param_2 == 0u) {
        r5 = (u32)cm_fn_00785830((void *)param_1, 5u);
        cm_fn_0053e140(r1, r2, r3, buf12c, 300u, 5u, r0);
        edi = r4 * 581u;
        iVar7 = (u32)(s32)REC_S8(DAT_AE23B4 + edi, 0x52);
        cm_fn_0066f4e0(OUT_BUF, 0x00a5c888u, iVar7, iVar7, buf12c, DAT_AE23B4 + edi + 0x38u);
        cm_fn_004ae660(param_3, OUT_BUF);
        if (cm_fn_0053e990(r0 * 0x6eu + DAT_AE23BC, r1, r2, r3, 0u) != 0u) {
            cm_fn_004ae8a0(param_3, buf12c, 0x7d7u, r0 * 0x6eu + DAT_AE23BC);
        }
        if (r5 != 0u) {
            cm_fn_004ae8a0(param_3, DAT_AE23B4 + edi + 0x38u, 0x7d6u, DAT_AE23B4 + edi);
        } else {
            cm_fn_004ae8a0(param_3, DAT_AE23B4 + edi + 0x38u, 0x7d5u, DAT_AE23B4 + edi);
        }
        return;
    }

    r5 = (u32)cm_fn_00785830((void *)param_1, 5u);
    edi = r4 * 581u;
    cm_fn_00542100(DAT_AE23B4 + edi, r5, buf1f4, 200u, 2u, 1u);
    cm_fn_0053e140(r1, r2, r3, buf12c, 300u, 1u, r0);
    iVar7 = (u32)(s32)REC_S8(DAT_AE23B4 + edi, 0x52);
    cm_fn_0066f4e0(OUT_BUF, 0x00a5c8dcu, iVar7, iVar7, buf12c, buf1f4);
    cm_fn_004ae660(param_3, OUT_BUF);
    if (cm_fn_0053e990(r0 * 0x6eu + DAT_AE23BC, r1, r2, r3, 0u) != 0u) {
        cm_fn_004ae8a0(param_3, buf12c, 0x7d7u, r0 * 0x6eu + DAT_AE23BC);
    }
    if (r5 != 0u) {
        cm_fn_004ae8a0(param_3, buf1f4, 0x7d6u, DAT_AE23B4 + edi);
    } else {
        cm_fn_004ae8a0(param_3, buf1f4, 0x7d5u, DAT_AE23B4 + edi);
    }
}
