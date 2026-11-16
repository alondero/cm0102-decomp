/* fn_00769380 -- VA 0x00769380, 1378 bytes, award/nation_awards.cpp.
    Preferred image base 0x00400000.
    CM0102 3.9.68 SHA-256:
    0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   x87 / ST0 contract (why this record was held).  The original keeps the
   per-record score in memory as a float but performs every transfer, compare,
   divide and multiply in x87 extended precision, and it consumes two
   ST0-returning calls plus three `call 0x00945640` (__ftol) lowers.  All of
   those physical contracts are expressible in plain C, with no assembly, no
   FPU control-word change and without replacing any callee:

     0x00769596 / 0x007695c9  cm_fn_007c1e30 hands its result back in ST0.  It
        is declared `long double` (the i386 x87 80-bit type), so the value
        arrives in ST0 and stays exact through any 80-bit spill.
     0x0076959b  `fst dword ptr [edi]` -> *(float *)outp = (float)v; a store
        *without* pop, so the value is still live in ST0 afterwards.
     0x007695a2  `fcomp dword ptr [0x968970]` therefore compares the unrounded
        long double: the bl==2 guard below tests `v`, never the stored float.
     0x007695ce, 0x007696e7, 0x00769789  `call 0x00945640` (__ftol) are MSVC's
        lowering of a floating-point-to-int64 cast (ADR 0006); each is written
        as a `(long long)` cast of the long double that the listing leaves in
        ST0, then narrowed exactly as the listing reads it (EAX low dword at
        0x007696ee / 0x00769792, AX low word at 0x007695d6).
     0x007695e8 fild/fdivr/fstp -> (long double) division, stored back as float
        (the float is widened exactly, so the quotient is the original one).
     0x00769657 fild/fmul/fstp -> (long double) multiply, stored back as float.
     0x007696cc fld/fcomp qword, 0x007697b5 fsub qword -> long double compare
        and subtract against the original double operands.

   The four floating-point operands that drive control flow are read from
   their original addresses (0x00968970, 0x009689a0, 0x00967880, 0x0096af88)
   instead of re-typed literals. */

#include "cmre.h"

/* ---- explicit original callees (unchanged boundaries) ------------------- */
extern u32 CM_CDECL cm_fn_005481a0(volatile u8 *object);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern long double CM_THISCALL cm_fn_007c1e30(u32 self, u32 player, u32 category,
                                              u32 field, float incoming);
/* 0x0076941b: three arguments are pushed for this call and the single
   `add esp, 0x24` at 0x00769440 accounts for them, so 0x0090d130 is the
   three-argument callee (destination, source, count) whose EAX feeds
   0x00944c9f. */
extern char *cm_fn_0090d130(char *destination, const char *source, u32 count);
/* 0x007698c2 `mov ecx, [esp + 0x2a8]` is the receiver of the call at
   0x007698d0; the two remaining pushes (0x007698cd, 0x007698cf) are its stack
   arguments, so the declaration keeps `this` first. */
