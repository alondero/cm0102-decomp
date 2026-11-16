/* Draft for preferred-image VA 0x0062b7b0, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
/* The original decrements two separate 16-bit fields; volatile word accesses
   preserve their width and fault order at both optimization levels. */
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_009e7ad9 CM_GLOBAL(u8, 0x009e7ad9)
#define DAT_009e7f05 CM_GLOBAL(u8, 0x009e7f05)
#define DAT_009e7f06 CM_GLOBAL(u8, 0x009e7f06)
#define DAT_00ae2364 CM_GLOBAL(volatile s32, 0x00ae2364)
#define DAT_00ae23bc CM_GLOBAL(volatile s32, 0x00ae23bc)
#define DAT_00ae2c90 CM_GLOBAL(volatile u8, 0x00ae2c90)
#define DAT_00dd6ec8 CM_GLOBAL(volatile u32, 0x00dd6ec8)
#define DAT_00dd6ecc CM_GLOBAL(volatile u32, 0x00dd6ecc)
extern /* WARNING: Globals starting with '_' overlap smaller symbols at the same address */ u32 CM_THISCALL cm_fn_00418f60(int param_1,u32 *param_2,int param_3);
extern u32 CM_THISCALL cm_fn_00456770(short *self,u32 param_1,u32 param_2);
extern u16 CM_FASTCALL cm_fn_0054a3f0(short *param_1);
extern void CM_THISCALL cm_fn_0062c680(int *param_1,int *param_2);
extern char CM_THISCALL cm_fn_0062d140(int *self,char param_1,int param_2,int param_3);
extern int CM_THISCALL cm_fn_0062d250(int *self,u32 param_1,int param_2,int param_3,int param_4,int param_5,int param_6,u32 param_7,int param_8);
extern void CM_FASTCALL cm_fn_0062e010(int *param_1);
extern void CM_FASTCALL cm_fn_0062e9e0(int *param_1);
extern void CM_FASTCALL cm_fn_0068d040(int param_1);
extern int CM_STDCALL cm_fn_0062ce30(int param_1);
extern int cm_fn_0090cfc0(int param_1);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);

void CM_FASTCALL cm_fn_0062b7b0(int *param_1)

