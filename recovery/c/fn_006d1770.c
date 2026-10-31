/* Draft for preferred-image VA 0x006d1770, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_00ae24d0 (*(volatile u8 *)0x00ae24d0)
#define DAT_00b67a34 (*(volatile u32 *)0x00b67a34)
#define OUT8(offset, value) (*(volatile u8 *)((u32)param_2 + (u32)(offset)) = (u8)(value))
#define s_E__dev_CM3_cm3_00_01_cm3_code_ma_00a15884 ((char *)0x00a15884)
#define s_Error_009870e0 ((char *)0x009870e0)
#define s_v_s__s__s__d_009870e8 ((char *)0x009870e8)
extern void cm_fn_005e8290(const char * param_1,const char * param_2,int param_3);
extern u32 CM_FASTCALL cm_fn_00717410(u32 object);
extern int cm_fn_0090cfc0(int param_1);
extern char * cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

void CM_THISCALL cm_fn_006d1770(u32 self,volatile u8 *param_2)

{
  s8 cVar1;
  s8 cVar2;
  s8 cVar3;
  s8 cVar4;
  u16 uVar5;
  u32 uVar6;
  volatile u8 *puVar7;
  int iVar8;
  int iVar9;
  u32 uVar10;
  s8 cVar11;
  u32 iVar12;
  s8 cVar13;
  short sVar14;
  s8 local_434;
  s8 local_433;
  s8 local_430;
  int local_424;
  int local_410;
  int local_40c;
  u32 local_408;
  int local_404;
  union {
    u32 alignment;
    u8 bytes[0x434];
  } scratch_union;
  u8 *scratch = scratch_union.bytes;
  volatile s8 *local_41c = (volatile s8 *)(scratch + 0x18);
  u8 *local_400 = scratch + 0x34;
  u8 *local_300 = scratch + 0x134;
  u8 *local_200 = scratch + 0x234;
  u8 *local_100 = scratch + 0x334;
  
  if (param_2 == (u8 *)0x0) {
    cm_fn_00944cff((u32)s_E__dev_CM3_cm3_00_01_cm3_code_ma_00a15884,0,0,local_300,local_400);
    uVar6 = (u32)cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,(u32)s_v_s__s__s__d_009870e8,uVar6,local_300,local_400,0x4383);
    cm_fn_005e8290(s_Error_009870e0,(const char *)0x00ae24d0,0);
    DAT_00b67a34 = 0;
  }
  puVar7 = param_2 + 7;
  *param_2 = *(volatile u8 *)(self + 0x8ea7U);
  param_2[1] = *(volatile u8 *)(self + 0x8ea8U);
  iVar12 = 2;
  do {
    iVar9 = 0xb;
    do {
      *puVar7 = 0;
      puVar7 = puVar7 + 6;
      iVar9 = iVar9 + -1;
    } while (iVar9 != 0);
    iVar12 = iVar12 + -1;
  } while (iVar12 != 0);
  local_433 = '\0';
  local_404 = 0;
  local_410 = 0;
  do {
    local_430 = '\0';
    local_424 = 0;
    local_40c = 0;
    do {
      cVar13 = (char)(local_410 / 9) + '\x01';
      cVar11 = (((char)(local_40c / 0xc) + (char)(local_40c >> 0x1f)) -
               (char)((long long)local_40c * 0x2aaaaaab >> 0x3f)) + '\x01';
      *(volatile u32 *)(void *)local_41c = 0;
      *(volatile u32 *)(void *)(local_41c + 4) = 0;
      iVar12 = self + 0x215eU + (u32)(local_424 + local_404) * 0x5aU;
      local_41c[8] = 0;
      cVar1 = cm_fn_0090cfc0(2);
      iVar9 = (int)cVar1;
      local_434 = '\0';
      if (0 < *(volatile s8 *)(iVar12 + 0x58U + (u32)iVar9)) {
        do {
          local_408 = iVar9 * 0xb;
      puVar7 = (volatile u8 *)*(volatile u32 *)(iVar12 + ((u32)(s32)local_434 + local_408) * 4U);
          cVar3 = (s8)*(volatile s8 *)(puVar7 + 0x19);
          if (cVar3 < '\0') {
            cm_fn_00944cff((u32)s_E__dev_CM3_cm3_00_01_cm3_code_ma_00a15884,0,0,local_400,local_300);
            uVar6 = (u32)cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0,(u32)s_v_s__s__s__d_009870e8,uVar6,local_400,local_300,0x43a7);
            cm_fn_005e8290(s_Error_009870e0,(const char *)0x00ae24d0,0);
            DAT_00b67a34 = 0;
          }
          else if ((u32)puVar7 == *(volatile u32 *)(self + 0xf582U)) {
            iVar8 = (int)cVar3 + local_408;
            OUT8((u32)iVar8 * 6U + 2U, cVar13);
            OUT8((u32)iVar8 * 6U + 3U, cVar11);
            OUT8((u32)iVar8 * 6U + 4U, *puVar7);
            uVar5 = (u16)cm_fn_00717410((u32)puVar7);
            *(volatile u16 *)((u32)param_2 + (u32)iVar8 * 6U + 5U) = uVar5;
            OUT8((u32)iVar8 * 6U + 7U, 1);
            local_41c[4] = 1;
          }
          else {
            sVar14 = 0;
LAB_006d1987:
            do {
              cVar4 = (s8)cm_fn_0090cfc0(3);
              cVar2 = (s8)cm_fn_0090cfc0(3);
              sVar14 = sVar14 + 1;
              iVar8 = (int)*(volatile u32 *)(self + 0xf582U);
              if (((iVar8 != 0) && (*(volatile s8 *)((u32)iVar8 + 0x102U) == local_433)) &&
                 (*(volatile s8 *)((u32)iVar8 + 0x103U) == local_430)) {
                if ((cVar4 == '\x01') && (cVar2 == '\x01')) goto LAB_006d1987;
              }
            } while ((int)sVar14 / 0x32 < (int)local_41c[cVar4 * 3 + (int)cVar2]);
            iVar8 = (int)cVar3 + local_408;
            OUT8((u32)iVar8 * 6U + 2U, cVar4 + cVar13 + -1);
            OUT8((u32)iVar8 * 6U + 3U, cVar2 + cVar11 + -1);
            OUT8((u32)iVar8 * 6U + 4U, *puVar7);
            uVar5 = (u16)cm_fn_00717410((u32)puVar7);
            *(volatile u16 *)((u32)param_2 + (u32)iVar8 * 6U + 5U) = uVar5;
            OUT8((u32)iVar8 * 6U + 7U, 1);
            local_41c[cVar4 * 3 + (int)cVar2] = '\x01';
          }
          local_434 = local_434 + '\x01';
        } while (local_434 < *(volatile s8 *)(iVar12 + 0x58U + (u32)iVar9));
      }
      local_434 = '\0';
      uVar10 = (u32)(cVar1 == '\0');
      local_408 = uVar10;
      if (0 < *(volatile s8 *)(iVar12 + 0x58U + uVar10)) {
        do {
          iVar9 = uVar10 * 0xb;
          puVar7 = (volatile u8 *)*(volatile u32 *)(iVar12 + ((u32)(s32)local_434 + (u32)iVar9) * 4U);
          cVar1 = (s8)*(volatile s8 *)(puVar7 + 0x19);
          if (cVar1 < '\0') {
            cm_fn_00944cff((u32)s_E__dev_CM3_cm3_00_01_cm3_code_ma_00a15884,0,0,local_100,local_200);
            uVar6 = (u32)cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0,(u32)s_v_s__s__s__d_009870e8,uVar6,local_100,local_200,0x43d0);
            cm_fn_005e8290(s_Error_009870e0,(const char *)0x00ae24d0,0);
            DAT_00b67a34 = 0;
          }
          else if ((u32)puVar7 == *(volatile u32 *)(self + 0xf582U)) {
            iVar9 = cVar1 + iVar9;
            OUT8((u32)iVar9 * 6U + 2U, cVar13);
            OUT8((u32)iVar9 * 6U + 3U, cVar11);
            OUT8((u32)iVar9 * 6U + 4U, *puVar7);
            uVar5 = (u16)cm_fn_00717410((u32)puVar7);
            *(volatile u16 *)((u32)param_2 + (u32)iVar9 * 6U + 5U) = uVar5;
            OUT8((u32)iVar9 * 6U + 7U, 1);
            local_41c[4] = 1;
          }
          else {
            sVar14 = 0;
LAB_006d1b6d:
            do {
              cVar3 = (s8)cm_fn_0090cfc0(3);
              cVar4 = (s8)cm_fn_0090cfc0(3);
              sVar14 = sVar14 + 1;
              iVar8 = (int)*(volatile u32 *)(self + 0xf582U);
              if ((iVar8 != 0) && (*(volatile s8 *)((u32)iVar8 + 0x102U) == local_433)) {
                if ((*(volatile s8 *)((u32)iVar8 + 0x103U) == local_430) &&
                   ((cVar3 == '\x01' && (cVar4 == '\x01')))) goto LAB_006d1b6d;
              }
            } while ((int)sVar14 / 0x32 < (int)local_41c[cVar3 * 3 + (int)cVar4]);
            iVar9 = cVar1 + iVar9;
            OUT8((u32)iVar9 * 6U + 2U, cVar3 + cVar13 + -1);
            OUT8((u32)iVar9 * 6U + 3U, cVar4 + cVar11 + -1);
            OUT8((u32)iVar9 * 6U + 4U, *puVar7);
            uVar5 = (u16)cm_fn_00717410((u32)puVar7);
            uVar10 = local_408;
            *(volatile u16 *)((u32)param_2 + (u32)iVar9 * 6U + 5U) = uVar5;
            OUT8((u32)iVar9 * 6U + 7U, 1);
            local_41c[cVar3 * 3 + (int)cVar4] = '\x01';
          }
          local_434 = local_434 + '\x01';
        } while (local_434 < *(volatile s8 *)(iVar12 + 0x58U + uVar10));
      }
      local_430 = local_430 + '\x01';
      local_424 = local_424 + 1;
      local_40c = local_40c + 0x24;
    } while (local_430 < '\f');
    local_433 = local_433 + '\x01';
    local_410 = local_410 + 0x1b;
    local_404 = local_404 + 0xc;
    if ('\b' < local_433) {
      return;
    }
  } while( true );
}
