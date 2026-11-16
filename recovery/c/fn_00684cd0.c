/* fn_00684cd0 -- comp/league.cpp, preferred image base 0x00400000, image 3.9.68.
   Plain-C reconstruction from the original instructions (listing in the job
   bundle). `this` arrives in ECX; `ret` pops no stack bytes.

   HELD concern resolved (REP STOSD 0x00684da5 / REP STOSB 0x00684dae and DF):
   the routine never executes STD and the x86-32 Windows calling convention
   requires DF clear on entry and exit of every function (MSVC emits CLD before
   any REP that must ascend and never returns with DF set). DF is therefore 0
   at both REP sites, so both stores ascend from the destination pointer:
   n>>2 dwords of 0xffffffff followed by n&3 bytes of 0xff, starting at the
   allocation. The forward C loops below reproduce exactly that, with the
   listing's logical shift for the dword count and the 3-bit tail. The fixture
   seeds carry EFLAGS=0x202 (DF clear) as well. */

#include "cmre.h"

#define G_009cf45c  CM_GLOBAL(u32, 0x009cf45c)
#define G_009cf6e8  CM_GLOBAL(u32, 0x009cf6e8)
#define G_009cf6f0  CM_GLOBAL(u32, 0x009cf6f0)
#define G_009cfa10  CM_GLOBAL(u32, 0x009cfa10)
#define G_00ae2d14  CM_GLOBAL(u32, 0x00ae2d14)
#define G_00ae1050  CM_GLOBAL(u32, 0x00ae1050)
#define G_00b67a34  CM_GLOBAL(u32, 0x00b67a34)
#define G_00de1f58  CM_GLOBAL(u32, 0x00de1f58)

extern u32 CM_CDECL cm_fn_004b0700(u32 v);
/* 0x00685232 MOV EDX,[ESI+4]; 0x00685235 MOV EAX,[EAX];
   0x00685237 MOV ECX,[0x00ae1050]; then two stack pushes and CALL 0x004cd030,
   whose RET8 removes them. EAX/EDX/ECX are the first three formals. */
extern u32 CM_STDCALL __attribute__((regparm(3)))
cm_fn_004cd030(u32 incoming_eax, u32 unused_edx, u8 *self, u8 *param_1,
               u32 param_2);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
/* 0x00684d16 sets ECX=this and 0x00684d14 pushes one stack argument; no caller
   cleanup follows the call, so the callee pops those four bytes (ret 4). */
