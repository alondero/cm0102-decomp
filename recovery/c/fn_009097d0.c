/* fn_009097d0 @ preferred-image VA 0x009097d0, image base 0x00400000.
   CM0102 3.9.68 binary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   Frame recovered from the original instructions:
     009097d0 push -1 / 009097d2 push 0x0096543e / 009097d7 push fs:[0]
     009097de mov fs:[0], esp        -> one SEH record {prev, handler, state}
     009097e5 sub esp, 0x414         -> scratch base L, SEH record at L+0x414
     009097eb..009097f0 push ebx,ebp,esi,edi; 009097ee esi = ecx (this)
   Scratch slots from the listing:
     L+0x03 byte counter (bl, stored/reloaded at 0x009097f8/0x00909898)
     L+0x04 malloc(8) pointer (0x0090980d store, read via edx and at 0x009099a2)
     L+0x08 arg3 out-param of the 0x009098c0 virtual call (read 0x009098e3)
     L+0x0c arg2 out-param of the same call (read after 0x009098fa)
     L+0x10 object from 0x00944cf1(0xb2) (0x009098d2, write-only)
     L+0x14 four adjacent 256-byte message buffers (0x0090981a/0x0090981e,
       0x00909938/0x0090993f use L+0x14/L+0x214 and L+0x114/L+0x314).
   The virtual slot is called with ECX=this and exactly four stack arguments
   (009098b4/009098b6/009098ba/009098bb/009098bc); the callee pops 0x10 bytes
    (no add esp after 009098c0), so the slot is thiscall with 4 args.

    Held-boundary limitation: this source preserves the actual slot read and
    four-argument dispatch, rather than substituting a guessed target. The
    supplied listing does not contain ctor 00906fc7, table 00970844, or entry
    009072d0 instructions. Thus it cannot establish that target's ABI or
    register that entry in the harness. Passing current semantic fixtures is
    not evidence that the held real-target/virtual-fixture gap is closed;
    original-derived fixtures and parent registration remain required. */
#include "cmre.h"

struct cm_fn_009097d0_frame {
    u8 scratch[0x414];      /* L .. L+0x413 */
    void *volatile prev;    /* L+0x414 */
    volatile u32 handler;   /* L+0x418 */
    volatile u32 state;     /* L+0x41c */
};

/* 009098ae mov edx,[esi] ; 009098c0 call dword ptr [edx+0x3c], ECX=esi */
typedef u32 (CM_THISCALL *cm_fn_009097d0_vt3c)(u32 self, u32 a, u32 b, u32 c,
                                               u32 d);

extern u32 CM_THISCALL cm_fn_0051c800(u32 self, volatile u32 round_arg);
extern u32 CM_THISCALL cm_fn_00522e00(u32 self, u32 owner, s16 count, u32 clubs,
                                      s16 rounds, u32 competition, u32 schedule,
                                      u16 year, u8 flag_a, u8 flag_b,
                                      u16 round_flag, u8 flag_c, u32 builder,
                                      u32 numbered, u32 value_38,
                                      u32 club_flags);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
/* 0090983a..00909851 and 00909965..0090997c leave three pushes
   live across 0090d130 for the subsequent variadic formatter. */
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                   void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

