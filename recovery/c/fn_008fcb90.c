/* fn_008fcb90 — VA 0x008fcb90 (preferred image base 0x00400000), 544 bytes,
   module comp\cups\tur_cup.cpp. Pinned binary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   ABI evidence from the original instructions:
   - 0x008fcb98 `mov esi, ecx` reads ECX before it is written; no stack argument
     is read anywhere in the body and 0x008fcdaf `ret` pops 0 bytes ->
     __thiscall with the receiver in ECX.
   - 0x008fcb90 `sub esp, 0x348` establishes the scratch frame L (L = entry_esp
     - 0x348).  Registers pushed before the first local store: ebx (0x008fcb96),
     esi (0x008fcb97), edi (0x008fcb9a), 0x180 (0x008fcb9d) = 16 bytes, so
     `mov dword ptr [esp+0x10], ebx` (0x008fcba4) is L+0x0 and
     `mov dword ptr [esp+0x14], ebx` (0x008fcba8) is L+0x4.
   - `push ebp` at 0x008fcc3e makes esp L-0x10 for the rest of the body; the
     remaining frame offsets below are derived from that constant esp.

   Frame layout used verbatim from the listing (bytes from L):
      L+0x000  dword  first counter   (0x008fcba4 store, 0x008fcc5e word read,
                                       0x008fcc7c dword store, 0x008fcd14
                                       word read for the helper count)
      L+0x004  dword  second counter  (0x008fcba8 store, 0x008fcc90 dword read,
                                       0x008fcca5 dword store, 0x008fcd62
                                       word read)
      L+0x008  dword[30] first array  (0x008fcc78 `[esp+ebp*4+0x18]` with
                                       esp == L-0x10)
      L+0x080  dword[20] second array (0x008fcc9e `[esp+eax*4+0x90]`)
      L+0x148  byte[256]              (0x008fcbc4 `lea eax,[esp+0x154]`)
      L+0x248  byte[256]              (0x008fcbcb `lea ecx,[esp+0x254]`)
      L+0x348  end of frame

   The first copy loop at 0x008fcd39..0x008fcd60 runs exactly 26 times (edx =
   0x1a at 0x008fcd34, `dec edx; jne`) and walks L+0x6c downwards to L+0x08,
   i.e. first-array slots 25..0, regardless of how many entries the scan
   actually wrote.

   UNRESOLVED FIDELITY BOUNDARY (same pinned binary and preferred-image VAs):
   0x008fcc4b/0x008fcc4d permit skipping the scan entirely; 0x008fcba4 and
   0x008fcba8 initialize only the counters.  Thus 0x008fcd3f can export the
   preexisting DWORDs at entry_esp-0x340 through entry_esp-0x2dc.
   An uninitialized C automatic object does NOT bind those bytes to that
   original entry stack range, and its indeterminate values are not a faithful
   substitute.  The object below preserves adjacency only, not entry contents.
   Resolving this requires an authorized entry-stack capture/binding boundary
   before a compiler prologue can overwrite those bytes.  The supplied plain-C
   interface has no such input.  Zeroing, clamping the copy, guessing a frame
   offset, or manufacturing a stack argument would change original behaviour.
   No behavioural repair is claimed here, even if the current gate passes. */

#include "cmre.h"

#define G_009cf8d8 CM_GLOBAL(u32, 0x009cf8d8)
#define G_009cf8dc CM_GLOBAL(u32, 0x009cf8dc)
#define G_009cf8e0 CM_GLOBAL(u32, 0x009cf8e0)
#define G_00ae2350 CM_GLOBAL(u32, 0x00ae2350)
#define G_00ae235c CM_GLOBAL(u32, 0x00ae235c)
#define G_00ae23b4 CM_GLOBAL(u32, 0x00ae23b4)
#define G_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define G_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

extern void CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2,
                                   int param_3);
/* 0x0090d130 is reached by `call` with no stack argument of its own: the three
   pushes at 0x008fcbf1..0x008fcbf7 belong to the next call (0x00944c9f, whose
   single `add esp, 0x24` at 0x008fcc1d covers all nine pushes of the block). */
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                   void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
/* Original call site 0x008fcd26 pushes exactly four stack arguments in
   right-to-left order: 0x004c0a40 (0x008fcd19), 4 (0x008fcd1e), the count
   (0x008fcd24) and the array base (0x008fcd25), then `add esp, 0x10` at
   0x008fcd2b.  The callee is therefore cdecl with four stack arguments. */
extern void CM_CDECL cm_fn_00945333(void *base_arg, u32 count_arg,
                                    u32 width_arg, void *compare_arg);

struct frame_008fcb90 {
  u32 counter1;      /* L+0x000 */
  u32 counter2;      /* L+0x004 */
  u32 array1[30];    /* L+0x008: 0x1e slots, guard at 0x008fcc5e */
  u32 array2[20];    /* L+0x080: 0x14 slots, guard at 0x008fcc94 */
  u8 gap[0x78];      /* L+0x0d0 .. L+0x147, never touched */
  u8 buf2[0x100];    /* L+0x148 */
  u8 buf1[0x100];    /* L+0x248 */
};                   /* ends at L+0x348 */

