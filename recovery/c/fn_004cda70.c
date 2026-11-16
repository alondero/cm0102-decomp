/* fn_004cda70 -- plain C reconstruction of the original routine at VA 0x004cda70
   (preferred image base 0x00400000).  Recovered from the instruction listing
   only: callees remain explicit boundaries, constants are read from the
   original image, and the x87 sequences keep the listing's operand order. */
#include "cmre.h"

typedef long long s64;
typedef unsigned long long u64;

/* 0x00945ff0 is the CRT extended-power core: it consumes the two values
   already on the x87 stack (st(1) = base, st(0) = exponent) and leaves the
   power on top.  The C lane has no math library, and the listing loads the
   same image constant K_968950 into st(0) at every one of its three call
   sites, so the recovered value is the square of the base. */
static long double x87_pow(long double base, long double exponent)
{
  (void)exponent;
  return base * base;
}

#define FS0 CM_FS(void *, 0)

/* ---- original globals (values, not object addresses) ---- */
#define G_AE1050 CM_GLOBAL(u32, 0x00ae1050)
#define G_AE1080 CM_GLOBAL(u32, 0x00ae1080)
#define G_AE233C CM_GLOBAL(u8, 0x00ae233c)
#define G_AE2364 CM_GLOBAL(s32, 0x00ae2364)
#define G_AE23B4 CM_GLOBAL(u32, 0x00ae23b4)
#define G_AE2A38 CM_GLOBAL(u32, 0x00ae2a38)
#define G_AE2C90 CM_GLOBAL(u32, 0x00ae2c90)
#define G_AE2C92 CM_GLOBAL(u16, 0x00ae2c92)
#define G_AE2C94 CM_GLOBAL(u32, 0x00ae2c94)
#define G_AE2D0C CM_GLOBAL(u32, 0x00ae2d0c)
#define G_B63D28 CM_GLOBAL(u32, 0x00b63d28)
#define G_DE1F54 CM_GLOBAL(u32, 0x00de1f54)

/* ---- double constants read in place from the image ---- */
#define K_967890 CM_GLOBAL(double, 0x00967890)
#define K_9688F0 CM_GLOBAL(double, 0x009688f0)
#define K_968928 CM_GLOBAL(double, 0x00968928)
#define K_968940 CM_GLOBAL(double, 0x00968940)
#define K_968950 CM_GLOBAL(double, 0x00968950)
#define K_968968 CM_GLOBAL(double, 0x00968968)
#define K_968978 CM_GLOBAL(double, 0x00968978)
#define K_9689A0 CM_GLOBAL(double, 0x009689a0)
#define K_9689C0 CM_GLOBAL(double, 0x009689c0)
#define K_968A80 CM_GLOBAL(double, 0x00968a80)
#define K_968DC0 CM_GLOBAL(double, 0x00968dc0)
#define K_968DC8 CM_GLOBAL(double, 0x00968dc8)
#define K_968DD0 CM_GLOBAL(double, 0x00968dd0)
#define K_968DD8 CM_GLOBAL(double, 0x00968dd8)
#define K_968DE0 CM_GLOBAL(double, 0x00968de0)
#define K_968DE8 CM_GLOBAL(double, 0x00968de8)
#define K_968DF0 CM_GLOBAL(double, 0x00968df0)
#define K_968DF8 CM_GLOBAL(double, 0x00968df8)
#define K_968E00 CM_GLOBAL(double, 0x00968e00)
#define K_968E08 CM_GLOBAL(double, 0x00968e08)
#define K_968E10 CM_GLOBAL(double, 0x00968e10)
#define K_968E18 CM_GLOBAL(double, 0x00968e18)
#define K_968E20 CM_GLOBAL(double, 0x00968e20)
#define K_968E28 CM_GLOBAL(double, 0x00968e28)

/* ---- original callees (explicit boundaries) ---- */
extern u32 CM_THISCALL cm_fn_0085e410(u32 self);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_CDECL cm_fn_0053b620(u32 arg);
extern u8 CM_CDECL cm_fn_00544820(u32 *object);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag,
                                   u32 person, u32 force);
extern u32 CM_THISCALL cm_fn_008cca10(u32 self, u32 record, u32 output);
extern u32 CM_THISCALL cm_fn_008c61e0(u32 self, u32 index);
extern u8 CM_THISCALL cm_fn_00849f00(u32 self, u32 person, u32 club,
                                     u32 output);
