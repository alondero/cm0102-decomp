/* WIP for preferred-image VA 0x006be710, image base 0x00400000. Original VA
   0x006bea6a-0x006beab5 carries the FILD/FMUL value through __ftol at 0x006beab5,
   then feeds EAX to the next random helper. The plain-C FP boundary has no faithful
   transport here; no test run claimed. */
/* Draft for preferred-image VA 0x006be710, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define CONCAT31(a, b) ((u32)(((u32)(u32)(a) << 8) | (u32)(u8)(b)))
extern void CM_FASTCALL cm_fn_006be520(int param_1);
extern u32 CM_THISCALL cm_fn_006ce110(volatile u8 *self, s8 selector);
extern void CM_THISCALL cm_fn_006d6f70(char *param_1,u16 param_2,char *param_3,char param_4,u8 param_5,char param_6, char param_7,u32 param_8);
extern void CM_FASTCALL cm_fn_006f0630(int param_1);
extern void CM_THISCALL cm_fn_006f46b0(int param_1,char param_2,char param_3);
extern short CM_THISCALL cm_fn_00700bd0(int param_1,u8 *param_2,short *param_3);
extern /* WARNING: Type propagation algorithm not settling */ /* WARNING: Exceeded maximum restarts with more pending */ void CM_THISCALL cm_fn_00702050(int param_1,u32 param_2,int param_3,int param_4,char param_5,char param_6, int param_7);
extern u32 CM_THISCALL cm_fn_007109f0(int param_1,u32 param_2);
extern void CM_FASTCALL cm_fn_00714d40(int param_1);
extern void CM_THISCALL cm_fn_007160d0(int param_1,char param_2);
extern void CM_THISCALL cm_fn_007160e0(int param_1,u8 param_2);
extern int cm_fn_0090cfc0(int param_1);
extern /* Library Function - Single Match cm_fn_00945640 Library: Visual Studio */ long long cm_fn_00945640(void);
/* NEEDS-REVIEW: ._1_3_ */

void CM_THISCALL cm_fn_006be710(int param_1,char param_2)

