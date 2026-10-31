/* Draft for preferred-image VA 0x0059b630, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0

void CM_THISCALL cm_fn_0059b630(volatile u32 *param_1,volatile u32 *param_2)

{
  u32 uVar1;
  short sVar2;
  u32 uVar3;
  char cVar4;
  volatile u32 *puVar5;
  u32 idx;
  u32 row;
  u32 old_12c;
  u32 old_8c;
  
  cVar4 = *(volatile s8 *)((u8 *)param_2 + 0x7f);
  uVar3 = cVar4 * 50000 + 75000;
  if (cVar4 < '\x13') {
    cVar4 = cVar4 + '\x02';
  }
  else {
    cVar4 = cVar4 + '\x01';
  }
  *(volatile s8 *)((u8 *)param_2 + 0x7f) = cVar4;
  idx = *(volatile u32 *)param_2;
  row = idx * 0x167u;
  row += *(volatile u32 *)param_1;
  puVar5 = (volatile u32 *)row;
  old_12c = puVar5[0x4b];
  old_8c = puVar5[0x23];
  old_12c += uVar3;
  old_8c += uVar3;
  puVar5[0x4b] = old_12c;
  puVar5[0x23] = old_8c;
  uVar1 = *puVar5;
  *puVar5 = uVar1 - uVar3;
  puVar5[1] = (puVar5[1] - ((int)uVar3 >> 0x1f)) - (u32)(uVar1 < uVar3);
  cVar4 = *(volatile s8 *)((u8 *)param_2 + 0x7f);
  if ((cVar4 < '\x14') && (0x203a < *(volatile s16 *)((u8 *)param_2 + 0x80))) {
    uVar3 = (0x16 - cVar4) * 250000;
    if ((int)uVar3 < 2000000) {
      uVar3 = 2000000;
    }
    *(volatile u8 *)((u8 *)param_2 + 0x7f) = 0x14;
  }
  else if ((cVar4 < '\x0e') && (0x1482 < *(volatile s16 *)((u8 *)param_2 + 0x80))) {
    uVar3 = cVar4 * -150000 + 3500000;
    if ((int)uVar3 < 1000000) {
      uVar3 = 1000000;
    }
    *(volatile s8 *)((u8 *)param_2 + 0x7f) = cVar4 + '\x04';
    if ((char)(cVar4 + '\x04') < '\x10') {
      *(volatile u8 *)((u8 *)param_2 + 0x7f) = 0x10;
    }
  }
  else {
    if ((('\x0f' < cVar4) || (sVar2 = *(volatile s16 *)((u8 *)param_2 + 0x80), sVar2 < 0x109b)) &&
       (('\x13' < cVar4 || (sVar2 = *(volatile s16 *)((u8 *)param_2 + 0x80), sVar2 < 0x1c53)))) goto LAB_0059b7c8;
    if (sVar2 < 0x1c53) {
      *(volatile s8 *)((u8 *)param_2 + 0x7f) = cVar4 + '\x02';
      if ((char)(cVar4 + '\x02') < '\f') {
        *(volatile u8 *)((u8 *)param_2 + 0x7f) = 0xc;
      }
    }
    else {
      *(volatile u8 *)((u8 *)param_2 + 0x7f) = 0x14;
    }
    uVar3 = *(volatile s8 *)((u8 *)param_2 + 0x7f) * 50000 + 75000;
  }
  idx = *(volatile u32 *)param_2;
  row = idx * 0x167u;
  row += *(volatile u32 *)param_1;
  puVar5 = (volatile u32 *)row;
  puVar5[0x23] = puVar5[0x23] + uVar3;
  puVar5[0x4b] = puVar5[0x4b] + uVar3;
  uVar1 = *puVar5;
  *puVar5 = uVar1 - uVar3;
  puVar5[1] = (puVar5[1] - ((int)uVar3 >> 0x1f)) - (u32)(uVar1 < uVar3);
LAB_0059b7c8:
  if ('\x14' < *(volatile s8 *)((u8 *)param_2 + 0x7f)) {
    *(volatile u8 *)((u8 *)param_2 + 0x7f) = 0x14;
  }
  return;
}
