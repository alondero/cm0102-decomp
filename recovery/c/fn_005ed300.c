/* Translation of fn_005ed300 (preferred-image VA 0x005ed300, image base 0x00400000).
   Checked against the original listing. */
#include "cmre.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

#define CONCAT22(hi, lo) ((u32)(((u32)(u16)(hi) << 16) | (u32)(u16)(lo)))

#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00ae3184 CM_GLOBAL(u16, 0x00ae3184)
#define DAT_00aebdec CM_GLOBAL(u16, 0x00aebdec)
#define DAT_00aebe04 CM_GLOBAL(u16, 0x00aebe04)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define DAT_004568f0 CM_GLOBAL(u8, 0x004568f0)
#define IDX CM_GLOBAL(u16, 0x00b77c7a)
#define SELF_TABLE CM_GLOBAL(u32, 0x00b74c4c)

#define S_PATH ((u32)0x009cdc00)
#define S_ERR  ((const char *)0x009870e0)
#define S_FMT  ((u32)0x009870e8)
#define SEH_HANDLER ((void *)0x0095abbe)
#define A53C0 ((void *)0x005a53c0)
#define X568F0 ((u32)0x004568f0)

extern u32 CM_THISCALL cm_fn_0055f990(u32 self,
  volatile u32 a0, volatile u32 a1, volatile u32 a2, volatile u32 a3,
  volatile u32 a4, volatile u32 a5, volatile u32 a6, volatile u32 a7,
  volatile u32 a8, volatile u32 a9, volatile u32 a10, volatile u32 a11,
  volatile u32 a12, volatile u32 a13, volatile u32 a14, volatile u32 a15,
  volatile u32 a16, volatile u32 a17);
extern u32 CM_THISCALL cm_fn_0055fba0(u32 self, u32 a0, u32 a1, u32 a2,
  u32 a3, u32 a4, u32 a5, u32 a6, u32 a7, u32 a8, u32 a9, u32 a10);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern void CM_CDECL cm_fn_0090ba90(u8 *a, u8 *b, u32 count);
/* 0x90d130 returns the version string pointer (0x00ad4b94 in the pinned image).
   The original callee returns with `ret` only: it does NOT pop its three
   stack arguments.  The sprintf at 0x005ed73c therefore reads its varargs
   from those leftover slots: [esp+16] = the filename buffer, [esp+20] = the
   extension buffer, [esp+24] = 0xc1, behind the pushed version pointer.
   The call below passes all four varargs explicitly so the stack layout
   matches the original byte for byte at every optimisation level. */
extern char * cm_fn_0090d130(u32 a0, u32 a1, u32 a2);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern void CM_STDCALL cm_fn_009462bb(u32 param_1, u32 param_2, int param_3, void *param_4);
extern void CM_STDCALL cm_fn_009463af(u32 param_1, u32 param_2, int param_3, u32 param_4, u32 param_5);

