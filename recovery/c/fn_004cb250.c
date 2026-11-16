/* fn_004cb250 @ preferred-image VA 0x004cb250 (image base 0x00400000).
   thiscall, ret 0xc (three stack arguments). Incoming ECX (this) is stored to
   the stack slot at 0x004cb259 and forwarded to cm_fn_004cbb60 at
   0x004cb6df / 0x004cb8a4, so it stays an explicit parameter. */
#include "cmre.h"

/* ---- original globals -------------------------------------------------- */
#define D_AE1050   CM_GLOBAL(u32, 0x00ae1050)   /* int * table base           */
#define D_AE1080   CM_GLOBAL(u32, 0x00ae1080)   /* dword store only           */
#define D_AE2364   CM_GLOBAL(s32, 0x00ae2364)
#define D_AE23B4   CM_GLOBAL(u32, 0x00ae23b4)
#define D_AE2C90   CM_GLOBAL(u32, 0x00ae2c90)
#define D_AE2C92   CM_GLOBAL(u16, 0x00ae2c92)
#define D_AE2D0C   CM_GLOBAL(u32, 0x00ae2d0c)
#define D_B63D28   CM_GLOBAL(u32, 0x00b63d28)
#define D_B7434C   CM_GLOBAL(u32, 0x00b7434c)
#define D_00000053 CM_GLOBAL(u32, 0x00000053)

/* ---- original floating point constants -------------------------------- */
#define K_968D90 CM_GLOBAL(double, 0x00968d90)
#define K_968DB8 CM_GLOBAL(double, 0x00968db8)
#define K_968DA0 CM_GLOBAL(double, 0x00968da0)
#define K_968D98 CM_GLOBAL(double, 0x00968d98)
#define K_9689A0 CM_GLOBAL(double, 0x009689a0)
#define K_967890 CM_GLOBAL(double, 0x00967890)
#define K_967880 CM_GLOBAL(double, 0x00967880)
#define K_9688B0 CM_GLOBAL(double, 0x009688b0)
#define K_968968 CM_GLOBAL(double, 0x00968968)
#define K_968DB0 CM_GLOBAL(float, 0x00968db0)
#define K_968DAC CM_GLOBAL(float, 0x00968dac)
#define K_968DA8 CM_GLOBAL(float, 0x00968da8)

#define RD8(a)   (*(volatile u8 *)(void *)(a))
#define RD16(a)  (*(volatile u16 *)(void *)(a))
#define RS16(a)  (*(volatile s16 *)(void *)(a))
#define RD32(a)  (*(volatile u32 *)(void *)(a))
#define WR8(a, v) (*(volatile u8 *)(void *)(a) = (u8)(v))

/* ---- original callees -------------------------------------------------- */
/* Both call sites push exactly record and club and load ECX from saved self. */
extern u32 CM_THISCALL cm_fn_004cbb60(u32 self, u32 record, u32 club);
extern u8 CM_THISCALL cm_fn_0062c400(u32 self, u32 person);
extern u8 CM_CDECL cm_fn_00669340(u32 value);
extern void CM_THISCALL cm_fn_008674e0(void *self, u32 param_2);
extern void CM_THISCALL cm_fn_00867700(u32 self, u32 arg1, u32 arg2);
/* ECX is loaded at the call site, but is not an argument of this callee. */
extern u8 CM_STDCALL cm_fn_007a7eb0(s32 *param_1);
/* Each call pushes only bound and removes four bytes afterwards. */
extern unsigned long long CM_CDECL cm_fn_0090cfc0(u32 bound);

/* 0x004cb596 / 0x004cb774: resolve the club record of p1 and read its
   attribute byte (0x53).  The value lands in the low byte of EAX; the
   intermediate club pointer is also written to 0x00ae1080. */
