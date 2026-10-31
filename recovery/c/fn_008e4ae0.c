/* Draft for preferred-image VA 0x008e4ae0, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
/* WIP: capped after repeated case-0 table-read fault-address mismatch. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_00991944 CM_GLOBAL(u8, 0x00991944)
#define DAT_00ae23b4 CM_GLOBAL(volatile u32, 0x00ae23b4)
#define DAT_00ae23d0 CM_GLOBAL(volatile u32, 0x00ae23d0)
#define DAT_00ae2d14 CM_GLOBAL(volatile u32, 0x00ae2d14)
#define _s_00a9e09c ((char *)0x00a9e09c)
#define _s___00a9e0d8 ((char *)0x00a9e0d8)
#define _s___Reason_009a9f28 ((char *)0x009a9f28)
#define s_does_not_want_to_lose_the_opport_00a9df68 ((char *)0x00a9df68)
#define s_doesn_t_want_to_drop_down_to_a_l_00a9ded4 ((char *)0x00a9ded4)
#define s_has_only_recently_joined_current_00a9e008 ((char *)0x00a9e008)
#define s_is_concerned_about_the_differenc_00a9ddcc ((char *)0x00a9ddcc)
#define s_is_currently_very_happy_with_his_00a9e050 ((char *)0x00a9e050)
#define s_is_not_prepared_to_move_abroad_t_00a9df30 ((char *)0x00a9df30)
#define s_is_not_prepared_to_move_unless_h_00a9de90 ((char *)0x00a9de90)
#define s_is_not_yet_ready_to_come_to_a_de_00a9dfcc ((char *)0x00a9dfcc)
#define s_is_unsure_whether_the_move_would_00a9dd8c ((char *)0x00a9dd8c)
#define s_is_unwilling_to_move_to_a_club_i_00a9de08 ((char *)0x00a9de08)
#define s_is_unwilling_to_move_to_a_club_i_00a9de4c ((char *)0x00a9de4c)
#define s_revealed_that_he_would_find_it_v_00a9e124 ((char *)0x00a9e124)
#define s_wants_to_wait_until_the_end_of_t_00a9e17c ((char *)0x00a9e17c)
extern s32 CM_CDECL cm_fn_0066f4e0(u8 *destination, const char *format, ...);
extern u32 CM_CDECL cm_fn_00944c9f(u8 *destination, const volatile u8 *format, ...);

u8 CM_STDCALL cm_fn_008e4ae0(int param_1,int param_2,u32 param_3,u32 param_4,u32 param_5,
                            double param_6,u8 *param_7)

{
  char cVar1;
  u8 *puVar2;
  u32 uVar3;
  int iVar4;
  int iVar5;
  u8 bVar6;
  u8 bVar7;
  char *pcVar8;
  u8 local_4e4 [250];
  u8 local_3ea [1002];
  
  (void)param_3;
  (void)param_4;
  bVar6 = 0;
  bVar7 = false;
  puVar2 = local_4e4;
  iVar4 = 5;
  do {
    *puVar2 = 0;
    puVar2 = puVar2 + 0xfa;
    iVar4 = iVar4 + -1;
  } while (iVar4 != 0);
  u32 table_base = DAT_00ae23b4;
  if ((-1 < param_2) && (-1 < param_1)) {
    u32 first_record = *(volatile u32 *)(table_base + (u32)param_1 * 0x245u + 0x53u);
    u32 second_record = *(volatile u32 *)(table_base + (u32)param_2 * 0x245u + 0x53u);
    bVar7 = first_record == second_record;
  }
  if ((param_5 & 0x80000000) != 0) {
    cm_fn_0066f4e0(param_7,s_wants_to_wait_until_the_end_of_t_00a9e17c);
    return 1;
  }
  if (((param_5 & 8) == 0) || (param_1 < 0)) {
    if (((param_5 & 0x20) != 0) && (-1 < param_1)) {
      iVar4 = table_base + (u32)param_1 * 0x245u;
      iVar5 = (int)*(char *)(iVar4 + 0x52);
      iVar4 = iVar4 + 0x38;
      pcVar8 = (char *)0x00a9e0d8;
      goto LAB_008e4c4d;
    }
    if (((param_5 & 0x400) != 0) && (-1 < param_1)) {
      iVar4 = table_base + (u32)param_1 * 0x245u;
      iVar5 = (int)*(char *)(iVar4 + 0x52);
      iVar4 = iVar4 + 0x38;
      pcVar8 = (char *)0x00a9e09c;
      goto LAB_008e4c4d;
    }
    if (((param_5 & 0x1000) != 0) && (-1 < param_1)) {
      iVar4 = table_base + (u32)param_1 * 0x245u;
      iVar5 = (int)*(char *)(iVar4 + 0x52);
      iVar4 = iVar4 + 0x38;
      pcVar8 = s_is_currently_very_happy_with_his_00a9e050;
      goto LAB_008e4c4d;
    }
    if (((param_5 & 0x100000) != 0) && (-1 < param_1)) {
      iVar4 = table_base + (u32)param_1 * 0x245u;
      iVar5 = (int)*(char *)(iVar4 + 0x52);
      iVar4 = iVar4 + 0x38;
      pcVar8 = s_has_only_recently_joined_current_00a9e008;
      goto LAB_008e4c4d;
    }
    if (((param_5 & 1) != 0) && (2.75 <= param_6)) {
      cm_fn_0066f4e0(local_4e4,s_is_not_yet_ready_to_come_to_a_de_00a9dfcc);
      goto LAB_008e4c87;
    }
  }
  else {
    iVar4 = table_base + (u32)param_1 * 0x245u;
    iVar5 = (int)*(char *)(iVar4 + 0x52);
    iVar4 = iVar4 + 0x38;
    pcVar8 = s_revealed_that_he_would_find_it_v_00a9e124;
LAB_008e4c4d:
    cm_fn_0066f4e0(local_4e4,pcVar8,iVar5,iVar5,iVar4);
LAB_008e4c87:
    bVar6 = 1;
  }
  if ((((param_5 & 0x400000) != 0) && (-1 < param_1)) &&
     ((uVar3 = *(u32 *)(DAT_00ae23b4 + param_1 * 0x245 + 0x1db), uVar3 < 0x80000000 ||
      (uVar3 = *(u32 *)(DAT_00ae2d14 + param_1 * 6), -1 < (int)uVar3)))) {
    iVar4 = (int)*(char *)(uVar3 * 0x6b + 0x37 + DAT_00ae23d0);
    cm_fn_0066f4e0(local_4e4 + (char)bVar6 * 0xfa,s_does_not_want_to_lose_the_opport_00a9df68,iVar4,
                 iVar4,uVar3 * 0x6b + DAT_00ae23d0 + 4);
    bVar6 = bVar6 + 1;
  }
  if (1 < bVar6) goto LAB_008e4eba;
  if ((bVar7) || ((param_5 & 0x80000) == 0)) {
    if ((param_5 & 0x8000) != 0) {
      if (((((param_2 < 0) || (param_1 < 0)) ||
           (iVar4 = *(int *)(DAT_00ae23b4 + param_1 * 0x245 + 0x57), iVar4 == 0)) ||
          ((iVar5 = *(int *)(DAT_00ae23b4 + param_2 * 0x245 + 0x57), iVar5 == 0 ||
           (*(int *)(iVar5 + 0x5d) != *(int *)(iVar4 + 0x5d))))) ||
         (*(short *)(iVar4 + 0x69) <= *(short *)(iVar5 + 0x69))) {
        pcVar8 = s_is_not_prepared_to_move_unless_h_00a9de90;
      }
      else {
        pcVar8 = s_doesn_t_want_to_drop_down_to_a_l_00a9ded4;
      }
      puVar2 = local_4e4 + (char)bVar6 * 0xfa;
      goto LAB_008e4dae;
    }
  }
  else {
    pcVar8 = s_is_not_prepared_to_move_abroad_t_00a9df30;
    puVar2 = local_4e4 + (char)bVar6 * 0xfa;
LAB_008e4dae:
    cm_fn_0066f4e0(puVar2,pcVar8);
    bVar6 = bVar6 + 1;
  }
  if (1 < bVar6) goto LAB_008e4eba;
  if (((bVar7) || ((param_5 & 0x1000000) == 0)) ||
     ((param_2 < 0 || (iVar4 = *(int *)(DAT_00ae23b4 + param_2 * 0x245 + 0x53), iVar4 == 0)))) {
    if (((((param_5 & 0x2000000) != 0) && (-1 < param_2)) &&
        (iVar4 = *(int *)(DAT_00ae23b4 + param_2 * 0x245 + 0x53), iVar4 != 0)) &&
       (iVar4 = *(int *)(iVar4 + 0x71), iVar4 != 0)) {
      cVar1 = *(char *)(iVar4 + 0x1e);
      pcVar8 = s_is_unwilling_to_move_to_a_club_i_00a9de08;
      goto LAB_008e4e41;
    }
    if ((!bVar7) && ((param_5 & 0x10000) != 0)) {
      cm_fn_0066f4e0(local_4e4 + (char)bVar6 * 0xfa,s_is_concerned_about_the_differenc_00a9ddcc);
      goto LAB_008e4e86;
    }
  }
  else {
    cVar1 = *(char *)(iVar4 + 0x37);
    pcVar8 = s_is_unwilling_to_move_to_a_club_i_00a9de4c;
LAB_008e4e41:
    cm_fn_0066f4e0(local_4e4 + (char)bVar6 * 0xfa,pcVar8,(int)cVar1,(int)cVar1,iVar4 + 4);
LAB_008e4e86:
    bVar6 = bVar6 + 1;
  }
  if ((bVar6 < 2) && ((param_5 & 0x20000000) != 0)) {
    cm_fn_0066f4e0(local_4e4 + (char)bVar6 * 0xfa,s_is_unsure_whether_the_move_would_00a9dd8c);
    bVar6 = bVar6 + 1;
  }
LAB_008e4eba:
  if (bVar6 == 1) {
    cm_fn_00944c9f(param_7,&DAT_00991944,local_4e4);
  }
  else if (bVar6 == 2) {
    cm_fn_0066f4e0(param_7,(const char *)0x009a9f28,local_4e4,local_3ea);
    return 2;
  }
  return bVar6;
}
