/* Tier-2 C recovery for the routine at preferred-image VA 0x009409b0
   (2765 bytes, cdecl, no callee cleanup, one stack argument pair used:
   param_1 = object pointer at [esp+4], param_3 = low byte of [esp+0xc]).

   Method: every register and every stack slot the listing touches is a local
   with the listing's name (eax..edi; sp<N> = [esp+0xN] at the point of use).
   Each instruction is transcribed in listing order, with labels at the listing
   jump targets. Object fields and heap/list memory are read and written through
   volatile accesses at the listing offsets. Partial-register writes go through
   SETB, which keeps the upper bits exactly as the listing does. */
#include "cmre.h"

#define MEM8(a)    (*(volatile u8 *)(u32)(a))
#define MEM16(a)   (*(volatile u16 *)(u32)(a))
#define MEM32(a)   (*(volatile u32 *)(u32)(a))
#define SETB(r, v) ((r) = ((r) & 0xffffff00u) | (u32)(u8)(v))
#define SAR(x, n)  ((u32)((s32)(x) >> (n)))

extern u32 cm_fn_00944250(int param_1, u32 param_2, int param_3);

void cm_fn_009409b0(u32 param_1, u32 param_2, u8 param_3)
{
    u32 eax = 0, ebx = 0, ecx = 0, edx = 0, edi = 0, ebp = 0, esi = 0;
    u32 sp13 = 0, sp14 = 0, sp18 = 0, sp1c = 0, sp20 = 0, sp24 = 0;
    u32 sp28 = 0, sp2c = 0, sp30 = 0, sp34 = 0, sp38 = 0, sp3c = 0;
    u32 sp40 = 0, sp44 = 0, sp48 = 0, sp4c = 0, sp54 = 0, sp5c = 0;
    u32 cf = 0;

    (void)param_2;
    sp54 = param_1;     /* [esp+0x54] = first stack argument, never written */
    sp5c = param_3;     /* [esp+0x5c] = third argument; only its low byte is read first */
    esi = param_1;      /* mov esi, [esp+0x50] (009409b6) */

    /* 009409bb */
    SETB(eax, MEM8(esi + 0x2200));
    SETB(sp13, eax);
    eax = MEM32(esi + 0x20);
    if (eax != 0) goto L940bc0;

    /* 009409d0 */
    ebp = MEM32(esi + 0x38);
    edx = MEM32(esi + 0x34);
    ebx = MEM32(esi + 0x3c);
    edi = MEM32(esi + 0xd0);
    ebp += edx;
    edx = MEM32(esi + 0x40);
    edx += ebx;
    ebx = MEM32(esi + 0xd4);
    eax = 0;
    ebp = SAR(ebp, 1);
    edx = SAR(edx, 1);
    sp24 = ebp;
    sp20 = ebp;
    sp30 = edx;
    sp28 = edx;
    if ((s32)edi > (s32)ebx) goto L940ac8;

    /* 00940a0a: first scan, from edi upward for a non-null row head */
    ecx = MEM32(esi + 0xc8);
    ecx += edi * 4;
L940a13:
    eax = MEM32(ecx);
    if (eax != 0) goto L940a23;
    edi++;
    ecx += 4;
    if ((s32)edi <= (s32)ebx) goto L940a13;
    goto L940a62;

L940a23:
    ecx = edi << 6;
    ecx += 0x20;
    if ((s32)ecx < (s32)edx) sp28 = ecx;

    if ((s32)edi > (s32)ebx) goto L940a50;
    ecx = MEM32(esi + 0xc8);
    ecx += ebx * 4;
L940a40:
    eax = MEM32(ecx);
    if (eax != 0) goto L940a50;
    ebx--;
    ecx -= 4;
    if ((s32)edi <= (s32)ebx) goto L940a40;
    goto L940a62;

L940a50:
    ecx = ebx << 6;
    ecx += 0x20;
    if ((s32)ecx <= (s32)edx) goto L940a62;
    edx = ecx;
    sp30 = edx;

L940a62:
    if ((s32)edi > (s32)ebx) goto L940ac8;
    ecx = MEM32(esi + 0xc8);
    ebp = ebx;
    ebp -= edi;
    eax = ecx + edi * 4;
    ebp++;
    sp2c = eax;

    /* 00940a78: walk every row between edi and ebx */
L940a78:
    ecx = sp2c;
    eax = MEM32(ecx);
    if (eax == 0) goto L940ab6;
    ecx = MEM32(eax);
    edx = sp20;
    ecx = SAR(ecx, 1);
    if ((s32)ecx >= (s32)edx) goto L940a92;
    sp20 = ecx;
L940a92:
    ecx = MEM32(eax + 4);
    if (ecx == 0) goto L940aa2;
L940a99:
    eax = ecx;
    ecx = MEM32(ecx + 4);
    if (ecx != 0) goto L940a99;
L940aa2:
    ecx = MEM32(eax);
    edx = sp24;
    ecx = SAR(ecx, 1);
    if ((s32)ecx <= (s32)edx) goto L940ab2;
    sp24 = ecx;
L940ab2:
    edx = sp30;
L940ab6:
    ecx = sp2c;
    ecx += 4;
    ebp--;
    sp2c = ecx;
    if (ebp != 0) goto L940a78;
    ebp = sp24;

L940ac8:
    /* 00940ac8 */
    SETB(ecx, MEM8(esi + 0x2203));
    MEM32(esi + 0xd0) = edi;
    MEM32(esi + 0xd4) = ebx;
    if ((u8)ecx == 0) goto L940ba5;

    edi = MEM32(esi + 0x160);
    ecx = MEM32(esi + 0x164);
    if ((s32)edi > (s32)ecx) goto L940b0b;
    eax = MEM32(esi + 0x158);
    ebx = eax + edi * 4;
L940afb:
    eax = MEM32(ebx);
    if (eax != 0) goto L940b0f;
    edi++;
    ebx += 4;
    if ((s32)edi <= (s32)ecx) goto L940afb;
    goto L940b55;

L940b0b:
    if (eax == 0) goto L940b27;

L940b0f:
    ebp = sp20;
    ebx = edi << 6;
    ebx += 0x20;
    if ((s32)ebx >= (s32)ebp) goto L940b23;
    sp20 = ebx;
L940b23:
    ebp = sp24;

L940b27:
    if ((s32)edi > (s32)ecx) goto L940b43;
    eax = MEM32(esi + 0x158);
    eax += ecx * 4;
L940b34:
    if (MEM32(eax) != 0) goto L940b47;
    ecx--;
    eax -= 4;
    if ((s32)edi <= (s32)ecx) goto L940b34;
    goto L940b55;

L940b43:
    if (eax == 0) goto L940b55;

L940b47:
    eax = ecx << 6;
    eax += 0x20;
    if ((s32)eax <= (s32)ebp) goto L940b55;
    ebp = eax;

L940b55:
    MEM32(esi + 0x160) = edi;
    MEM32(esi + 0x164) = ecx;
    if ((s32)edi > (s32)ecx) goto L940ba5;
    eax = MEM32(esi + 0x158);
    ecx -= edi;
    ecx++;
    ebx = eax + edi * 4;
L940b71:
    edi = MEM32(ebx);
    if (edi == 0) goto L940b9f;
    eax = MEM32(edi);
    eax = SAR(eax, 1);
    if ((s32)eax >= (s32)sp28) goto L940b85;
    sp28 = eax;
L940b85:
    eax = MEM32(edi + 4);
    if (eax == 0) goto L940b95;
L940b8c:
    edi = eax;
    eax = MEM32(eax + 4);
    if (eax != 0) goto L940b8c;
L940b95:
    edi = MEM32(edi);
    edi = SAR(edi, 1);
    if ((s32)edi <= (s32)edx) goto L940b9f;
    edx = edi;
L940b9f:
    ebx += 4;
    ecx--;
    if (ecx != 0) goto L940b71;

L940ba5:
    ecx = sp20;
    eax = sp28;
    MEM32(esi + 0x24) = ecx;
    MEM32(esi + 0x28) = ebp;
    MEM32(esi + 0x2c) = eax;
    MEM32(esi + 0x30) = edx;
    MEM32(esi + 0x20) = 1;

L940bc0:
    /* 00940bc0: derive the output rectangle and row/column extents */
    ebx = MEM32(esi + 0x24);
    eax = MEM32(esi + 0x28);
    edi = MEM32(esi + 0x2c);
    ecx = MEM32(esi + 0x30);
    ebp = eax + ebx;
    eax += 0x40;
    ebp = SAR(ebp, 1);
    sp48 = ebp;
    ebp = ecx + edi;
    MEM32(esi + 0x14) = ebx;
    ecx += 0x40;
    eax = SAR(eax, 6);
    ebx = SAR(ebx, 6);
    ebp = SAR(ebp, 1);
    MEM32(esi + 0x4) = eax;
    MEM32(esi + 0x10) = ecx;
    eax -= ebx;
    sp38 = ebp;
    ecx = SAR(ecx, 6);
    edi = SAR(edi, 6);
    ebp = eax;
    SETB(eax, sp13);
    sp44 = ecx;
    MEM32(esi + 0xc) = ecx;
    ecx -= edi;
    sp30 = ebx;
    MEM32(esi) = ebx;
    sp20 = edi;
    MEM32(esi + 0x8) = edi;
    sp24 = ecx;
    if ((u8)eax != 0) goto L940c2d;
    eax = ebp + 7;
    edx = (u32)((s32)eax >> 31);
    edx &= 7;
    eax += edx;
    eax = SAR(eax, 3);
    goto L940c2f;

L940c2d:
    eax = ebp;

L940c2f:
    SETB(edx, sp5c);
    sp18 = eax;
    MEM32(esi + 0x18) = eax;
    MEM32(esi + 0x1c) = 0;
    if ((u8)edx != 0) goto L941475;

    /* 00940c49: allocate and zero the working buffer */
    ebp = eax;
    ebp = ebp * ecx;
    ecx = MEM32(esi + 0x2222);
    eax = cm_fn_00944250((int)ecx, ebp, 5);
    edx = ebp;
    edi = eax;
    edx = SAR(edx, 2);
    eax = 0;
    sp14 = edi;
    MEM32(esi + 0x1c) = edi;
    if ((s32)edx <= 0) goto L940c7e;
    ecx = edx;
    {
        register u32 cursor = edi;
        register u32 remaining = ecx;
        register u32 fill = eax;
        while (remaining >= 4u) {
            MEM32(cursor) = fill; cursor += 4u;
            MEM32(cursor) = fill; cursor += 4u;
            MEM32(cursor) = fill; cursor += 4u;
            MEM32(cursor) = fill; cursor += 4u;
            remaining -= 4u;
        }
        while (remaining != 0u) {
            MEM32(cursor) = fill;
            cursor += 4u;
            --remaining;
        }
        edi = cursor;
        ecx = remaining;
    }
    ecx = 0;
    edi = sp14;
    eax = edx;

L940c7e:
    eax <<= 2;
    if ((s32)eax >= (s32)ebp) goto L940c9b;
    ecx = ebp;
    edi += eax;
    ecx -= eax;
    eax = 0;
    edx = ecx;
    ecx >>= 2;
    while (ecx != 0) {
        MEM32(edi) = eax;
        edi += 4;
        ecx--;
    }
    ecx = edx;
    ecx &= 3;
    while (ecx != 0) {
        MEM8(edi) = (u8)eax;
        edi++;
        ecx--;
    }

L940c9b:
    eax = MEM32(esi + 0xd0);
    ecx = MEM32(esi + 0xd4);
    if ((s32)eax > (s32)ecx) goto L940cba;
    edx = sp44;
    if ((s32)ecx < (s32)edx) goto L940cba;
    edx--;
    MEM32(esi + 0xd4) = edx;

L940cba:
    SETB(ecx, sp13);
    if ((u8)ecx == 0) goto L94107f;
    ecx = sp24;
    edx = sp14;
    sp5c = eax;
    ebp = ecx - 1;
    ecx = sp20;
    sp40 = ebp;
    ebp -= eax;
    ebp += ecx;
    ecx = MEM32(esi + 0xd4);
    ebp = ebp * sp18;
    ebp -= ebx;
    ebp += edx;
    if ((s32)eax > (s32)ecx) goto L940df2;

    /* 00940cf8: per-row walk over the lists in [esi+0xc8] */
L940cf8:
    edx = MEM32(esi + 0xc8);
    ecx = MEM32(edx + eax * 4);
    if (ecx == 0) goto L940dd9;
    eax = MEM32(ecx + 4);
    ebx = 0;
    sp28 = eax;
L940d12:
    edx = MEM32(ecx);
    esi = MEM32(eax);
    ecx = esi;
    edi = edx;
    ecx &= 1;
    edi &= 1;
    ecx += edi;
    ebx = ebx + ecx * 2 - 2;
    if (ebx == 0) goto L940d40;
L940d2a:
    eax = MEM32(eax + 4);
    esi = MEM32(eax);
    ecx = esi;
    ecx &= 1;
    ebx = ebx + ecx * 2 - 1;
    if (ebx != 0) goto L940d2a;
    sp28 = eax;

L940d40:
    edx = SAR(edx, 1);
    esi = SAR(esi, 1);
    eax = edx;
    sp3c = esi;
    edx = SAR(edx, 6);
    esi = SAR(esi, 6);
    if (edx != esi) goto L940d70;

    /* equal columns: single-cell update */
    SETB(eax, eax & 0x3f);
    ecx = (ecx & 0xffffff00u) | 0x7f;
    SETB(ecx, (u8)eax * 0x7fu);
    SETB(eax, sp3c);
    SETB(eax, eax & 0x3f);
    SETB(ecx, (u8)ecx + (u8)eax);
    SETB(eax, MEM8(edx + ebp));
    SETB(ecx, (u8)ecx << 1);
    SETB(eax, (u8)eax + (u8)ecx);
    MEM8(edx + ebp) = (u8)eax;
    goto L940db7;

L940d70:
    SETB(eax, eax & 0x3f);
    SETB(ecx, 0x7e);
    SETB(eax, (u8)eax << 1);
    SETB(ecx, (u8)ecx - (u8)eax);
    SETB(eax, MEM8(edx + ebp));
    SETB(eax, (u8)eax + (u8)ecx);
    MEM8(edx + ebp) = (u8)eax;
    edx++;
    if ((s32)edx >= (s32)esi) goto L940dac;
    eax = edx + ebp;
    if (((u8)eax & 1) == 0) goto L940d90;
    MEM8(eax) = 0x7e;
    edx++;

L940d90:
    if ((s32)edx >= (s32)esi) goto L940dac;
    ecx = esi;
    edi = edx + ebp;
    ecx -= edx;
    eax = 0x7e7e7e7eu;
    ecx++;
    ecx = ecx >> 1;
    cf = ecx & 1;      /* CF after the second shr = bit shifted out by it */
    ecx = ecx >> 1;
    /* rep stosd: ecx dwords of eax at edi */
    while (ecx != 0) {
        MEM32(edi) = eax;
        edi += 4;
        ecx--;
    }
    /* adc ecx,ecx with ecx = 0: ecx = carry */
    ecx = cf;
    /* rep stosw: ecx words of ax at edi */
    while (ecx != 0) {
        MEM16(edi) = (u16)eax;
        edi += 2;
        ecx--;
    }

L940dac:
    SETB(eax, sp3c);
    SETB(eax, eax & 0x3f);
    SETB(eax, (u8)eax << 1);
    MEM8(esi + ebp) = (u8)eax;

L940db7:
    edx = sp28;
    ecx = MEM32(edx + 4);
    if (ecx == 0) goto L940dd1;
    eax = MEM32(ecx + 4);
    sp28 = eax;
    if (eax != 0) goto L940d12;

L940dd1:
    esi = sp54;
    eax = sp5c;

L940dd9:
    edx = sp18;
    ecx = MEM32(esi + 0xd4);
    eax++;
    ebp -= edx;
    sp5c = eax;
    if ((s32)eax <= (s32)ecx) goto L940cf8;

L940df2:
    /* 00940df2 */
    eax = MEM32(esi + 0x160);
    ecx = MEM32(esi + 0x164);
    sp5c = eax;
    if ((s32)eax > (s32)ecx) goto L941475;
    esi = sp54;

L940e0e:
    esi = MEM32(esi + 0x158);
    ecx = 0;
    SETB(edx, 0);
    eax = MEM32(esi + eax * 4);
    SETB(sp24, edx);
    if (eax == ecx) goto L94105c;
    esi = MEM32(eax + 4);
    sp1c = ecx;
    sp2c = esi;

L940e32:
    esi = sp2c;
    edi = MEM32(eax);
    ebx = edi;
    ebp = MEM32(esi);
    ebx &= 1;
    eax = ebp;
    eax &= 1;
    eax += ebx;
    ebx = sp1c;
    eax = ebx + eax * 2 - 2;
    sp1c = eax;
    if (eax == 0) goto L940e70;
L940e56:
    esi = MEM32(esi + 4);
    ebp = MEM32(esi);
    ebx = ebp;
    ebx &= 1;
    eax = eax + ebx * 2 - 1;
    if (eax != 0) goto L940e56;
    sp1c = eax;
    sp2c = esi;

L940e70:
    esi = sp5c;
    esi -= sp30;
    edi = SAR(edi, 1);
    eax = edi;
    sp44 = esi;
    esi = sp40;
    sp48 = eax;
    edi = SAR(edi, 6);
    sp4c = edi;
    esi -= edi;
    edi = sp20;
    esi += edi;
    edi = sp44;
    esi = esi * sp18;
    ebp = SAR(ebp, 1);
    ebx = ebp;
    esi += edi;
    ebp = SAR(ebp, 6);
    if (ecx == 0) goto L940f39;
    edi = sp14;
    edi += esi;
    if (ecx == edi) goto L940f39;

    /* 00940ebb: a cell that is already non-empty */
    SETB(edx, MEM8(ecx));
    eax = 0x3f;
    SETB(sp28, edx);
    edx = sp24;
    edx &= 0xff;
    edi = 0x3f;
    eax -= edx;
    sp34 = eax;
    eax = sp28;
    eax &= 0xff;
    sp3c = eax;
    edi -= eax;
    eax = sp34;
    sp38 = edi;
    edi = eax;
    edi *= eax;
    eax = 0xf82;
    eax -= edi;
    sp34 = eax;
    eax = sp38;
    edi = eax;
    edi *= eax;
    eax = edi;
    edi = 0xf82;
    edi -= eax;
    eax = sp3c;
    sp38 = edi;
    eax *= edi;
    edi = sp34;
    edx *= edi;
    eax += edx;
    edx = sp38;
    edi += edx;
    {
        s32 num = (s32)eax, den = (s32)edi;
        eax = (u32)(num / den);
        edx = (u32)(num % den);
    }
    SETB(edx, 0);
    MEM8(ecx) = (u8)eax;
    eax = sp48;
    goto L940f39;

L940f39:
    if (sp4c == ebp) goto L940f56;
    /* 00940f5e: different row, interpolate with the neighbour cell */
    SETB(eax, eax & 0x3f);
    SETB(ecx, 0x7e);
    SETB(eax, (u8)eax << 1);
    SETB(ecx, (u8)ecx - (u8)eax);
    edi = 0x3f;
    SETB(edx, (u8)edx + (u8)ecx);
    SETB(ebx, ebx & 0x3f);
    SETB(sp24, edx);
    edx = sp14;
    ecx = sp24;
    SETB(eax, MEM8(esi + edx));
    ecx &= 0xff;
    SETB(sp28, eax);
    edx = 0x3f;
    eax = sp28;
    edx -= ecx;
    eax &= 0xff;
    sp3c = eax;
    edi -= eax;
    eax = edx;
    eax *= edx;
    edx = 0xf82;
    edx -= eax;
    eax = edi;
    eax *= edi;
    ecx *= edx;
    edi = 0xf82;
    edi -= eax;
    eax = sp3c;
    eax *= edi;
    eax += ecx;
    edi += edx;
    {
        s32 num = (s32)eax, den = (s32)edi;
        eax = (u32)(num / den);
        edx = (u32)(num % den);
    }
    ecx = sp40;
    edi = sp14;
    SETB(ebx, (u8)ebx << 1);
    SETB(edx, ebx);
    ebx = sp20;
    ecx -= ebp;
    SETB(sp24, edx);
    ecx += ebx;
    ecx = ecx * sp18;
    MEM8(esi + edi) = (u8)eax;
    esi = sp44;
    ecx += esi;
    ecx += edi;
    goto L940ff0;

L940f56:
    /* 00940f56: same-row shortcut (the listing's lea/jmp path) */
    SETB(eax, eax & 0x3f);
    ecx = (ecx & 0xffffff00u) | 0x7f;
    SETB(eax, (u8)eax * 0x7fu);
    SETB(ebx, ebx & 0x3f);
    SETB(eax, (u8)eax + (u8)ebx);
    SETB(eax, (u8)eax << 1);
    SETB(edx, (u8)edx + (u8)eax);
    eax = sp14;
    SETB(sp24, edx);
    ecx = esi + eax;

L940ff0:
    eax = sp2c;
    eax = MEM32(eax + 4);
    if (eax == 0) goto L94100a;
    esi = MEM32(eax + 4);
    sp2c = esi;
    if (esi != 0) goto L940e32;

L94100a:
    if (ecx == 0) goto L94105c;
    SETB(edx, MEM8(ecx));
    esi = 0x3f;
    SETB(sp28, edx);
    edx = sp24;
    edx &= 0xff;
    eax = sp28;
    esi -= edx;
    eax &= 0xff;
    ebx = esi;
    edi = 0x3f;
    ebx *= esi;
    esi = 0xf82;
    edi -= eax;
    esi -= ebx;
    ebx = edi;
    ebx *= edi;
    edx *= esi;
    edi = 0xf82;
    edi -= ebx;
    eax *= edi;
    eax += edx;
    edi += esi;
    {
        s32 num = (s32)eax, den = (s32)edi;
        eax = (u32)(num / den);
        edx = (u32)(num % den);
    }
    MEM8(ecx) = (u8)eax;

L94105c:
    esi = sp54;
    eax = sp5c;
    eax++;
    ecx = MEM32(esi + 0x164);
    sp5c = eax;
    if ((s32)eax <= (s32)ecx) goto L940e0e;
    return;

    /* 00940cc0 jbe target: the flag byte at [esi+0x2200] was zero. */
L94107f:
    ecx = MEM32(esi + 0xd4);
    edx = sp24;
    edi = sp20;
    edx -= ecx;
    sp5c = ecx;
    ebp = edx + edi - 1;
    edx = sp18;
    ebp *= edx;
    edi = sp14;
    ebp += edi;
    if ((s32)ecx < (s32)eax) goto L9411e1;

L9410ac:
    eax = MEM32(esi + 0xc8);
    eax = MEM32(eax + ecx * 4);
    if (eax == 0) goto L9411cc;
    ecx = MEM32(eax + 4);
    sp1c = 0;
    sp2c = ecx;

L9410cc:
    esi = sp2c;
    ecx = MEM32(eax);
    edi = ecx;
    eax = MEM32(esi);
    edi &= 1;
    edx = eax;
    edx &= 1;
    edx += edi;
    edi = sp1c;
    edx = edi + edx * 2 - 2;
    sp1c = edx;
    if (edx == 0) goto L94110a;

L9410f0:
    esi = MEM32(esi + 4);
    eax = MEM32(esi);
    edi = eax;
    edi &= 1;
    edx = edx + edi * 2 - 1;
    if (edx != 0) goto L9410f0;
    sp1c = edx;
    sp2c = esi;

L94110a:
    ecx += 0x40;
    edx = eax + 0x40;
    ecx = SAR(ecx, 7);
    edx = SAR(edx, 7);
    if ((s32)ecx >= (s32)edx) goto L9411a2;
    eax = sp30;
    esi |= 0xffffffffu;
    esi -= eax;
    ecx -= eax;
    edx += esi;
    esi = ecx;
    ebx = edx;
    esi = SAR(esi, 3);
    ebx = SAR(ebx, 3);
    if ((s32)esi != (s32)ebx) goto L94115b;
    ecx &= 7;
    eax = 0xff;
    eax = SAR(eax, (u8)ecx);
    ecx = edx;
    edx = 0xff80;
    ecx &= 7;
    edx = SAR(edx, (u8)ecx);
    SETB(ecx, MEM8(esi + ebp));
    SETB(eax, (u8)eax & (u8)edx);
    SETB(ecx, (u8)ecx | (u8)eax);
    MEM8(esi + ebp) = (u8)ecx;
    goto L9411a2;

L94115b:
    ecx &= 7;
    eax = 0xff;
    eax = SAR(eax, (u8)ecx);
    SETB(ecx, MEM8(esi + ebp));
    SETB(ecx, (u8)ecx | (u8)eax);
    MEM8(esi + ebp) = (u8)ecx;
    esi++;
    eax = esi + ebp;
    if (((u8)eax & 1) == 0) goto L941179;
    MEM8(eax) = 0xff;
    esi++;

L941179:
    if ((s32)esi >= (s32)ebx) goto L941193;
    ecx = ebx;
    eax = 0xffffffffu;
    ecx -= esi;
    edi = esi + ebp;
    ecx++;
    ecx = ecx >> 1;
    cf = ecx & 1;
    ecx = ecx >> 1;
    while (ecx != 0) {
        MEM32(edi) = eax;
        edi += 4;
        ecx--;
    }
    ecx = cf;
    while (ecx != 0) {
        MEM16(edi) = (u16)eax;
        edi += 2;
        ecx--;
    }

L941193:
    ecx = edx;
    edx = 0xff80;
    ecx &= 7;
    edx = SAR(edx, (u8)ecx);
    MEM8(ebx + ebp) = (u8)edx;

L9411a2:
    eax = sp2c;
    eax = MEM32(eax + 4);
    if (eax == 0) goto L9411bc;
    ecx = MEM32(eax + 4);
    sp2c = ecx;
    if (ecx != 0) goto L9410cc;

L9411bc:
    esi = sp54;
    ecx = sp5c;
    ebx = sp30;
    edx = sp18;

L9411cc:
    eax = MEM32(esi + 0xd0);
    ecx--;
    ebp += edx;
    sp5c = ecx;
    if ((s32)ecx >= (s32)eax) goto L9410ac;

L9411e1:
    /* 009411e1 */
    SETB(eax, MEM8(esi + 0x2201));
    if ((u8)eax == 0) goto L941338;
    ecx = MEM32(esi + 0xd0);
    eax = MEM32(esi + 0xd4);
    sp44 = 0;
    sp5c = ecx;
    if ((s32)ecx > (s32)eax) goto L941326;
    eax = sp24;
    edi = sp20;
    eax -= ecx;
    eax = eax + edi - 1;
    eax *= edx;
    edx = 0u - edx;
    sp28 = eax;
    sp40 = edx;
    eax = MEM32(esi + 0xc8);
    eax = MEM32(eax + ecx * 4);
    if (eax == 0) goto L941309;
    ecx = MEM32(eax + 4);
    sp3c = ecx;

L941242:
    edx = sp3c;
    edi = MEM32(eax);
    edi = SAR(edi, 1);
    ebp = MEM32(edx);
    ebp = SAR(ebp, 1);
    eax = ebp - edi;
    if ((s32)eax >= 0x40) goto L9412e3;
    eax = edi;
    edx = 0x80;
    eax = SAR(eax, 6);
    eax -= ebx;
    esi = ebp;
    ecx = eax;
    ecx &= 7;
    edx = SAR(edx, (u8)ecx);
    ecx = sp28;
    eax = SAR(eax, 3);
    esi = SAR(esi, 6);
    sp4c = edx;
    edx = sp14;
    eax += ecx;
    ecx = sp4c;
    esi -= ebx;
    if ((MEM8(eax + edx) & (u8)ecx) != 0) goto L9412e3;
    ecx = esi;
    eax = 0x80;
    ecx &= 7;
    eax = SAR(eax, (u8)ecx);
    ecx = sp28;
    esi = SAR(esi, 3);
    esi += ecx;
    if ((MEM8(esi + edx) & (u8)eax) != 0) goto L9412e3;
    eax = sp48;
    if ((s32)edi > (s32)eax) goto L9412ac;
    eax = edi + ebp + 1;
    goto L9412b9;

L9412ac:
    eax = edi + ebp - 2;

L9412b9:
    eax = SAR(eax, 1);
    ebp = sp28;
    sp44 = 1;
    eax = SAR(eax, 6);
    eax -= ebx;
    ecx = eax;
    ecx = SAR(ecx, 3);
    ecx += ebp;
    edx += ecx;
    ecx = eax;
    ecx &= 7;
    eax = 0x80;
    eax = SAR(eax, (u8)ecx);
    MEM8(edx) = MEM8(edx) | (u8)eax;

L9412e3:
    ecx = sp3c;
    eax = MEM32(ecx + 4);
    if (eax == 0) goto L9412fd;
    ecx = MEM32(eax + 4);
    sp3c = ecx;
    if (ecx != 0) goto L941242;

L9412fd:
    esi = sp54;
    edx = sp40;
    ecx = sp5c;

L941309:
    edi = sp28;
    eax = MEM32(esi + 0xd4);
    ecx++;
    edi += edx;
    sp5c = ecx;
    sp28 = edi;
    if ((s32)ecx <= (s32)eax) goto L94122a;
    goto L941326;

L94122a:
    eax = MEM32(esi + 0xc8);
    eax = MEM32(eax + ecx * 4);
    if (eax == 0) goto L941309;
    ecx = MEM32(eax + 4);
    sp3c = ecx;
    goto L941242;

L941326:
    edx = sp44;
    eax = MEM32(esi + 0x2204);
    eax |= edx;
    MEM32(esi + 0x2204) = eax;

L941338:
    /* 00941338 */
    SETB(eax, MEM8(esi + 0x2202));
    if ((u8)eax == 0) goto L941475;
    ecx = MEM32(esi + 0x160);
    eax = MEM32(esi + 0x164);
    sp40 = 0;
    sp5c = ecx;
    if ((s32)ecx > (s32)eax) goto L941463;

L941366:
    eax = MEM32(esi + 0x158);
    eax = MEM32(eax + ecx * 4);
    if (eax == 0) goto L941450;
    edx = MEM32(eax + 4);
    sp44 = edx;
    goto L941384;

L941380:
    ecx = sp5c;

L941384:
    esi = MEM32(eax);
    eax = MEM32(edx);
    eax = SAR(eax, 1);
    esi = SAR(esi, 1);
    sp3c = eax;
    eax -= esi;
    if ((s32)eax >= 0x40) goto L94142e;
    eax = sp30;
    ebx = 0x80;
    ecx -= eax;
    eax = sp24;
    edx = ecx;
    ecx &= 7;
    ebx = SAR(ebx, (u8)ecx);
    ecx = esi;
    edi = eax;
    ecx = SAR(ecx, 6);
    edi -= ecx;
    ecx = sp20;
    edx = SAR(edx, 3);
    ebp = edi + ecx - 1;
    edi = sp18;
    ebp *= edi;
    ecx = sp14;
    ebp += edx;
    if ((MEM8(ecx + ebp) & (u8)ebx) != 0) goto L94142e;
    ebp = sp3c;
    ebp = SAR(ebp, 6);
    eax -= ebp;
    ebp = sp20;
    eax = eax + ebp - 1;
    eax *= edi;
    eax += edx;
    if ((MEM8(eax + ecx) & (u8)ebx) != 0) goto L94142e;
    eax = sp38;
    if ((s32)esi <= (s32)eax) goto L941404;
    eax = sp3c + esi - 2;
    goto L941408;

L941404:
    eax = sp3c + esi + 1;

L941408:
    esi = sp24;
    sp40 = 1;
    eax = SAR(eax, 1);
    eax = SAR(eax, 6);
    esi -= eax;
    eax = esi + ebp - 1;
    eax *= edi;
    edx += eax;
    eax = edx + ecx;
    SETB(ecx, MEM8(edx + ecx));
    SETB(ecx, (u8)ecx | (u8)ebx);
    MEM8(eax) = (u8)ecx;

L94142e:
    ecx = sp44;
    eax = MEM32(ecx + 4);
    if (eax == 0) goto L941448;
    edx = MEM32(eax + 4);
    sp44 = edx;
    if (edx != 0) goto L941380;

L941448:
    esi = sp54;
    ecx = sp5c;

L941450:
    eax = MEM32(esi + 0x164);
    ecx++;
    sp5c = ecx;
    if ((s32)ecx <= (s32)eax) goto L941366;

L941463:
    edx = sp40;
    eax = MEM32(esi + 0x2208);
    eax |= edx;
    MEM32(esi + 0x2208) = eax;

L941475:
    return;
}