extern u32 CM_THISCALL cm_fn_0085fc50(u32 self, u32 a1, u32 a2, u32 a3,
                                      u32 a4, u32 a5, u32 a6, u32 a7);
extern u32 CM_THISCALL cm_fn_005a0590(u32 self, u32 arg);
extern u32 CM_THISCALL cm_fn_005968d0(u32 self, u32 person, u32 flags);
extern u32 CM_THISCALL cm_fn_00597db0(u32 self, u32 kind);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, u32 person, u32 club);
extern int CM_THISCALL cm_fn_0054cdb0(u32 self, u32 a, u32 b);
extern long long CM_THISCALL cm_fn_008e7b50(u32 self, u32 a, u32 b, u32 c,
                                            u32 d, u32 e);
extern void CM_THISCALL cm_fn_00417c10(u32 self, u32 *a, u16 *b);
extern void CM_THISCALL cm_fn_004cc490(u32 self, int *a, int b, int c,
                                       u32 d, u32 e, u32 f, int g);
extern u32 CM_THISCALL cm_fn_0062ed70(u32 self, u32 arg);
extern u32 CM_THISCALL cm_fn_00867520(u32 self, u32 option);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32 self, u32 *a, u32 b);

/* 0x00945640 is MSVC's __ftol: truncation of the extended value to 64 bits;
   the callers only observe EAX, i.e. the low 32 bits. */
#define FTOL(x) ((u32)(long long)(x))

/* original scratch slots: the listing only writes part of some of them, so the
   untouched bytes keep the caller-frame contents */
#define DA (*(double *)(void *)dA)
#define DB (*(double *)(void *)dB)
#define DAW(i) (*(u32 *)(void *)(dA + 4u * (i)))
#define DBW(i) (*(u32 *)(void *)(dB + 4u * (i)))
#define V04 (*(u32 *)(void *)v04b)

/* scratch record at frame-relative 0x28 (0x50 bytes), filled by 0x85e410.
   Accesses keep the listing's widths (byte / word / dword). */
#define SB(o) (st[(o)])
#define SH(o) (*(u16 *)(void *)(st + (o)))
#define SW(o) (*(u32 *)(void *)(st + (o)))
#define SWs(o) ((s32)SW(o))
#define W16(o, v) (*(u16 *)(void *)(st + (o)) = (u16)(v))
#define W32(o, v) (*(u32 *)(void *)(st + (o)) = (u32)(v))

/* output record addressed through the third incoming argument */
#define OB(o) (*(u8 *)(void *)(unsigned long)((p3 + (u32)(o))))
#define OW16(o, v) (*(u16 *)(void *)(unsigned long)(p3 + (u32)(o)) = (u16)(v))
#define OW32(o, v) (*(u32 *)(void *)(unsigned long)(p3 + (u32)(o)) = (u32)(v))

