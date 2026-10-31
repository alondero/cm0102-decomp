/* Draft for preferred-image VA 0x00522950, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self,u32 arg1,u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self,u32 arg1,u32 arg2);

void CM_THISCALL cm_fn_00522950(u32 param_1,s16 param_2,s16 param_3)

{
  short sVar1;
  short sVar2;
  u32 iVar3;
  volatile short *psVar4;
  int iVar5;
  volatile short *psVar6;
  u32 iVar7;
  volatile u32 *puVar8;
  volatile u32 *puVar9;
  volatile u32 *puVar10;
  u32 uVar11;
  volatile u32 *local_108;
  volatile u32 *local_104;
  volatile u32 *local_100;
  volatile u8 local_ec [224];
  volatile struct { u32 prev, handler, state; } frame;


  
  frame.state = 0xffffffffu;
  frame.handler = 0x009577fbu;
  frame.prev = CM_FS(u32, 0);
  CM_FS(u32, 0) = (u32)&frame;
  iVar3 = *(volatile u32 *)(param_1 + 0xad);
  puVar9 = (u32 *)0x0;
  psVar4 = (volatile short *)((u32)*(volatile u32 *)(iVar3 + (u32)(s32)param_2 * 4u) +
                              (u32)(s32)param_3 * 0x35u);
  local_108 = (u32 *)0x0;
  local_104 = (u32 *)0x0;
  local_100 = (u32 *)0x0;
  sVar1 = *psVar4;
  if ((sVar1 != -1) && (psVar4[1] != -1)) {
    goto cleanup;
  }
  iVar5 = param_2 * 0x68;
  u32 count_base = *(volatile u32 *)(param_1 + 0xa3);
  if (*(volatile s16 *)(count_base + 0x1au + (u32)iVar5) < 2) {
    goto cleanup;
  }
  if (sVar1 == -1) {
    sVar2 = psVar4[7];
    if (sVar2 == -1) {
      goto cleanup;
    }
    s16 row_index = psVar4[5];
    psVar6 = (volatile short *)((u32)*(volatile u32 *)(iVar3 + (u32)(s32)sVar2 * 4u) +
                                (u32)(s32)row_index * 0x35u);
    sVar1 = *psVar6;
    if (sVar1 == -1) {
      goto cleanup;
    }
    sVar2 = psVar6[1];
    if (sVar2 == -1) {
      goto cleanup;
    }
    iVar7 = *(volatile u32 *)(param_1 + 0xa7);
    puVar10 = (volatile u32 *)(u32)*(volatile u32 *)(iVar7 + (u32)(s32)sVar1 * 6u);
    if (puVar10 != (u32 *)0x0) {
      local_108 = puVar10;
    }
    puVar10 = (volatile u32 *)(u32)*(volatile u32 *)(iVar7 + (u32)(s32)sVar2 * 6u);
    if (puVar10 != (u32 *)0x0) {
      local_104 = puVar10;
    }
  }
  else {
    iVar7 = *(volatile u32 *)(param_1 + 0xa7);
    puVar10 = (volatile u32 *)(u32)*(volatile u32 *)(iVar7 + (u32)(s32)sVar1 * 6u);
    if (puVar10 != (u32 *)0x0) {
      puVar9 = puVar10;
    }
  }
  s16 second_id = psVar4[1];
  if (second_id == -1) {
    sVar2 = psVar4[8];
    if (sVar2 == -1) {
      goto cleanup;
    }
    s16 row_index = psVar4[6];
    psVar4 = (volatile short *)((u32)*(volatile u32 *)(iVar3 + (u32)(s32)sVar2 * 4u) +
                                (u32)(s32)row_index * 0x35u);
    sVar1 = *psVar4;
    if (sVar1 == -1) {
      goto cleanup;
    }
    sVar2 = psVar4[1];
    if (sVar2 == -1) {
      goto cleanup;
    }
    puVar10 = (volatile u32 *)(u32)*(volatile u32 *)(iVar7 + (u32)(s32)sVar1 * 6u);
    if (puVar10 != (u32 *)0x0) {
      local_100 = puVar10;
    }
    puVar8 = (volatile u32 *)(u32)*(volatile u32 *)(iVar7 + (u32)(s32)sVar2 * 6u);
    if (puVar8 == (u32 *)0x0) {
LAB_00522ae1:
      puVar10 = (u32 *)0x0;
      goto LAB_00522ae5;
    }
    puVar10 = (u32 *)0x0;
  }
  else {
    puVar10 = (volatile u32 *)(u32)*(volatile u32 *)(iVar7 + (u32)(s32)second_id * 6u);
    if (puVar10 == (u32 *)0x0) goto LAB_00522ae1;
LAB_00522ae5:
    puVar8 = (u32 *)0x0;
  }

  cm_fn_0077bbe0((void *)local_ec,0x26,0);
  u32 nested = *(volatile u32 *)(param_1 + 4);
  frame.state = 0;
  u32 nested_value = *(volatile u32 *)nested;
  cm_fn_00785790((void *)local_ec,0,nested_value);
  cm_fn_00785790((void *)local_ec,1,(int)*(volatile short *)(param_1 + 0x3a));
  u32 row_base = *(volatile u32 *)(param_1 + 0xa3);
  cm_fn_00785790((void *)local_ec,2,(u32)(s32)*(volatile s16 *)(row_base + (u32)iVar5 + 7u));
  if (puVar9 == (u32 *)0x0) {
    cm_fn_00785790((void *)local_ec,3,*local_108);
    cm_fn_00785790((void *)local_ec,4,*local_104);
  }
  else {
    cm_fn_00785790((void *)local_ec,3,*puVar9);
    cm_fn_00785790((void *)local_ec,4,0xffffffff);
  }
  if (puVar10 == (u32 *)0x0) {
    cm_fn_00785790((void *)local_ec,5,*local_100);
    uVar11 = *puVar8;
  }
  else {
    cm_fn_00785790((void *)local_ec,5,*puVar10);
    uVar11 = 0xffffffff;
  }
  cm_fn_00785790((void *)local_ec,6,uVar11);
  if (puVar9 == (u32 *)0x0) {
    if (local_108 != (u32 *)0x0) {
      cm_fn_007861e0((void *)CM_GLOBAL(volatile u32, 0x00b7431c),
                     (u32)(volatile void *)local_ec,(u32)local_108);
    }
    puVar9 = local_104;
    if (local_104 == (u32 *)0x0) goto LAB_00522bf7;
  }
  cm_fn_007861e0((void *)CM_GLOBAL(volatile u32, 0x00b7431c),
                 (u32)(volatile void *)local_ec,(u32)puVar9);
LAB_00522bf7:
  if (puVar10 == (u32 *)0x0) {
    if (local_100 != (u32 *)0x0) {
      cm_fn_007861e0((void *)CM_GLOBAL(volatile u32, 0x00b7431c),
                     (u32)(volatile void *)local_ec,(u32)local_100);
    }
    puVar10 = puVar8;
    if (puVar8 == (u32 *)0x0) {
      goto cleanup;
    }
  }
  cm_fn_007861e0((void *)CM_GLOBAL(volatile u32, 0x00b7431c),
                 (u32)(volatile void *)local_ec,(u32)puVar10);
cleanup:
  CM_FS(u32, 0) = frame.prev;
  return;
}



