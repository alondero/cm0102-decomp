/* WIP: wrong arg map and copied clears; listing has six args and no output clears */
/* Draft for preferred-image VA 0x00933e50, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0

void cm_fn_00933e50(int param_1,int param_2,u32 param_3,u32 param_4,short param_5,
                 short param_6,short param_7)

{
  (void)param_4;
  *(volatile u32 *)param_3 = 0;
  *(volatile u32 *)param_2 = 0;
  *(volatile u16 *)(param_2 + 0x14) = 0;
  *(volatile u16 *)(param_2 + 0x16) = 0;
  *(volatile u16 *)(param_3 + 0x14) = 0;
  *(volatile u16 *)(param_3 + 0x16) = 0;
  *(volatile u16 *)(param_2 + 0x14) = *(volatile u16 *)(param_1 + 0x34);
  *(volatile u16 *)(param_2 + 0x16) = *(volatile u16 *)(param_1 + 0x36);
  volatile s16 common_scale = *(volatile s16 *)(param_1 + 0x29c);
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  if ((s16)param_3 == 0) {
    iVar4 = *(int *)(param_1 + 0x2a2);
    iVar3 = *(int *)(param_1 + 0x16);
    iVar5 = *(int *)(param_2 + 0x1c);
  }
  else {
    iVar4 = *(int *)(param_1 + 0x29e);
    iVar3 = *(int *)(param_1 + 0x12);
    iVar5 = *(int *)(param_2 + 0x18);
  }
  iVar1 = (int)*(short *)(iVar3 + param_7 * 2);
  iVar6 = (int)common_scale;
  iVar2 = (((iVar1 - *(short *)(iVar3 + param_6 * 2)) * iVar4 * 0x40 + (iVar6 >> 1)) / iVar6) *
          *(int *)(param_1 + 0x426) + 0x20;
  iVar2 = *(int *)(iVar5 + param_6 * 4) + ((int)((iVar2 >> 6) + (iVar2 >> 0x1f & 0x3fU)) >> 6);
  iVar3 = (((iVar1 - *(short *)(iVar3 + param_5 * 2)) * iVar4 * 0x40 + (iVar6 >> 1)) / iVar6) *
          *(int *)(param_1 + 0x426) + 0x20;
  iVar4 = ((int)((iVar3 >> 6) + (iVar3 >> 0x1f & 0x3fU)) >> 6) + *(int *)(iVar5 + param_5 * 4);
  iVar3 = iVar4;
  if (*(int *)(param_1 + 0x42a) == 0) {
    if (iVar4 < iVar2) {
      iVar3 = iVar4 + 0x40;
      if (iVar3 <= iVar2) goto LAB_00933f44;
    }
    else {
      iVar3 = iVar4 + -0x40;
      if (iVar2 <= iVar4 + -0x40) goto LAB_00933f44;
    }
    iVar3 = iVar2;
  }
LAB_00933f44:
  if (*(int *)(param_1 + 0x2a6) == 2) {
    *(int *)(iVar5 + param_7 * 4) = (iVar3 + 1 + iVar2) / 2;
  }
  else {
    *(int *)(iVar5 + param_7 * 4) = (iVar3 + 1 + iVar2 * 2) / 3;
  }
  *(u32 *)(iVar5 + param_7 * 4) = (*(int *)(iVar5 + param_7 * 4) + 0x20U) & 0xffffffc0;
  return;
}