void CM_THISCALL cm_fn_009097d0(u32 self)
{
    struct cm_fn_009097d0_frame fr;
    u8 *L = fr.scratch;
    u32 obj;
    u32 edi;
    u32 arr;
    u32 newobj;
    u32 uVar3;
    u32 uVar5;
    u32 f;
    u32 e;
    u32 off;
    s32 i;
    s8 n;
    u32 vt, out_value, rounds, competition;
    u16 year;

    fr.state = 0xffffffffu;         /* 009097d0 push -1 */
    fr.handler = 0x0096543eu;       /* 009097d2 push 0x96543e */
    fr.prev = CM_FS(void *, 0);     /* 009097d7 */
    CM_FS(void *, 0) = (void *)&fr.prev; /* 009097de */

    n = 0;                                                       /* 009097f1 */
    obj = *(volatile u32 *)(self + 0x0c);                        /* 009097f3 */
    *(u8 *)(L + 3) = 0;                                         /* 009097f8 */
    edi = *(volatile u32 *)(obj + 0x08);                         /* 009097fc */
    arr = (u32)cm_fn_00944e46(8);                                /* 009097ff */
    i = 0;                                                       /* 00909806 */
    *(u32 *)(L + 0x04) = arr;                                    /* 0090980d */
    if (arr == 0) {                                              /* 0090980b */
        cm_fn_00944cff((u32)0x00aa1fb4u, 0u, 0u, (void *)(L + 0x14),
                       (void *)(L + 0x214));                     /* 00909827 */
        uVar3 = (u32)cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, uVar3,
                      L + 0x14, L + 0x214, 0x5e5u);             /* 00909851 */
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(u32, 0x00b67a34) = 0;                          /* 00909869 */
    }
    else {
        if (*(volatile s16 *)(edi + 0x3e) > 0) {                 /* 00909874 */
            off = 0;                                             /* 0090987b */
            for (;;) {
                if (n >= 2) break;                               /* 0090987d */
                e = *(volatile u32 *)(edi + 0xa7) + off;         /* 00909882 */
                if (*(volatile u8 *)(e + 5) == 1) {              /* 0090988a */
                    *(u32 *)(arr + (s32)n * 4) = *(volatile u32 *)e;
                    n = (s8)(*(u8 *)(L + 3) + 1);               /* 00909898 */
                    *(u8 *)(L + 3) = (u8)n;
                }
                rounds = (u32)(s32)*(volatile s16 *)(edi + 0x3e);
                i = i + 1;                                       /* 009098a6 */
                off = off + 6;                                   /* 009098a7 */
                if (!(i < (s32)rounds)) break;
            }
        }
        vt = *(volatile u32 *)self;                              /* 009098ae */
        uVar3 = ((cm_fn_009097d0_vt3c)
                 (*(volatile u32 *)(vt + 0x3c)))
                    (self, 3u, (u32)(L + 0x0c), (u32)(L + 0x08), 0u);
        edi = uVar3;                   /* 009098c8 mov edi, eax */
        newobj = (u32)cm_fn_00944cf1(0xb2);                      /* 009098ca */
        *(u32 *)(L + 0x10) = newobj;                             /* 009098d2 */
        fr.state = 0;                                            /* 009098da */
        if (*(u32 *)(L + 0x10) == 0) {
            uVar5 = 0;                                           /* 00909912 */
        }
        else {
            out_value = *(volatile u32 *)(L + 0x08);             /* 009098e3 */
            year = *(volatile u16 *)(self + 0x40);               /* 009098e7 */
            competition = *(volatile u32 *)(self + 4);           /* 009098f2 */
            rounds = *(volatile u32 *)(L + 0x0c);                /* 009098fa */
            arr = *(volatile u32 *)(L + 0x04);                   /* 00909900 */
            uVar5 = cm_fn_00522e00(
                newobj,                                          /* ECX 00909909 */
                self,                                            /* push15 arg2 00909908 */
                2,                                               /* push14 arg3 00909906 */
                arr,                                             /* push13 arg4 00909905 */
                (s16)rounds,                                     /* push12 arg5 00909904 */
                competition,                                     /* push11 arg6 009098ff */
                edi,                                             /* push10 arg7 009098fe */
                year,                                            /* push9  arg8 009098f9 */
                3,                                               /* push8  arg9 009098f7 */
                1,                                               /* push7  arg10 009098f5 */
                (u16)out_value,                                  /* push6  arg11 009098f1 */
                0x14,                                            /* push5  arg12 009098ef */
                0, 0, 0, 0);                                     /* push4..1 009098eb */
        }
        obj = *(volatile u32 *)(self + 0x0c);                    /* 00909914 */
        fr.state = 0xffffffffu;                                  /* 00909918 */
        *(volatile u32 *)(obj + 0x0c) = uVar5;                   /* 00909923 */
        cm_fn_009452ca(edi);                                     /* 00909926 */
        obj = *(volatile u32 *)(self + 0x0c);                    /* 0090992b */
        f = *(volatile u32 *)(obj + 0x0c);                       /* 00909931 */
        if (f == 0) {                                            /* 00909936 */
            cm_fn_00944cff((u32)0x00aa1fb4u, 0u, 0u, (void *)(L + 0x314),
                           (void *)(L + 0x114));                 /* 0090994f */
            uVar3 = (u32)cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, uVar3,
                          L + 0x314, L + 0x114, 0x5feu);         /* 0090997c */
            cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u,
                           0);
            CM_GLOBAL(u32, 0x00b67a34) = 0;                      /* 00909994 */
        }
        else {
            cm_fn_0051c800(f, 0);                                /* 0090999d */
            cm_fn_009452ca(*(volatile u32 *)(L + 4));             /* 009099a7 */
        }
    }
    CM_FS(void *, 0) = fr.prev;                                  /* 009099ba */
}
