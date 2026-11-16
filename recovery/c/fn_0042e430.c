/* fn_0042e430, preferred-image VA 0x0042e430, image base 0x00400000, pinned
   CM0102 3.9.68 (SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1c627a22c445d14d489bd9bff86).
   Module comp\leagues\bra_nat_third.cpp.  ECX is read before written (mov edi,ecx at
   0x0042e450) and the routine returns with a bare `ret` (0x0042e6f2): thiscall, no
   stack arguments, void result.  ESI is only written by `or esi,0xffffffff`
   (0x0042e4ec), whose result is -1 regardless of the incoming value.

   Scratch frame: `sub esp,0x480` at 0x0042e445 gives base B; ebx/esi/edi are pushed
   to B-4/B-8/B-12, and the SEH record sits at B+0x480 {prev,handler,state}
   (state pushed -1 at 0x0042e430, handler 0x0095454e pushed at 0x0042e432,
   prev linked at 0x0042e43e; writes to state at 0x0042e609 and 0x0042e647 address
   B+0x488, and 0x0042e6db reloads prev from B+0x480).

   Callee stack-cleanup evidence used for the two thiscall boundaries below:
   - vtable slot +0x3c, call at 0x0042e5f1 with four pushed arguments (0x0042e5e6,
     0x0042e5e7, 0x0042e5eb, 0x0042e5ec).  After the call plus the 0x944cf1(0xb2)
     call and its `add esp,4` (0x0042e600), 0x0042e603 addresses B+0x1c with
     [esp+0x2c] and 0x0042e609 addresses the SEH state with [esp+0x498], i.e. esp is
     B-16 again, and 0x0042e610 `pop ebp` restores the saved ebp: the callee popped
     16 bytes (ret 0x10).
   - cm_fn_00522e00 call at 0x0042e63b pushes 15 dwords (0x0042e61b..0x0042e638) and
     sets ecx at 0x0042e639; 0x0042e644/0x0042e647 afterwards address the same frame
     as before (esp B-12 after the pop), so the callee pops 60 bytes (ret 0x3c).
     The pushed dwords, right to left, are 0,0,0,0,0x14,[B+0x1c],1,7,cx(=WORD
     [edi+0x40]),esi(0x0042e5f9),[edi+4],[B+0x14],[B+0x10],0x10,edi, with ecx = the
     0x944cf1(0xb2) allocation.  Slot [B+0x1c] holds that same allocation pointer
     (stored at 0x0042e603 as [esp+0x2c] with esp = B-16, reloaded at 0x0042e613 as
     [esp+0x24] with esp = B-12), so both the receiver and that dword argument carry
     the full pointer; that argument is therefore declared u32 here.  Likewise
     [B+0x14] is read as a full dword at 0x0042e621 and pushed whole, so it is passed
     as u32.  The cx pushed at 0x0042e629 leaves the dword's high half as whatever
     ecx held after the vtable call; only the 16-bit value is knowable and is passed
     as u16.
   - cm_fn_0090d130 is called three times with three cdecl pushes (0x0042e591-97,
     0x0042e4e0/e5 plus 0x0042e690, 0x0042e68a/8f/90) whose 12 bytes are all covered
     by the caller's `add esp,0x24` (0x0042e5bd/6b6) besides the two other calls, so
     it consumes three dwords and cleans none itself.

   Frame layout (B + offset), from the listing's lea/byte displacements:
     +0x000 16-byte index table (bytes 0,2,4,6,8,10,12,14,15,13,11,9,7,5,3,1)
     +0x010 0x40-byte target allocation pointer (0x944e46(0x40))
     +0x014 vtable output slot 1 (0x0042e5e7)      read as a dword at 0x0042e621
     +0x018 vtable output slot 2 (0x0042e5eb)      never read back
     +0x01c 0xb2 allocation pointer (0x944cf1(0xb2))
     +0x020 .. +0x07f  sixteen 6-byte candidate slots {u32,u16} filled from seven
                      objects (two slots each) at 0x0042e51a..0x0042e52f; the last
                      two slots are never written by the original
     +0x080, +0x180, +0x280, +0x380  message buffers (0x2e6/0x30b/0x31b paths)
   B+0x480 holds the SEH record (see above).

   Held-concern handling, item by item:
   - 0x0042d620 (vtable +0x3c) stays an explicit indirect boundary.  This file
     evaluates only what the listing fixes: a thiscall receiver (ecx = edi at
     0x0042e5ef), the four pushed arguments and the 16-byte callee pop established
     by the frame arithmetic above.  No branch, field or return convention of that
     callee is reproduced or invented here.
   - The two output slots are left uninitialised 32-bit words because the listing
     only reserves them; they are read back as full dwords (0x0042e621 with esp =
     B-12, [esp+0x24]) and pushed whole to cm_fn_00522e00, so no WORD narrowing or
     zero-extension of the untouched high half is introduced.
   - cm_fn_00522e00 stack slots are passed at the widths the pushes show: the value
     stored at [B+0x14] and the 0xb2 pointer are full dwords (u32), the cp-visible
     16-bit `mov cx,[edi+0x40]` (0x0042e617) is passed as u16 since its upper half is
     the unknown post-call ecx remainder.
   - The null-selected dereference after the 0x30b diagnostic is retained: the
     report block at 0x0042e564..0x0042e5c0 does not return, and 0x0042e5cf then
     loads through the still-null `esi`, exactly as kept below.
   - Entry ESI needs no preservation: the only read of ESI before its definition is
     `or esi,0xffffffff` (0x0042e4ec), whose result is -1 for every input, and no
     saved-register slot is ever read back (the listing's stack reads address the
     frame slots at B+0x00..B+0x1f only).                                              */

