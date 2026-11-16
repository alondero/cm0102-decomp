/* fn_007e99a0 -- CM 01/02 3.9.68 (pinned SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1c627a22c445d14d489bd9bff86), preferred-image
   VA 0x007e99a0, module comp/cups/rus_cup.cpp.  Plain C translation of the
   original instructions; every callee remains an explicit original-code
   boundary and no engine logic is invented.

   Calling convention: epilogue is `add esp,0x3b0; ret` (ret 0) and ECX is read
   before it is written (0x007e99a9 `mov esi,ecx`), so the single input is the
   receiver object in ECX.  EAX is the 32-bit result (`xor eax,eax; setge al`).

   Original frame (after `sub esp,0x3b0` = frame base L, then ebx/ebp/esi/edi
   saved).  Offsets below are measured from L; all of them are reproduced by the
   single scratch aggregate below so the compiler keep each original address
   adjacency:
     L+0x02  byte   [esp+0x12] counting slot (B), stores at 0x7e99c8/0x7e99ec
     L+0x03  byte   [esp+0x13] counting slot (A), init 0 at 0x007e99c8
     L+0x04  dword  [esp+0x14] do/while countdown, 0x007e9b20 .. 0x007e9b68
     L+0x08  dword  [esp+0x18] 107*[0x9cf92c]+[0xae23d0]  (0x007e9a40)
     L+0x0c  dword  [esp+0x24] EAX of 4afd80             (0x007e9b0c)
     L+0x10  dword  [esp+0x20] 107*[0x9cf930]+[0xae23d0]  (0x007e9a6e)
     L+0x14  dword  [esp+0x24] 107*[0x9cf920]+[0xae23d0]  (0x007e9a12)
     L+0x18  dword  [esp+0x28] 107*[0x9cf924]+[0xae23d0]  (0x007e9a29)
     L+0x1c  dword  [esp+0x2c] 107*[0x9cf928]+[0xae23d0]  (0x007e9a5d)
     L+0x20  256B   [esp+0x30]  local buffer                  (0x007e9a84)
     L+0x120 256B   [esp+0x130] local buffer                  (0x007e9a88)
     L+0x220 100*4  [esp+0x230] entry pool passed to 4afbd0   (0x007e9b52)
   0x007e9a12 writes L+0x14 with the allocation argument still pushed.
   After its cleanup at 0x007e9a77, 0x007e9b33 reads that slot as
   [esp+0x24]. The result stored at 0x007e9b0c with two arguments pushed
   instead occupies L+0x0c, later read at 0x007e9bf2.

   Held harness concern: the pool at L+0x220 is only written for indices
   [0,A) by the scan at 0x007e9b24..0x007e9b6c (A = byte L+0x03, capped at 100)
   and 4afbd0(pool,A,14) does not fill the suffix when A < 14, yet the loop at
   0x007e9c2c..0x007e9c7d reads pool dwords L+0x220 .. L+0x24c whenever the
   L+0x02 counter is below 0x20 (B < 32).
   UNRESOLVED FIDELITY BOUNDARY: this automatic aggregate does not transport
   the incoming original stack bytes reserved at 0x007e99a0. Volatile loads
   at the translated 0x007e9c41 cannot repair that missing input. No
   all-path initialization follows from this listing. Parent intervention
   is required for an original-frame-byte transport contract; passing the
   current gate is not fidelity evidence for the unwritten suffix.
   Do not initialize, clamp, or fabricate these bytes. */
#include "cmre.h"

#define G_009cf914 CM_GLOBAL(u32, 0x009cf914)
#define G_009cf918 CM_GLOBAL(u32, 0x009cf918)
#define G_009cf91c CM_GLOBAL(u32, 0x009cf91c)
#define G_009cf920 CM_GLOBAL(u32, 0x009cf920)
#define G_009cf924 CM_GLOBAL(u32, 0x009cf924)
#define G_009cf928 CM_GLOBAL(u32, 0x009cf928)
#define G_009cf92c CM_GLOBAL(u32, 0x009cf92c)
#define G_009cf930 CM_GLOBAL(u32, 0x009cf930)
#define G_00ae2350 CM_GLOBAL(s32, 0x00ae2350)
#define G_00ae235c CM_GLOBAL(s32, 0x00ae235c)
#define G_00ae23b4 CM_GLOBAL(u32, 0x00ae23b4)
#define G_00ae23d0 CM_GLOBAL(u32, 0x00ae23d0)
#define G_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

