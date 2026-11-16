/* fn_005acee0 — comp/fix_man.cpp, VA 0x005acee0, preferred image base 0x00400000.
   Plain-C reconstruction from the pinned 3.9.68 listing; frame offsets are
   relative to L = esp immediately after `sub esp, 0x228` (the four saved
   registers are accounted for). */
#include "cmre.h"

extern u8 CM_CDECL cm_fn_0048dd70(u32 param_1, u32 param_2);
extern u32 cm_fn_0048dee0(u32 obj);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_THISCALL cm_fn_005abf10(u32 self, volatile u32 first,
                                       volatile u32 second, volatile u32 third,
                                       volatile s8 fourth);
extern u32 CM_CDECL cm_fn_005af5b0(u32 arg_1);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(volatile u8 *self, u32 edx_in,
                                                      const u8 *src, s16 count);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern char *CM_CDECL cm_fn_0090d130(const char *p1, const char *p2, u32 code);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

#define F8(o)  (*(volatile u8 *)(F + (o)))
#define F16(o) (*(volatile u16 *)(F + (o)))
#define F32(o) (*(volatile u32 *)(F + (o)))

/* ret 0x48 -> 18 stack dwords; ECX read before written -> thiscall receiver. */
u16 CM_THISCALL
cm_fn_005acee0(u32 self, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5,
               u32 arg6, u32 arg7, u32 arg8, u32 arg9, u32 arg10, u32 arg11,
               u32 arg12, u32 arg13, u32 arg14, u32 arg15, u32 arg16,
               u32 arg17, u32 arg18)
{
  u8 F[0x228];
  u32 month, count, res, i, ok;
  u32 eax_, ebx_, ebp_, esi_, edx_, base, k;
  u8 bl;
  char *sp;

  (void)arg10;
  (void)arg11;
  (void)arg12;

  bl = 0;                                    /* 005acf2d xor bl,bl            */
  F32(0x24) = self;                          /* 005acef4                      */
  F32(0x08) = (u32)(*(volatile u16 *)arg1);  /* 005acef9/005acf02             */
  F32(0x04) = (u32)(*(volatile u16 *)arg2);  /* 005acefc/005acf30             */
  month = (arg3 & 0xffff0000u) | (u32)(*(volatile u8 *)arg3);
  F32(0x18) = 0xffffffffu;                   /* 005acf0e                      */
  F32(0x1c) = 0xffffffffu;                   /* 005acf12                      */
  F32(0x20) = 0xffffffffu;                   /* 005acf22                      */
  ebp_ = arg9;                               /* 005acf1b                      */
  edx_ = arg5;                               /* 005acf26                      */
  F32(0x00) = month;                         /* 005acf40                      */
  F32(0x10) = 0;                             /* 005acf44                      */

  if (edx_ != 0xffffffffu) {                 /* 005acf3c                      */
    eax_ = edx_ * 0x6bu + CM_GLOBAL(u32, 0x00ae23d0);
    F8(0x18) = (u8)cm_fn_0048dd70(eax_, 0);  /* 005acf54..005acf6c            */
    bl = 1;
  } else {
    if (arg8 != 0xffffffffu) {               /* 005acf77                      */
      eax_ = CM_GLOBAL(u32, 0x00ae23b4) + arg8 * 0x245u;
      if (cm_fn_0053b900((u32 *)eax_) != 0) {
        F8(0x18) = 0;                        /* 005acf9c                      */
        F8(0x19) = 1;
        bl = 2;
      } else {
        u32 v;
        v = cm_fn_0048dee0(eax_);            /* 005acfab                      */
        F8(0x18) = (u8)v;                    /* 005acfb1                      */
        v = cm_fn_005af5b0(eax_);            /* 005acfb5                      */
        F8(0x19) = (u8)v;                    /* 005acfba                      */
        bl = 2;
        base = 0x00b63d64u;                  /* 005acfc3                      */
        for (k = 0; k < 0x23u; k++) {        /* 005acfcd..005acff2            */
          if (*(volatile u32 *)(base - 4u) == 0) {
            u32 lst = *(volatile u32 *)(eax_ + 0x53u);
            if (lst != 0 &&
                *(volatile u32 *)base == *(volatile u32 *)(lst + 0x71u)) {
              F8(0x18u + (u32)(s32)(s8)bl) = *(volatile u8 *)(base + 0x10u);
              bl = (u8)(bl + 1);
            }
          }
          base = base + 0x48u;
        }
      }
      month = F32(0x00);                     /* 005acff4                      */
    }
    if (ebp_ != 0xffffffffu) {               /* 005acff8                      */
      u32 v;
      eax_ = CM_GLOBAL(u32, 0x00ae23b4) + ebp_ * 0x245u;
      v = cm_fn_0048dee0(eax_);              /* 005ad012                      */
      i = (u32)(s32)(s8)bl;
      bl = (u8)(bl + 1);
      F8(0x18u + i) = (u8)v;                 /* 005ad01d                      */
      v = cm_fn_005af5b0(eax_);              /* 005ad021                      */
      i = (u32)(s32)(s8)bl;
      bl = (u8)(bl + 1);
      F8(0x18u + i) = (u8)v;                 /* 005ad02e                      */
      base = 0x00b63d64u;                    /* 005ad032                      */
      for (k = 0; k < 0x23u; k++) {          /* 005ad03c..005ad063            */
        if (*(volatile u32 *)(base - 4u) == 0) {
          u32 lst = *(volatile u32 *)(eax_ + 0x53u);
          if (lst != 0 &&
              *(volatile u32 *)base == *(volatile u32 *)(lst + 0x71u)) {
            F8(0x18u + (u32)(s32)(s8)bl) = *(volatile u8 *)(base + 0x10u);
            bl = (u8)(bl + 1);
          }
        }
        base = base + 0x48u;
      }
    }
  }

  month = F32(0x00);                         /* 005ad065                      */
  count = (u32)(s32)(s8)bl;                  /* 005ad069                      */
  F32(0x14) = count;
  F32(0x0c) = 0;
  if ((s32)count - 1 > 0) {                  /* 005ad077                      */
    i = 0;
    for (;;) {
      u8 an = (u8)((u8)i + 1u);              /* 005ad07b                      */
      if ((s8)an < (s8)bl) {
        s32 n = (s32)count - (s32)(s8)an;
        volatile u8 *q = F + 0x18u + i;
        volatile u8 *p = F + 0x18u + (u32)(s32)(s8)an;
        do {
          u8 av = *q;
          if ((s8)av > -1 && av == *p) *p = 0xff;
          p = p + 1;
          n = n - 1;
        } while (n != 0);
      }
      i = i + 1u;                            /* 005ad0a9                      */
      F32(0x0c) = i;
      if (!((s32)i < (s32)count - 1)) break;
    }
  }

  ebx_ = arg6;                               /* 005ad0b9                      */
  if (arg4 != 0) goto L_F_start;             /* 005ad0c0                      */
  goto L_R_start;

  /* ---------------- increasing day-of-year ---------------- */
L_F_start:                                   /* 005ad0c8                      */
  if ((s16)F16(0x04) >= 0x16e) goto L_F_year;
  ebp_ = F32(0x10);                          /* 005ad0d5                      */
L_F_month:                                   /* 005ad0db                      */
  if ((s16)(u16)month >= 3) goto L_F_aftermonth;
L_F_buf:                                     /* 005ad0e5                      */
  if ((s32)F32(0x14) <= 0) goto L_F_montail;
  F32(0x0c) = 0;
L_F_cand:                                    /* 005ad0f5                      */
  {
    u8 av = F8(0x18u + F32(0x0c));
    u32 p4;
    if (av == 0xff) goto L_F_nextbuf;
    p4 = (F32(0x0c) & 0xffffff00u) | (u32)av;
    esi_ = cm_fn_005abf10(F32(0x24), F32(0x08), F32(0x04), month, (s8)p4);
    if (esi_ == 0) goto L_F_reload;
    *(volatile u32 *)(esi_ + 0xau) = *(volatile u32 *)(esi_ + 2u);
    eax_ = cm_fn_0068d1b0(esi_);
    if (eax_ == 0) goto L_F_reload;
  }
L_F_filter:                                  /* 005ad139                      */
  ok = 1;
  {
    u8 cl = (u8)arg16;
    if (cl != 0xfe && cl != *(volatile u8 *)(eax_ + 0x42u)) ok = 0;
  }
  if (arg5 != 0xffffffffu && arg5 != *(volatile u32 *)eax_) ok = 0;
  if (ebx_ != 0xffffffffu) {
    s32 v = (s32)*(volatile s16 *)(eax_ + 0x34u);
    if (arg7 != 0xffffffffu) {
      if ((s32)ebx_ > v || (s32)arg7 < v) ok = 0;
    } else {
      if (ebx_ != (u32)v) ok = 0;
    }
  }
  if (arg8 != 0xffffffffu && arg8 != *(volatile u32 *)(eax_ + 0xcu) &&
      arg8 != *(volatile u32 *)(eax_ + 0x10u)) ok = 0;
  if (arg9 != 0xffffffffu && arg9 != *(volatile u32 *)(eax_ + 0xcu) &&
      arg9 != *(volatile u32 *)(eax_ + 0x10u)) ok = 0;
  {
    u8 cl = (u8)arg17;
    if (cl != 0xff &&
        (cl != *(volatile u8 *)(eax_ + 0x40u) ||
         cl != *(volatile u8 *)(eax_ + 0x41u))) ok = 0;
  }
  {
    u8 cl = (u8)arg15;
    if (cl == 0) {
      if (*(volatile u8 *)(eax_ + 0x43u) != 0xff) ok = 0;
    } else if (cl == 1) {
      if (*(volatile u8 *)(eax_ + 0x43u) == 0xff) ok = 0;
    }
  }
  if ((u8)arg18 == 0 && (s8)*(volatile u8 *)(eax_ + 0x43u) < -1)
    goto L_F_nextcand;
  if (!ok) goto L_F_nextcand;
  if ((u32)cm_fn_0068d0d0((volatile u8 *)arg14, 1u, (const u8 *)eax_,
                          (s16)0x4f) == 0)
    goto L_F_error;                          /* 005ad228                      */
  ebp_ = ebp_ + 1u;                          /* 005ad236                      */
  F32(0x10) = ebp_;
  if ((u16)arg13 == 0xffff) goto L_F_ret1;
  if ((ebp_ & 0xffffu) == (u32)(s32)(s16)(u16)arg13) goto L_F_ret1;
L_F_nextcand:                                /* 005ad258                      */
  eax_ = cm_fn_0068d1b0(esi_);
  if (eax_ != 0) goto L_F_filter;
L_F_reload:                                  /* 005ad267                      */
  month = F32(0x00);
L_F_nextbuf:                                 /* 005ad26b                      */
  eax_ = F32(0x0c) + 1u;
  F32(0x0c) = eax_;
  if ((s32)eax_ < (s32)F32(0x14)) goto L_F_cand;
L_F_montail:                                 /* 005ad280                      */
  month = month + 1u;
  F32(0x00) = month;
  if ((s16)(u16)month < 3) goto L_F_buf;
L_F_aftermonth:                              /* 005ad28f                      */
  F32(0x00) = 0;
  month = 0;
  if ((u16)arg13 == 0xfffe && (u16)ebp_ > 0) goto L_F_retfffe;
  eax_ = F32(0x04) + 1u;                     /* 005ad2a7                      */
  F32(0x04) = eax_;
  if ((s16)(u16)eax_ < 0x16e) goto L_F_month;
L_F_year:                                    /* 005ad2be                      */
  edx_ = F32(0x08) + 1u;
  F32(0x04) = 0;
  eax_ = (u32)((s32)(s16)(u16)edx_ -
               (s32)(s16)(*(volatile u16 *)arg1));
  F32(0x08) = edx_;
  if ((s32)eax_ >= 5) goto L_F_end5;
  if ((u16)arg13 != 0xffff) goto L_F_start;
  if ((s32)eax_ > 1) goto L_F_end1;
  goto L_F_start;

L_F_ret1:                                    /* 005ad370                      */
  *(volatile u16 *)arg2 = F16(0x04);
  *(volatile u16 *)arg1 = F16(0x08);
  *(volatile u8 *)arg3 = (u8)month;
  return (u16)ebp_;

L_F_retfffe:                                 /* 005ad3ab                      */
  *(volatile u16 *)arg2 = F16(0x04);
  *(volatile u16 *)arg1 = F16(0x08);
  *(volatile u8 *)arg3 = 0;
  return (u16)ebp_;

L_F_end5:                                    /* 005ad3e3                      */
  *(volatile u16 *)arg2 = 0;
  *(volatile u16 *)arg1 = (u16)edx_;
  goto L_ret758;

L_F_end1:                                    /* 005ad3f7                      */
  *(volatile u16 *)arg2 = 0;
  goto L_end1year;

L_F_error:                                   /* 005ad304                      */
  /* esp = L-0x10 here, so 005ad304/005ad308 form the scratch buffers at
     L+0x28 (lea ecx,[esp+0x38]) and L+0x128 (lea edx,[esp+0x138]); pushes at
     005ad30f/005ad310 pass them as arg5/arg4, and 005ad320/005ad324 as p2/p1.
     Buffer bases are F+0x28 and F+0x128; F+0x128+0x100 fills the 0x228 frame. */
  cm_fn_00944cff(0x009cc208u, 0, 0, (void *)(F + 0x128), (void *)(F + 0x28));
  sp = cm_fn_0090d130((const char *)(F + 0x128), (const char *)(F + 0x28),
                      0xa54u);
  cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)sp);
  cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
  CM_GLOBAL(u32, 0x00b67a34) = 0;
  return (u16)ebp_;

  /* ---------------- decreasing day-of-year ---------------- */