u32
cm_fn_005ed300(int param_1, u32 param_2, int param_3, u32 param_4,
               u32 param_5, u32 param_6, int param_7, int param_8,
               int param_9, u32 param_10, int param_11)
{
  volatile struct { void *prev; void *handler; u32 state; } frame;
  /* Scratch at S-0x22c..; the two 256-byte buffers are S-0x20c and S-0x10c. */
  u8 scratch[0x220];
  u8 *buf20c = scratch + 0x20;
  u8 *buf10c = scratch + 0x120;

  u32 ret, P, row_ptr, w, cur, src, dst;
  u32 mask = param_10;
  int i, k, oi, ij;
  u32 bit_i, bit_j, delta;
  int cmpres;
  u16 *ps;
  u32 fba0_result, base_row, pos_a, base_col;
  int ctr, remaining, col, row, width;
  s16 idx;
  u32 ea, self;
  u32 color_local;
  u32 ucolor, a8, a9, uVar8;
  u16 f, uVar9;
  u32 v_ca, v_ce;

  /* 005ed300-005ed30e: link the SEH frame */
  frame.prev = CM_FS(void *, 0);
  frame.handler = SEH_HANDLER;
  frame.state = 0xffffffffu;
  CM_FS(void *, 0) = (void *)&frame;

  if (param_8 <= 0 || param_9 == 0) {
    /* 005ed6fe: error path */
    cm_fn_00944cff(S_PATH, 0, 0, buf10c, buf20c);
    ret = (u32)cm_fn_0090d130((u32)buf10c, (u32)buf20c, 0xc1);
    /* 005ed731-005ed73c: push the version pointer, the format and the
       destination; the three arguments of the preceding 0x90d130 call are
       still on the stack and are read as the remaining varargs. */
    cm_fn_00944c9f((u32)&DAT_00ae24d0, S_FMT, ret, (u32)buf10c, (u32)buf20c,
                   193u);
    cm_fn_005e8290(S_ERR, (const char *)&DAT_00ae24d0, 0);
    DAT_00b67a34 = 0;
    ret = 0;
  } else {
    /* 005ed33e-005ed356: allocate and initialise */
    P = (u32)cm_fn_00944cf1((u32)param_8 * 0xd4u + 4u);
    frame.state = 0;
    if (P == 0) {
      row_ptr = 0;
    } else {
      row_ptr = P + 4;
      *(volatile u32 *)P = (u32)param_8;
      cm_fn_009463af(row_ptr, 0xd4, param_8, X568F0, (u32)A53C0);
    }
    frame.state = 0xffffffffu;

    /* 005ed3a0-005ed3b8: copy param_8 rows of 0xd4 bytes */
    if (param_8 > 0) {
      src = (u32)param_9;
      dst = row_ptr;
      i = param_8;
      do {
        for (k = 0x35; k != 0; k--) {
          *(volatile u32 *)dst = *(volatile u32 *)src;
          src += 4;
          dst += 4;
        }
        i--;
      } while (i != 0);
    }

    /* 005ed3c0-005ed4da: duplicate-row detection and bit swap */
    w = row_ptr + 0xd4;
    if (param_11 != 0 && param_8 - 1 > 0) {
      oi = 0;
      do {
        ij = oi + 1;
        cur = w;
        for (int j = ij; j < param_8; j++) {
          u8 *pa = (u8 *)(w - 0xd0);
          u8 *pb = (u8 *)(cur + 4);
          cmpres = 0;
          for (;;) {
            u8 b1 = *pa;
            u8 b2 = *pb;
            if (b1 != b2) {
              cmpres = (b1 < b2) ? -1 : 1;
              break;
            }
            if (b1 == 0) break;
            b1 = pa[1];
            b2 = pb[1];
            if (b1 != b2) {
              cmpres = (b1 < b2) ? -1 : 1;
              break;
            }
            pa += 2;
            pb += 2;
            if (b1 == 0) break;
          }
          if (cmpres > 0) {
            cm_fn_0090ba90((u8 *)(w - 0xd4), (u8 *)cur, 0xd4);
            bit_i = 1u << ((u8)oi & 0x1f);
            bit_j = 1u << ((u8)j & 0x1f);
             if ((bit_i & mask) == 0) {
               if ((mask & bit_j) != 0) {
                 delta = bit_i - bit_j;
              } else {
                goto no_mask_change;
              }
            } else {
              if ((mask & bit_j) != 0) goto no_mask_change;
               delta = bit_j - bit_i;
            }
            mask = mask + (u32)delta;
          no_mask_change:
            (void)0;
          }
          cur += 0xd4;
        }
        w += 0xd4;
        oi = ij;
      } while (ij < param_8 - 1);
    }
    color_local = w;

    /* 005ed4da-005ed524: thiscall receiver from the table */
    idx = (s16)IDX;
    ea = ((u32)idx * 3u) << 8;
    self = *(volatile u32 *)(ea + 0xb74c4cu);
    fba0_result = cm_fn_0055fba0(self, (u32)param_1, param_2, (u32)param_3,
      param_4, param_5, 0, param_6, 0, 1, 0, 0xffffffffu);

    col = 0;
    row = 0;
    ctr = 0;
    base_col = 0;
    base_row = (int)fba0_result;
    width = (int)(param_5 & 0xffu);
    pos_a = 0;
    ps = (u16 *)(row_ptr + 2);
    if (param_8 > 0) {
      remaining = param_8;
      do {
        if ((ctr % width == 0) && (remaining < width)) {
          base_row = (u32)((s32)((u32)param_3 - (u32)width * 2u - (u32)param_1 + 3u) / width);
          pos_a = (u32)((s32)((u32)param_3 - (base_row + 2u) * (u32)remaining - (u32)param_1 + 3u) / 2) + (u32)param_1;
          base_col = base_row - 1u + pos_a;
        }
        uVar9 = (u16)(mask >> 16);
        ucolor = (mask & (1u << ((u8)ctr & 0x1f))) ? 0x822u : 0x22u;
        f = *ps;
        if (f != 0) {
          color_local = CONCAT22(uVar9, f);
        } else if (ucolor == 0x822u) {
          color_local = (color_local & 0xffff0000u) | (u32)DAT_00aebdec;
        } else {
          color_local = (color_local & 0xffff0000u) | (u32)DAT_00ae3184;
        }
        if (ps[-1] == (u16)0xffff) {
          uVar8 = (u32)param_7;
        } else {
          uVar8 = CONCAT22(uVar9, ps[-1]);
        }
        a8 = CONCAT22((u16)(uVar8 >> 16), DAT_00aebe04);
        a9 = CONCAT22((u16)(color_local >> 16), DAT_00aebe04);
        v_ce = *(volatile u32 *)((u8 *)ps + 0xce);
        v_ca = *(volatile u32 *)((u8 *)ps + 0xca);
        idx = (s16)IDX;
        self = *(volatile u32 *)((((u32)idx * 3u) << 8) + 0xb74c4cu);
        cm_fn_0055f990(self,
          2, pos_a, 0, (u32)base_col, 0,
          (u32)col, (u32)row, ucolor, a8, a9, 0xc,
          uVar8, color_local, (u32)((u8 *)ps + 2), 0, v_ca, v_ce,
          fba0_result);
        if (pos_a != 0 && base_col != 0) {
          pos_a = pos_a + (u32)base_row + 2u;
          base_col = base_row + 2 + base_col;
        }
        col++;
        if (col == width) {
          col = 0;
          row++;
        }
        ctr++;
        remaining--;
        ps += 0x6a;
      } while (ctr < param_8);
    }

    if (row_ptr != 0) {
      cm_fn_009462bb(row_ptr, 0xd4, (int)*(volatile u32 *)(row_ptr - 4), (void *)A53C0);
      cm_fn_00944c94((void *)(row_ptr - 4));
    }
    ret = 1;
  }

  CM_FS(void *, 0) = frame.prev;
  return ret;
}
