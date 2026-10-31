#include "cmre.h"

/* Original thiscall (ECX is stored at entry; ret 8 pops two stack arguments). The
   result is returned on the x87 stack (ST0). Callees are cdecl unless they take
   self first (thiscall). */
extern long double CM_THISCALL cm_fn_004a3250(u32 self, u32 param_1, u32 param_2);
extern u32 CM_THISCALL cm_fn_004a4a30(u32 self, u32 param_1);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 cm_fn_0090d130(u8 *arg1, u8 *arg2, u32 arg3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

long double CM_THISCALL cm_fn_004a31a0(u32 self, u32 param_1, u32 param_2)
{
  /* One 0x200-byte frame block: buffer at offset 0x000 and buffer at offset 0x100. */
  u8 scratch[0x200];
  u32 uVar1;
  u32 iVar2;

  if (param_1 == 0) {
    cm_fn_00944cff(0x0099ca94, 0, 0, scratch + 0x100, scratch);
    /* The three pushes made for 0x0090d130 stay on the stack (ret 0 cleans nothing)
       and are the variadic arguments of the next call. */
    uVar1 = cm_fn_0090d130(scratch + 0x100, scratch, 0x9b5);
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, uVar1, scratch + 0x100, scratch, 0x9b5);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    CM_GLOBAL(u32, 0x00b67a34) = 0;
  } else {
    iVar2 = cm_fn_004a4a30(self, param_1);
    if (iVar2 != 0) {
      return cm_fn_004a3250(self, iVar2, param_2);
    }
  }
  return (long double)CM_GLOBAL(float, 0x00968970);
}