L_R_start:                                   /* 005ad408                      */
  if ((s16)F16(0x04) < 0) goto L_R_yeardec;
  ebp_ = arg9;                               /* 005ad414                      */
  res = F32(0x10);                           /* 005ad41b                      */
L_R_monchk:                                  /* 005ad41f                      */
  if ((s16)(u16)month < 0) goto L_R_aftermonth;
L_R_buf:                                     /* 005ad428                      */
  if ((s32)F32(0x14) <= 0) goto L_R_montail;
  F32(0x0c) = 0;
L_R_cand:                                    /* 005ad43c                      */
  {
    u8 av = F8(0x18u + F32(0x0c));
    u32 p4;
    if (av == 0xff) goto L_R_nextbuf;
    p4 = (F32(0x0c) & 0xffffff00u) | (u32)av;
    esi_ = cm_fn_005abf10(F32(0x24), F32(0x08), F32(0x04), month, (s8)p4);
    if (esi_ == 0) goto L_R_reload;
    *(volatile u32 *)(esi_ + 0xau) = *(volatile u32 *)(esi_ + 2u);
    eax_ = cm_fn_0068d1b0(esi_);
    if (eax_ == 0) goto L_R_reload;
  }
L_R_filter:                                  /* 005ad480                      */
  ok = 1;
  {
    u8 cl = (u8)arg16;
    if (cl != 0xfe && cl != *(volatile u8 *)(eax_ + 0x42u)) ok = 0;
  }
  if (arg5 != 0xffffffffu && arg5 != *(volatile u32 *)eax_) ok = 0;
  if (ebx_ != 0xffffffffu) {
    s32 v = (s32)*(volatile s16 *)(eax_ + 0x34u);
    if (arg7 != 0xffffffffu) {
      if ((s32)ebx_ > v || (s32)arg7 < v) ok = 0;
    } else {
      if (ebx_ != (u32)v) ok = 0;
    }
  }
  if (arg8 != 0xffffffffu && arg8 != *(volatile u32 *)(eax_ + 0xcu) &&
      arg8 != *(volatile u32 *)(eax_ + 0x10u)) ok = 0;
  if (ebp_ != 0xffffffffu && ebp_ != *(volatile u32 *)(eax_ + 0xcu) &&
      ebp_ != *(volatile u32 *)(eax_ + 0x10u)) ok = 0;
  {
    u8 cl = (u8)arg17;
    if (cl != 0xff &&
        (cl != *(volatile u8 *)(eax_ + 0x40u) ||
         cl != *(volatile u8 *)(eax_ + 0x41u))) ok = 0;
  }
  {
    u8 cl = (u8)arg15;
    if (cl == 0) {
      if (*(volatile u8 *)(eax_ + 0x43u) != 0xff) ok = 0;
    } else if (cl == 1) {
      if (*(volatile u8 *)(eax_ + 0x43u) == 0xff) ok = 0;
    }
  }
  if ((u8)arg18 == 0 && (s8)*(volatile u8 *)(eax_ + 0x43u) < -1)
    goto L_R_nextcand;
  if (!ok) goto L_R_nextcand;
  if ((u32)cm_fn_0068d0d0((volatile u8 *)arg14, 1u, (const u8 *)eax_,
                          (s16)0x4f) == 0)
    goto L_R_error;                          /* 005ad568                      */
  res = res + 1u;                            /* 005ad576                      */
  F32(0x10) = res;
  if ((u16)arg13 == 0xffff) goto L_R_ret1;
  if ((res & 0xffffu) == (u32)(s32)(s16)(u16)arg13) goto L_R_ret1;
