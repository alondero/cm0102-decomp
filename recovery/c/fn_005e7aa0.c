/* Tier-2 C recovery for preferred-image VA 0x005e7aa0 (image base 0x00400000). */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
/* 32-bit wrapping add/sub, as the listing's add/sub/inc/dec do. Doing it in u32 avoids
   signed-overflow UB, so GCC cannot rewrite a bound such as (p - 1 <= i). */
#define WRAP_ADD(a, b) ((s32)((u32)(a) + (u32)(b)))
#define WRAP_SUB(a, b) ((s32)((u32)(a) - (u32)(b)))

void CM_CDECL cm_fn_005e7aa0(u32 param_1,s32 param_2,s32 param_3,s32 param_4,s32 param_5,
                             s32 param_6,s32 param_7,u16 param_8,s32 param_9)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int local_14;

  iVar1 = param_9;
  local_14 = param_2;
  iVar3 = (int)(short)((short)param_3 + 1);
  iVar4 = (int)(short)((short)param_2 + 1);
  iVar8 = WRAP_ADD((WRAP_ADD(WRAP_SUB(param_6, param_4), 1)) / 2, param_4);
  iVar5 = WRAP_ADD((WRAP_ADD(WRAP_SUB(param_7, param_5), 1)) / 2, param_5);
  if (param_2 < iVar4) {
    iVar6 = WRAP_ADD(iVar5, param_3);
    iVar2 = WRAP_SUB(iVar5, param_3);
    iVar10 = WRAP_ADD(param_2, iVar8);
    iVar9 = WRAP_SUB(iVar8, param_2);
    do {
      if ((((WRAP_SUB(param_5, 1) <= iVar6) && (iVar6 <= WRAP_ADD(param_7, 1))) && (WRAP_SUB(param_4, 1) <= iVar10)) &&
         (iVar10 <= WRAP_ADD(param_6, 1))) {
        if (param_9 != 0) {
          iVar7 = WRAP_ADD(param_3, WRAP_SUB(iVar5, param_5));
          if (iVar10 < *(volatile s32 *)(WRAP_ADD(WRAP_ADD(param_9, 8), iVar7 * 8))) {
            *(volatile s32 *)(WRAP_ADD(WRAP_ADD(param_9, 8), iVar7 * 8)) = iVar10;
          }
          if (*(volatile s32 *)(WRAP_ADD(WRAP_ADD(param_9, 0xc), iVar7 * 8)) < iVar10) {
            *(volatile s32 *)(WRAP_ADD(WRAP_ADD(param_9, 0xc), iVar7 * 8)) = iVar10;
          }
        }
        *(volatile u16 *)(param_1 + WRAP_ADD(WRAP_ADD(iVar6 * 800, local_14), iVar8) * 2) = param_8;
      }
      if (WRAP_SUB(param_5, 1) <= iVar2) {
        if (((iVar2 <= WRAP_ADD(param_7, 1)) && (WRAP_SUB(param_4, 1) <= iVar10)) && (iVar10 <= WRAP_ADD(param_6, 1))) {
          if (param_9 != 0) {
            iVar7 = WRAP_SUB(iVar2, param_5);
            if (iVar10 < *(volatile s32 *)(WRAP_ADD(WRAP_ADD(param_9, 8), iVar7 * 8))) {
              *(volatile s32 *)(WRAP_ADD(WRAP_ADD(param_9, 8), iVar7 * 8)) = iVar10;
            }
            if (*(volatile s32 *)(WRAP_ADD(WRAP_ADD(param_9, 0xc), iVar7 * 8)) < iVar10) {
              *(volatile s32 *)(WRAP_ADD(WRAP_ADD(param_9, 0xc), iVar7 * 8)) = iVar10;
            }
          }
          *(volatile u16 *)(param_1 + WRAP_ADD(WRAP_ADD(iVar2 * 800, local_14), iVar8) * 2) = param_8;
        }
        if (((iVar2 <= WRAP_ADD(param_7, 1)) && (WRAP_SUB(param_4, 1) <= iVar9)) && (iVar9 <= WRAP_ADD(param_6, 1))) {
          if (param_9 != 0) {
            iVar7 = WRAP_SUB(iVar2, param_5);
            if (iVar9 < *(volatile s32 *)(WRAP_ADD(WRAP_ADD(param_9, 8), iVar7 * 8))) {
              *(volatile s32 *)(WRAP_ADD(WRAP_ADD(param_9, 8), iVar7 * 8)) = iVar9;
            }
            if (*(volatile s32 *)(WRAP_ADD(WRAP_ADD(param_9, 0xc), iVar7 * 8)) < iVar9) {
              *(volatile s32 *)(WRAP_ADD(WRAP_ADD(param_9, 0xc), iVar7 * 8)) = iVar9;
            }
          }
          *(volatile u16 *)(param_1 + WRAP_ADD(WRAP_SUB(iVar2 * 800, local_14), iVar8) * 2) = param_8;
        }
      }
      if ((((WRAP_SUB(param_5, 1) <= iVar6) && (iVar6 <= WRAP_ADD(param_7, 1))) && (WRAP_SUB(param_4, 1) <= iVar9)) &&
         (iVar9 <= WRAP_ADD(param_6, 1))) {
        if (param_9 != 0) {
          iVar7 = WRAP_ADD(param_3, WRAP_SUB(iVar5, param_5));
          if (iVar9 < *(volatile s32 *)(WRAP_ADD(WRAP_ADD(param_9, 8), iVar7 * 8))) {
            *(volatile s32 *)(WRAP_ADD(WRAP_ADD(param_9, 8), iVar7 * 8)) = iVar9;
          }
          if (*(volatile s32 *)(WRAP_ADD(WRAP_ADD(param_9, 0xc), iVar7 * 8)) < iVar9) {
            *(volatile s32 *)(WRAP_ADD(WRAP_ADD(param_9, 0xc), iVar7 * 8)) = iVar9;
          }
        }
        *(volatile u16 *)(param_1 + WRAP_ADD(WRAP_SUB(iVar6 * 800, local_14), iVar8) * 2) = param_8;
      }
      local_14 = WRAP_ADD(local_14, 1);
      iVar10 = WRAP_ADD(iVar10, 1);
      iVar9 = WRAP_SUB(iVar9, 1);
    } while (local_14 < iVar4);
  }
  param_9 = param_3;
  if (param_3 < iVar3) {
    iVar10 = WRAP_SUB(iVar5, param_2);
    iVar4 = WRAP_ADD(param_2, iVar5);
    iVar2 = WRAP_SUB(iVar8, param_3);
    param_3 = WRAP_ADD(param_3, iVar8);
    do {
      if (((WRAP_SUB(param_5, 1) <= iVar4) && (iVar4 <= WRAP_ADD(param_7, 1))) &&
         ((WRAP_SUB(param_4, 1) <= param_3 && (param_3 <= WRAP_ADD(param_6, 1))))) {
        if (iVar1 != 0) {
          iVar6 = WRAP_ADD(WRAP_SUB(iVar5, param_5), param_2);
          if (param_3 < *(volatile s32 *)(WRAP_ADD(WRAP_ADD(iVar1, 8), iVar6 * 8))) {
            *(volatile s32 *)(WRAP_ADD(WRAP_ADD(iVar1, 8), iVar6 * 8)) = param_3;
          }
          if (*(volatile s32 *)(WRAP_ADD(WRAP_ADD(iVar1, 0xc), iVar6 * 8)) < param_3) {
            *(volatile s32 *)(WRAP_ADD(WRAP_ADD(iVar1, 0xc), iVar6 * 8)) = param_3;
          }
        }
        *(volatile u16 *)(param_1 + WRAP_ADD(WRAP_ADD(iVar4 * 800, param_9), iVar8) * 2) = param_8;
      }
      if (WRAP_SUB(param_5, 1) <= iVar10) {
        if (((iVar10 <= WRAP_ADD(param_7, 1)) && (WRAP_SUB(param_4, 1) <= param_3)) && (param_3 <= WRAP_ADD(param_6, 1))) {
          if (iVar1 != 0) {
            iVar6 = WRAP_SUB(iVar10, param_5);
            if (param_3 < *(volatile s32 *)(WRAP_ADD(WRAP_ADD(iVar1, 8), iVar6 * 8))) {
              *(volatile s32 *)(WRAP_ADD(WRAP_ADD(iVar1, 8), iVar6 * 8)) = param_3;
            }
            if (*(volatile s32 *)(WRAP_ADD(WRAP_ADD(iVar1, 0xc), iVar6 * 8)) < param_3) {
              *(volatile s32 *)(WRAP_ADD(WRAP_ADD(iVar1, 0xc), iVar6 * 8)) = param_3;
            }
          }
          *(volatile u16 *)(param_1 + WRAP_ADD(WRAP_ADD(iVar10 * 800, param_9), iVar8) * 2) = param_8;
        }
        if (((iVar10 <= WRAP_ADD(param_7, 1)) && (WRAP_SUB(param_4, 1) <= iVar2)) && (iVar2 <= WRAP_ADD(param_6, 1))) {
          if (iVar1 != 0) {
            iVar6 = WRAP_SUB(iVar10, param_5);
            if (iVar2 < *(volatile s32 *)(WRAP_ADD(WRAP_ADD(iVar1, 8), iVar6 * 8))) {
              *(volatile s32 *)(WRAP_ADD(WRAP_ADD(iVar1, 8), iVar6 * 8)) = iVar2;
            }
            if (*(volatile s32 *)(WRAP_ADD(WRAP_ADD(iVar1, 0xc), iVar6 * 8)) < iVar2) {
              *(volatile s32 *)(WRAP_ADD(WRAP_ADD(iVar1, 0xc), iVar6 * 8)) = iVar2;
            }
          }
          *(volatile u16 *)(param_1 + WRAP_ADD(WRAP_SUB(iVar10 * 800, param_9), iVar8) * 2) = param_8;
        }
      }
      if (((WRAP_SUB(param_5, 1) <= iVar4) && (iVar4 <= WRAP_ADD(param_7, 1))) &&
         ((WRAP_SUB(param_4, 1) <= iVar2 && (iVar2 <= WRAP_ADD(param_6, 1))))) {
        if (iVar1 != 0) {
          iVar6 = WRAP_ADD(WRAP_SUB(iVar5, param_5), param_2);
          if (iVar2 < *(volatile s32 *)(WRAP_ADD(WRAP_ADD(iVar1, 8), iVar6 * 8))) {
            *(volatile s32 *)(WRAP_ADD(WRAP_ADD(iVar1, 8), iVar6 * 8)) = iVar2;
          }
          if (*(volatile s32 *)(WRAP_ADD(WRAP_ADD(iVar1, 0xc), iVar6 * 8)) < iVar2) {
            *(volatile s32 *)(WRAP_ADD(WRAP_ADD(iVar1, 0xc), iVar6 * 8)) = iVar2;
          }
        }
        *(volatile u16 *)(param_1 + WRAP_ADD(WRAP_SUB(iVar4 * 800, param_9), iVar8) * 2) = param_8;
      }
      param_9 = WRAP_ADD(param_9, 1);
      param_3 = WRAP_ADD(param_3, 1);
      iVar2 = WRAP_SUB(iVar2, 1);
    } while (param_9 < iVar3);
  }
  return;
}
