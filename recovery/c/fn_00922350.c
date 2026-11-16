/* fn_00922350 — comp\worldcup\wc_south_american_league.cpp
   Reconstructed from the original instruction listing at preferred-image VAs
   (image base 0x00400000) against the pinned CM0102 3.9.68 executable
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1c627a22c445d14d489bd9bff86.

   ABI evidence from the listing:
   - 0x0092236d `mov esi, ecx` -> the receiver (this) is ECX; 0x009226b9
     `ret 0x20` -> eight caller-pushed dwords (arg homes at entry-ESP+4..+0x24).
   - 0x0092245e `lea ecx, [esp+0x1c]` (= frame base L+0) before
     0x009224a9 `call 0x00954f70` with pushes 0x9224a9: -1, arg4, arg3, arg2.
   - 0x009224bd/0x009224dc `lea ecx, [esp+0xc]` (= L+0) before
     0x009224d1/0x009224e0 `call 0x00954a3f0` (one ECX argument, no stack args).
   - 0x00922561 `mov ecx, [0x00dd7ef4]` before 0x00922573 `call 0x0085c260`
     with pushes -1, L+0x08, 0 (single caller `add esp` absent -> callee pops).
   - 0x009225b4 `mov ecx, [0x00ae2a58]` before 0x009225c1 `call 0x005aa680`
     with pushes 1, L+0x08.
   - 0x009225e8/0x009225fa receiver for 0x006827d0 (push 0 shared by both
     call sites at 0x009225e2).
   - 0x00922613..0x0092261d `call 0x00944cff` takes five caller-cleaned
     arguments (0x00922622 `add esp, 0x14`), likewise 0x00922652;
     0x00922668..0x0092266e push three arguments for `call 0x0090d130`
     (draft said `(void)`: contradicts 0x00922630/0x00922635/0x00922636).
   - 0x00922695 `add esp, 0x24` cleans 9 dwords for 0x0090d130 / 0x00944c9f /
     0x005e8290 -> all three are caller-cleanup.
   - 0x009224d6 `cmp ax, 5` / 0x009224e5 `cmp ax, 6` compare only the low word
     of 0x00954a3f0's result, so the C compares `(u16)`.

   UNRESOLVED FIDELITY HOLD: original stack residue is not mapped by this C.
   Evidence below uses preferred-image VAs and the pinned binary hash above.
   If S is original entry ESP, the prologue at 0x00922356..0x0092236f
   leaves ESP=S-0x47c and L=S-0x470. Thus 0x009223c1 reads S-0x41b,
   not an arbitrary uninitialised C allocation. F.loc has no established
   correspondence to those pre-existing bytes.
   The masks at 0x009223ce/0x009223de/0x009223f1 retain original bits
   12..15; 0x0092244c stores them at L+0x55. The record at L+0x08
   crosses the call boundary at 0x00922573 (address formed/pushed at
   0x00922567/0x0092256c), with this word at record offset 0x4d.
   No non-observation or intervening overwrite is established here.
   A passing gate does not discharge this hold. Resolving it needs an
   authorized original-stack input boundary or original-instruction proof
   that the retained bits cannot affect observable behavior. Zero fill,
   volatile residue, or matching a compiler frame layout is not a fix.

   Original uninitialised accesses (not proven preserved by this candidate):
   - 0x009223c1 `mov ecx, dword ptr [esp + 0x61]` reads L+0x55 before any store
     to that dword; bits 4..15 survive through `and ecx, 0xfff0`, `or eax, ecx`,
     `and eax, 0xff0f`, `or eax, ecx (dl<<4)` and are stored at
     0x009223ec/0x0092244c as the packed flag word at L+0x55 (bits 8..15 come
     from the unwritten byte L+0x56, bits 12..15 from the unwritten nibble).
   - 0x009225de `mov al, byte ptr [esp + 0x56]` reads L+0x44 (ESP is
     entry-ESP-0x47c there: the two pushes of the virtual call were popped by
     the callee, as proved by 0x009225ca/0x009226a2 sharing the epilogue, which
     reloads the saved SEH link from [esp+0x470] = L+0x464).
   Both currently read the C scratch frame, not proven original stack residue.
   Executable code is retained pending that missing boundary/evidence. */

#include "cmre.h"

struct seh_frame {
  u32 volatile prev;
  u32 volatile handler;
  u32 volatile state;
};

/* One scratch allocation: locals L+0x00..L+0x463, then the linked SEH record
   at L+0x464..L+0x46f (the three prologue pushes), as in the listing. */
struct frame {
  u8 loc[0x464];
  struct seh_frame seh;
};

