/* Reconstruction of the original 3.9.68 routine at preferred-image VA
 * 0x004d8990 (image base 0x00400000).  Derived only from the original
 * instruction listing; every statement below is traceable to it.
 *
 * Calling convention: `ret 0xc` at 0x004d939a and 0x004d943e, with three
 * 32-bit stack arguments:
 *   a1 ([esp+4] at entry, read at 0x004d8999 into ESI) is forwarded as ECX to
 *      the twelve cm_fn_00785830 calls; it is a stack argument, not a
 *      thiscall receiver (ECX is never read before being written).
 *   a2 ([esp+8], read at 0x004d8a39) selects the short path at 0x004d939d.
 *   a3 ([esp+0xc], read at 0x004d92da and 0x004d93d5) is forwarded as ECX to
 *      cm_fn_004ae660 / cm_fn_004ae770 / cm_fn_004ae8a0.
 * The routine returns its final EAX (the result of the last call made on the
 * taken path): 0 from the 0x004d9370/0x004d9419 zero tests, otherwise the
 * cm_fn_004ae8a0 result.
 *
 * Frame layout.  After `sub esp,0xb6c` and the four callee-saved pushes the
 * frame base is L.  Every displacement in the listing has been converted to
 * an L-relative offset; the aggregate below reproduces those offsets so the
 * adjacency the original relies on (a 300-byte fill immediately followed by
 * the five 250-byte slots, then the two 300-byte buffers) is preserved:
 *
 *   L+0x0000 dword  copy of cm_fn_00785830(6)          (0x004d8a48)
 *   L+0x0007 byte   low byte of cm_fn_00785830(9)      (0x004d8a12)
 *   L+0x0008 dword  0x245 * cm_fn_00785830(0)          (0x004d8be0)
 *   L+0x000f byte   low byte of cm_fn_00785830(0xb)    (0x004d8a2c)
 *   L+0x0010..0x002c  getter(5),(4),(3),(0xa),(8),(7),(0xc),getter(6)&1
 *   L+0x0030 300B   cm_fn_0053e140 output, kind 1/5    (0x004d90ce)
 *   L+0x015c 5x250B text slots written by cm_fn_0066f4e0 and read by the
 *                   switch at 0x004d8fd9
 *   L+0x0640 300B   second cm_fn_0053e140 output       (0x004d90eb)
 *   L+0x076c 300B   formatted text destination         (0x004d8fef)
 *   L+0x0898..0x0b6b the remainder of the 0xb6c frame, never named by the
 *                   original and left untouched here.
 *
 * Held-concern resolution (four-reason path 0x004d906a): the selector is the
 * signed remainder of cm_fn_00785830(2) by 2, reconstructed from the original
 * partial-register sequence 0x004d906a..0x004d9077:
 *     eax = value & 0x80000001; if (SF) eax = ((eax-1) | 0xfffffffe) + 1;
 * which is -1 for negative odd values and 0 for negative even values.  The
 * scaled offset is 250 * remainder in 32-bit wrapping arithmetic, so a
 * negative odd selector yields 0xffffff06 and moves both string pointers
 * *below* the slot bases: the first becomes L+0x15c-0xfa = L+0x62 (inside the
 * as-yet-unwritten 300-byte buffer at L+0x30) and the second becomes
 * L+0x350-0xfa = L+0x256 (the first byte of the second text slot).  Both
 * pointers are handed to the original cm_fn_0066f4e0 at 0x004d90a0 exactly
 * as computed.  Nothing here guesses an empty string, clamps the index or
 * narrows the selector: the signed remainder, the input domain and the
 * wraparound address arithmetic are preserved.  Whether the original
 * formatter consumes that out-of-range text and whether anything writes it
 * first is unproven and deliberately not asserted.
 */

#include "cmre.h"

