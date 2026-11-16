/* fn_007fe600 -- scrman.cpp, preferred-image VA 0x007fe600, 1307 bytes, `ret 0x14`
   (5 stack arguments, receiver in ECX; image base 0x00400000).

   Direction evidence: CM0102 3.9.68, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preferred-image VA 0x007fe872 calls 0x00944cf1; no subsequent instruction
   clears DF before REP STOSD at 0x007fe88c. Sample architectural flags after
   that call, not on entry. No call or DF write separates that REP from
   REPNE SCASB (0x007fe8c2), REP MOVSD (0x007fe8dd), REP MOVSB (0x007fe8e4).
   All four therefore use the same sampled direction. This is target-level
   state transport, not a claim about any callee's implementation or ABI.

   Slot record: base = self + 0x300 * (s16)self[0x382a] (0x007fe60c..0x007fe61f).
   Node records are 0x300-byte heap blocks (0x007fe86d/0x007fe872), zeroed by
   REP STOSD, then filled:
       +0x000 arg1    +0x004 arg4    +0x008 arg2    +0x00c arg5
       +0x010 third argument's home (see below)
       +0x014 0x3c pointer pairs (free(pair[0]) when both dwords are nonzero)
       +0x1f4 back link, +0x1f8 next link, +0x1fc copied literal (0x009c3730)

   Argument homes: thiscall homes are L+0x20c..L+0x21c for the five arguments
   (L = ESP after `sub esp,0x208`).  0x007fe882 loads `[esp+0x228]` while ESP is
   still one dword below L-16 because of the outstanding `push 0x300`, i.e. the
   physical address is L+0x214 = the *third* argument's caller-owned home (the
   same value 0x007fe6ac tests for the duplicate search).  It is held in `pri`
   and reused for the 0x7e6 counter, the 0x7f2 clear and record+0x10.
*/

#include "cmre.h"

/* literal bytes in the original image */
#define STR_00A7DDBC ((u32)0x00a7ddbc) /* 0x007feabf */
#define STR_009870E0 ((const char *)0x009870e0)
#define STR_009870E8 ((u32)0x009870e8)
#define ADDR_00AE24D0 ((u32)0x00ae24d0)
#define ADDR_009C3730 ((u32)0x009c3730)

#define DAT_00b67a34 CM_GLOBAL(volatile u32, 0x00b67a34)

/* Explicit widths for every original memory access; volatile keeps each
   original reload (the slot fields and node links are written as DWORDs that
   overlap the pointer-typed views). */
#define LD32(p, off) (*(volatile u32 *)((u8 *)(u32)(p) + (u32)(off)))
#define ST32(p, off, v) \
  (*(volatile u32 *)((u8 *)(u32)(p) + (u32)(off)) = (u32)(v))
#define LD16(p, off) (*(volatile u16 *)((u8 *)(u32)(p) + (u32)(off)))
#define LD16S(p, off) (*(volatile s16 *)((u8 *)(u32)(p) + (u32)(off)))
#define ST16(p, off, v) \
  (*(volatile u16 *)((u8 *)(u32)(p) + (u32)(off)) = (u16)(v))
#define LD8(p, off) (*(volatile u8 *)((u8 *)(u32)(p) + (u32)(off)))
#define ST8(p, off, v) \
  (*(volatile u8 *)((u8 *)(u32)(p) + (u32)(off)) = (u8)(v))

/* 0x007fe60c: movsx eax,[self+0x382a]; lea eax,[eax+eax*2]; shl eax,8 */
#define IDXOFF(s) \
  (((u32)(s32)(*(volatile s16 *)((u8 *)(u32)(s) + 0x382a))) * 0x300u)
#define SLOT(s, off) ((u32)(s) + IDXOFF(s) + (u32)(off))

extern void cm_fn_005e8290(const char *param_1, const char *param_2,
                           int param_3);
extern void CM_THISCALL cm_fn_007ff270(int param_1, int param_2,
                                      char *param_3, int param_4);