/* Callee signatures exactly as recorded for this routine.  4afbd0 takes its
   entry EAX/EDX explicitly: the original call at 0x007e9b7f only pushes the
   three stack arguments (0x007e9b74 push 0xe, 0x007e9b7d push eax,
   0x007e9b7e push ecx; `add esp,0xc` at 0x007e9b8f), so EAX and EDX are
   incoming register inputs of that boundary and the three stack arguments
   start at the first cdecl argument. */
extern u32 CM_CDECL __attribute__((regparm(2))) cm_fn_004afbd0(
    u32 entry_eax, u32 entry_edx, u32 base, u32 count, u32 wanted);
extern u32 cm_fn_004afd80(u32 param_1, char param_2);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);

struct cm_frame_7e99a0 {
    u8 pad_low[2];              /* L+0x00 .. L+0x01 */
    u8 count;                   /* L+0x02 [esp+0x12] */
    u8 pooln;                   /* L+0x03 [esp+0x13] */
    s32 rem;                    /* L+0x04 [esp+0x14] */
    u32 v92c;                   /* L+0x08 [esp+0x18] */
    u32 v4afd;                  /* L+0x0c [esp+0x24] before the 8-byte cleanup */
    u32 v930;                   /* L+0x10 [esp+0x20] */
    u32 v920;                   /* L+0x14 [esp+0x24] read at 0x007e9b33 */
    u32 v924;                   /* L+0x18 [esp+0x28] */
    u32 v928;                   /* L+0x1c [esp+0x2c] */
    u8 buf1[0x100];             /* L+0x20 [esp+0x30] */
    u8 buf2[0x100];             /* L+0x120 [esp+0x130] */
    u32 pool[100];              /* L+0x220 [esp+0x230] */
};