/* Callees remain explicit original-code boundaries.  cm_fn_0066f4e0 is called
   with 5, 6, 7, 8 and 9 stack arguments at 0x004d8c67, 0x004d8e6b, 0x004d91dc,
   0x004d91a2, 0x004d9175 (and with 2, 3 and 4 at 0x004d8aff, 0x004d9011,
   0x004d902e/0x004d9060), so the recovered two-parameter form is extended
   with an ellipsis; the two recovered parameter types are unchanged. */
extern char *cm_fn_0066f4e0(int param_1, char *param_2, ...);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_0053e140(u32 first, u32 second, u32 third, u32 output,
                                   u32 size, u32 kind, u32 person);
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third,
                                   u32 date);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae770(u32 self, u32 addition);
extern u32 CM_THISCALL cm_fn_004ae8a0(int param_1, u8 *param_2, int param_3,
                                      int param_4);

/* Original dword globals that hold object bases.  The listing re-reads each
   of them at every use site (0x004d8bcf/0x004d8c36/0x004d8e3f/0x004d9139/
   0x004d91af/0x004d9203/0x004d9276/0x004d92ad/0x004d9321/0x004d933e/
   0x004d9349/0x004d9376/0x004d93eb/0x004d93fa/0x004d941b), between calls to
   original code, and the resulting pointer dereferences are observable.  They
   are therefore volatile so each read stays at its own site instead of being
   cached across those calls.  Each site loads the base only once; multiple
   arguments derived at that site share that snapshot. */
#define G_AE23A8 CM_GLOBAL(volatile u32, 0x00ae23a8)
#define G_AE23B4 CM_GLOBAL(volatile u32, 0x00ae23b4)
#define G_AE23BC CM_GLOBAL(volatile u32, 0x00ae23bc)

/* The original pushes the address 0x00de1f64 (a global text buffer) as a
   pointer argument; only its address is used. */
#define L_DE1F64 0x00de1f64u

typedef struct cm_frame_4d8990 {
    u32 l00;            /* L+0x0000 */
    u8 l04[3];
    u8 l07;             /* L+0x0007 */
    u32 l08;            /* L+0x0008 */
    u8 l0c[3];
    u8 l0f;             /* L+0x000f */
    u32 l10;            /* L+0x0010 */
    u32 l14;            /* L+0x0014 */
    u32 l18;            /* L+0x0018 */
    u32 l1c;            /* L+0x001c */
    u32 l20;            /* L+0x0020 */
    u32 l24;            /* L+0x0024 */
    u32 l28;            /* L+0x0028 */
    u32 l2c;            /* L+0x002c */
    u8 l30[300];        /* L+0x0030 .. L+0x015b */
    u8 reason[5][250];  /* L+0x015c .. L+0x063d */
    u8 l63e[2];         /* L+0x063e .. L+0x063f */
    u8 l640[300];       /* L+0x0640 .. L+0x076b */
    u8 l76c[300];       /* L+0x076c .. L+0x0897 */
    u8 tail[0x2d4];     /* L+0x0898 .. L+0x0b6b */
} cm_frame_4d8990;

/* Argument the original stores as an int-sized value. */
#define A32(p) ((int)(u32)(void *)(p))
/* Text slot selected by the running reason count, sign-extended as the
   listing's `movsx eax, bl` before the 125*x then doubled scaling. */
#define REASON_SLOT(n) ((u8 *)(void *)f.reason[(s32)(s8)(n)])

