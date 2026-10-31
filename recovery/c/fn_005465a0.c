/* Draft for preferred-image VA 0x005465a0, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_00ae23bc CM_GLOBAL(volatile s32, 0x00ae23bc)
#define DAT_00ae24d0 CM_GLOBAL(volatile u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(volatile u32, 0x00b67a34)
#define s_E__dev_CM3_cm3_00_01_cm3_code_Da_009b5ac8 ((char *)0x009b5ac8)
#define s_Error_009870e0 ((char *)0x009870e0)
#define s_ball_winner_009b7e14 ((char *)0x009b7e14)
#define s_creative_forward_009b7e88 ((char *)0x009b7e88)
#define s_cultured_defender_009b7d78 ((char *)0x009b7d78)
#define s_deadly_attacker_009b7f20 ((char *)0x009b7f20)
#define s_dynamic_forward_009b7ec4 ((char *)0x009b7ec4)
#define s_dynamic_midfielder_009b7e00 ((char *)0x009b7e00)
#define s_exciting_forward_009b7e74 ((char *)0x009b7e74)
#define s_exciting_full_back_009b7d9c ((char *)0x009b7d9c)
#define s_free_scoring_midfielder_009b7e2c ((char *)0x009b7e2c)
#define s_goalpoacher_009b7ef0 ((char *)0x009b7ef0)
#define s_hard_working_midfielder_009b7de8 ((char *)0x009b7de8)
#define s_hitman_009b7ee8 ((char *)0x009b7ee8)
#define s_inspirational_midfielder_009b7e44 ((char *)0x009b7e44)
#define s_pacey_defender_009b7d8c ((char *)0x009b7d8c)
#define s_pacey_full_back_009b7db0 ((char *)0x009b7db0)
#define s_pacey_goalscorer_009b7efc ((char *)0x009b7efc)
#define s_pacey_hitman_009b7f10 ((char *)0x009b7f10)
#define s_pacey_midfielder_009b7dc0 ((char *)0x009b7dc0)
#define s_playmaker_009b7e20 ((char *)0x009b7e20)
#define s_powerful_forward_009b7e9c ((char *)0x009b7e9c)
#define s_powerhouse_forward_009b7eb0 ((char *)0x009b7eb0)
#define s_prolific_striker_009b7ed4 ((char *)0x009b7ed4)
#define s_reliable_midfielder_009b7dd4 ((char *)0x009b7dd4)
#define s_rookie_009b7d68 ((char *)0x009b7d68)
#define s_skilful_attacker_009b7e60 ((char *)0x009b7e60)
#define s_v_s__s__s__d_009870e8 ((char *)0x009870e8)
#define s_veteran_009b7d70 ((char *)0x009b7d70)
extern void cm_fn_005e8290(const char * param_1,const char * param_2,int param_3);
extern u32 cm_fn_0066f4d0(void);
extern /* WARNING: Globals starting with '_' overlap smaller symbols at the same address */ char * cm_fn_0066f4e0(int param_1,char *param_2);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

u8
cm_fn_005465a0(int param_1,u32 param_2,u32 param_3,u32 param_4,int param_5,u8 param_6
            )

