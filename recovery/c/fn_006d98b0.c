/* Translation of preferred-image VA 0x006d98b0 (match_events.cpp), image base
   0x00400000, pinned CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1c627a22c445d14d489bd9bff86.
   thiscall (ECX read before written at 006d98c6 `mov ebp, ecx`), 2 stack
   arguments, `ret 8` at 006d9aab/006d9b73/006d9c3d.

   Stack frame: `sub esp, 0x218` at 006d98b0 makes L the frame base; the four
   saved registers then sit below L.  Original local offsets from L, recovered
   from every `lea [esp+disp]` at the listing's push state:
     L+0x00  third 4-byte header field (read #3 of each header; also incremented
             and re-written by the record trailer at 006d9c01..006d9c23)
     L+0x04  header field #1 (version)
     L+0x08  header field saved for the record (006d9969 / 006d9a29)
     L+0x0c  events2 temp path ([esp+0x1c])
     L+0x10  second header's field #2 (read at 006d9a07)
     L+0x14  first header's field #2 (read at 006d9947)
     L+0x18  256-byte text buffer ([esp+0x28] facts)
     L+0x118 256-byte text buffer ([esp+0x128] facts)
   The two 256-byte buffers are adjacent in the original frame, so they are
   kept as one scratch allocation with explicit offsets.

   L+0x00 is deliberately NOT initialized: the original never initializes it
   (006d98b0..006d98d4 only clear L+0x04 and set L+0x14/L+0x10 to -1), the
   header reads at 006d993f/006d994e/006d995d and 006d99ff/006d9a0e/006d9a1d
   ignore 00945bfe's result (which can return short/EOF before its destination
   store), and 006d99aa/006d9aae read L+0x00 before the only initializing store
   at 006d9b84.

   UNRESOLVED FIDELITY DEFECT (confirmed against the preferred-image VAs
   above in the pinned binary): the original count occupies entry ESP-0x218;
   this automatic L allocation is not bound to those incoming bytes. Volatile
   accesses preserve neither their identity nor their value. If a count read
   leaves bytes unwritten with version 1, comparisons at 006d99aa/006d9aae
   and the increment/write-back at 006d9c01..006d9c23 can therefore diverge.
   Leaving L uninitialized is NOT a faithful implementation of that case.
   A fix needs an entry-state binding that supplies/preserves the original
   bytes before the rebuilt prologue overwrites them. The supplied plain-C
   interface/header provides no such binding; changing initialization, using
   a guessed compiled-frame offset, or adding a helper without a binding would
   invent behaviour. Executable code is retained pending that boundary work;
   a passing current gate must not be treated as resolution of this defect. */

#include "cmre.h"

#define G_009a7f28 CM_GLOBAL(u8, 0x009a7f28)
#define G_009c7848 CM_GLOBAL(u8, 0x009c7848)
#define G_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define G_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define G_00b6b7c0 CM_GLOBAL(volatile u32, 0x00b6b7c0)

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 cm_fn_0090bde0(u8 *arg1, u8 *arg2);
/* Legacy abi boundary with a (void) recovered signature.  The dwords pushed
   before `call 0x90d130` at 006d9a5f/006d9a64 (block 1), 006d9a5f/006d9a64
   (block 2) and 006d9b32/006d9b37/006d9b38 (block 3) therefore belong to the
   following variable-argument call 0x944c9f, and its 0x9870e8 format
   ("%s %s %s %d" shape: three string pointers plus an int) takes exactly the
   three pending dwords after the returned string. */
extern u32 cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern u32 CM_CDECL cm_fn_00945a09(u32 stream);
extern volatile u32 *CM_CDECL cm_fn_00945b00(u32 path, volatile s8 *mode_text);
extern u32 CM_CDECL cm_fn_00945bfe(u32 buffer, u32 count, u32 element_size, u32 state_address);
extern u32 CM_CDECL cm_fn_00945ce6(u32 destination, u32 size, u32 count, u32 stream);
extern s32 CM_CDECL cm_fn_00946a22(volatile u8 *file, u32 second, u32 mode);
extern s32 CM_CDECL cm_fn_00946aae(volatile u8 *file);

