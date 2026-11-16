/* fn_0076e0b0 at preferred-image VA 0x0076e0b0 (image base 0x00400000), reconstructed from
   the original 3.9.68 instructions.  Stack frame after `sub esp,0x10c` (call it L):
     L+0x000..L+0x10b  scratch (the emitted object lives at L+0x2c, 0xe0 bytes)
     L+0x10c prev, L+0x110 handler, L+0x114 state   (SEH registration, handler 0x0095ddfb)
     L+0x118 return address, L+0x11c..L+0x134 the seven stack arguments
   `ret 0x1c` pops seven words; ECX is read at 0x0076e0d5 before any write, so the entry
   convention is __thiscall with the receiver as the first formal.  The receiver is only
   forwarded (0x0076e186 loads it back out of L+0x18 for the 0x0076d560 call).

   Tails/registers that the original reuses:
   - ebp always mirrors the L+4 dword (0 and 0x0076e0f3, 0x0076e1a4, 0x0076e326).
   - bl holds the 0x0076e111 mask across the whole scan loop (restored from L+3 at
     0x0076e2b6 and 0x0076e32e).
   - the slot indices are built in AL (0x0076e2d0/0x0076e2e4/...) and CL (0x0076e29a), i.e.
     only the byte is computed; they are handed to the callee as the 32-bit slot word.
   - L+0x10 (WORDL10) is written only at 0x0076e0f7 (entry arg6 != 0) and at 0x0076e19f
     (the 0x0076d560 lookup).  It is read unconditionally at 0x0076e268 and 0x0076e279, so
      the local below is NOT bound to the original entry-ESP-0x108 home. This remains
      an unresolved fidelity defect: an independent uninitialized C automatic cannot
      reproduce the incoming word. No witness value or default is invented.
   - EAX at the RET is live (dword): 0 when the scan ran out with ebp == 0, otherwise the
     arg6 dword, or the low dword of cm_fn_00944c94's 64-bit result. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif

typedef struct {
    void *prev;
    void *handler;
    u32 state;
} cm_seh_frame;

extern u32 CM_THISCALL cm_fn_0068d040(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
/* Bound-target instruction review only (not composition/runtime proof):
   CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86,
   preferred-image VAs 0x0068d1b0..0x0068d1c2: loads/stores use ECX as base;
   only EAX, EDX and flags are written, on both RET paths. Thus ECX after the
   calls at 0x0076e28f/0x0076e2ad equals their receiver, cur. The CL write
   at 0x0076e29a preserves its high 24 bits before PUSH ECX at 0x0076e2a1. */
/* 0x0076e186 loads ECX (the entry receiver) and 0x0076e193 leaves ESP where it was before
   the two argument pushes, so this callee takes ECX and pops its 8 argument bytes. */
extern u32 CM_THISCALL cm_fn_0076d560(u32 self, u32 *param_1, u32 param_2);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);
extern u32 CM_STDCALL cm_fn_007875e0(u32 club);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);

/* 0x0095ddfb: the SEH handler word pushed at 0x0076e0b2. */
#define CM_SEH_HANDLER 0x0095ddfbu
/* 0x00b7431c: loaded at 0x0076e158 (dead, before the 0x007875e0 call) and at 0x0076e332
   as the receiver of 0x007861e0. */
#define CM_LIST 0x00b7431cu

