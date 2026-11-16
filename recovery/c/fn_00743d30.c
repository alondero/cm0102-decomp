/* fn_00743d30, preferred-image VA 0x00743d30 (image base 0x00400000).
   Translated statement by statement from the original instruction listing.

   Entry stack layout: 0x743d30 is `mov eax,0x1588; call 0x00945610`.  That helper
   (0x945610, 47 bytes) probes callerESP-0x1000 and callerESP-0x1588 and returns
   with ESP = callerESP-0x1588, so the scratch frame base is
      L = E - 0x1588            (E = entry ESP; [E] = return address)
   and the four saved registers live at [L-0x10,L).  Every access below is an
   L-relative scratch offset; the three formals are read from their caller-owned
   homes E+8 (branch selector), E-0xc (object, equals the first stack slot) and
   E+0xc (receiver of 0x4ae660 / 0x4ae8a0), i.e. the first, second and third
   caller stack slots.

   L-relative scratch layout recovered from the listing (the five output buffers
   tile exactly, which fixes the base):
     0x00 idx10  0x04 idx1  0x08 idx11  0x0c idx2  0x10 idx3
     0x14 581-stride record address   0x18 idx6   0x1c idx5 (switch selector)
     0x20 buffer1 (0x78)  0x98 buffer3 (0x78)  0x110 buffer4 (0x78)
     0x188 buffer2 (0x78)  0x200 format output */

#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(int param_1, u8 *param_2, int param_3, int param_4);
extern u32 CM_CDECL cm_fn_0053e140(u32 first, u32 second, u32 third, u32 output, u32 size, u32 kind, u32 person);
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third, u32 date);
extern char *CM_CDECL cm_fn_0066f4e0(void *out, u32 fmt, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);

#define FI10  0x000u
#define FI1   0x004u
#define FI11  0x008u
#define FI2   0x00cu
#define FI3   0x010u
#define FP581 0x014u
#define FI6   0x018u
#define FI5   0x01cu
#define BUF1  0x020u
#define BUF3  0x098u
#define BUF4  0x110u
#define BUF2  0x188u
#define FMT   0x200u

#define A32(p) ((u32)(unsigned long)(p))
#define SX8(p) ((s32)*(volatile s8 *)(unsigned long)(p))

static u32 fld(const u8 *f, u32 off)
{
    return *(const volatile u32 *)(f + off);
}

static void fst(u8 *f, u32 off, u32 v)
{
    *(volatile u32 *)(f + off) = v;
}

