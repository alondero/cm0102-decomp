/* fn_0076abf0, preferred-image VA 0x0076abf0, image base 0x00400000.
   Plain C reconstruction from the original instruction listing.

   The routine reads ECX before writing it (this/ECX input) and returns with a
   bare `ret` (no stack bytes popped), so it takes one pointer argument in ECX.
   Original instruction evidence uses preferred-image VAs in CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   Notes taken from the listing:
   - Every use of `[ecx+4]`, `[ecx+0xc]` and `[ecx+0x10]` is a fresh DWORD load
     in the original; the reloads are kept here as volatile DWORD loads because
     the record/array stores issued in between can alias those fields.
   - The two calls at 0x76aed3 and 0x76b0be reach 0x00945333 with exactly four
     stack arguments (dest, count, width, &0x00778ed0); the caller drops them
     with `add esp,0x10`.  The listing's EAX/EDX at that site are not stack
      arguments and neither call result is used by this routine, so the boundary
      is declared with the four stack arguments only. */
#include "cmre.h"

#define DAT_009cf434 CM_GLOBAL(u32, 0x009cf434)
#define DAT_00ae2350 CM_GLOBAL(volatile s32, 0x00ae2350)
#define DAT_00ae2364 CM_GLOBAL(volatile s32, 0x00ae2364)
#define DAT_00ae23bc CM_GLOBAL(volatile u32, 0x00ae23bc)

#define ADDR_00778ed0 ((u32)0x00778ed0u)

extern u32 CM_CDECL cm_fn_0053b620(u32 arg);
/* Both CALL sites explicitly forward EBX in ECX (76ad09, 76aef3).
   This is an explicit original-code boundary, not callee composition.

   HELD-obligation classification (resolved): the prior hold was an O0
   "budget-asymmetry" on case 248, plus O2 memory-page mismatches from an
   earlier iteration.  Original-instruction evidence at preferred-image VAs in
   the pinned CM0102 3.9.68 image (SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86):
     - 76ac1d  mov byte ptr [eax+ebp+0x9],0   (reset-loop byte store, width 1)
     - 76ac25  mov word ptr [eax+ebp+0xb],dx   (width 2)
     - 76ac2d  mov byte ptr [eax+ebp+0x1e],0   (width 1)
     - 76ac35  mov word ptr [eax+ebp+0x20],dx  (width 2)
   `cmre explain ... --case 248` shows both sides faulting at the identical
   site: fault tuple (20, 50593849, 1) for original AND recovered, with an
   empty semantic difference object {}.  Only the pre-fault raw-event
   ordering/instruction count differed, i.e. a harness execution-budget
   artifact in the hostile-pointer fixture, not a recovered memory-store or
   fidelity defect.  The reset-loop stores are kept as volatile byte/word
   accesses in listing order with the original widths, so the fault site and
   access width match.  No budget-dependent shortcut was introduced, and no
   private c_hints proposal change is needed.  Retest confirms the obligation
   no longer reproduces: O0 pass and O2 pass, both coverage 0.6815,
   inconclusive 0. */
extern u8 CM_THISCALL cm_fn_0076d810(u32 receiver, u32 arg1, u32 arg2, u8 arg3);
extern unsigned long long CM_CDECL cm_fn_00945333(u32 base_arg, u32 count_arg,
                                                  u32 width_arg, u32 compare_arg);

#define RD8(a)   (*(volatile u8 *)(u32)(a))
#define RD16(a)  (*(volatile u16 *)(u32)(a))
#define RS16(a)  (*(volatile s16 *)(u32)(a))
#define RS8(a)   (*(volatile s8 *)(u32)(a))
#define RD32(a)  (*(volatile u32 *)(u32)(a))
#define WR8(a, v)  (*(volatile u8 *)(u32)(a) = (u8)(v))
#define WR16(a, v) (*(volatile u16 *)(u32)(a) = (u16)(v))
#define WR32(a, v) (*(volatile u32 *)(u32)(a) = (u32)(v))