#include "cmre.h"

#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

/* Original callees, bound by VA. */
extern u32 CM_THISCALL cm_fn_00522e00(u32 self, u32 owner, s16 count, u32 clubs, u32 rounds,
                                      u32 competition, u32 schedule, u16 year, u8 flag_a,
                                      u8 flag_b, u32 round_flag, u8 flag_c, u32 builder,
                                      u32 numbered, u32 value_38, u32 club_flags);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char *cm_fn_0090d130(u32 param_1, u32 param_2, u32 param_3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

struct cm_42e430_seh {
  void *prev;
  void *handler;
  u32 state;
};

struct cm_42e430_frame {
  u8 opt[16];         /* +0x000 */
  u32 list_alloc;     /* +0x010 */
  u32 out_word_14;    /* +0x014 */
  u32 out_word_18;    /* +0x018 */
  u32 sched_alloc;    /* +0x01c */
  u8 slot[96];        /* +0x020 .. +0x07f */
  u8 msg[0x400];      /* +0x080 .. +0x47f */
};

/* 0x944cff(fmt-blob,0,0,A,B); 0x90d130(A,B,line); sprintf; 0x5e8290; DAT=0.
   The three call sites keep the same (A,B) pairing, only the buffer pair and the
   line number differ (0x0042e4c1..0x0042e4e5 with 0x0042e690, 0x0042e572..0x0042e597,
   0x0042e66b..0x0042e68f).

   The sprintf at 0x0042e6a1 is reached with a single explicit vararg (the 0x90d130
   result pushed at 0x0042e696), but the caller pops all three calls with one
   `add esp,0x24` (0x0042e6b6), so at the call the stack above that vararg still holds
   the 0x90d130 argument list: [esp+0x10] = A, [esp+0x14] = B, [esp+0x18] = the line
   number pushed at 0x0042e4e0 / 0x0042e591 / 0x0042e68a.  The format string at
   CM0102 0x009870e8 is the 12-byte "v%s %s.%s %d" (four conversions), so the original
   consumes all three slots.  They are therefore forwarded as explicit varargs, in
   that order, reproducing the original argument image instead of the caller garbage
   a three-argument call would leave there. */
#define REPORT_ERROR(pa, pb, line_)                                                       \
  do {                                                                                    \
    char *s_42e430;                                                                       \
    cm_fn_00944cff(0x0098aba4u, 0u, 0u, (void *)(pa), (void *)(pb));                       \
    s_42e430 = cm_fn_0090d130((u32)(pa), (u32)(pb), (u32)(line_));                         \
    cm_fn_00944c9f((u32)(&DAT_00ae24d0), 0x009870e8u, (u32)(s_42e430), (u32)(pa),          \
                   (u32)(pb), (u32)(line_));                                              \
    cm_fn_005e8290((const char *)0x009870e0, (const char *)(&DAT_00ae24d0), 0);            \
    DAT_00b67a34 = 0u;                                                                    \
  } while (0)

void CM_THISCALL cm_fn_0042e430(u32 self)
{
  volatile struct cm_42e430_seh seh;
  struct cm_42e430_frame f;
  u8 *obj = (u8 *)self;
  u8 *o;
  u8 *p;
  u8 *best;
  u32 n;
  u32 i;
  s32 j;
  u32 so;
  u32 vtab;
  u32 vcall;
  u32 res;
  u32 v;
  s8 k;

  seh.state = 0xffffffffu;                   /* push -1, 0x0042e430 */
  seh.handler = (void *)0x0095454eu;         /* push 0x95454e, 0x0042e432 */
  seh.prev = CM_FS(void *, 0);               /* 0x0042e437..0x0042e43d */
  CM_FS(void *, 0) = (void *)&seh;           /* 0x0042e43e */

  f.opt[0x0] = 0;                            /* 0x0042e454 bl = 0 */
  f.opt[0x1] = 2;                            /* 0x0042e458 */
  f.opt[0x2] = 4;                            /* 0x0042e45d */
  f.opt[0x3] = 6;                            /* 0x0042e462 */
  f.opt[0x4] = 8;                            /* 0x0042e467 */
  f.opt[0x5] = 0x0a;                         /* 0x0042e46c */
  f.opt[0x6] = 0x0c;                         /* 0x0042e471 */
  f.opt[0x7] = 0x0e;                         /* 0x0042e476 */
  f.opt[0x8] = 0x0f;                         /* 0x0042e47b */
  f.opt[0x9] = 0x0d;                         /* 0x0042e480 */
  f.opt[0xa] = 0x0b;                         /* 0x0042e485 */
  f.opt[0xb] = 9;                            /* 0x0042e48a */
  f.opt[0xc] = 7;                            /* 0x0042e48f */
  f.opt[0xd] = 5;                            /* 0x0042e494 */
  f.opt[0xe] = 3;                            /* 0x0042e499 */
  f.opt[0xf] = 1;                            /* 0x0042e49e */

  f.list_alloc = (u32)cm_fn_00944e46(0x40u);  /* push 0x40 / call, 0x0042e452..0x0042e4a8 */
  if (f.list_alloc == 0u) {                   /* 0x0042e4ab/0x0042e4b1 */
    REPORT_ERROR(&f.msg[0x000], &f.msg[0x100], 0x2e6);
  }
  else {
    j = -1;                                   /* or esi,-1 at 0x0042e4ec */
    so = 0u;
    do {
      if (j == -1) {                          /* 0x0042e4f7..0x0042e4fe */
        o = obj;
      }
      else {
        o = (u8 *)*(u32 *)((u8 *)*(u32 *)(obj + 0xc) + (u32)j * 4u);
      }
      o = (u8 *)*(u32 *)(o + 0xb1);           /* 0x0042e506 */
      *(u32 *)(f.slot + so + 0) = *(u32 *)(o + 0x00);   /* 0x0042e515/0x0042e51a */
      *(u16 *)(f.slot + so + 4) = *(u16 *)(o + 0x19);   /* 0x0042e51d/0x0042e521 */
      *(u32 *)(f.slot + so + 6) = *(u32 *)(o + 0x3b);   /* 0x0042e525/0x0042e52c */
      *(u16 *)(f.slot + so + 10) = *(u16 *)(o + 0x54);  /* 0x0042e528/0x0042e52f */
      j = j + 1;                              /* 0x0042e533 */
      so = so + 12u;
    } while (j < 7);                          /* 0x0042e534/0x0042e537 */

    i = 0u;                                   /* xor ebp,ebp at 0x0042e539 */
    do {
      best = (u8 *)0;                         /* xor esi,esi at 0x0042e53b */
      p = f.slot;                             /* lea eax,[esp+0x30] at 0x0042e53d */
      n = 0x10u;                              /* mov ecx,0x10 at 0x0042e541 */
      do {
        if ((best != (u8 *)0) && (*(s16 *)(p + 4) <= *(s16 *)(best + 4))) {
          /* 0x0042e546..0x0042e552: candidate does not beat the current best */
        }
        else if (*(u32 *)p != 0u) {           /* 0x0042e554..0x0042e558 */
          best = p;
        }
        p = p + 6;                            /* 0x0042e55a */
        n = n - 1u;                           /* 0x0042e55d */
      } while (n != 0u);                      /* 0x0042e55e */

      if (best == (u8 *)0) {                  /* 0x0042e560/0x0042e562 */
        REPORT_ERROR(&f.msg[0x100], &f.msg[0x000], 0x30b);
      }

      k = (s8)f.opt[i];                       /* movsx ecx,[esp+ebp+0x10], 0x0042e5c6 */
      v = *(u32 *)best;                       /* 0x0042e5cf; best may be the null
                                                 selected above, as in the original */
      i = i + 1u;                             /* 0x0042e5d1 */
      *(u32 *)best = 0u;                      /* 0x0042e5d2 */
      *(u32 *)((u8 *)f.list_alloc + (u32)(s32)k * 4u) = v;  /* 0x0042e5cb/0x0042e5d7 */
    } while (i < 0x10u);                      /* 0x0042e5d4/0x0042e5da */

    vtab = *(u32 *)obj;                       /* mov edx,[edi], 0x0042e5e0 */
    vcall = ((u32 (CM_THISCALL *)(u32, u32, u32 *, u32 *, u32))
             *(u32 *)((u8 *)vtab + 0x3c))(self, 7u, &f.out_word_14, &f.out_word_18, 0u);

    f.sched_alloc = (u32)cm_fn_00944cf1(0xb2u);  /* 0x0042e5f4..0x0042e603 */
    seh.state = 0u;                           /* 0x0042e609 */
    if (f.sched_alloc == 0u) {                /* 0x0042e607/0x0042e611 */
      res = 0u;                               /* xor eax,eax at 0x0042e642 */
    }
    else {
      res = cm_fn_00522e00(f.sched_alloc, self, 0x10, f.list_alloc, f.out_word_14,
                           *(u32 *)(obj + 4), vcall, (u16)*(u16 *)(obj + 0x40),
                           (u8)7, (u8)1, f.sched_alloc, (u8)0x14, 0u, 0u, 0u, 0u);
    }
    seh.state = 0xffffffffu;                  /* 0x0042e647 */
    *(u32 *)((u8 *)*(u32 *)(obj + 0xc) + 0x1c) = res;          /* 0x0042e644/0x0042e652 */
    if (*(u32 *)((u8 *)*(u32 *)(obj + 0xc) + 0x1c) != 0u) {    /* 0x0042e655/0x0042e658 */
      cm_fn_009452ca(vcall);                  /* 0x0042e6c1/0x0042e6c2 */
      cm_fn_009452ca(f.list_alloc);           /* 0x0042e6c7/0x0042e6cc */
      *(u32 *)(obj + 0x30) = 7u;              /* 0x0042e6d4 */
    }
    else {
      REPORT_ERROR(&f.msg[0x300], &f.msg[0x200], 0x31b);
    }
  }

  CM_FS(void *, 0) = seh.prev;                /* 0x0042e6db..0x0042e6e5 */
}
