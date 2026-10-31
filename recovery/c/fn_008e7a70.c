/* fn_008e7a70 (VA 0x008e7a70): thiscall (this = ECX), four stack arguments, ret 0x10.
   Callees are explicit original boundaries:
   - cm_fn_004d0c60: ECX is set to the global at 0x00ae1050 before the call, and five
     dwords are pushed (no caller cleanup; the callee pops 0x14).
   - cm_fn_0068d1b0: fastcall, one argument in ECX, no stack arguments. */
#include "cmre.h"

#define DAT_00ae23bc CM_GLOBAL(int, 0x00ae23bc)

extern u32 CM_THISCALL cm_fn_004d0c60(int *ecx_in,int *param_1,int param_2,char param_3,short param_4,int param_5);
extern u32 CM_FASTCALL cm_fn_0068d1b0(int param_1);

int CM_THISCALL cm_fn_008e7a70(int param_1,int param_2,int param_3,int param_4,int param_5)
{
  u32 uVar1;
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;

  uVar1 = *(volatile u32 *)(param_1 + 0xc);
  iVar4 = *(volatile int *)(param_1 + 0x1b);
  iVar1 = (int)((u32)DAT_00ae23bc + uVar1 * 0x6eu);
  if (param_2 == 0) {
    /* Each field is loaded once (0x008e7a94, 0x008e7a9d, 0x008e7aa6) and the
       loaded value is reused. */
    int f35 = *(volatile int *)(param_1 + 0x35);
    if (0 < f35) {
      iVar4 = (int)((u32)iVar4 + (u32)f35);
    }
    int f3a = *(volatile int *)(param_1 + 0x3a);
    if (0 < f3a) {
      iVar4 = (int)((u32)iVar4 + (u32)f3a);
    }
    char f3f = *(volatile char *)(param_1 + 0x3f);
    if (('\0' < f3f) && (param_5 != 0)) {
      iVar3 = (int)((u32)*(int *)(iVar1 + 0x52) * (u32)(int)f3f);
      iVar4 = (int)((u32)iVar4 + (u32)(iVar3 / 100));
    }
  }
  iVar3 = *(int *)(param_1 + 4);
  if ((iVar3 != 0) && (param_3 == 0)) {
    *(u32 *)(iVar3 + 10) = *(u32 *)(iVar3 + 2);
    piVar2 = (int *)cm_fn_0068d1b0(*(int *)(param_1 + 4));
    if (piVar2 != (int *)0x0) {
      do {
        if (param_4 == 0) {
          u32 uVar2;

          uVar2 = *(u32 *)piVar2;
          iVar3 = *(int *)((u32)DAT_00ae23bc + 0x52u + uVar2 * 0x6eu);
        }
        else {
          iVar3 = (int)cm_fn_004d0c60(CM_GLOBAL(int *, 0x00ae1050),(int *)iVar1,0,0,-1,0);
        }
        iVar4 = (int)((u32)iVar4 + (u32)iVar3);
        piVar2 = (int *)cm_fn_0068d1b0(*(int *)(param_1 + 4));
      } while (piVar2 != (int *)0x0);
      return iVar4;
    }
  }
  return iVar4;
}