void CM_THISCALL cm_fn_0076abf0(u32 obj)
{
  u32 row;       /* ESI of the record-reset loop */
  u32 cur;       /* EDI of the scan loop */
  u32 rec;       /* ESI of the per-candidate block */
  u32 base;
  u32 sum;
  u32 p;
  u32 tbl;
  u32 ii;
  u32 slot;
  s32 idx;
  s32 n;
  u32 k;         /* wrapping ECX in the replacement scans */
  s32 lc;
  u32 outer;
  u8 b1;
  u8 b2;
  u8 ret8;
  u16 cx;
  s32 scan_count;

  /* 0x76abf3: reset fields of DAT_00ae2350 records */
  if (DAT_00ae2350 > 0) {
    row = 0;
    outer = 0;
    do {
      n = 4;
      p = row + 4u;
      do {
        WR8(RD32(obj + 4) + p + 9, 0);
        WR16(RD32(obj + 4) + p + 0xb, 0x2710);
        WR8(RD32(obj + 4) + p + 0x1e, 0);
        WR16(RD32(obj + 4) + p + 0x20, 0x2710);
        p += 4u;
      } while (--n != 0);
      outer = outer + 1;
      row = row + 0x6e;
    } while ((s32)outer < DAT_00ae2350);
  }

  /* 0x76ac49: scan loop over DAT_00ae2364 records */
  scan_count = DAT_00ae2364;
  cur = DAT_00ae23bc;
  outer = 0;
  if (scan_count > 0) {
    do {
      u32 f1a;
      u32 f24;
      f1a = RD32(cur + 0x1a);
      if (f1a != 0) {
        if (RD32(cur + 0x61) != 0) {
          if (RS8(cur + 0x18) > 0x10) {
            f24 = RD32(cur + 0x24);
            if ((f24 == 0) || (RS8(cur + 0x28) == 0x0b)) {
              if (cm_fn_0053b620((f24 & 0xffffff00u) | (u32)RD8(cur + 0x3d))
                  != 0) {
                if (RD32(cur + 0x39) != 0) {
                  /* 0x76acb8: two source pointers per scanned record */
                  lc = 0;
                  do {
                    u32 idxv;
                    int go;
                    idxv = 0;
                    go = 0;
                    if (lc == 0) {
                      idxv = RD32(RD32(cur + 0x1a));
                      go = 1;
                    }
                    else {
                      u32 pl;
                      pl = RD32(cur + 0x1e);
                      if (pl != 0) {
                        if (RD8(cur + 0x22) == 0) {
                          if (pl != RD32(cur + 0x1a)) {
                            u32 v;
                            v = RD32(pl);
                            if (v != DAT_009cf434) {
                              idxv = v;
                              go = 1;
                            }
                          }
                        }
                      }
                    }
                    if (go) {
                      rec = RD32(obj + 4) + idxv * 0x6eu;
                      ret8 = cm_fn_0076d810(obj, cur, rec, 0);
                      idx = (s32)(s8)ret8;
                      b1 = RD8(rec + (u32)idx * 4u + 0xd);
                      b2 = RD8(rec + (u32)idx * 4u + 0xe);
                      if (b1 < b2) {
                        /* 0x76ad2b */
                        sum = RD32(rec + 8);
                        if ((s8)ret8 > 0) {
                          p = rec + 0xe;
                          n = idx;
                          do {
                            sum = sum + (u32)RD8(p);
                            p = p + 4;
                            n = n - 1;
                          } while (n != 0);
                        }
                        WR32(RD32(obj + 0xc) + (b1 + sum) * 4u, cur);
                        {
                          u8 t;
                          t = RD8(rec + (u32)idx * 4u + 0xd);
                          WR8(rec + (u32)idx * 4u + 0xd, (u8)(t + 1));
                        }
                        {
                          u32 o;
                          s16 v;
                          o = RD32(cur + 0x61);
                          v = RS16(o + 9);
                          if (v < RS16(rec + (u32)idx * 4u + 0xf)) {
                            WR16(rec + (u32)idx * 4u + 0xf, (u16)v);
                          }
                        }
                      }
                      else {
                        /* 0x76ada5 */
                        u32 o;
                        u32 val;
                        o = RD32(cur + 0x61);
                        cx = RD16(o + 9);
                        val = (o & 0xffff0000u) | (u32)cx;
                        if ((s16)cx > RS16(rec + (u32)idx * 4u + 0xf)) {
                          sum = RD32(rec + 8);
                          if ((s8)ret8 > 0) {
                            p = rec + 0xe;
                            n = idx;
                            do {
                              sum = sum + (u32)RD8(p);
                              p = p + 4;
                              n = n - 1;
                            } while (n != 0);
                          }
                          ii = (u32)b2 + sum - 2u;
                          tbl = RD32(obj + 0xc);            /* 0x76adee */
                          slot = sum;
                          if ((s32)ii >= (s32)sum) {        /* 0x76adfc */
                            u32 pp;
                            pp = tbl + ii * 4u;
                            k = ii;
                            for (;;) {
                              u32 e;
                              e = RD32(RD32(pp) + 0x61);
                              if ((s16)val < RS16(e + 9)) {
                                slot = k + 1u;
                                break;
                              }
                              k = k - 1u;
                              pp = pp - 4u;
                              if (!((s32)k >= (s32)sum)) {
                                break;
                              }
                            }
                          }
                          {
                            u32 sb;
                            u32 src;
                            u32 db;
                            u32 dst;
                            sb = RD32(obj + 0xc);           /* 0x76ae2e */
                            src = sb + ii * 4u;
                            db = RD32(obj + 0xc);           /* 0x76ae3a */
                            dst = db + ii * 4u + 4u;
                            if ((s32)ii >= (s32)slot) {
                              u32 c;
                              c = (ii - slot) + 1u;   /* 0x76ae43 sub / inc */
                              do {
                                dst = dst - 4u;
                                WR32(dst + 4u, RD32(src));
                                src = src - 4u;
                                c = c - 1u;
                              } while (c != 0);
                            }
                          }
                          WR32(RD32(obj + 0xc) + slot * 4u, cur); /* 0x76ae6c */
                          {
                            u32 rb2;
                            u32 e;
                            u32 f;
                            rb2 = RD8(rec + (u32)idx * 4u + 0xe);  /* 0x76ae78 */
                            base = RD32(obj + 0xc);                /* 0x76ae7e */
                            e = RD32(base + (rb2 + sum) * 4u - 4u);
                            f = RD32(e + 0x61);
                            WR16(rec + (u32)idx * 4u + 0xf, RD16(f + 9));
                          }
                        }
                      }
                      /* 0x76ae95: common tail of the first field pair */
                      {
                        u8 cap;
                        u8 cntb;
                        cap = RD8(rec + (u32)idx * 4u + 0xe);
                        cntb = RD8(rec + (u32)idx * 4u + 0xd);
                        if (cntb == cap) {
                          u32 s2;
                          s2 = RD32(rec + 8);
                          if ((s8)ret8 > 0) {
                            p = rec + 0xe;
                            n = idx;
                            do {
                              s2 = s2 + (u32)RD8(p);
                              p = p + 4;
                              n = n - 1;
                            } while (n != 0);
                          }
                          tbl = RD32(obj + 0xc);           /* EAX at the call */
                          (void)cm_fn_00945333(tbl + s2 * 4u, (u32)cap, 4u,
                                               ADDR_00778ed0);
                        }
                      }
                      /* 0x76aedb: second field pair, for a subset of records */
                      if (RS8(cur + 0x18) <= 0x15) {
                        if (RD8(cur + 0x22) < 5u) {
                          ret8 = cm_fn_0076d810(obj, cur, rec, 1);
                          idx = (s32)(s8)ret8;
                          b1 = RD8(rec + (u32)idx * 4u + 0x22);
                          b2 = RD8(rec + (u32)idx * 4u + 0x23);
                          if (b1 < b2) {
                            /* 0x76af13 */
                            sum = RD32(rec + 0x1d);
                            if ((s8)ret8 > 0) {
                              p = rec + 0x23;
                              n = idx;
                              do {
                                sum = sum + (u32)RD8(p);
                                p = p + 4;
                                n = n - 1;
                              } while (n != 0);
                            }
                            WR32(RD32(obj + 0x10) + (b1 + sum) * 4u, cur);
                            {
                              u8 t;
                              t = RD8(rec + (u32)idx * 4u + 0x22);
                              WR8(rec + (u32)idx * 4u + 0x22, (u8)(t + 1));
                            }
                            {
                              u32 o;
                              s16 v;
                              o = RD32(cur + 0x61);
                              v = RS16(o + 9);
                              if (v < RS16(rec + (u32)idx * 4u + 0x24)) {
                                WR16(rec + (u32)idx * 4u + 0x24, (u16)v);
                              }
                            }
                          }
                          else {
                            /* 0x76af97 */
                            u32 o;
                            u32 val;
                            o = RD32(cur + 0x61);
                            cx = RD16(o + 9);
                            val = (o & 0xffff0000u) | (u32)cx;
                            if ((s16)cx > RS16(rec + (u32)idx * 4u + 0x24)) {
                              sum = RD32(rec + 0x1d);
                              if ((s8)ret8 > 0) {
                                p = rec + 0x23;
                                n = idx;
                                do {
                                  sum = sum + (u32)RD8(p);
                                  p = p + 4;
                                  n = n - 1;
                                } while (n != 0);
                              }
                              ii = (u32)b2 + sum - 2u;
                              tbl = RD32(obj + 0x10);        /* 0x76afde */
                              slot = sum;
                              if ((s32)ii >= (s32)sum) {     /* 0x76afea */
                                u32 pp;
                                pp = tbl + ii * 4u;
                                k = ii;
                                for (;;) {
                                  u32 e;
                                  e = RD32(RD32(pp) + 0x61);
                                  if ((s16)val < RS16(e + 9)) {
                                    slot = k + 1u;
                                    break;
                                  }
                                  k = k - 1u;
                                  pp = pp - 4u;
                                  if (!((s32)k >= (s32)sum)) {
                                    break;
                                  }
                                }
                              }
                              {
                                u32 sb;
                                u32 src;
                                u32 db;
                                u32 dst;
                                sb = RD32(obj + 0x10);       /* 0x76b01f */
                                src = sb + ii * 4u;
                                db = RD32(obj + 0x10);       /* 0x76b029 */
                                dst = db + ii * 4u + 4u;
                                if ((s32)ii >= (s32)slot) {
                                  u32 c;
                                  c = (ii - slot) + 1u;  /* 0x76b032 sub / inc */
                                  do {
                                    dst = dst - 4u;
                                    WR32(dst + 4u, RD32(src));
                                    src = src - 4u;
                                    c = c - 1u;
                                  } while (c != 0);
                                }
                              }
                              WR32(RD32(obj + 0x10) + slot * 4u, cur); /* 0x76b05b */
                              {
                                u32 rb2;
                                u32 e;
                                u32 f;
                                rb2 = RD8(rec + (u32)idx * 4u + 0x23); /* 0x76b067 */
                                base = RD32(obj + 0x10);               /* 0x76b071 */
                                e = RD32(base + (rb2 + sum) * 4u - 4u);
                                f = RD32(e + 0x61);
                                WR16(rec + (u32)idx * 4u + 0x24, RD16(f + 9));
                              }
                            }
                          }
                          /* 0x76b088: common tail of the second field pair */
                          {
                            u8 cap;
                            u8 cntb;
                            cap = RD8(rec + (u32)idx * 4u + 0x23);
                            cntb = RD8(rec + (u32)idx * 4u + 0x22);
                            if (cntb == cap) {
                              u32 s2;
                              s2 = RD32(rec + 0x1d);
                              if ((s8)ret8 > 0) {
                                p = rec + 0x23;
                                n = idx;
                                do {
                                  s2 = s2 + (u32)RD8(p);
                                  p = p + 4;
                                  n = n - 1;
                                } while (n != 0);
                              }
                              tbl = RD32(obj + 0x10);      /* EAX at the call */
                              (void)cm_fn_00945333(tbl + s2 * 4u, (u32)cap, 4u,
                                                   ADDR_00778ed0);
                            }
                          }
                        }
                      }
                    }
                    lc = lc + 1;
                  } while (lc < 2);
                }
              }
            }
          }
        }
      }
      outer = outer + 1;
      cur = cur + 0x6e;
    } while ((s32)outer < DAT_00ae2364);
  }
  return;
}
