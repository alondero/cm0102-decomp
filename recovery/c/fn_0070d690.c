/* WIP: original __ftol calls at 0x0070d783, 0x0070d796, and 0x0070d879 consume ST0.
   The plain-C declaration cannot supply those implicit x87 inputs faithfully. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_00ae24d0 CM_GLOBAL(volatile u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(volatile s32, 0x00b67a34)
#define s_E__dev_CM3_cm3_00_01_cm3_code_ma_00a16058 ((char *)0x00a16058)
#define s_Error_009870e0 ((char *)0x009870e0)
#define s_v_s__s__s__d_009870e8 ((char *)0x009870e8)
extern void cm_fn_005a8f60(void);
extern void cm_fn_005cc7d0(void);
extern void CM_CDECL cm_fn_005e8290(const char * param_1,const char * param_2,int param_3);
extern s16 CM_THISCALL cm_fn_006c38c0(u32 self,u32 param_1);
extern s8 CM_STDCALL cm_fn_006cb650(s8 param_1);
extern s32 CM_CDECL cm_fn_0090cfc0(s32 param_1);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern /* Library Function - Single Match cm_fn_00945640 Library: Visual Studio */ long long cm_fn_00945640(void);
extern void CM_CDECL cm_fn_00945934(u32 param_1);

short CM_THISCALL cm_fn_0070d690(u32 param_1,char *param_2,char *param_3,char *param_4)

