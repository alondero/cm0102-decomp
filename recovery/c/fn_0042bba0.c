/* fn_0042bba0 — preferred-image VA 0x0042bba0, image base 0x00400000.
   Pinned binary: CM0102 3.9.68 cm0102.exe
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1c627a22c445d14d489bd9bff86.

   Frame: `sub esp,0xa0` -> L. After `push ebx` the twelve-DWORD object table
   lives at L+0x40; after `push ebp/esi/edi` the scratch locals are
      L+0x00  scratch int used by fild/fidiv (0x0042bcdb, 0x0042bced)
      L+0x04  best score word (0x0042bc95, 0x0042bcc6, 0x0042bccf)
      L+0x08  outer group count (0x0042bc87, 0x0042bd10)
      L+0x0c  (unused gap)
      L+0x10  selected-entry DWORD, 12 slots (0x0042bce5 / tail reads)
      L+0x40  object table, 12 DWORDs (0x0042bbb4..0x0042bc43)
      L+0x70  selected-entry ratio, 12 floats (0x0042bcf5 / tail reads)

   The selected-entry DWORD/ratio slots are written only when a group yields a
   lookup hit whose BYTE at +0x73 is below the group's current best
   (0x0042bcc1..0x0042bcf5). The tail then reads all twelve ID slots and the
   ratios of groups 4..10 unconditionally (0x0042bd2d..0x0042be19), so a group
   with no qualifying candidate contributes whatever stale bytes those stack
   slots already held. They are therefore declared here with no initializer and
   are never clamped or filled in. */
#include "cmre.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

#define DAT_009cf994 CM_GLOBAL(int, 0x009cf994)
#define DAT_009cf998 CM_GLOBAL(int, 0x009cf998)
#define DAT_009cf9a4 CM_GLOBAL(int, 0x009cf9a4)
#define DAT_009cf9b0 CM_GLOBAL(int, 0x009cf9b0)
#define DAT_009cf9b4 CM_GLOBAL(int, 0x009cf9b4)
#define DAT_009cf9b8 CM_GLOBAL(int, 0x009cf9b8)
#define DAT_009cf9bc CM_GLOBAL(int, 0x009cf9bc)
#define DAT_009cf9c0 CM_GLOBAL(int, 0x009cf9c0)
#define DAT_009cf9c8 CM_GLOBAL(int, 0x009cf9c8)
#define DAT_009cf9d0 CM_GLOBAL(int, 0x009cf9d0)
#define DAT_009cf9d4 CM_GLOBAL(int, 0x009cf9d4)
#define DAT_009cf9dc CM_GLOBAL(int, 0x009cf9dc)
#define DAT_009cf9e4 CM_GLOBAL(int, 0x009cf9e4)
#define DAT_00adadfc CM_GLOBAL(int, 0x00adadfc)

/* 0x0042bcb8: ECX = [ebx+8] (receiver), one stack target, boolish u32 result. */
extern u32 CM_THISCALL cm_fn_004a4850(u32 self, u32 target);
/* 0x0042be2a: ECX = [edx+eax*4], no stack arguments. */
extern void CM_FASTCALL cm_fn_00427320(int *param_1);

/* 0x0042bc57: `mov edx,[ecx]; call dword ptr [edx+0x10]` with ECX = object and
   pushes 0 then 1; only AL is consumed (0x0042bc5a). The target is original
   code reached through the object's vtable, so it stays an explicit indirect
   boundary with the listing's thiscall receiver and argument order. */
typedef u8 (CM_THISCALL *cm_vt_u8_fn)(void *self, u32 a, u32 b);

