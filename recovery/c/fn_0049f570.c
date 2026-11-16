/* fn_0049f570 (VA 0x0049f570, image base 0x00400000) - comp\comp_stats.cpp
 *
 * Held concern (NOT resolved): the routine hands the packed scratch
 * record at E-0x444 to original callees (0x0054a110 at entry, 0x007e4750 per
 * side) and later reads fields that those callees may not have written
 * (record+0x10 is the packed type byte; 0x007e56d0 reads record+0x19/+0x1a;
 * 0x007e4750 may return 0 for team 0 without writing).  This translation
 * keeps ONE uninitialised automatic scratch block (no zero init, no default
 * record) covering every original E-relative stack local with its exact
 * address, passes pointers into that block to the original callees, and
 * reloads each byte/word/dword at the same width and order as the pinned
 * instructions.  Scratch bytes the original never writes therefore stay
 * indeterminate here too. This does NOT prove original scratch-byte transport:
 * incoming stack bytes and C automatic storage are distinct inputs. In
 * particular team==0 can leave +0x10/+0x19/+0x1a unwritten. No initialization
 * or reachability proof is supplied by the pinned root listing.
 *
 * No replacement helper, no adapter, no inline assembly.
 */
#include "cmre.h"

typedef struct {
  void *prev;
  void *handler;
  u32 state;
} cm_seh_frame;

/* Original stack frame: after `sub esp,0x440; push esi/edi/ebx/ebp` the
   base of the 0x440-byte scratch region sits at E-0x44c.  scratch[i] is the
   byte at E-0x44c+i, i.e. E-relative address (0x45c - i). */
#define LA(x) (0x45cu - (u32)(x))
#define M8(x) (*(volatile u8 *)(scratch + LA(x)))
#define M16(x) (*(volatile u16 *)(scratch + LA(x)))
#define M32(x) (*(volatile u32 *)(scratch + LA(x)))
#define PB(x) ((u32)(scratch + LA(x)))

#define G8(a) (*(volatile u8 *)(u32)(a))
#define G16(a) (*(volatile u16 *)(u32)(a))
#define G32(a) (*(volatile u32 *)(u32)(a))

#define DAT_00ae2c90 0x00ae2c90u
#define DAT_00ae2c74 0x00ae2c74u
#define DAT_00b6b7c8 0x00b6b7c8u
#define DAT_00ae2a38 0x00ae2a38u
#define DAT_00b67a44 0x00b67a44u
#define DAT_00ae2364 0x00ae2364u
#define DAT_00ae23bc 0x00ae23bcu
#define DAT_00ae23c4 0x00ae23c4u

#define CHK5C() ((u32)cm_fn_005c6710((void *)G32(DAT_00ae2c74), \
                                     G32(G32(self_ + 0x502u) + 4u)))
#define DEREF4 (G32(G32(G32(self_ + 0x502u) + 4u)))

extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
extern u32 CM_THISCALL cm_fn_00448110(u32 self, u32 src);
extern u32 CM_THISCALL cm_fn_004a25f0(u32 self, u32 match);
extern u32 CM_THISCALL cm_fn_004a4850(u32 self, u32 target);
extern u32 CM_THISCALL cm_fn_004a4a30(volatile u8 *self, volatile u32 *input);
extern u32 *CM_THISCALL cm_fn_004aceb0(int param_1, u32 *param_2);
extern u32 *CM_THISCALL cm_fn_004ad030(int param_1, u32 *param_2, u32 param_3);
extern u32 CM_CDECL cm_fn_004b0700(u32 v);
extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern u32 *CM_THISCALL cm_fn_0054cc30(void *self, u32 *buf, u32 a, u32 b);
extern void CM_CDECL cm_fn_0058a4d0(volatile u8 *record, s8 byte_arg, u32 value_arg);
extern u32 CM_THISCALL cm_fn_00599850(u32 self, u32 value);
extern u32 CM_THISCALL cm_fn_005a0590(u32 self, u32 arg1);
extern unsigned long long CM_THISCALL cm_fn_005c6710(void *self, u32 arg);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_0074b060(u32 self, u32 person, u32 mode, u32 other,
                                      u32 data, u32 attribute, u32 last);
extern unsigned long long CM_CDECL cm_fn_007e4750(void *first, void *second, void *third);
extern u32 CM_CDECL cm_fn_007e4920(u32 source, u32 destination);
extern u32 CM_CDECL cm_fn_007e4a30(u32 arg1, u32 arg2);
extern u32 CM_CDECL cm_fn_007e4be0(u32 param_1, u32 param_2);
extern unsigned long long CM_CDECL cm_fn_007e56d0(void *record_arg);
/* 0049fde5/0049fdef: only bound is pushed and caller-cleaned. */
extern unsigned long long __attribute__((regparm(2))) cm_fn_0090cfc0(u32 entry_eax, u32 entry_edx, u32 bound);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

u32 CM_THISCALL cm_fn_0049f570(u32 self_, u32 arg0, u32 arg1, short arg2, short arg3)
{
  volatile u8 scratch[0x450];
  volatile cm_seh_frame seh;
  u32 sideidx;
  u32 team;
  u32 rec;
  u32 v;
  u32 a0, b0;
  u8 vlow;
  u32 ebx;
  u32 val;
  s8 barg;
  u32 idx;
  u32 found;
  short sc;

  seh.state = 0xffffffffu;
  seh.handler = (void *)0x009566e7u;
  seh.prev = CM_FS(void *, 0);
  CM_FS(void *, 0) = (void *)&seh;

  cm_fn_0054a110((u16 *)PB(0x444));
  seh.state = 0;

  if (arg0 == 0) {
    cm_fn_00944cff(0x0099ca94u, 0, 0, (void *)PB(0x10c), (void *)PB(0x20c));
    v = (u32)cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, v, PB(0x10c), PB(0x20c), 0xedu);
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    G32(0x00b67a34u) = 0;
    CM_FS(void *, 0) = seh.prev;
    return 0;
  }
  if (arg1 == 0) {
    cm_fn_00944cff(0x0099ca94u, 0, 0, (void *)PB(0x20c), (void *)PB(0x10c));
    v = (u32)cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, v, PB(0x20c), PB(0x10c), 0xf5u);
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    G32(0x00b67a34u) = 0;
    CM_FS(void *, 0) = seh.prev;
    return 0;
  }

  sideidx = 0;

loop_side:
  team = G32(arg0 + (sideidx != 0 ? 0x20u : 0x1cu));
  M32(0x44c) = team;
  cm_fn_007e4750((void *)arg0, (void *)team, (void *)PB(0x444));
  M32(0x414) = sideidx * 1573u;

  v = (u32)cm_fn_004a4850(self_, team);
  if (v == 0) {
    v = (u32)cm_fn_004aceb0((int)self_, (u32 *)team);
  }
  rec = v;

  G8(rec + 4) = (u8)(G8(rec + 4) + 1u);
  v = (u32)cm_fn_007e56d0((void *)PB(0x444));
  vlow = (u8)v;
  {
    u8 al = M8(0x434);
    M8(0x41d) = vlow;
    if (al == 1) goto blk1;
    if (al == 3) goto blk3;
    if (al == 2) goto blk2;
    goto common;
  }

