/* Draft for preferred-image VA 0x006d78b0, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_00b68790 CM_GLOBAL(u32, 0x00b68790)

u16 * CM_THISCALL cm_fn_006d78b0(int param_1,int *param_2)

{
  u16 *puVar1;
  u16 *puVar2;
  char cVar3;
  short sVar4;
  short sVar5;
  char cVar6;
  int iVar7;
  int iVar8;
  short sVar9;
  char *pcVar10;
  char *pcVar11;
  
  if (*(char *)(param_1 + 5) == '\0') {
    return (u16 *)0x0;
  }
  sVar9 = 0;
  if (param_2 != (int *)0x0) {
    *param_2 = param_1 + 0xd8 + *(short *)(param_1 + 0xc) * 0x86;
  }
  sVar4 = *(short *)(param_1 + 0xc);
  puVar1 = (u16 *)(param_1 + 0x30 + sVar4 * 0xe);
  sVar5 = *(short *)(param_1 + 0x30 + sVar4 * 0xe);
  while (sVar5 == 0) {
    *(short *)(param_1 + 0xc) = sVar4 + 1;
    if (0xb < (short)(sVar4 + 1)) {
      *(u16 *)(param_1 + 0xc) = 0;
    }
    *(u8 *)(param_1 + 0x3d + *(short *)(param_1 + 0xc) * 0xe) = 0xff;
    puVar1 = (u16 *)(param_1 + 0x30 + *(short *)(param_1 + 0xc) * 0xe);
    *puVar1 = *puVar1 & 0x7fff;
    sVar4 = *(short *)(param_1 + 0xc);
    if (sVar4 == *(short *)(param_1 + 8)) {
      *(u32 *)(param_1 + 0x2d68) = 0xffffffff;
LAB_006d79ec:
      *(u8 *)(param_1 + 5) = 0;
      return (u16 *)0x0;
    }
    if (sVar4 == *(short *)(param_1 + 10)) goto LAB_006d79ec;
    puVar1 = (u16 *)(param_1 + 0x30 + sVar4 * 0xe);
    sVar5 = *(short *)(param_1 + 0x30 + sVar4 * 0xe);
  }
  cVar6 = *(char *)((int)puVar1 + 0xd);
  iVar7 = (int)(short)((*puVar1 & 0x7fff) + 0xe0c0);
  iVar8 = -1;
  pcVar11 = *(char **)((short)puVar1[1] * 0x110 + 0x108 +
                      *(int *)((int)&DAT_00b68790 + iVar7 * 0x12));
  pcVar10 = pcVar11;
  do {
    if (iVar8 == 0) break;
    iVar8 = iVar8 + -1;
    cVar3 = *pcVar10;
    pcVar10 = pcVar10 + 1;
  } while (cVar3 != '\0');
  if (0 < (short)(~(u16)iVar8 - 1)) {
    do {
      if (pcVar11[sVar9] == '|') {
        if (cVar6 == -1) {
          *(u8 *)((int)puVar1 + 0xd) = 0;
          break;
        }
        cVar6 = cVar6 + -1;
        if (cVar6 < '\0') {
          sVar9 = sVar9 + 1;
          *puVar1 = *puVar1 | 0x8000;
          *(char *)((int)puVar1 + 0xd) = *(char *)((int)puVar1 + 0xd) + '\x01';
          break;
        }
      }
      iVar8 = -1;
      sVar9 = sVar9 + 1;
      pcVar10 = pcVar11;
      do {
        if (iVar8 == 0) break;
        iVar8 = iVar8 + -1;
        cVar3 = *pcVar10;
        pcVar10 = pcVar10 + 1;
      } while (cVar3 != '\0');
    } while (sVar9 < (short)(~(u16)iVar8 - 1));
  }
  if (*(char *)((int)puVar1 + 0xd) != -1) {
    pcVar11 = *(char **)((short)puVar1[1] * 0x110 + 0x108 +
                        *(int *)((int)&DAT_00b68790 + iVar7 * 0x12));
    iVar8 = -1;
    pcVar10 = pcVar11;
    do {
      if (iVar8 == 0) break;
      iVar8 = iVar8 + -1;
      cVar6 = *pcVar10;
      pcVar10 = pcVar10 + 1;
    } while (cVar6 != '\0');
    if (sVar9 < (short)(~(u16)iVar8 - 1)) {
      do {
        if (pcVar11[sVar9] == '|') break;
        iVar8 = -1;
        sVar9 = sVar9 + 1;
        pcVar10 = pcVar11;
        do {
          if (iVar8 == 0) break;
          iVar8 = iVar8 + -1;
          cVar6 = *pcVar10;
          pcVar10 = pcVar10 + 1;
        } while (cVar6 != '\0');
      } while (sVar9 < (short)(~(u16)iVar8 - 1));
    }
  }
  iVar8 = -1;
  pcVar11 = *(char **)((short)puVar1[1] * 0x110 + 0x108 +
                      *(int *)((int)&DAT_00b68790 + iVar7 * 0x12));
  do {
    if (iVar8 == 0) break;
    iVar8 = iVar8 + -1;
    cVar6 = *pcVar11;
    pcVar11 = pcVar11 + 1;
  } while (cVar6 != '\0');
  if ((short)(~(u16)iVar8 - 1) <= sVar9) {
    *(short *)(param_1 + 0xc) = *(short *)(param_1 + 0xc) + 1;
    if (0xb < *(short *)(param_1 + 0xc)) {
      *(u16 *)(param_1 + 0xc) = 0;
    }
    *(u8 *)(param_1 + 0x3d + *(short *)(param_1 + 0xc) * 0xe) = 0xff;
    puVar2 = (u16 *)(param_1 + 0x30 + *(short *)(param_1 + 0xc) * 0xe);
    *puVar2 = *puVar2 & 0x7fff;
    if (*(short *)(param_1 + 0xc) == *(short *)(param_1 + 8)) {
      *(u32 *)(param_1 + 0x2d68) = 0xffffffff;
      *(u8 *)(param_1 + 5) = 0;
    }
    if (*(short *)(param_1 + 0xc) == *(short *)(param_1 + 10)) {
      *(u8 *)(param_1 + 5) = 0;
    }
  }
  return puVar1;
}