L_R_nextcand:                                /* 005ad598                      */
  eax_ = cm_fn_0068d1b0(esi_);
  if (eax_ != 0) goto L_R_filter;
L_R_reload:                                  /* 005ad5a7                      */
  month = F32(0x00);
L_R_nextbuf:                                 /* 005ad5ab                      */
  eax_ = F32(0x0c) + 1u;
  F32(0x0c) = eax_;
  if ((s32)eax_ < (s32)F32(0x14)) goto L_R_cand;
L_R_montail:                                 /* 005ad5c0                      */
  month = month - 1u;
  F32(0x00) = month;
  if ((s16)(u16)month >= 0) goto L_R_buf;
L_R_aftermonth:                              /* 005ad5ce                      */
  F32(0x00) = 2;
  month = 2;
  if ((u16)arg13 == 0xfffe && (u16)res > 0) goto L_R_retfffe;
  eax_ = F32(0x04) - 1u;                     /* 005ad5ea                      */
  F32(0x04) = eax_;
  if ((s16)(u16)eax_ >= 0) goto L_R_monchk;
L_R_yeardec:                                 /* 005ad600                      */
  edx_ = F32(0x08) - 1u;
  F32(0x04) = 0x16d;
  eax_ = (u32)((s32)(s16)(*(volatile u16 *)arg1) -
               (s32)(s16)(u16)edx_);
  F32(0x08) = edx_;
  if ((s32)eax_ >= 5) goto L_R_end5;
  if ((u16)arg13 != 0xffff) goto L_R_start;
  if ((s32)eax_ > 1) goto L_R_end1;
  goto L_R_start;

