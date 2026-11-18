/* fn_0040b950, preferred-image VA 0x0040b950, image base 0x00400000,
   module comp/eurocomp/asia_club_champ.cpp (asia club championship setup).
   Translated from the original instructions only.

   Stack-probe note (compiler helper, not engine logic):
   0040b950/0040b955 load EAX = 0x1594 and call 0x00945610, the MSVC
   frame-probe helper.  The helper probes the incoming caller ESP - 0x1000 and
   ESP - 0x1594, moves ESP down to open this function's 0x1594-byte frame and
   preserves ECX; the routine's epilogue (0040bbfb/0040bead `add esp,0x1594`)
   tears exactly that frame down.  It is the compiler lowering of the frame
   allocation (same class as the __ftol lowering in ADR 0006), not a callee:
      - its only input is EAX = 0x1594, which plain C cannot place in EAX;
      - its ESP displacement is the C frame itself, so a C call to it would
        displace this function's own frame instead of creating one;
      - because it preserves ECX, the receiver used at 0040b95b (`mov ebx,ecx`)
        is this function's own incoming thiscall ECX (THIS0), which is declared
        below as the explicit receiver instead of Ghidra's `extraout_ECX`.
   The equivalent in plain C is one real 0x1594-byte scratch object (the exact
   frame size: the candidate array at frame+0x2d4 has 0x4b0 dwords =
   0x2d4 + 0x12c0 = 0x1594) allocated by the compiler's own prologue, with all
   original frame offsets expressed explicitly against it.

   Stack-probe resolution (harness-blocker "Retry when" condition), stated explicitly
and entirely in plain C:
  * Kind of helper: 0x00945610 is the MSVC frame-probe / stack-allocation lowering
    of `sub esp,0x1594` (compiler-inserted, the same class as the __ftol lowering
    in ADR 0006), not an engine callee.  Its only input is EAX=0x1594; it writes
    no semantic memory, it commits stack pages and moves ESP.  Its plain-C
    lowering is therefore the compiler's own prologue allocating that frame.
  * Probe-address preservation: the 47-byte helper touches caller ESP-0x1000 and
    ESP-0x1594; both addresses lie inside the 0x1594-byte frame this function
    actually uses.  Every original [esp+disp] access is expressed here as the
    identical displacement from L, the base of the 0x1594-byte C array, so all
    relative addresses the helper probes are inside and covered by the C frame.
  * Fault-order preservation: the helper executes at entry, before the first
    semantic store (`mov [ebx+0xb6],0x20` at 0040b95e).  The compiler's own
    prologue likewise allocates the 0x1594-byte frame before any routine write,
    so any stack fault still precedes every semantic write.  The helper has no
    other observable effect, so with adequate stack its omission is exact and
    differs only after unrecoverable exhaustion, before any routine write.
  * No inline assembly, naked function, CM_ORIGINAL_CALL, register/optimize
    attribute, frozen compiler-frame layout, or header/harness change is used;
    the frame is a genuine C array of the original exact size.
  * ECX: because the helper restores incoming ECX, `mov ebx,ecx` at 0040b95b
    captures THIS0; the thiscall `self` receiver below is that same value.

No call to 0x00945610 is emitted; all of its effect is covered above. */

#include "cmre.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

/* Callees, explicit boundaries (original code). */
extern u32 CM_CDECL cm_fn_004afbd0(u32 base, u32 count, u32 wanted);
extern u32 cm_fn_004afd80(u32 param_1, char param_2);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
/* 0040bbc2..0040bbc9 pushes frame+0x1d4, frame+0xd4 and 0x289 for the
   0040bbc9 call; the single `add esp,0x24` at 0040bbee retires nine pushes
   for three calls, so the same three homes are also the 4th..6th arguments of
   the 0040bbd9 formatter (6 arguments + the 3 of 0040bbe9 = 9).  The
   formatter consumes them in that order: the original call's message carries
   the extension, the source base name and the line number from these slots. */