u32 CM_THISCALL cm_fn_0076e0b0(u32 self, u32 *param_1, u32 param_2, u32 param_3,
                               u32 param_4, u32 param_5, u32 param_6, s8 param_7)
{
    volatile cm_seh_frame frame;   /* L+0x10c prev / +0x110 handler / +0x114 state */
    u8 n;                          /* L+2  matched-this-group counter byte */
    u32 cur;                       /* L+4  ebp: 0, arg6, or the 0x0076d560 result */
    u8 mask;                       /* L+3  byte mask tested against [obj+0x5e] */
    u32 scan;                      /* L+8  arg1+0xdb, advanced by 4 */
    s32 idx;                       /* L+0x0c scan index, starts at 1 */
    s32 rem;                       /* L+0x14 iteration countdown, starts at 0x32 */
    u32 club;                      /* edi: [matched+0x39] of the current match */
    u32 grp[3];                    /* L+0x20..L+0x2b matched objects of this group */
    u8 obj[0xe0];                  /* L+0x2c..L+0x10b emitted object (built by 0x0077bbe0) */
    s16 w10;                       /* L+0x10 WORDL10, only ever touched as a word home */
#define W10 (*(volatile s16 *)&w10) /* unresolved: not the original physical home */
    u32 result;
    u32 ent;                       /* esi: scanned object */
    u32 emit;

    /* 0x0076e0b0..0x0076e0be: register the handler with state -1. */
    frame.state = 0xffffffffu;
    frame.handler = (void *)CM_SEH_HANDLER;
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;

    n = 0;                          /* 0x0076e0db */
    cur = 0;                        /* 0x0076e0e0 (xor ebp,ebp) */
    if (param_6 != 0) {             /* 0x0076e0d9..0x0076e0f7 */
        cur = param_6;
        W10 = (s16)param_7;         /* movsx ax, byte ptr [esp+0x138] */
    }
    /* 0x0076e10c..0x0076e11c: neg/sbb/and/add gives 8 when arg2 != 0, else 4. */
    mask = (u8)((param_2 != 0) ? 8u : 4u);
    idx = 1;                        /* 0x0076e114 */
    scan = (u32)param_1 + 0xdb;     /* 0x0076e11f, 0x0076e129 */
    rem = 0x32;                     /* 0x0076e12d */

    do {
        u32 s;
        u32 next;

        /* 0x0076e135..0x0076e139 */
        s = *(volatile u32 *)(scan - 4);
        ent = s;
        if (ent != 0) {
            if ((*(volatile u8 *)(ent + 0x5e) & mask) != 0) {
                u32 cl = *(volatile u32 *)(ent + 0x39);   /* 0x0076e14d */
                if (cl != 0) {
                    /* 0x0076e158: dead pre-call load of the list head. */
                    (void)*&CM_GLOBAL(volatile u32, CM_LIST);
                    if (cm_fn_007875e0(cl) != 0) {
                        /* 0x0076e16c..0x0076e1a4 */
                        if ((cur == 0) && (param_5 == 0) && (param_3 != 0)) {
                            u32 p = cm_fn_0076d560(self, param_1, param_3);
                            s16 w = *(s16 *)p;            /* mov cx, word ptr [eax] */
                            cur = p;                      /* 0x0076e19b */
                            W10 = w;                      /* 0x0076e19f */
                        }
                        /* 0x0076e1a6..0x0076e1ba */
                        if ((s8)n < 3) {
                            grp[(s8)n] = ent;             /* movsx edx,al; mov [esp+edx*4+0x30] */
                        }
                        club = *(volatile u32 *)(ent + 0x39);   /* 0x0076e1b5 -> edi */
                        n = (u8)(n + 1);
                        emit = 1;
                        if (idx < 0x32) {                       /* 0x0076e1be..0x0076e1c9 */
                            next = *(volatile u32 *)scan;       /* 0x0076e1cf */
                            /* L+0x1c = edi at 0x0076e1c5 is this same club value. */
                            if (next != 0 &&
                                *(volatile u32 *)(next + 0x39) == club &&
                                (*(volatile u8 *)(next + 0x5e) & mask) != 0) {
                                emit = 0;                       /* 0x0076e1dd -> tail */
                            }
                        }
                        if (emit) {
                            u32 eax_word;
                            /* 0x0076e1e3: (re)initialise the emitted object. */
                            cm_fn_0077bbe0(obj, 0x1d, 0);
                            frame.state = 0;                    /* 0x0076e1f7 */
                            cm_fn_00785790(obj, 0, *(volatile u32 *)param_1);
                            cm_fn_00785790(obj, 1, *(volatile u32 *)club);
                            cm_fn_00785790(obj, 2, param_5);
                            cm_fn_00785790(obj, 3, param_4);
                            cm_fn_00785790(obj, 4, (u32)(s32)(s8)n);
                            cm_fn_00785790(obj, 5, param_2);
                            eax_word = (u32)cm_fn_00785790(obj, 6, (u32)(s32)W10);
                            /* 0x0076e279: signed word compare of WORDL10 with 4. */
                            if ((W10 < 4) && (cur != 0)) {
                                u32 f = *(volatile u32 *)(cur + 2);
                                *(u32 *)(cur + 0xa) = f;        /* 0x0076e28c */
                                {
                                    u8 slot = 7;                /* mov bl,7 */
                                    u32 node;
                                    while ((node = cm_fn_0068d1b0(cur)) != 0) {
                                        u32 q = *(volatile u32 *)node;      /* 0x0076e298 */
                                        u32 v = *(volatile u32 *)q;         /* 0x0076e29e */
                                        cm_fn_00785790(obj, (cur & 0xffffff00u) | slot, v);
                                        slot = (u8)(slot + 1);              /* inc bl */
                                    }
                                }
                                /* Final 0x0068d1b0 return was zero; TEST at
                                   0x0076e2b2 does not change EAX. */
                                eax_word = 0;
                                /* 0x0076e2b6 restores bl from L+3. */
                            }
                            if ((s8)n <= 3) {                   /* 0x0076e2be */
                                u8 slot = 0xa;                  /* mov bl,0xa */
                                if ((s8)n > 0) {                /* 0x0076e2c6 */
                                    u32 cnt = (u32)(s32)(s8)n;  /* mov ebp,esi */
                                    u32 *pe = grp;              /* lea edi,[esp+0x30] */
                                    do {
                                        u32 e = *pe;            /* 0x0076e2ce */
                                        u32 f;
                                        /* Original 3.9.68, hash above, preferred-image
                                           VAs 0x0076e2ba/0x0076e2d0 and
                                           0x0076e2e4/0x0076e2f9/0x0076e30e:
                                           byte writes preserve the current EAX high
                                           bits; each CALL supplies the next EAX. */
                                        eax_word = (u32)cm_fn_00785790(obj, (eax_word & 0xffffff00u) | slot, *(volatile u32 *)e);
                                        slot = (u8)(slot + 1);
                                        f = *(volatile u32 *)(e + 4);
                                        eax_word = (u32)cm_fn_00785790(obj, (eax_word & 0xffffff00u) | slot, *(volatile u32 *)(f + 0x33));
                                        slot = (u8)(slot + 1);
                                        f = *(volatile u32 *)(e + 8);
                                        eax_word = (u32)cm_fn_00785790(obj, (eax_word & 0xffffff00u) | slot, *(volatile u32 *)(f + 0x33));
                                        slot = (u8)(slot + 1);
                                        f = *(volatile u32 *)(e + 0xc);
                                        eax_word = (u32)cm_fn_00785790(obj, (eax_word & 0xffffff00u) | slot, *(volatile u32 *)(f + 0x33));
                                        slot = (u8)(slot + 1);
                                        pe = pe + 1;
                                    } while (--cnt != 0);
                                    /* 0x0076e326/0x0076e32a reload ebp and edi from L+4
                                       and L+0x1c; both already hold cur and club. */
                                }
                                /* 0x0076e32e restores bl from L+3. */
                            }
                            /* 0x0076e332..0x0076e33e */
                            cm_fn_007861e0((void *)CM_GLOBAL(volatile u32, CM_LIST),
                                           (u32)obj, club);
                            n = 0;                              /* 0x0076e343 */
                            frame.state = 0xffffffffu;          /* 0x0076e348 */
                        }
                    }
                }
            }
        }
        /* 0x0076e353..0x0076e370 */
        idx = idx + 1;
        scan = scan + 4;
    } while (--rem != 0);

    /* 0x0076e376..0x0076e395: EAX is live across the epilogue. */
    if (cur != 0) {
        if (param_6 != 0) {
            result = param_6;                  /* 0x0076e37d mov eax,[esp+0x134] */
        } else {
            cm_fn_0068d040(cur);               /* 0x0076e38a */
            result = (u32)cm_fn_00944c94((void *)cur);   /* 0x0076e390 */
        }
    } else {
        result = 0;                            /* dec eax at 0x0076e363 left 0 */
    }

    CM_FS(void *, 0) = frame.prev;             /* 0x0076e398/0x0076e3a0 */
    return result;
}