#define CLUB_ATTR()                                                           \
  do {                                                                        \
    u32 e0_ = RD32((u32)p1);                                                  \
    u32 gc_ = (u32)D_AE2364;                                                  \
    u32 gb_ = D_AE1050;                                                       \
    u32 gx_ = D_AE2D0C;                                                       \
    u32 g4_;                                                                  \
    u32 cp_;                                                                  \
    u32 rec_;                                                                 \
    if ((s32)e0_ < (s32)gc_) {                                                \
      s32 ix_ = (s32)RD32(gx_ + e0_ * 0x4fu);                                 \
      if (ix_ < 0) {                                                          \
        cp_ = 0;                                                              \
      } else {                                                                \
        cp_ = RD32(gb_) + (u32)ix_ * 0x50u;                                   \
      }                                                                       \
    } else {                                                                  \
      cp_ = 0;                                                                \
    }                                                                         \
    g4_ = D_AE23B4;                                                           \
    D_AE1080 = cp_;                                                           \
    if (cp_ == 0) {                                                           \
      rec_ = 0;                                                               \
    } else {                                                                  \
      rec_ = RD32(cp_ + 4) * 0x245u + g4_;                                    \
    }                                                                         \
    if (RD32(rec_ + 0x53) != 0) {                                             \
      u32 e1_ = RD32((u32)p1);                                                \
      u32 cp2_;                                                               \
      if ((s32)e1_ < (s32)gc_) {                                              \
        s32 ix2_ = (s32)RD32(gx_ + e1_ * 0x4fu);                              \
        if (ix2_ < 0) {                                                       \
          cp2_ = 0;                                                           \
        } else {                                                              \
          cp2_ = RD32(gb_) + (u32)ix2_ * 0x50u;                               \
        }                                                                     \
      } else {                                                                \
        cp2_ = 0;                                                             \
      }                                                                       \
      D_AE1080 = cp2_;                                                        \
      if (cp2_ == 0) {                                                        \
        al = (u8)cm_fn_00669340(D_00000053);                                  \
      } else {                                                                \
        u32 rec2_ = RD32(cp2_ + 4) * 0x245u + g4_;                            \
        al = (u8)cm_fn_00669340(RD32(rec2_ + 0x53));                          \
      }                                                                       \
    } else {                                                                  \
      al = (u8)(u32)p2;                                                       \
    }                                                                         \
  } while (0)

/* The original loads the dword at VA 0x00000053 (0x004cb64a: xor eax,eax;
   0x004cb64c: mov eax,dword ptr [eax+0x53]); the brief lists it as global
   0x00000053.  The dereference is the original access, so only the compiler
   diagnostic for a constant low-address load is suppressed. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"

void CM_THISCALL cm_fn_004cb250(u32 self, u8 *p1, u8 *p2, u8 *p3)
{
  u8 *row = &CM_GLOBAL(u8, 0x00b63d8a);
  long double x;
  long long ft;
  u32 dayA, dayC, u32v, year, person_year;
  s32 day;
  s32 sv;
  u8 cvar2, al, b44, clv;
  s8 cvar4;
  u32 ea, w;
  u32 ridx;
  s16 rdy;
  u16 rhw;

  if (p3 == (u8 *)0) {
    return;
  }
  if (p2 == (u8 *)0) {
    return;
  }
  if (p1 == (u8 *)0) {
    return;
  }
  if ((u32)p1 == RD32((u32)p2 + 0xcf)) {
    return;
  }
  if ((u32)p1 == RD32((u32)p2 + 0xd3)) {
    return;
  }
  if (RD8((u32)p1 + 0x3d) != 0x0b) {
    return;
  }

  /* 0x004cb29f: day-of-year of p1, clamped to [0, 400] */
  person_year = RD16((u32)p1 + 0x40);
  year = D_AE2C92;
  cvar4 = (s8)RD8((u32)p1 + 0x18);
  day = RS16(0x00ae2c90);
  u32v = 365u * (year - person_year)
          - (u32)(s32)RS16((u32)p1 + 0x3e)
          + (u32)day;
  sv = (s32)u32v;
  if (sv < 0) {
    sv = 0;
  } else if (sv > 0x190) {
    sv = 0x190;
  }
  dayA = (u32)sv;

  /* 0x004cb2e3: day-of-year of p3, clamped to [0, 400] */
  u32v = 365u * (year - (u32)RD16((u32)p3 + 0x2f))
          - (u32)(s32)RS16((u32)p3 + 0x2d)
          + (u32)day;
  sv = (s32)u32v;
  if (sv < 0) {
    sv = 0;
  } else if (sv > 0x190) {
    sv = 0x190;
  }
  dayC = (u32)sv;

  /* 0x004cb30f */
  x = (long double)(s32)((s32)RS16(RD32((u32)p1 + 0x61) + 0xb)
                         - (s32)RS16((u32)p2 + 0x80));
  x = x + (long double)(s32)(dayA * 50u);
  x = x + (long double)cvar4 * (long double)K_968D90;
  x = x - (long double)K_968DB8;

  if (dayC > 0x190u) { /* dayC is always <= 0x190: block is never entered */
    if (cvar4 > 0x17 && cvar4 < 0x20) {
      x = x + (long double)(s32)(50u * (u32)(s32)(s8)RD8((u32)p1 + 0x57));
      x = x + (long double)K_968DB0;
      goto L419;
    }
  }

  /* 0x004cb38a */
  al = RD8((u32)p1 + 0x22);
  if (al != 0
      && (s32)RS16(RD32((u32)p1 + 0x1a) + 0x8e) > 0xdac
      && cvar4 < 0x20) {
    u32 c57 = (u32)(s32)(s8)RD8((u32)p1 + 0x57);
    u32 b22 = al;
    x = x + ((long double)(s32)(250u * c57) + (long double)(s32)(25u * b22));
    x = x + (long double)K_968DAC;
    goto L419;
  }

  /* 0x004cb3d2: shift of the attribute byte at p3+0x4f (read once) */
  u32v = (u32)(s32)(s8)(u8)(RD8((u32)p3 + 0x4f) >> 4);
  if ((s32)u32v == 1) {
    u32 c57 = (u32)(s32)(s8)RD8((u32)p1 + 0x57);
    x = x + (long double)(s32)(250u * c57);
    x = x + (long double)K_968DB0;
    if (x >= (long double)K_968DA8) {
      x = (long double)K_968DA8;
    }
    goto L45a;
  }
  if ((s32)u32v == 2) {
    u32 c57 = (u32)(s32)(s8)RD8((u32)p1 + 0x57);
    x = x + (long double)(s32)(125u * c57);
    x = x + (long double)K_968DAC;
    goto L419;
  }
  if ((s32)u32v == 3) {
    s32 e3e = (s32)(s8)RD8((u32)p3 + 0x3e);
    x = x + (long double)(s32)(1000u * (u32)e3e);
    goto L419;
  }
  goto L45a;

