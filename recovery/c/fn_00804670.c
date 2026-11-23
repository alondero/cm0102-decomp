/* fn_00804670 (VA 0x00804670): thiscall, this = ECX, one stack argument, ret 4.
   Same record-scan shape as fn_00804590; only the final callee differs.
   Callees are explicit original boundaries:
   - cm_fn_007b7690: ECX = element pointer puVar4, one pushed pointer.
   - cm_fn_007b1260: ECX = element pointer p (saved at frame slot E-4), one pushed
     argument param_2. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007b1260(int *param_1,int param_2);
extern void CM_THISCALL cm_fn_007b7690(u32 *param_1,u32 *param_2);

u32 CM_THISCALL cm_fn_00804670(u8 *param_1,u32 param_2)
{
  s8 cVar1;
  u8 *p;
  u32 *puVar3;
  u8 *puVar4;
  u32 iVar2;
  s32 local_8;

  cVar1 = 0;
  do {
    p = param_1 + (s32)cVar1 * 0x2b0 + 1;
    if (*(volatile u8 *)(p + 0xa) == 1) {
      *(volatile u8 *)(param_1 + 0x1b0d) = *(volatile u8 *)param_1;
      puVar3 = (u32 *)(param_1 + 0x35ee);
      puVar4 = param_1 + 0x1b0e;
      iVar2 = (u32)param_1 - (u32)(param_1 + 0x1b0d);
      local_8 = 10;
      do {
        cm_fn_007b7690((u32 *)puVar4,(u32 *)((u32)puVar4 + iVar2));
        *(volatile u32 *)puVar3 = *(volatile u32 *)((u32)puVar3 + iVar2);
        puVar4 = puVar4 + 0x2b0;
        puVar3 = puVar3 + 1;
        local_8 = local_8 - 1;
      } while (local_8 != 0);
      *(volatile u32 *)(param_1 + 0x3616) = *(volatile u32 *)(param_1 + 0x1b09);
      *(volatile u32 *)(param_1 + 0x361a) = 1;
      cm_fn_007b1260((int *)p,(int)param_2);
      *(volatile u32 *)(param_1 + cVar1 * 4 + 0x1ae1) = 1;
      *(volatile u32 *)(param_1 + 0x1b09) = 1;
      *(volatile u32 *)(param_1 + 0x3634) = 0;
      return 1;
    }
    cVar1 = (s8)(cVar1 + 1);
  } while (cVar1 < 10);
  return 0;
}
