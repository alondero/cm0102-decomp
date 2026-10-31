#include "cmre.h"

/* Original thiscall: ECX (self) is stored at entry; ret 8 pops two stack arguments.
   Callees below are cdecl unless marked; thiscall ones take self first. */
extern int CM_THISCALL cm_fn_004a4850(u32 self, u32 param_1);
extern u32 CM_THISCALL cm_fn_004aceb0(u32 self, u32 param_1);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 cm_fn_0090d130(u8 *arg1, u8 *arg2, u32 arg3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

void CM_THISCALL cm_fn_004a2540(u32 self, u32 param_1, u8 param_2)
{
  /* One 0x200-byte frame block: buffer at offset 0x000 (listing esp+8 after the
     sub esp, 0x200 base) and buffer at offset 0x100 (listing esp+0x108). */
  u8 scratch[0x200];
  u32 uVar1;
  int iVar2;

  if (param_1 == 0) {
    cm_fn_00944cff(0x0099ca94, param_1, param_1, scratch + 0x100, scratch);
    /* The listing never cleans the three pushes made for 0x0090d130 (its ret 0 leaves
       them); they stay on the stack as the variadic arguments of the next call. */
    uVar1 = cm_fn_0090d130(scratch + 0x100, scratch, 0x680);
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, uVar1, scratch + 0x100, scratch, 0x680);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, (int)param_1);
    CM_GLOBAL(u32, 0x00b67a34) = param_1;
    return;
  }
  iVar2 = cm_fn_004a4850(self, param_1);
  if (iVar2 == 0) {
    iVar2 = (int)cm_fn_004aceb0(self, param_1);
  }
  *(u8 *)((u32)iVar2 + 0x72) = param_2;
}