L419:
  if (x >= (long double)K_968DA8) {
    x = (long double)K_968DA8;
  }
L45a:
  if (x >= (long double)K_9689A0) {
    x = x * (long double)K_968D98;
  } else {
    x = x * (long double)K_968DA0;
  }
  x = (long double)K_967890 - x;
  if (x >= (long double)K_967880) {
    if (x > (long double)K_9688B0) {
      x = (long double)K_9688B0;
    }
  } else {
    x = (long double)K_967880;
  }
  {
    s32 e3e = (s32)(s8)RD8((u32)p3 + 0x3e) + 4;
    x = (long double)e3e / x;
    x = x + (long double)K_968968;
  }
  ft = (long long)x; /* 0x004cb4c4 call 0x00945640 (__ftol) */
  /* 0x004cb4d0: the low byte is stored to the arg1 stack home */
  cvar2 = (u8)(u32)ft;

  /* 0x004cb4d4 */
  if ((u8)cm_fn_0062c400(D_B63D28, (u32)p1) != 0) {
    goto L8f1;
  }

  /* 0x004cb4e1 */
  {
    u32 e0 = RD32((u32)p1);
    if ((s32)e0 < D_AE2364) {
      u32 gx = D_AE2D0C;
      s32 ix = (s32)RD32(gx + e0 * 0x4fu + 4u);
      if (ix >= 0) {
        u32 cp = RD32(D_AE1050) + (u32)ix * 0x50u;
        /* stored to the [esp+0x14] local at 0x004cb511 */
        if (cp != 0) {
          goto L8f1;
        }
      }
    }
  }

  /* 0x004cb51b: ECX = [0x00b7434c] is the receiver forwarded to this callee */
  (void)CM_GLOBAL(volatile u32, 0x00b7434c);
  if ((u8)cm_fn_007a7eb0((s32 *)p1) != 0) {
    goto L8f1;
  }
  if (cvar4 < 0x12) {
    goto L8f1;
  }
  if (RD8((u32)p3 + 0x49) != 0) {
    goto L8f1;
  }
  if ((s8)cvar2 <= 5) {
    goto L90f;
  }
  {
    s32 a = (s32)(s8)RD8((u32)p3 + 0x44) - (s32)(s8)cvar2;
    ea = 0xffffffe2u - (u32)(s32)(s8)RD8((u32)p1 + 0x59);
    if (a >= (s32)ea) {
      goto L90f;
    }
  }
  if (cvar4 <= 0x15) {
    goto L8a0;
  }

  /* 0x004cb574 */
  w = RD16(RD32((u32)p1 + 0x61) + 0xb);
  if ((s16)w <= RS16((u32)p2 + 0x80)) {
    goto L6df;
  }
  if ((s16)w <= 0x8ca) {
    goto L6df;
  }

  /* 0x004cb596 */
  CLUB_ATTR();
  goto L65e;

L6df:
  {
    u32 r = cm_fn_004cbb60(self, (u32)p1, (u32)p2);
    if ((u8)r == 1) {
      u32 w2 = RD16(RD32((u32)p1 + 0x61) + 0xb);
      if ((s16)w2 > 0x4e2 && cvar4 > 0x17) {
        u32 q = (u32)((s32)(s8)RD8((u32)p1 + 0x59) + 5);
        if ((u32)cm_fn_0090cfc0(q) == 0) {
          cm_fn_008674e0((void *)p3, 4);
          cm_fn_00867700((u32)p3, 0x10, 0);
          goto L90f;
        }
      }
    }
  }

  /* 0x004cb6ec / 0x004cb6f7 / 0x004cb6fc / 0x004cb710: jne 0x004cb72b */
  goto L72b;