/* ret 0xc: three caller-supplied stack arguments. */
u32 CM_STDCALL cm_fn_00743d30(u32 a1, u32 a2, u32 a3)
{
    u8 frame[0x1588];
    u32 i0, i1, i2, i3, i4, i5, i6, i7, i9, i10, i11, i12;
    u32 p581, p110, p;
    u32 sel, tgt, v;
    s32 sb;

    if (a2 == 0) goto L_pathB;

    /* ---- a2 != 0 ---------------------------------------------------- */
    i0 = (u32)cm_fn_00785830((void *)a1, 0);
    i1 = (u32)cm_fn_00785830((void *)a1, 1);
    fst(frame, FI1, i1);
    i2 = (u32)cm_fn_00785830((void *)a1, 2);
    fst(frame, FI2, i2);
    i3 = (u32)cm_fn_00785830((void *)a1, 3);
    fst(frame, FI3, i3);
    i4 = (u32)cm_fn_00785830((void *)a1, 4);
    i5 = (u32)cm_fn_00785830((void *)a1, 5);
    fst(frame, FI5, i5);
    i6 = (u32)cm_fn_00785830((void *)a1, 6);
    fst(frame, FI6, i6);
    (void)cm_fn_00785830((void *)a1, 7);
    i9 = (u32)cm_fn_00785830((void *)a1, 9);
    i10 = (u32)cm_fn_00785830((void *)a1, 10);
    fst(frame, FI10, i10);
    i11 = (u32)cm_fn_00785830((void *)a1, 11);
    fst(frame, FI11, i11);
    i12 = (u32)cm_fn_00785830((void *)a1, 12);

    /* 0x743dea/0x743dfb: base + edi + (145*edi)*4 = base + 581*edi */
    p581 = CM_GLOBAL(u32, 0xae23b4) + i4;
    p581 = p581 + 580u * i4;
    fst(frame, FP581, p581);

    cm_fn_0053e140(fld(frame, FI1), fld(frame, FI2), fld(frame, FI3),
                   A32(frame + BUF1), 0x78, 1, i0);
    cm_fn_0053e140(fld(frame, FI1), fld(frame, FI2), fld(frame, FI3),
                   A32(frame + BUF2), 0x78, 5, i0);
    cm_fn_0053e140(fld(frame, FI10), fld(frame, FI11), i12,
                   A32(frame + BUF3), 0x78, 1, i9);
    cm_fn_0053e140(fld(frame, FI10), fld(frame, FI11), i12,
                   A32(frame + BUF4), 0x78, 5, i9);

    /* 0x743e7e: cmp sel,4 / ja default / jmp [sel*4+0x744220] */
    sel = fld(frame, FI5);
    if (sel > 4)
        tgt = 0x00743f87u;
    else
        tgt = *(volatile u32 *)(unsigned long)(0x00744220u + sel * 4u);
    switch (tgt) {
    case 0x00743ee1u: goto L_ee1;
    case 0x00743f27u: goto L_f27;
    case 0x00743f87u: goto L_f87;
    case 0x00743e8eu: goto L_e8e;
    case 0x00743eb9u: goto L_eb9;
    default: goto L_after;
    }

L_e8e: /* case 3, fmt 0xa395d8, 9 arguments */
    sb = SX8(p581 + 0x52);
    p581 = p581 + 0x38;
    cm_fn_0066f4e0(frame + FMT, 0xa395d8u, (u32)sb, (u32)sb, 0x64,
                   p581, A32(frame + BUF1), A32(frame + BUF3), A32(frame + BUF4));
    goto L_after;

L_eb9: /* case 4, fmt 0xa39498, 9 arguments */
    sb = SX8(p581 + 0x52);
    p581 = p581 + 0x38;
    cm_fn_0066f4e0(frame + FMT, 0xa39498u, (u32)sb, (u32)sb, 0x64,
                   p581, A32(frame + BUF1), A32(frame + BUF3), A32(frame + BUF4));
    goto L_after;

L_ee1: /* case 0: sub eax,2 on the frame+0x18 value */
    v = fld(frame, FI6) - 2u;
    if (v == 0) goto L_f03;
    sb = SX8(p581 + 0x52);
    p581 = p581 + 0x38;
    cm_fn_0066f4e0(frame + FMT, 0xa393a4u, (u32)sb, (u32)sb,
                   p581, A32(frame + BUF1));
    goto L_after;
L_f03:
    sb = SX8(p581 + 0x52);
    p581 = p581 + 0x38;
    cm_fn_0066f4e0(frame + FMT, 0xa392a0u, (u32)sb, (u32)sb, 0x64,
                   p581, A32(frame + BUF1), A32(frame + BUF3), A32(frame + BUF4));
    goto L_after;

L_f27: /* case 1: dec chain on the frame+0x18 value */
    v = fld(frame, FI6) - 1u;
    if (v == 0) goto L_f71;
    v = v - 1u;
    if (v == 0) goto L_f39;
    v = v - 1u;
    if (v == 0) goto L_f71;
    goto L_after;
L_f39: /* case 1 type 2, fmt 0xa39158, 9 arguments */
    sb = SX8(p581 + 0x52);
    p581 = p581 + 0x38;
    cm_fn_0066f4e0(frame + FMT, 0xa39158u, (u32)sb, (u32)sb, 0x64,
                   p581, A32(frame + BUF1), A32(frame + BUF3), A32(frame + BUF4));
    goto L_after;
L_f71: /* case 1 type 1/3, fmt 0xa39064, 6 arguments */
    sb = SX8(p581 + 0x52);
    p581 = p581 + 0x38;
    cm_fn_0066f4e0(frame + FMT, 0xa39064u, (u32)sb, (u32)sb,
                   p581, A32(frame + BUF1));
    goto L_after;

L_f87: /* case 2 (and out-of-range): dec chain on the frame+0x18 value */
    v = fld(frame, FI6) - 1u;
    if (v == 0) goto L_fc6;
    v = v - 1u;
    if (v == 0) goto L_f96;
    v = v - 1u;
    if (v == 0) goto L_fc6;
    goto L_after;
L_f96: /* case 2 type 2, fmt 0xa38f40, 8 arguments */
    sb = SX8(p581 + 0x52);
    p581 = p581 + 0x38;
    cm_fn_0066f4e0(frame + FMT, 0xa38f40u, (u32)sb, (u32)sb, 0x64,
                   p581, A32(frame + BUF1), A32(frame + BUF3));
    goto L_after;
L_fc6: /* case 2 type 1/3, fmt 0xa38e40, 6 arguments */
    sb = SX8(p581 + 0x52);
    p581 = p581 + 0x38;
    cm_fn_0066f4e0(frame + FMT, 0xa38e40u, (u32)sb, (u32)sb,
                   p581, A32(frame + BUF1));
    goto L_after;

L_after:
    cm_fn_004ae660((void *)a3, A32(frame + FMT));

    p110 = CM_GLOBAL(u32, 0xae23bc) + 110u * i0;
    if (cm_fn_0053e990(p110, fld(frame, FI1), fld(frame, FI2),
                       fld(frame, FI3), 0) != 0) {
        p = CM_GLOBAL(u32, 0xae23bc) + 110u * i0;
        cm_fn_004ae8a0((int)a3, frame + BUF1, 0x7d7, (int)p);
        p = CM_GLOBAL(u32, 0xae23bc) + 110u * i0;
        cm_fn_004ae8a0((int)a3, frame + BUF2, 0x7d7, (int)p);
    }

    p110 = CM_GLOBAL(u32, 0xae23bc) + 110u * i9;
    if (cm_fn_0053e990(p110, fld(frame, FI10), fld(frame, FI11),
                       i12, 0) != 0) {
        p = CM_GLOBAL(u32, 0xae23bc) + 110u * i9;
        cm_fn_004ae8a0((int)a3, frame + BUF3, 0x7d7, (int)p);
        p = CM_GLOBAL(u32, 0xae23bc) + 110u * i9;
        cm_fn_004ae8a0((int)a3, frame + BUF4, 0x7d7, (int)p);
    }

    /* 0x7440e3: reload the 581-stride address, 0x7d5 kind */
    p = fld(frame, FP581);
    return (u32)cm_fn_004ae8a0((int)a3, (u8 *)(unsigned long)(p + 0x38), 0x7d5, (int)p);

    /* ---- a2 == 0 ---------------------------------------------------- */
L_pathB:
    i0 = (u32)cm_fn_00785830((void *)a1, 0);
    i1 = (u32)cm_fn_00785830((void *)a1, 1);
    i2 = (u32)cm_fn_00785830((void *)a1, 2);
    i3 = (u32)cm_fn_00785830((void *)a1, 3);
    fst(frame, FI10, i3);
    (void)cm_fn_00785830((void *)a1, 4);
    i5 = (u32)cm_fn_00785830((void *)a1, 5);
    fst(frame, FP581, i5);
    (void)cm_fn_00785830((void *)a1, 6);
    i7 = (u32)cm_fn_00785830((void *)a1, 7);

    cm_fn_0053e140(i1, i2, fld(frame, FI10), A32(frame + BUF1), 0x190, 5, i0);

    /* 0x744175: test esi(7); then frame+0x14 == 3 || == 4 selects the format */
    if (i7 != 0 && (fld(frame, FP581) == 3u || fld(frame, FP581) == 4u))
        cm_fn_0066f4e0(frame + FMT, 0xa38e10u, A32(frame + BUF1));
    else
        cm_fn_0066f4e0(frame + FMT, 0xa38ddcu, A32(frame + BUF1));

    cm_fn_004ae660((void *)a3, A32(frame + FMT));

    p110 = CM_GLOBAL(u32, 0xae23bc) + 110u * i0;
    if (cm_fn_0053e990(p110, i1, i2, fld(frame, FI10), 0) != 0) {
        p = CM_GLOBAL(u32, 0xae23bc) + 110u * i0;
        return (u32)cm_fn_004ae8a0((int)a3, frame + BUF1, 0x7d7, (int)p);
    }
    return 0;
}
