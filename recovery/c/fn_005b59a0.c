/* Draft for preferred-image VA 0x005b59a0, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
extern u32 CM_STDCALL cm_fn_005b5a60(s16 param_1);

void CM_THISCALL cm_fn_005b59a0(u32 self)

{
  u32 uVar1;
  int iVar2;
  volatile u16 *puVar3;
  
  iVar2 = 0xb;
  *(volatile u32 *)(self + 0x187b) = 0;
  puVar3 = (volatile u16 *)(self + 0x12d);
  do {
    uVar1 = cm_fn_005b5a60((s16)*puVar3);
    puVar3 = puVar3 + 1;
    uVar1 = *(volatile u32 *)(self + 0x187b) | uVar1;
    iVar2 = iVar2 + -1;
    *(volatile u32 *)(self + 0x187b) = uVar1;
  } while (iVar2 != 0);
  if ((uVar1 == 0x101176c1) || (uVar1 == 0x101b26c1)) {
    *(volatile u32 *)(self + 0x187b) = 0x101f06c1;
  }
  else {
    if ((uVar1 == 0x38115381) || (uVar1 == 0x3a242381)) {
      *(volatile u32 *)(self + 0x187b) = 0x381b0381;
      return;
    }
    if ((((uVar1 == 0x280e07c1) || (uVar1 == 0x91a07c1)) || (uVar1 == 0x2884ab81)) ||
       (((uVar1 == 0x280aab81 || (uVar1 == 0x288c07c1)) || (uVar1 == 0x281507c1)))) {
      *(volatile u32 *)(self + 0x187b) = 0x280e8b81;
      return;
    }
    if (uVar1 == 0x288a8b81) {
      *(volatile u32 *)(self + 0x187b) = 0x281b2381;
      return;
    }
  }
  return;
}