u32 CM_THISCALL cm_fn_008fcb90(u32 self)
{
  struct frame_008fcb90 L; /* unresolved entry-stack binding; see above */
  u32 idx;
  u32 off;
  u32 h;
  u32 n;
  u32 p;
  u32 rec;
  s32 i;
  u32 class3n;
  unsigned long long alloc;

  L.counter1 = 0;                 /* 0x008fcba4 */
  L.counter2 = 0;                 /* 0x008fcba8 */
  *(u16 *)(self + 0x3e) = 0x40;   /* 0x008fcbac, word store */

  alloc = cm_fn_00944e46(0x180);  /* 0x008fcbb2 */
  *(u32 *)(self + 0xa7) = (u32)alloc; /* 0x008fcbbc, dword store */
  if ((u32)alloc == 0) {          /* 0x008fcbba cmp eax,ebx / jne 0x8fcc33 */
    cm_fn_00944cff(0x00aa180cu, 0, 0, (void *)L.buf1, (void *)L.buf2);
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)cm_fn_0090d130(),
                  (u32)L.buf1, (u32)L.buf2, 0x142);
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    G_00b67a34 = 0;               /* 0x008fcc20 */
    return 0xffffffffu;           /* 0x008fcc26 or eax,0xffffffff */
  }

  class3n = 0;                    /* edi, 0x008fcba2 */
  i = 0;                          /* edx, 0x008fcc47 */
  rec = G_00ae23b4;               /* ecx, 0x008fcc38 */
  /* 0x008fcc45 add eax,eax / 0x008fcc49 sub ebp,eax / test / jle */
  if ((s32)(G_00ae235c - (G_00ae2350 + G_00ae2350)) > 0) {
    do {
      p = *(u32 *)(rec + 0x57);   /* 0x008fcc53 */
      if (p != 0) {               /* 0x008fcc56 */
        if ((s16)(u16)L.counter1 < 0x1e && *(u32 *)p == G_009cf8e0) {
          /* 0x008fcc70..0x008fcc7c: index is the sign-extended low word,
             slot store is a dword, the counter write is a dword. */
          idx = (u32)(s32)(s16)(u16)L.counter1;
          L.array1[idx] = rec;
          L.counter1 = L.counter1 + 1;
        } else if (*(u32 *)p == G_009cf8dc &&
                   (s16)(u16)L.counter2 < 0x14) {
          /* 0x008fcc90..0x008fcca5 */
          idx = (u32)(s32)(s16)(u16)L.counter2;
          L.array2[idx] = rec;
          L.counter2 = L.counter2 + 1;
        } else if (*(u32 *)p == G_009cf8d8) {
          /* 0x008fccbb..0x008fccf1: three reloads of this+0xa7 and of the
             word at this+0x3e, one per record field store. */
          class3n = class3n + 1;
          n = (u32)(s32)(s16)*(u16 *)(self + 0x3e);
          off = ((n - class3n) + (n - class3n) * 2) * 2;
          h = *(u32 *)(self + 0xa7);
          *(u32 *)(h + off) = rec;
          n = (u32)(s32)(s16)*(u16 *)(self + 0x3e);
          off = ((n - class3n) + (n - class3n) * 2) * 2;
          h = *(u32 *)(self + 0xa7);
          *(u8 *)(h + off + 5) = 0;
          n = (u32)(s32)(s16)*(u16 *)(self + 0x3e);
          off = ((n - class3n) + (n - class3n) * 2) * 2;
          h = *(u32 *)(self + 0xa7);
          *(u8 *)(h + off + 4) = 1;
        }
      }
      /* 0x008fccf6..0x008fcd0e: bound recomputed from the globals each pass */
      i = i + 1;
      rec = rec + 0x245;
    } while (i < (s32)(G_00ae235c - (G_00ae2350 + G_00ae2350)));
  }

  /* 0x008fcd14..0x008fcd2b.  Four stack arguments, pushed right to left:
     compare 0x004c0a40, width 4, count = sign-extended word at L+0x0, base =
     lea [esp+0x20] = L+0x008.  (EAX holds 2*DAT_00ae2350 and EDX the same base
     at 0x008fcd26; neither is a stack argument of the original call.) */
  cm_fn_00945333((void *)&L.array1[0], (u32)(s32)(s16)(u16)L.counter1, 4,
                 (void *)0x004c0a40u);

  /* 0x008fcd2e..0x008fcd60: eax = 0, ecx = L+0x6c, edx = 0x1a; read [ecx],
     ecx -= 4, 26 dword slots written at heap offsets 0,6,..,150. */
  idx = 25;                       /* first read, L+0x6c = slot 25 */
  off = 0;
  n = 0x1a;
  do {
    u32 v = L.array1[idx];
    idx = idx - 1;
    h = *(u32 *)(self + 0xa7);
    *(u32 *)(h + off) = v;
    h = *(u32 *)(self + 0xa7);
    *(u8 *)(h + off + 5) = 0;
    h = *(u32 *)(self + 0xa7);
    *(u8 *)(h + off + 4) = 1;
    off = off + 6;
    n = n - 1;
  } while (n != 0);

  /* 0x008fcd62..0x008fcd9e: count is the sign-extended low word of the second
     counter; -1 skips.  First read is L+0x80+4*(count-1), heap offset 0x9c. */
  idx = (u32)(s32)(s16)(u16)L.counter2;
  if (idx != 0) {
    n = idx;
    idx = idx - 1;
    off = 0x9c;
    do {
      u32 v = L.array2[idx];
      idx = idx - 1;
      h = *(u32 *)(self + 0xa7);
      *(u32 *)(h + off) = v;
      h = *(u32 *)(self + 0xa7);
      *(u8 *)(h + off + 5) = 0;
      h = *(u32 *)(self + 0xa7);
      *(u8 *)(h + off + 4) = 1;
      off = off + 6;
      n = n - 1;
    } while (n != 0);
  }

  return 1;                       /* 0x008fcda3 mov eax,1 */
}
