#include "cmre.h"
#define CM_ROW(base, index) ((u32)(base) + (u32)(s32)(index) * 0x3bu)
u8 CM_THISCALL cm_fn_0068a4d0(u32 param_1,u32 param_2,u32 param_3)

{
  u32 uVar1;
  char cVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  short sVar8;
  short sVar9;
  int iVar10;
  u8 bVar11;
  s8 early_candidate4;
  s16 competitor19;
  s8 self_mode;
  
  uVar1 = (int)*(volatile s16 *)(param_1 + 0xa9) - 1;
  if (((int)*(volatile s8 *)(param_2 + 0x11) < (int)uVar1) || (*(volatile s8 *)(param_2 + 0x37) != -1))
  goto LAB_0068a793;
  if (((*(volatile u8 *)(param_1 + 0xd9) & 1) != 0) &&
     (iVar5 = *(volatile s32 *)(param_1 + 0xb1), *(volatile s8 *)((u32)iVar5 + 0x37u) != '\x05')) {
    early_candidate4 = *(volatile s8 *)(param_2 + 4);
    if (early_candidate4 < 1) {
      if (1 < *(volatile s16 *)(param_1 + 0x3e)) {
        iVar10 = (int)*(volatile s16 *)((u32)iVar5 + 0x54u) + (int)*(volatile s8 *)(param_1 + 0xc2);
        competitor19 = *(volatile s16 *)((u32)iVar5 + 0x19u);
        if (competitor19 < iVar10) {
          *(volatile u8 *)param_3 = 0x14;
          return 0xff;
        }
        if (iVar10 == competitor19) {
          *(volatile u8 *)param_3 = 0x14;
          return 0;
        }
        goto LAB_0068a793;
      }
    }
    else {
      iVar7 = *(volatile s8 *)(param_1 + 0xc2);
      iVar10 = (int)*(volatile s16 *)(param_2 + 0x19) + iVar7;
      competitor19 = *(volatile s16 *)((u32)iVar5 + 0x19u);
      if (competitor19 < iVar10) goto LAB_0068a793;
      if (iVar10 == competitor19) {
        self_mode = *(volatile s8 *)(param_1 + 0xc5);
        cVar4 = self_mode;
        if (cVar4 == '\x01') {
LAB_0068a53f:
          s8 competitor15 = *(volatile s8 *)((u32)iVar5 + 0x15u);
          s8 competitor17 = *(volatile s8 *)((u32)iVar5 + 0x17u);
          s8 candidate15 = *(volatile s8 *)(param_2 + 0x15);
          s8 candidate17 = *(volatile s8 *)(param_2 + 0x17);
          cVar4 = (s8)((u8)competitor15 - (u8)competitor17 - (u8)candidate15 + (u8)candidate17);
LAB_0068a831:
          cVar4 = cVar4 + '\x01';
          if (cVar4 < '\x01') {
            cVar4 = -1;
          }
          return (u8)cVar4;
        }
        if (cVar4 == '\x02') {
          cVar4 = *(volatile s8 *)((u32)iVar5 + 0x15u);
LAB_0068a82e:
          cVar4 = cVar4 - *(volatile s8 *)(param_2 + 0x15);
          goto LAB_0068a831;
        }
      }
    }
  }
  cVar4 = *(volatile s8 *)(param_1 + 0xbe);
  s8 self_bf;
  if (cVar4 != '\0' || (self_bf = *(volatile s8 *)(param_1 + 0xbf)) != 0) {
    self_bf = *(volatile s8 *)(param_1 + 0xbf);
    cVar4 = (s8)((u8)self_bf + (u8)cVar4);
    if (*(volatile s8 *)(param_2 + 4) < cVar4) {
      sVar8 = *(volatile s16 *)(param_1 + 0x3e);
      sVar9 = (short)cVar4;
      if (sVar9 < sVar8) {
        u32 table_base = *(volatile u32 *)(param_1 + 0xb1);
        do {
          u32 row = CM_ROW(table_base, sVar9);
          if (*(volatile s8 *)(row + 0x37) == -1) {
            s16 row19 = *(volatile s16 *)(row + 0x19);
            s8 offset = *(volatile s8 *)(param_1 + 0xc2);
            s16 candidate19 = *(volatile s16 *)(param_2 + 0x19);
            iVar5 = (s32)row19 + offset;
            bVar11 = iVar5 == candidate19;
            if (candidate19 < iVar5) goto LAB_0068a78c;
            goto LAB_0068a752;
          }
          sVar9 = sVar9 + 1;
        } while (sVar9 < sVar8);
      }
    }
    else {
      sVar8 = 0;
      if ('\0' < cVar4) {
        u32 table_base = *(volatile u32 *)(param_1 + 0xb1);
        do {
          iVar5 = CM_ROW(table_base, sVar8);
          if (*(volatile s8 *)((u32)iVar5 + 0x37u) == -1) {
            s16 candidate19 = *(volatile s16 *)(param_2 + 0x19);
            s8 offset = *(volatile s8 *)(param_1 + 0xc2);
            s16 row19 = *(volatile s16 *)((u32)iVar5 + 0x19u);
            iVar10 = (s32)candidate19 + offset;
            if (row19 < iVar10) goto LAB_0068a793;
            if (iVar10 == candidate19) {
              self_mode = *(volatile s8 *)(param_1 + 0xc5);
              if (self_mode == 1) {
                iVar5 = CM_ROW(table_base, sVar8);
                goto LAB_0068a53f;
              }
              if (self_mode == 2) {
                cVar4 = *(volatile s8 *)(CM_ROW(table_base, sVar8) + 0x15);
                goto LAB_0068a82e;
              }
            }
          }
          sVar8 = sVar8 + 1;
        } while (sVar8 < cVar4);
      }
    }
  }
  s8 self_c1 = *(volatile s8 *)(param_1 + 0xc1);
  if (self_c1 != 0 || *(volatile s8 *)(param_1 + 0xc0) != 0) {
    s8 self_c0 = *(volatile s8 *)(param_1 + 0xc0);
    cVar4 = (s8)((u8)self_c0 + (u8)self_c1);
    sVar8 = *(volatile s16 *)(param_1 + 0x3e);
    iVar10 = (int)sVar8 - (int)cVar4;
    uVar1 = (u32)*(volatile s8 *)(param_2 + 4);
    if ((int)uVar1 < iVar10) {
      sVar9 = sVar8 - cVar4;
      if (sVar9 < sVar8) {
        u32 table_base = *(volatile u32 *)(param_1 + 0xb1);
        while (iVar5 = (int)sVar9, *(volatile s8 *)(CM_ROW(table_base, sVar9) + 0x37) != -1)
        {
          sVar9 = sVar9 + 1;
          if (sVar8 <= sVar9) {
            return 0xff;
          }
        }
        s16 row19 = *(volatile s16 *)(CM_ROW(table_base, sVar9) + 0x19);
        s8 offset = *(volatile s8 *)(param_1 + 0xc2);
        s16 candidate19 = *(volatile s16 *)(param_2 + 0x19);
        iVar5 = (s32)row19 + offset;
        bVar11 = iVar5 == candidate19;
        if (candidate19 < iVar5) {
LAB_0068a78c:
          *(volatile u8 *)param_3 = 0x14;
        }
        else {
LAB_0068a752:
          if (bVar11) {
            *(volatile u8 *)param_3 = 0x14;
            return 0;
          }
        }
      }
    }
    else {
      uVar1 = 0;
      if (0 < iVar10) {
        iVar5 = *(volatile s32 *)(param_1 + 0xb1);
        iVar6 = 0;
        do {
          iVar6 = (int)CM_ROW((u32)iVar5, (s16)uVar1);
          if (*(volatile s8 *)((u32)iVar6 + 0x37u) == -1) {
            s16 candidate19 = *(volatile s16 *)(param_2 + 0x19);
            s8 offset = *(volatile s8 *)(param_1 + 0xc2);
            s16 row19 = *(volatile s16 *)((u32)iVar6 + 0x19u);
            iVar7 = (s32)candidate19 + offset;
            if (row19 < iVar7) break;
            if (iVar7 == candidate19) {
              sVar8 = (short)uVar1;
              self_mode = *(volatile s8 *)(param_1 + 0xc5);
              if (self_mode == 1) {
                uVar1 = sVar8 * 7;
                iVar5 = CM_ROW((u32)iVar5, sVar8);
                goto LAB_0068a53f;
              }
              if (self_mode == 2) {
                uVar1 = sVar8 * 7;
                cVar4 = *(volatile s8 *)(CM_ROW((u32)iVar5, sVar8) + 0x15);
                goto LAB_0068a82e;
              }
            }
          }
          uVar1 = uVar1 + 1;
          iVar6 = (int)(short)uVar1;
          if (iVar10 <= iVar6) {
            return 0xff;
          }
        } while( true );
      }
    }
  }
LAB_0068a793:
  return 0xff;
}