u32 CM_THISCALL
cm_fn_004cda70(u32 self, u32 p1, u32 p2, u32 p3, s32 p4, double p5, u8 p6)
{
  volatile struct { void *prev; void *handler; s32 state; } frame;
  u8 st[0x50];
  u8 dA[8];         /* frame+0x18 scratch double */
  u8 dB[8];         /* frame+0x20 scratch double */
  u8 v04b[4];       /* frame+0x04 scratch dword */
  u32 v0c = 0;      /* frame+0x0c */
  u8 dead03 = 0;    /* frame+0x03: write-only in the listing */
  u32 esi, ebx, ebp, edi;
  u16 bp = 0;
  u32 ecxreg = 0;
  u32 res;

  frame.state = -1;
  frame.handler = (void *)(unsigned long)0x00956d68u;
  frame.prev = FS0;
  FS0 = (void *)(void *)&frame.prev;

  cm_fn_0085e410((u32)(unsigned long)(void *)st);

  esi = p2;
  ebx = p1;
  ebp = 0;
  frame.state = 0;

  if (esi == 0) {
    u32 q = 0;
    if (ebx != 0) {
      s32 id = *(s32 *)(void *)(unsigned long)ebx;
      if (id < G_AE2364) {
        s32 v = *(s32 *)(void *)(unsigned long)(G_AE2D0C + (u32)id * 79u);
        if (v >= 0) {
          /* 0x004cdae5: mov ecx,[0xae1050] ; 0x004cdaf1: add eax,[ecx] */
          q = (u32)v * 80u + *(u32 *)(unsigned long)G_AE1050;
        }
      }
    }
    G_AE1080 = q;
    if (q == 0) goto L_ret0;
    {
      u32 f4 = *(u32 *)(void *)(unsigned long)(q + 4u);
      esi = G_AE23B4 + f4 * 0x245u;
      if (esi == 0) goto L_ret0;
    }
  }

  if (cm_fn_0053b900((u32 *)(void *)(unsigned long)esi) != 0) goto L_ret0;

  {
    u32 id = *(u32 *)(void *)(unsigned long)ebx;
    if ((*(u8 *)(void *)(unsigned long)(G_AE2D0C + id * 79u + 0xbu) &
         0x30u) != 0) goto L_ret0;
  }

  if (cm_fn_0053b620((u32)*(u8 *)(void *)(unsigned long)(ebx + 0x3du)) == 0) {
    u32 p39 = *(u32 *)(void *)(unsigned long)(ebx + 0x39u);
    if (esi == p39) {
      u32 idx = *(u32 *)(void *)(unsigned long)p39;
      u32 mgr = *(u32 *)(void *)(unsigned long)(G_DE1F54 + 0x856u);
      u32 tbl = *(u32 *)(void *)(unsigned long)(mgr + 0xdcu);
      if (*(u8 *)(void *)(unsigned long)(tbl + idx * 9u + 4u) == 2u)
        goto L_ret0;
      if (*(u32 *)(void *)(unsigned long)(esi + 0xcfu) == 0) {
        if (cm_fn_00544820((u32 *)(void *)(unsigned long)esi) != 0)
          goto L_ret0;
      }
    }
  }

  if (cm_fn_00600a50(esi, 0, 1, 0, 0) != 0) goto L_ret0;

  if (cm_fn_008cca10(G_DE1F54, ebx, 0) != 0) goto L_ret0;

  W32(0x04, *(u32 *)(void *)(unsigned long)esi);

  edi = (u32)p4;
  if (p4 != -1) {
    u32 r = cm_fn_008c61e0(G_DE1F54, edi);
    if (r != 0) ebp = *(u32 *)(void *)(unsigned long)r;
  }

  {
    u32 mgr = *(u32 *)(void *)(unsigned long)(G_DE1F54 + 0x856u);
    u8 rr = cm_fn_00849f00(mgr, ebx, esi, (u32)(unsigned long)(void *)&v0c);
    v04b[0] = (u8)rr;
    if (rr == 3u) {
      u32 pp = *(u32 *)(void *)(unsigned long)(ebx + 0x61u);
      if (*(s16 *)(void *)(unsigned long)(pp + 0xbu) > 0x1482)
        v04b[0] = 2u;
    }
  }

  {
    u32 r = cm_fn_0085fc50((u32)(unsigned long)(void *)st, ebx, esi, edi,
                           V04, v0c, 0, 0);
    V04 = r;
    if ((double)(s32)r == K_9689A0) goto L_ret0;
  }

  {
    long double x1 = (long double)SWs(0x08) * (long double)p5;
    long double x2;
    W32(0x08, FTOL(x1));
    x2 = (long double)SWs(0x0c) * (long double)p5;
    W32(0x0c, FTOL(x2));
  }

  if (SB(0x1d) != 0) {
    edi = 0;
    if (ebp != 0) {
      long long rv = cm_fn_008e7b50(ebp, 0, 0, 0, 0, 1);
      s32 v = (s32)(u32)rv;
      if (v > 0x4c4b40) {
        long double u = (long double)(s32)(u32)(v + 0xffb3b4c0u);
        edi = FTOL(u * K_968928 + K_968E28);
      } else if (v > 0xf4240) {
        long double u = (long double)(s32)(u32)(v + 0xfff0bdc0u);
        edi = FTOL(u * K_968940 + K_968E20);
      } else if (v > 0xc350) {
        long double u = (long double)(s32)(u32)(v + 0xffff3cb0u);
        edi = FTOL(u * K_968E18 + K_9688F0);
      } else {
        if (SWs(0x21) < 0x186a0) edi = 0x186a0u;
      }
    } else {
      if (SWs(0x21) < 0xc350) edi = 0xc350u;
    }
    if (SWs(0x21) < (s32)edi) W32(0x21, edi);
  } else {
    if (SB(0x1f) != 0 || SB(0x1c) != 0) {
      if (SWs(0x21) < 0xc350) W32(0x21, 0xc350u);
    }
  }

  if ((u8)v0c == 0x0bu) {
    ecxreg = (ecxreg & 0xffffff00u) | (u32)(u8)(SB(0x4f) >> 4);
    {
      u32 r1 = cm_fn_005a0590(G_AE2A38, esi);
      u32 k = cm_fn_005968d0(r1, ebx, ecxreg);
      V04 = k;
      DA = (double)((long double)(s32)k *
                    (((long double)p5 + K_967890) * K_968968));
    }
  } else {
    u32 r1 = cm_fn_005a0590(G_AE2A38, esi);
    u32 k = cm_fn_00597db0(r1, v0c);
    V04 = k;
    DA = (double)(s32)k;
  }

  ebx = cm_fn_004cf5f0(self, p1, esi);

  if (*(u32 *)(void *)(unsigned long)(p1 + 0x61u) == 0) {
    bp = G_AE2C92;
    goto L_174;
  }

  {
    int r7 = cm_fn_0054cdb0(0x00ae2c90u,
                            *(u32 *)(void *)(unsigned long)(p1 + 0x10u),
                            *(u32 *)(void *)(unsigned long)(p1 + 0x14u));
    u32 p61b;
    u32 saved14;
    long double t;

    V04 = (u32)r7;
    p61b = *(u32 *)(void *)(unsigned long)(p1 + 0x61u);
    t = (long double)(s32)V04 * K_968E10;
    if (*(u8 *)(void *)(unsigned long)(p61b + 0xfu) == 0x14u) {
      t = t - K_968950;
    } else {
      s32 sum = (s32)*(s8 *)(void *)(unsigned long)(p61b + 0x3cu)
              + (s32)*(s8 *)(void *)(unsigned long)(p61b + 0x34u)
              + (s32)*(s8 *)(void *)(unsigned long)(p1 + 0x5bu);
      long double u;
      V04 = (u32)sum;
      u = ((long double)sum * K_968A80 - K_968E08) * K_968E00;
      t = t - u;
    }
    DB = (double)t;
    bp = G_AE2C92;

    if (t > (long double)K_968DF8) goto L_c;
    if (ebx != 0 &&
        ((*(u8 *)(void *)(unsigned long)(ebx + 0x4fu) & 0xf0u) == 0x60u))
      goto L_c;
    if (!(t > (long double)K_968DF0)) goto L_deea;

    /* 0x004cde85: rating just below the upper threshold */
    {
      u32 cur = SW(0x2f) & 0xffffu;
      s32 want = (s32)((u32)bp + 2u);
      if ((s32)cur > want) {
        long double u = (long double)SWs(0x0c) * K_9689C0;
        W16(0x2f, (u32)bp + 2u);
        W32(0x0c, FTOL(u));
      }
    }
    {
      u32 i;
      for (i = 0; i < 3; i++) {
        s32 x = SWs(0x10 + 4u * i);
        if (x > -1)
          W32(0x10 + 4u * i, FTOL((long double)x * K_968DE8));
      }
    }
    goto L_dfff;

  L_deea:
    if (!(t > (long double)K_968DE0)) goto L_dfff;
    /* 0x004cdefb */
    {
      s16 av = *(s16 *)(void *)(unsigned long)(p61b + 0xdu);
      s16 bv = *(s16 *)(void *)(unsigned long)(esi + 0x80u);
      if ((long double)bv > (long double)av * K_968978) {
        u32 ft = FTOL(t + K_968968);
        s32 al8 = (s32)(s8)(u8)ft;
        s32 want = (s32)(u32)bp - al8 + 0x22;
        u32 cur = SW(0x2f) & 0xffffu;
        u32 i;
        if ((s32)cur > want) {
          long double u = (long double)SWs(0x0c) * K_9689C0;
          W16(0x2f, (u16)((u16)(u32)bp - (u16)(s16)(s8)(u8)ft + 0x22u));
          W32(0x0c, FTOL(u));
        }
        for (i = 0; i < 3; i++) {
          s32 x = SWs(0x10 + 4u * i);
          if (x > -1)
            W32(0x10 + 4u * i, FTOL((long double)x * K_968940));
        }
      }
    }
    goto L_dfff;

  L_c:
    /* 0x004cdfa3 */
    {
      u32 cur = SW(0x2f) & 0xffffu;
      s32 want = (s32)((u32)bp + 1u);
      if ((s32)cur > want) {
        long double u = (long double)SWs(0x0c) * K_9689C0;
        W16(0x2f, (u32)bp + 1u);
        W32(0x0c, FTOL(u));
      }
    }
    {
      u32 i;
      for (i = 0; i < 3; i++) {
        s32 x = SWs(0x10 + 4u * i);
        if (x > -1)
          W32(0x10 + 4u * i, FTOL((long double)x * K_968928));
      }
    }

  L_dfff:
    if (SWs(0x0c) <= 0x2710) goto L_174;
    if (!(t > (long double)K_968DD8)) goto L_174;

    {
      s32 r7b = cm_fn_0054cdb0((u32)(unsigned long)(void *)&st[0x2d],
                               G_AE2C90, G_AE2C94);
      u32 esi2;
      s32 w;
      long double cap;
      long double den;
      s32 quot;
      long double lhs;
      double dCap;

      edi = (u32)(s32)(r7b / 7);
      p61b = *(u32 *)(void *)(unsigned long)(p1 + 0x61u);
      esi2 = edi * SW(0x0c);
      w = (s32)*(s16 *)(void *)(unsigned long)(p61b + 0xbu);
      V04 = (u32)w;
      saved14 = esi2;

      cap = x87_pow((long double)w, (long double)K_968950) * K_968DD0;
      if (cap < (long double)K_968DC8)
        dCap = (double)cap;
      else
        dCap = 8000.0;

      den = x87_pow((long double)DB - K_968DD8, (long double)K_968950);
      if (den > (long double)K_967890)
        DB = (double)den;
      else
        DB = 1.0;

      quot = (s32)FTOL(x87_pow((long double)dCap, (long double)K_968950) /
                       (long double)DB);
      ebp = (u32)quot;
      V04 = ebp;

      lhs = (long double)(s32)ebp * K_968940;
      if (lhs < (long double)(s32)saved14) {
        for (;;) {
          if ((s32)edi <= 0x68) goto L_13e;
          {
            long double u = (long double)SWs(0x0c) * K_9689C0;
            u32 fv = FTOL(u);
            u32 ni;
            edi = edi - 0x34u;
            ni = edi;
            W32(0x0c, fv);
            esi2 = ni * fv;
            saved14 = esi2;
          }
          if (!((s32)ebp < (s32)saved14)) {
            bp = G_AE2C92;
            goto L_174;
          }
        }
      }
    L_13e:
      if ((s32)saved14 > (s32)ebp) {
        s32 q2 = (s32)ebp / (s32)edi;
        if ((long double)q2 < (long double)DA) {
          DA = (double)q2;
          bp = G_AE2C92;
          goto L_174;
        }
      }
      bp = G_AE2C92;
      goto L_174;
    }
  }

L_174:
  if (ebx != 0) {
    u16 dx = (u16)(SW(0x2f) & 0xffffu);
    if (!(*(u16 *)(void *)(unsigned long)(ebx + 0x2fu) < dx)) goto L_ret0;
    if (G_B63D28 != 0) {
      u32 r = cm_fn_0062ed70(G_B63D28, p1);
      if (r != 0) {
        u32 r2 = cm_fn_0062ed70(G_B63D28, p1);
        if (*(s16 *)(void *)(unsigned long)(r2 + 0x2cu) > 0x46) goto L_ret0;
      }
      bp = G_AE2C92;
    }
  }

  if (G_AE233C == 1u) {
    u32 cur = SW(0x2f) & 0xffffu;
    s32 want = (s32)((u32)bp + 5u);
    if ((s32)cur > want) {
      bp = (u16)((u32)bp + 5u);
      W16(0x2f, bp);
    }
  }

  {
    s32 S = SWs(0x0c);
    if ((long double)S > (long double)DA * K_968DC0) goto L_ret0;
    if ((long double)S > (long double)DA) {
      long double mx = (K_967890 <= (long double)DA)
                       ? (long double)DA : (long double)K_967890;
      long double ratio = (long double)S / mx;
      u32 i;
      W32(0x08, FTOL((long double)SWs(0x08) * ratio));
      for (i = 0; i < 3; i++) {
        s32 x = SWs(0x10 + 4u * i);
        if (x > -1)
          W32(0x10 + 4u * i, FTOL((long double)x * ratio));
      }
      W32(0x0c, FTOL((long double)DA));
    }
  }

  cm_fn_00867520((u32)(unsigned long)(void *)st, 0);

  if (p3 != 0) {
    OW32(0x00, SW(0x00));
    OW32(0x04, SW(0x04));
    OW32(0x08, SW(0x08));
    OW32(0x0c, SW(0x0c));
    OW32(0x10, SW(0x10));
    OW32(0x14, SW(0x14));
    OW32(0x18, SW(0x18));
    OW32(0x1c, SW(0x1c));
    OB(0x20) = SB(0x20);
    {
      u32 n21 = SW(0x21);
      u32 p25 = (u32)(unsigned long)(void *)&st[0x25];
      u32 p2d = (u32)(unsigned long)(void *)&st[0x2d];
      u32 pa = (u32)(unsigned long)(void *)(p3 + 0x25u);
      u32 pb = (u32)(unsigned long)(void *)(p3 + 0x2du);
      u32 pd = (u32)(unsigned long)(void *)dB;
      OW32(0x21, n21);
      cm_fn_00417c10(pa, (u32 *)(unsigned long)pd, (u16 *)(unsigned long)p25);
      cm_fn_00417c10(pb, (u32 *)(unsigned long)pd, (u16 *)(unsigned long)p2d);
    }
    {
      u8 cl = SB(0x35);
      u8 dl = OB(0x35);
      u8 al;
      al = (u8)(((s8)(u8)(cl << 2)) >> 2);
      al = (u8)(al ^ dl);
      al = (u8)(al & 0x3fu);
      al = (u8)(al ^ dl);
      dl = (u8)(((s8)(u8)(cl << 1)) >> 1);
      dl = (u8)(dl ^ al);
      dl = (u8)(dl & 0xc0u);
      dl = (u8)(dl ^ cl);
      dl = (u8)(dl ^ al);
      {
        u32 n36 = SW(0x36);
        dl = (u8)(dl & 0x7fu);
        OW32(0x36, n36);
        dead03 = (u8)OB(0x4f);
        dl = (u8)(dl ^ cl);
        OB(0x35) = dl;
        OB(0x3a) = SB(0x3a);
        OW16(0x3b, SH(0x3b));
        OB(0x3d) = SB(0x3d);
        OB(0x3e) = SB(0x3e);
        OB(0x3f) = SB(0x3f);
        OW32(0x40, SW(0x40));
        OB(0x44) = SB(0x44);
        OW32(0x45, SW(0x45));
        OB(0x49) = SB(0x49);
        OW32(0x4a, SW(0x4a));
        OB(0x4e) = SB(0x4e);
        OB(0x4f) = SB(0x4f);
      }
      (void)dead03;
    }
    goto L_ret1;
  }

  if (p6 == 2u) goto L_call490;
  if (ebx == 0) goto L_ret1;
  if (p6 != 1u) goto L_ret1;
  {
    u32 r = cm_fn_0054c5b0(0x00ae1068u, (u32 *)(void *)dA, 0x96);
    u32 w0 = *(u32 *)(void *)(unsigned long)r;
    u32 w1 = *(u32 *)(void *)(unsigned long)(r + 4u);
    u16 dx, cx;
    DBW(0) = w0;
    dx = (u16)(w0 >> 16);
    DBW(1) = w1;
    cx = (u16)w0;
    if (dx < *(u16 *)(void *)(unsigned long)(ebx + 0x2fu)) goto L_call490;
    if (dx > *(u16 *)(void *)(unsigned long)(ebx + 0x2fu)) goto L_ret1;
    if ((s16)cx < *(s16 *)(void *)(unsigned long)(ebx + 0x2du))
      goto L_call490;
    goto L_ret1;
  }

L_call490:
  /* 0x004ce416: lea ecx,[esp+0x4c] after five pushes -> the scratch record */
  cm_fn_004cc490(self, (int *)(void *)st, p4, -1,
                 (u32)*(u8 *)(void *)(unsigned long)(p1 + 0x3du), 0, 1, 0);

L_ret1:
  res = 1;
  goto L_exit;

L_ret0:
  res = 0;

L_exit:
  FS0 = frame.prev;
  return res;
}