{
  char *pcVar1;
  char cVar2;
  u8 uVar3;
  char cVar4;
  short sVar5;
  volatile u16 *psVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  u32 uVar12;
  int local_18;
  short local_8;
  
  iVar10 = param_1[10];
  if (iVar10 != 0) {
    cm_fn_0068d040(iVar10);
    cm_fn_00944c94((void *)iVar10);
    param_1[10] = 0;
  }
  psVar6 = (volatile u16 *)((u32)*(volatile u32 *)&param_1[1] + 0x2e);
  iVar10 = 0;
  if (0 < *(volatile s32 *)&param_1[2]) {
    do {
      psVar6[-1] = psVar6[-1] + -1;
      *psVar6 = *psVar6 + -1;
      iVar10 = iVar10 + 1;
      psVar6 = psVar6 + 0x18;
    } while (iVar10 < *(volatile s32 *)&param_1[2]);
  }
  iVar10 = cm_fn_00456770((short *)0x00ae2c90,DAT_00dd6ec8,DAT_00dd6ecc);
  if (((iVar10 != 0) && (CM_GLOBAL(volatile u16, 0x00ae2c90) == 0xb4)) &&
     (iVar11 = 0, iVar10 = DAT_00ae23bc, 0 < DAT_00ae2364)) {
    do {
      cm_fn_0062c680(param_1,(int *)iVar10);
      iVar11 = iVar11 + 1;
      iVar10 = iVar10 + 0x6e;
    } while (iVar11 < DAT_00ae2364);
  }
  sVar5 = cm_fn_0054a3f0((short *)0x00ae2c90);
  if ((sVar5 != 6) && (sVar5 = cm_fn_0054a3f0((short *)0x00ae2c90), sVar5 != 5)) {
    sVar5 = ((short)(DAT_00ae2364 / 0x96) + (short)(DAT_00ae2364 >> 0x1f)) -
            (short)((long long)DAT_00ae2364 * 0x1b4e81b5 >> 0x3f);
    if (sVar5 < 10) {
      sVar5 = 10;
    }
    local_18 = 0;
    local_8 = sVar5 / 0xe;
    if (0 < sVar5) {
      do {
        iVar11 = cm_fn_0090cfc0(DAT_00ae2364);
        iVar7 = iVar11 * 0x6e;
        iVar10 = iVar7 + DAT_00ae23bc;
        if (*(int *)(iVar7 + 0x61 + DAT_00ae23bc) == 0) {
          local_18 = local_18 + -1;
        }
        else {
          cVar2 = *(char *)(iVar10 + 0x3d);
          if ((((cVar2 == '\v') || (cVar2 == '\x0f')) || (cVar2 == '\r')) &&
             (cVar2 = cm_fn_00418f60(CM_GLOBAL(volatile u32, 0x00ad9c6c),(u32 *)iVar10,0), cVar2 == '\0')) {
            if (*(char *)(*(int *)(iVar10 + 0x61) + 0xf) == '\x14') {
              iVar8 = cm_fn_0090cfc0(5);
              iVar9 = cm_fn_0090cfc0(2);
              if (iVar9 < iVar8) goto LAB_0062bc92;
            }
            iVar11 = iVar11 * 0x1f;
            if (*(int *)(*param_1 + iVar11 + 0xe) == -1) {
              if (*(char *)(*param_1 + iVar11 + 0x12) < '\0') {
                iVar8 = cm_fn_0090cfc0(0xa0);
                if (iVar8 <= ((*(char *)(*(int *)(iVar10 + 0x61) + 0x2d) + -3) * 5 +
                             (int)*(char *)(*param_1 + 0x1e + iVar11)) * 2) goto LAB_0062b9bb;
              }
              else {
                iVar8 = *param_1;
                iVar9 = cm_fn_0090cfc0(100);
                if (iVar9 <= ((*(char *)(*(int *)(iVar10 + 0x61) + 0x2d) + -3) * 5 +
                              (int)*(char *)(iVar8 + iVar11 + 0x1e) +
                             (int)*(char *)(iVar8 + iVar11 + 0x13)) * 2) {
LAB_0062b9bb:
                  iVar8 = cm_fn_0090cfc0(3);
                  if ((iVar8 == 0) &&
                     (iVar8 = *param_1 + iVar11, '\x13' < *(char *)(*param_1 + 0x1e + iVar11))) {
                    if (*(char *)(iVar8 + 0x12) < '\0') {
LAB_0062ba19:
                      do {
                        cVar2 = cm_fn_0090cfc0(0xc);
                        if (cVar2 == '\0') {
                          iVar8 = cm_fn_0090cfc0(100);
                          if (iVar8 != 0) goto LAB_0062ba19;
                        }
                        iVar8 = cm_fn_0090cfc0(((int)(char)(&DAT_009e7f06)[cVar2 * 3] -
                                             (int)(char)(&DAT_009e7f05)[cVar2 * 3]) + 2);
                      } while (iVar8 == 0);
                    }
                    else {
                      iVar8 = cm_fn_0090cfc0(*(char *)(iVar8 + 0x13) + 2);
                      iVar9 = cm_fn_0090cfc0(3);
                      if (iVar8 <= iVar9) goto LAB_0062ba19;
                      cVar2 = (&DAT_009e7ad9)[*(char *)(*param_1 + 0x12 + iVar11) * 0xd];
                    }
                    if ((*(char *)(*(int *)(iVar10 + 0x61) + 0xf) < '\x0f') && (cVar2 == '\a')) {
                      local_18 = local_18 + -1;
                    }
                    else {
                      iVar8 = cm_fn_0062ce30(iVar10);
                      if (iVar8 == 0) {
                        if (local_18 < local_8) {
                          uVar12 = 0xffffffff;
                        }
                        else {
                          uVar12 = 0;
                        }
                        uVar3 = cm_fn_0062d140(param_1,cVar2,(int)uVar12,iVar7 + DAT_00ae23bc);
                        switch(uVar3) {
                        case 0xc:
                        case 0x17:
                        case 0x1a:
                        case 0x1b:
                        case 0x1f:
                          cVar4 = *(char *)(*(int *)(iVar10 + 0x61) + 0x1b);
                          if (cVar4 == '\x14') {
                            cVar4 = *(char *)(*param_1 + 0x12 + iVar11);
                            if ((cVar4 < '\0') || ((&DAT_009e7ad9)[cVar4 * 0xd] != cVar2)) {
                              iVar8 = cm_fn_0090cfc0(3);
                              if (iVar8 != 0) {
                                iVar8 = *(int *)(iVar10 + 0x61);
                                cVar4 = cm_fn_0090cfc0(2);
                                *(char *)(iVar8 + 0x1b) = *(char *)(iVar8 + 0x1b) + (-1 - cVar4);
                              }
                            }
                            else {
                              iVar8 = *(int *)(iVar10 + 0x61);
                              cVar4 = cm_fn_0090cfc0(3);
                              *(char *)(iVar8 + 0x1b) = *(char *)(iVar8 + 0x1b) + (-1 - cVar4);
                            }
                          }
                          else if (('\x0f' < cVar4) && (iVar8 = cm_fn_0090cfc0(3), iVar8 == 0)) {
                            pcVar1 = (char *)(*(int *)(iVar10 + 0x61) + 0x1b);
                            *pcVar1 = *pcVar1 + -1;
                          }
                          if (*(char *)(*(int *)(iVar10 + 0x61) + 0x36) == '\x14') {
                            cVar4 = *(char *)(*param_1 + 0x12 + iVar11);
                            if ((cVar4 < '\0') || ((&DAT_009e7ad9)[cVar4 * 0xd] != cVar2)) {
                              iVar11 = cm_fn_0090cfc0(3);
                              if (iVar11 != 0) {
                                pcVar1 = (char *)(*(int *)(iVar10 + 0x61) + 0x36);
                                *pcVar1 = *pcVar1 + -1;
                              }
                            }
                            else {
                              iVar10 = *(int *)(iVar10 + 0x61);
                              cVar2 = cm_fn_0090cfc0(3);
                              *(char *)(iVar10 + 0x36) = *(char *)(iVar10 + 0x36) + (-1 - cVar2);
                            }
                          }
                          else if ('\x11' < *(char *)(*(int *)(iVar10 + 0x61) + 0x1b))
                          goto LAB_0062bbf9;
                          break;
                        case 0x14:
                          goto switchD_0062bad5_caseD_14;
                        case 0x1c:
                          cVar4 = *(char *)(*param_1 + 0x12 + iVar11);
                          if (((-1 < cVar4) && ((&DAT_009e7ad9)[cVar4 * 0xd] == cVar2)) &&
                             (*(char *)(*(int *)(iVar10 + 0x61) + 0x1b) == '\x14')) {
LAB_0062bbf9:
                            iVar11 = cm_fn_0090cfc0(3);
                            if (iVar11 == 0) {
                              pcVar1 = (char *)(*(int *)(iVar10 + 0x61) + 0x1b);
                              *pcVar1 = *pcVar1 + -1;
                            }
                          }
                          break;
                        case 0x2d:
                          cVar2 = *(char *)(*(int *)(iVar10 + 0x61) + 0x1d);
                          if (cVar2 == '\x14') {
                            *(u8 *)(*(int *)(iVar10 + 0x61) + 0x1d) = 0x13;
                          }
                          else if ('\x0f' < cVar2) {
                            iVar11 = cm_fn_0090cfc0(3);
joined_r0x0062bc5f:
                            if (iVar11 == 0) goto LAB_0062bc61;
                          }
                          break;
                        case 0x3b:
                        case 0x3c:
                          cVar2 = *(char *)(*(int *)(iVar10 + 0x61) + 0x1d);
                          if (cVar2 == '\x14') {
                            iVar11 = cm_fn_0090cfc0(3);
                            if (iVar11 != 0) {
LAB_0062bc61:
                              pcVar1 = (char *)(*(int *)(iVar10 + 0x61) + 0x1d);
                              *pcVar1 = *pcVar1 + -1;
                            }
                          }
                          else if ('\x0f' < cVar2) {
                            iVar11 = cm_fn_0090cfc0(3);
                            goto joined_r0x0062bc5f;
                          }
                        }
                        cm_fn_0062d250(param_1,uVar3,0,iVar7 + DAT_00ae23bc,0,1,0xffffffff,0,0);
                      }
                    }
switchD_0062bad5_caseD_14:
                  }
                }
              }
            }
          }
        }
LAB_0062bc92:
        local_18 = local_18 + 1;
      } while (local_18 < sVar5);
    }
  }
  cm_fn_0062e010(param_1);
  cm_fn_0062e9e0(param_1);
  return;
}
