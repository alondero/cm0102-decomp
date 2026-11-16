/* Preferred-image VA 0x006be710, image base 0x00400000, pinned SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   Scratch-state boundary (resolves the HELD note without narrowing or zeroing):
   the listing's `sub esp, 0x18` frame is one allocation, modelled as `L` below.
   Original offsets from L (L = esp after `sub esp,0x18`):
     L+0x03  drain loop counter (byte store 0x006becfa, byte loads 0x006bed4c)
     L+0x04  local (200 - ...)*2, dword only (0x006bea0c/0x006beac9/0x006becbb)
     L+0x08  char save as a full dword (0x006be9ea), then partial byte stores
             (0x006beb2e/0x006bec2e), then read back as a full dword
             (0x006beb5e/0x006bec2e) and pushed whole: upper bytes stay whatever
             the sign-extended char store left.
     L+0x0c  saved record pointer (0x006be98b)
     L+0x10  700bd0 in-byte: only the low byte is ever stored (0x33/0xff), upper
             three bytes are never written; pushed as a full DWORD to 0x00702050.
     L+0x14  700bd0 out-WORD: only two bytes are written by 0x00700bd0; pushed as
             a full DWORD to 0x00702050.
   No byte above is initialized or narrowed by this translation: each partial
   write keeps its original width and each full-DWORD push re-reads the whole
   slot at the push site. The slots are kept adjacent in one object so the
   cross-width aliasing (byte store -> dword load) is preserved. */

#include "cmre.h"

typedef union cmre_frame710_u {
  u8 b[0x18];
  u16 w[12];
  u32 d[6];
} cmre_frame710;

/* 0x006be520: ECX = self. */
extern u32 CM_FASTCALL cm_fn_006be520(u32 self);
/* 0x006ce110: ECX = self, one stack arg. */
extern u32 CM_THISCALL cm_fn_006ce110(volatile u8 *self, s8 selector);
/* 0x006d6f70: ECX = message sink from [esi+0x8f7a], seven stack args.  a4 is
   pushed as full EBX whose upper 24 bits are the record pointer returned by
   0x006ce110, so the parameter is 32 bits and carries those bytes. */
extern void CM_THISCALL cm_fn_006d6f70(u32 self, u32 message, u32 object,
                                       u32 a3, u32 a4, s32 a5, s32 a6, u32 a7);
/* 0x006f0630: ECX = record, EDX = signed byte at [record]. */
extern unsigned long long CM_FASTCALL cm_fn_006f0630(u32 self, u32 edx_in);
/* 0x006f46b0: ECX = self.  Both stack slots are pushed as full DWORDs by the
   caller (0x006beb64 pushes the L+0x08 dword), so 32-bit parameters keep the
   original slot contents. */
extern void CM_THISCALL cm_fn_006f46b0(u32 self, s32 a, s32 b);
/* 0x00700bd0: ECX = record, in byte, out WORD. */
extern s16 CM_THISCALL cm_fn_00700bd0(u32 self, u8 *in, s16 *out);
/* 0x00702050: ECX = record, six stack args; a1/a3 are the full scratch DWORDs. */
extern void CM_THISCALL cm_fn_00702050(u32 self, u32 a1, s32 a2, u32 a3,
                                       u8 a4, u8 a5, u32 a6);
/* 0x007109f0: ECX = record. */
extern u32 CM_THISCALL cm_fn_007109f0(u32 self, u32 a);
/* 0x00714d40: ECX = self. */
extern void CM_FASTCALL cm_fn_00714d40(u32 self);
/* 0x007160d0: ECX = self, one byte arg. */
extern void CM_THISCALL cm_fn_007160d0(u32 self, u8 value);
/* 0x007160e0: ECX = self, one dword arg. */
extern void CM_THISCALL cm_fn_007160e0(u32 self, u32 value);
/* 0x0090cfc0: cdecl, one stack arg (callers push one dword and `add esp,4`). */
extern s32 CM_CDECL cm_fn_0090cfc0(s32 bound);

#define F8(o) (*(volatile u8 *)(param_1 + (u32)(o)))
#define F16(o) (*(volatile u16 *)(param_1 + (u32)(o)))
#define F32(o) (*(volatile u32 *)(param_1 + (u32)(o)))