u32 CM_STDCALL cm_fn_004d8990(u32 a1, u32 a2, u32 a3)
{
    cm_frame_4d8990 f;
    void *self;
    u32 i0;
    s32 i2;
    u32 rec;
    u32 b4;
    u8 bl;
    u8 b9;
    u32 r53;
    s32 c;
    u32 sel;

    self = (void *)a1;

    /* 0x004d89a1 .. 0x004d8a35: twelve thiscall reads, each argument pushed
       immediately before its call.  The dword stores land in the slots the
       listing names; the two byte stores keep only AL. */
    i0 = (u32)cm_fn_00785830(self, 0u);
    i2 = (s32)(u32)cm_fn_00785830(self, 2u);
    f.l18 = (u32)cm_fn_00785830(self, 3u);
    f.l14 = (u32)cm_fn_00785830(self, 4u);
    f.l10 = (u32)cm_fn_00785830(self, 5u);
    f.l00 = (u32)cm_fn_00785830(self, 6u);
    f.l24 = (u32)cm_fn_00785830(self, 7u);
    f.l20 = (u32)cm_fn_00785830(self, 8u);
    f.l07 = (u8)cm_fn_00785830(self, 9u);
    f.l1c = (u32)cm_fn_00785830(self, 0xau);
    f.l0f = (u8)cm_fn_00785830(self, 0xbu);
    f.l28 = (u32)cm_fn_00785830(self, 0xcu);

    /* 0x004d8a39: second stack argument selects the short path. */
    if (a2 == 0u) {
        /* 0x004d939d .. 0x004d943e */
        cm_fn_0053e140(f.l18, f.l14, f.l10, (u32)(void *)f.l30, 0x12cu, 5u,
                       (u32)i2);
        cm_fn_0066f4e0(A32(L_DE1F64), (char *)0x009acb34,
                       (char *)(void *)f.l30);
        cm_fn_004ae660((void *)a3, L_DE1F64);
        r53 = cm_fn_0053e990((u32)G_AE23BC + (u32)i2 * 110u, f.l18, f.l14,
                             f.l10, 0u);
        if (r53 == 0u) {
            return 0u;
        }
        return cm_fn_004ae8a0((int)a3, (u8 *)(void *)f.l30, 0x7d7,
                              (int)((u32)G_AE23BC + (u32)i2 * 110u));
    }

    /* 0x004d8a4c .. 0x004d8a80 */
    bl = 0u;
    f.l76c[0] = 0u;
    f.reason[0][0] = bl;
    f.reason[1][0] = bl;
    f.reason[2][0] = bl;
    f.reason[3][0] = bl;
    f.reason[4][0] = bl;
    f.l2c = f.l00 & 1u;

    /* 0x004d8a80: `je` on the result of `and eax,1`. */
    if ((f.l00 & 1u) != 0u) {
        cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009acb14);
        bl = 1u;
    }

    /* 0x004d8a99: `test al,2` */
    if ((f.l00 & 2u) != 0u) {
        char *fmt;

        if (f.l07 != 0u) {
            fmt = (char *)0x009ad598;
        }
        else if ((f.l00 & 0x01000000u) != 0u) {
            fmt = (char *)0x009acae0;
        }
        else {
            fmt = (char *)0x009acab8;
        }
        cm_fn_0066f4e0(A32(REASON_SLOT(bl)), fmt);
        bl = (u8)(bl + 1u);
    }

    /* 0x004d8b09: each further append is guarded by `cmp bl,5 / jge`. */
    if ((s32)(s8)bl < 5 && (f.l00 & 4u) != 0u) {
        cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009ad568);
        bl = (u8)(bl + 1u);
    }
    if ((s32)(s8)bl < 5 && (f.l00 & 0x100u) != 0u) {
        cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009aca88);
        bl = (u8)(bl + 1u);
    }
    if ((s32)(s8)bl < 5 && (f.l00 & 0x8u) != 0u) {
        cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009ad540);
        bl = (u8)(bl + 1u);
    }
    if ((s32)(s8)bl < 5 && (f.l00 & 0x10u) != 0u) {
        cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009aca64);
        bl = (u8)(bl + 1u);
    }

    /* 0x004d8bcf .. 0x004d8bee: record offset 0x245 * getter(0), kept in the
       listing's ESI (and mirrored into L+0x0008), then the word at
       base + offset + 0x80 tested against 0x1a5e.  The base read is pinned to
       this site (see the G_AE23B4 comment); the offset arithmetic itself has no
       memory access, so forming it before the base read changes nothing. */
    rec = i0 * 0x245u;
    f.l08 = rec;
    if (*(volatile s16 *)(void *)((u32)G_AE23B4 + rec + 0x80u) < 0x1a5e) {
        if ((s32)(s8)bl < 5 && (f.l00 & 0x20000000u) != 0u) {
            cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009ad518);
            bl = (u8)(bl + 1u);
        }
    }

    /* 0x004d8c26: one five-argument text with a record-relative character. */
    if ((s32)(s8)bl < 5 && (f.l00 & 0x20u) != 0u) {
        u32 obj = *(u32 *)(void *)((u32)G_AE23B4 + rec + 0x53u);

        if (obj != 0u) {
            c = *(s8 *)(void *)(obj + 0x37u);
            cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009ad4cc, c, c,
                           (char *)(void *)(obj + 0x4u));
            bl = (u8)(bl + 1u);
        }
    }
    if ((s32)(s8)bl < 5 && (f.l00 & 0x40u) != 0u) {
        cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009ad4a8);
        bl = (u8)(bl + 1u);
    }
    if ((s32)(s8)bl < 5 && (f.l00 & 0x80u) != 0u) {
        cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009ad468);
        bl = (u8)(bl + 1u);
    }
    if ((s32)(s8)bl < 5 && (f.l00 & 0x200u) != 0u) {
        cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009ad444);
        bl = (u8)(bl + 1u);
    }
    if ((s32)(s8)bl < 5 && (f.l00 & 0x4000u) != 0u) {
        cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009ad40c);
        bl = (u8)(bl + 1u);
    }
    if ((s32)(s8)bl < 5 && (f.l00 & 0x400u) != 0u) {
        cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009ad3cc);
        bl = (u8)(bl + 1u);
    }
    /* 0x004d8d7f: the flag is cached in ESI and reused at 0x004d8dec, so bit
       0x800 can produce two entries. */
    if ((s32)(s8)bl < 5 && (f.l00 & 0x800u) != 0u) {
        cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009ad3ac);
        bl = (u8)(bl + 1u);
    }
    if ((s32)(s8)bl < 5 && (f.l00 & 0x2000u) != 0u) {
        cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009ad380);
        bl = (u8)(bl + 1u);
    }
    if ((s32)(s8)bl < 5 && (f.l00 & 0x800u) != 0u) {
        cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009ad360);
        bl = (u8)(bl + 1u);
    }
    /* 0x004d8e1c: byte getter(0xb) and the 0xa-th entry of the 0xae23a8
       array (290 bytes per entry). */
    if ((s32)(s8)bl < 5 && (f.l00 & 0x20000u) != 0u) {
        if (f.l0f != 0u && f.l1c != 0xffffffffu) {
            u32 o = f.l1c * 290u;
            u32 b2 = (u32)G_AE23A8;

            c = *(s8 *)(void *)(b2 + o + 0x52u);
            cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009ad320, c, c,
                           (char *)(void *)(b2 + o + 0x38u));
        }
        else {
            cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009ad314);
        }
        bl = (u8)(bl + 1u);
    }
    if ((s32)(s8)bl < 5 && (f.l00 & 0x400000u) != 0u) {
        cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009ad2e0);
        bl = (u8)(bl + 1u);
    }
    if ((s32)(s8)bl < 5 && (f.l00 & 0x02000000u) != 0u) {
        cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009ad2b8);
        bl = (u8)(bl + 1u);
    }
    if ((s32)(s8)bl < 5 && (f.l00 & 0x04000000u) != 0u) {
        cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009ad284);
        bl = (u8)(bl + 1u);
    }
    if ((s32)(s8)bl < 5 && (f.l00 & 0x00200000u) != 0u) {
        cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009ad248);
        bl = (u8)(bl + 1u);
    }
    /* 0x004d8f75: needs bit 0x40000 and the low bit of getter(6) clear. */
    if ((s32)(s8)bl < 5 && (f.l00 & 0x40000u) != 0u && f.l2c == 0u) {
        cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009ad224);
        bl = (u8)(bl + 1u);
    }
    /* 0x004d8faf: only the low byte of getter(0xc) is tested. */
    if ((s32)(s8)bl < 5 && (f.l28 & 0x80u) != 0u) {
        cm_fn_0066f4e0(A32(REASON_SLOT(bl)), (char *)0x009ad1d8);
        bl = (u8)(bl + 1u);
    }

    /* 0x004d8fd9: `movsx edi,bl; lea eax,[edi-1]; cmp eax,4; ja` then the
       dword jump table at 0x004d9444 (five entries, 0x004d8fef .. 0x004d90aa). */
    switch ((s32)(s8)bl) {
    case 1:                                       /* 0x004d8fef */
        cm_fn_00944c9f((u32)(void *)f.l76c, 0x00991944u,
                       (u32)(void *)f.reason[0]);
        break;
    case 2:                                       /* 0x004d9011 */
        cm_fn_0066f4e0(A32(f.l76c), (char *)0x009a9f28,
                       (char *)(void *)f.reason[0],
                       (char *)(void *)f.reason[1]);
        break;
    case 3:                                       /* 0x004d903b */
        cm_fn_0066f4e0(A32(f.l76c), (char *)0x009ad1a0,
                       (char *)(void *)f.reason[0],
                       (char *)(void *)f.reason[1],
                       (char *)(void *)f.reason[2]);
        break;
    case 4:                                       /* 0x004d906a */
        /* Signed remainder by two, exactly as 0x004d906a..0x004d9077. */
        sel = (u32)i2 & 0x80000001u;
        if ((s32)sel < 0) {
            sel = ((sel - 1u) | 0xfffffffeu) + 1u;
        }
        cm_fn_0066f4e0(A32(f.l76c), (char *)0x009ad158,
                       (char *)(void *)((u32)(void *)f.reason[0]
                                        + 250u * sel),
                       (char *)(void *)((u32)(void *)f.reason[2]
                                        + 250u * sel));
        break;
    case 5:                                       /* 0x004d90aa */
        cm_fn_0066f4e0(A32(f.l76c), (char *)0x009ad100);
        break;
    default:
        break;
    }

    /* 0x004d90bf: two fills of the 300-byte buffers.  The listing reloads
       L+0x0010, L+0x0014, L+0x0018 for both calls. */
    cm_fn_0053e140(f.l18, f.l14, f.l10, (u32)(void *)f.l30, 0x12cu, 1u,
                   (u32)i2);
    cm_fn_0053e140(f.l18, f.l14, f.l10, (u32)(void *)f.l640, 0x12cu, 5u,
                   (u32)i2);

    /* 0x004d9103 */
    if (bl == 0u) {
        /* 0x004d92ad */
        b4 = G_AE23B4; /* 0x004d92ad */
        c = *(s8 *)(void *)(b4 + f.l08 + 0x37u);
        cm_fn_0066f4e0(A32(L_DE1F64), (char *)0x009acba4, c, c,
                       (char *)(void *)f.l30,
                       (char *)(void *)(b4 + f.l08 + 0x4u));
    }
    else {
        /* 0x004d910b: `mov al,[esp+0x17]` caches the L+0x0007 byte across a
           signed remainder by three of getter(2), then compares the reason
           count against that remainder plus two. */
        b9 = f.l07;
        if ((s32)(s8)bl > i2 % 3 + 2) {
            if (b9 != 0u && (s32)(f.l00 - (u32)i2) % 3 != 0) {
                b4 = G_AE23B4; /* 0x004d9139 */
                c = *(s8 *)(void *)(b4 + f.l08 + 0x37u);
                if ((s32)(s8)bl < 4) {
                    /* 0x004d9175, nine arguments */
                    cm_fn_0066f4e0(A32(L_DE1F64), (char *)0x009ad020, 0x64, c,
                                   c, (char *)(void *)f.l30,
                                   (char *)(void *)(b4 + f.l08
                                                    + 0x4u),
                                   (char *)(void *)f.l640,
                                   (char *)(void *)f.l76c);
                }
                else {
                    /* 0x004d91a2, eight arguments */
                    cm_fn_0066f4e0(A32(L_DE1F64), (char *)0x009acf20, 0x64, c,
                                   c, (char *)(void *)f.l30,
                                   (char *)(void *)(b4 + f.l08
                                                    + 0x4u),
                                   (char *)(void *)f.l640);
                }
            }
            else {
                /* 0x004d91af, seven arguments */
                b4 = G_AE23B4; /* 0x004d91af */
                c = *(s8 *)(void *)(b4 + f.l08 + 0x37u);
                cm_fn_0066f4e0(A32(L_DE1F64), (char *)0x009ace88, c, c,
                               (char *)(void *)f.l30,
                               (char *)(void *)(b4 + f.l08 + 0x4u),
                               (char *)(void *)f.l76c);
            }
        }
        else {
            /* 0x004d91e9 */
            if (b9 != 0u && (s32)(f.l00 - (u32)i2) % 3 == 0) {
                b4 = G_AE23B4; /* 0x004d9203 */
                c = *(s8 *)(void *)(b4 + f.l08 + 0x37u);
                if ((s32)(s8)bl < 4) {
                    /* 0x004d923f, nine arguments */
                    cm_fn_0066f4e0(A32(L_DE1F64), (char *)0x009acdb0, 0x64, c,
                                   c, (char *)(void *)f.l30,
                                   (char *)(void *)(b4 + f.l08
                                                    + 0x4u),
                                   (char *)(void *)f.l640,
                                   (char *)(void *)f.l76c);
                }
                else {
                    /* 0x004d926c, eight arguments */
                    cm_fn_0066f4e0(A32(L_DE1F64), (char *)0x009acca4, 0x64, c,
                                   c, (char *)(void *)f.l30,
                                   (char *)(void *)(b4 + f.l08
                                                    + 0x4u),
                                   (char *)(void *)f.l640);
                }
            }
            else {
                /* 0x004d9276, seven arguments */
                b4 = G_AE23B4; /* 0x004d9276 */
                c = *(s8 *)(void *)(b4 + f.l08 + 0x37u);
                cm_fn_0066f4e0(A32(L_DE1F64), (char *)0x009acc1c, c, c,
                               (char *)(void *)f.l30,
                               (char *)(void *)(b4 + f.l08 + 0x4u),
                               (char *)(void *)f.l76c);
            }
        }
    }

    /* 0x004d92da: third stack argument becomes the receiver for the rest. */
    cm_fn_004ae660((void *)a3, L_DE1F64);
    if (f.l24 != 0u) {
        if (f.l20 == 1u) {
            cm_fn_0066f4e0(A32(L_DE1F64), (char *)0x009acb84);
        }
        else {
            cm_fn_0066f4e0(A32(L_DE1F64), (char *)0x009acb60);
        }
        cm_fn_004ae770(a3, L_DE1F64);
    }
    b4 = G_AE23B4; /* 0x004d9321: one snapshot feeds both arguments. */
    rec = b4 + f.l08;
    cm_fn_004ae8a0((int)a3,
                   (u8 *)(void *)(rec + 0x4u), 0x7d5, (int)rec);
    r53 = cm_fn_0053e990((u32)G_AE23BC + (u32)i2 * 110u, f.l18, f.l14, f.l10,
                         0u);
    if (r53 == 0u) {
        return 0u;
    }
    return cm_fn_004ae8a0((int)a3, (u8 *)(void *)f.l30, 0x7d7,
                          (int)((u32)G_AE23BC + (u32)i2 * 110u));
}
