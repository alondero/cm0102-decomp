/* Draft for preferred-image VA 0x005b5a60, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0

u32 CM_STDCALL cm_fn_005b5a60(s16 param_1)

{
  int iVar1;
  
  iVar1 = (int)param_1;
  if (iVar1 < 0x209) {
    if (iVar1 == 0x208) {
      return 0x2000;
    }
    if (iVar1 < 0x103) {
      if (iVar1 == 0x102) {
        return 4;
      }
      u32 index = (u32)iVar1 - 1u;
      if (index <= 0xbfu) {
        u8 slot = CM_GLOBAL(volatile u8, 0x005b5c4c + index);
        u32 target = CM_GLOBAL(volatile u32, 0x005b5c2c + (u32)slot * 4u);
        (void)target;
      }
      switch(iVar1) {
      case 1:
        return 1;
      case 0x82:
        return 2;
      case 0x84:
        return 0x40;
      case 0x88:
        return 0x800;
      case 0x90:
        return 0x10000;
      case 0xa0:
        return 0x200000;
      case 0xc0:
        return 0x4000000;
      }
    }
    else if (iVar1 < 0x121) {
      if (iVar1 == 0x120) {
        return 0x400000;
      }
      if (iVar1 == 0x104) {
        return 0x80;
      }
      if (iVar1 == 0x108) {
        return 0x1000;
      }
      if (iVar1 == 0x110) {
        return 0x20000;
      }
    }
    else {
      if (iVar1 == 0x140) {
        return 0x8000000;
      }
      if (iVar1 == 0x202) {
        return 8;
      }
      if (iVar1 == 0x204) {
        return 0x100;
      }
    }
  }
  else if (iVar1 < 0x421) {
    if (iVar1 == 0x420) {
      return 0x1000000;
    }
    if (iVar1 < 0x403) {
      if (iVar1 == 0x402) {
        return 0x10;
      }
      if (iVar1 == 0x210) {
        return 0x40000;
      }
      if (iVar1 == 0x220) {
        return 0x800000;
      }
      if (iVar1 == 0x240) {
        return 0x10000000;
      }
    }
    else {
      if (iVar1 == 0x404) {
        return 0x200;
      }
      if (iVar1 == 0x408) {
        return 0x4000;
      }
      if (iVar1 == 0x410) {
        return 0x80000;
      }
    }
  }
  else if (iVar1 < 0x809) {
    if (iVar1 == 0x808) {
      return 0x8000;
    }
    if (iVar1 == 0x440) {
      return 0x20000000;
    }
    if ((iVar1 == 0x802) || (iVar1 == 0x804)) {
      return 0x400;
    }
  }
  else {
    if (iVar1 == 0x810) {
      return 0x100000;
    }
    if (iVar1 == 0x820) {
      return 0x2000000;
    }
    if (iVar1 == 0x840) {
      return 0x40000000;
    }
  }
  return 0;
}
