/* fn_00561210 -- preferred-image VA 0x00561210 (image base 0x00400000),
   CM0102 3.9.68 primary executable,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Body 0x00561210..0x005617b6 (1447 bytes) translated from the original
   instructions only.  ECX is read before written (0x00561222: mov esi, ecx)
   and the routine returns with a plain `ret` (0x005617b6), so the entry is
   thiscall with the object pointer in ECX; the return value is AX
   (0x00561445 `or ax,0xffff`, 0x005617a2 `mov ax,word ptr [esi+0x12f531]`).
   Callee cm_fn_005e7840 is called with SIX pushed arguments at 0x005613da
   (push ecx / push 1 / push edx / push eax / push ecx / push edx) and the
   caller cleans 0x18 bytes at 0x0056141d, so it is cdecl with six arguments. */
#include "cmre.h"

#define DAT_00ae31a8 CM_GLOBAL(volatile u16, 0x00ae31a8)
#define DAT_00b67a14 CM_GLOBAL(volatile int, 0x00b67a14)
#define DAT_00b67a20 CM_GLOBAL(volatile int, 0x00b67a20)
#define DAT_00b67a34 CM_GLOBAL(volatile int, 0x00b67a34)
#define DAT_00b67a38 CM_GLOBAL(volatile int, 0x00b67a38)

extern u32 CM_CDECL cm_fn_005e2eb0(volatile u32 first, volatile u32 second, volatile u32 third, volatile u32 fourth);
extern void cm_fn_005e3b50(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6);
extern int * cm_fn_005e3dd0(u32 param_1,u32 param_2,u32 param_3,u32 param_4, int *param_5);
extern void cm_fn_005e3fd0(int param_1,int param_2,int *param_3);
extern void CM_CDECL cm_fn_005e42b0(u32 object);
extern void cm_fn_005e7840(int param_1,int param_2,u32 param_3,int param_4,u32 param_5,u32 param_6);

/* Object-relative accesses keep the original widths and order. */
#define OBJP  ((volatile u8 *)param_1)
#define RD32(o) (*(volatile u32 *)(OBJP + (u32)(o)))
#define RD16(o) (*(volatile u16 *)(OBJP + (u32)(o)))
#define RDS16(o) (*(volatile s16 *)(OBJP + (u32)(o)))
#define RD8(o)  (*(volatile u8 *)(OBJP + (u32)(o)))
#define WR32(o, v) (*(volatile u32 *)(OBJP + (u32)(o)) = (u32)(v))
#define WR16(o, v) (*(volatile u16 *)(OBJP + (u32)(o)) = (u16)(v))