blk1:
  G8(rec + 5) = (u8)(G8(rec + 5) + 1u);
  G32(rec + 6) = (u32)(G32(rec + 6) + G32(arg1 + 0x34u));
  {
    u32 p = rec + 0x13u;
    do {
      G8(p) = G8(p - 1u);
      p--;
    } while ((s32)(p - (rec + 15u)) > 0);
  }
  G8(rec + 0x0fu) = M8(0x41d);
  if (CHK5C() == 0) {
    u32 e34 = G32(arg1 + 0x34u);
    if ((s32)e34 > 0) {
      u32 c = G32(self_ + 0x2b4u);
      if (c == 0xffffffffu || (s32)e34 > (s32)c) {
        G32(self_ + 0x2b4u) = e34;
        cm_fn_00417c10((u32 *)(self_ + 0x2b8u), (u32 *)PB(0x24c), (u16 *)PB(0x444));
        G32(self_ + 0x2c0u) = M32(0x43c);
        G16(self_ + 0x2c4u) = M16(0x438);
        G16(self_ + 0x2c6u) = M16(0x436);
        G8(self_ + 0x2c8u) = M8(0x434);
        G32(self_ + 0x2c9u) = M32(0x433);
        G32(self_ + 0x2cdu) = M32(0x42f);
        G8(self_ + 0x2d1u) = M8(0x42b);
        G8(self_ + 0x2d2u) = M8(0x42a);
        G32(self_ + 0x2d3u) = M32(0x429);
        if (G32(self_ + 0x4bu) == 0xffffffffu ||
            (s32)G32(self_ + 0x4bu) < (s32)G32(arg1 + 0x34u)) {
          if ((s16)G16(self_ + 0x500u) > 2) {
            if ((u32)cm_fn_004b0700(DEREF4) != 0) {
              G32(self_ + 0x51eu) = 1;
              G32(self_ + 0x562u) = 1;
            }
          }
          G32(self_ + 0x4bu) = G32(self_ + 0x2b4u);
          cm_fn_00448110(self_ + 0x4fu, self_ + 0x2b8u);
        }
      }
    }
  }
  if (CHK5C() == 0) {
    u32 e34 = G32(arg1 + 0x34u);
    if ((s32)e34 > 0) {
      u32 c = G32(self_ + 0x2d7u);
      if (c == 0xffffffffu || (s32)e34 < (s32)c) {
        G32(self_ + 0x2d7u) = e34;
        cm_fn_00417c10((u32 *)(self_ + 0x2dbu), (u32 *)PB(0x224), (u16 *)PB(0x444));
        G32(self_ + 0x2e3u) = M32(0x43c);
        G16(self_ + 0x2e7u) = M16(0x438);
        G16(self_ + 0x2e9u) = M16(0x436);
        G8(self_ + 0x2ebu) = M8(0x434);
        G32(self_ + 0x2ecu) = M32(0x433);
        G32(self_ + 0x2f0u) = M32(0x42f);
        G8(self_ + 0x2f4u) = M8(0x42b);
        G8(self_ + 0x2f5u) = M8(0x42a);
        G32(self_ + 0x2f6u) = M32(0x429);
        if (G32(self_ + 0x6eu) == 0xffffffffu ||
            (s32)G32(arg1 + 0x34u) < (s32)G32(self_ + 0x6eu)) {
          if ((s16)G16(self_ + 0x500u) > 2) {
            if ((u32)cm_fn_004b0700(DEREF4) != 0) {
              G32(self_ + 0x522u) = 1;
              G32(self_ + 0x562u) = 1;
            }
          }
          G32(self_ + 0x6eu) = G32(self_ + 0x2d7u);
          cm_fn_00448110(self_ + 0x72u, self_ + 0x2dbu);
        }
      }
    }
  }
  goto common;

blk3:
  if (sideidx != 0) goto common;
  if (CHK5C() == 0) {
    u32 e34 = G32(arg1 + 0x34u);
    if ((s32)e34 > 0) {
      u32 c = G32(self_ + 0x2b4u);
      if (c == 0xffffffffu || (s32)e34 > (s32)c) {
        G32(self_ + 0x2b4u) = e34;
        cm_fn_00417c10((u32 *)(self_ + 0x2b8u), (u32 *)PB(0x3bc), (u16 *)PB(0x444));
        G32(self_ + 0x2c0u) = M32(0x43c);
        G16(self_ + 0x2c4u) = M16(0x438);
        G16(self_ + 0x2c6u) = M16(0x436);
        G8(self_ + 0x2c8u) = M8(0x434);
        G32(self_ + 0x2c9u) = M32(0x433);
        G32(self_ + 0x2cdu) = M32(0x42f);
        G8(self_ + 0x2d1u) = M8(0x42b);
        G8(self_ + 0x2d2u) = M8(0x42a);
        G32(self_ + 0x2d3u) = M32(0x429);
        if (G32(self_ + 0x4bu) == 0xffffffffu ||
            (s32)G32(self_ + 0x4bu) < (s32)G32(arg1 + 0x34u)) {
          if ((s16)G16(self_ + 0x500u) > 2) {
            if ((u32)cm_fn_004b0700(DEREF4) != 0) {
              G32(self_ + 0x51eu) = 1;
              G32(self_ + 0x562u) = 1;
            }
          }
          G32(self_ + 0x4bu) = G32(self_ + 0x2b4u);
          cm_fn_00448110(self_ + 0x4fu, self_ + 0x2b8u);
        }
      }
    }
  }
  if (CHK5C() == 0) {
    u32 e34 = G32(arg1 + 0x34u);
    if ((s32)e34 > 0) {
      u32 c = G32(self_ + 0x2d7u);
      if (c == 0xffffffffu || (s32)e34 < (s32)c) {
        G32(self_ + 0x2d7u) = e34;
        cm_fn_00417c10((u32 *)(self_ + 0x2dbu), (u32 *)PB(0x2dc), (u16 *)PB(0x444));
        G32(self_ + 0x2e3u) = M32(0x43c);
        G16(self_ + 0x2e7u) = M16(0x438);
        G16(self_ + 0x2e9u) = M16(0x436);
        G8(self_ + 0x2ebu) = M8(0x434);
        G32(self_ + 0x2ecu) = M32(0x433);
        G32(self_ + 0x2f0u) = M32(0x42f);
        G8(self_ + 0x2f4u) = M8(0x42b);
        G8(self_ + 0x2f5u) = M8(0x42a);
        G32(self_ + 0x2f6u) = M32(0x429);
        if (G32(self_ + 0x6eu) == 0xffffffffu ||
            (s32)G32(arg1 + 0x34u) < (s32)G32(self_ + 0x6eu)) {
          if ((s16)G16(self_ + 0x500u) > 2) {
            if ((u32)cm_fn_004b0700(DEREF4) != 0) {
              G32(self_ + 0x522u) = 1;
              G32(self_ + 0x562u) = 1;
            }
          }
          G32(self_ + 0x6eu) = G32(self_ + 0x2d7u);
          cm_fn_00448110(self_ + 0x72u, self_ + 0x2dbu);
        }
      }
    }
  }
  goto common;

blk2:
  {
    u32 p = rec + 0x18u;
    do {
      G8(p) = G8(p - 1u);
      p--;
    } while ((s32)(p - (rec + 20u)) > 0);
  }
  G8(rec + 0x14u) = vlow;

common:
  {
    u32 p = rec + 0x0eu;
    do {
      G8(p) = G8(p - 1u);
      p--;
    } while ((s32)(p - (rec + 10u)) > 0);
  }
  G8(rec + 0x0au) = M8(0x41d);

  if ((s8)M8(0x42b) > (s8)M8(0x42a)) goto blkA;
  if ((s8)M8(0x42b) == (s8)M8(0x42a)) goto L41;
  goto blkB;