{
  int iVar1;
  int iVar2;
  u32 uVar3;
  u32 uVar4;
  char *pcVar5;
  u8 local_200 [256];
  u8 local_100 [256];
  
  iVar2 = cm_fn_0066f4d0();
  if (param_5 < 0x1e) {
    cm_fn_00944cff((u32)s_E__dev_CM3_cm3_00_01_cm3_code_Da_009b5ac8,0,0,local_100,local_200);
    uVar3 = cm_fn_0090d130();
    cm_fn_00944c9f((u32)&DAT_00ae24d0,(u32)s_v_s__s__s__d_009870e8,
                   uVar3,(u32)local_100,(u32)local_200,0x543c);
    cm_fn_005e8290(s_Error_009870e0,(const char *)&DAT_00ae24d0,0);
    DAT_00b67a34 = 0;
    return 0;
  }
  if (iVar2 == 5) {
    param_6 = 1;
  }
  if ((param_3 & 0x40) == 0) {
    if ((param_2 & 0x10) == 0) {
      if ((param_3 & 2) == 0) {
        if ((param_3 & 4) == 0) {
          if (param_1 < 0) {
            return param_6;
          }
          if (((param_3 & 1) == 0) &&
             (param_1 % 3 + 0x21 < (int)*(char *)(DAT_00ae23bc + 0x18 + param_1 * 0x6e))) {
            cm_fn_0066f4e0(param_4,s_veteran_009b7d70);
            if (iVar2 != 4) {
              return param_6;
            }
            return 4;
          }
          if ((int)(param_1 + param_3) % 3 != 0) {
            return param_6;
          }
          if ((param_3 & 1) != 0) {
            return param_6;
          }
          iVar1 = DAT_00ae23bc + param_1 * 0x6e;
          if (param_1 % 3 + 0x12 <= (int)*(char *)(iVar1 + 0x18)) {
            return param_6;
          }
          if (*(char *)(iVar1 + 0x22) != '\0') {
            return param_6;
          }
          pcVar5 = s_rookie_009b7d68;
          goto LAB_00546a7b;
        }
      }
      else if ((param_3 & 4) == 0) {
        if ((param_3 & 0x200000) == 0x200000) {
          pcVar5 = s_pacey_full_back_009b7db0;
        }
        else {
          if ((param_3 & 0x10000) != 0x10000) {
            return param_6;
          }
          pcVar5 = s_exciting_full_back_009b7d9c;
        }
        goto LAB_00546972;
      }
      if ((param_3 & 0x200000) == 0x200000) {
        pcVar5 = s_pacey_defender_009b7d8c;
      }
      else {
        if ((param_3 & 0x1200) != 0x1200) {
          return param_6;
        }
        pcVar5 = s_cultured_defender_009b7d78;
      }
    }
    else {
      if ((param_3 & 0x40000) == 0x40000) {
        if ((param_3 & 0x800200) == 0x800200) {
          cm_fn_0066f4e0(param_4,s_inspirational_midfielder_009b7e44);
        }
        cm_fn_0066f4e0(param_4,s_free_scoring_midfielder_009b7e2c);
        return 0;
      }
      if ((((param_3 & 0x200) == 0) || ((param_2 & 0x800) != 0)) || ((param_2 & 0x80) != 0)) {
        if ((param_3 & 0x800000) == 0) {
          if ((param_3 & 0x102100) == 0x102100) {
            pcVar5 = s_dynamic_midfielder_009b7e00;
          }
          else if ((param_3 & 0x2000) == 0x2000) {
            pcVar5 = s_hard_working_midfielder_009b7de8;
          }
          else if ((param_3 & 0x8000) == 0x8000) {
            pcVar5 = s_reliable_midfielder_009b7dd4;
          }
          else {
            if ((param_3 & 0x200000) != 0x200000) {
              return param_6;
            }
            pcVar5 = s_pacey_midfielder_009b7dc0;
          }
        }
        else {
          pcVar5 = s_ball_winner_009b7e14;
        }
      }
      else {
        pcVar5 = s_playmaker_009b7e20;
      }
    }
  }
  else if ((param_3 & 0x2c0000) == 0x2c0000) {
    pcVar5 = s_deadly_attacker_009b7f20;
  }
  else {
    if ((param_3 & 0x240000) != 0x240000) {
      if ((param_3 & 0xc0000) == 0xc0000) {
        pcVar5 = s_goalpoacher_009b7ef0;
      }
      else {
        if ((param_3 & 0x40000) == 0x40000) {
          uVar4 = (param_1 + param_3) & 0x80000003;
          if ((int)uVar4 < 0) {
            uVar4 = ((uVar4 - 1) | 0xfffffffc) + 1;
          }
          if (uVar4 != 1) {
            cm_fn_0066f4e0(param_4,s_prolific_striker_009b7ed4);
            if (iVar2 != 4) {
              return param_6;
            }
            return 3;
          }
          pcVar5 = s_hitman_009b7ee8;
          goto LAB_00546972;
        }
        if ((param_3 & 0x102000) == 0x102000) {
          pcVar5 = s_dynamic_forward_009b7ec4;
        }
        else if ((param_3 & 0x200800) == 0x200800) {
          pcVar5 = s_powerhouse_forward_009b7eb0;
        }
        else if ((param_3 & 0x800) == 0x800) {
          pcVar5 = s_powerful_forward_009b7e9c;
        }
        else if ((param_3 & 0x1200) == 0x1200) {
          pcVar5 = s_creative_forward_009b7e88;
        }
        else {
          if ((param_3 & 0x10000) != 0x10000) {
            if ((param_3 & 0x1000) != 0x1000) {
              return param_6;
            }
            pcVar5 = s_skilful_attacker_009b7e60;
            goto LAB_00546972;
          }
          pcVar5 = s_exciting_forward_009b7e74;
        }
      }
LAB_00546a7b:
      cm_fn_0066f4e0(param_4,pcVar5);
      if (iVar2 != 4) {
        return param_6;
      }
      return 4;
    }
    if ((int)(param_1 + param_3) % 3 == 1) {
      pcVar5 = s_pacey_hitman_009b7f10;
    }
    else {
      pcVar5 = s_pacey_goalscorer_009b7efc;
    }
  }
LAB_00546972:
  cm_fn_0066f4e0(param_4,pcVar5);
  if (iVar2 != 4) {
    return param_6;
  }
  return 1;
}
