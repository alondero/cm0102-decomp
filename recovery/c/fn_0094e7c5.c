/* Draft for preferred-image VA 0x0094e7c5, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_00de4520 CM_GLOBAL(u8 volatile, 0x00de4520)

void cm_fn_0094e7c5(volatile u8 *param_1,volatile u32 *param_2,volatile u8 *param_3,volatile u32 *param_4,volatile u32 *param_5)

{
  u8 bVar1;
  u8 bVar2;
  u8 bVar3;
  volatile u8 *pbVar4;
  volatile u8 *pbVar5;
  u32 uVar6;
  volatile u32 *puVar7;
  u8 old_byte;
  u32 old_dword;
  
  old_dword = *param_5;
  *param_5 = old_dword & 0;
  *param_4 = 1;
  if (param_2 != (u32 *)0x0) {
    *param_2 = (u32)param_3;
    param_2 = param_2 + 1;
  }
  if (*param_1 == 0x22) {
    while( true ) {
      bVar1 = param_1[1];
      pbVar4 = param_1 + 1;
      if ((bVar1 == 0x22) || (bVar1 == 0)) break;
      if (((*(volatile u8 *)((int)&DAT_00de4520 + bVar1 + 1) & 4) != 0) &&
         (*param_5 = *param_5 + 1, param_3 != (u8 *)0x0)) {
        *param_3 = *pbVar4;
        param_3 = param_3 + 1;
        pbVar4 = param_1 + 2;
      }
      *param_5 = *param_5 + 1;
      param_1 = pbVar4;
      if (param_3 != (u8 *)0x0) {
        *param_3 = *pbVar4;
        param_3 = param_3 + 1;
      }
    }
    *param_5 = *param_5 + 1;
    if (param_3 != (u8 *)0x0) {
      old_byte = *param_3;
      *param_3 = old_byte & 0;
      param_3 = param_3 + 1;
    }
    if (*pbVar4 == 0x22) {
      pbVar4 = param_1 + 2;
    }
  }
  else {
    do {
      *param_5 = *param_5 + 1;
      if (param_3 != (u8 *)0x0) {
        *param_3 = *param_1;
        param_3 = param_3 + 1;
      }
      bVar1 = *param_1;
      pbVar4 = param_1 + 1;
      if ((*(volatile u8 *)((int)&DAT_00de4520 + bVar1 + 1) & 4) != 0) {
        *param_5 = *param_5 + 1;
        if (param_3 != (u8 *)0x0) {
          *param_3 = *pbVar4;
          param_3 = param_3 + 1;
        }
        pbVar4 = param_1 + 2;
      }
      if (bVar1 == 0x20) break;
      if (bVar1 == 0) goto LAB_0094e870;
      param_1 = pbVar4;
    } while (bVar1 != 9);
    if (bVar1 == 0) {
LAB_0094e870:
      pbVar4 = pbVar4 + -1;
    }
    else if (param_3 != (u8 *)0x0) {
      old_byte = param_3[-1];
      param_3[-1] = old_byte & 0;
    }
  }
  bVar2 = false;
  puVar7 = param_2;
  while (*pbVar4 != 0) {
    for (;;) {
      u8 skip = *pbVar4;
      if (skip != 0x20 && skip != 9) break;
      pbVar4 = pbVar4 + 1;
    }
    if (*pbVar4 == 0) break;
    if (puVar7 != (u32 *)0x0) {
      *puVar7 = (u32)param_3;
      puVar7 = puVar7 + 1;
      param_2 = puVar7;
    }
    *param_4 = *param_4 + 1;
    while( true ) {
      bVar3 = true;
      uVar6 = 0;
      for (; *pbVar4 == 0x5c; pbVar4 = pbVar4 + 1) {
        uVar6 = uVar6 + 1;
      }
      if (*pbVar4 == 0x22) {
        pbVar5 = pbVar4;
        if ((uVar6 & 1) == 0) {
          if ((!bVar2) || (pbVar5 = pbVar4 + 1, pbVar4[1] != 0x22)) {
            bVar3 = false;
            pbVar5 = pbVar4;
          }
          bVar2 = !bVar2;
          puVar7 = param_2;
        }
        uVar6 = uVar6 >> 1;
        pbVar4 = pbVar5;
      }
      for (; uVar6 != 0; uVar6 = uVar6 - 1) {
        if (param_3 != (u8 *)0x0) {
          *param_3 = 0x5c;
          param_3 = param_3 + 1;
        }
        *param_5 = *param_5 + 1;
      }
      bVar1 = *pbVar4;
      if ((bVar1 == 0) || ((!bVar2 && ((bVar1 == 0x20 || (bVar1 == 9)))))) break;
      if (bVar3) {
        if (param_3 == (u8 *)0x0) {
          if ((*(volatile u8 *)((int)&DAT_00de4520 + bVar1 + 1) & 4) != 0) {
            pbVar4 = pbVar4 + 1;
            *param_5 = *param_5 + 1;
          }
        }
        else {
          if ((*(volatile u8 *)((int)&DAT_00de4520 + bVar1 + 1) & 4) != 0) {
            *param_3 = bVar1;
            param_3 = param_3 + 1;
            pbVar4 = pbVar4 + 1;
            *param_5 = *param_5 + 1;
          }
          *param_3 = *pbVar4;
          param_3 = param_3 + 1;
        }
        *param_5 = *param_5 + 1;
      }
      pbVar4 = pbVar4 + 1;
    }
    if (param_3 != (u8 *)0x0) {
      old_byte = *param_3;
      *param_3 = old_byte & 0;
      param_3 = param_3 + 1;
    }
    *param_5 = *param_5 + 1;
  }
  if (puVar7 != (u32 *)0x0) {
    old_dword = *puVar7;
    *puVar7 = old_dword & 0;
  }
  *param_4 = *param_4 + 1;
  return;
}