blkA:
  if (CHK5C() == 0) {
    if (G32(self_ + 0x30cu) == 0xffffffffu ||
        (u32)cm_fn_007e4a30(PB(0x444), self_ + 0x304u) != 0) {
      cm_fn_00417c10((u32 *)(self_ + 0x304u), (u32 *)PB(0x3ac), (u16 *)PB(0x444));
      G32(self_ + 0x30cu) = M32(0x43c);
      G16(self_ + 0x310u) = M16(0x438);
      G16(self_ + 0x312u) = M16(0x436);
      G8(self_ + 0x314u) = M8(0x434);
      G32(self_ + 0x315u) = M32(0x433);
      G32(self_ + 0x319u) = M32(0x42f);
      G8(self_ + 0x31du) = M8(0x42b);
      G8(self_ + 0x31eu) = M8(0x42a);
      G32(self_ + 0x31fu) = M32(0x429);
      if (G32(self_ + 0xa3u) == 0xffffffffu ||
          (u32)cm_fn_007e4a30(PB(0x444), self_ + 0x9bu) != 0) {
        if ((s32)(s8)M8(0x42b) - (s32)(s8)M8(0x42a) >= 8) {
          G32(self_ + 0x52au) = 1;
          G32(self_ + 0x562u) = 1;
        }
        cm_fn_00448110(self_ + 0x9bu, self_ + 0x304u);
      }
    }
  }
  G32(rec + 0x19u) = G32(rec + 0x19u) + 1u;
  if (G32(rec + 0x19u) == 1) {
    cm_fn_00417c10((u32 *)(rec + 0x1du), (u32 *)PB(0x26c), (u16 *)0x00ae2c90u);
  }
  cm_fn_00417c10((u32 *)(rec + 0x25u), (u32 *)PB(0x39c), (u16 *)0x00ae2c90u);
  if (CHK5C() == 0) {
    u32 c = G32(self_ + 0x342u);
    if (c == 0xffffffffu || (s32)G32(rec + 0x19u) > (s32)c) {
      G32(self_ + 0x342u) = G32(rec + 0x19u);
      cm_fn_00417c10((u32 *)(self_ + 0x346u), (u32 *)PB(0x2cc), (u16 *)(rec + 0x1du));
      cm_fn_00417c10((u32 *)(self_ + 0x34eu), (u32 *)PB(0x38c), (u16 *)(rec + 0x25u));
      G32(self_ + 0x356u) = G32(team);
      if (G32(self_ + 0xd9u) == 0xffffffffu ||
          (s32)G32(rec + 0x19u) > (s32)G32(self_ + 0xd9u)) {
        if ((s32)G32(rec + 0x19u) > 0xb) {
          u32 x = G32(self_ + 0xddu);
          M32(0x40c) = x;
          M32(0x408) = G32(self_ + 0xe1u);
          if ((u16)(x >> 16) != G16(rec + 0x1fu) || (u16)x != G16(rec + 0x1du) ||
              G32(team) != G32(self_ + 0xedu)) {
            G32(self_ + 0x532u) = 1;
            G32(self_ + 0x562u) = 1;
          }
        }
        G32(self_ + 0xd9u) = G32(self_ + 0x342u);
        cm_fn_00417c10((u32 *)(self_ + 0xddu), (u32 *)PB(0x21c), (u16 *)(self_ + 0x346u));
        cm_fn_00417c10((u32 *)(self_ + 0xe5u), (u32 *)PB(0x37c), (u16 *)(self_ + 0x34eu));
        G32(self_ + 0xedu) = G32(self_ + 0x356u);
      }
    }
  }
  /* 0x0049fda0 random event (mode 0x51) */
  if (G32(team + 0x57u) != 0 && G32(arg0 + 0x14u) == G32(team + 0x57u)) {
    u32 p53 = G32(team + 0x53u);
    if (p53 != 0 && G8(p53 + 0x11cu) != 0 && G32(team + 0xcfu) != 0) {
      u32 cnt = G32(rec + 0x55u);
      if ((s32)cnt > (s32)((u32)cm_fn_0090cfc0(0, 0, 10) + 10u)) {
        if ((s32)cnt <= (s32)(u32)G8(rec + 4u)) {
          if ((s32)(s16)G16(team + 0x80u) >
              (s32)(u32)cm_fn_0090cfc0(0, 0, 10000)) {
            if ((u32)cm_fn_0090cfc0(0, 0, 3) == 0) {
              u32 base = G32(DAT_00ae23bc);
              u32 count = G32(DAT_00ae2364);
              u32 p;
              idx = 0;
              found = 0;
              p = base;
              if ((s32)count > 0) {
                do {
                  u32 q = G32(p + 0x61u);
                  if (q != 0 && G32(q) == G32(arg1 + 0x3cu) &&
                      !(p != 0 && (s32)G32(p) >= (s32)count)) {
                    if (G32(p + 0x39u) == team) {
                      found = p;
                      break;
                    }
                  }
                  idx++;
                  p += 0x6eu;
                } while ((s32)idx < (s32)count);
              }
              cm_fn_0074b060(G32(DAT_00b6b7c8), team, 0x51u, found, arg0,
                             (idx & 0xffffff00u) | (u32)G8(rec + 0x55u), 0xffffffffu);
            }
          }
        }
      }
    }
  }
  G32(rec + 0x2du) = 0;
  G32(rec + 0x55u) = 0;
  goto L40a010b;

blkB:
  G32(rec + 0x2du) = G32(rec + 0x2du) + 1u;
  if (G32(rec + 0x2du) == 1) {
    cm_fn_00417c10((u32 *)(rec + 0x31u), (u32 *)PB(0x2bc), (u16 *)0x00ae2c90u);
  }
  cm_fn_00417c10((u32 *)(rec + 0x39u), (u32 *)PB(0x36c), (u16 *)0x00ae2c90u);
  if (CHK5C() == 0) {
    u32 c = G32(self_ + 0x35au);
    if (c == 0xffffffffu || (s32)G32(rec + 0x2du) > (s32)c) {
      G32(self_ + 0x35au) = G32(rec + 0x2du);
      cm_fn_00417c10((u32 *)(self_ + 0x35eu), (u32 *)PB(0x25c), (u16 *)(rec + 0x31u));
      cm_fn_00417c10((u32 *)(self_ + 0x366u), (u32 *)PB(0x35c), (u16 *)(rec + 0x39u));
      G32(self_ + 0x36eu) = G32(team);
      if (G32(self_ + 0xf1u) == 0xffffffffu ||
          (s32)G32(rec + 0x2du) > (s32)G32(self_ + 0xf1u)) {
        if ((s32)G32(rec + 0x2du) > 0xb) {
          u32 x = G32(self_ + 0xf5u);
          M32(0x3d4) = x;
          M32(0x3d0) = G32(self_ + 0xf9u);
          if ((u16)(x >> 16) != G16(rec + 0x33u) || (u16)x != G16(rec + 0x31u) ||
              G32(team) != G32(self_ + 0x105u)) {
            G32(self_ + 0x536u) = 1;
            G32(self_ + 0x562u) = 1;
          }
        }
        G32(self_ + 0xf1u) = G32(self_ + 0x35au);
        cm_fn_00417c10((u32 *)(self_ + 0xf5u), (u32 *)PB(0x2ac), (u16 *)(self_ + 0x35eu));
        cm_fn_00417c10((u32 *)(self_ + 0xfdu), (u32 *)PB(0x34c), (u16 *)(self_ + 0x366u));
        G32(self_ + 0x105u) = G32(self_ + 0x36eu);
      }
    }
  }
  /* 0x004a0008 random event (mode 0x50) */
  if (G32(team + 0x57u) != 0 && G32(arg0 + 0x14u) == G32(team + 0x57u)) {
    u32 p53 = G32(team + 0x53u);
    if (p53 != 0 && G8(p53 + 0x11cu) != 0 && G32(team + 0xcfu) != 0) {
      u32 cnt = G32(rec + 0x41u);
      if ((s32)cnt > (s32)((u32)cm_fn_0090cfc0(0, 0, 10) + 10u)) {
        if ((s32)cnt <= (s32)(u32)G8(rec + 4u)) {
          if ((s32)(s16)G16(team + 0x80u) >
              (s32)(u32)cm_fn_0090cfc0(0, 0, 10000)) {
            if ((u32)cm_fn_0090cfc0(0, 0, 3) == 0) {
              u32 base = G32(DAT_00ae23bc);
              u32 count = G32(DAT_00ae2364);
              u32 p;
              idx = 0;
              found = 0;
              p = base;
              if ((s32)count > 0) {
                do {
                  u32 q = G32(p + 0x61u);
                  if (q != 0 && G32(q) == G32(arg1 + 0x3cu) &&
                      !(p != 0 && (s32)G32(p) >= (s32)count)) {
                    if (G32(p + 0x39u) != team) {
                      found = p;
                      break;
                    }
                  }
                  idx++;
                  p += 0x6eu;
                } while ((s32)idx < (s32)count);
              }
              cm_fn_0074b060(G32(DAT_00b6b7c8), team, 0x50u, found, arg0,
                             (idx & 0xffffff00u) | (u32)G8(rec + 0x41u), 0xffffffffu);
            }
          }
        }
      }
    }
  }
  G32(rec + 0x19u) = 0;
  G32(rec + 0x41u) = 0;

L40a010b:
  if ((s8)M8(0x42b) < (s8)M8(0x42a)) goto L275;