{
  char *pcVar1;
  char cVar2;
  u8 uVar3;
  char cVar4;
  short sVar5;
  char *pcVar6;
  u32 uVar7;
  int iVar8;
  u32 uVar9;
  u32 uVar10;
  char *pcVar11;
  char local_15;
  int local_14;
  int local_10;
  u32 local_8;
  u32 local_4;
  
  switch(*(u8 *)(param_1 + 0x8eb3)) {
  case 1:
    *(u16 *)(param_1 + 0x475f) = 0xffff;
    *(u32 *)(param_1 + 0x4761) = 0;
    *(u32 *)(param_1 + 0x4765) = 0;
    *(u8 *)(param_1 + 0x4769) = 0;
    *(u8 *)(param_1 + 0x475a) = 0;
    *(u8 *)(param_1 + 0x475b) = 0xff;
    *(u8 *)(param_1 + 0x475c) = 0xff;
    *(u8 *)(param_1 + 0x475d) = 0xff;
    *(u8 *)(param_1 + 0x475e) = 0xff;
    *(u8 *)(param_1 + 0x8ea4) = **(u8 **)(param_1 + 0x8f7a);
    **(u8 **)(param_1 + 0x8f7a) = 0;
    *(u8 *)(param_1 + 0x8e9e) = 1;
    *(u8 *)(param_1 + 0x1c5) = 0;
    *(u32 *)(param_1 + 0x477e) = 0;
    *(u8 *)(param_1 + 0x8eaa) = 0;
    *(u8 *)(param_1 + 0x8ea9) = 0;
    *(u32 *)(param_1 + 0xf5aa) = 0;
    *(u32 *)(param_1 + 0xf5a6) = 0;
    *(u32 *)(param_1 + 0xf576) = 0;
    *(u32 *)(param_1 + 0xf57a) = 0;
    *(u8 *)(param_1 + 0x8ea6) = 0xff;
    *(u8 *)(param_1 + 0x8eb9) = 0xff;
    *(u16 *)(param_1 + 0x8ed6) = 0;
    *(u32 *)(param_1 + 0x91de) = 0;
    *(u8 *)(param_1 + 0x8ea2) = 5;
    *(u8 *)(param_1 + 0x8ea1) = 5;
    *(u8 *)(param_1 + 0x8ea0) = 0;
    *(u8 *)(param_1 + 0x8e9f) = 0;
    cm_fn_006be520();
    *(u8 *)(param_1 + 0x8ea3) = 0;
    *(u8 *)(param_1 + 0x8eb3) = 2;
    return;
  case 2:
  case 5:
    *(u8 *)(param_1 + 0x8eb2) = 0;
    if ((*(char *)(param_1 + 0x8ea1) == '\0') && (*(char *)(param_1 + 0x8ea2) == '\0')) {
      *(u8 *)(param_1 + 0x8ea2) = 1;
      *(u8 *)(param_1 + 0x8ea1) = 1;
      *(u8 *)(param_1 + 0x8ea3) = 1;
    }
    iVar8 = (int)*(char *)(param_1 + 0x8e9e);
    cVar4 = *(char *)(iVar8 * 0xb + 0x1af + param_1 + *(char *)(iVar8 + 0x8e9f + param_1) % '\v');
    pcVar1 = (char *)(param_1 + 0x4796 + ((int)cVar4 + iVar8 * 0x14) * 0x1be);
    pcVar6 = (char *)cm_fn_006ce110(CONCAT31(cVar4 >> 7,*(char *)(param_1 + 0x8e9e) == '\0'));
    *(char *)(*(char *)(param_1 + 0x8e9e) + 0x8e9f + param_1) =
         *(char *)(*(char *)(param_1 + 0x8e9e) + 0x8e9f + param_1) + '\x01';
    cm_fn_007160e0(0);
    cm_fn_007160e0(0);
    *(u8 *)(param_1 + *pcVar1 + 0x8eda + *(char *)(param_1 + 0x8e9e) * 0x14) = 1;
    cm_fn_006f0630();
    cVar4 = *(char *)(param_1 + 0x8e9e);
    *(u8 *)(param_1 + 0x8ea9) = 0;
    cVar2 = *(char *)(param_1 + 0x8ea1 + (int)cVar4);
    iVar8 = cm_fn_0090cfc0((int)*(char *)(*(int *)(pcVar1 + 0x6d) + 0x35));
    local_14 = (200 - iVar8 * (cVar2 + 4)) * 2;
    uVar9 = (u32)*(u8 *)(param_1 + 0xf5bc + cVar4 * 0x36);
    uVar10 = (u32)*(u8 *)(param_1 + 0xf5bc + (u32)(cVar4 == '\0') * 0x36);
    if ((int)(cVar2 + -1 + uVar9) < (int)uVar10) {
      if (*(char *)(param_1 + 0x8ea3) == '\0') {
        cm_fn_006d6f70(0x2005,param_1,pcVar1[0x2e],cVar4,0xffffffff,0xffffffff,0);
      }
LAB_006beab5:
      uVar7 = cm_fn_00945640();
      iVar8 = cm_fn_0090cfc0(uVar7);
      local_14 = local_14 + iVar8;
    }
    else if ((int)((int)*(char *)((cVar4 == '\0') + 0x8ea1 + param_1) + uVar10) < (int)(uVar9 + 1))
    {
      if (*(char *)(param_1 + 0x8ea3) == '\0') {
        cm_fn_006d6f70(0x2006,param_1,pcVar1[0x2e],cVar4,0xffffffff,0xffffffff,0);
      }
      goto LAB_006beab5;
    }
    if (param_2 == '\x03') {
      local_8 = CONCAT31(local_8._1_3_,0x33);
      sVar5 = cm_fn_00700bd0(&local_8,&local_4);
      iVar8 = cm_fn_0090cfc0(local_14);
      iVar8 = sVar5 - iVar8;
      uVar7 = 3;
LAB_006beda9:
      cm_fn_00702050(local_8,iVar8,local_4,uVar7,0,0);
    }
    else {
      local_10._1_3_ = (int3)(cVar2 >> 7);
      if (*(char *)(param_1 + 0x8e9e) == '\0') {
        pcVar1[0x19a] = '\0';
        pcVar1[0x19b] = '\0';
        local_10 = CONCAT31(local_10._1_3_,4);
        if ('\x03' < pcVar1[0x2d]) {
          if (pcVar1[0x2c] == '\0') {
            cVar4 = cm_fn_0090cfc0(2);
            local_10 = CONCAT31(local_10._1_3_,'\x04' - cVar4);
          }
          else {
            cVar4 = cm_fn_0090cfc0(2);
            local_10 = CONCAT31(local_10._1_3_,cVar4 + '\x04');
          }
        }
        cm_fn_006f46b0(local_10,7);
        cm_fn_006f46b0(4,0xb);
        pcVar11 = (char *)(param_1 + 0x4796);
        local_10 = 0x14;
        do {
          if (((pcVar11 != pcVar1) && (pcVar11 != pcVar6)) && (-1 < pcVar11[0x19])) {
            cm_fn_006f46b0(0,0);
            cm_fn_007160e0(10);
          }
          if (((pcVar11 + 0x22d8 != pcVar1) && (pcVar11 + 0x22d8 != pcVar6)) &&
             (-1 < pcVar11[0x22f1])) {
            cm_fn_006f46b0(0,0);
            cm_fn_007160e0(10);
          }
          pcVar11 = pcVar11 + 0x1be;
          local_10 = local_10 + -1;
        } while (local_10 != 0);
      }
      else {
        pcVar1[0x19a] = -0x4c;
        pcVar1[0x19b] = '\0';
        local_10 = CONCAT31(local_10._1_3_,4);
        if ('\x03' < pcVar1[0x2d]) {
          if (pcVar1[0x2c] == '\0') {
            cVar4 = cm_fn_0090cfc0(2);
            local_10 = CONCAT31(local_10._1_3_,cVar4 + '\x04');
          }
          else {
            cVar4 = cm_fn_0090cfc0(2);
            local_10 = CONCAT31(local_10._1_3_,'\x04' - cVar4);
          }
        }
        cm_fn_006f46b0(local_10,4);
        cm_fn_006f46b0(4,0);
        pcVar11 = (char *)(param_1 + 0x4796);
        local_10 = 0x14;
        do {
          if (((pcVar11 != pcVar1) && (pcVar11 != pcVar6)) && (-1 < pcVar11[0x19])) {
            cm_fn_006f46b0(8,0xb);
            cm_fn_007160e0(10);
          }
          if (((pcVar11 + 0x22d8 != pcVar1) && (pcVar11 + 0x22d8 != pcVar6)) &&
             (-1 < pcVar11[0x22f1])) {
            cm_fn_006f46b0(8,0xb);
            cm_fn_007160e0(10);
          }
          pcVar11 = pcVar11 + 0x1be;
          local_10 = local_10 + -1;
        } while (local_10 != 0);
      }
      sVar5 = cm_fn_0090cfc0(local_14);
      sVar5 = 200 - sVar5;
      *(short *)(pcVar1 + 0x19c) = sVar5;
      if (sVar5 < 0xc9) {
        if (sVar5 < -200) {
          pcVar1[0x19c] = '8';
          pcVar1[0x19d] = -1;
        }
      }
      else {
        pcVar1[0x19c] = -0x38;
        pcVar1[0x19d] = '\0';
      }
      cVar4 = '\0';
      local_15 = '\0';
      do {
        cm_fn_00714d40();
        if ((*(char *)(param_1 + 0x8eb2) == '\0') && (*(char *)(param_1 + 0x8ea9) != -1)) {
          cVar4 = cm_fn_007109f0(4);
        }
        cm_fn_007160d0(0xffffffff);
        cm_fn_007160d0(0xffffffff);
      } while (((*(char *)(param_1 + 0x8eb2) == '\0') && (*(char *)(param_1 + 0x8ea9) != -1)) &&
              ((local_15 < '\x04' &&
               ((cVar4 == '\0' &&
                (local_15 = local_15 + '\x01', *(char **)(param_1 + 0xf582) == pcVar1))))));
      if ((*(char **)(param_1 + 0xf582) == pcVar1) &&
         ((*(char *)(param_1 + 0x8eb2) == '\0' && (*(char *)(param_1 + 0x8ea9) != -1)))) {
        local_8 = CONCAT31(local_8._1_3_,0xff);
        sVar5 = cm_fn_00700bd0(&local_8,&local_4);
        iVar8 = (int)sVar5;
        uVar7 = 4;
        goto LAB_006beda9;
      }
    }
    *(char *)(*(char *)(param_1 + 0x8e9e) + 0x8ea1 + param_1) =
         *(char *)(*(char *)(param_1 + 0x8e9e) + 0x8ea1 + param_1) + -1;
    *(u8 *)(param_1 + 0x8e9e) = *(char *)(param_1 + 0x8e9e) == '\0';
    if (((int)((int)*(char *)(param_1 + 0x8ea2) + (u32)*(u8 *)(param_1 + 0xf5f2)) <
         (int)(u32)*(u8 *)(param_1 + 0xf5bc)) ||
       ((int)((int)*(char *)(param_1 + 0x8ea1) + (u32)*(u8 *)(param_1 + 0xf5bc)) <
        (int)(u32)*(u8 *)(param_1 + 0xf5f2))) {
      *(u8 *)(param_1 + 0x8eb3) = ((param_2 != '\x04') - 1U & 0xfd) + 6;
      return;
    }
    break;
  case 3:
  case 6:
    cm_fn_006d6f70(0x2004,param_1,0xffffffff,
                 *(u8 *)(param_1 + 0xf5bc) <= *(u8 *)(param_1 + 0xf5f2),0xffffffff,0xffffffff,0)
    ;
    *(u8 *)(*(int *)(param_1 + 0x4792) + 0x49) = *(u8 *)(param_1 + 0xf5bc);
    *(u8 *)(*(int *)(param_1 + 0x4792) + 0x4a) = *(u8 *)(param_1 + 0xf5f2);
    *(u8 *)(param_1 + 0x8eb3) = 0;
    **(u8 **)(param_1 + 0x8f7a) = *(u8 *)(param_1 + 0x8ea4);
    break;
  case 4:
    *(u16 *)(param_1 + 0x475f) = 0xffff;
    *(u32 *)(param_1 + 0x4761) = 0;
    *(u32 *)(param_1 + 0x4765) = 0;
    *(u8 *)(param_1 + 0x4769) = 0;
    *(u8 *)(param_1 + 0x475a) = 0;
    *(u8 *)(param_1 + 0x475b) = 0xff;
    *(u8 *)(param_1 + 0x475c) = 0xff;
    *(u8 *)(param_1 + 0x475d) = 0xff;
    *(u8 *)(param_1 + 0x475e) = 0xff;
    *(u8 *)(param_1 + 0x8ea4) = **(u8 **)(param_1 + 0x8f7a);
    **(u8 **)(param_1 + 0x8f7a) = 0;
    uVar3 = cm_fn_0090cfc0(2);
    *(u8 *)(param_1 + 0x8e9e) = uVar3;
    *(u8 *)(param_1 + 0x1c5) = 0;
    *(u32 *)(param_1 + 0x477e) = 0;
    *(u8 *)(param_1 + 0x8eaa) = 0;
    *(u8 *)(param_1 + 0x8ea9) = 0;
    *(u32 *)(param_1 + 0xf5aa) = 0;
    *(u32 *)(param_1 + 0xf5a6) = 0;
    *(u32 *)(param_1 + 0xf576) = 0;
    *(u32 *)(param_1 + 0xf57a) = 0;
    *(u8 *)(param_1 + 0x8ea6) = 0xff;
    *(u8 *)(param_1 + 0x8eb9) = 0xff;
    *(u16 *)(param_1 + 0x8ed6) = 0;
    *(u32 *)(param_1 + 0x91de) = 0;
    *(u8 *)(param_1 + 0x8ea2) = 5;
    *(u8 *)(param_1 + 0x8ea1) = 5;
    *(u8 *)(param_1 + 0x8ea0) = 0;
    *(u8 *)(param_1 + 0x8e9f) = 0;
    cm_fn_006be520();
    *(u8 *)(param_1 + 0x8ea3) = 0;
    *(u8 *)(param_1 + 0x8eb3) = 5;
    return;
  }
  return;
}
