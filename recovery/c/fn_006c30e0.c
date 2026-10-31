/* Draft for preferred-image VA 0x006c30e0, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
extern long double CM_THISCALL cm_fn_006f09b0(u32 self,s8 param_2,s8 param_3);
extern u32 CM_THISCALL cm_fn_00717410(u32 self);

u32 CM_THISCALL
cm_fn_006c30e0(int param_1,s8 param_2,u32 param_3,u32 param_4,s8 param_5)

{
  u32 iVar1;
  short sVar2;
  int iVar3;
  long double fVar4;
  float local_4;
  
  iVar3 = 0;
  local_4 = 5000.0;
  iVar1 = (u32)param_1 + 0x4796 + (u32)(s32)param_2 * 0x22d8;
  if (((-1 < *(volatile s8 *)(iVar1 + 0x19)) && (sVar2 = (short)cm_fn_00717410(iVar1 + 0), sVar2 != 1)
      ) && ((param_5 != '\0' || (*(volatile s8 *)(iVar1 + 0x2b) < '\x02')))) {
    fVar4 = (long double)cm_fn_006f09b0(iVar1 + 0,(s8)param_3,(s8)param_4);
    if ((fVar4 == (long double)0.0 || fVar4 != fVar4)) {
      return iVar1;
    }
    if (fVar4 < (long double)5000.0) {
      local_4 = (float)fVar4;
      iVar3 = iVar1;
    }
  }
  if (((-1 < *(volatile s8 *)(iVar1 + 0x1d7)) && (sVar2 = (short)cm_fn_00717410(iVar1 + 0x1be), sVar2 != 1)) &&
     ((param_5 != '\0' || (*(volatile s8 *)(iVar1 + 0x1e9) < '\x02')))) {
    fVar4 = (long double)cm_fn_006f09b0(iVar1 + 0x1be,(s8)param_3,(s8)param_4);
    if ((fVar4 == (long double)0.0 || fVar4 != fVar4)) {
      return iVar1 + 0x1be;
    }
    if (fVar4 < (long double)local_4) {
      local_4 = (float)fVar4;
      iVar3 = iVar1 + 0x1be;
    }
  }
  if (((-1 < *(volatile s8 *)(iVar1 + 0x395)) && (sVar2 = (short)cm_fn_00717410(iVar1 + 0x37c), sVar2 != 1)) &&
     ((param_5 != '\0' || (*(volatile s8 *)(iVar1 + 0x3a7) < '\x02')))) {
    fVar4 = (long double)cm_fn_006f09b0(iVar1 + 0x37c,(s8)param_3,(s8)param_4);
    if ((fVar4 == (long double)0.0 || fVar4 != fVar4)) {
      return iVar1 + 0x37c;
    }
    if (fVar4 < (long double)local_4) {
      local_4 = (float)fVar4;
      iVar3 = iVar1 + 0x37c;
    }
  }
  if (((-1 < *(volatile s8 *)(iVar1 + 0x553)) && (sVar2 = (short)cm_fn_00717410(iVar1 + 0x53a), sVar2 != 1)) &&
     ((param_5 != '\0' || (*(volatile s8 *)(iVar1 + 0x565) < '\x02')))) {
    fVar4 = (long double)cm_fn_006f09b0(iVar1 + 0x53a,(s8)param_3,(s8)param_4);
    if ((fVar4 == (long double)0.0 || fVar4 != fVar4)) {
      return iVar1 + 0x53a;
    }
    if (fVar4 < (long double)local_4) {
      local_4 = (float)fVar4;
      iVar3 = iVar1 + 0x53a;
    }
  }
  if (((-1 < *(volatile s8 *)(iVar1 + 0x711)) && (sVar2 = (short)cm_fn_00717410(iVar1 + 0x6f8), sVar2 != 1)) &&
     ((param_5 != '\0' || (*(volatile s8 *)(iVar1 + 0x723) < '\x02')))) {
    fVar4 = (long double)cm_fn_006f09b0(iVar1 + 0x6f8,(s8)param_3,(s8)param_4);
    if ((fVar4 == (long double)0.0 || fVar4 != fVar4)) {
      return iVar1 + 0x6f8;
    }
    if (fVar4 < (long double)local_4) {
      local_4 = (float)fVar4;
      iVar3 = iVar1 + 0x6f8;
    }
  }
  if (((-1 < *(volatile s8 *)(iVar1 + 0x8cf)) && (sVar2 = (short)cm_fn_00717410(iVar1 + 0x8b6), sVar2 != 1)) &&
     ((param_5 != '\0' || (*(volatile s8 *)(iVar1 + 0x8e1) < '\x02')))) {
    fVar4 = (long double)cm_fn_006f09b0(iVar1 + 0x8b6,(s8)param_3,(s8)param_4);
    if ((fVar4 == (long double)0.0 || fVar4 != fVar4)) {
      return iVar1 + 0x8b6;
    }
    if (fVar4 < (long double)local_4) {
      local_4 = (float)fVar4;
      iVar3 = iVar1 + 0x8b6;
    }
  }
  if (((-1 < *(volatile s8 *)(iVar1 + 0xa8d)) && (sVar2 = (short)cm_fn_00717410(iVar1 + 0xa74), sVar2 != 1)) &&
     ((param_5 != '\0' || (*(volatile s8 *)(iVar1 + 0xa9f) < '\x02')))) {
    fVar4 = (long double)cm_fn_006f09b0(iVar1 + 0xa74,(s8)param_3,(s8)param_4);
    if ((fVar4 == (long double)0.0 || fVar4 != fVar4)) {
      return iVar1 + 0xa74;
    }
    if (fVar4 < (long double)local_4) {
      local_4 = (float)fVar4;
      iVar3 = iVar1 + 0xa74;
    }
  }
  if (((-1 < *(volatile s8 *)(iVar1 + 0xc4b)) && (sVar2 = (short)cm_fn_00717410(iVar1 + 0xc32), sVar2 != 1)) &&
     ((param_5 != '\0' || (*(volatile s8 *)(iVar1 + 0xc5d) < '\x02')))) {
    fVar4 = (long double)cm_fn_006f09b0(iVar1 + 0xc32,(s8)param_3,(s8)param_4);
    if ((fVar4 == (long double)0.0 || fVar4 != fVar4)) {
      return iVar1 + 0xc32;
    }
    if (fVar4 < (long double)local_4) {
      local_4 = (float)fVar4;
      iVar3 = iVar1 + 0xc32;
    }
  }
  if (((-1 < *(volatile s8 *)(iVar1 + 0xe09)) && (sVar2 = (short)cm_fn_00717410(iVar1 + 0xdf0), sVar2 != 1)) &&
     ((param_5 != '\0' || (*(volatile s8 *)(iVar1 + 0xe1b) < '\x02')))) {
    fVar4 = (long double)cm_fn_006f09b0(iVar1 + 0xdf0,(s8)param_3,(s8)param_4);
    if ((fVar4 == (long double)0.0 || fVar4 != fVar4)) {
      return iVar1 + 0xdf0;
    }
    if (fVar4 < (long double)local_4) {
      local_4 = (float)fVar4;
      iVar3 = iVar1 + 0xdf0;
    }
  }
  if (((-1 < *(volatile s8 *)(iVar1 + 0xfc7)) && (sVar2 = (short)cm_fn_00717410(iVar1 + 0xfae), sVar2 != 1)) &&
     ((param_5 != '\0' || (*(volatile s8 *)(iVar1 + 0xfd9) < '\x02')))) {
    fVar4 = (long double)cm_fn_006f09b0(iVar1 + 0xfae,(s8)param_3,(s8)param_4);
    if ((fVar4 == (long double)0.0 || fVar4 != fVar4)) {
      return iVar1 + 0xfae;
    }
    if (fVar4 < (long double)local_4) {
      local_4 = (float)fVar4;
      iVar3 = iVar1 + 0xfae;
    }
  }
  if (((-1 < *(volatile s8 *)(iVar1 + 0x1185)) && (sVar2 = (short)cm_fn_00717410(iVar1 + 0x116c), sVar2 != 1)) &&
     ((param_5 != '\0' || (*(volatile s8 *)(iVar1 + 0x1197) < '\x02')))) {
    fVar4 = (long double)cm_fn_006f09b0(iVar1 + 0x116c,(s8)param_3,(s8)param_4);
    if ((fVar4 == (long double)0.0 || fVar4 != fVar4)) {
      return iVar1 + 0x116c;
    }
    if (fVar4 < (long double)local_4) {
      local_4 = (float)fVar4;
      iVar3 = iVar1 + 0x116c;
    }
  }
  if (((-1 < *(volatile s8 *)(iVar1 + 0x1343)) && (sVar2 = (short)cm_fn_00717410(iVar1 + 0x132a), sVar2 != 1)) &&
     ((param_5 != '\0' || (*(volatile s8 *)(iVar1 + 0x1355) < '\x02')))) {
    fVar4 = (long double)cm_fn_006f09b0(iVar1 + 0x132a,(s8)param_3,(s8)param_4);
    if ((fVar4 == (long double)0.0 || fVar4 != fVar4)) {
      return iVar1 + 0x132a;
    }
    if (fVar4 < (long double)local_4) {
      local_4 = (float)fVar4;
      iVar3 = iVar1 + 0x132a;
    }
  }
  if (((-1 < *(volatile s8 *)(iVar1 + 0x1501)) && (sVar2 = (short)cm_fn_00717410(iVar1 + 0x14e8), sVar2 != 1)) &&
     ((param_5 != '\0' || (*(volatile s8 *)(iVar1 + 0x1513) < '\x02')))) {
    fVar4 = (long double)cm_fn_006f09b0(iVar1 + 0x14e8,(s8)param_3,(s8)param_4);
    if ((fVar4 == (long double)0.0 || fVar4 != fVar4)) {
      return iVar1 + 0x14e8;
    }
    if (fVar4 < (long double)local_4) {
      local_4 = (float)fVar4;
      iVar3 = iVar1 + 0x14e8;
    }
  }
  if (((-1 < *(volatile s8 *)(iVar1 + 0x16bf)) && (sVar2 = (short)cm_fn_00717410(iVar1 + 0x16a6), sVar2 != 1)) &&
     ((param_5 != '\0' || (*(volatile s8 *)(iVar1 + 0x16d1) < '\x02')))) {
    fVar4 = (long double)cm_fn_006f09b0(iVar1 + 0x16a6,(s8)param_3,(s8)param_4);
    if ((fVar4 == (long double)0.0 || fVar4 != fVar4)) {
      return iVar1 + 0x16a6;
    }
    if (fVar4 < (long double)local_4) {
      local_4 = (float)fVar4;
      iVar3 = iVar1 + 0x16a6;
    }
  }
  if (((-1 < *(volatile s8 *)(iVar1 + 0x187d)) && (sVar2 = (short)cm_fn_00717410(iVar1 + 0x1864), sVar2 != 1)) &&
     ((param_5 != '\0' || (*(volatile s8 *)(iVar1 + 0x188f) < '\x02')))) {
    fVar4 = (long double)cm_fn_006f09b0(iVar1 + 0x1864,(s8)param_3,(s8)param_4);
    if ((fVar4 == (long double)0.0 || fVar4 != fVar4)) {
      return iVar1 + 0x1864;
    }
    if (fVar4 < (long double)local_4) {
      local_4 = (float)fVar4;
      iVar3 = iVar1 + 0x1864;
    }
  }
  if (((-1 < *(volatile s8 *)(iVar1 + 0x1a3b)) && (sVar2 = (short)cm_fn_00717410(iVar1 + 0x1a22), sVar2 != 1)) &&
     ((param_5 != '\0' || (*(volatile s8 *)(iVar1 + 0x1a4d) < '\x02')))) {
    fVar4 = (long double)cm_fn_006f09b0(iVar1 + 0x1a22,(s8)param_3,(s8)param_4);
    if ((fVar4 == (long double)0.0 || fVar4 != fVar4)) {
      return iVar1 + 0x1a22;
    }
    if (fVar4 < (long double)local_4) {
      local_4 = (float)fVar4;
      iVar3 = iVar1 + 0x1a22;
    }
  }
  if (((-1 < *(volatile s8 *)(iVar1 + 0x1bf9)) && (sVar2 = (short)cm_fn_00717410(iVar1 + 0x1be0), sVar2 != 1)) &&
     ((param_5 != '\0' || (*(volatile s8 *)(iVar1 + 0x1c0b) < '\x02')))) {
    fVar4 = (long double)cm_fn_006f09b0(iVar1 + 0x1be0,(s8)param_3,(s8)param_4);
    if ((fVar4 == (long double)0.0 || fVar4 != fVar4)) {
      return iVar1 + 0x1be0;
    }
    if (fVar4 < (long double)local_4) {
      local_4 = (float)fVar4;
      iVar3 = iVar1 + 0x1be0;
    }
  }
  if (((-1 < *(volatile s8 *)(iVar1 + 0x1db7)) && (sVar2 = (short)cm_fn_00717410(iVar1 + 0x1d9e), sVar2 != 1)) &&
     ((param_5 != '\0' || (*(volatile s8 *)(iVar1 + 0x1dc9) < '\x02')))) {
    fVar4 = (long double)cm_fn_006f09b0(iVar1 + 0x1d9e,(s8)param_3,(s8)param_4);
    if ((fVar4 == (long double)0.0 || fVar4 != fVar4)) {
      return iVar1 + 0x1d9e;
    }
    if (fVar4 < (long double)local_4) {
      local_4 = (float)fVar4;
      iVar3 = iVar1 + 0x1d9e;
    }
  }
  if (((-1 < *(volatile s8 *)(iVar1 + 0x1f75)) && (sVar2 = (short)cm_fn_00717410(iVar1 + 0x1f5c), sVar2 != 1)) &&
     ((param_5 != '\0' || (*(volatile s8 *)(iVar1 + 0x1f87) < '\x02')))) {
    fVar4 = (long double)cm_fn_006f09b0(iVar1 + 0x1f5c,(s8)param_3,(s8)param_4);
    if ((fVar4 == (long double)0.0 || fVar4 != fVar4)) {
      return iVar1 + 0x1f5c;
    }
    if (fVar4 < (long double)local_4) {
      local_4 = (float)fVar4;
      iVar3 = iVar1 + 0x1f5c;
    }
  }
  if (((-1 < *(volatile s8 *)(iVar1 + 0x2133)) && (sVar2 = (short)cm_fn_00717410(iVar1 + 0x211a), sVar2 != 1)) &&
     ((param_5 != '\0' || (*(volatile s8 *)(iVar1 + 0x2145) < '\x02')))) {
    fVar4 = (long double)cm_fn_006f09b0(iVar1 + 0x211a,(s8)param_3,(s8)param_4);
    if ((fVar4 == (long double)0.0 || fVar4 != fVar4)) {
      return iVar1 + 0x211a;
    }
    if (fVar4 < (long double)local_4) {
      iVar3 = iVar1 + 0x211a;
    }
  }
  return iVar3;
}