u8 CM_THISCALL cm_fn_006d98b0(int param_1, u32 *param_2, char param_3)
{
  u8 L[0x218];
  u32 stream;
  u32 r;
  u32 name_a;
  u32 name_b;
  u32 name_sel;
  u32 counter;
  u32 base;
  u32 save_210;
  u8 save_p3;
  s32 f204;
  s32 f208;

  *(volatile s32 *)(L + 0x04) = 0;
  *(volatile s32 *)(L + 0x14) = -1;
  *(volatile s32 *)(L + 0x10) = -1;
  if (param_2 == (u32 *)0) {
    return 0;
  }
  if (param_3 == (char)0) {
    name_a = 0x00a15a60;
    name_b = 0x00a15a48;
  }
  else {
    name_a = 0x00a15a8c;
    name_b = 0x00a15a78;
  }
  *(volatile u32 *)(L + 0x0c) = name_b;
  stream = (u32)cm_fn_00945b00(cm_fn_0090bde0((u8 *)0x00987da0, (u8 *)name_a),
                                (volatile s8 *)&G_009c7848);
  if (stream != 0) {
    cm_fn_00946a22((volatile u8 *)stream, 0, 0);
    cm_fn_00945bfe((u32)(L + 0x04), 4, 1, stream);
    cm_fn_00945bfe((u32)(L + 0x14), 4, 1, stream);
    cm_fn_00945bfe((u32)(L + 0x00), 4, 1, stream);
    *(volatile u32 *)(L + 0x08) = *(volatile u32 *)(L + 0x14);
    if (*(volatile s32 *)(L + 0x04) != 1) {
      cm_fn_00944cff(0x00a159b0, 0, 0, (void *)(L + 0x118), (void *)(L + 0x18));
      cm_fn_00944c9f((u32)&G_00ae24d0, 0x009870e8, cm_fn_0090d130(),
                     (u32)(L + 0x118), (u32)(L + 0x18), 0xb5c);
      cm_fn_005e8290((const char *)0x009870e0, (const char *)&G_00ae24d0, 0);
      G_00b67a34 = 0;
      cm_fn_00945a09(stream);
      return 0;
    }
    if (*(volatile s32 *)(L + 0x00) < 0x9c4) {
      goto tail;
    }
    cm_fn_00945a09(stream);
  }
  stream = (u32)cm_fn_00945b00(cm_fn_0090bde0((u8 *)0x00987da0, (u8 *)name_b),
                                (volatile s8 *)&G_009c7848);
  if (stream != 0) {
    cm_fn_00946a22((volatile u8 *)stream, 0, 0);
    cm_fn_00945bfe((u32)(L + 0x04), 4, 1, stream);
    cm_fn_00945bfe((u32)(L + 0x10), 4, 1, stream);
    cm_fn_00945bfe((u32)(L + 0x00), 4, 1, stream);
    *(volatile u32 *)(L + 0x08) = *(volatile u32 *)(L + 0x10);
    if (*(volatile s32 *)(L + 0x04) != 1) {
      cm_fn_00944cff(0x00a159b0, 0, 0, (void *)(L + 0x18), (void *)(L + 0x118));
      cm_fn_00944c9f((u32)&G_00ae24d0, 0x009870e8, cm_fn_0090d130(),
                     (u32)(L + 0x18), (u32)(L + 0x118), 0xb7b);
      cm_fn_005e8290((const char *)0x009870e0, (const char *)&G_00ae24d0, 0);
      G_00b67a34 = 0;
      cm_fn_00945a09(stream);
      return 0;
    }
    if (*(volatile s32 *)(L + 0x00) < 0x9c4) {
      goto tail;
    }
    cm_fn_00945a09(stream);
  }
  f204 = *(volatile s32 *)(L + 0x14);
  if (f204 == -1) {
    name_sel = name_a;
  }
  else {
    f208 = *(volatile s32 *)(L + 0x10);
    if (f208 == -1) {
      name_sel = name_b;
    }
    else if (f204 > f208) {
      name_sel = name_b;
    }
    else {
      name_sel = name_a;
    }
  }
  stream = (u32)cm_fn_00945b00(cm_fn_0090bde0((u8 *)0x00987da0, (u8 *)name_sel),
                                (volatile s8 *)&G_009a7f28);
  if (stream == 0) {
    cm_fn_00944cff(0x00a159b0, 0, 0, (void *)(L + 0x18), (void *)(L + 0x118));
    cm_fn_00944c9f((u32)&G_00ae24d0, 0x009870e8, cm_fn_0090d130(),
                   (u32)(L + 0x18), (u32)(L + 0x118), 0xba4);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)&G_00ae24d0, 0);
    G_00b67a34 = 0;
    return 0;
  }
  *(volatile s32 *)(L + 0x04) = 1;
  *(volatile u32 *)(L + 0x00) = 0;
  base = G_00b6b7c0;
  counter = *(volatile u32 *)(base + 0x18) + 1;
  *(volatile u32 *)(L + 0x08) = counter;
  *(volatile u32 *)(base + 0x18) = counter;
  cm_fn_00945ce6((u32)(L + 0x04), 4, 1, stream);
  cm_fn_00945ce6((u32)(L + 0x08), 4, 1, stream);
  cm_fn_00945ce6((u32)(L + 0x00), 4, 1, stream);

tail:
  cm_fn_00946a22((volatile u8 *)stream, 0, 2);
  r = (u32)cm_fn_00946aae((volatile u8 *)stream);
  save_210 = *(volatile u32 *)(L + 0x08);
  save_p3 = (u8)param_3;
  param_2[0] = r;
  param_2[1] = (u32)(s32)*(volatile s16 *)((u32)param_1 + 6);
  param_2[2] = save_210;
  *(u8 *)((u32)param_2 + 0x0c) = save_p3;
  counter = cm_fn_00945ce6((u32)param_1 + 0x720, 0xe,
                           (u32)(s32)*(volatile s16 *)((u32)param_1 + 6), stream);
  *(volatile u32 *)(L + 0x00) = *(volatile u32 *)(L + 0x00) + 1;
  cm_fn_00946a22((volatile u8 *)stream, 8, 0);
  cm_fn_00945ce6((u32)(L + 0x00), 4, 1, stream);
  cm_fn_00945a09(stream);
  return (u8)(counter != 0);
}
