#include "cmre.h"

void CM_THISCALL cm_fn_00522610(volatile u8 *param_1,s16 param_2)

{
  short sVar1;
  volatile s16 *psVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  u16 uVar6;
  u16 uVar7;
  u16 uVar8;
  short sVar9;
  u32 iVar10;
  u32 iVar11;
  short sVar12;
  u32 iVar13;
  u32 object_array;
  u32 link_table;
  
  iVar13 = (u32)(s32)param_2;
  sVar9 = 0;
  sVar3 = *(volatile s16 *)(iVar13 * 0x68u + 0x1au + *(volatile u32 *)(param_1 + 0xa3));
  if (0 < sVar3) {
    do {
      sVar1 = sVar9 + 1;
      if (sVar1 < sVar3) {
        iVar11 = *(volatile u32 *)(*(volatile u32 *)(param_1 + 0xad) + iVar13 * 4u);
        psVar2 = (volatile s16 *)((u32)(s32)sVar9 * 0x35u + iVar11);
        sVar9 = *psVar2;
        sVar12 = sVar1;
        do {
          if ((sVar9 != -1) && (sVar4 = *(volatile s16 *)((u32)(s32)sVar12 * 0x35u + iVar11), sVar4 != -1) &&
              ((object_array = *(volatile u32 *)(param_1 + 0xa7)),
               (iVar10 = *(volatile u32 *)(*(volatile u32 *)(object_array + (u32)(s32)sVar9 * 6u) + 0x69)),
               iVar10 == *(volatile u32 *)(*(volatile u32 *)(object_array + (u32)(s32)sVar4 * 6u) + 0x69) && iVar10 != 0)) {
            iVar10 = (u32)(s32)sVar12 * 0x35u;
            uVar6 = *(volatile u16 *)(iVar10 + 10 + iVar11);
            uVar7 = *(volatile u16 *)(iVar10 + iVar11);
            uVar8 = *(volatile u16 *)(iVar10 + 0xe + iVar11);
            *(volatile u16 *)(iVar10 + iVar11) = *(volatile u16 *)(iVar10 + 2 + iVar11);
            iVar11 = *(volatile u32 *)(*(volatile u32 *)(param_1 + 0xad) + iVar13 * 4u) + iVar10;
            *(volatile u16 *)(iVar11 + 0xe) = *(volatile u16 *)(iVar11 + 0x10);
            iVar11 = *(volatile u32 *)(*(volatile u32 *)(param_1 + 0xad) + iVar13 * 4u) + iVar10;
            *(volatile u16 *)(iVar11 + 10) = *(volatile u16 *)(iVar11 + 0xc);
            *(volatile u16 *)(*(volatile u32 *)(*(volatile u32 *)(param_1 + 0xad) + iVar13 * 4u) + 2 + iVar10) = uVar7;
            *(volatile u16 *)(*(volatile u32 *)(*(volatile u32 *)(param_1 + 0xad) + iVar13 * 4u) + 0x10 + iVar10) = uVar8
            ;
            *(volatile u16 *)(*(volatile u32 *)(*(volatile u32 *)(param_1 + 0xad) + iVar13 * 4u) + 0xc + iVar10) = uVar6;
            link_table = *(volatile u32 *)(param_1 + 0xad);
            iVar11 = *(volatile u32 *)(link_table + iVar13 * 4u) + iVar10;
            sVar3 = *(volatile s16 *)(iVar11 + 0xe);
            if (sVar3 != -1) {
              s16 link_row = *(volatile s16 *)(iVar11 + 10);
              u32 link_object = *(volatile u32 *)(link_table + (u32)(s32)sVar3 * 4u);
              *(volatile u16 *)(link_object + (u32)(s32)link_row * 0x35u + 8) = 1;
            }
            iVar11 = *(volatile u32 *)(param_1 + 0xad);
            iVar10 = iVar10 + *(volatile u32 *)(iVar11 + iVar13 * 4u);
            sVar3 = *(volatile s16 *)(iVar10 + 0x10);
joined_r0x005227f0:
            if (sVar3 != -1) {
              s16 cross_row = *(volatile s16 *)(iVar10 + 0xc);
              u32 cross_object = *(volatile u32 *)(iVar11 + (u32)(s32)sVar3 * 4u);
              *(volatile u16 *)(cross_object + (u32)(s32)cross_row * 0x35u + 8) = 0;
            }
            break;
          }
          sVar4 = psVar2[1];
          if ((sVar4 != -1) && (sVar5 = *(volatile s16 *)(iVar11 + 2 + (u32)(s32)sVar12 * 0x35u), sVar5 != -1) &&
              ((object_array = *(volatile u32 *)(param_1 + 0xa7)),
               (iVar10 = *(volatile u32 *)(*(volatile u32 *)(object_array + (u32)(s32)sVar4 * 6u) + 0x69)),
               iVar10 == *(volatile u32 *)(*(volatile u32 *)(object_array + (u32)(s32)sVar5 * 6u) + 0x69) && iVar10 != 0)) {
            iVar10 = (u32)(s32)sVar12 * 0x35u;
            uVar6 = *(volatile u16 *)(iVar10 + 10 + iVar11);
            uVar7 = *(volatile u16 *)(iVar10 + iVar11);
            uVar8 = *(volatile u16 *)(iVar10 + 0xe + iVar11);
            *(volatile u16 *)(iVar10 + iVar11) = *(volatile u16 *)(iVar10 + 2 + iVar11);
            iVar11 = *(volatile u32 *)(*(volatile u32 *)(param_1 + 0xad) + iVar13 * 4u) + iVar10;
            *(volatile u16 *)(iVar11 + 0xe) = *(volatile u16 *)(iVar11 + 0x10);
            iVar11 = *(volatile u32 *)(*(volatile u32 *)(param_1 + 0xad) + iVar13 * 4u) + iVar10;
            *(volatile u16 *)(iVar11 + 10) = *(volatile u16 *)(iVar11 + 0xc);
            *(volatile u16 *)(*(volatile u32 *)(*(volatile u32 *)(param_1 + 0xad) + iVar13 * 4u) + 2 + iVar10) = uVar7;
            *(volatile u16 *)(*(volatile u32 *)(*(volatile u32 *)(param_1 + 0xad) + iVar13 * 4u) + 0x10 + iVar10) = uVar8
            ;
            *(volatile u16 *)(*(volatile u32 *)(*(volatile u32 *)(param_1 + 0xad) + iVar13 * 4u) + 0xc + iVar10) = uVar6;
            link_table = *(volatile u32 *)(param_1 + 0xad);
            iVar11 = *(volatile u32 *)(link_table + iVar13 * 4u) + iVar10;
            sVar3 = *(volatile s16 *)(iVar11 + 0xe);
            if (sVar3 != -1) {
              s16 link_row = *(volatile s16 *)(iVar11 + 10);
              u32 link_object = *(volatile u32 *)(link_table + (u32)(s32)sVar3 * 4u);
              *(volatile u16 *)(link_object + (u32)(s32)link_row * 0x35u + 8) = 1;
            }
            iVar11 = *(volatile u32 *)(param_1 + 0xad);
            iVar10 = iVar10 + *(volatile u32 *)(iVar11 + iVar13 * 4u);
            sVar3 = *(volatile s16 *)(iVar10 + 0x10);
            goto joined_r0x005227f0;
          }
          sVar12 = sVar12 + 1;
        } while (sVar12 < sVar3);
      }
      sVar3 = *(volatile s16 *)(iVar13 * 0x68u + 0x1au + *(volatile u32 *)(param_1 + 0xa3));
      sVar9 = sVar1;
    } while (sVar1 < sVar3);
  }
  return;
}