static u8 ld8(const void *p)
{
  u8 v;
  __builtin_memcpy(&v, p, 1);
  return v;
}

static u16 ld16(const void *p)
{
  u16 v;
  __builtin_memcpy(&v, p, 2);
  return v;
}

static u32 ld32(const void *p)
{
  u32 v;
  __builtin_memcpy(&v, p, 4);
  return v;
}

static void st8(void *p, u8 v)
{
  __builtin_memcpy(p, &v, 1);
}

static void st16(void *p, u16 v)
{
  __builtin_memcpy(p, &v, 2);
}

static void st32(void *p, u32 v)
{
  __builtin_memcpy(p, &v, 4);
}

/* Globals at their preferred-image VAs. */
#define G_00ae23a8 CM_GLOBAL(u32, 0x00ae23a8)
#define G_00ae2a58 CM_GLOBAL(u32, 0x00ae2a58)
#define G_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define G_00dd6ec8 CM_GLOBAL(u32, 0x00dd6ec8)
#define G_00dd6ecc CM_GLOBAL(u32, 0x00dd6ecc)
#define G_00dd7ef4 CM_GLOBAL(u32, 0x00dd7ef4)

/* Address constants used as arguments. */
#define A_00ad4ce0 0x00ad4ce0u
#define A_00ae24d0 0x00ae24d0u
#define A_009870e0 0x009870e0u
#define A_009870e8 0x009870e8u

extern u32 CM_CDECL cm_fn_0053b3d0(u32 nation);
extern short * CM_THISCALL cm_fn_00549f70(short *param_1, short param_2,
                                          char param_3, u16 param_4,
                                          u32 param_5);
extern int CM_FASTCALL cm_fn_0054a3f0(short *param_1);
extern u8 CM_THISCALL cm_fn_005aa680(int param_1, int param_2, int param_3);
extern void cm_fn_005e8290(const char *param_1, const char *param_2,
                           int param_3);
extern u32 CM_THISCALL cm_fn_006827d0(int param_1, int param_2);
extern u32 CM_THISCALL cm_fn_0085c260(u32 self, volatile u32 flags_arg,
                                      volatile u32 object_arg,
                                      volatile u32 explicit_arg);
/* 0x0090d130 is `mov eax, 0x00ad4394 / ret` (6 bytes): it takes no argument
   and pops nothing.  The pushes at 0x00922630/0x00922635/0x00922636 therefore
   belong to the following 0x00944c9f call, whose callee reads them as its
   fourth/sixth arguments through the va_list ([ebp+0x10]); the single
   `add esp, 0x24` at 0x00922695 cleans all nine dwords. */
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                   void *arg5);