{
  u16 uVar1;
  char cVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  u32 uVar6;
  int iVar7;
  int iVar8;
  char cVar9;
  short sVar10;
  short sVar11;
  u32 game_context;
  u32 mapped_target;
  s32 switch_index;
  u32 local_204;
  u8 scratch[0x200];
  u8 *local_200 = scratch;
  u8 *local_100 = scratch + 0x100;
  
  sVar11 = 0;
  local_204 = 0;
  game_context = *(volatile u32 *)(param_1 + 0x19e);
  cVar9 = *(volatile s8 *)(game_context + 0x8ea9);
  *(volatile s8 *)param_3 = '\0';
  *(volatile s8 *)param_4 = '\0';
  if ((*(volatile s8 *)param_2 != '%') && (*(volatile s8 *)param_2 != '\x15')) {
    cVar2 = *(volatile s8 *)(param_1 + 0x102);
    if (*(volatile s8 *)(param_1 + 0x27) == '\x01') {
      if (((cVar2 < '\x02') || ('\x06' < cVar2)) ||
         ((*(volatile s8 *)(param_1 + 0x103) < '\n' || ('\v' < *(volatile s8 *)(param_1 + 0x103))))) {
LAB_0070d76f:
        uVar6 = cm_fn_00945640();
        iVar7 = cm_fn_0090cfc0(uVar6);
        uVar6 = cm_fn_00945640();
        iVar8 = cm_fn_0090cfc0(uVar6);
        if (iVar7 < iVar8) {
          *(volatile char *)param_4 = '\0';
          *(volatile char *)param_3 = '\0';
          return -0x14;
        }
      }
    }
    else if ((((cVar2 < '\x02') || ('\x06' < cVar2)) || (*(volatile s8 *)(param_1 + 0x103) < '\0')) ||
            ('\x01' < *(volatile s8 *)(param_1 + 0x103))) goto LAB_0070d76f;
  }
  iVar7 = *(volatile s32 *)(*(volatile u32 *)(param_1 + 0x19e) + 0xf582);
  if (((iVar7 == 0) && (cVar2 = *param_2, cVar2 != '\x15')) &&
     ((cVar2 != '%' && (cVar2 = cm_fn_006cb650(cVar2), cVar2 == '\0')))) {
    cm_fn_00944cff(s_E__dev_CM3_cm3_00_01_cm3_code_ma_00a16058,0,0,local_100,local_200);
    uVar6 = (u32)cm_fn_0090d130();
    cm_fn_00944c9f((u32)&DAT_00ae24d0,(u32)s_v_s__s__s__d_009870e8,uVar6,(u32)local_100,(u32)local_200,0x457f);
    cm_fn_005e8290(s_Error_009870e0,&DAT_00ae24d0,0);
    DAT_00b67a34 = iVar7;
    cm_fn_005cc7d0();
    cm_fn_005a8f60();
                    /* NOTE: Subroutine does not return */
    cm_fn_00945934(0xffffffff);
  }
  if (iVar7 == param_1) {
    sVar10 = *(volatile s16 *)(iVar7 + 0x19c);
  }
  else {
    sVar10 = 0;
  }
  sVar3 = cm_fn_00945640();
  sVar3 = cm_fn_0090cfc0(((10000 - sVar3) * (10000 - sVar3)) / 50000);
  if (iVar7 == 0) {
    game_context = *(volatile u32 *)(param_1 + 0x19e);
    iVar7 = (int)*(volatile s16 *)(param_1 + 0x19a);
    iVar7 -= (int)*(volatile s16 *)(game_context + 0x8ed8);
    uVar1 = (u16)(iVar7 >> 0x1f);
    sVar4 = ((u16)iVar7 ^ uVar1) - uVar1;
    if (0xb4 < sVar4) {
      sVar4 = 0x168 - sVar4;
    }
    iVar7 = 5 - *(volatile s8 *)(*(volatile s32 *)(param_1 + 0x19e) + 0x8ea9);
    if (iVar7 < 1) {
      iVar7 = 1;
    }
    local_204 = cm_fn_0090cfc0((int)(short)((short)((0xb4 - sVar4) * 2) / (short)iVar7));
  }
    game_context = *(volatile u32 *)(param_1 + 0x19e);
    cVar2 = *(volatile s8 *)(game_context + 0x8eaa);
    sVar4 = (short)(char)(cVar2 - 1);
  sVar4 = sVar4 * sVar4 * sVar4;
  sVar5 = cm_fn_006c38c0(game_context,param_1);
  if (sVar5 == 1) {
    cVar2 = *(volatile s8 *)(param_1 + 0x102);
    if (*(volatile s8 *)(param_1 + 0x27) == '\x01') {
      if ((('\x01' < cVar2) && (cVar2 < '\a')) &&
         (('\t' < *(volatile s8 *)(param_1 + 0x103) && (*(volatile s8 *)(param_1 + 0x103) < '\f')))) {
LAB_0070d9be:
        cVar9 = ((cVar9 != '\x04') - 1U & 0xd8) + 0x29;
        *(volatile char *)param_2 = cVar9;
        sVar11 = (*(volatile s16 *)(param_1 + 0x11a + cVar9 * 2) - sVar3) - sVar10;
        goto switchD_0070d986_caseD_d;
      }
    }
    else if (((('\x01' < cVar2) && (cVar2 < '\a')) && (-1 < *(volatile s8 *)(param_1 + 0x103))) &&
            (*(volatile s8 *)(param_1 + 0x103) < '\x02')) goto LAB_0070d9be;
  }
  switch_index = (s32)*(volatile s8 *)param_2 - 0xc;
  if ((u32)switch_index <= 0x26u) {
    u8 map = *(volatile u8 *)(0x0070de54u + (u32)switch_index);
    mapped_target = *(volatile u32 *)(0x0070de18u + (u32)map * 4u);
    switch (mapped_target) { default: break; }
  }
switch(*(volatile s8 *)param_2) {
  case '\f':
    sVar11 = (((*(volatile s16 *)(param_1 + 0x132) + sVar4 * -0x14) - sVar3) - local_204) - sVar10;
    break;
  case '\x13':
    sVar11 = (*(volatile s16 *)(param_1 + 0x140) - sVar3) - sVar10;
    if ((sVar11 < 2) && (iVar7 = cm_fn_0090cfc0(2), iVar7 != 0)) {
      sVar11 = (*(volatile s16 *)(param_1 + 0x174) - sVar3) - sVar10;
      *(volatile char *)param_2 = '-';
    }
    break;
  case '\x15':
    sVar11 = (((*(volatile s16 *)(param_1 + 0x144) + sVar4 * -0x14) - sVar3) - local_204) - sVar10;
    break;
  case '!':
    sVar11 = (*(volatile s16 *)(param_1 + 0x15c) - sVar3) - sVar10;
    if ((sVar11 < 2) && (iVar7 = cm_fn_0090cfc0(2), iVar7 != 0)) {
      sVar11 = (*(volatile s16 *)(param_1 + 0x174) - sVar3) - sVar10;
      *(volatile char *)param_2 = '-';
    }
    break;
  case '\"':
    sVar11 = (*(volatile s16 *)(param_1 + 0x15e) - sVar3) - sVar10;
    if ((sVar11 < 2) && (iVar7 = cm_fn_0090cfc0(2), iVar7 != 0)) {
      sVar11 = (*(volatile s16 *)(param_1 + 0x174) - sVar3) - sVar10;
      *(volatile char *)param_2 = '-';
    }
    break;
  case '%':
    sVar11 = *(volatile s16 *)(param_1 + 0x164);
    goto LAB_0070db3b;
  case '*':
    sVar11 = (*(volatile s16 *)(param_1 + 0x16e) - sVar3) - sVar10;
    break;
  case '+':
    sVar11 = (*(volatile s16 *)(param_1 + 0x170) - sVar3) - sVar10;
    break;
  case ',':
    sVar11 = (*(volatile s16 *)(param_1 + 0x172) - sVar3) - sVar10;
    break;
  case '-':
    sVar11 = (*(volatile s16 *)(param_1 + 0x174) - sVar3) - sVar10;
    break;
  case '.':
    sVar11 = (*(volatile s16 *)(param_1 + 0x176) - sVar3) - sVar10;
    break;
  case '/':
    sVar11 = (*(volatile s16 *)(param_1 + 0x178) - sVar3) - sVar10;
    break;
  case '1':
    sVar11 = *(volatile s16 *)(param_1 + 0x17c);
    goto LAB_0070db3b;
  case '2':
    sVar11 = *(volatile s16 *)(param_1 + 0x17e);
LAB_0070db3b:
    sVar11 = sVar11 - sVar10;
  }
switchD_0070d986_caseD_d:
  switch_index = (s32)*(volatile s8 *)param_2 - 1;
  if ((u32)switch_index <= 0x31u) {
    u8 map = *(volatile u8 *)(0x0070debcU + (u32)switch_index);
    mapped_target = *(volatile u32 *)(0x0070de7cu + (u32)map * 4u);
    switch (mapped_target) { default: break; }
  }
  switch(*(volatile s8 *)param_2) {
  case '\x01':
    cVar9 = cm_fn_0090cfc0(2);
    *(volatile char *)param_4 = cVar9 + '\x02';
    *(volatile char *)param_3 = '\x01';
    return sVar11;
  default:
    *(volatile char *)param_4 = '\0';
    *(volatile char *)param_3 = '\0';
    cm_fn_00944cff(s_E__dev_CM3_cm3_00_01_cm3_code_ma_00a16058,0,0,local_200,local_100);
    uVar6 = (u32)cm_fn_0090d130();
    cm_fn_00944c9f((u32)&DAT_00ae24d0,(u32)s_v_s__s__s__d_009870e8,uVar6,(u32)local_200,(u32)local_100,0x4651);
    cm_fn_005e8290(s_Error_009870e0,&DAT_00ae24d0,0);
    DAT_00b67a34 = 0;
    return sVar11;
  case '\f':
    *(volatile char *)param_4 = '\0';
    *(volatile char *)param_3 = '\0';
    return sVar11;
  case '\x13':
  case '!':
  case '-':
    *(volatile char *)param_4 = '\x02';
    *(volatile char *)param_3 = '\x02';
    return sVar11;
  case '\x15':
    *(volatile char *)param_4 = '\x02';
    *(volatile char *)param_3 = '\x01';
    return sVar11;
  case '\"':
    *(volatile char *)param_4 = '\x02';
    cVar9 = cm_fn_0090cfc0(2);
    *(volatile char *)param_3 = cVar9 + '\x01';
    return sVar11;
  case '$':
    *(volatile char *)param_4 = '\x02';
    *(volatile char *)param_3 = '\0';
    return sVar11;
  case '%':
    *(volatile char *)param_4 = '\x02';
    *(volatile char *)param_3 = '\0';
    return sVar11;
  case ')':
    cVar9 = cm_fn_0090cfc0(2);
    *(volatile char *)param_4 = cVar9 + '\x02';
    *(volatile char *)param_3 = '\x01';
    return sVar11;
  case '*':
    *(volatile char *)param_4 = '\x02';
    cVar9 = cm_fn_0090cfc0(2);
    *(volatile char *)param_3 = cVar9;
    return sVar11;
  case '+':
    *(volatile char *)param_4 = '\x02';
    *(volatile char *)param_3 = '\0';
    return sVar11;
  case ',':
    *(volatile char *)param_4 = '\x02';
    *(volatile char *)param_3 = '\x05';
    return sVar11;
  case '.':
    *(volatile char *)param_4 = '\x02';
    cVar9 = cm_fn_0090cfc0(2);
    *(volatile char *)param_3 = cVar9;
    return sVar11;
  case '/':
    uVar6 = 4;
    *(volatile char *)param_4 = '\x02';
    break;
  case '1':
    uVar6 = 4;
    *(volatile char *)param_4 = '\0';
    break;
  case '2':
    uVar6 = 2;
    *(volatile char *)param_4 = '\x02';
  }
  cVar9 = cm_fn_0090cfc0(uVar6);
  *(volatile char *)param_3 = cVar9 + '\x01';
  return sVar11;
}