L41:
  G32(rec + 0x41u) = G32(rec + 0x41u) + 1u;
  if (G32(rec + 0x41u) == 1) {
    cm_fn_00417c10((u32 *)(rec + 0x45u), (u32 *)PB(0x22c), (u16 *)0x00ae2c90u);
  }
  cm_fn_00417c10((u32 *)(rec + 0x4du), (u32 *)PB(0x33c), (u16 *)0x00ae2c90u);
  if (CHK5C() == 0) {
    u32 c = G32(self_ + 0x372u);
    if (c == 0xffffffffu || (s32)G32(rec + 0x41u) > (s32)c) {
      G32(self_ + 0x372u) = G32(rec + 0x41u);
      cm_fn_00417c10((u32 *)(self_ + 0x376u), (u32 *)PB(0x29c), (u16 *)(rec + 0x45u));
      cm_fn_00417c10((u32 *)(self_ + 0x37eu), (u32 *)PB(0x32c), (u16 *)(rec + 0x4du));
      G32(self_ + 0x386u) = G32(team);
      if (G32(self_ + 0x109u) == 0xffffffffu ||
          (s32)G32(rec + 0x41u) > (s32)G32(self_ + 0x109u)) {
        if ((s32)G32(rec + 0x41u) > 0x17) {
          u32 x = G32(self_ + 0x10du);
          M32(0x3fc) = x;
          M32(0x3f8) = G32(self_ + 0x111u);
          if ((u16)(x >> 16) != G16(rec + 0x47u) || (u16)x != G16(rec + 0x45u) ||
              G32(team) != G32(self_ + 0x11du)) {
            G32(self_ + 0x53au) = 1;
            G32(self_ + 0x562u) = 1;
          }
        }
        G32(self_ + 0x109u) = G32(self_ + 0x372u);
        cm_fn_00417c10((u32 *)(self_ + 0x10du), (u32 *)PB(0x3cc), (u16 *)(self_ + 0x376u));
        cm_fn_00417c10((u32 *)(self_ + 0x115u), (u32 *)PB(0x31c), (u16 *)(self_ + 0x37eu));
        G32(self_ + 0x11du) = G32(self_ + 0x386u);
      }
    }
  }
  G32(rec + 0x2du) = 0;
  goto L27a;

L275:
  goto L27a;

L27a:
  if ((s8)M8(0x42b) > (s8)M8(0x42a)) goto L3e6;

  /* block D: record+0x55 counter */
  G32(rec + 0x55u) = G32(rec + 0x55u) + 1u;
  if (G32(rec + 0x55u) == 1) {
    cm_fn_00417c10((u32 *)(rec + 0x59u), (u32 *)PB(0x28c), (u16 *)0x00ae2c90u);
  }
  cm_fn_00417c10((u32 *)(rec + 0x61u), (u32 *)PB(0x30c), (u16 *)0x00ae2c90u);
  if (CHK5C() == 0) {
    u32 c = G32(self_ + 0x38au);
    if (c == 0xffffffffu || (s32)G32(rec + 0x55u) > (s32)c) {
      G32(self_ + 0x38au) = G32(rec + 0x55u);
      cm_fn_00417c10((u32 *)(self_ + 0x38eu), (u32 *)PB(0x214), (u16 *)(rec + 0x59u));
      cm_fn_00417c10((u32 *)(self_ + 0x396u), (u32 *)PB(0x2fc), (u16 *)(rec + 0x61u));
      G32(self_ + 0x39eu) = G32(team);
      if (G32(self_ + 0x121u) == 0xffffffffu ||
          (s32)G32(rec + 0x55u) > (s32)G32(self_ + 0x121u)) {
        if ((s32)G32(rec + 0x55u) > 0x17) {
          u32 x = G32(self_ + 0x125u);
          M32(0x3dc) = x;
          M32(0x3d8) = G32(self_ + 0x129u);
          if ((u16)(x >> 16) != G16(rec + 0x5bu) || (u16)x != G16(rec + 0x59u) ||
              G32(team) != G32(self_ + 0x135u)) {
            G32(self_ + 0x53eu) = 1;
            G32(self_ + 0x562u) = 1;
          }
        }
        G32(self_ + 0x121u) = G32(self_ + 0x38au);
        cm_fn_00417c10((u32 *)(self_ + 0x125u), (u32 *)PB(0x27c), (u16 *)(self_ + 0x38eu));
        cm_fn_00417c10((u32 *)(self_ + 0x12du), (u32 *)PB(0x2ec), (u16 *)(self_ + 0x396u));
        G32(self_ + 0x135u) = G32(self_ + 0x39eu);
      }
    }
  }
  G32(rec + 0x19u) = 0;

L3e6:
  sc = (sideidx == 0) ? arg2 : arg3;
  if (CHK5C() == 0 && (u32)cm_fn_004b0700(DEREF4) != 0) {
    if ((s32)sc > 0) {
      if ((s16)G16(self_ + 0x294u) == -1 || (s16)G16(self_ + 0x294u) < (s32)sc) {
        G16(self_ + 0x294u) = (u16)sc;
        G32(self_ + 0x296u) = G32(team);
        G16(self_ + 0x29au) = 0;
        if ((s16)G16(self_ + 0x2bu) == -1 || (s16)G16(self_ + 0x2bu) < (s32)sc) {
          if ((s16)G16(self_ + 0x500u) > 2) {
            if ((s32)(s16)G16(G32(self_ + 0x502u) + 0x40u) != (s32)G16(self_ + 0x31u) ||
                G32(team) != G32(self_ + 0x2du)) {
              if ((u32)cm_fn_004b0700(DEREF4) != 0) {
                G32(self_ + 0x50eu) = 1;
                G32(self_ + 0x562u) = 1;
              }
            }
          }
          G32(self_ + 0x2bu) = G32(self_ + 0x294u);
          G32(self_ + 0x2fu) = G32(self_ + 0x298u);
          G16(self_ + 0x31u) = G16(G32(self_ + 0x502u) + 0x40u);
        }
      }
    }
  }

  if (M8(0x42b) != 0) {
    G8(rec + 0x69u) = (u8)(G8(rec + 0x69u) + M8(0x42b));
    if (CHK5C() == 0) {
      u16 cnt = (u16)G8(rec + 0x69u);
      if ((s16)G16(self_ + 0x29cu) == -1 || (s16)G16(self_ + 0x29cu) < (s16)cnt) {
        G16(self_ + 0x29cu) = cnt;
        G32(self_ + 0x29eu) = G32(team);
        G16(self_ + 0x2a2u) = 0;
        if ((s16)G16(self_ + 0x33u) == -1 || (s16)G16(self_ + 0x33u) < (s16)cnt) {
          if ((s16)G16(self_ + 0x500u) > 2) {
            if ((s32)(s16)G16(G32(self_ + 0x502u) + 0x40u) != (s32)G16(self_ + 0x39u) ||
                G32(team) != G32(self_ + 0x35u)) {
              if ((u32)cm_fn_004b0700(DEREF4) != 0) {
                G32(self_ + 0x512u) = 1;
                G32(self_ + 0x562u) = 1;
              }
            }
          }
          G32(self_ + 0x33u) = G32(self_ + 0x29cu);
          G32(self_ + 0x37u) = G32(self_ + 0x2a0u);
          G16(self_ + 0x39u) = G16(G32(self_ + 0x502u) + 0x40u);
        }
      }
    }
  }
  if (M8(0x42a) != 0) {
    G8(rec + 0x6au) = (u8)(G8(rec + 0x6au) + M8(0x42a));
    if (CHK5C() == 0) {
      u16 cnt = (u16)G8(rec + 0x6au);
      if ((s16)G16(self_ + 0x2a4u) == -1 || (s16)G16(self_ + 0x2a4u) < (s16)cnt) {
        G16(self_ + 0x2a4u) = cnt;
        G32(self_ + 0x2a6u) = G32(team);
        G16(self_ + 0x2aau) = 0;
        if ((s16)G16(self_ + 0x3bu) == -1 || (s16)G16(self_ + 0x3bu) < (s16)cnt) {
          if ((s16)G16(self_ + 0x500u) > 2) {
            if ((s32)(s16)G16(G32(self_ + 0x502u) + 0x40u) != (s32)G16(self_ + 0x41u) ||
                G32(team) != G32(self_ + 0x3du)) {
              if ((u32)cm_fn_004b0700(DEREF4) != 0) {
                G32(self_ + 0x516u) = 1;
                G32(self_ + 0x562u) = 1;
              }
            }
          }
          G32(self_ + 0x3bu) = G32(self_ + 0x2a4u);
          G32(self_ + 0x3fu) = G32(self_ + 0x2a8u);
          G16(self_ + 0x41u) = G16(G32(self_ + 0x502u) + 0x40u);
        }
      }
    }
  }

  {
    u32 off = M32(0x414);
    G8(rec + 0x6bu) = (u8)(G8(rec + 0x6bu) + G8(arg1 + off + 0x122u));
    G8(rec + 0x6cu) = (u8)(G8(rec + 0x6cu) + G8(arg1 + off + 0x123u));
    G8(rec + 0x6du) = (u8)(G8(rec + 0x6du) + G8(arg1 + off + 0x124u));
    G8(rec + 0x6eu) = (u8)(G8(rec + 0x6eu) + G8(arg1 + off + 0x125u));
    G8(rec + 0x6fu) = (u8)(G8(rec + 0x6fu) + G8(arg1 + off + 0x126u));
    {
      u8 ca = G8(arg1 + off + 0x12eu);
      u8 cb = G8(arg1 + off + 0x12fu);
      if (ca != 0 || cb != 0) {
        G8(rec + 0x70u) = (u8)(G8(rec + 0x70u) + ca);
        G8(rec + 0x71u) = (u8)(G8(rec + 0x71u) + cb);
        {
          u32 s01 = (u32)G8(rec + 0x70u) + (u32)G8(rec + 0x71u) * 4u;
          if (G32(DAT_00ae2a38) == 0 || G32(DAT_00b67a44) == 0) goto L08bc;
          if (team == 0) {
            barg = 0;
          } else {
            u32 tp = (u32)G32(team) * 73u + 0x40u + G32(G32(DAT_00b67a44));
            barg = (s8)((s32)(s8)(u8)(G8(tp) << 5) >> 5);
          }
          M8(0x410) = (u8)barg;
          v = cm_fn_005a0590(G32(DAT_00ae2a38), team);
          M32(0x448) = v;
          if (barg == 0) {
            if (s01 <= 0x41u) goto L08bc;
            val = (u32)((s32)(s16)G16(team + 0x80u) * 3 + 0x9c4);
            barg = 0;
            goto L080c;
          } else if (barg == 1) {
            if (s01 <= 0x5au) goto L08bc;
            val = (u32)(((s32)(s16)G16(team + 0x80u) + 2000) * 5);
            barg = 1;
            goto L080c;
          } else if (barg == 2) {
            if (s01 <= 0x73u) goto L08bc;
            val = (u32)(((s32)(s16)G16(team + 0x80u) + 0x9c4) * 10);
            barg = 2;
            goto L080c;
          } else {
            if (barg <= 2) goto L08bc;
            if (s01 <= 0x8cu) goto L08bc;
            if ((u32)cm_fn_0090cfc0(0, 0, 10) != 0) goto L08bc;
            val = (u32)(((s32)(s16)G16(team + 0x80u) + 0x9c4) * 10);
            barg = (s8)M8(0x410);
            goto L080c;
          }
        }
      }
    }
  }
  goto L09be;