void CM_THISCALL
cm_fn_00922350(void *self_arg, s8 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5,
               s8 arg6, u32 arg7, s8 arg8)
{
  const u8 *const self = (const u8 *)self_arg;
  struct frame F;
  u8 *const L = F.loc;
  u32 saved_fs;
  u32 obj, vtbl, t32;
  u32 off, res1, res2, d0, d1, fpslot;
  u16 pack, w0, w2, hi, r16;
  s32 idx;
  char *msg;

  /* 0x00922350 SEH prologue: state, handler, previous link, then link. */
  saved_fs = CM_FS(u32, 0);
  F.seh.state = 0xffffffffu;
  F.seh.handler = 0x00965c2bu;
  F.seh.prev = saved_fs;
  CM_FS(u32, 0) = (u32)(&F.seh.prev);

  /* 0x00922370..0x0092237f */
  idx = (s32)arg1;
  if (idx > (s32)ld32(self + 0x30))
    goto err_index;

  /* 0x00922385..0x00922393 */
  if (arg1 != (s8)-1) {
    if (ld32((const u8 *)(u32)ld32(self + 0x0c) + (u32)idx * 4u) == 0)
      goto err_index;
  }

  /* 0x00922399..0x009223b8 */
  if ((u16)arg4 < 0x07d0u)
    goto err_range;
  if ((u16)arg4 > 0x07d1u)
    goto err_range;
  if (arg1 != (s8)-1)
    goto err_range;

  /* 0x009223be..0x009223ec: packed flag word, low bits from [esi+0x49]/[esi+0x4a],
      upper bits require original residue at L+0x55 (unresolved hold above). */
  t32 = ld32(L + 0x55);                                 /* 0x009223c1 */
  pack = (u16)((u32)ld8(self + 0x49) & 0x0fu);          /* 0x009223be/c8/ca */
  pack = (u16)((u32)pack | (t32 & 0x0000fff0u));        /* 0x009223ce/d7 */
  pack = (u16)((u32)pack & 0x0000ff0fu);                /* 0x009223de */
  pack = (u16)((u32)pack |
               ((((u32)ld8(self + 0x4a)) & 0x0fu) << 4)); /* 0x009223d4/e4/e7 */
  st16(L + 0x55, pack);                                 /* 0x009223ec */
  pack = (u16)((u32)pack & 0x0000f0ffu);                /* 0x009223f1 */

  /* 0x009223e9..0x00922458: record fields. */
  obj = ld32(self + 4);                                 /* 0x009223e9 */
  vtbl = ld32((const u8 *)(u32)obj);                    /* 0x009223f5 */
  st32(L + 0x1c, obj);                                  /* 0x009223f7 */
  st32(L + 0x08, vtbl);                                 /* 0x009223ff */
  st16(L + 0x34, ld16(self + 0x40));                    /* 0x00922407 */
  st16(L + 0x3a, ld16(self + 0x3a));                    /* 0x00922412 */
  st8(L + 0x42, ld8(self + 0xc4));                      /* 0x0092241e */
  st16(L + 0x3e, ld16(self + 0xdb));                    /* 0x00922429 */
  st16(L + 0x40, ld16(self + 0xab));                    /* 0x00922435 */
  st8(L + 0x4b, (u8)arg6);                              /* 0x00922441 */
  st16(L + 0x55, pack);                                 /* 0x0092244c */
  st8(L + 0x4c, (u8)arg8);                              /* 0x00922458 */

  /* 0x00922462..0x009224a4 */
  st16(L + 0x38, 0);                                    /* 0x00922462 */
  st8(L + 0x4d, 0xff);                                  /* 0x00922469 */
  st8(L + 0x4e, 0xff);                                  /* 0x0092246d */
  st8(L + 0x4f, 0xff);                                  /* 0x00922471 */
  st8(L + 0x50, 0xff);                                  /* 0x00922475 */
  st8(L + 0x51, 0xff);                                  /* 0x00922479 */
  st8(L + 0x52, 0xff);                                  /* 0x0092247d */
  st8(L + 0x53, 0xff);                                  /* 0x00922481 */
  st8(L + 0x54, 0xff);                                  /* 0x00922485 */
  st32(L + 0x10, 0xffffffffu);                          /* 0x00922489 */
  st8(L + 0x4a, 0xff);                                  /* 0x0092248d */
  st8(L + 0x45, 0);                                     /* 0x00922491 */
  st8(L + 0x46, 0);                                     /* 0x00922496 */
  st8(L + 0x43, 0xff);                                  /* 0x0092249b */
  st8(L + 0x48, 1);                                     /* 0x0092249f */
  st8(L + 0x49, 1);                                     /* 0x009224a4 */

  /* 0x009224a9: this = L+0, pushes -1, arg4, arg3, arg2. */
  cm_fn_00549f70((short *)L, (short)arg2, (char)arg3, (u16)arg4,
                 0xffffffffu);

  /* 0x009224ae..0x009224d1 */
  w0 = ld16(L + 0x00);                                  /* 0x009224ae */
  w2 = ld16(L + 0x02);                                  /* 0x009224b3 */
  st16(L + 0x32, w0);                                   /* 0x009224b8 */
  F.seh.state = 0;                                      /* 0x009224c1 */
  st16(L + 0x30, w2);                                   /* 0x009224cc */
  r16 = (u16)cm_fn_0054a3f0((short *)L);                /* 0x009224d1 */
  if (r16 != 5u) {
    r16 = (u16)cm_fn_0054a3f0((short *)L);              /* 0x009224e0 */
    st8(L + 0x47, 2);                                   /* 0x009224e9 */
    if (r16 != 6u)
      goto after47;
  }
  st8(L + 0x47, 1);                                     /* 0x009224f0 */
after47:
  /* 0x009224f5..0x00922510 */
  st16(L + 0x2e, ld16(L + 0x32));                       /* 0x009224f5/ff */
  st16(L + 0x2c, ld16(L + 0x30));                       /* 0x009224fa/50b */
  st16(L + 0x3c, 0);                                    /* 0x00922510 */

  /* 0x00922517..0x0092252d: record pointer for arg5 (index*145 elements). */
  off = ((arg5 * 9u) << 4) + arg5;
  res1 = cm_fn_0053b3d0(G_00ae23a8 + off * 2u);         /* 0x00922528 */
  st32(L + 0x24, res1);                                 /* 0x0092252d */

  /* 0x00922531..0x00922552: same for arg7. */
  off = ((arg7 * 9u) << 4) + arg7;
  res2 = cm_fn_0053b3d0(G_00ae23a8 + off * 2u);         /* 0x00922549 */
  res1 = ld32(L + 0x24);                                /* 0x0092254e */
  st32(L + 0x28, res2);                                 /* 0x00922552 */

  /* 0x00922559..0x00922573 */
  st32(L + 0x14, ld32((const u8 *)(u32)res1));          /* 0x00922559/55b */
  st32(L + 0x18, ld32((const u8 *)(u32)res2));          /* 0x0092255f/56f */
  cm_fn_0085c260(G_00dd7ef4, 0, (u32)(L + 0x08), 0xffffffffu);

  /* 0x00922578..0x009225b0 */
  d0 = G_00dd6ec8;                                      /* 0x00922578 */
  d1 = G_00dd6ecc;                                      /* 0x0092257d */
  w2 = ld16(L + 0x02);                                  /* 0x00922583 */
  st32(L + 0x5c, d0);                                   /* 0x00922588 */
  st32(L + 0x60, d1);                                   /* 0x0092258c */
  hi = ld16(L + 0x5e);                                  /* 0x00922590 */
  if (hi < w2)
    goto set_ff;                                        /* 0x00922598 */
  if (hi > w2)
    goto no_ff;                                         /* 0x0092259a */
  if (!((s16)(u16)d0 < (s16)ld16(L + 0x00)))            /* 0x0092259e/5a3 */
    goto no_ff;                                         /* 0x009225aa */
set_ff:
  st8(L + 0x4b, 0xff);                                  /* 0x009225ac */
  st8(L + 0x4c, 0xff);                                  /* 0x009225b0 */
no_ff:
  /* 0x009225b4..0x009225c1 */
  cm_fn_005aa680((int)G_00ae2a58, (int)(u32)(L + 0x08), 1);
  if (ld8(L + 0x4b) == 0xff)                            /* 0x009225c6 */
    goto done;

  /* 0x009225d0..0x009225db: virtual slot +4 of *self, this = self,
     arguments (&L+0x08, 0).  Target, arity and cleanup are whatever the
     original vtable holds; the call is emitted exactly as the listing's
     `call dword ptr [edx+4]` after `mov ecx, esi`. */
  vtbl = ld32(self);
  fpslot = ld32((const u8 *)(u32)vtbl + 4);
  ((void (CM_THISCALL *)(void *, void *, u32))fpslot)(self_arg,
                                                      (void *)(L + 0x08), 0);

  /* 0x009225de..0x009225fd */
  idx = (s32)(s8)ld8(L + 0x44);
  if (idx == -1) {
    cm_fn_006827d0((int)(u32)self_arg, 0);              /* 0x009225ea */
  } else {
    u32 recv;

    recv = ld32((const u8 *)(u32)ld32(self + 0x0c) + (u32)idx * 4u);
    cm_fn_006827d0((int)recv, 0);                       /* 0x009225fd */
  }

done:
  /* 0x009226a2/0x009226ac: restore the saved SEH predecessor. */
  saved_fs = F.seh.prev;
  CM_FS(u32, 0) = saved_fs;
  return;

err_index:
  /* 0x00922639..0x0092266e, then 0x00922674..0x0092267f. */
  cm_fn_00944cff(A_00ad4ce0, 0, 0, (void *)(L + 0x364),
                 (void *)(L + 0x164));                  /* 0x00922652 */
  msg = cm_fn_0090d130();                               /* 0x0092266f */
  cm_fn_00944c9f(A_00ae24d0, A_009870e8, (u32)msg,      /* 0x0092267f */
                 (u32)(L + 0x364), (u32)(L + 0x164), 0x03c7u);
  goto report;

err_range:
  /* 0x00922607..0x0092261d, then 0x00922625..0x00922630. */
  cm_fn_00944cff(A_00ad4ce0, 0, 0, (void *)(L + 0x264),
                 (void *)(L + 0x64));                   /* 0x0092261d */
  msg = cm_fn_0090d130();                               /* 0x0092266f */
  cm_fn_00944c9f(A_00ae24d0, A_009870e8, (u32)msg,      /* 0x0092267f */
                 (u32)(L + 0x264), (u32)(L + 0x64), 0x03ceu);
  goto report;

report:
  /* 0x00922684..0x00922698 */
  cm_fn_005e8290((const char *)(u32)A_009870e0,
                 (const char *)(u32)A_00ae24d0, 0);     /* 0x00922690 */
  G_00b67a34 = 0;                                       /* 0x00922698 */
  goto done;
}