u32 CM_FASTCALL cm_fn_007e99a0(int param_1)
{
    struct cm_frame_7e99a0 f;
    u32 self;
    u32 a914, a918, a91c;
    u32 edi_, ebp_, ebx_;
    u32 base;
    u32 p;
    u32 hp;
    u32 hpdw;
    u32 eaxv;
    u32 edxv;
    u32 fld;
    u32 rec;
    u32 u;
    u32 *pp;
    s32 cnt;
    s32 i;
    s32 n;
    s32 idx;
    u8 cl;
    char *omsg;

    self = (u32)param_1;

    a918 = G_009cf918;                       /* 0x007e99ab */
    a914 = G_009cf914;                       /* 0x007e99b1 */
    f.pooln = 0;                             /* 0x007e99c8 (byte L+0x03) */
    edi_ = a914 * 107u;                      /* 0x007e99c0 .. 0x007e99e5 */
    ebp_ = a918 * 107u;                      /* 0x007e99bd .. 0x007e99d7 */
    a91c = G_009cf91c;                       /* 0x007e99d9 */
    base = G_00ae23d0;                       /* 0x007e99e7 */
    f.count = 0;                             /* 0x007e99ec (byte L+0x02) */
    ebx_ = a91c * 107u;                      /* 0x007e99e2 .. 0x007e99f9 */
    edi_ = edi_ + base;                      /* 0x007e99f4 */
    ebp_ = ebp_ + base;                      /* 0x007e99fe */
    ebx_ = ebx_ + base;                      /* 0x007e9a00 */
    f.v920 = G_009cf920 * 107u + base;       /* 0x007e99fb .. 0x007e9a12 */
    f.v924 = G_009cf924 * 107u + base;       /* 0x007e9a16 .. 0x007e9a29 */
    f.v92c = G_009cf92c * 107u + base;       /* 0x007e9a38 .. 0x007e9a40 */
    *(volatile u16 *)(self + 0x3e) = 0x30;   /* 0x007e9a57 */
    f.v928 = G_009cf928 * 107u + base;       /* 0x007e9a44 .. 0x007e9a5d */
    f.v930 = G_009cf930 * 107u + base;       /* 0x007e9a4f .. 0x007e9a6e */

    hp = (u32)cm_fn_00944e46(0x120);         /* 0x007e9a72 */
    *(volatile u32 *)(self + 0xa7) = hp;     /* 0x007e9a7a */
    if (hp == 0) {                           /* 0x007e9a80 */
        /* 0x007e9a8f..0x007e9a9a: 5 pushes, 0x14 cleaned at 0x007e9a9f. */
        cm_fn_00944cff(0x00a7ac58u, 0u, 0u, (void *)f.buf2, (void *)f.buf1);
        /* 0x007e9aad..0x007e9ab4: 0x148, L+0x20, L+0x120 are pushed for the
           following 944c9f call because 90d130 is declared (void). */
        omsg = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)omsg,
                       (u32)(void *)f.buf2, (u32)(void *)f.buf1, 0x148u);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        G_00b67a34 = 0;                      /* 0x007e9add */
        return 0xffffffffu;                  /* 0x007e9ae7 `or eax,0xffffffff` */
    }

    f.v4afd = cm_fn_004afd80(*(volatile u32 *)(self + 4), 0); /* 0x007e9af5..0x7e9b0c */
    edxv = 2u * (u32)G_00ae2350;             /* 0x007e9b00 / 0x007e9b15 */
    cnt = (s32)((u32)G_00ae235c - edxv);     /* 0x007e9b1a */
    eaxv = (u32)cnt;                         /* EAX after 0x007e9b1c */
    if (cnt > 0) {                           /* 0x007e9b1e */
        f.rem = cnt;                         /* 0x007e9b20 */
        p = G_00ae23b4;
        do {
            fld = *(volatile u32 *)(p + 0x57); /* 0x007e9b24 */
            if (fld == edi_ || fld == ebp_ || fld == ebx_ ||
                fld == f.v920 || fld == f.v924 || fld == f.v92c) {
                if ((s32)(s8)f.pooln < 0x64) { /* 0x007e9b49 `cmp al,0x64; jge` */
                    idx = (s32)(s8)f.pooln;    /* 0x007e9b4d `movsx edx,al` */
                    edxv = (u32)idx;           /* EDX left by that movsx */
                    f.pooln = (u8)(f.pooln + 1); /* 0x007e9b50 / 0x007e9b59 */
                    f.pool[idx] = p;           /* 0x007e9b52 */
                }
            }
            p = p + 0x245;                     /* 0x007e9b61 */
            n = f.rem - 1;                     /* 0x007e9b67 */
            f.rem = n;                         /* 0x007e9b68 */
        } while (n != 0);                      /* 0x007e9b6c */
        eaxv = 0;                              /* EAX reloaded from L+0x04 */
    }

    /* 0x007e9b6e `movsx ax,byte [esp+0x13]` only writes AX; the pushed EAX
       keeps the high half of the value left in EAX above (0 after the loop, the
       negative count when the loop body was skipped). */
    u = (eaxv & 0xffff0000u) | (u32)(u16)(s16)(s8)f.pooln;
    /* 0x007e9b7f: push 0xe; push eax; push &pool; call 004afbd0.
       entry_eax equals the pushed count, entry_edx is the live EDX. */
    cm_fn_004afbd0(u, edxv, (u32)(void *)f.pool, u, 0xeu);

    /* 0x007e9b84 .. 0x007e9c21: clubs whose field at +0x57 equals v928 fill the
       record buffer downwards from index 31 as the L+0x02 counter grows. */
    i = 0;
    p = G_00ae23b4;
    n = (s32)((u32)G_00ae235c - 2u * (u32)G_00ae2350);
    if (n > 0) {
        do {
            if (*(volatile u32 *)(p + 0x57) == f.v928) {  /* 0x007e9ba8 */
                f.count = (u8)(f.count + 1);              /* 0x007e9bb3..0x7e9bbe */
                idx = (s32)(s8)f.count;                   /* 0x007e9bbb */
                rec = (u32)((0x20 - idx) * 6);            /* 0x007e9bc2..0x7e9bc9 */
                hpdw = *(volatile u32 *)(self + 0xa7);    /* 0x007e9bb3 */
                *(volatile u32 *)(hpdw + rec) = p;        /* 0x007e9bcc */
                *(volatile u8 *)(*(volatile u32 *)(self + 0xa7) + rec + 5) = 0;
                *(volatile u8 *)(*(volatile u32 *)(self + 0xa7) + rec + 4) = 2;
                if (p == f.v4afd) {                       /* 0x007e9bf6 */
                    *(volatile u8 *)(*(volatile u32 *)(self + 0xa7) + rec + 4) = 1;
                }
            }
            i = i + 1;                                    /* 0x007e9c0e */
            p = p + 0x245;                                /* 0x007e9c0f */
            n = (s32)((u32)G_00ae235c - 2u * (u32)G_00ae2350);
        } while (i < n);                                  /* 0x007e9c21 */
    }

    /* 0x007e9c23 .. 0x007e9c7d: copy at most 14 pool dwords while the counter is
       still below 0x20.  The pool suffix may be unwritten by the original, see
       the header comment: original-frame-byte transport is unresolved. */
    i = 0;
    pp = f.pool;
    for (;;) {
        if ((s32)(s8)f.count >= 0x20) {                   /* 0x007e9c30 */
            break;
        }
        f.count = (u8)(f.count + 1);                      /* 0x007e9c35 */
        hpdw = *(volatile u32 *)(self + 0xa7);            /* 0x007e9c37 */
        fld = *(volatile u32 *)pp;                        /* 0x007e9c41 */
        idx = (s32)(s8)f.count;                           /* 0x007e9c43 */
        pp = pp + 1;                                      /* 0x007e9c4b */
        rec = (u32)((0x20 - idx) * 6);                    /* 0x007e9c46..0x7e9c50 */
        *(volatile u32 *)(hpdw + rec) = fld;              /* 0x007e9c53 */
        *(volatile u8 *)(*(volatile u32 *)(self + 0xa7) + rec + 5) = 0;
        i = i + 1;                                        /* 0x007e9c72 */
        *(volatile u8 *)(*(volatile u32 *)(self + 0xa7) + rec + 4) = 2;
        if (i >= 0xe) {                                   /* 0x007e9c73 */
            break;
        }
    }

    /* 0x007e9c83: CL is reloaded from L+0x02 (0x007e9c7f) and then incremented
       in the register only; the frame byte is not rewritten by this loop. */
    cl = f.count;
    i = 0;
    p = G_00ae23b4;
    n = (s32)((u32)G_00ae235c - 2u * (u32)G_00ae2350);
    if (n > 0) {
        do {
            if (*(volatile u32 *)(p + 0x57) == f.v930) {  /* 0x007e9ca2 */
                idx = (s32)(s8)cl;                        /* 0x007e9caf */
                rec = (u32)(idx * 6);                     /* 0x007e9cb2..0x7e9cb5 */
                cl = (u8)(cl + 1);                        /* 0x007e9cb7 */
                hpdw = *(volatile u32 *)(self + 0xa7);    /* 0x007e9ca9 */
                *(volatile u32 *)(hpdw + rec) = p;        /* 0x007e9cb9 */
                *(volatile u8 *)(*(volatile u32 *)(self + 0xa7) + rec + 5) = 0;
                *(volatile u8 *)(*(volatile u32 *)(self + 0xa7) + rec + 4) = 1;
            }
            i = i + 1;                                    /* 0x007e9cdf */
            p = p + 0x245;                                /* 0x007e9ce2 */
            n = (s32)((u32)G_00ae235c - 2u * (u32)G_00ae2350);
        } while (i < n);                                  /* 0x007e9cea */
    }

    /* 0x007e9cec `movsx cx,cl`; 0x007e9cf2 cmp cx,word [esi+0x3e]; setge al */
    return (u32)((s16)(s8)cl >= (s16)*(volatile u16 *)(self + 0x3e));
}
