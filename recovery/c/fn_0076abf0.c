/* Draft for preferred-image VA 0x0076abf0, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
/* WIP: O2 memory page mismatches remain after five failed test iterations. */
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define CONCAT31(a, b) ((u32)(((u32)(u32)(a) << 8) | (u32)(u8)(b)))
#define DAT_009cf434 CM_GLOBAL(int, 0x009cf434)
#define DAT_00ae2350 CM_GLOBAL(volatile s32, 0x00ae2350)
#define DAT_00ae2364 CM_GLOBAL(volatile s32, 0x00ae2364)
#define DAT_00ae23bc CM_GLOBAL(volatile s32, 0x00ae23bc)
extern u32 cm_fn_0053b620(char param_1);
extern u32 CM_STDCALL cm_fn_0076d810(int param_1,short *param_2,char param_3);
extern void cm_fn_00945333(u32 *param_1,u32 *param_2,u32 param_3,void *param_4);

void CM_FASTCALL cm_fn_0076abf0(int param_1)

{
  u8 bVar1;
  u8 bVar2;
  char cVar3;
  short sVar4;
  char cVar5;
  u32 *puVar6;
  u8 *pbVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int *piVar13;
  int iVar14;
  int iVar15;
  u32 *local_24;
  u8 *local_1c;
  int local_18;
  int local_10;
  int local_c;
  int local_8;
  
  iVar15 = 0;
  if (0 < DAT_00ae2350) {
    iVar14 = 0;
    do {
      iVar8 = 4;
      iVar12 = iVar14;
      do {
        iVar8 = iVar8 + -1;
        *(u8 *)(iVar12 + 0xd + *(int *)(param_1 + 4)) = 0;
        *(u16 *)(iVar12 + 0xf + *(int *)(param_1 + 4)) = 10000;
        *(u8 *)(iVar12 + 0x22 + *(int *)(param_1 + 4)) = 0;
        *(u16 *)(iVar12 + 0x24 + *(int *)(param_1 + 4)) = 10000;
        iVar12 = iVar12 + 4;
      } while (iVar8 != 0);
      iVar15 = iVar15 + 1;
      iVar14 = iVar14 + 0x6e;
    } while (iVar15 < DAT_00ae2350);
  }
  local_8 = 0;
  iVar15 = DAT_00ae23bc;
  if (0 < DAT_00ae2364) {
    do {
      if (((((*(int *)(iVar15 + 0x1a) != 0) && (*(int *)(iVar15 + 0x61) != 0)) &&
           ('\x10' < *(char *)(iVar15 + 0x18))) &&
          ((*(int *)(iVar15 + 0x24) == 0 || (*(char *)(iVar15 + 0x28) == '\v')))) &&
         ((iVar14 = cm_fn_0053b620(*(char *)(iVar15 + 0x3d)), iVar14 != 0 &&
          (*(int *)(iVar15 + 0x39) != 0)))) {
        local_c = 0;
        do {
          if (local_c == 0) {
            iVar14 = **(int **)(iVar15 + 0x1a);
LAB_0076acf8:
            iVar14 = *(int *)(param_1 + 4) + iVar14 * 0x6e;
            cVar5 = cm_fn_0076d810(iVar15,(short *)iVar14,0);
            iVar12 = (int)cVar5;
            bVar1 = *(u8 *)(iVar14 + 0xd + iVar12 * 4);
            bVar2 = *(u8 *)(iVar14 + 0xe + iVar12 * 4);
            if (bVar1 < bVar2) {
              local_24 = *(u32 **)(iVar14 + 8);
              if ('\0' < cVar5) {
                local_1c = (u8 *)(iVar14 + 0xe);
                local_18 = iVar12;
                do {
                  local_24 = (u32 *)((int)local_24 + (u32)*local_1c);
                  local_1c = local_1c + 4;
                  local_18 = local_18 + -1;
                } while (local_18 != 0);
              }
              *(int *)(*(int *)(param_1 + 0xc) + ((u32)bVar1 + (int)local_24) * 4) = iVar15;
              *(char *)(iVar14 + 0xd + iVar12 * 4) = *(char *)(iVar14 + 0xd + iVar12 * 4) + '\x01';
              sVar4 = *(short *)(*(int *)(iVar15 + 0x61) + 9);
              if (sVar4 < *(short *)(iVar14 + 0xf + iVar12 * 4)) {
                *(short *)(iVar14 + 0xf + iVar12 * 4) = sVar4;
              }
            }
            else {
              sVar4 = *(short *)(*(int *)(iVar15 + 0x61) + 9);
              if (*(short *)(iVar14 + 0xf + iVar12 * 4) < sVar4) {
                iVar8 = *(int *)(iVar14 + 8);
                if ('\0' < cVar5) {
                  pbVar7 = (u8 *)(iVar14 + 0xe);
                  local_1c = (u8 *)iVar12;
                  do {
                    bVar1 = *pbVar7;
                    pbVar7 = pbVar7 + 4;
                    iVar8 = iVar8 + (u32)bVar1;
                    local_1c = (u8 *)((int)local_1c + -1);
                  } while (local_1c != (u8 *)0x0);
                }
                iVar10 = (bVar2 - 2) + iVar8;
                piVar13 = (int *)(*(int *)(param_1 + 0xc) + iVar10 * 4);
                for (iVar11 = iVar10; iVar9 = iVar8, iVar8 <= iVar11; iVar11 = iVar11 + -1) {
                  if (sVar4 < *(short *)(*(int *)(*piVar13 + 0x61) + 9)) {
                    iVar9 = iVar11 + 1;
                    break;
                  }
                  piVar13 = piVar13 + -1;
                }
                local_24 = (u32 *)(*(int *)(param_1 + 0xc) + iVar10 * 4);
                if (iVar9 <= iVar10) {
                  local_10 = (iVar10 - iVar9) + 1;
                  puVar6 = (u32 *)(*(int *)(param_1 + 0xc) + 4 + iVar10 * 4);
                  do {
                    *puVar6 = *local_24;
                    local_24 = local_24 + -1;
                    local_10 = local_10 + -1;
                    puVar6 = puVar6 + -1;
                  } while (local_10 != 0);
                }
                *(int *)(*(int *)(param_1 + 0xc) + iVar9 * 4) = iVar15;
                *(u16 *)(iVar14 + 0xf + iVar12 * 4) =
                     *(u16 *)
                      (*(int *)(*(int *)(*(int *)(param_1 + 0xc) + -4 +
                                        ((u32)*(u8 *)(iVar14 + 0xe + iVar12 * 4) + iVar8) * 4) +
                               0x61) + 9);
              }
            }
            cVar3 = *(char *)(iVar14 + 0xe + iVar12 * 4);
            if (*(char *)(iVar14 + 0xd + iVar12 * 4) == cVar3) {
              iVar8 = *(int *)(iVar14 + 8);
              if ('\0' < cVar5) {
                pbVar7 = (u8 *)(iVar14 + 0xe);
                do {
                  bVar1 = *pbVar7;
                  pbVar7 = pbVar7 + 4;
                  iVar8 = iVar8 + (u32)bVar1;
                  iVar12 = iVar12 + -1;
                } while (iVar12 != 0);
              }
                cm_fn_00945333((u32 *)(*(int *)(param_1 + 0xc) + iVar8 * 4),
                               (u32 *)(u32)(u8)cVar3,4,(void *)0x00778ed0);
            }
            if ((*(char *)(iVar15 + 0x18) < '\x16') && (*(u8 *)(iVar15 + 0x22) < 5)) {
              cVar5 = cm_fn_0076d810(iVar15,(short *)iVar14,1);
              iVar12 = (int)cVar5;
              bVar1 = *(u8 *)(iVar14 + 0x22 + iVar12 * 4);
              bVar2 = *(u8 *)(iVar14 + 0x23 + iVar12 * 4);
              if (bVar1 < bVar2) {
                local_24 = *(u32 **)(iVar14 + 0x1d);
                if ('\0' < cVar5) {
                  local_1c = (u8 *)(iVar14 + 0x23);
                  local_10 = iVar12;
                  do {
                    local_24 = (u32 *)((int)local_24 + (u32)*local_1c);
                    local_1c = local_1c + 4;
                    local_10 = local_10 + -1;
                  } while (local_10 != 0);
                }
                *(int *)(*(int *)(param_1 + 0x10) + ((u32)bVar1 + (int)local_24) * 4) = iVar15;
                *(char *)(iVar14 + 0x22 + iVar12 * 4) =
                     *(char *)(iVar14 + 0x22 + iVar12 * 4) + '\x01';
                sVar4 = *(short *)(*(int *)(iVar15 + 0x61) + 9);
                if (sVar4 < *(short *)(iVar14 + 0x24 + iVar12 * 4)) {
                  *(short *)(iVar14 + 0x24 + iVar12 * 4) = sVar4;
                }
              }
              else {
                sVar4 = *(short *)(*(int *)(iVar15 + 0x61) + 9);
                if (*(short *)(iVar14 + 0x24 + iVar12 * 4) < sVar4) {
                  iVar8 = *(int *)(iVar14 + 0x1d);
                  if ('\0' < cVar5) {
                    pbVar7 = (u8 *)(iVar14 + 0x23);
                    iVar10 = iVar12;
                    do {
                      bVar1 = *pbVar7;
                      pbVar7 = pbVar7 + 4;
                      iVar8 = iVar8 + (u32)bVar1;
                      iVar10 = iVar10 + -1;
                    } while (iVar10 != 0);
                  }
                  iVar10 = (bVar2 - 2) + iVar8;
                  piVar13 = (int *)(*(int *)(param_1 + 0x10) + iVar10 * 4);
                  for (iVar11 = iVar10; iVar9 = iVar8, iVar8 <= iVar11; iVar11 = iVar11 + -1) {
                    if (sVar4 < *(short *)(*(int *)(*piVar13 + 0x61) + 9)) {
                      iVar9 = iVar11 + 1;
                      break;
                    }
                    piVar13 = piVar13 + -1;
                  }
                  local_24 = (u32 *)(*(int *)(param_1 + 0x10) + iVar10 * 4);
                  if (iVar9 <= iVar10) {
                    local_10 = (iVar10 - iVar9) + 1;
                    puVar6 = (u32 *)(*(int *)(param_1 + 0x10) + 4 + iVar10 * 4);
                    do {
                      *puVar6 = *local_24;
                      local_24 = local_24 + -1;
                      local_10 = local_10 + -1;
                      puVar6 = puVar6 + -1;
                    } while (local_10 != 0);
                  }
                  *(int *)(*(int *)(param_1 + 0x10) + iVar9 * 4) = iVar15;
                  *(u16 *)(iVar14 + 0x24 + iVar12 * 4) =
                       *(u16 *)
                        (*(int *)(*(int *)(*(int *)(param_1 + 0x10) + -4 +
                                          ((u32)*(u8 *)(iVar14 + 0x23 + iVar12 * 4) + iVar8) * 4)
                                 + 0x61) + 9);
                }
              }
              cVar3 = *(char *)(iVar14 + 0x23 + iVar12 * 4);
              if (*(char *)(iVar14 + 0x22 + iVar12 * 4) == cVar3) {
                iVar8 = *(int *)(iVar14 + 0x1d);
                if ('\0' < cVar5) {
                  pbVar7 = (u8 *)(iVar14 + 0x23);
                  do {
                    bVar1 = *pbVar7;
                    pbVar7 = pbVar7 + 4;
                    iVar8 = iVar8 + (u32)bVar1;
                    iVar12 = iVar12 + -1;
                  } while (iVar12 != 0);
                }
                cm_fn_00945333((u32 *)(*(int *)(param_1 + 0x10) + iVar8 * 4),
                               (u32 *)(u32)(u8)cVar3,4,(void *)0x00778ed0);
              }
            }
          }
          else {
            piVar13 = *(int **)(iVar15 + 0x1e);
            if (((piVar13 != (int *)0x0) && (*(char *)(iVar15 + 0x22) == '\0')) &&
               ((piVar13 != *(int **)(iVar15 + 0x1a) && (iVar14 = *piVar13, iVar14 != DAT_009cf434))
               )) goto LAB_0076acf8;
          }
          local_c = local_c + 1;
        } while (local_c < 2);
      }
      local_8 = local_8 + 1;
      iVar15 = iVar15 + 0x6e;
    } while (local_8 < DAT_00ae2364);
  }
  return;
}
