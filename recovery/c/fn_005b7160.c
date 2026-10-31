/* Draft for preferred-image VA 0x005b7160, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0

u32 CM_THISCALL cm_fn_005b7160(u8 *param_1)

{
  int iVar1;
  u8 *pbVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  u8 bVar7;
  int iVar8;
  char cVar9;
  u8 bVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int local_44;
  int local_40;
  int local_3c;
  int local_38;
  int local_34;
  int local_2c;
  int local_1c;
  int local_18;
  int local_14;
  
  local_34 = 0;
  do {
    local_38 = 0;
    do {
      local_40 = 0;
      local_3c = 9;
      local_14 = 3;
      do {
        iVar5 = (local_3c + -9) / 3;
        local_44 = 0;
        local_1c = 0xc;
        local_18 = 4;
        do {
          iVar11 = (int)(short)iVar5;
          if (iVar11 < local_3c / 3) {
            iVar8 = (int)(local_1c + -0xc + ((local_1c - 0xc) >> 0x1f & 3U)) >> 2;
            iVar6 = (int)(local_1c + (local_1c >> 0x1f & 3U)) >> 2;
            iVar13 = iVar5;
            do {
              iVar12 = (int)(short)iVar8;
              if (iVar12 < iVar6) {
                iVar3 = (local_38 + local_34) * 9;
                iVar1 = local_44 + (local_40 + (local_38 + local_34) * 3) * 4;
                local_2c = iVar8;
                do {
                  pbVar2 = (u8 *)((iVar3 + iVar11) * 0xc + iVar12 + 0x5eb + param_1);
                  iVar4 = *(volatile s16 *)(param_1 + 0x164 + iVar1 * 4) * 9;
                  bVar7 = ((char)(iVar4 / 0x1b) + (char)(iVar4 >> 0x1f)) -
                          (char)((long long)iVar4 * 0x4bda12f7 >> 0x3f);
                  {
                    u8 original_pixel = *(volatile u8 *)pbVar2;
                    bVar10 = (((original_pixel ^ bVar7) & 0xf) ^ original_pixel);
                  }
                  *(volatile u8 *)pbVar2 = bVar10;
                  cVar9 = (char)((*(volatile s16 *)(param_1 + 0x166 + iVar1 * 4) * 0xc) / 0x24);
                  *(volatile u8 *)pbVar2 = (bVar10 & 0xf) | ((u8)cVar9 << 4);
                  {
                    volatile u8 *reverse_pixel = (volatile u8 *)(((iVar3 - iVar11) * 0xc - iVar12) + 0xf9e + param_1);
                    u8 unused_reverse_pixel = *reverse_pixel;
                    (void)unused_reverse_pixel;
                    *reverse_pixel = (cVar9 * -0x10 + 0xb0U) ^ ((-bVar7 - 8) & 0xf);
                  }
                  local_2c = local_2c + 1;
                  iVar12 = (int)(short)local_2c;
                } while (iVar12 < iVar6);
              }
              iVar13 = iVar13 + 1;
              iVar11 = (int)(short)iVar13;
            } while (iVar11 < local_3c / 3);
          }
          local_44 = local_44 + 1;
          local_1c = local_1c + 0xc;
          local_18 = local_18 + -1;
        } while (local_18 != 0);
        local_40 = local_40 + 1;
        local_3c = local_3c + 9;
        local_14 = local_14 + -1;
      } while (local_14 != 0);
      local_38 = local_38 + 1;
    } while (local_38 < 2);
    local_34 = local_34 + 2;
  } while (local_34 < 0x16);
  return 1;
}