L_R_ret1:                                    /* 005ad6b9                      */
  *(volatile u16 *)arg2 = F16(0x04);
  *(volatile u16 *)arg1 = F16(0x08);
  *(volatile u8 *)arg3 = (u8)month;
  return (u16)res;

L_R_retfffe:                                 /* 005ad6f4                      */
  *(volatile u16 *)arg2 = F16(0x04);
  *(volatile u16 *)arg1 = F16(0x08);
  *(volatile u8 *)arg3 = 2;
  return (u16)res;

L_R_end5:                                    /* 005ad72c                      */
  *(volatile u16 *)arg2 = 0x16d;
  *(volatile u16 *)arg1 = (u16)edx_;
  goto L_ret758;

L_R_end1:                                    /* 005ad73d                      */
  *(volatile u16 *)arg2 = 0x16d;
L_end1year:                                  /* 005ad749                      */
  *(volatile u16 *)arg1 = F16(0x08);
L_ret758:                                    /* 005ad758                      */
  *(volatile u8 *)arg3 = (u8)month;
  return F16(0x10);

  /* 005ad646 */
L_R_error:
  /* esp = L-0x10 here too (0x68d0d0 is fastcall and pops its two stack args),
     so 005ad646 lea ecx,[esp+0x138] = L+0x128 and 005ad64d lea edx,[esp+0x38]
     = L+0x28; the pushes at 005ad651/005ad652 pass them as arg5/arg4 and
     005ad664/005ad66b as p2/p1. */
  cm_fn_00944cff(0x009cc208u, 0, 0, (void *)(F + 0x28), (void *)(F + 0x128));
  sp = cm_fn_0090d130((const char *)(F + 0x28), (const char *)(F + 0x128),
                      0xaf5u);
  cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)sp);
  cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
  CM_GLOBAL(u32, 0x00b67a34) = 0;
  return (u16)res;
}