u32 CM_THISCALL cm_fn_006be710(u32 param_1, u8 param_2)

{
  volatile cmre_frame710 L;
  u32 sel;
  u32 ptr;
  u32 res;
  u32 ebxv;
  u32 kind;
  s32 val;
  s32 i;
  s32 idx;
  s32 remv;
  s32 rem;
  s32 recidx;
  s32 rr;
  u32 u9;
  u32 u10;
  u32 off;
  u32 blv;
  u8 c4;
  u8 a2c;
  u8 a2d;
  s16 k16;
  long double t;
  volatile u8 *pc1;
  volatile u8 *pc6;

  /* 0x006be71d: movsx eax,[esi+0x8eb3] / dec eax / cmp eax,5 / ja default. */
  sel = (u32)(s32)*(volatile s8 *)(param_1 + 0x8eb3) - 1u;
  switch (sel) {

  case 0:
    /* 0x006be734: state byte 1 -> 2. */
    ptr = F32(0x8f7a);
    F16(0x475f) = 0xffff;
    F32(0x4761) = 0;
    F32(0x4765) = 0;
    F8(0x4769) = 0;
    F8(0x475a) = 0;
    F8(0x475b) = 0xff;
    F8(0x475c) = 0xff;
    F8(0x475d) = 0xff;
    F8(0x475e) = 0xff;
    F8(0x8ea4) = *(volatile u8 *)ptr;
    *(volatile u8 *)ptr = 0;
    F8(0x8e9e) = 1;
    F8(0x1c5) = 0;
    F32(0x477e) = 0;
    F8(0x8eaa) = 0;
    F8(0x8ea9) = 0;
    F32(0xf5aa) = 0;
    F32(0xf5a6) = 0;
    F32(0xf576) = 0;
    F32(0xf57a) = 0;
    F8(0x8ea6) = 0xff;
    F8(0x8eb9) = 0xff;
    F16(0x8ed6) = 0;
    F32(0x91de) = 0;
    F8(0x8ea2) = 5;
    F8(0x8ea1) = 5;
    F8(0x8ea0) = 0;
    F8(0x8e9f) = 0;
    res = cm_fn_006be520(param_1);
    F8(0x8ea3) = 0;
    F8(0x8eb3) = 2;
    return res;

  case 3:
    /* 0x006be80b: state byte 4 -> 5. */
    ptr = F32(0x8f7a);
    F16(0x475f) = 0xffff;
    F32(0x4761) = 0;
    F32(0x4765) = 0;
    F8(0x4769) = 0;
    F8(0x475a) = 0;
    F8(0x475b) = 0xff;
    F8(0x475c) = 0xff;
    F8(0x475d) = 0xff;
    F8(0x475e) = 0xff;
    F8(0x8ea4) = *(volatile u8 *)ptr;
    *(volatile u8 *)ptr = 0;
    c4 = (u8)cm_fn_0090cfc0(2);
    F8(0x8e9e) = c4;
    F8(0x1c5) = 0;
    F32(0x477e) = 0;
    F8(0x8eaa) = 0;
    F8(0x8ea9) = 0;
    F32(0xf5aa) = 0;
    F32(0xf5a6) = 0;
    F32(0xf576) = 0;
    F32(0xf57a) = 0;
    F8(0x8ea6) = 0xff;
    F8(0x8eb9) = 0xff;
    F16(0x8ed6) = 0;
    F32(0x91de) = 0;
    F8(0x8ea2) = 5;
    F8(0x8ea1) = 5;
    F8(0x8ea0) = 0;
    F8(0x8e9f) = 0;
    res = cm_fn_006be520(param_1);
    F8(0x8ea3) = 0;
    F8(0x8eb3) = 5;
    return res;

  case 2:
  case 5:
    /* 0x006bee2d: state byte 3 or 6 -> 0. */
    c4 = F8(0xf5bc);
    blv = F8(0xf5f2);
    /* 0x006bee39..0x006bee57: p4 = ([0xf5bc] <= [0xf5f2]) as an unsigned byte
       compare, pushed between the two -1 arguments. */
    cm_fn_006d6f70(F32(0x8f7a), 0x2004u, param_1, 0xffffffffu,
                   (u32)(c4 <= (u8)blv), 0xffffffffu, -1, 0);
    ptr = F32(0x4792);
    *(volatile u8 *)(ptr + 0x49) = F8(0xf5bc);
    ptr = F32(0x4792);
    *(volatile u8 *)(ptr + 0x4a) = F8(0xf5f2);
    ptr = F32(0x8f7a);
    /* 0x006bee80 reads the saved byte before 0x006bee86 clears the state. */
    c4 = F8(0x8ea4);
    F8(0x8eb3) = 0;
    *(volatile u8 *)ptr = c4;
    return ptr;

  default:
    return sel;

  case 1:
  case 4:
    /* 0x006be8f2: main state (byte 2 or 5). */
    c4 = F8(0x8ea1);
    F8(0x8eb2) = 0;
    if ((s8)c4 == 0 && (s8)F8(0x8ea2) == 0) {
      F8(0x8ea2) = 1;
      F8(0x8ea1) = 1;
      F8(0x8ea3) = 1;
    }
    /* 0x006be922: record selection. */
    c4 = F8(0x8e9e);
    idx = (s32)(s8)c4;
    remv = (s32)*(volatile s8 *)(param_1 + (u32)idx + 0x8e9f);
    rem = remv % 11;
    recidx = (s32)*(volatile s8 *)(param_1 + (u32)(11 * idx + rem) + 0x1af);
    pc1 = (volatile u8 *)(param_1 + 0x4796u +
                          (u32)(s32)(recidx + 20 * idx) * 0x1beu);
    pc6 = (volatile u8 *)(u32)cm_fn_006ce110((volatile u8 *)param_1,
                                             (s8)(c4 == 0));
    /* 0x006be97d: index is re-read from the object after 0x006ce110. */
    i = (s32)*(volatile s8 *)(param_1 + 0x8e9e);
    off = param_1 + (u32)i + 0x8e9fu;
    *(volatile u8 *)off = (u8)(*(volatile u8 *)off + 1u);
    L.d[3] = (u32)pc6;
    cm_fn_007160e0((u32)pc1, 0);
    cm_fn_007160e0((u32)pc6, 0);
    /* 0x006be9aa */
    i = (s32)*(volatile s8 *)(param_1 + 0x8e9e);
    recidx = (s32)*(volatile s8 *)pc1;
    *(volatile u8 *)(param_1 + (u32)recidx + (u32)i * 20u + 0x8edau) = 1;
    (void)cm_fn_006f0630((u32)pc1, (u32)recidx);
    /* 0x006be9ca */
    blv = F8(0x8e9e);
    F8(0x8ea9) = 0;
    ptr = *(volatile u32 *)(pc1 + 0x6d);
    i = (s32)(s8)blv;
    u9 = (u32)(s32)*(volatile s8 *)(ptr + 0x35);
    L.d[2] = (u32)(s32)*(volatile s8 *)(param_1 + (u32)i + 0x8ea1);
    rr = cm_fn_0090cfc0((s32)u9);
    L.d[1] = (200u - ((u32)rr * ((u32)L.d[2] + 4u))) * 2u;
    ebxv = ((u32)pc6 & 0xffffff00u) | (u32)(u8)blv;
    u9 = (u32)*(volatile u8 *)(param_1 + 0xf5bc + (u32)(i * 0x36));
    u10 = (u32)*(volatile u8 *)(param_1 + 0xf5bc + (u32)((blv == 0) * 0x36));
    /* 0x006bea40: cmp ecx,edx / jle 0x6bea76, signed.  edx is the DWORD at
       L+0x08 (the sign-extended char store from 0x006be9ea), not L+0x04. */
    if ((s32)u10 > (s32)L.d[2] + (s32)u9 - 1) {
      if ((s8)F8(0x8ea3) == 0) {
        cm_fn_006d6f70(F32(0x8f7a), 0x2005u, param_1, (u32)(u8)pc1[0x2e],
                       ebxv, -1, -1, 0);
      }
      /* 0x006bea6a: fild dword [esp+0x14] / fmul qword [0x00968ec8]. */
      t = (long double)(s32)L.d[1] *
          (long double)CM_GLOBAL(const double, 0x00968ec8);
    }
    else {
      /* 0x006bea76 */
      if ((s32)(u9 + 1u) >
          (s32)(s32)*(volatile s8 *)(param_1 + (u32)(blv == 0) + 0x8ea1) +
              (s32)u10) {
        if ((s8)F8(0x8ea3) == 0) {
          cm_fn_006d6f70(F32(0x8f7a), 0x2006u, param_1, (u32)(u8)pc1[0x2e],
                         ebxv, -1, -1, 0);
        }
        /* 0x006beaab: fild dword [esp+0x14] / fmul qword [0x00969040]. */
        t = (long double)(s32)L.d[1] *
            (long double)CM_GLOBAL(const double, 0x00969040);
      }
      else {
        goto after_ftol;
      }
    }
    /* 0x006beab5: call 0x00945640 (__ftol), net 0x006beaba push eax into
       0x0090cfc0, then 0x006beac9 stores L+0x04 += result. */
    rr = cm_fn_0090cfc0((s32)(u32)(long long)t);
    L.d[1] = L.d[1] + (u32)rr;
after_ftol:
    /* 0x006beacd: cmp al,3 with al = [[esp+0x28]] (the incoming byte arg). */
    if ((u8)param_2 == 3) {
      L.b[0x10] = 0x33;
      k16 = cm_fn_00700bd0((u32)pc1, (u8 *)&L.b[0x10], (s16 *)&L.w[10]);
      rr = cm_fn_0090cfc0((s32)L.d[1]);
      val = (s32)k16 - rr;
      kind = 3;
      goto call702050;
    }
    if ((s8)F8(0x8e9e) == 0) {
      /* 0x006beb20 */
      a2d = (u8)pc1[0x2d];
      *(volatile u16 *)(pc1 + 0x19a) = 0;
      L.b[0x08] = 4;
      if ((s8)a2d > 3) {
        a2c = (u8)pc1[0x2c];
        if (a2c == 0) {
          c4 = (u8)cm_fn_0090cfc0(2);
          L.b[0x08] = (u8)(4u - (u32)c4);
        }
        else {
          c4 = (u8)cm_fn_0090cfc0(2);
          L.b[0x08] = (u8)((u32)c4 + 4u);
        }
      }
      cm_fn_006f46b0((u32)pc1, (s32)L.d[2], 7);
      cm_fn_006f46b0((u32)pc6, 4, 0xb);
      L.d[2] = 0x14;
      off = param_1 + 0x4796u;
      do {
        if ((volatile u8 *)off != pc1 && (volatile u8 *)off != pc6 &&
            -1 < (s32)*(volatile s8 *)(off + 0x19)) {
          cm_fn_006f46b0(off, 0, 0);
          cm_fn_007160e0(off, 10);
        }
        if ((volatile u8 *)(off + 0x22d8) != pc1 &&
            (volatile u8 *)(off + 0x22d8) != pc6 &&
            -1 < (s32)*(volatile s8 *)(off + 0x22f1)) {
          cm_fn_006f46b0(off + 0x22d8, 0, 0);
          cm_fn_007160e0(off + 0x22d8, 10);
        }
        off = off + 0x1beu;
        L.d[2] = L.d[2] - 1u;
      } while (L.d[2] != 0);
    }
    else {
      /* 0x006bebf0 */
      a2d = (u8)pc1[0x2d];
      *(volatile u16 *)(pc1 + 0x19a) = 0xb4;
      L.b[0x08] = 4;
      if ((s8)a2d > 3) {
        a2c = (u8)pc1[0x2c];
        if (a2c == 0) {
          c4 = (u8)cm_fn_0090cfc0(2);
          L.b[0x08] = (u8)((u32)c4 + 4u);
        }
        else {
          c4 = (u8)cm_fn_0090cfc0(2);
          L.b[0x08] = (u8)(4u - (u32)c4);
        }
      }
      cm_fn_006f46b0((u32)pc1, (s32)L.d[2], 4);
      cm_fn_006f46b0((u32)pc6, 4, 0);
      L.d[2] = 0x14;
      off = param_1 + 0x4796u;
      do {
        if ((volatile u8 *)off != pc1 && (volatile u8 *)off != pc6 &&
            -1 < (s32)*(volatile s8 *)(off + 0x19)) {
          cm_fn_006f46b0(off, 8, 0xb);
          cm_fn_007160e0(off, 10);
        }
        if ((volatile u8 *)(off + 0x22d8) != pc1 &&
            (volatile u8 *)(off + 0x22d8) != pc6 &&
            -1 < (s32)*(volatile s8 *)(off + 0x22f1)) {
          cm_fn_006f46b0(off + 0x22d8, 8, 0xb);
          cm_fn_007160e0(off + 0x22d8, 10);
        }
        off = off + 0x1beu;
        L.d[2] = L.d[2] - 1u;
      } while (L.d[2] != 0);
    }
    /* 0x006becbb: 200 - 0x0090cfc0(local), clamped as a signed 16-bit value. */
    rr = cm_fn_0090cfc0((s32)L.d[1]);
    k16 = (s16)(u16)(200u - (u32)rr);
    *(volatile u16 *)(pc1 + 0x19c) = (u16)k16;
    if (k16 > 0xc8) {
      *(volatile u16 *)(pc1 + 0x19c) = 0xc8;
    }
    else if (k16 < -200) {
      *(volatile u16 *)(pc1 + 0x19c) = 0xff38;
    }
    /* 0x006becf8 */
    blv = 0;
    L.b[0x03] = 0;
    for (;;) {
      cm_fn_00714d40((u32)pc6);
      if (F8(0x8eb2) == 0 && F8(0x8ea9) != 0xff) {
        blv = (u8)cm_fn_007109f0((u32)pc1, 4);
      }
      cm_fn_007160d0((u32)pc1, 0xff);
      cm_fn_007160d0((u32)pc6, 0xff);
      if (F8(0x8eb2) != 0 || F8(0x8ea9) == 0xff) {
        break;
      }
      c4 = L.b[0x03];
      L.b[0x03] = (u8)(c4 + 1u);
      if ((s8)c4 >= 4) {
        break;
      }
      if ((u8)blv != 0) {
        break;
      }
      if (F32(0xf582) != (u32)pc1) {
        break;
      }
    }
    if (F32(0xf582) == (u32)pc1 && F8(0x8eb2) == 0 && F8(0x8ea9) != 0xff) {
      /* 0x006bed84: second 0x00700bd0 pair, kind 4. */
      L.b[0x10] = 0xff;
      k16 = cm_fn_00700bd0((u32)pc1, (u8 *)&L.b[0x10], (s16 *)&L.w[10]);
      val = (s32)k16;
      kind = 4;
      goto call702050;
    }
    goto tail;

call702050:
    /* 0x006beda9: both scratch slots are pushed as full DWORDs. */
    cm_fn_00702050((u32)pc1, L.d[4], val, L.d[5], (u8)kind, 0, 0);
tail:
    /* 0x006bedb5 */
    i = (s32)*(volatile s8 *)(param_1 + 0x8e9e);
    off = param_1 + (u32)i + 0x8ea1u;
    *(volatile u8 *)off = (u8)(*(volatile u8 *)off - 1u);
    c4 = F8(0x8e9e);
    i = (s32)*(volatile s8 *)(param_1 + 0x8ea2);
    F8(0x8e9e) = (u8)(c4 == 0);
    u9 = (u32)F8(0xf5f2);
    u10 = (u32)F8(0xf5bc);
    /* 0x006bedf9: cmp ecx,edx / jg state; cmp eax,edx / jle default return. */
    if ((s32)u10 > i + (s32)u9 ||
        (s32)u9 > (s32)(s32)*(volatile s8 *)(param_1 + 0x8ea1) + (s32)u10) {
      /* 0x006bee0e: bl = incoming byte arg. */
      res = (u32)((u8)param_2 != 4u) - 1u;
      res = (res & 0xffffff00u) | (((res & 0xffu) & 0xfdu));
      res = res + 6u;
      F8(0x8eb3) = (u8)res;
      return res;
    }
    return u9;
  }
}
