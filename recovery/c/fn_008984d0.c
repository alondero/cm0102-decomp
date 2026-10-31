/* fn_008984d0 (VA 0x008984d0), module tactics.cpp.
   Listing: thiscall (ECX read before written: `mov esi, ecx`), `ret 0xc` (three stack args).
   Scratch buffer `scratch` is the original 0x200-byte frame area at L (esp after `sub esp, 0x200`):
   scratch[0x000..0x0ff] = [L+0]   (lea eax, [esp+4] before the first call),
   scratch[0x100..0x1ff] = [L+0x100] (lea ecx, [esp+0x104]). */
#include "cmre.h"

#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
/* `mov ecx, esi` precedes the call; the callee does not read ECX but the
   receiver is kept as the original's ECX value. It pops its two stack args (ret 8). */
extern int CM_THISCALL cm_fn_008982b0(u32 self, int *param_1, int param_2);
/* Listing: `mov eax, 0xad4394; ret` (no arguments). */
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

u32 CM_THISCALL cm_fn_008984d0(u32 self, u32 arg1, u32 *arg2, u32 arg3)
{
  u8 scratch[0x200];
  u32 result;
  s32 index;
  u32 *dst;
  u32 i;

  if (arg1 == 0) {
    cm_fn_00944cff(0x00a8b908u, 0, 0, scratch + 0x100, scratch);
    result = (u32)cm_fn_0090d130();
    /* The three pushes made before the no-argument call 0x90d130 are the trailing
       varargs of the following call 0x944c9f (after its format and the result). */
    cm_fn_00944c9f((u32)(&DAT_00ae24d0), 0x009870e8u, result,
                   (u32)(scratch + 0x100), (u32)scratch, 0x79du);
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)(&DAT_00ae24d0), 0);
    DAT_00b67a34 = 0;
    return 0;
  }

  index = cm_fn_008982b0(self, (int *)arg1, (int)arg3);
  if (index == -1) {
    return 0;
  }
  /* Signed comparison: `jl` in the listing. */
  if (*(s32 *)(self + 0x605) <= index) {
    return 0;
  }
  dst = (u32 *)(*(u32 *)(self + 0x601) + (u32)index * 0x91u);
  for (i = 0; i < 0x24; i++) {
    dst[i] = arg2[i];
  }
  *((u8 *)dst + 0x90) = *((u8 *)arg2 + 0x90);
  return 1;
}
