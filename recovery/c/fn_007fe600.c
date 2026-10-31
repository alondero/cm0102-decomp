/* WIP: incoming DF controls the original REP direction; plain C has no DF transport. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_009c3730 CM_GLOBAL(volatile u8, 0x009c3730)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(volatile u32, 0x00b67a34)
#define s_E__dev_CM3_cm3_00_01_si_code_scr_00a7ddbc ((char *)0x00a7ddbc)
#define s_Error_009870e0 ((char *)0x009870e0)
#define s_v_s__s__s__d_009870e8 ((char *)0x009870e8)
extern void cm_fn_005e8290(const char * self,const char * param_2,int param_3);
extern void CM_THISCALL cm_fn_007ff270(u32 self,s32 index,u32 vector_value,u32 value);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

u32 CM_THISCALL
cm_fn_007fe600(u32 self,u32 param_2,u32 param_3,u32 param_4,u32 param_5,u32 param_6)

{
  short *psVar1;
  char cVar2;
  void *pcVar3;
  u8 bVar4;
  int iVar5;
  int *piVar6;
  u32 uVar7;
  int iVar8;
  u32 slot_ecx;
  u32 uVar9;
  u32 uVar10;
  short sVar11;
  int *piVar12;
  volatile char *pcVar13;
  volatile char *pcVar14;
  int *piVar15;
  u8 scratch[0x200];
  u8 *local_200 = scratch;
  u8 *local_100 = scratch + 0x100;
  
  bVar4 = false;
  iVar5 = *(short *)(self + 0x382a) * 0x300;
  iVar8 = iVar5 + self;
  slot_ecx = (u32)iVar8;
  if (((*(int *)(iVar5 + 0x7d6 + self) == 0) && (*(int *)(iVar8 + 0x7da) == 0)) &&
     (*(int *)(self + 0x37d4) != 0)) {
    param_3 = *(int *)(self + 0x37d8);
    param_4 = *(int *)(self + 0x37dc);
    param_5 = *(int *)(self + 0x37e0);
    param_6 = *(int *)(self + 0x37e4);
    bVar4 = true;
    param_2 = *(int *)(self + 0x37d4);
  }
  if ((param_2 == 0) || (param_3 == 0)) {
    cm_fn_00944cff((u32)s_E__dev_CM3_cm3_00_01_si_code_scr_00a7ddbc,0,0,local_100,local_200);
    uVar7 = cm_fn_0090d130();
    cm_fn_00944c9f((u32)&DAT_00ae24d0,(u32)s_v_s__s__s__d_009870e8,uVar7,(u32)local_100,(u32)local_200,0x4ed);
    cm_fn_005e8290(s_Error_009870e0,(const char *)&DAT_00ae24d0,0);
    DAT_00b67a34 = 0;
    return 0;
  }
  iVar5 = *(int *)(iVar8 + 0x7e2);
  if ((iVar5 != 0) && (param_4 != 0)) {
    for (piVar12 = *(int **)(iVar8 + 0x7da); piVar12 != (int *)0x0; piVar12 = (int *)piVar12[0x7e])
    {
      if ((piVar12[4] != 0) && ((u32)*piVar12 == param_2)) {
        for (; piVar12 != (int *)0x0; piVar12 = (int *)piVar12[0x7e]) {
          if (piVar12[4] != 0) {
            *(int **)(*(short *)(self + 0x382a) * 0x300 + 0x7e2 + self) = piVar12;
          }
        }
        iVar8 = *(short *)(self + 0x382a) * 0x300;
        if ((u32)**(int **)(iVar8 + 0x7e2 + self) == param_2) {
          return 0;
        }
        *(int *)(iVar8 + 0x7e2 + self) = iVar5;
        return 0;
      }
    }
  }
  if (iVar5 != *(int *)(iVar8 + 0x7de)) {
    *(int *)(iVar8 + 0x7de) = iVar5;
    iVar5 = *(short *)(self + 0x382a) * 0x300;
    iVar8 = *(int *)(*(int *)(iVar5 + 0x7e2 + self) + 0x1f8);
    *(u32 *)(*(int *)(iVar5 + self + 0x7de) + 0x1f8) = 0;
    while (iVar8 != 0) {
      piVar12 = (int *)(iVar8 + 0x14);
      iVar5 = 0x3c;
      *(u32 *)(*(short *)(self + 0x382a) * 0x300 + 0x7e2 + self) =
           *(u32 *)(iVar8 + 0x1f8);
      do {
        if ((*piVar12 != 0) && (piVar12[1] != 0)) {
          cm_fn_00944c94((void *)(u32)*piVar12);
        }
        piVar12 = piVar12 + 2;
        iVar5 = iVar5 + -1;
      } while (iVar5 != 0);
      cm_fn_00944c94((void *)(u32)iVar8);
      psVar1 = (short *)(*(short *)(self + 0x382a) * 0x300 + 0x7e8 + self);
      *psVar1 = *psVar1 + -1;
      iVar8 = *(int *)(*(short *)(self + 0x382a) * 0x300 + 0x7e2 + self);
    }
    iVar8 = *(short *)(self + 0x382a) * 0x300;
    *(u32 *)(iVar8 + self + 0x7e2) = *(u32 *)(iVar8 + 0x7de + self);
    slot_ecx = (u32)iVar8;
  }
  iVar8 = *(short *)(self + 0x382a) * 0x300 + self;
  if (100 < *(short *)(iVar8 + 0x7e8)) {
    iVar5 = *(int *)(iVar8 + 0x7da);
    if (*(int *)(iVar5 + 0x10) != 0) {
      return 0;
    }
    piVar12 = (int *)(iVar5 + 0x14);
    *(u32 *)(iVar8 + 0x7da) = *(u32 *)(iVar5 + 0x1f8);
    *(u32 *)(*(int *)(*(short *)(self + 0x382a) * 0x300 + 0x7da + self) + 500) = 0;
    iVar8 = 0x3c;
    do {
      if ((*piVar12 != 0) && (piVar12[1] != 0)) {
        cm_fn_00944c94((void *)(u32)*piVar12);
      }
      piVar12 = piVar12 + 2;
      iVar8 = iVar8 + -1;
    } while (iVar8 != 0);
    cm_fn_00944c94((void *)(u32)iVar5);
    psVar1 = (short *)(*(short *)(self + 0x382a) * 0x300 + 0x7e8 + self);
    *psVar1 = *psVar1 + -1;
    slot_ecx = (u32)(*(short *)(self + 0x382a) * 0x300);
  }
  iVar8 = *(int *)(*(short *)(self + 0x382a) * 0x300 + 0x7e2 + self);
  if ((iVar8 != 0) && (pcVar3 = *(void **)(iVar8 + 4), pcVar3 != (void *)0x0)) {
    ((void (CM_THISCALL *)(u32))(u32)pcVar3)(slot_ecx);
  }
  piVar6 = (int *)(u32)cm_fn_00944cf1(0x300);
  piVar12 = piVar6;
  for (iVar8 = 0xc0; iVar8 != 0; iVar8 = iVar8 + -1) {
    *piVar12 = 0;
    piVar12 = piVar12 + 1;
  }
  *piVar6 = param_2;
  piVar6[1] = param_5;
  piVar6[3] = param_6;
  piVar6[2] = param_3;
  uVar9 = 0xffffffff;
  piVar6[4] = param_4;
  pcVar13 = (volatile char *)&DAT_009c3730;
  do {
    pcVar14 = pcVar13;
    if (uVar9 == 0) break;
    uVar9 = uVar9 - 1;
    pcVar14 = pcVar13 + 1;
    cVar2 = *pcVar13;
    pcVar13 = pcVar14;
  } while (cVar2 != '\0');
  uVar9 = ~uVar9;
  piVar12 = (int *)(pcVar14 + -uVar9);
  piVar15 = piVar6 + 0x7f;
  for (uVar10 = uVar9 >> 2; uVar10 != 0; uVar10 = uVar10 - 1) {
    *piVar15 = *piVar12;
    piVar12 = piVar12 + 1;
    piVar15 = piVar15 + 1;
  }
  for (uVar9 = uVar9 & 3; uVar9 != 0; uVar9 = uVar9 - 1) {
    *(volatile u8 *)piVar15 = *(volatile u8 *)piVar12;
    piVar12 = (int *)((int)piVar12 + 1);
    piVar15 = (int *)((int)piVar15 + 1);
  }
  if (param_4 != 0) {
    psVar1 = (short *)(*(short *)(self + 0x382a) * 0x300 + 0x7e6 + self);
    *psVar1 = *psVar1 + 1;
  }
  iVar8 = *(int *)(*(short *)(self + 0x382a) * 0x300 + 0x7de + self);
  if (iVar8 != 0) {
    *(int **)(iVar8 + 0x1f8) = piVar6;
    piVar6[0x7d] = *(int *)(*(short *)(self + 0x382a) * 0x300 + 0x7de + self);
  }
  *(int **)(*(short *)(self + 0x382a) * 0x300 + 0x7de + self) = piVar6;
  iVar8 = *(short *)(self + 0x382a) * 0x300;
  if (*(int *)(iVar8 + 0x7da + self) == 0) {
    *(int **)(iVar8 + 0x7da + self) = piVar6;
  }
  *(int **)(*(short *)(self + 0x382a) * 0x300 + 0x7e2 + self) = piVar6;
  psVar1 = (short *)(*(short *)(self + 0x382a) * 0x300 + 0x7e8 + self);
  *psVar1 = *psVar1 + 1;
  *(u32 *)(*(short *)(self + 0x382a) * 0x300 + 0x7ee + self) = 0;
  *(u16 *)(*(short *)(self + 0x382a) * 0x300 + 0x7f6 + self) = 0xffff;
  *(u32 *)(*(short *)(self + 0x382a) * 0x300 + 0x7f8 + self) = 0;
  if (param_4 != 0) {
    *(u32 *)(*(short *)(self + 0x382a) * 0x300 + 0x7f2 + self) = 0;
  }
  *(u32 *)(*(short *)(self + 0x382a) * 0x300 + 0x7ea + self) = 1;
  if ((bVar4) && (sVar11 = 0, 0 < *(short *)(self + 0x37ec))) {
    do {
      cm_fn_007ff270(self,(s32)sVar11,*(u32 *)(*(u32 *)(self + 0x37e8) + (u32)(s32)sVar11 * 4),0);
      sVar11 = sVar11 + 1;
    } while (sVar11 < *(short *)(self + 0x37ec));
  }
  return 1;
}





