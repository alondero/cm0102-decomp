#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define builtin_strncpy(destination, source, count) __builtin_memcpy((destination),(source),(count))
#define DAT_00ae24d0 CM_GLOBAL(volatile u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(volatile u32, 0x00b67a34)
#define s_E__dev_CM3_cm3_00_01_cm3_code_fo_009cc3dc ((char *)0x009cc3dc)
#define s_Error_009870e0 ((char *)0x009870e0)
#define s_v_s__s__s__d_009870e8 ((char *)0x009870e8)
extern u32 CM_CDECL cm_fn_005b73a0(u32 param_1,volatile u16 *param_2,volatile u16 *param_3);
extern void cm_fn_005e8290(const char * param_1,const char * param_2,int param_3);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
u32 CM_CDECL cm_fn_005b7e60(u32 param_1,s8 param_2,s32 param_3,volatile s16 *param_4)

{
  u32 uVar1;
  char cVar2;
  short sVar3;
  int iVar4;
  short sVar5;
  u8 scratch[0x210];
  char *local_210 = (char *)scratch;
  volatile u16 *local_208 = (volatile u16 *)(scratch + 8);
  volatile u16 *local_204 = (volatile u16 *)(scratch + 0xc);
  u8 *local_200 = scratch + 0x10;
  u8 *local_100 = scratch + 0x110;
  s16 low;
  s16 high;
  
  if ((param_2 < 0) || (param_2 > 1)) {
    goto invalid_input;
  }
  low = (s16)param_3;
  if ((low < 0) || (low > 2)) {
    goto invalid_input;
  }
  sVar5 = low;
  high = (s16)((u32)param_3 >> 16);
  if ((high < 0) || (high > 3)) {
    goto invalid_input;
  }
  uVar1 = cm_fn_005b73a0(param_1,local_204,local_208);
  if ((*local_204 & 1) == 0) {
    if ((*local_204 & 2) == 0) {
      if ((*local_204 & 4) == 0) {
        if ((*local_204 & 8) == 0) {
          if ((*local_204 & 0x10) == 0) {
            if ((*local_204 & 0x20) == 0) {
              if (param_2 != '\0') {
                builtin_strncpy(local_210,"\x0f\x18\x1f\"",4);
                sVar3 = (short)local_210[high];
                param_4[1] = sVar3;
                if (((*local_208 & 0x80) != 0) || ((*local_208 & 0x800) != 0)) goto LAB_005b81cb;
                goto LAB_005b81d0;
              }
              builtin_strncpy(local_210,"\r\x0f\x17\x1f",4);
              sVar3 = (short)local_210[high];
            }
            else {
              if (param_2 != '\0') {
                builtin_strncpy(local_210,"\f\x13\x1b\x1e",4);
                sVar3 = (short)local_210[high];
                goto LAB_005b8080;
              }
              builtin_strncpy(local_210,"\t\r\x13\x1a",4);
              sVar3 = (short)local_210[high];
            }
          }
          else {
            if (param_2 != '\0') {
              builtin_strncpy(local_210,"\t\x0e\x17\x1a",4);
              sVar3 = (short)local_210[high];
              goto LAB_005b8080;
            }
            builtin_strncpy(local_210,"\x06\v\x10\x16",4);
            sVar3 = (short)local_210[high];
          }
        }
        else if (param_2 == '\0') {
          builtin_strncpy(local_210,"\x04\t\r\x12",4);
          sVar3 = (short)local_210[high];
        }
        else {
          builtin_strncpy(local_210,"\a\v\x14\x16",4);
          sVar3 = (short)local_210[high];
LAB_005b8080:
          param_4[1] = sVar3;
          if (((*local_208 & 0x80) == 0) && ((*local_208 & 0x800) == 0)) goto LAB_005b81d0;
LAB_005b81cb:
          sVar3 = sVar3 + 1;
        }
      }
      else {
        if (param_2 != '\0') {
          builtin_strncpy(local_210,"\x04\t\x11\x11",4);
          cVar2 = local_210[high];
          goto LAB_005b7f96;
        }
        builtin_strncpy(local_210,"\x02\a\t\r",4);
        sVar3 = (short)local_210[high];
      }
      goto LAB_005b81cc;
    }
    if (param_2 == '\0') {
      builtin_strncpy(local_210,"\x01\x03\x05\t",4);
      sVar3 = (short)local_210[high];
      goto LAB_005b81cc;
    }
    builtin_strncpy(local_210,"\x02\x04\r\x0f",4);
    cVar2 = local_210[high];
LAB_005b7f96:
    param_4[1] = (short)cVar2;
    if (((*local_208 & 0x80) != 0) || ((*local_208 & 0x800) != 0)) {
      param_4[1] = (-(u16)(high != 3) & 0xfffd) + 4 + (short)cVar2;
    }
  }
  else {
    if (param_2 == '\0') {
      local_210[0] = '\0';
      local_210[1] = 1;
      local_210[2] = 2;
      local_210[3] = 3;
      sVar3 = (short)local_210[high];
    }
    else {
      local_210[0] = '\0';
      local_210[1] = 2;
      local_210[2] = 4;
      local_210[3] = 6;
      sVar3 = (short)local_210[high];
    }
LAB_005b81cc:
    param_4[1] = sVar3;
  }
LAB_005b81d0:
  if ((*local_208 & 0x80) == 0) {
    if ((*local_208 & 0x100) == 0) {
      if ((*local_208 & 0x200) == 0) {
        cVar2 = '\x04' - ((*local_208 & 0x400) != 0);
      }
      else {
        cVar2 = '\x02';
      }
    }
    else {
      cVar2 = '\x01';
    }
  }
  else {
    cVar2 = '\0';
  }
  if ((*local_204 & 1) == 0) {
    if ((*local_204 & 6) == 0) {
      if ((*local_204 & 0x38) == 0) {
        if (param_2 == '\0') {
          builtin_strncpy(local_210,"\a\v\r\x0f\x13",5);
          sVar3 = (short)local_210[(s32)cVar2];
        }
        else if ((high == 0) || (high == 1)) {
          builtin_strncpy(local_210,"\x02\b\r\x12\x18",5);
          sVar3 = (short)local_210[(s32)cVar2];
        }
        else if (high == 2) {
          builtin_strncpy(local_210,"\x03\n\r\x10\x17",5);
          sVar3 = (short)local_210[(s32)cVar2];
        }
        else {
          builtin_strncpy(local_210,"\x06\v\r\x0f\x14",5);
          sVar3 = (short)local_210[(s32)cVar2];
        }
      }
      else if (param_2 == '\0') {
        if (high == 0) {
          builtin_strncpy(local_210,"\t\v\r\x0f\x11",5);
          sVar3 = (short)local_210[(s32)cVar2];
        }
        else {
          builtin_strncpy(local_210,"\a\v\r\x0f\x13",5);
          sVar3 = (short)local_210[(s32)cVar2];
        }
      }
      else {
        builtin_strncpy(local_210,"\x03\n\r\x10\x17",5);
        sVar3 = (short)local_210[(s32)cVar2];
      }
    }
    else if (param_2 == '\0') {
      if (high == 0) {
        builtin_strncpy(local_210,"\t\v\r\x0f\x11",5);
        sVar3 = (short)local_210[(s32)cVar2];
      }
      else {
        builtin_strncpy(local_210,"\a\v\r\x0f\x13",5);
        sVar3 = (short)local_210[(s32)cVar2];
      }
    }
    else if (high == 0) {
      builtin_strncpy(local_210,"\x06\v\r\x0f\x14",5);
      sVar3 = (short)local_210[(s32)cVar2];
    }
    else {
      builtin_strncpy(local_210,"\x03\n\r\x10\x17",5);
      sVar3 = (short)local_210[(s32)cVar2];
    }
  }
  else if (param_2 == '\0') {
    builtin_strncpy(local_210,"\r\r\r\r\r",5);
    sVar3 = (short)local_210[(s32)cVar2];
  }
  else {
    builtin_strncpy(local_210,"\r\r\r\r\r",5);
    sVar3 = (short)local_210[(s32)cVar2];
  }
  *param_4 = sVar3;
  if (((((*local_204 & 1) == 0) || (high < 1)) &&
      (((*local_204 & 6) == 0 || (high != 0)))) &&
     (((*local_204 & 0x40) == 0 ||
      (((((*local_208 & 0x100) == 0 && ((*local_208 & 0x200) == 0)) && ((*local_208 & 0x400) == 0)) ||
       ((high != 3 || (param_2 != '\x01')))))))) {
    if ((*local_204 & 1) == 0) {
      if (sVar5 == 0) {
        iVar4 = *param_4 + -5;
        goto LAB_005b84ac;
      }
      if (sVar5 == 2) {
        iVar4 = *param_4 + 5;
        goto LAB_005b84ec;
      }
    }
  }
  else if (sVar5 == 0) {
    iVar4 = *param_4 + -1;
LAB_005b84ac:
    if (iVar4 < 3) {
LAB_005b84bb:
      iVar4 = 2;
    }
    else if (iVar4 < 0x11) {
      if (iVar4 < 3) goto LAB_005b84bb;
    }
    else {
      iVar4 = 0x11;
    }
LAB_005b8507:
    *param_4 = (short)iVar4;
  }
  else if (sVar5 == 2) {
    iVar4 = *param_4 + 1;
LAB_005b84ec:
    if (iVar4 < 10) {
LAB_005b84fb:
      iVar4 = 9;
    }
    else if (iVar4 < 0x18) {
      if (iVar4 < 10) goto LAB_005b84fb;
    }
    else {
      iVar4 = 0x18;
    }
    goto LAB_005b8507;
  }
  sVar5 = *param_4;
  if (0 < sVar5) {
    if (0x19 < sVar5) {
      sVar5 = 0x1a;
      goto LAB_005b852f;
    }
    if (0 < sVar5) goto LAB_005b852f;
  }
  sVar5 = 0;
LAB_005b852f:
  *param_4 = sVar5;
  sVar5 = param_4[1];
  if (0 < sVar5) {
    if (0x22 < sVar5) {
      param_4[1] = 0x23;
      return 1;
    }
    if (0 < sVar5) {
      param_4[1] = sVar5;
      return 1;
    }
  }
  param_4[1] = 0;
  return 1;

invalid_input:
  cm_fn_00944cff((u32)s_E__dev_CM3_cm3_00_01_cm3_code_fo_009cc3dc,0,0,
                 local_100,local_200);
  uVar1 = (u32)cm_fn_0090d130();
  cm_fn_00944c9f((u32)&DAT_00ae24d0,(u32)s_v_s__s__s__d_009870e8,uVar1,
                 (u32)local_100,(u32)local_200,0xf19);
  cm_fn_005e8290(s_Error_009870e0,(const char *)&DAT_00ae24d0,0);
  DAT_00b67a34 = 0;
  return 0;
}
