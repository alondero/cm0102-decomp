/* Draft for preferred-image VA 0x004ca470, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0

u8 CM_STDCALL cm_fn_004ca470(u32 param_1,u32 param_2)

{
  int iVar1;
  char cVar2;
  char age;
  
  if ((param_1 == 0) || (iVar1 = *(volatile int *)(param_1 + 0x61), iVar1 == 0))
  goto switchD_004ca4c0_default;
  if (param_2 == 0) {
    return '\x04';
  }
  cVar2 = '\x05';
  if ((*(volatile u8 *)(param_2 + 0x35) & 0x3f) == 5) {
    return '\b';
  }
  u8 kind = *(volatile u8 *)(param_2 + 0x4f);
  u32 index = kind >> 4;
  if (index <= 6) { volatile u32 target = *(volatile u32 *)(0x004ca6a8 + index * 4); (void)target; }
  switch(index) {
  case 0:
    age = *(volatile s8 *)(param_1 + 0x18);
    cVar2 = *(volatile char *)(param_2 + 0x3d);
    if ('\x15' < age) {
      if ('K' < cVar2) {
        return '\x01';
      }
      if ('\'' < cVar2) {
        return '\x02';
      }
      if ('\t' < cVar2) {
        return '\x03';
      }
LAB_004ca5d0:
      return (((cVar2 < -0x1d) - 1U) & 0xfd) + 7;
    }
    if ('\0' < cVar2) {
      return '\x05';
    }
    if ((-0x1e < cVar2) || ('\x05' < *(volatile char *)(param_1 + 0x57))) {
      return '\x06';
    }
    goto switchD_004ca4c0_default;
  default:
switchD_004ca4c0_default:
    cVar2 = '\a';
    break;
  case 1:
    cVar2 = *(volatile char *)(param_2 + 0x3d);
    if ('-' < cVar2) {
      return '\x01';
    }
    return (cVar2 < '\x1a') + '\x02';
  case 2:
    cVar2 = *(volatile char *)(param_2 + 0x3d);
    if ('K' < cVar2) {
      s32 first = *(volatile s8 *)(param_1 + 0x57);
      s32 second = *(volatile s8 *)(param_1 + 0x59);
      s32 third = *(volatile s16 *)(iVar1 + 0xd);
      s32 total = first * 25 + second * -10 + third;
      if (total > 0x186a) {
        age = *(volatile s8 *)(param_1 + 0x18);
        if (age > 0x17 && age < 0x22) return '\x01';
      }
    }
    if ('\x19' < cVar2) {
      return '\x02';
    }
    goto LAB_004ca535;
  case 3:
    cVar2 = *(volatile char *)(param_2 + 0x3d);
    if ('7' < cVar2) {
      age = *(volatile s8 *)(param_1 + 0x18);
      if (age > 0x17 && age < 0x22) return '\x02';
    }
LAB_004ca535:
    return (cVar2 < '\x01') + '\x03';
  case 4:
    cVar2 = *(volatile char *)(param_2 + 0x3d);
    if ('A' < cVar2) {
      age = *(volatile s8 *)(param_1 + 0x18);
      if (age > 0x17) return (age < 0x22) + '\x02';
    }
    if (-0x1e < cVar2) {
      return '\x04';
    }
    age = *(volatile s8 *)(param_1 + 0x18);
    return ((('\x17' < age) - 1U) & 0xfe) + 7;
  case 5:
    age = *(volatile s8 *)(param_1 + 0x18);
    if ('\x18' < age) {
      if ('\"' < age) {
        return '\x04';
      }
      cVar2 = *(volatile char *)(param_2 + 0x3d);
      if ('K' < cVar2) {
        return '\x02';
      }
      if ('\0' < cVar2) {
        return '\x03';
      }
      goto LAB_004ca5d0;
    }
    if ((*(volatile char *)(param_1 + 0x57) < '\x06') || (*(volatile short *)(iVar1 + 0xb) < 0x4e3)) {
      return '\x06';
    }
    break;
  case 6:
    age = *(volatile s8 *)(param_1 + 0x18);
    if ('\x17' < age) {
      cVar2 = *(volatile char *)(param_2 + 0x3d);
      if ('K' < cVar2) {
        return '\x01';
      }
      if ('\x1e' < cVar2) {
        return '\x02';
      }
      if ('\0' < cVar2) {
        return '\x03';
      }
      return (((cVar2 < -0x31) - 1U) & 0xfd) + 7;
    }
    if ((*(volatile short *)(iVar1 + 0xb) < 0x4e3) || (*(volatile char *)(param_1 + 0x57) < '\x10')) {
      return '\x06';
    }
  }
  return cVar2;
}

