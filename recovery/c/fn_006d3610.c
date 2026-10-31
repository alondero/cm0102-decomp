/* WIP for preferred-image VA 0x006d3610, image base 0x00400000. Original VA
   0x006d391f-0x006d3cea keeps values in x87 ST0/ST1 across repeated __ftol calls
   at 0x006d399c, 0x006d39bc, 0x006d39de, 0x006d3a54, 0x006d3abf, 0x006d3beb,
   0x006d3c60 and 0x006d3cea. Plain C cannot transport that stack faithfully;
   partial WORD markers also remain unresolved. No test run claimed. */
/* Draft for preferred-image VA 0x006d3610, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
extern u8 cm_fn_0053b900(int *param_1);
extern /* Library Function - Single Match cm_fn_00945640 Library: Visual Studio */ long long cm_fn_00945640(void);
/* NEEDS-REVIEW: ._0_2_, extraout_ST1 */

void cm_fn_006d3610(short *param_1,int param_2,int param_3,int *param_4,u32 param_5,
                 u32 param_6,char param_7)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  int iVar6;
  int iVar7;
  char cVar8;
  long double extraout_ST1;
  short local_2c;
  short local_28;
  
  local_28 = 2;
  iVar7 = *(int *)((int)param_1 + 0x1b);
  if ((iVar7 != 0) && (*(int *)(iVar7 + 0x69) != 0)) {
    if (*param_4 == -2) {
      cVar8 = '\x05';
    }
    else {
      if ((char)param_4[0x10] == '\0') {
        cVar8 = *(char *)(param_4[5] + 0x69);
      }
      else {
        cVar8 = (char)(((int)*(short *)(param_3 + 0x80) / 500 + (int)*(short *)(param_4[5] + 0x69))
                      / 2);
      }
      if ('\x14' < cVar8) {
        cVar8 = '\x14';
      }
    }
    iVar6 = cm_fn_0053b900(param_2);
    if (((iVar6 != 0) ||
        (((*param_4 == -2 && (*(int *)(param_2 + 0x53) != 0)) &&
         (*(int *)(param_2 + 0x53) == *(int *)(iVar7 + 0x1a))))) ||
       (((param_4[5] != 0 && (*(int *)(param_4[5] + 0x5d) == *(int *)(iVar7 + 0x1a))) ||
        (*(int *)(param_2 + 0x53) == *(int *)(iVar7 + 0x1a))))) {
      local_28 = 1;
    }
    iVar6 = cm_fn_0053b900(param_2);
    if (iVar6 == 0) {
      sVar5 = 2;
      if (*(char *)((int)param_4 + 0x3a) == '\b') {
        sVar1 = *(short *)((int)param_4 + 0x32);
        local_2c = 1;
        sVar5 = local_2c;
        if ((((sVar1 != 0x41f) && (sVar1 != 0x420)) &&
            ((sVar1 != 0x421 &&
             ((((sVar1 != 0x422 && (sVar1 != 0x423)) && (sVar1 != 0x424)) &&
              ((sVar1 != 0x425 && (sVar1 != 0x426)))))))) &&
           ((((sVar1 = (short)param_4[0xc], sVar1 == 0x78 || ((sVar1 == 0x79 || (sVar1 == 0x7a))))
             || ((((sVar1 == 0x7b || (((sVar1 == 0x7c || (sVar1 == 0x82)) || (sVar1 == 0x83)))) ||
                  (((sVar1 == 0x84 || (sVar1 == 0x85)) || (sVar1 == 0x86)))) ||
                 ((sVar1 == 0x87 || (sVar1 == 0x96)))))) ||
            (((sVar1 == 0x97 || (((sVar1 == 0x98 || (sVar1 == 0x99)) || (sVar1 == 0x9a)))) ||
             (sVar5 = 1, sVar1 == 0x9b)))))) {
          cVar8 = *(char *)(param_4[5] + 0x69);
          sVar5 = local_2c;
        }
      }
    }
    else {
      local_2c = 1;
      sVar5 = local_2c;
    }
    local_2c = sVar5;
    iVar7 = *(int *)(iVar7 + 0x69);
    sVar5 = *(short *)(iVar7 + 8);
    cm_fn_00945640();
    sVar1 = *(short *)(iVar7 + 10);
    cm_fn_00945640();
    sVar2 = *(short *)(iVar7 + 0xc);
    cm_fn_00945640();
    if ((param_7 != '\0') && (cm_fn_00945640(), extraout_ST1 < (long double)0.0)) {
      cm_fn_00945640();
    }
    param_4._0_2_ = cm_fn_00945640();
    param_6._0_2_ = cm_fn_00945640();
    sVar3 = cm_fn_00945640();
    iVar6 = cVar8 * 400;
    iVar7 = iVar6 / (int)local_28;
    if (iVar7 < 5000) {
      iVar7 = 5000;
    }
    else if (10000 < iVar7) {
      iVar7 = 10000;
    }
    sVar4 = (short)iVar7;
    if ((short)iVar7 <= sVar5) {
      sVar4 = sVar5;
    }
    if (sVar4 < (short)param_6) {
      param_6._0_2_ = sVar4;
    }
    if ((short)param_6 < 1) {
      param_6._0_2_ = 1;
    }
    if (iVar6 < 5000) {
      iVar7 = 5000;
    }
    else {
      iVar7 = 10000;
      if (iVar6 < 0x2711) {
        iVar7 = iVar6;
      }
    }
    sVar5 = (short)iVar7;
    if ((short)iVar7 <= sVar1) {
      sVar5 = sVar1;
    }
    if (sVar5 < (short)param_4) {
      param_4._0_2_ = sVar5;
    }
    if ((short)param_4 < 1) {
      param_4._0_2_ = 1;
    }
    iVar6 = iVar6 / (int)local_2c;
    if (iVar6 < 3000) {
      iVar6 = 3000;
    }
    else if (10000 < iVar6) {
      iVar6 = 10000;
    }
    sVar5 = (short)iVar6;
    if ((short)iVar6 <= sVar2) {
      sVar5 = sVar2;
    }
    if (sVar5 < sVar3) {
      sVar3 = sVar5;
    }
    if (sVar3 < 1) {
      sVar3 = 1;
    }
    *param_1 = (short)param_4;
    param_1[1] = (short)param_6;
    param_1[2] = sVar3;
  }
  return;
}
