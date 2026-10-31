#include "cmre.h"

/* Callees. 0x0090d130 is `mov eax, imm; ret` and ignores its three cdecl
   arguments, so the caller cleans them up (the original defers that to one
   `add esp, 0x24` after the 0x005e8290 call). The 0x00945934 call never returns
   in the original: its chain ends in ExitProcess. */
extern u32 CM_STDCALL cm_fn_0058f110(u32 unused, u32 destination);
extern void cm_fn_005a8f60(void);
extern void cm_fn_005cc7d0(void);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char *CM_CDECL cm_fn_0090d130(void *arg1, void *arg2, u32 arg3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern void CM_CDECL cm_fn_00945934(u32 param_1);

/* Original frame: `sub esp, 0x200`, with [esp] at scratch[0] and [esp+0x100] at
   scratch[0x100]. One array keeps the two related buffers adjacent. */
u32 CM_THISCALL
cm_fn_00631f90(u32 self, u32 param_2)
{
  u8 scratch[0x200];
  u32 uVar1;
  u32 s;
  u32 edx;
  u32 this_v;
  u32 iVar2;

  if (param_2 == 0) {
    cm_fn_00944cff((u32)&CM_GLOBAL(char, 0x009e7f28), 0, 0, scratch + 0x100, scratch);
    uVar1 = (u32)cm_fn_0090d130(scratch + 0x100, scratch, 0x1479);
    /* The three 0x0090d130 argument dwords stay on the stack (its cdecl caller
       cleanup is deferred), so 0x00944c9f also receives them as varargs. */
    cm_fn_00944c9f((u32)&CM_GLOBAL(u8, 0x00ae24d0), (u32)&CM_GLOBAL(char, 0x009870e8), uVar1,
                   scratch + 0x100, scratch, 0x1479);
    cm_fn_005e8290(&CM_GLOBAL(char, 0x009870e0), (const char *)&CM_GLOBAL(u8, 0x00ae24d0), 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    cm_fn_005cc7d0();
    cm_fn_005a8f60();
    cm_fn_00945934(0xffffffffu);
  }
  s = *(volatile u32 *)param_2;
  edx = s * 31u;
  this_v = *(volatile u32 *)self;
  (void)*(volatile u32 *)(self + 0xcu);
  iVar2 = this_v + edx;
  return cm_fn_0058f110(iVar2 + 0x14u, iVar2 + 0x18u);
}
