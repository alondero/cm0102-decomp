/* Preferred-image VA 0x0093c200, image base 0x00400000, CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Checked statement-by-statement against the farm listing. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0

extern u32 CM_CDECL cm_fn_009443a0(u32 arg1, u32 arg2);
extern unsigned long long CM_CDECL cm_fn_009445d0(u32 entry_eax, u32 entry_edx, volatile u32 owner_home, volatile u32 allocation_home);

void cm_fn_0093c200(int *param_1,int param_2,u32 param_3,u32 param_4,u32 param_5)

{
  u32 *puVar1;
  u16 *puVar2;
  short *psVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  short sVar8;
  int *piVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  short *psVar15;
  int *piVar16;
  int iVar17;
  int iVar18;
  int edx_residue;
  u32 local_20;

  local_20 = param_4;
  piVar9 = (int *)*param_1;
  if (piVar9 != (int *)0x0) {
    /* 0093c26e-0093c290: edi = iVar18, ebp/eax feed iVar17 */
    iVar18 = (int)*(short *)(param_2 + 0x36) + (int)*(short *)((int)piVar9 + 0x36);
    iVar17 = (int)*(short *)(param_2 + 0x34) + (int)(short)piVar9[0xd];
    iVar11 = (int)cm_fn_009443a0((u32)*piVar9,(u32)(iVar18 * 0xd + 0x34));
    iVar4 = iVar11 + 0x10 + iVar18 * 4;
    iVar5 = iVar4 + 0x10 + iVar18 * 4;
    iVar6 = iVar5 + 8 + iVar18 * 2;
    iVar7 = iVar6 + 8 + iVar18 * 2;
    iVar12 = (int)cm_fn_009443a0((u32)*piVar9,(u32)(iVar17 * 4));
    iVar10 = iVar12 + iVar17 * 2;
    *(int *)((int)piVar9 + 6) = (int)(short)iVar18;
    *(short *)(piVar9 + 1) = (short)iVar17;
    /* copy loop 0093c2f5 */
    if (0 < *(short *)((int)piVar9 + 0x36)) {
      iVar14 = 0;
      do {
        puVar1 = (u32 *)(iVar4 + iVar14 * 4);
        iVar13 = iVar14 + 1;
        *(u32 *)((int)puVar1 + (iVar11 - iVar4)) = *(u32 *)(piVar9[0x13] + iVar14 * 4);
        *puVar1 = *(u32 *)(piVar9[0x14] + -4 + iVar13 * 4);
        *(u8 *)(iVar7 + -1 + iVar13) = *(u8 *)(iVar14 + piVar9[0x12]);
        iVar14 = iVar13;
      } while (iVar13 < *(short *)((int)piVar9 + 0x36));
    }
    /* 0093c333-0093c373, taken when (param_3 & 0x200)==0 */
    if ((param_3 & 0x200) == 0) {
      *(u32 *)(iVar11 + iVar18 * 4) =
           *(u32 *)(piVar9[0x13] + *(short *)((int)piVar9 + 0x36) * 4);
      *(u32 *)(iVar4 + iVar18 * 4) =
           *(u32 *)(piVar9[0x14] + *(short *)((int)piVar9 + 0x36) * 4);
      *(u32 *)(iVar11 + 4 + iVar18 * 4) =
           *(u32 *)(piVar9[0x13] + 4 + *(short *)((int)piVar9 + 0x36) * 4);
      *(u32 *)(iVar4 + 4 + iVar18 * 4) =
           *(u32 *)(piVar9[0x14] + 4 + *(short *)((int)piVar9 + 0x36) * 4);
    }
    /* 0093c377 onward: (param_3 & 2) */
    if ((param_3 & 2) == 0) {
      local_20 = (u32)(*(int *)(piVar9[0x13] + (int)param_4 * 4) -
                 *(int *)(*(int *)(param_2 + 0x4c) + (int)param_5 * 4));
      param_4 = (u32)(*(int *)(piVar9[0x14] + (int)param_4 * 4) -
                 *(int *)(*(int *)(param_2 + 0x50) + (int)param_5 * 4));
    }
    else {
      param_4 = param_5;
      if ((param_3 & 4) != 0) {
        local_20 = (local_20 + 0x20) & 0xffffffc0;
        param_4 = (param_5 + 0x20) & 0xffffffc0;
      }
    }
    /* loop 0093c40f */
    param_5 = (u32)*(short *)((int)piVar9 + 0x36);
    iVar14 = 0;
    if ((int)param_5 < iVar18) {
      piVar16 = (int *)(iVar4 + (int)param_5 * 4);
      do {
        *(u32 *)((int)piVar16 + (iVar11 - iVar4)) =
             (u32)(*(int *)(*(int *)(param_2 + 0x4c) + iVar14 * 4) + (int)local_20);
        *piVar16 = (int)((u32)(*(int *)(*(int *)(param_2 + 0x50) + iVar14 * 4) + (int)param_4));
        param_5 = param_5 + 1;
        *(u8 *)(iVar7 + -1 + param_5) = *(u8 *)(*(int *)(param_2 + 0x48) + iVar14);
        piVar16 = piVar16 + 1;
        iVar14 = iVar14 + 1;
      } while ((int)param_5 < iVar18);
    }
    /* loop 0093c472 */
    param_5 = 0;
    if (0 < (short)piVar9[0xd]) {
      do {
        puVar2 = (u16 *)(iVar10 + (int)param_5 * 2);
        *(u16 *)((iVar12 - iVar10) + (int)puVar2) =
             *(u16 *)(piVar9[0xe] + (int)param_5 * 2);
        iVar14 = (int)param_5 * 2;
        param_5 = param_5 + 1;
        *puVar2 = *(u16 *)(piVar9[0xf] + iVar14);
      } while ((int)param_5 < (int)(short)piVar9[0xd]);
    }
    /* loop 0093c4d2 */
    iVar14 = (int)(short)piVar9[0xd];
    if (iVar14 < iVar17) {
      param_5 = 0;
      param_4 = (u32)(iVar17 - iVar14);
      psVar15 = (short *)(iVar10 + iVar14 * 2);
      do {
        *(short *)((int)psVar15 + (iVar12 - iVar10)) =
             (short)(*(short *)((int)param_5 + *(int *)(param_2 + 0x38)) + *(short *)((int)piVar9 + 0x36));
        psVar3 = (short *)((int)param_5 + *(int *)(param_2 + 0x3c));
        param_5 = param_5 + 2;
        *psVar15 = (short)(*psVar3 + *(short *)((int)piVar9 + 0x36));
        param_4 = param_4 + -1;
        psVar15 = psVar15 + 1;
      } while (param_4 != 0);
    }
    /* EDX residue at the 0093c52e call: loop 0093c4d2 leaves edx=0 when run,
       otherwise edx=(short)old f34 (= iVar14). */
    edx_residue = (iVar14 < iVar17) ? 0 : iVar14;
    iVar14 = *piVar9;
    *(short *)((int)piVar9 + 0x36) = (short)iVar18;
    *(short *)(piVar9 + 0xd) = (short)iVar17;
    if (piVar9[0x13] == *(int *)(iVar14 + 0x54)) {
      *(u32 *)(iVar14 + 0x84) = 1;
    }
    else {
      /* call 0093c52e: EAX=owner, EDX=residue, stack(owner, allocation) */
      cm_fn_009445d0((u32)iVar14,(u32)piVar9[0x13],(volatile u32)(u32)iVar14,(volatile u32)(u32)edx_residue);
    }
    if (piVar9[0xe] != (int)piVar9 + 0x12) {
      iVar14 = *(volatile int *)piVar9;
      /* call 0093c544: EAX=piVar9[0xe], EDX=owner, stack(owner, allocation) */
      cm_fn_009445d0((u32)iVar14,(u32)piVar9[0xe],(volatile u32)(u32)piVar9[0xe],(volatile u32)(u32)iVar14);
    }
    piVar9[0x13] = iVar11;
    piVar9[0x14] = iVar4;
    piVar9[0x10] = iVar5;
    piVar9[0x11] = iVar6;
    piVar9[0x12] = iVar7;
    piVar9[0xe] = iVar12;
    piVar9[0xf] = iVar10;
    *(u16 *)((int)piVar9 + 0x32) = *(u16 *)(param_2 + 0x32);
    return;
  }
  /* *param_1 == 0 path, 0093c212-0093c269 */
  if ((param_3 & 2) != 0) {
    if ((param_3 & 4) != 0) {
      param_4 = (param_4 + 0x20) & 0xffffffc0;
      param_5 = (param_5 + 0x20) & 0xffffffc0;
    }
    if ((param_4 != 0) || (param_5 != 0)) {
      sVar8 = *(short *)(param_2 + 0x36);
      iVar10 = 0;
      if (0 < sVar8) {
        do {
          piVar9 = (int *)(*(int *)(param_2 + 0x4c) + iVar10 * 4);
          *piVar9 = *piVar9 + (int)param_4;
          piVar9 = (int *)(*(int *)(param_2 + 0x50) + iVar10 * 4);
          *piVar9 = *piVar9 + (int)param_5;
          iVar10 = iVar10 + 1;
        } while (iVar10 < sVar8);
      }
    }
  }
  *param_1 = (int)param_2;
  return;
}