L72b:
  /* 0x004cb72b */
  {
    u32 pr6 = RD32((u32)p1 + 0x61);
    s32 lim = (s32)RS16((u32)p2 + 0x80);
    u32 wd = RD16(pr6 + 0xd);
    lim = lim - 0x5dc;
    if ((s32)(s16)wd <= lim) {
      goto L90f;
    }
    if ((s16)wd <= 0x1f4) {
      goto L90f;
    }
    {
      u32 q = (u32)(s32)(s8)RD8((u32)p1 + 0x57);
      u32 r = (u32)cm_fn_0090cfc0(q);
      if (r == 0) {
        goto L90f;
      }
    }
  }
  if ((s8)RD8((u32)p3 + 0x3d) >= 0) {
    goto L90f;
  }

  /* 0x004cb774 */
  CLUB_ATTR();

  /* 0x004cb83c */
  if ((s8)al < 0) {
    goto L90f;
  }
  if ((s8)al >= 0x23) {
    goto L90f;
  }
  ridx = (u32)(s32)(s8)al * 0x48u;
  rdy = (s16)(u16)D_AE2C90;
  rhw = (u16)D_AE2C92;
  if (!(rdy <= RS16((u32)row + ridx + 0x00))) {
    if (rhw == RD16((u32)row + ridx + 0x02)) {
      goto L881;
    }
  }
  if (rhw <= RD16((u32)row + ridx + 0x02)) {
    goto L90f;
  }
L881:
  if (rdy >= RS16((u32)row + ridx + 0x0f)) {
    goto L6b5;
  }
  if (rhw == RD16((u32)row + ridx + 0x11)) {
    goto L6c2;
  }
  goto L6b5;

L8a0:
  /* 0x004cb8a0 */
  {
    u32 r = cm_fn_004cbb60(self, (u32)p1, (u32)p2);
    if ((u8)r == 1) {
      u32 q = (u32)(s32)(s8)RD8((u32)p1 + 0x57);
      u32 rr = (u32)cm_fn_0090cfc0(q);
      if ((s32)rr <= 5) {
        goto L90f;
      }
      w = RD16(RD32((u32)p1 + 0x61) + 0xb);
      if ((s16)w <= RS16((u32)p2 + 0x80)) {
        goto L90f;
      }
      if ((s16)w <= 0x4e2) {
        goto L90f;
      }
      cm_fn_008674e0((void *)p3, 4);
      cm_fn_00867700((u32)p3, 0x10, 0);
    }
  }
  goto L90f;

L8f1:
  /* 0x004cb8f1 */
  if ((RD8((u32)p3 + 0x45) & 4) != 0) {
    cm_fn_008674e0((void *)p3, 4);
  }
  if ((RD8((u32)p3 + 0x45) & 0x10) != 0) {
    cm_fn_008674e0((void *)p3, 0x10);
  }
  goto L90f;

L65e:
  /* 0x004cb65e */
  if ((s8)al < 0) {
    goto L90f;
  }
  if ((s8)al >= 0x23) {
    goto L90f;
  }
  ridx = (u32)(s32)(s8)al * 0x48u;
  rdy = (s16)(u16)D_AE2C90;
  rhw = (u16)D_AE2C92;
  if (!(rdy <= RS16((u32)row + ridx + 0x00))) {
    if (rhw == RD16((u32)row + ridx + 0x02)) {
      goto L6a3;
    }
  }
  if (rhw <= RD16((u32)row + ridx + 0x02)) {
    goto L90f;
  }
L6a3:
  /* 0x004cb6a3 */
  if (rdy >= RS16((u32)row + ridx + 0x0f)) {
    goto L6b5;
  }
  if (rhw == RD16((u32)row + ridx + 0x11)) {
    goto L6c2;
  }
L6b5:
  /* 0x004cb6b5 */
  if (rhw >= RD16((u32)row + ridx + 0x11)) {
    goto L90f;
  }
L6c2:
  /* 0x004cb6c2 */
  cm_fn_008674e0((void *)p3, 4);
  cm_fn_00867700((u32)p3, 4, 0);
  goto L90f;

L90f:
  /* 0x004cb90f */
  clv = (u8)(0u - (u8)cvar2);
  b44 = RD8((u32)p3 + 0x44);
  sv = (s32)(s8)b44 + (s32)(s8)clv;
  if (sv > 0x64) {
    WR8((u32)p3 + 0x44, 0x64);
    return;
  }
  if (sv >= -0x64) {
    WR8((u32)p3 + 0x44, (u8)(b44 + clv));
    return;
  }
  WR8((u32)p3 + 0x44, 0x9c);
  return;
}

#pragma GCC diagnostic pop