extern void CM_THISCALL cm_fn_009302f0(int param_1, u16 *param_2, int param_3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                   void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
/* 0x007696a8 and 0x007696bf each clean exactly 0x10 bytes for this callee, so
   the original ABI is four stack arguments; the record's extra EAX/EDX inputs
   are register state, not pushed arguments, and are left to the caller's
   registers as in the original. */
extern unsigned long long CM_CDECL cm_fn_00945333(volatile u32 base_arg,
                                                  volatile u32 count_arg,
                                                  volatile u32 width_arg,
                                                  volatile u32 compare_arg);

/* ---- original globals --------------------------------------------------- */
/* reloaded inside the record loops by the original, so declared volatile */
#define DAT_00adadfc CM_GLOBAL(volatile u32, 0x00adadfc)
#define DAT_00ae2364 CM_GLOBAL(volatile s32, 0x00ae2364)
#define DAT_00b74350 CM_GLOBAL(volatile u32, 0x00b74350)
#define DAT_00ae23a8 CM_GLOBAL(u32, 0x00ae23a8) /* segment array base */
#define DAT_00ae23bc CM_GLOBAL(u32, 0x00ae23bc) /* record array base (dword) */
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
/* original floating-point operands */
#define DAT_00968970 CM_GLOBAL(float, 0x00968970)
#define DAT_009689a0 CM_GLOBAL(double, 0x009689a0)
#define DAT_00967880 CM_GLOBAL(double, 0x00967880)
#define DAT_0096af88 CM_GLOBAL(float, 0x0096af88)

/* The original stack frame after `sub esp, 0x290` (L).  Every member keeps the
   listing's byte offset from L; the two scratch buffers at L+0xa0 and L+0x1a0
   stay in the same allocation, and the output record block at L+0x56 is 2
   bytes off a dword boundary exactly as in the original, hence `packed`. */
typedef struct __attribute__((packed)) {
    u8  pad_00[0x13];
    s8  nsel;      /* 0x13  count of selected records                */
    volatile union {
        u32 word;
        u8 byte[4];
    } category;    /* 0x14: only byte[0] is initialized */
    float rate;    /* 0x18  search rate                              */
    s32 accepted;  /* 0x1c  accepted record count                    */
    u32 buf;       /* 0x20  allocated record buffer                  */
    s32 skipped;   /* 0x24  per-rate skip counter                    */
    u32 p5;        /* 0x28  filter 3 (from argument 5)               */
    u32 p6;        /* 0x2c  filter 4 (from argument 6)               */
    s32 idx;       /* 0x30  record index                             */
    u32 p3;        /* 0x34  filter 1 (from argument 3)               */
    u32 p4;        /* 0x38  filter 2 (from argument 4)               */
    u32 picked[3]; /* 0x3c  selected record pointers                 */
    u32 xv[3];     /* 0x48  selected record scores (int)             */
    u16 hdr;       /* 0x54  output header word                       */
    u32 out[18];   /* 0x56  3 records x 6 dwords                     */
    u8  pad_9e[2];
    u8  bufa[0x100]; /* 0xa0 */
    u8  bufb[0xf0];  /* 0x1a0 */
} frame_t;

void CM_STDCALL cm_fn_00769380(u32 param_1, u32 param_2, s32 param_3, s32 param_4,
                               s32 param_5, s32 param_6, s8 param_7, u16 param_8,
                               u32 param_9)
{
    frame_t L;
    u8  bl = (u8)param_9; /* the listing works on BL throughout */
    u32 catv;
    u32 obj;
    u32 c;
    s32 i;
    u32 off;

    /* 0x00769391..0x007693c0 */
    L.p3 = 0;
    L.p4 = 0;
    L.p5 = 0;
    L.p6 = 0;
    L.accepted = 0;
    L.nsel = 0;
    L.rate = 0.66f;
    L.skipped = 0;
    if (bl == 5 || bl == 2) {
        L.rate = 0.01f;
    }

    /* 0x007693ca..0x007693e5: size = count * 10 (32-bit wrapping) */
    L.buf = (u32)cm_fn_00944e46((u32)DAT_00ae2364 * 10u);
    if (L.buf == 0) {
        /* 0x007693e7..0x00769453 */
        char *msg;

        cm_fn_00944cff(0x00a5b26cu, 0u, 0u, (void *)(L.bufb), (void *)(L.bufa));
        msg = cm_fn_0090d130((char *)(L.bufb), (char *)(L.bufa), 0x4fcu);
        /* The original leaves the three 0x0090d130 argument dwords on the
           stack and pushes three more here, so 0x00944c9f sees six stack
           dwords; the same six are passed so its format sees the original
           operands whichever way 0x0090d130 consumes them. */
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)msg, (void *)(L.bufb),
                       (void *)(L.bufa), 0x4fcu);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        DAT_00b67a34 = 0;
        return;
    }

    /* 0x00769456..0x007694c8: four optional segment filters */
    {
        u32 seg = DAT_00ae23a8;

        if (param_3 > -1) {
            L.p3 = seg + (u32)param_3 * 0x122u;
        }
        if (param_4 > -1) {
            L.p4 = seg + (u32)param_4 * 0x122u;
        }
        if (param_5 > -1) {
            L.p5 = seg + (u32)param_5 * 0x122u;
        }
        if (param_6 > -1) {
            L.p6 = seg + (u32)param_6 * 0x122u;
        }
    }

    /* 0x007694c8..0x007694e9 */
    L.idx = 0;
    {
        /* setne al; dec eax; and al, 0xfc; add eax, 6 -> 6 or 2 */
        u8 a = (u8)(((u32)(bl != 2) - 1u) & 0xffu);

        a = (u8)(a & 0xfc);
        a = (u8)(a + 6u);
        L.category.byte[0] = a;
    }

    if (DAT_00ae2364 > 0) {
        /* 0x007694de writes only the low byte; 0x007694ef loads all four.
           Preserve the unknown upper bytes, not a zero-extension. EBP keeps
           this value for both pushes at 0x0076958e and 0x007695c7. */
        catv = L.category.word;
        u32 rec = DAT_00ae23bc;
        u8 *outp = (u8 *)(u32)(L.buf + 4u);

        L.idx = 0;
        do {
            u32 o61 = *(volatile u32 *)(u32)(rec + 0x61u);
            int keep = 0;

            /* 0x007694f6..0x0076957c */
            if (o61 != 0) {
                c = *(u32 *)(u32)(rec + 0x39u);
                keep = 1;
                if (c != 0 && DAT_00adadfc != 0) {
                    u32 a57 = *(u32 *)(u32)(c + 0x57u);

                    if (a57 == 0) {
                        keep = 0;
                    } else if (*(u32 *)(u32)(DAT_00adadfc + *(u32 *)(u32)a57 * 4u) == 0) {
                        keep = 0;
                    }
                }
                if (keep && L.p3 != 0) {
                    if (c == 0 || *(u32 *)(u32)(c + 0x53u) != L.p3) {
                        keep = 0;
                    }
                }
                if (keep && L.p4 != 0) {
                    if (*(u32 *)(u32)(rec + 0x1au) != L.p4) {
                        keep = 0;
                    }
                }
                if (keep && L.p5 != 0) {
                    if (*(u32 *)(u32)(rec + 0x1au) == L.p5) {
                        keep = 0;
                    }
                }
                if (keep && L.p6 != 0) {
                    if (*(u32 *)(u32)(rec + 0x1au) != L.p6) {
                        if (c == 0 || *(u32 *)(u32)(c + 0x53u) != L.p6) {
                            keep = 0;
                        }
                    }
                }
            }

            if (keep) {
                /* 0x00769582..0x007695ad */
                long double v;
                u32 self;

                *(volatile u32 *)(u32)(outp - 4) = rec;
                /* 0x00769585 reloads after the possibly overlapping store;
                   0x00769590 then reads the receiver. */
                o61 = *(volatile u32 *)(u32)(rec + 0x61u);
                self = DAT_00b74350;
                v = cm_fn_007c1e30(self, o61, catv, param_9, 6.5f);
                *(volatile float *)(u32)outp = (float)v; /* 0x0076959b FST */
                /* 0x007695a2..0x007695ad: C3 skips equal OR unordered.
                   Continue only for an ordered, unequal extended value. */
                if (bl != 2 || __builtin_islessgreater(v, (long double)DAT_00968970)) {
                    /* 0x007695b7..0x007695d6 */
                    long long q;
                    o61 = *(volatile u32 *)(u32)(rec + 0x61u);
                    self = DAT_00b74350; /* 0x007695ba, after the field reload */
                    q = (long long)cm_fn_007c1e30(self, o61, catv,
                                                            1u, 6.5f);

                    *(u16 *)(u32)(outp + 4) = (u16)q;
                    if (bl == 3) {
                        /* 0x007695dc..0x007695f0 */
                        s16 n = (s16)(u16)q;

                        if (n > 0) {
                            long double d = (long double)*(float *)(u32)outp;

                            *(float *)(u32)outp = (float)(d / (long double)n);
                        } else {
                            *(u32 *)(u32)outp = 0;
                        }
                    }
                    if (bl != 2) {
                        /* 0x007695ff..0x0076965d */
                        u32 o39 = *(u32 *)(u32)(rec + 0x39u);
                        s32 v2;
                        s16 dx;
                        s32 h;

                        if (o39 == 0 || *(u32 *)(u32)(o39 + 0x57u) == 0) {
                            v2 = 1;
                        } else {
                            u32 p57 = *(u32 *)(u32)(o39 + 0x57u);
                            s32 a = (s32)*(s16 *)(u32)(p57 + 0x69u);
                            s32 b = (s32)*(s16 *)(u32)(o39 + 0x80u);
                            s32 qq = b / 500;
                            s32 s = (s32)((u32)qq + (u32)((u32)a * 4u));

                            v2 = s / 5;
                        }
                        dx = (s16)v2;
                        h = (s32)dx / 2;
                        *(float *)(u32)outp =
                            (float)((long double)h * (long double)*(float *)(u32)outp);
                    }
                    L.accepted = L.accepted + 1;
                    outp = outp + 10;
                }
            }

            /* 0x0076966b..0x0076967f */
            L.idx = L.idx + 1;
            rec = rec + 0x6eu;
        } while (L.idx < DAT_00ae2364);
    }

    if (L.accepted != 0) {
        /* 0x00769691..0x007696c2 */
        if (bl == 3) {
            cm_fn_00945333(L.buf, (volatile u32)L.accepted, 10u, 0x0076a480u);
        } else {
            cm_fn_00945333(L.buf, (volatile u32)L.accepted, 10u, 0x0076a4b0u);
        }

        for (;;) {
            long long k;
            s32 thresh;

            /* 0x007696c8..0x007696d7 */
            if (!(L.rate > (double)DAT_009689a0)) {
                break;
            }
            /* 0x007696dd..0x007696ee */
            k = (long long)((long double)L.rate * (long double)DAT_0096af88);
            thresh = (s32)(u32)k;

            i = 0;
            off = 4u;
            while (i < L.accepted) {
                int skip = 0;

                /* 0x00769701..0x0076972a */
                obj = *(u32 *)(u32)(L.buf + off - 4u);
                if (param_8 != 0) {
                    u32 rv = cm_fn_005481a0((volatile u8 *)(u32)obj);

                    if ((u16)(rv & (u32)param_8) == 0) {
                        skip = 1;
                    } else if (param_8 == 4 && (rv & 2u) != 0) {
                        skip = 1;
                    }
                }
                /* 0x0076972c..0x00769743 */
                if (!skip) {
                    s8 cl = L.nsel;
                    s32 j;

                    for (j = 0; j < (s32)cl; j++) {
                        if (L.picked[j] == obj) {
                            skip = 1;
                            break;
                        }
                    }
                }
                /* 0x00769745..0x0076974d */
                if (!skip) {
                    if ((s32)*(s16 *)(u32)(L.buf + off + 4u) < thresh) {
                        skip = 1;
                    }
                }
                /* 0x0076974f..0x0076976f */
                if (!skip) {
                    u8 p7 = (u8)param_7;

                    if (p7 == 0xffu) {
                        /* 0x00769771: take without the skip test */
                    } else if ((s32)*(s8 *)(u32)(obj + 0x18u) > (s32)(s8)p7) {
                        L.skipped = L.skipped + 1;
                        skip = 1;
                    } else if (L.skipped < 3) {
                        skip = 1;
                    }
                }
                /* 0x00769771..0x0076979f */
                if (!skip) {
                    s32 cnt = (s32)L.nsel;

                    L.picked[cnt] = obj;
                    if ((u8)param_9 == 2u) {
                        long long tv = (long long)*(float *)(u32)(L.buf + off);

                        L.xv[cnt] = (u32)tv;
                    }
                    L.nsel = (s8)(cnt + 1);
                    if (L.nsel == 3) {
                        goto rate_step;
                    }
                }
                /* 0x007697a1..0x007697ab */
                i = i + 1;
                off = off + 10u;
            }
        rate_step:
            /* 0x007697b5..0x007697c6 */
            L.rate = (float)((long double)L.rate - (long double)DAT_00967880);
            if (L.nsel != 3) {
                continue;
            }
            break;
        }

        /* 0x007697cc..0x007697df */
        cm_fn_009452ca(L.buf);
        if (L.nsel != 3) {
            return;
        }

        /* 0x007697e5..0x007698bc */
        L.hdr = *(const u16 *)(u32)(param_1 + 2u);
        for (i = 0; i < 3; i++) {
            u32 o = L.picked[i];
            u32 a4 = *(u32 *)(u32)(o + 4u);
            u32 a8 = *(u32 *)(u32)(o + 8u);
            u32 ac = *(u32 *)(u32)(o + 0xcu);
            u32 a39 = *(u32 *)(u32)(o + 0x39u);

            L.out[i * 6 + 0] = (a4 != 0) ? *(u32 *)(u32)(a4 + 0x33u) : 0xffffffffu;
            L.out[i * 6 + 1] = (a8 != 0) ? *(u32 *)(u32)(a8 + 0x33u) : 0xffffffffu;
            L.out[i * 6 + 2] = (ac != 0) ? *(u32 *)(u32)(ac + 0x33u) : 0xffffffffu;
            L.out[i * 6 + 3] = *(u32 *)(u32)o;
            L.out[i * 6 + 4] = L.xv[i];
            L.out[i * 6 + 5] = (a39 != 0) ? *(u32 *)(u32)a39 : 0xffffffffu;
        }

        /* 0x007698c2..0x007698d0 */
        cm_fn_009302f0((int)param_2, (u16 *)(void *)((u8 *)(void *)&L + 0x54), 1);
    }
    return;
}