void cm_fn_0042bba0(void)
{
  u32 table[12];        /* L+0x40 */
  u32 sel_id[12];       /* L+0x10: intentionally uninitialized (stale) */
  float sel_score[12];  /* L+0x70: intentionally uninitialized (stale) */
  u32 base;
  s8 g;
  s32 group;
  s32 groups_left;
  u32 obj;
  s32 off;
  s32 n;
  s16 best;
  u32 self;
  u32 target;
  u16 raw;
  u32 ent;
  u32 field;
  u32 p;
  u32 vtbl;
  cm_vt_u8_fn method;

  g = 0;
  /* 0x0042bba6: EAX (the array base) is loaded once and kept for all twelve
     index reads below; every index global is re-read at its own instruction. */
  base = (u32)DAT_00adadfc;
  table[0] = *(u32 *)(base + (u32)DAT_009cf9e4 * 4);
  table[1] = *(u32 *)(base + (u32)DAT_009cf9dc * 4);
  table[2] = *(u32 *)(base + (u32)DAT_009cf9d4 * 4);
  table[3] = *(u32 *)(base + (u32)DAT_009cf9d0 * 4);
  table[4] = *(u32 *)(base + (u32)DAT_009cf998 * 4);
  table[5] = *(u32 *)(base + (u32)DAT_009cf9bc * 4);
  table[6] = *(u32 *)(base + (u32)DAT_009cf9c0 * 4);
  table[7] = *(u32 *)(base + (u32)DAT_009cf9b0 * 4);
  table[8] = *(u32 *)(base + (u32)DAT_009cf9b4 * 4);
  table[9] = *(u32 *)(base + (u32)DAT_009cf994 * 4);
  table[10] = *(u32 *)(base + (u32)DAT_009cf9a4 * 4);
  table[11] = *(u32 *)(base + (u32)DAT_009cf9c8 * 4);

  /* 0x0042bc4a: bl is the byte index and is sign-extended for the table read. */
  do {
    obj = table[(s32)g];
    vtbl = *(u32 *)obj;
    method = (cm_vt_u8_fn)*(u32 *)(vtbl + 0x10);
    if (method((void *)obj, 1, 0) == 0) {
      return;                    /* 0x0042bc5c -> 0x0042be2f */
    }
    g = (s8)(g + 1);
  } while ((s32)g < 0xc);

  /* 0x0042bc69: fresh base/index reads, unaligned DWORD probe at +0x45. */
  base = (u32)DAT_00adadfc;
  n = (s32)DAT_009cf9b8;
  if (*(u32 *)(*(u32 *)(base + (u32)n * 4) + 0x45) != 0) {
    return;                      /* 0x0042bc7c -> 0x0042be2f */
  }

  group = 0;
  groups_left = 12;
  do {
    obj = table[group];
    off = 0;
    best = 100;                  /* 0x0042bc95, re-established per group */
    n = 0;                       /* L+0x1c entry counter */
    if (*(s16 *)(obj + 0x3e) > 0) {   /* 0x0042bca1 signed word compare */
      field = obj + 0xb1;        /* lea edi,[ebx+0xb1] at 0x0042bca7 */
      do {
        u32 list;
        u32 r;
        list = *(u32 *)field;    /* 0x0042bcad / 0x0042bcaf */
        target = *(u32 *)(list + (u32)off);
        self = *(u32 *)(obj + 8);
        r = cm_fn_004a4850(self, target);
        if (r != 0) {
          raw = *(u8 *)(r + 0x73);   /* movzx cx, byte */
          if ((s16)raw < best) {     /* 0x0042bcc6 signed word compare */
            s16 num;
            s8 den;
            list = *(u32 *)field;    /* 0x0042bccd reload */
            best = (s16)raw;         /* 0x0042bccf DWORD store, low word used */
            num = *(s16 *)(list + off + 0x19);
            ent = list + (u32)off;
            sel_id[group] = *(u32 *)ent;              /* 0x0042bce5 */
            den = *(s8 *)(ent + 0x11);
            sel_score[group] = (float)num / (float)(s32)den;
          }
        }
        off = off + 0x3b;
        n = n + 1;
        /* 0x0042bd03: the per-group count is re-read from the object. */
      } while (n < (s32)*(s16 *)(obj + 0x3e));
    }
    group = group + 1;
    groups_left = groups_left - 1;
  } while (groups_left != 0);

  /* Tail. 0x0042bd22: fresh base/index reads; ECX = current object. */
  base = (u32)DAT_00adadfc;
  n = (s32)DAT_009cf9b8;
  obj = *(u32 *)(base + (u32)n * 4);

  /* 0x0042bd48..0x0042bd70: five DWORD ID stores, each preceded by its own
     reload of [obj+0x14] (0x0042bd42, 0x0042bd4b, 0x0042bd55, 0x0042bd5f,
     0x0042bd69). */
  p = *(volatile u32 *)(obj + 0x14);
  *(u32 *)(p + 0x12) = sel_id[1];
  p = *(volatile u32 *)(obj + 0x14);
  *(u32 *)(p + 0x18) = sel_id[0];
  p = *(volatile u32 *)(obj + 0x14);
  *(u32 *)(p + 0x1e) = sel_id[2];
  p = *(volatile u32 *)(obj + 0x14);
  *(u32 *)(p + 0x24) = sel_id[11];
  p = *(volatile u32 *)(obj + 0x14);
  *(u32 *)(p + 0x2a) = sel_id[3];

  /* 0x0042bd2d/0x0042bd3b: fld [L+0x94]=score[9]; fcomp [L+0x98]=score[10].
     0x0042bd7a..0x0042bdb8 picks the winner of groups 9/10 only if it beats
     group 4's ratio, otherwise group 4. */
  if (sel_score[9] > sel_score[10]) {
    u32 v = (sel_score[9] > sel_score[4]) ? sel_id[9] : sel_id[4];
    p = *(volatile u32 *)(obj + 0x14);
    *(u32 *)p = v;
  } else {
    u32 v = (sel_score[10] > sel_score[4]) ? sel_id[10] : sel_id[4];
    p = *(volatile u32 *)(obj + 0x14);
    *(u32 *)p = v;
  }

  /* 0x0042bdb8: fld score[8]; fcomp score[7]; winner stored at +6. */
  {
    u32 v = (sel_score[8] > sel_score[7]) ? sel_id[8] : sel_id[7];
    p = *(volatile u32 *)(obj + 0x14);
    *(u32 *)(p + 6) = v;
  }

  /* 0x0042bdda: fld score[6]; fcomp score[5]. The two store orders are the
     original's own: greater takes 6 into +0x30 and 5 into +0xc (0x0042bdf2),
     otherwise 5 into +0x30 and 6 into +0xc (0x0042be08). */
  if (sel_score[6] > sel_score[5]) {
    p = *(volatile u32 *)(obj + 0x14);
    *(u32 *)(p + 0x30) = sel_id[6];
    p = *(volatile u32 *)(obj + 0x14);
    *(u32 *)(p + 0xc) = sel_id[5];
  } else {
    p = *(volatile u32 *)(obj + 0x14);
    *(u32 *)(p + 0x30) = sel_id[5];
    p = *(volatile u32 *)(obj + 0x14);
    *(u32 *)(p + 0xc) = sel_id[6];
  }

  /* 0x0042be1c: fresh base/index reads, ECX = object, no stack arguments. */
  base = (u32)DAT_00adadfc;
  n = (s32)DAT_009cf9b8;
  cm_fn_00427320((int *)*(u32 *)(base + (u32)n * 4));
}