u32 CM_THISCALL cm_fn_00561210(void *param_1)
{
  u32 gx;                 /* ebx : copy of DAT_00b67a20            */
  u32 gy;                 /* edx : copy of DAT_00b67a14            */
  s32 dx;                 /* ebp : horizontal delta                */
  s32 dy;                 /* edi : vertical delta                  */
  s32 idx;                /* sign extended word at +0x12f531       */
  u32 t99;                /* 99 * idx (table stride / 4)           */
  u32 selo;               /* 396 * idx (byte offset of the entry)  */
  u32 a6;                 /* partial-register argument             */
  u32 v511, v515, v519, v51d;
  u32 x, y, z, w;
  u32 res;
  u32 fl;
  u32 off2;
  u32 b3;
  u32 diff;
  u8  limb;
  u16 v;
  u16 wv;
  s16 cxv, limv;
  s32 i;
  s32 j;
  s32 idx2;
  s32 idx3;

  /* 0x00561214 .. 0x0056126a */
  gx = (u32)DAT_00b67a20;
  gy = (u32)DAT_00b67a14;
  dx = (s32)(gx - RD32(0x12f509));
  dy = (s32)(gy - RD32(0x12f50d));
  idx = (s32)RDS16(0x12f531);
  t99 = (u32)idx * 99u;
  fl = RD32(0xba96a + t99 * 4u);
  {
    u32 sel = (u32)DAT_00b67a38;
    if (!(fl & 0x100u)) sel = (u32)DAT_00b67a34;
    if (sel == 0) goto path2;
  }

  /* 0x00561270 .. 0x00561280 : nothing moved -> nothing to store.
     EAX still holds the +0x12f521 word when `or ax,0xffff` runs. */
  if (dy == 0 && dx == 0) {
    u32 f521 = RD32(0x12f521);
    if (f521 != 0) {
      return (f521 & 0xffff0000u) | 0xffffu;
    }
  }

  /* 0x00561286 .. 0x005612d6 : clamp the deltas */
  v511 = RD32(0x12f511);
  if ((s32)(v511 + (u32)dx) < 0) dx = (s32)(0u - v511);
  v519 = RD32(0x12f519);
  if ((s32)(v519 + (u32)dx) > 0x31f) dx = (s32)(0x31fu - v519);
  v515 = RD32(0x12f515);
  if ((s32)(v515 + (u32)dy) < 0) dy = (s32)(0u - v515);
  v51d = RD32(0x12f51d);
  if ((s32)(v51d + (u32)dy) > 0x257) dy = (s32)(0x257u - v51d);

  /* 0x005612d8 .. 0x005612e8 */
  if (dx == 0 && dy == 0) {
    u32 f521 = RD32(0x12f521);
    if (f521 != 0) {
      /* 0x00561436 .. 0x00561445 */
      WR32(0x12f509, gx);
      WR32(0x12f50d, gy);
      return (f521 & 0xffff0000u) | 0xffffu;
    }
  }

  /* 0x005612ee .. 0x0056133c */
  {
    u32 f521 = RD32(0x12f521);
    if (f521 != 0) {
      u32 b2 = RD32(0x12f515);
      u32 b1 = RD32(0x12f511);
      cm_fn_005e3fd0((int)b1, (int)b2, (int *)(u32)f521);
      w = RD32(0x12f51d);
      y = RD32(0x12f519);
      z = RD32(0x12f515);
      x = RD32(0x12f511);
      cm_fn_005e2eb0(x, z, y, w);
      cm_fn_005e42b0(RD32(0x12f521));
      WR32(0x12f521, 0);
    }
  }

  /* 0x00561346 .. 0x00561394 : apply the deltas and build the entry */
  {
    u32 n511 = RD32(0x12f511);
    u32 n515 = RD32(0x12f515);
    u32 n519, n51d;
    /* 0x00561352/54: DWORD ADD wraps even after the signed clamps. */
    n511 = n511 + (u32)dx;
    n515 = n515 + (u32)dy;
    WR32(0x12f511, n511);
    n519 = RD32(0x12f519);
    n519 = n519 + (u32)dx;
    n51d = RD32(0x12f51d);
    n51d = n51d + (u32)dy;
    WR32(0x12f515, n515);
    WR32(0x12f519, n519);
    WR32(0x12f51d, n51d);
    res = (u32)cm_fn_005e3dd0(n511, n515, n519, n51d, (int *)0);
  }
  idx = (s32)RDS16(0x12f531);
  WR32(0x12f521, res);
  t99 = (u32)idx * 99u;

  /* 0x005613a7 .. 0x00561406 */
  if (RD32(0xba996 + t99 * 4u) & 0x10000u) {
    /* 0x005613b4 : mov cx keeps the upper half of ecx (the index) */
    a6 = ((u32)idx & 0xffff0000u) | (u32)DAT_00ae31a8;
    w = RD32(0x12f51d);
    y = RD32(0x12f519);
    z = RD32(0x12f515);
    x = RD32(0x12f511);
    cm_fn_005e7840((int)x, (int)z, y, (int)w, 1u, a6);
  } else {
    /* 0x005613e1 : mov ax keeps the upper half of eax (99 * index) */
    a6 = (t99 & 0xffff0000u) | (u32)DAT_00ae31a8;
    w = RD32(0x12f51d);
    y = RD32(0x12f519);
    z = RD32(0x12f515);
    x = RD32(0x12f511);
    cm_fn_005e3b50(x, z, y, w, 1u, a6);
  }

  /* 0x0056140b .. 0x0056144d : EAX keeps the callee's return above ax */
  w = RD32(0x12f51d);
  y = RD32(0x12f519);
  z = RD32(0x12f515);
  x = RD32(0x12f511);
  res = cm_fn_005e2eb0(x, z, y, w);
  WR32(0x12f509, gx);
  WR32(0x12f50d, gy);
  return (res & 0xffff0000u) | 0xffffu;

path2:
  /* 0x0056144e .. 0x0056149c */
  {
    u32 f521 = RD32(0x12f521);
    if (f521 != 0) {
      u32 b1 = RD32(0x12f511);
      u32 b2 = RD32(0x12f515);
      cm_fn_005e3fd0((int)b1, (int)b2, (int *)(u32)f521);
      w = RD32(0x12f51d);
      y = RD32(0x12f519);
      z = RD32(0x12f515);
      x = RD32(0x12f511);
      cm_fn_005e2eb0(x, z, y, w);
      cm_fn_005e42b0(RD32(0x12f521));
      WR32(0x12f521, 0);
    }
  }

  /* 0x005614a6 .. 0x005614ce */
  idx = (s32)RDS16(0x12f531);
  t99 = (u32)idx * 99u;
  selo = t99 * 4u;
  WR16(0xba9dc + selo, 0xffffu);
  i = 0;

  /* 0x005614d4 .. 0x005615b0 */
  if ((s16)RD16(0x12f4f8) > 0) {
    for (;;) {
      v = RD16(0x12eb96 + (u32)(s16)i * 2u);
      idx2 = (s32)(s16)v;
      off2 = (u32)idx2 * 396u;
      fl = RD32(0xba96a + off2);
      if ((fl & 2u) && !(fl & 0x200u) &&
          (s32)gx >= (s32)RD32(0xba96e + off2) &&
          (s32)gx <= (s32)RD32(0xba976 + off2) &&
          (s32)gy >= (s32)RD32(0xba972 + off2) &&
          (s32)gy <= (s32)RD32(0xba97a + off2) &&
          (s16)RD16(0xba9dc + selo) == (s16)-1) {
        WR16(0xba9dc + selo, v);
        v511 = RD32(0x12f511);
        v515 = RD32(0x12f515);
        v519 = RD32(0x12f519);
        v51d = RD32(0x12f51d);
        WR32(0xba9be + selo, v511);
        WR32(0xba9be + selo + 4u, v515);
        WR32(0xba9be + selo + 8u, v519);
        WR32(0xba9be + selo + 12u, v51d);
      }
      i++;
      if (!((s16)i < (s16)RD16(0x12f4f8))) break;
    }
  }

  /* 0x005615b6 .. 0x0056179c */
  j = 0;
  if ((s16)RD16(0x12f4f6) > 0) {
    for (;;) {
      volatile u8 *p2 = OBJP + (0x12e9a2 + (u32)(s16)j * 2u);
      idx3 = (s32)*(volatile s16 *)p2;
      b3 = (u32)idx3 * 3057u;
      if (RD32(b3 + 0xbedu) == 0) goto l2_next;
      if ((s16)RD16(b3 + 0x212u) != (s16)-1) goto l2_next;
      WR16(b3 + 0xb7cu, RD16(b3 + 0xb7au));

      /* inner loop, 0x0056161b .. 0x00561787 */
      for (;;) {
        s32 a, w2;
        idx3 = (s32)*(volatile s16 *)p2;
        b3 = (u32)idx3 * 3057u;
        cxv = (s16)RD16(b3 + 4u + 0xb78u);
        limv = (s16)RD16(b3 + 4u + 0xb74u);
        if (cxv >= limv) break;
        wv = RD16(b3 + 4u + (u32)(s32)cxv * 2u + 0x212u);
        WR16(b3 + 4u + 0xb78u, (u16)(cxv + 1));
        if (wv == 0xffffu) break;
        /* 0x00561662..74: the counter store can overlap *p2.
           Reload the signed index and recompute the record base. */
        idx3 = (s32)*(volatile s16 *)p2;
        b3 = (u32)idx3 * 3057u;
        off2 = (u32)(s32)(s16)wv * 396u;
        a = (s32)RDS16(b3 + 0xb7au);
        w2 = (s32)RDS16(b3 + (u32)a * 2u + 0x216u);
        diff = RD32(off2 + 0xba982u) - RD32((u32)w2 * 396u + 0xba982u);
        limb = RD8(b3 + 0xbb0u);
        if ((s32)diff > (s32)((u32)limb - 1u)) break;
        fl = RD32(off2 + 0xba96au);
        if (!(fl & 2u)) continue;
        if (fl & 0x200u) continue;
        if ((s32)gx < (s32)RD32(off2 + 0xba96eu)) continue;
        if ((s32)gx > (s32)RD32(off2 + 0xba976u)) continue;
        if ((s32)gy < (s32)RD32(off2 + 0xba972u)) continue;
        if ((s32)gy > (s32)RD32(off2 + 0xba97au)) continue;
        if ((s16)RD16(0xba9dc + selo) != (s16)-1) continue;
        WR16(0xba9dc + selo, wv);
        v511 = RD32(0x12f511);
        v515 = RD32(0x12f515);
        v519 = RD32(0x12f519);
        v51d = RD32(0x12f51d);
        WR32(0xba9be + selo, v511);
        WR32(0xba9be + selo + 4u, v515);
        WR32(0xba9be + selo + 8u, v519);
        WR32(0xba9be + selo + 12u, v51d);
      }

    l2_next:
      j++;
      if (!((s16)j < (s16)RD16(0x12f4f6))) break;
    }
  }

  /* 0x005617a2 .. 0x005617b6 */
  v = (u16)RDS16(0x12f531);
  WR32(0, 0);
  return v;
}