L080c:
  cm_fn_0058a4d0((volatile u8 *)team, barg, val);
  if (team != 0) {
    u32 tp = (u32)G32(team) * 73u + 0x40u + G32(G32(DAT_00b67a44));
    u8 old = G8(tp);
    s32 sv = (s32)(s8)(u8)(old << 5);
    sv = sv >> 5;
    if (sv < 4) {
      u8 nv = (u8)(((((u8)(sv + 1)) ^ old) & 7u) ^ old);
      G8(tp) = nv;
    }
  }
  cm_fn_00599850(M32(0x448), val);
  /* fall through to the second-record ability block */
L08bc:
  {
    u32 sum;
    /* 0x4a08c0 */
    sum = (u32)G8(rec + 0x71u) * 3u + (u32)G8(rec + 0x70u);
    ebx = sum;
    if (CHK5C() == 0) {
      u8 c = G8(self_ + 0x2acu);
      if (c == 0xffu ||
          (s32)ebx > ((s32)(s8)G8(self_ + 0x2adu) * 3 + (s32)(s8)c)) {
        G8(self_ + 0x2acu) = G8(rec + 0x70u);
        G8(self_ + 0x2adu) = G8(rec + 0x71u);
        {
          u32 id = G32(M32(0x44c)); /* 004a0920, before the word store */
          G16(self_ + 0x2b2u) = 0;
          G32(self_ + 0x2aeu) = id;
        }
        {
          u8 c2 = G8(self_ + 0x43u);
          if (c2 == 0xffu ||
              (s32)ebx > ((s32)(s8)G8(self_ + 0x44u) * 3 + (s32)(s8)c2)) {
            if ((s16)G16(self_ + 0x500u) > 2 && G8(rec + 0x70u) > 1 &&
                G8(rec + 0x71u) > 1) {
              if ((s32)(s16)G16(G32(self_ + 0x502u) + 0x40u) != (s32)G16(self_ + 0x49u) ||
                  G32(team) != G32(self_ + 0x45u)) {
                if ((u32)cm_fn_004b0700(DEREF4) != 0) {
                  G32(self_ + 0x51au) = 1;
                  G32(self_ + 0x562u) = 1;
                }
              }
            }
            G32(self_ + 0x43u) = G32(self_ + 0x2acu);
            G32(self_ + 0x47u) = G32(self_ + 0x2b0u);
            G16(self_ + 0x49u) = G16(G32(self_ + 0x502u) + 0x40u);
          }
        }
      }
    }
  }

L09be:
  if (CHK5C() == 0 && sideidx == 0) {
    if ((s32)(s8)M8(0x42a) + (s32)(s8)M8(0x42b) > 0) {
      if (G32(self_ + 0x32bu) == 0xffffffffu ||
          (u32)cm_fn_007e4be0(PB(0x444), self_ + 0x323u) != 0) {
        cm_fn_00417c10((u32 *)(self_ + 0x323u), (u32 *)PB(0x23c), (u16 *)PB(0x444));
        G32(self_ + 0x32bu) = M32(0x43c);
        G16(self_ + 0x32fu) = M16(0x438);
        G16(self_ + 0x331u) = M16(0x436);
        G8(self_ + 0x333u) = M8(0x434);
        G32(self_ + 0x334u) = M32(0x433);
        G32(self_ + 0x338u) = M32(0x42f);
        G8(self_ + 0x33cu) = M8(0x42b);
        G8(self_ + 0x33du) = M8(0x42a);
        G32(self_ + 0x33eu) = M32(0x429);
        if (G32(self_ + 0xc2u) == 0xffffffffu ||
            (u32)cm_fn_007e4be0(PB(0x444), self_ + 0xbau) != 0) {
          if ((s32)(s8)M8(0x42a) + (s32)(s8)M8(0x42b) >= 10) {
            G32(self_ + 0x52eu) = 1;
            G32(self_ + 0x562u) = 1;
          }
          cm_fn_00417c10((u32 *)(self_ + 0xbau), (u32 *)PB(0x3c4), (u16 *)(self_ + 0x323u));
          G32(self_ + 0xc2u) = G32(self_ + 0x32bu);
          G16(self_ + 0xc6u) = G16(self_ + 0x32fu);
          G16(self_ + 0xc8u) = G16(self_ + 0x331u);
          G8(self_ + 0xcau) = G8(self_ + 0x333u);
          G32(self_ + 0xcbu) = G32(self_ + 0x334u);
          G32(self_ + 0xcfu) = G32(self_ + 0x338u);
          G8(self_ + 0xd3u) = G8(self_ + 0x33cu);
          G8(self_ + 0xd4u) = G8(self_ + 0x33du);
          G32(self_ + 0xd5u) = G32(self_ + 0x33eu);
        }
      }
    }
  }

  /* entity loop, i = 0..0x13 */
  M32(0x448) = 0;
