/* Scratch remains uninitialized as in the original; callers may observe
   differences if a callee reads bytes it did not initialize. */
/* fn_00682f70 (league.cpp): thiscall, one stack argument (ret 4).
   Preferred-image VA 0x00682f70, image base 0x00400000. */
#include "cmre.h"

extern void cm_fn_005e8290(const char *param_1, u32 param_2, int param_3);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

int CM_THISCALL cm_fn_00682f70(int param_1, int param_2)
{
  /* One scratch area L (esp after sub esp, 0x200): [L+0] is the buffer passed
     as arg5 of 944cff, [L+0x100] is the buffer passed as arg4. Both are
     uninitialized in the listing. */
  u8 scratch[0x200];
  u32 sum;
  s32 count;

  sum = 0;
  if (*(volatile s16 *)((u32)param_1 + 0x3e) == 0) {
    cm_fn_00944cff(0x009f15f4u, 0, 0, scratch + 0x100, scratch);
    /* 0x214, [L+0] and [L+0x100] are pushed before the (void) call to
       0x90d130; they stay on the stack as varargs of 0x944c9f. */
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, cm_fn_0090d130(),
                   scratch + 0x100, scratch, 0x214u);
    cm_fn_005e8290((const char *)0x009870e0, 0x00ae24d0u, 0);
    CM_GLOBAL(u32, 0x00b67a34) = 0;
  }

  count = (s32)*(volatile s16 *)((u32)param_1 + 0x3e);
  if (count > 0) {
    /* The register-only loop sums the first count multiples modulo 2^32. */
    u32 n = (u32)count;
    sum = (n * (n + 1u) / 2u) * (u32)param_2;
  }
  return (int)sum;
}