extern u32 CM_THISCALL cm_fn_006827d0(u32 self, u32 param_1);
/* 0x006853ef loads ECX from 0x00de1f58; three stack arguments are pushed. */
extern void CM_THISCALL cm_fn_00905c60(u32 self, u32 key, u32 first, u32 second);
extern char * CM_CDECL cm_fn_0090d130(u32 arg1, u32 arg2, u32 arg3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                   void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

/* Virtual slots called with ECX=this and five stack arguments; the callees pop
   those twenty bytes (no `add esp` follows any of the calls). */
typedef void (CM_THISCALL *cm_vfn5)(u32 self, u32 a1, u32 a2, u32 a3, u32 a4,
                                    u32 a5);

void CM_FASTCALL cm_fn_00684cd0(int *param_1)
{
    u8 *obj = (u8 *)param_1;
    u8 *buf;                 /* [esp+0x14] while esi/edi are saved: the allocation */
    u8 l2l3[2];              /* [esp+0x12] = order counter, [esp+0x13] = index-1 */
    u32 cntw;                /* [esp+0x14] later: 4 per-pass counts, byte-wise */
    u8 *counts = (u8 *)&cntw;
    u8 scratch[512];         /* [esp+0x1c] and [esp+0x11c] error-format buffers */
    u8 *bufLo = scratch;
    u8 *bufHi = scratch + 0x100;
    u32 i;

    /* 0x00684cda */
    l2l3[1] = 0xff;

    /* 0x00684cdf..0x00684cff */
    if (*(s16 *)(obj + 0xa7) == -1) {
        s16 ax = *(s16 *)(obj + 0x3e);
        if (ax > 0) {
            if (*(u32 *)(obj + 0x45) != 0) {
                /* dec ax ; imul ax, word ptr [esi+0x3c] */
                *(s16 *)(obj + 0xa7) =
                    (s16)(u16)((u16)(ax - 1) * *(u16 *)(obj + 0x3c));
            }
        }
    }

    /* 0x00684d06 */
    if (*(u32 *)(obj + 0xdd) == 0) {
        u32 bv;

        /* 0x00684d14..0x00684d18 */
        cm_fn_006827d0((u32)obj, 0);

        /* 0x00684d1d..0x00684d32 */
        bv = (u32)cm_fn_00944e46((u32)(s32)*(s16 *)(obj + 0x3e));
        buf = (u8 *)bv;
        if (bv == 0) {
            /* 0x00684d34..0x00684d92: allocation failure report.  The three
               arguments pushed for cm_fn_0090d130 are never popped before the
               next call (they are released by the single `add esp,0x24` at
               0x00684d87), so they also occupy cm_fn_00944c9f's varargs list:
               (dest, fmt, file, bufHi, bufLo, 0x7c9). */
            cm_fn_00944cff(0x009f15f4u, bv, bv, bufHi, bufLo);
            bv = (u32)cm_fn_0090d130((u32)bufHi, (u32)bufLo, 0x7c9u);
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, bv, (u32)bufHi,
                           (u32)bufLo, 0x7c9u);
            cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
            G_00b67a34 = (u32)buf;
            return;
        }

        /* 0x00684d99..0x00684db0: fill the n-byte block with 0xff.
           REP STOSD (n>>2, logical shift) then REP STOSB (n&3); DF is clear. */
        {
            u32 nn = (u32)(s32)*(s16 *)(obj + 0x3e);   /* movsx ecx */
            u32 c = nn >> 2;
            u8 *p = buf;

            while (c != 0) {
                *(u32 *)p = 0xffffffffu;
                p += 4;
                c--;
            }
            c = nn & 3;
            while (c != 0) {
                *p = 0xff;
                p++;
                c--;
            }
        }

        /* 0x00684db0..0x00684dc0 */
        cntw = *(u32 *)(obj + 0xbe);

        /* 0x00684dc4..0x00684e02: first non-special record sets the index-1 */
        if (*(u8 *)(obj + 0xd9) & 1) {
            s16 di = *(s16 *)(obj + 0x3e);
            s16 cx = 0;
            if (di > 0) {
                while (1) {
                    u32 base = *(u32 *)(obj + 0xb1);
                    s32 e = (s32)(s16)cx;
                    u8 st = *(u8 *)(base + (u32)e * 59u + 0x37);
                    if (st == 0xfe || st == 0xfc || st == 3) {
                        cx = (s16)(cx + 1);
                        if (cx < di) continue;
                        break;
                    }
                    l2l3[1] = (u8)cx;
                    break;
                }
            }
        }

        /* 0x00684e02..0x00684e7b: forward pass over counts[0..1] */
        i = 0;
        l2l3[0] = 0;
        if (*(s16 *)(obj + 0x3e) > 0) {
            for (;;) {
                u8 *cnt;
                s8 cl;

                if ((s8)l2l3[0] > 1) break;
                cnt = &counts[(u8)l2l3[0]];
                cl = (s8)*cnt;
                if (cl != 0) {
                    while ((s16)i < *(s16 *)(obj + 0x3e)) {
                        u32 base = *(u32 *)(obj + 0xb1);
                        u8 st =
                            *(u8 *)(base + (u32)(s32)(s16)i * 59u + 0x37);
                        if (st != 0xfe && st != 0xfc && st != 3) {
                            buf[(u32)(s32)(s16)i] = l2l3[0];
                            cl = (s8)(cl - 1);
                            *cnt = (u8)cl;
                        }
                        cl = (s8)*cnt;
                        i++;
                        if (cl == 0) break;
                    }
                }
                l2l3[0] = (u8)(l2l3[0] + 1);
                if ((s16)i >= *(s16 *)(obj + 0x3e)) break;
            }
        }

        /* 0x00684e7d..0x00684ef7: reverse pass over counts[3..2] */
        {
            s16 di = (s16)(*(s16 *)(obj + 0x3e) - 1);
            l2l3[0] = 3;
            if (di >= 0) {
                for (;;) {
                    u8 *cnt;
                    s8 cl;

                    if ((s8)l2l3[0] < 2) break;
                    cnt = &counts[(u8)l2l3[0]];
                    cl = (s8)*cnt;
                    if (cl != 0) {
                        while (di >= 0) {
                            u32 base = *(u32 *)(obj + 0xb1);
                            u8 st = *(u8 *)(base + (u32)(s32)di * 59u + 0x37);
                            if (st != 0 && st != 1 && st != 5) {
                                buf[(u32)(s32)di] = l2l3[0];
                                cl = (s8)(cl - 1);
                                *cnt = (u8)cl;
                            }
                            cl = (s8)*cnt;
                            di = (s16)(di - 1);
                            if (cl == 0) break;
                        }
                    }
                    l2l3[0] = (u8)((s8)l2l3[0] - 1);
                    if (di < 0) break;
                }
            }
        }

        /* 0x00684ef7..0x006850e2: per-record predecessor counting */
        i = 0;
        if (*(s16 *)(obj + 0x3e) > 0) {
            for (;;) {
                u32 base = *(u32 *)(obj + 0xb1);
                u32 roff = (u32)(s32)(s16)i * 59u;   /* edi = 0x3b * i */
                u8 *rec = (u8 *)(base + roff);

                if (rec[0x39] != rec[0x3a]) {
                    /* 0x00684f2d: `inc al` operates on EAX, the record index, so
                       the stored byte is (u8)(i+1), not a bumped field value. */
                    u8 al = (u8)(i + 1);
                    s32 key;
                    u32 b1;

                    rec[0x39] = al;
                    /* 0x00684f32 MOV ECX,[ESI+0xb1]: the original reloads the
                       record-array pointer after the counter store, so a store
                       aliasing obj+0xb1 changes the base used for the key. */
                    b1 = *(volatile u32 *)(obj + 0xb1);
                    l2l3[0] = al;
                    /* 0x00684f3c..0x00684f5b */
                    key = ((s32)*(s16 *)(obj + 0xa7) -
                           (s32)*(s8 *)(b1 + roff + 0x11)) *
                              (s32)*(s8 *)(obj + 0xc2) +
                          (s32)*(s16 *)(b1 + roff + 0x19);
                    if ((s16)i > 0) {
                        u32 j = 0;            /* ebp = 0, +0x3b per step */
                        u32 rem = i;          /* [esp+0x18] */
                        do {
                            u32 b2 = *(u32 *)(obj + 0xb1);
                            s16 a7 = *(s16 *)(obj + 0xa7);
                            s16 fo = (s16)*(s8 *)(b2 + roff + 0x11);
                            s16 fi = (s16)*(s8 *)(b2 + j + 0x11);
                            s32 v = (s32)*(s16 *)(b2 + j + 0x19);
                            int dec;

                            /* 0x00684f82 -> 0x00684f9d -> 0x00684fb3 */
                            if (fo < a7 && fi < a7) {
                                dec = (key >= v);
                            } else if (fi < a7) {
                                dec = (key == v);
                            } else if (fo < a7) {
                                dec = (key >= v);
                            } else if (key != v) {
                                dec = 0;
                            } else {
                                dec = (*(u32 *)(obj + 0xe6) != 0);
                            }
                            if (dec) {
                                /* 0x00684fd7 */
                                u8 *d = (u8 *)(b2 + roff + 0x39);
                                *d = (u8)(*d - 1);
                            }
                            /* 0x00684fe3 */
                            j += 0x3b;
                            rem--;
                        } while (rem != 0);
                    }
                    /* 0x00684ff9 */
                    rec[0x3a] = l2l3[0];
                    /* 0x00685003..0x006850d3: successors of the new counter */
                    {
                        s32 k = (s32)(s16)(s8)l2l3[0];
                        if (k < (s32)*(s16 *)(obj + 0x3e)) {
                            do {
                                u32 b3 = *(u32 *)(obj + 0xb1);
                                s16 a7b = *(s16 *)(obj + 0xa7);
                                u8 *krec = (u8 *)(b3 + (u32)k * 59u);
                                s16 fk = (s16)*(s8 *)(krec + 0x11);
                                s32 v19 = (s32)*(s16 *)(krec + 0x19);
                                s32 key2 = ((s32)a7b - (s32)fk) *
                                               (s32)*(s8 *)(obj + 0xc2) +
                                           v19;
                                s16 fo2 = (s16)*(s8 *)(b3 + roff + 0x11);
                                s32 v2 = (s32)*(s16 *)(b3 + roff + 0x19);
                                int inc;

                                /* 0x0068505d -> 0x00685076 -> 0x0068508a */
                                if (fo2 < a7b && fk < a7b) {
                                    inc = (key2 >= v2);
                                } else if (fk < a7b) {
                                    /* 0x00685076: fo2>=a7, fk<a7 increments for
                                       key2>=v2 (0x00685086 JL skip). */
                                    inc = (key2 >= v2);
                                } else if (fo2 < a7b) {
                                    /* 0x0068508a: fk>=a7, fo2<a7 increments only
                                       for key2==v2 (0x0068509c JNE skip). */
                                    inc = (key2 == v2);
                                } else if (key2 != v19) {
                                    inc = 0;
                                } else {
                                    inc = (*(u32 *)(obj + 0xe6) != 0);
                                }
                                if (inc) {
                                    /* 0x006850b0 */
                                    u8 *d = (u8 *)(b3 + roff + 0x3a);
                                    *d = (u8)(*d + 1);
                                }
                                /* 0x006850bc */
                                k++;
                            } while (k < (s32)*(s16 *)(obj + 0x3e));
                        }
                    }
                }
                /* 0x006850cf */
                i++;
                if ((s16)i >= *(s16 *)(obj + 0x3e)) break;
            }
        }

        /* 0x006850e2..0x00685485: order fix-up and per-record notification.

           The virtual calls below push bytes with `mov al, ..` / `mov cl, ..`,
           which leave the rest of EAX/ECX untouched; those stale high bits are
           part of the pushed arguments, so eax_v/ecx_v shadow the registers
           exactly as the listing writes them.  Where a virtual callee runs
           first, its leftover ECX is unknowable in plain C (in that case the
           gate has already stopped at that callee); the shadow is reset to 0
           there and never used. */
        i = 0;
        if (*(s16 *)(obj + 0x3e) > 0) {
            s32 want = (s32)(s8)l2l3[1] + 1;

            for (;;) {
                u32 base = *(u32 *)(obj + 0xb1);
                u32 roff = (u32)(s32)(s16)i * 59u;   /* edi = 0x3b * i */
                u8 *rec = (u8 *)(base + roff);
                u32 eax_v = (u32)(base + roff);      /* 0x0068510e..0x00685118 */
                u32 ecx_v = (u32)(s32)(s8)rec[0x3a]; /* 0x0068511a */

                /* 0x0068511e */
                if ((s32)ecx_v == want) {
                    u8 st = rec[0x37];
                    ecx_v = (ecx_v & 0xffffff00u) | rec[0x37];  /* 0x00685122 */
                    if (st != 3 && st != 0xfe && st != 0xfc && st != 5) {
                        u32 vt;
                        /* 0x00685149..0x00685159 */
                        rec[0x37] = 5;
                        eax_v = (eax_v & 0xffffff00u) | *(u8 *)(obj + 0x44);
                        vt = *(u32 *)obj;
                        (*(cm_vfn5 *)(vt + 0x40))((u32)obj, *(u32 *)rec, 5,
                                                  eax_v, 0, 0);
                        ecx_v = 0;
                    }
                } else {
                    /* 0x0068515e */
                    s32 f3a = (s32)(s8)rec[0x3a];
                    u8 c;
                    ecx_v = (ecx_v & 0xffffff00u) |
                            (u32)*(u8 *)((u32)buf + (u32)(f3a - 1));
                    c = (u8)ecx_v;
                    if (c == 0) {
                        u8 st;
                        ecx_v = (ecx_v & 0xffffff00u) | rec[0x37]; /* 0x0068516e */
                        st = (u8)ecx_v;
                        if (st == 0xff || st == 1) {
                            u16 fl = *(u16 *)(obj + 0xd9);
                            if (fl & 2) {
                                if (st != 0) {
                                    u32 vt;
                                    /* 0x006851a5..0x006851b5 */
                                    rec[0x37] = 0;
                                    eax_v = (eax_v & 0xffffff00u) |
                                            *(u8 *)(obj + 0x44);
                                    vt = *(u32 *)obj;
                                    (*(cm_vfn5 *)(vt + 0x40))(
                                        (u32)obj, *(u32 *)rec, 0, eax_v, 0, 0);
                                    ecx_v = 0;
                                }
                            } else if (fl & 4) {
                                if (st != 7) {
                                    u32 vt;
                                    /* 0x006851c8..0x006851d8 */
                                    rec[0x37] = 7;
                                    eax_v = (eax_v & 0xffffff00u) |
                                            *(u8 *)(obj + 0x44);
                                    vt = *(u32 *)obj;
                                    (*(cm_vfn5 *)(vt + 0x40))(
                                        (u32)obj, *(u32 *)rec, 7, eax_v, 0, 0);
                                    ecx_v = 0;
                                }
                            } else if (fl & 8) {
                                if (st != 6) {
                                    u32 vt;
                                    /* 0x006851ef..0x006851ff */
                                    rec[0x37] = 6;
                                    eax_v = (eax_v & 0xffffff00u) |
                                            *(u8 *)(obj + 0x44);
                                    vt = *(u32 *)obj;
                                    (*(cm_vfn5 *)(vt + 0x40))(
                                        (u32)obj, *(u32 *)rec, 6, eax_v, 0, 0);
                                    ecx_v = 0;
                                }
                            }
                        }
                    } else if (c == 1) {
                        if (rec[0x37] == 0xff) {
                            u32 vt;
                            /* 0x00685209..0x00685223 */
                            rec[0x37] = 1;
                            eax_v = (eax_v & 0xffffff00u) |
                                    *(u8 *)(obj + 0x44);
                            vt = *(u32 *)obj;
                            (*(cm_vfn5 *)(vt + 0x40))((u32)obj, *(u32 *)rec, 1,
                                                      eax_v, 0, 0);
                            ecx_v = 0;
                        }
                    } else {
                        s32 f39 = (s32)(s8)rec[0x39];
                        u8 c2;
                        ecx_v = (u32)(s32)(s8)rec[0x39];        /* 0x00685225 */
                        ecx_v = (ecx_v & 0xffffff00u) |
                                (u32)*(u8 *)((u32)buf + (u32)(f39 - 1));
                        c2 = (u8)ecx_v;
                        if (c2 == 3) {
                            u32 f0;
                            u32 r;
                            /* 0x00685232..0x0068523f */
                            eax_v = *(u32 *)rec;      /* 0x00685235 */
                            f0 = eax_v;
                            /* 0x00685237 loads the pointer stored at
                               0x00ae1050 into ECX; passing the literal address
                               would change the callee's accesses. */
                            r = cm_fn_004cd030(f0, *(u32 *)(obj + 4),
                                               (u8 *)G_00ae1050, (u8 *)f0,
                                               *(u32 *)(obj + 4));
                            /* 0x00685244..0x0068526d */
                            {
                                u32 b4 = *(u32 *)(obj + 0xb1);
                                u8 st;
                                ecx_v = b4;                       /* 0x00685244 */
                                eax_v = (r & 0xffffff00u) |
                                        *(u8 *)(b4 + roff + 0x37);
                                st = (u8)eax_v;
                                if (st == 0xff || st == 2 || st == 0xfc) {
                                    u32 vt;
                                    *(u8 *)(b4 + roff + 0x37) = 3;
                                    eax_v = (eax_v & 0xffffff00u) |
                                            *(u8 *)(obj + 0x44);
                                    vt = *(u32 *)obj;
                                    (*(cm_vfn5 *)(vt + 0x40))(
                                        (u32)obj, *(u32 *)rec, 3, eax_v, 0, 0);
                                    ecx_v = 0;
                                }
                            }
                        } else if (c2 == 2) {
                            u8 st = rec[0x37];
                            if (st == 0xff || st == 0xfe) {
                                u32 vt;
                                /* 0x00685281..0x0068528f */
                                rec[0x37] = 2;
                                eax_v = (eax_v & 0xffffff00u) |
                                        *(u8 *)(obj + 0x44);
                                vt = *(u32 *)obj;
                                (*(cm_vfn5 *)(vt + 0x40))(
                                    (u32)obj, *(u32 *)rec, 2, eax_v, 0, 0);
                                ecx_v = 0;
                            }
                        }
                    }
                }

                /* 0x006852a0: clear a record whose order window collapsed */
                if (*(u8 *)(obj + 0xd9) & 0x20) {
                    u32 b5 = *(u32 *)(obj + 0xb1);
                    s32 d0 = (s32)*(s8 *)(obj + 0xc0);
                    u8 alv;
                    u8 *recp;
                    s32 av;
                    s32 lim;

                    ecx_v = b5;                             /* 0x006852ad */
                    alv = *(u8 *)(b5 + roff + 0x39);        /* 0x006852ba */
                    recp = (u8 *)(b5 + roff);                /* 0x006852be */
                    ecx_v = (u32)(b5 + roff);
                    av = (s32)(s8)alv;
                    lim = (s32)*(s8 *)(obj + 0xbe) + d0;
                    if (av > lim) {
                        s32 hi = (s32)*(s16 *)(obj + 0x3e) -
                                 (s32)*(s8 *)(obj + 0xc1) - d0;
                        s32 c39 = (s32)(s8)recp[0x3a];

                        /* 0x006852df */
                        ecx_v = (ecx_v & 0xffffff00u) |
                                (u32)*(u8 *)((u32)(b5 + roff) + 0x3a);
                        if (c39 <= hi) {
                            s16 s17 = (s16)(av - 1);
                            s16 s12 = (s16)c39;
                            int ok = 1;

                            /* 0x006852f1: movsx cx, cl keeps ECX bits 16..31 */
                            ecx_v = (ecx_v & 0xffff0000u) |
                                    ((u32)(u16)(s16)(s8)recp[0x3a] & 0xffffu);
                            if (s17 < s12) {
                                while (s17 < s12 && ok) {
                                    if (*(u8 *)((u32)buf + (u32)(s32)s17) !=
                                        0xff)
                                        ok = 0;
                                    s17++;
                                }
                            }
                            if (ok && recp[0x37] == 0xff) {
                                u32 vt;
                                /* 0x00685327..0x00685343 */
                                recp[0x37] = 0xfe;
                                ecx_v = (ecx_v & 0xffffff00u) |
                                        *(u8 *)(obj + 0x44);
                                vt = *(u32 *)obj;
                                (*(cm_vfn5 *)(vt + 0x40))(
                                    (u32)obj, *(u32 *)recp, 0xfffffffeu,
                                    ecx_v, 0, 0);
                                ecx_v = 0;
                            }
                        }
                    }
                }

                /* 0x00685346 */
                {
                    u32 b6 = *(u32 *)(obj + 0xb1);
                    u8 c39 = *(u8 *)(b6 + roff + 0x39);
                    u8 c3a = *(u8 *)(b6 + roff + 0x3a);

                    ecx_v = (ecx_v & 0xffffff00u) | c39;      /* 0x0068534c */
                    if (c39 == c3a && (s8)c39 < 9 &&
                        *(u8 *)(obj + 0x44) == 0xff) {
                        u32 sub = *(u32 *)(obj + 4);
                        if (cm_fn_004b0700(*(u32 *)sub) != 0 &&
                            *(u32 *)(obj + 0x1c) == 0xffffffffu) {
                            u32 f0 = *(u32 *)(b6 + roff);
                            u32 id = *(u32 *)f0;
                            if (*(u32 *)(G_00ae2d14 + id * 6u) ==
                                0xffffffffu) {
                                u32 o = *(u32 *)(obj + 4);
                                u32 ob = *(u32 *)(o + 0x5d);
                                if (ob != 0) {
                                    if (*(u32 *)ob != G_009cf45c) {
                                        u32 x = *(u32 *)(ob + 0x71);
                                        if (x != 0 &&
                                            *(u32 *)x == G_009cfa10) {
                                            u32 b7;
                                            s32 l211;
                                            s32 l212;
                                            s32 c5s;
                                            u32 r0;

                                            /* 0x006853e1..0x006853f5 */
                                            cm_fn_00905c60(
                                                G_00de1f58, ob,
                                                (u32)(u8 *)&l2l3[1],
                                                (u32)(u8 *)&l2l3[0]);
                                            /* 0x006853fa..0x00685424 */
                                            b7 = *(u32 *)(obj + 0xb1);
                                            c5s = (s32)(s8)*(u8 *)(
                                                b7 + roff + 0x39);
                                            l211 = (s32)(s8)l2l3[1];
                                            l212 = (s32)(s8)l2l3[0];
                                            r0 = *(u32 *)(b7 + roff);
                                            if (c5s <= l211) {
                                                u32 v = *(u32 *)r0;
                                                *(u32 *)(G_00ae2d14 +
                                                         v * 6u) = G_009cf6e8;
                                            } else if (c5s <= l211 + l212 - 1) {
                                                u32 v = *(u32 *)r0;
                                                *(u32 *)(G_00ae2d14 +
                                                         v * 6u) = G_009cf6f0;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                /* 0x0068544f..0x00685473 */
                {
                    u32 b8 = *(u32 *)(obj + 0xb1);
                    u8 *r8 = (u8 *)(b8 + roff);
                    u32 vt;
                    u32 a5;
                    u32 a4;
                    u32 a3;

                    eax_v = b8;                             /* 0x0068544f */
                    vt = *(u32 *)obj;                       /* 0x0068545b */
                    eax_v = (eax_v & 0xffffff00u) | r8[0x3a]; /* 0x0068545d */
                    a5 = eax_v;
                    ecx_v = (ecx_v & 0xffffff00u) | r8[0x39]; /* 0x00685460 */
                    a4 = ecx_v;
                    eax_v = (eax_v & 0xffffff00u) | (u8)(i + 1); /* 0x00685464 */
                    a3 = eax_v;
                    ecx_v = (ecx_v & 0xffffff00u) |
                            *(u8 *)(obj + 0x44);            /* 0x00685469 */
                    (*(cm_vfn5 *)(vt + 0x68))((u32)obj, *(u32 *)r8, ecx_v, a3,
                                              a4, a5);
                }

                /* 0x00685476 */
                i++;
                if ((s16)i >= *(s16 *)(obj + 0x3e)) break;
            }
        }

        /* 0x00685485 */
        cm_fn_009452ca((u32)buf);
    }
    return;
}