L_ent_loop:
  {
    u32 i = M32(0x448);
    u32 rows2 = arg1 + 0x190u + M32(0x414) + 73u * i;
    u32 f0, f1;
    f0 = G32(rows2);
    if (f0 == 0xffffffffu) goto L_next;
    f1 = G32(rows2 + 4u);
    if (f1 == 0xffffffffu) goto L_next;
    if (i > 10 && G8(rows2 + 0x32u) == 0xffu) goto L_next;
    {
      u32 ebp = G32(DAT_00ae23bc) + f0 * 110u;
      u32 edx = G32(DAT_00ae23c4) + f1 * 70u;
      M32(0x424) = ebp;
      M32(0x418) = edx;
      if (ebp != 0 && (s32)G32(ebp) >= (s32)G32(DAT_00ae2364)) goto L_next;
      v = (u32)cm_fn_004a4a30((volatile u8 *)self_, (volatile u32 *)M32(0x418));
      if (v == 0) {
        v = (u32)cm_fn_004ad030((int)self_, (u32 *)M32(0x418), G32(ebp));
      }
      rec = v;
      G32(rec + 8u) = G32(team);
      G8(rec + 0x1cu) = G8(arg0 + 0x34u);
      G8(rec + 0xcu) = (u8)(G8(rec + 0xcu) + 1u);
      if (i > 10) {
        G8(rec + 0xdu) = (u8)(G8(rec + 0xdu) + 1u);
      }
      {
        u8 al = G8(rows2 + 0x21u);
        if (al != 0) {
          G8(rec + 0xeu) = (u8)(G8(rec + 0xeu) + al);
          if (CHK5C() == 0) {
            if (G8(self_ + 0x3a2u) == 0xffu ||
                (s32)G8(rec + 0xeu) > (s32)(s8)G8(self_ + 0x3a2u)) {
              G8(self_ + 0x3a2u) = G8(rec + 0xeu);
              cm_fn_007e4920(M32(0x424), self_ + 0x3a3u);
              {
                u32 r0 = M32(0x44c);
                u32 id = G32(r0); /* 004a0c63 */
                G16(self_ + 0x3bfu) = 0;
                G32(self_ + 0x3bbu) = id;
              }
              if (G8(self_ + 0x139u) == 0xffu ||
                  (s32)G8(rec + 0xeu) > (s32)(s8)G8(self_ + 0x139u)) {
                if ((s16)G16(self_ + 0x500u) > 2) {
                  if (G32(self_ + 0x146u) != G32(M32(0x424)) ||
                      (u32)G16(self_ + 0x156u) !=
                          (u32)(s32)(s16)G16(G32(self_ + 0x502u) + 0x40u)) {
                    if ((u32)cm_fn_004b0700(DEREF4) != 0) {
                      G32(self_ + 0x542u) = 1;
                      G32(self_ + 0x562u) = 1;
                    }
                  }
                }
                G8(self_ + 0x139u) = G8(self_ + 0x3a2u);
                G32(self_ + 0x13au) = G32(self_ + 0x3a3u);
                G32(self_ + 0x13eu) = G32(self_ + 0x3a7u);
                G32(self_ + 0x142u) = G32(self_ + 0x3abu);
                G32(self_ + 0x146u) = G32(self_ + 0x3afu);
                cm_fn_00417c10((u32 *)(self_ + 0x14au), (u32 *)PB(0x3b4),
                               (u16 *)(self_ + 0x3b3u));
                G32(self_ + 0x152u) = G32(self_ + 0x3bbu);
                G16(self_ + 0x156u) = G16(self_ + 0x3bfu);
                G16(self_ + 0x156u) = G16(G32(self_ + 0x502u) + 0x40u);
              }
            }
          }
          if (CHK5C() == 0) {
            if (G8(self_ + 0x3c1u) == 0xffu ||
                (s32)G8(rows2 + 0x21u) > (s32)(s8)G8(self_ + 0x3c1u)) {
              G8(self_ + 0x3c1u) = G8(rows2 + 0x21u);
              cm_fn_007e4920(M32(0x424), self_ + 0x3c2u);
              cm_fn_00417c10((u32 *)(self_ + 0x3dau), (u32 *)PB(0x3a4),
                             (u16 *)PB(0x444));
              G32(self_ + 0x3e2u) = M32(0x43c);
              G16(self_ + 0x3e6u) = M16(0x438);
              G16(self_ + 0x3e8u) = M16(0x436);
              G8(self_ + 0x3eau) = M8(0x434);
              G32(self_ + 0x3ebu) = M32(0x433);
              G32(self_ + 0x3efu) = M32(0x42f);
              G8(self_ + 0x3f3u) = M8(0x42b);
              G8(self_ + 0x3f4u) = M8(0x42a);
              G32(self_ + 0x3f5u) = M32(0x429);
              if (G8(self_ + 0x158u) == 0xffu ||
                  (s32)G8(rows2 + 0x21u) > (s32)(s8)G8(self_ + 0x158u)) {
                if (G8(rows2 + 0x21u) > 4u) {
                  G32(self_ + 0x546u) = 1;
                  G32(self_ + 0x562u) = 1;
                }
                G8(self_ + 0x158u) = G8(self_ + 0x3c1u);
                G32(self_ + 0x159u) = G32(self_ + 0x3c2u);
                G32(self_ + 0x15du) = G32(self_ + 0x3c6u);
                G32(self_ + 0x161u) = G32(self_ + 0x3cau);
                G32(self_ + 0x165u) = G32(self_ + 0x3ceu);
                cm_fn_00417c10((u32 *)(self_ + 0x169u), (u32 *)PB(0x394),
                               (u16 *)(self_ + 0x3d2u));
                cm_fn_00448110(self_ + 0x171u, self_ + 0x3dau);
              }
            }
          }
        }
      }
      G8(rec + 0xfu) = (u8)(G8(rec + 0xfu) + G8(rows2 + 0x22u));
      G8(rec + 0x10u) = (u8)(G8(rec + 0x10u) + G8(rows2 + 0x23u));
      G8(rec + 0x11u) = (u8)(G8(rec + 0x11u) + G8(rows2 + 0x24u));
      if (G8(rows2 + 0x27u) != 0) {
        G8(rec + 0x12u) = (u8)(G8(rec + 0x12u) + G8(rows2 + 0x27u));
        if (CHK5C() == 0) {
          if (G8(self_ + 0x3f9u) == 0xffu ||
              (s32)G8(rec + 0x12u) > (s32)(s8)G8(self_ + 0x3f9u)) {
            G8(self_ + 0x3f9u) = G8(rec + 0x12u);
            cm_fn_007e4920(M32(0x424), self_ + 0x3fau);
            {
              u32 r0 = M32(0x44c);
              u32 id = G32(r0); /* 004a0f38 */
              G16(self_ + 0x416u) = 0;
              G32(self_ + 0x412u) = id;
            }
            if (G8(self_ + 0x190u) == 0xffu ||
                (s32)G8(rec + 0x12u) > (s32)(s8)G8(self_ + 0x190u)) {
              if ((s16)G16(self_ + 0x500u) > 2) {
                if (G32(self_ + 0x19du) != G32(M32(0x424)) ||
                    (u32)G16(self_ + 0x1adu) !=
                        (u32)(s32)(s16)G16(G32(self_ + 0x502u) + 0x40u)) {
                  if ((u32)cm_fn_004b0700(DEREF4) != 0) {
                    G32(self_ + 0x54au) = 1;
                    G32(self_ + 0x562u) = 1;
                  }
                }
              }
              G8(self_ + 0x190u) = G8(self_ + 0x3f9u);
              G32(self_ + 0x191u) = G32(self_ + 0x3fau);
              G32(self_ + 0x195u) = G32(self_ + 0x3feu);
              G32(self_ + 0x199u) = G32(self_ + 0x402u);
              G32(self_ + 0x19du) = G32(self_ + 0x406u);
              cm_fn_00417c10((u32 *)(self_ + 0x1a1u), (u32 *)PB(0x384),
                             (u16 *)(self_ + 0x40au));
              G32(self_ + 0x1a9u) = G32(self_ + 0x412u);
              G16(self_ + 0x1adu) = G16(self_ + 0x416u);
              G16(self_ + 0x1adu) = G16(G32(self_ + 0x502u) + 0x40u);
            }
          }
        }
      }
      if (M8(0x42b) != 0) {
        G8(rec + 0x13u) = (u8)(G8(rec + 0x13u) + M8(0x42b));
      }
      if (M8(0x42a) != 0) {
        G8(rec + 0x14u) = (u8)(G8(rec + 0x14u) + M8(0x42a));
      }
      if ((s8)M8(0x42a) < (s8)M8(0x42b)) {
        G8(rec + 0x15u) = (u8)(G8(rec + 0x15u) + 1u);
      } else if ((s8)M8(0x42b) < (s8)M8(0x42a)) {
        G8(rec + 0x16u) = (u8)(G8(rec + 0x16u) + 1u);
      }
      if (G8(rows2 + 0x2cu) != 0 || G8(rows2 + 0x2du) != 0) {
        G8(rec + 0x17u) = (u8)(G8(rec + 0x17u) + G8(rows2 + 0x2cu));
        G8(rec + 0x18u) = (u8)(G8(rec + 0x18u) + G8(rows2 + 0x2du));
        {
          u32 s01 = (u32)G8(rec + 0x18u) * 3u + (u32)G8(rec + 0x17u);
          if (CHK5C() == 0) {
            u8 c = G8(self_ + 0x45au);
            if (c == 0xffu ||
                (s32)s01 > ((s32)(s8)G8(self_ + 0x45bu) * 3 + (s32)(s8)c)) {
              G8(self_ + 0x45au) = G8(rec + 0x17u);
              G8(self_ + 0x45bu) = G8(rec + 0x18u);
              cm_fn_007e4920(M32(0x424), self_ + 0x45cu);
              {
                u32 r0 = M32(0x44c);
                u32 id = G32(r0); /* 004a1111 */
                G16(self_ + 0x478u) = 0;
                G32(self_ + 0x474u) = id;
              }
              if (G8(self_ + 0x1f1u) == 0xffu ||
                  (s32)s01 > ((s32)(s8)G8(self_ + 0x1f2u) * 3 +
                              (s32)(s8)G8(self_ + 0x1f1u))) {
                if ((s16)G16(self_ + 0x500u) > 2 && G8(rec + 0x17u) > 1 &&
                    G8(rec + 0x18u) > 1) {
                  if (G32(self_ + 0x1ffu) != G32(M32(0x424)) ||
                      (u32)G16(self_ + 0x20fu) !=
                          (u32)(s32)(s16)G16(G32(self_ + 0x502u) + 0x40u)) {
                    if ((u32)cm_fn_004b0700(DEREF4) != 0) {
                      G32(self_ + 0x556u) = 1;
                      G32(self_ + 0x562u) = 1;
                    }
                  }
                }
                G8(self_ + 0x1f1u) = G8(self_ + 0x45au);
                G8(self_ + 0x1f2u) = G8(self_ + 0x45bu);
                G32(self_ + 0x1f3u) = G32(self_ + 0x45cu);
                G32(self_ + 0x1f7u) = G32(self_ + 0x460u);
                G32(self_ + 0x1fbu) = G32(self_ + 0x464u);
                G32(self_ + 0x1ffu) = G32(self_ + 0x468u);
                cm_fn_00417c10((u32 *)(self_ + 0x203u), (u32 *)PB(0x374),
                               (u16 *)(self_ + 0x46cu));
                G32(self_ + 0x20bu) = G32(self_ + 0x474u);
                G16(self_ + 0x20fu) = G16(self_ + 0x478u);
                G16(self_ + 0x20fu) = G16(G32(self_ + 0x502u) + 0x40u);
              }
            }
          }
        }
      }
      if (G32(rows2 + 4u) == G32(arg1 + 0x3cu)) {
        G8(rec + 0x19u) = (u8)(G8(rec + 0x19u) + 1u);
        if (CHK5C() == 0) {
          if (G8(self_ + 0x43bu) == 0xffu ||
              (s32)G8(rec + 0x19u) > (s32)(s8)G8(self_ + 0x43bu)) {
            G8(self_ + 0x43bu) = G8(rec + 0x19u);
            cm_fn_007e4920(M32(0x424), self_ + 0x43cu);
            {
              u32 r0 = M32(0x44c);
              u32 id = G32(r0); /* 004a12a7 */
              G16(self_ + 0x458u) = 0;
              G32(self_ + 0x454u) = id;
            }
            if (G8(self_ + 0x1d2u) == 0xffu ||
                (s32)G8(rec + 0x19u) > (s32)(s8)G8(self_ + 0x1d2u)) {
              if ((s16)G16(self_ + 0x500u) > 2) {
                if (G32(self_ + 0x1dfu) != G32(M32(0x424)) ||
                    (u32)G16(self_ + 0x1efu) !=
                        (u32)(s32)(s16)G16(G32(self_ + 0x502u) + 0x40u)) {
                  if ((u32)cm_fn_004b0700(DEREF4) != 0) {
                    G32(self_ + 0x552u) = 1;
                    G32(self_ + 0x562u) = 1;
                  }
                }
              }
              G8(self_ + 0x1d2u) = G8(self_ + 0x43bu);
              G32(self_ + 0x1d3u) = G32(self_ + 0x43cu);
              G32(self_ + 0x1d7u) = G32(self_ + 0x440u);
              G32(self_ + 0x1dbu) = G32(self_ + 0x444u);
              G32(self_ + 0x1dfu) = G32(self_ + 0x448u);
              cm_fn_00417c10((u32 *)(self_ + 0x1e3u), (u32 *)PB(0x364),
                             (u16 *)(self_ + 0x44cu));
              G32(self_ + 0x1ebu) = G32(self_ + 0x454u);
              G16(self_ + 0x1efu) = G16(self_ + 0x458u);
              G16(self_ + 0x1efu) = G16(G32(self_ + 0x502u) + 0x40u);
            }
          }
        }
      }
      G16(rec + 0x1au) = (u16)(G16(rec + 0x1au) + (u16)G8(rows2 + 0x35u));
      if (G8(arg1 + 0x4cu) == 0) {
        G16(rec + 0x1du) = (u16)(G16(rec + 0x1du) + (u16)G8(rows2 + 0x3eu));
        G16(rec + 0x1fu) = (u16)(G16(rec + 0x1fu) + (u16)G8(rows2 + 0x3fu));
        G16(rec + 0x21u) = (u16)(G16(rec + 0x21u) + (u16)G8(rows2 + 0x42u));
        G16(rec + 0x23u) = (u16)(G16(rec + 0x23u) + (u16)G8(rows2 + 0x28u));
        G16(rec + 0x25u) = (u16)(G16(rec + 0x25u) + (u16)G8(rows2 + 0x25u));
        G16(rec + 0x27u) = (u16)(G16(rec + 0x27u) + (u16)G8(rows2 + 0x26u));
        if (i < 11) {
          if ((s8)G8(rows2 + 0x34u) < 0) {
            if ((s8)G8(rows2 + 0x30u) < 0) {
              G32(rec + 0x29u) = G32(rec + 0x29u) + (u32)(s32)(s16)G16(arg1 + 0x4du);
              goto L145b;
            }
            G32(rec + 0x29u) = G32(rec + 0x29u) + (u32)(s32)(s8)G8(rows2 + 0x30u);
          } else {
            G32(rec + 0x29u) = G32(rec + 0x29u) + (u32)(s32)(s8)G8(rows2 + 0x34u);
          }
        } else if (G8(rows2 + 0x32u) != 0xffu) {
          s32 d;
          if ((s8)G8(rows2 + 0x34u) < 0) {
            if (G8(rows2 + 0x30u) > 0x7fu) {
              d = (s32)(s16)G16(arg1 + 0x4du);
            } else {
              d = (s32)(s8)G8(rows2 + 0x30u);
            }
          } else {
            d = (s32)(s8)G8(rows2 + 0x34u);
          }
          d = d - (s32)(s8)G8(rows2 + 0x32u);
          G32(rec + 0x29u) = G32(rec + 0x29u) + (u32)d;
        }
      }

L145b:
      if (CHK5C() == 0) {
        u32 entp = M32(0x424);
        if (G32(self_ + 0x4a2u) != 0xffffffffu) {
          u32 *rp = cm_fn_0054cc30((void *)(self_ + 0x49au), (u32 *)PB(0x354),
                                   G32(self_ + 0x47au), G32(self_ + 0x47eu));
          a0 = rp[0];
          b0 = rp[1];
          M32(0x3e8) = b0;
          *(volatile u8 *)&seh.state = 1; /* 004a14b3 */
          M32(0x3ec) = a0;
          {
            u32 *r2 = cm_fn_0054cc30((void *)0x00ae2c90u, (u32 *)PB(0x344),
                                     G32(entp + 0x10u), G32(entp + 0x14u));
            u32 c0 = r2[0];
            if ((u16)(a0 >> 16) > (u16)(c0 >> 16)) {
              v = 1;
            } else if ((u16)(a0 >> 16) < (u16)(c0 >> 16)) {
              v = 0;
            } else {
              v = ((s16)a0 > (s16)c0) ? 1u : 0u;
            }
          }
          *(volatile u8 *)&seh.state = 0; /* 004a1500 */
          if (v == 0) goto L16c7;
        }
        {
          u32 ebp = entp + 0x10u;
          cm_fn_00417c10((u32 *)(self_ + 0x47au), (u32 *)PB(0x334), (u16 *)ebp);
          cm_fn_007e4920(entp, self_ + 0x482u);
          cm_fn_00417c10((u32 *)(self_ + 0x49au), (u32 *)PB(0x324), (u16 *)PB(0x444));
          G32(self_ + 0x4a2u) = M32(0x43c);
          G16(self_ + 0x4a6u) = M16(0x438);
          G16(self_ + 0x4a8u) = M16(0x436);
          G8(self_ + 0x4aau) = M8(0x434);
          G32(self_ + 0x4abu) = M32(0x433);
          G32(self_ + 0x4afu) = M32(0x42f);
          G8(self_ + 0x4b3u) = M8(0x42b);
          G8(self_ + 0x4b4u) = M8(0x42a);
          G32(self_ + 0x4b5u) = M32(0x429);
          if (G32(self_ + 0x239u) != 0xffffffffu) {
            u32 *rp = cm_fn_0054cc30((void *)(self_ + 0x231u), (u32 *)PB(0x314),
                                     G32(self_ + 0x211u), G32(self_ + 0x215u));
            a0 = rp[0];
            b0 = rp[1];
            M32(0x400) = b0;
            {
              u32 hi = G32(ebp + 4u);
              u32 lo = G32(ebp);
              *(volatile u8 *)&seh.state = 2; /* 004a15e4 */
              M32(0x404) = a0;
              u32 *r2 = cm_fn_0054cc30((void *)0x00ae2c90u, (u32 *)PB(0x304),
                                       lo, hi);
              u32 c0 = r2[0];
              if ((u16)(a0 >> 16) > (u16)(c0 >> 16)) {
                v = 1;
              } else if ((u16)(c0 >> 16) > (u16)(a0 >> 16)) {
                v = 0;
              } else {
                v = ((s16)a0 > (s16)c0) ? 1u : 0u;
              }
            }
            *(volatile u8 *)&seh.state = 0; /* 004a161c */
            if (v == 0) goto L16c7;
          }
          if ((s16)G16(self_ + 0x500u) > 2) {
            u32 *r2 = cm_fn_0054cc30((void *)0x00ae2c90u, (u32 *)PB(0x2f4),
                                     G32(ebp), G32(ebp + 4u));
            if (G16((u32)r2 + 2u) < 0x14u && G32(self_ + 0x225u) != G32(M32(0x424))) {
              G32(self_ + 0x55au) = 1;
              G32(self_ + 0x562u) = 1;
            }
          }
          cm_fn_00417c10((u32 *)(self_ + 0x211u), (u32 *)PB(0x2e4),
                         (u16 *)(self_ + 0x47au));
          G32(self_ + 0x219u) = G32(self_ + 0x482u);
          G32(self_ + 0x21du) = G32(self_ + 0x486u);
          G32(self_ + 0x221u) = G32(self_ + 0x48au);
          G32(self_ + 0x225u) = G32(self_ + 0x48eu);
          cm_fn_00417c10((u32 *)(self_ + 0x229u), (u32 *)PB(0x2d4),
                         (u16 *)(self_ + 0x492u));
          cm_fn_00448110(self_ + 0x231u, self_ + 0x49au);
        }
      }
L16c7:
      if (CHK5C() == 0) {
        u32 entp = M32(0x424);
        if (G32(self_ + 0x4e1u) != 0xffffffffu) {
          u32 *rp = cm_fn_0054cc30((void *)(self_ + 0x4d9u), (u32 *)PB(0x2c4),
                                   G32(self_ + 0x4b9u), G32(self_ + 0x4bdu));
          a0 = rp[0];
          b0 = rp[1];
          M32(0x3f0) = b0;
          *(volatile u8 *)&seh.state = 3; /* 004a1726 */
          {
            u32 hi = G32(entp + 0x14u);
            u32 lo = G32(entp + 0x10u);
            M32(0x3f4) = a0;
            u32 *r2 = cm_fn_0054cc30((void *)0x00ae2c90u, (u32 *)PB(0x2b4),
                                     lo, hi);
            u32 c0 = r2[0];
            if ((u16)(a0 >> 16) < (u16)(c0 >> 16)) {
              v = 1;
            } else if ((u16)(c0 >> 16) < (u16)(a0 >> 16)) {
              v = 0;
            } else {
              v = ((s16)a0 < (s16)c0) ? 1u : 0u;
            }
          }
          *(volatile u8 *)&seh.state = 0; /* 004a176c */
          if (v == 0) goto L_next;
        }
        {
          u32 ebp = entp + 0x10u;
          cm_fn_00417c10((u32 *)(self_ + 0x4b9u), (u32 *)PB(0x2a4), (u16 *)ebp);
          cm_fn_007e4920(entp, self_ + 0x4c1u);
          cm_fn_00417c10((u32 *)(self_ + 0x4d9u), (u32 *)PB(0x294), (u16 *)PB(0x444));
          G32(self_ + 0x4e1u) = M32(0x43c);
          G16(self_ + 0x4e5u) = M16(0x438);
          G16(self_ + 0x4e7u) = M16(0x436);
          G8(self_ + 0x4e9u) = M8(0x434);
          G32(self_ + 0x4eau) = M32(0x433);
          G32(self_ + 0x4eeu) = M32(0x42f);
          G8(self_ + 0x4f2u) = M8(0x42b);
          G8(self_ + 0x4f3u) = M8(0x42a);
          G32(self_ + 0x4f4u) = M32(0x429);
          if (G32(self_ + 0x278u) != 0xffffffffu) {
            u32 *rp = cm_fn_0054cc30((void *)(self_ + 0x270u), (u32 *)PB(0x280),
                                     G32(self_ + 0x250u), G32(self_ + 0x254u));
            a0 = rp[0];
            b0 = rp[1];
            M32(0x3e0) = b0;
            {
              u32 hi = G32(ebp + 4u);
              u32 lo = G32(ebp);
              *(volatile u8 *)&seh.state = 4; /* 004a1850 */
              M32(0x3e4) = a0;
              u32 *r2 = cm_fn_0054cc30((void *)0x00ae2c90u, (u32 *)PB(0x270),
                                       lo, hi);
              u32 c0 = r2[0];
              if ((u16)(a0 >> 16) < (u16)(c0 >> 16)) {
                v = 1;
              } else if ((u16)(c0 >> 16) < (u16)(a0 >> 16)) {
                v = 0;
              } else {
                v = ((s16)a0 < (s16)c0) ? 1u : 0u;
              }
            }
            *(volatile u8 *)&seh.state = 0; /* 004a188b */
            if (v == 0) goto L_next;
          }
          if ((s16)G16(self_ + 0x500u) > 2) {
            u32 *r2 = cm_fn_0054cc30((void *)0x00ae2c90u, (u32 *)PB(0x264),
                                     G32(ebp), G32(ebp + 4u));
            if (G16((u32)r2 + 2u) > 0x23u && G32(self_ + 0x264u) != G32(M32(0x424))) {
              G32(self_ + 0x55eu) = 1;
              G32(self_ + 0x562u) = 1;
            }
          }
          cm_fn_00417c10((u32 *)(self_ + 0x250u), (u32 *)PB(0x254),
                         (u16 *)(self_ + 0x4b9u));
          G32(self_ + 0x258u) = G32(self_ + 0x4c1u);
          G32(self_ + 0x25cu) = G32(self_ + 0x4c5u);
          G32(self_ + 0x260u) = G32(self_ + 0x4c9u);
          G32(self_ + 0x264u) = G32(self_ + 0x4cdu);
          cm_fn_00417c10((u32 *)(self_ + 0x268u), (u32 *)PB(0x244),
                         (u16 *)(self_ + 0x4d1u));
          cm_fn_00417c10((u32 *)(self_ + 0x270u), (u32 *)PB(0x234),
                         (u16 *)(self_ + 0x4d9u));
          G32(self_ + 0x278u) = G32(self_ + 0x4e1u);
          G16(self_ + 0x27cu) = G16(self_ + 0x4e5u);
          G16(self_ + 0x27eu) = G16(self_ + 0x4e7u);
          G8(self_ + 0x280u) = G8(self_ + 0x4e9u);
          G32(self_ + 0x281u) = G32(self_ + 0x4eau);
          G32(self_ + 0x285u) = G32(self_ + 0x4eeu);
          G8(self_ + 0x289u) = G8(self_ + 0x4f2u);
          G8(self_ + 0x28au) = G8(self_ + 0x4f3u);
          G32(self_ + 0x28bu) = G32(self_ + 0x4f4u);
        }
      }
    }
  }
L_next:
  M32(0x448) = M32(0x448) + 1u;
  if ((s32)M32(0x448) < 0x14) goto L_ent_loop;

  sideidx = sideidx + 1u;
  if (sideidx < 2) goto loop_side;

  G32(self_ + 0x2fau) = 0xffffffffu;
  G32(self_ + 0x418u) = 0xbf800000u;
  if (G16(arg1 + 0x130u) != 0) {
    cm_fn_004a25f0(self_, arg1);
  }
  CM_FS(void *, 0) = seh.prev;
  return 1;
}