/* 0x007feade: no arguments.  The three pushes at 0x007fead7/0x007feadc/0x007feadd
   (0x4ed, L+0x08, L+0x108) are the deferred trailing arguments of the following
   0x944c9f call; the shared `add esp,0x24` at 0x007feb03 covers 9 dwords =
   6 (0x944c9f) + 3 (0x005e8290), none for this callee. */
extern char *CM_CDECL cm_fn_0090d130(void);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                   void *arg5);

u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 arg1, u32 arg2, u32 arg3,
                               u32 arg4, u32 arg5)
{
  u8 scratch[0x200];          /* [L+0x08] .. [L+0x208) */
  u8 *buf_a = scratch;        /* [L+0x08]  */
  u8 *buf_b = scratch + 0x100;/* [L+0x108] */
  u32 flag;                   /* [L+0x00] */
  u32 cur;                    /* edx from 0x007fe6a2 */
  u32 head, next, old, node, p;
  u32 i, n;
  u32 pri, step, src, dst, remaining;

  flag = 0;                   /* 0x007fe618 */
  /* 0x007fe62d..0x007fe684 */
  if (LD32(SLOT(self, 0x7d6), 0) == 0 && LD32(SLOT(self, 0x7da), 0) == 0 &&
      LD32(self, 0x37d4) != 0) {
    u32 v_d8 = LD32(self, 0x37d8);
    u32 v_d4 = LD32(self, 0x37d4);
    u32 v_dc = LD32(self, 0x37dc);
    u32 v_e0;
    arg2 = v_d8;              /* 0x007fe64d */
    v_e0 = LD32(self, 0x37e0);
    arg3 = v_dc;              /* 0x007fe65a */
    arg1 = v_d4;              /* 0x007fe667 */
    arg4 = v_e0;              /* 0x007fe66e */
    arg5 = LD32(self, 0x37e4);/* 0x007fe675 */
    flag = 1;                 /* 0x007fe67c */
  }

  /* 0x007fe68d / 0x007fe695 */
  if (arg1 == 0 || arg2 == 0) {
    /* 0x007feab0..0x007feb06 */
    u32 v;
    cm_fn_00944cff(STR_00A7DDBC, 0, 0, buf_b, buf_a);
    /* 0x007feade: cm_fn_0090d130() runs between the deferred pushes of
       cm_fn_00944c9f's last three arguments and its first three.  The format
       literal at 0x009870e8 has four conversions ("v%s %s %s %d"), which only
       a six-argument 0x944c9f call satisfies. */
    v = (u32)cm_fn_0090d130();
    /* 0x007feae3..0x007feaee */
    cm_fn_00944c9f(ADDR_00AE24D0, STR_009870E8, v, buf_b, buf_a, 0x4ed);
    cm_fn_005e8290(STR_009870E0, (const char *)ADDR_00AE24D0, 0);
    DAT_00b67a34 = 0;
    return 0;
  }

  /* 0x007fe6a2: edx = slot[0x7e2] */
  cur = LD32(SLOT(self, 0x7e2), 0);
  /* 0x007fe6ac: the duplicate search needs the third argument and a 0x7da list */
  if (cur != 0 && arg3 != 0) {
    head = LD32(SLOT(self, 0x7da), 0);
    if (head != 0) {
      p = head;
      for (;;) {                          /* 0x007fe6bf */
        if (LD32(p, 0x10) != 0 && LD32(p, 0) == arg1) {
          /* 0x007fea59: walk on from the match, re-pointing 0x7e2 at every
             node whose +0x10 is nonzero */
          while (p != 0) {
            if (LD32(p, 0x10) != 0) ST32(SLOT(self, 0x7e2), 0, p);
            p = LD32(p, 0x1f8);
          }
          /* 0x007fea80: 0x7e2 == 0 would fault in the original too */
          if (LD32(LD32(SLOT(self, 0x7e2), 0), 0) == arg1) return 0;
          ST32(SLOT(self, 0x7e2), 0, cur); /* 0x007feaa1 */
          return 0;
        }
        p = LD32(p, 0x1f8);               /* 0x007fe6cc */
        if (p == 0) break;
      }
    }
  }

  /* 0x007fe6d6: slot[0x7e2] != slot[0x7de] -> relink and free the old chain */
  if (cur != LD32(SLOT(self, 0x7de), 0)) {
    ST32(SLOT(self, 0x7de), 0, cur);
    head = LD32(SLOT(self, 0x7e2), 0);    /* 0x007fe6f5 */
    next = LD32(head, 0x1f8);             /* 0x007fe6ff */
    old = LD32(SLOT(self, 0x7de), 0);     /* 0x007fe705 */
    ST32(old, 0x1f8, 0);                  /* 0x007fe70d */
    while (next != 0) {                   /* 0x007fe78e */
      u32 nn = LD32(next, 0x1f8);         /* 0x007fe71c */
      p = next + 0x14;                    /* 0x007fe722 */
      i = 0x3c;                           /* 0x007fe728 */
      ST32(SLOT(self, 0x7e2), 0, nn);     /* 0x007fe730 */
      do {                                /* 0x007fe737 */
        u32 a = LD32(p, 0);
        if (a != 0) {
          u32 b = LD32(p, 4);
          if (b != 0) cm_fn_00944c94((void *)a);
        }
        p += 8;
        i--;
      } while (i != 0);
      cm_fn_00944c94((void *)next);       /* 0x007fe753 */
      ST16(SLOT(self, 0x7e8), 0,          /* 0x007fe769 dec word */
           (u16)(LD16(SLOT(self, 0x7e8), 0) - 1u));
      next = LD32(SLOT(self, 0x7e2), 0);  /* 0x007fe785 */
    }
    ST32(SLOT(self, 0x7e2), 0, LD32(SLOT(self, 0x7de), 0)); /* 0x007fe7a9 */
  }

  /* 0x007fe7be: signed word compare against 100 */
  if ((s32)LD16S(SLOT(self, 0x7e8), 0) > 100) {
    node = LD32(SLOT(self, 0x7da), 0);    /* 0x007fe7cc */
    if (LD32(node, 0x10) != 0) return 0;  /* 0x007fe7d5 */
    p = node + 0x14;                      /* 0x007fe7e1 */
    next = LD32(node, 0x1f8);             /* 0x007fe7db */
    ST32(SLOT(self, 0x7da), 0, next);     /* 0x007fe7e4 */
    /* 0x007fe7fe: the new 0x7da is dereferenced without a null test, as in
       the original */
    ST32(LD32(SLOT(self, 0x7da), 0), 0x1f4, 0);
    i = 0x3c;                             /* 0x007fe804 */
    do {                                  /* 0x007fe809 */
      u32 a = LD32(p, 0);
      if (a != 0) {
        u32 b = LD32(p, 4);
        if (b != 0) cm_fn_00944c94((void *)a);
      }
      p += 8;
      i--;
    } while (i != 0);
    cm_fn_00944c94((void *)node);         /* 0x007fe826 */
    ST16(SLOT(self, 0x7e8), 0,            /* 0x007fe83b dec word */
         (u16)(LD16(SLOT(self, 0x7e8), 0) - 1u));
  }

  /* 0x007fe859: call the node's +0x04 slot; ECX is not set up by the original
     for this dispatch, so it is a no-argument cdecl call. */
  node = LD32(SLOT(self, 0x7e2), 0);
  if (node != 0) {
    u32 f = LD32(node, 4);
    if (f != 0) ((void(CM_CDECL *)(void))(u32)f)();
  }

  /* 0x007fe86d/0x007fe872: malloc(0x300) */
  node = (u32)cm_fn_00944cf1(0x300);
  step = (__builtin_ia32_readeflags_u32() & 0x400u) ? 0xffffffffu : 1u;
  /* 0x007fe882: [esp+0x228] with ESP one dword low == the third argument's home */
  pri = arg3;
  /* 0x007fe879..0x007fe88c: REP STOSD, including DF=1. */
  p = node;
  for (i = 0xc0; i != 0; i--) {
    ST32(p, 0, 0);
    p += step * 4u;
  }
  ST32(node, 0x00, arg1);                 /* 0x007fe89c */
  ST32(node, 0x04, arg4);                 /* 0x007fe8a5 */
  ST32(node, 0x0c, arg5);                 /* 0x007fe8af */
  ST32(node, 0x08, arg2);                 /* 0x007fe8b2 */
  ST32(node, 0x10, pri);                  /* 0x007fe8bf */
  /* 0x007fe8c2: REPNE SCASB with ECX initially 0xffffffff. */
  p = ADDR_009C3730;
  remaining = 0xffffffffu;
  do {
    u8 b = LD8(p, 0);
    p += step;
    remaining--;
    if (b == 0) break;
  } while (remaining != 0);
  n = ~remaining;                         /* 0x007fe8c4 */
  src = p - n;                           /* 0x007fe8cc, even when DF=1 */
  dst = node + 0x1fc;
  for (i = n >> 2; i != 0; i--) {         /* 0x007fe8dd */
    u32 value = LD32(src, 0);
    ST32(dst, 0, value);
    src += step * 4u;
    dst += step * 4u;
  }
  for (i = n & 3u; i != 0; i--) {         /* 0x007fe8e4 */
    u8 value = LD8(src, 0);
    ST8(dst, 0, value);
    src += step;
    dst += step;
  }

  /* 0x007fe8e8 */
  if (pri != 0) {
    ST16(SLOT(self, 0x7e6), 0,
         (u16)(LD16(SLOT(self, 0x7e6), 0) + 1u));
  }
  /* 0x007fe908..0x007fe93a */
  old = LD32(SLOT(self, 0x7de), 0);
  if (old != 0) {
    ST32(old, 0x1f8, node);
    ST32(node, 0x1f4, LD32(SLOT(self, 0x7de), 0));
  }
  ST32(SLOT(self, 0x7de), 0, node);       /* 0x007fe94d */
  /* 0x007fe961 */
  if (LD32(SLOT(self, 0x7da), 0) == 0) {
    ST32(SLOT(self, 0x7da), 0, node);
  }
  ST32(SLOT(self, 0x7e2), 0, node);       /* 0x007fe982 */
  ST16(SLOT(self, 0x7e8), 0,              /* 0x007fe996 inc word */
       (u16)(LD16(SLOT(self, 0x7e8), 0) + 1u));
  ST32(SLOT(self, 0x7ee), 0, 0);          /* 0x007fe9b2 */
  ST16(SLOT(self, 0x7f6), 0, 0xffffu);    /* 0x007fe9c6 */
  ST32(SLOT(self, 0x7f8), 0, 0);          /* 0x007fe9df */
  if (pri != 0) {
    ST32(SLOT(self, 0x7f2), 0, 0);        /* 0x007fe9f5 */
  }
  ST32(SLOT(self, 0x7ea), 0, 1);          /* 0x007fea0f */

  /* 0x007fea1c: refresh the caller's vector entries through one thiscall */
  if (flag != 0) {
    s16 si = 0;
    if ((s32)(*(volatile s16 *)((u8 *)(u32)self + 0x37ec)) > 0) {
      do {
        cm_fn_007ff270(
            (int)self, (int)si,
            (char *)(u32)LD32(LD32(self, 0x37e8), (u32)(s32)si * 4), 0);
        si = (s16)(si + 1);
      } while (si < (s32)(*(volatile s16 *)((u8 *)(u32)self + 0x37ec)));
    }
  }
  return 1;                               /* 0x007fea4a */
}
