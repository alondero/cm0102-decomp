/* Tier-2 C recovery for preferred-image VA 0x00949f9e (cdecl, ret 0).
   Arguments are named by their [ebp+N] stack offset. The two locals are
   named by their [ebp-N] offset. The two "push ecx" instructions store the
   entry ECX into [ebp-4] and [ebp-8]; 0x00945161 overwrites both
   out-parameters before either is read, so the entry ECX is never observed.

   Exit EAX is kept explicit: it is the nonzero result of 0x00945038 when
   that call's result ends the routine, otherwise the loop index [ebp-4].
   The third 0x00945161 argument is [ebp+0x1c] (edi), loaded once.

   0x00949f9e's callee 0x0094a141 is pushed with eleven dwords (add esp,
   0x2c), the last being the constant 1. The Ghidra prototype lists only ten. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_00945038(u32, u32, u32, u32, u32, u32, u32);
extern u32 cm_fn_00945161(u32, u32, u32, u32 *, u32 *);
extern void cm_fn_0094a141(u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32);

u32
cm_fn_00949f9e(u32 arg_08, u32 arg_0c, u32 arg_10, u32 arg_14,
               u32 arg_18, u32 arg_1c, u32 arg_20, u32 arg_24)
{
  u32 l_m4;   /* [ebp-4]: loop index, its address passed to 0x00945161 */
  u32 l_m8;   /* [ebp-8]: loop bound, its address passed to 0x00945161 */
  u32 rec;    /* esi: record table returned by 0x00945161, 0x14 bytes each */
  u32 base;   /* eax after "shl eax,4 / add eax,ecx" in the loop body */
  u32 obj;    /* ecx: pointer loaded from base-0x0c */
  u32 r;

  if (CM_GLOBAL(volatile u32, 0x00de301c) != 0) {
    r = cm_fn_00945038(arg_08, arg_0c, arg_10, arg_14, arg_18, arg_20, arg_24);
    if (r != 0) {
      return r;
    }
  }
  rec = cm_fn_00945161(arg_18, arg_20, arg_1c, &l_m4, &l_m8);
  for (; l_m4 < l_m8; l_m4 = l_m4 + 1, rec = rec + 0x14) {
    if ((s32)arg_1c >= *(volatile s32 *)rec &&
        (s32)arg_1c <= *(volatile s32 *)(rec + 4)) {
      base = *(volatile u32 *)(rec + 0xc);
      base = (base << 4) + *(volatile u32 *)(rec + 0x10);
      obj = *(volatile u32 *)(base - 0xc);
      if (obj == 0 || *(volatile u8 *)(obj + 8) == 0) {
        cm_fn_0094a141(arg_08, arg_0c, arg_10, arg_14, arg_18,
                       base - 0x10, 0, rec, arg_20, arg_24, 1);
      }
    }
  }
  return l_m4;
}
