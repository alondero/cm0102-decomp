/* WIP: O2 indirect slot target mismatch, case 23 (original 0xffff89b7, recovered 0x00007e0d); iteration cap reached. */
/* Draft for preferred-image VA 0x0051e870, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0

typedef u32 (CM_THISCALL *cm_reorder_fn)(void *self, u32 first, u32 second);

void CM_THISCALL cm_fn_0051e870(int *param_1,short param_2,short param_3)

{
  u16 *puVar1;
  u16 uVar2;
  short sVar3;
  u16 uVar4;
  u16 uVar5;
  u16 uVar6;
  short sVar7;
  short sVar8;
  int *piVar9;
  short *psVar10;
  int iVar11;
  int iVar12;
  short sVar13;
  int iVar14;
  int iVar15;
  int _param_2;
  
  if (param_3 == -1) {
    param_3 = 0;
    sVar13 = *(short *)(*(int *)((int)param_1 + 0xa3) + 0x1a + param_2 * 0x68);
  }
  else {
    sVar13 = param_3 + 1;
  }
  iVar15 = (int)param_2;
  uVar2 = *(u16 *)(*(int *)((int)param_1 + 0xa3) + 0xb + iVar15 * 0x68);
  if ((uVar2 & 1) == 0) {
    if ((uVar2 & 8) == 0) {
      if ((param_1[9] != -1) && (param_3 < sVar13)) {
        iVar14 = param_3 * 0x35;
        _param_2 = (int)sVar13 - (int)param_3;
        do {
          iVar12 = *(int *)(*(int *)((int)param_1 + 0xad) + iVar15 * 4);
          sVar13 = *(short *)(iVar12 + iVar14);
          psVar10 = (short *)(iVar12 + iVar14);
          if ((((sVar13 != -1) && (sVar3 = psVar10[1], sVar3 != -1)) &&
              (iVar12 = *(int *)(*(int *)((int)param_1 + 0xa7) + sVar3 * 6), iVar12 != 0)) &&
             ((piVar9 = *(int **)(iVar12 + 0x69), piVar9 != (int *)0x0 && (*piVar9 == param_1[9]))))
          {
            sVar7 = psVar10[7];
            sVar8 = psVar10[5];
            *psVar10 = sVar3;
            iVar12 = *(int *)(*(int *)((int)param_1 + 0xad) + iVar15 * 4) + iVar14;
            *(u16 *)(iVar12 + 0xe) = *(u16 *)(iVar12 + 0x10);
            iVar12 = *(int *)(*(int *)((int)param_1 + 0xad) + iVar15 * 4) + iVar14;
            *(u16 *)(iVar12 + 10) = *(u16 *)(iVar12 + 0xc);
            *(short *)(*(int *)(*(int *)((int)param_1 + 0xad) + iVar15 * 4) + 2 + iVar14) = sVar13;
            *(short *)(*(int *)(*(int *)((int)param_1 + 0xad) + iVar15 * 4) + 0x10 + iVar14) = sVar7
            ;
            *(short *)(*(int *)(*(int *)((int)param_1 + 0xad) + iVar15 * 4) + 0xc + iVar14) = sVar8;
          }
          iVar14 = iVar14 + 0x35;
          _param_2 = _param_2 + -1;
        } while (_param_2 != 0);
      }
    }
    else if (param_3 < sVar13) {
      iVar14 = param_3 * 0x35;
      _param_2 = (int)sVar13 - (int)param_3;
      do {
        iVar12 = *(int *)(*(int *)((int)param_1 + 0xad) + iVar15 * 4);
        sVar13 = *(short *)(iVar12 + iVar14);
        puVar1 = (u16 *)(iVar12 + iVar14);
        if ((sVar13 != -1) && ((s16)puVar1[1] != -1)) {
          if (*(char *)(*(int *)((int)param_1 + 0xa7) + 4 + sVar13 * 6) <
              *(char *)(*(int *)((int)param_1 + 0xa7) + 4 + (short)puVar1[1] * 6)) {
            uVar4 = puVar1[7];
            uVar5 = puVar1[5];
            *puVar1 = puVar1[1];
            iVar12 = *(int *)(*(int *)((int)param_1 + 0xad) + iVar15 * 4);
            *(u16 *)(iVar12 + iVar14 + 0xe) = *(u16 *)(iVar12 + 0x10 + iVar14);
            iVar12 = *(int *)(*(int *)((int)param_1 + 0xad) + iVar15 * 4);
            *(u16 *)(iVar12 + iVar14 + 10) = *(u16 *)(iVar12 + 0xc + iVar14);
            *(short *)(*(int *)(*(int *)((int)param_1 + 0xad) + iVar15 * 4) + 2 + iVar14) = sVar13;
            *(u16 *)(*(int *)(*(int *)((int)param_1 + 0xad) + iVar15 * 4) + 0x10 + iVar14) =
                 uVar4;
            *(u16 *)(*(int *)(*(int *)((int)param_1 + 0xad) + iVar15 * 4) + 0xc + iVar14) =
                 uVar5;
          }
        }
        iVar14 = iVar14 + 0x35;
        _param_2 = _param_2 + -1;
      } while (_param_2 != 0);
      return;
    }
  }
  else if (param_3 < sVar13) {
    iVar14 = param_3 * 0x35;
    _param_2 = (int)sVar13 - (int)param_3;
    do {
      psVar10 = (short *)(*(int *)(*(int *)((int)param_1 + 0xad) + iVar15 * 4) + iVar14);
      sVar13 = *psVar10;
      if ((sVar13 != -1) && (sVar3 = psVar10[1], sVar3 != -1)) {
        u32 table_base = *(volatile u32 *)((u32)param_1 + 0xa7);
        u32 first_value = *(volatile u32 *)(table_base + (u32)(s32)sVar13 * 6u);
        iVar12 = (int)*(volatile u32 *)(table_base + (u32)(s32)sVar3 * 6u);
        u32 object_vtable = *(volatile u32 *)param_1;
        u32 virtual_target = *(volatile u32 *)(object_vtable + 0x90u);
        iVar11 = ((cm_reorder_fn)virtual_target)(param_1, first_value, (u32)iVar12);
        if ((iVar11 != 0) && (iVar11 == iVar12)) {
          iVar12 = *(int *)(*(int *)((int)param_1 + 0xad) + iVar15 * 4);
          uVar4 = *(u16 *)(iVar12 + iVar14);
          uVar5 = *(u16 *)(iVar12 + 10 + iVar14);
          puVar1 = (u16 *)(iVar12 + iVar14);
          uVar6 = puVar1[7];
          *puVar1 = puVar1[1];
          iVar12 = *(int *)(*(int *)((int)param_1 + 0xad) + iVar15 * 4) + iVar14;
          *(u16 *)(iVar12 + 0xe) = *(u16 *)(iVar12 + 0x10);
          iVar12 = *(int *)(*(int *)((int)param_1 + 0xad) + iVar15 * 4) + iVar14;
          *(u16 *)(iVar12 + 10) = *(u16 *)(iVar12 + 0xc);
          *(u16 *)(*(int *)(*(int *)((int)param_1 + 0xad) + iVar15 * 4) + 2 + iVar14) = uVar4
          ;
          *(u16 *)(*(int *)(*(int *)((int)param_1 + 0xad) + iVar15 * 4) + 0x10 + iVar14) =
               uVar6;
          *(u16 *)(*(int *)(*(int *)((int)param_1 + 0xad) + iVar15 * 4) + 0xc + iVar14) =
               uVar5;
        }
      }
      iVar14 = iVar14 + 0x35;
      _param_2 = _param_2 + -1;
    } while (_param_2 != 0);
    return;
  }
  return;
}