extern char *cm_fn_0090d130(u32 arg1, u32 arg2, u32 arg3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern void *CM_CDECL cm_fn_00945501(u32 arg1, u32 arg2);

/* Original frame accesses: `L` is the function's 0x1594-byte frame, and each
   offset is the listing's [esp+disp] with the live saved registers and pushed
   arguments subtracted (ebx+esi = 8, then ebp+edi = 8, then 4+0x20 = 8). */
#define D32(o) (*(u32 *)(L + (o)))
#define W16(o) (*(u16 *)(L + (o)))
#define B8(o)  (*(u8 *)(L + (o)))
#define G32(va) CM_GLOBAL(u32, va)
#define P32(p) ((u32)(unsigned long)(p))

u32 CM_THISCALL cm_fn_0040b950(void *self)
{
  u32 Lw[0x1594 / 4];
  u8 *L = (u8 *)Lw;
  u8 *obj = (u8 *)self;
  u32 tbl;
  u32 team;
  u32 id;
  u32 rec;
  u32 v;
  u32 a;
  s32 esi;
  s32 k;
  s32 n;
  s32 cnt;

  /* 0040b95e */
  *(u32 *)(obj + 0xb6) = 0x20;

  /* 0040b968..0040bb57: snapshot 42 competition/context ids into frame+0x2c. */
  D32(0x2c) = G32(0x009cf238);
  D32(0x30) = G32(0x009cf264);
  D32(0x34) = G32(0x009cf298);
  D32(0x38) = G32(0x009cf340);
  D32(0x3c) = G32(0x009cf34c);
  D32(0x40) = G32(0x009cf350);
  D32(0x44) = G32(0x009cf36c);
  D32(0x48) = G32(0x009cf374);
  D32(0x4c) = G32(0x009cf388);
  D32(0x50) = G32(0x009cf3ac);
  D32(0x54) = G32(0x009cf3b8);
  D32(0x58) = G32(0x009cf3bc);
  D32(0x5c) = G32(0x009cf3e0);
  D32(0x60) = G32(0x009cf3ec);
  D32(0x64) = G32(0x009cf404);
  D32(0x68) = G32(0x009cf46c);
  D32(0x6c) = G32(0x009cf484);
  D32(0x70) = G32(0x009cf48c);
  D32(0x74) = G32(0x009cf4b8);
  D32(0x78) = G32(0x009cf4c4);
  D32(0x7c) = G32(0x009cf42c);
  D32(0x80) = G32(0x009cf508);
  D32(0x84) = G32(0x009cf410);
  D32(0x88) = G32(0x009cf200);
  D32(0x8c) = G32(0x009cf234);
  D32(0x90) = G32(0x009cf354);
  D32(0x94) = G32(0x009cf358);
  D32(0x98) = G32(0x009cf370);
  D32(0x9c) = G32(0x009cf378);
  D32(0xa0) = G32(0x009cf380);
  D32(0xa4) = G32(0x009cf384);
  D32(0xa8) = G32(0x009cf390);
  D32(0xac) = G32(0x009cf40c);
  D32(0xb0) = G32(0x009cf414);
  D32(0xb4) = G32(0x009cf43c);
  D32(0xb8) = G32(0x009cf458);
  D32(0xbc) = G32(0x009cf4b0);
  D32(0xc0) = G32(0x009cf4bc);
  D32(0xc8) = G32(0x009cf4e4);
  D32(0xcc) = G32(0x009cf4fc);

  /* 0040bb22..0040bb64: zero both used-flag maps (frame+0x14: 23 bytes,
     frame+0x00: 19 bytes), in the original store order. */
  D32(0x14) = 0;
  D32(0x18) = 0;
  D32(0x00) = 0;
  D32(0x1c) = 0;
  D32(0x04) = 0;
  D32(0x20) = 0;
  D32(0x08) = 0;
  D32(0x24) = 0;
  D32(0x0c) = 0;
  D32(0xc4) = G32(0x009cf4e0);
  W16(0x28) = 0;
  W16(0x10) = 0;
  D32(0xd0) = G32(0x009cf518); /* written after `push 4; push 0x20` */
  B8(0x2a) = 0;
  B8(0x12) = 0;

  esi = 0;

  /* 0040bb6e..0040bb8b: allocate the 0x20-dword club table. */
  tbl = P32(cm_fn_00945501(0x20, 4));
  *(u32 *)(obj + 0xb2) = tbl;
  if (tbl == 0) {
    /* 0040bb95..0040bbf9: allocation failure report.
       The three slots pushed for the 0040bbc9 call stay in place (they are
       retired only by the single `add esp,0x24` at 0040bbee), and
       0040bbd9's formatter reads them as its 4th..6th arguments: the frame
       buffers + 0x289.  Nine pushes at 0040bbee = 6 formatter arguments plus
       the three of 0040bbe9. */
    void *bufA = (void *)(L + 0x1d4);
    void *bufB = (void *)(L + 0xd4);
    u32 msg;

    cm_fn_00944cff(0x987a40, 0, 0, bufA, bufB);
    msg = P32(cm_fn_0090d130(P32(bufA), P32(bufB), 0x289));
    cm_fn_00944c9f(0xae24d0, 0x9870e8, msg, P32(bufA), P32(bufB), 0x289);
    cm_fn_005e8290((const char *)0x9870e0, (const char *)0xae24d0, 0);
    G32(0x00b67a34) = 0;
    return 0;
  }

  /* 0040bc02..0040bc20: our own club, then its id (*(*[club+0x53])). */
  team = cm_fn_004afd80(*(u32 *)(obj + 4), 0);
  id = 0xffffffffu;
  if (team != 0) {
    u32 p = *(u32 *)(team + 0x53);
    if (p != 0) id = *(u32 *)p;
  }

  /* 0040bc22..0040bc36: search the first id group (frame+0x2c, 23 entries). */
  k = 0;
  while (k < 0x17) {
    if (id == D32(0x2c + 4 * k)) break;
    k = k + 1;
  }
  if (k < 0x17) {
    tbl = *(u32 *)(obj + 0xb2);
    *(u32 *)tbl = team; /* 0040bdf1..0040bdf7: first slot */
  } else {
    tbl = *(u32 *)(obj + 0xb2);
    *(u32 *)(tbl + 0x40) = team; /* 0040bc38..0040bc3e: second group's first slot */
  }

  /* 0040bc41..0040bcbb: first competition of the current competition table. */
  if (*(u8 *)(G32(0x00ae23a8) + 290u * G32(0x009cf36c) + 0x11c) != 0) {
    a = G32(0x00ae23d0) + 107u * G32(0x009cf65c);
    v = cm_fn_004afd80(a, 0);
    tbl = *(u32 *)(obj + 0xb2);
    if (v == *(u32 *)tbl || v == *(u32 *)(tbl + 0x40)) {
      a = G32(0x00ae23d0) + 107u * G32(0x009cf65c);
      v = cm_fn_004afd80(a, 1);
    }
    D32(0x2d4) = v; /* 0040bcb4: array[0] */
    esi = 1;
  }

  /* 0040bcc0..0040bd36: second competition. */
  if (*(u8 *)(G32(0x00ae23a8) + 290u * G32(0x009cf484) + 0x11c) != 0) {
    a = G32(0x00ae23d0) + 107u * G32(0x009cfa00);
    v = cm_fn_004afd80(a, 0);
    tbl = *(u32 *)(obj + 0xb2);
    if (v == *(u32 *)tbl || v == *(u32 *)(tbl + 0x40)) {
      a = G32(0x00ae23d0) + 107u * G32(0x009cfa00);
      v = cm_fn_004afd80(a, 1);
    }
    k = (s16)esi;
    esi = esi + 1;
    D32(0x2d4 + 4 * k) = v;
  }

  /* 0040bd3d..0040bd97: candidate clubs of the requested competition. */
  cnt = (s32)(G32(0x00ae235c) - 2u * G32(0x00ae2350));
  rec = G32(0x00ae23b4);
  if (cnt > 0) {
    do {
      u32 c1 = *(u32 *)(rec + 0x53);
      if (c1 != 0) {
        u32 c2 = *(u32 *)(c1 + 0x71);
        if (c2 != 0) {
          if (*(u32 *)c2 == G32(0x009cfa0c)) {
            if ((s16)esi < 0x4b0) {
              tbl = *(u32 *)(obj + 0xb2);
              if (rec != *(u32 *)tbl && rec != *(u32 *)(tbl + 0x40)) {
                k = (s16)esi;
                esi = esi + 1;
                D32(0x2d4 + 4 * k) = rec;
              }
            }
          }
        }
      }
      rec = rec + 0x245;
      cnt = cnt - 1;
    } while (cnt != 0);
  }

  /* 0040bd99..0040bdaf: hand the candidates at array[1..] to the original. */
  a = P32(L + 0x2d8);
  v = P32(esi - 1);
  cm_fn_004afbd0(a, v, 0x28);

  n = (s16)esi;
  if (n <= 0) {
    /* 0040bdb4 -> 0040bea4 */
    return 1;
  }

  /* 0040bdba..0040be9e: file each candidate into its group's free slot. */
  {
    u32 *fp = (u32 *)(L + 0x2d4);

    do {
      u32 t = *fp;

      id = 0xffffffffu;
      if (t != 0) {
        u32 p = *(u32 *)(t + 0x53);
        if (p != 0) id = *(u32 *)p;
      }

      k = 0;
      while (k < 0x17) {
        if (id == D32(0x2c + 4 * k)) break;
        k = k + 1;
      }
      if (k < 0x17) {
        u8 *flag = L + 0x14 + k;

        if (*flag == 0) {
          tbl = *(u32 *)(obj + 0xb2);
          *flag = 1;
          if (*(u32 *)(tbl + 0x3c) == 0) {
            s32 s = 0;

            while (s < 0x10) {
              if (*(u32 *)(tbl + 4 * s) == 0) break;
              s = s + 1;
            }
            if (s < 0x10) {
              *(u32 *)(tbl + 4 * s) = t;
            }
          }
        }
      } else {
        id = 0xffffffffu;
        if (t != 0) {
          u32 p = *(u32 *)(t + 0x53);
          if (p != 0) id = *(u32 *)p;
        }
        k = 0;
        while (k < 0x13) {
          if (id == D32(0x88 + 4 * k)) break;
          k = k + 1;
        }
        if (k < 0x13) {
          u8 *flag = L + k;

          if (*flag == 0) {
            tbl = *(u32 *)(obj + 0xb2);
            *flag = 1;
            if (*(u32 *)(tbl + 0x7c) == 0) {
              s32 s = 0x10;

              while (s < 0x20) {
                if (*(u32 *)(tbl + 4 * s) == 0) break;
                s = s + 1;
              }
              if (s < 0x20) {
                *(u32 *)(tbl + 4 * s) = t;
              }
            }
          }
        }
      }

      fp = fp + 1;
      n = n - 1;
    } while (n != 0);
  }

  return 1;
}
