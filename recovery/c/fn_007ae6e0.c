/* fn_007ae6e0 (player_search.cpp): thiscall with one byte stack argument (ret 4).
   ECX is the object pointer and is read only on the early-return path.
   Preferred-image VA 0x007ae6e0, image base 0x00400000. */
#include "cmre.h"

#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

u32 CM_THISCALL cm_fn_007ae6e0(int param_1, u8 param_2)
{
  /* One scratch area L (esp after sub esp, 0x200): [L+0] is the buffer passed
     as arg5 of 944cff, [L+0x100] is the buffer passed as arg4. Both are
     uninitialized in the listing. */
  u8 scratch[0x200];

  if (param_2 != 1 && param_2 != 2 && param_2 != 3 && param_2 != 4) {
    cm_fn_00944cff(0x00a701b0u, 0, 0, scratch + 0x100, scratch);
    /* 0xea, [L+0] and [L+0x100] are pushed before the (void) call to
       0x90d130; they stay on the stack as varargs of 0x944c9f. */
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, cm_fn_0090d130(),
                   scratch + 0x100, scratch, 0xeau);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0u, 0);
    DAT_00b67a34 = 0;
    return 0;
  }
  *(u8 *)(param_1 + 0xa) = param_2;
  return 1;
}
