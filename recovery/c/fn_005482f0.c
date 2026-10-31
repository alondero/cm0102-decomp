/* Draft for preferred-image VA 0x005482f0, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_00ad9c64 CM_GLOBAL(u8, 0x00ad9c64)
#define Pla_009b7f30 ((char *)0x009b7f30)
#define Pla_009b7f60 ((char *)0x009b7f60)
#define Playi_009b8048 ((char *)0x009b8048)
#define Playi_009b8078 ((char *)0x009b8078)
#define Playin_009b7fe8 ((char *)0x009b7fe8)
#define Playin_009b8018 ((char *)0x009b8018)
#define Playin_009b8110 ((char *)0x009b8110)
#define Playin_009b8140 ((char *)0x009b8140)
#define Playing_009b81c8 ((char *)0x009b81c8)
#define Playing_009b81f4 ((char *)0x009b81f4)
#define Playing_009b8278 ((char *)0x009b8278)
#define Playing_009b82a4 ((char *)0x009b82a4)
#define Playing_009b83f0 ((char *)0x009b83f0)
#define Playing_009b841c ((char *)0x009b841c)
#define Playing_P_009b7f90 ((char *)0x009b7f90)
#define Playing_P_009b7fbc ((char *)0x009b7fbc)
#define Playing_P_009b8170 ((char *)0x009b8170)
#define Playing_P_009b819c ((char *)0x009b819c)
#define Playing_P_009b82d0 ((char *)0x009b82d0)
#define Playing_P_009b82fc ((char *)0x009b82fc)
#define Playing_Po_009b8220 ((char *)0x009b8220)
#define Playing_Po_009b824c ((char *)0x009b824c)
#define Playing_Po_009b8398 ((char *)0x009b8398)
#define Playing_Po_009b83c4 ((char *)0x009b83c4)
#define _009b80a8 ((char *)0x009b80a8)
#define _009b80dc ((char *)0x009b80dc)
#define _s___COMMEN_009b8328 ((char *)0x009b8328)
#define _s___COMMEN_009b8360 ((char *)0x009b8360)
extern u32 cm_fn_0066f4d0(void);
extern s32 CM_CDECL cm_fn_0066f4e0(u8 *destination, const void *format, ...);

u8 CM_CDECL cm_fn_005482f0(u32 param_1,u8 *param_2,u8 param_3)

{
  u8 bVar1;
  int iVar2;
  char *pcVar3;
  
  if (param_2 == 0) {
    return 0;
  }
  iVar2 = cm_fn_0066f4d0();
  bVar1 = (u8)param_1;
  if ((bVar1 & 1) == 1) {
    if (param_3 == '\0') {
      pcVar3 = (char *)0x009b841c;
    }
    else {
      pcVar3 = (char *)0x009b83f0;
    }
    cm_fn_0066f4e0(param_2,pcVar3,&DAT_00ad9c64);
    if (iVar2 == 2) {
      return 2;
    }
    if (iVar2 == 5) {
      return 1;
    }
    goto LAB_00548353;
  }
  if ((((param_1 & 0x260) == 0x260) || ((param_1 & 0x160) == 0x160)) || ((param_1 & 0x460) == 0x460)
     ) {
    if (param_3 == '\0') {
      pcVar3 = (char *)0x009b7f60;
    }
    else {
      pcVar3 = (char *)0x009b7f30;
    }
    cm_fn_0066f4e0(param_2,pcVar3,&DAT_00ad9c64);
    if (iVar2 == 2) {
      return 5;
    }
    if (iVar2 == 5) {
      return 1;
    }
    if (iVar2 != 4) {
      return 0;
    }
    return 4;
  }
  if (((bVar1 & 0x40) == 0x40) && ((param_1 & 0x700) == 0x700)) {
    if (param_3 == '\0') {
      pcVar3 = (char *)0x009b83c4;
    }
    else {
      pcVar3 = (char *)0x009b8398;
    }
    cm_fn_0066f4e0(param_2,pcVar3,&DAT_00ad9c64);
    if (iVar2 == 2) {
      return 5;
    }
LAB_005483e9:
    if (iVar2 == 5) {
      return 1;
    }
LAB_0054871f:
    if (iVar2 != 4) {
      return 0;
    }
    return 4;
  }
  if (((bVar1 & 0x88) == 0x88) || ((param_1 & 0x808) == 0x808)) {
    if (param_3 == '\0') {
      pcVar3 = (char *)0x009b7fbc;
    }
    else {
      pcVar3 = (char *)0x009b7f90;
    }
    cm_fn_0066f4e0(param_2,pcVar3,&DAT_00ad9c64);
    if (iVar2 == 2) {
      return 5;
    }
  }
  else {
    if (((bVar1 & 0x90) == 0x90) || ((bVar1 & 0xa0) == 0xa0)) {
      if (param_3 == '\0') {
        pcVar3 = (char *)0x009b8018;
      }
      else {
        pcVar3 = (char *)0x009b7fe8;
      }
      cm_fn_0066f4e0(param_2,pcVar3,&DAT_00ad9c64);
      if (iVar2 == 2) {
        return 5;
      }
      if (iVar2 == 5) {
        return 1;
      }
      goto LAB_0054871f;
    }
    if (((param_1 & 0x810) == 0x810) || ((param_1 & 0x820) == 0x820)) {
      if (param_3 == '\0') {
        pcVar3 = (char *)0x009b8078;
      }
      else {
        pcVar3 = (char *)0x009b8048;
      }
      cm_fn_0066f4e0(param_2,pcVar3,&DAT_00ad9c64);
      if (iVar2 == 2) {
        return 5;
      }
      goto LAB_005483e9;
    }
    if (((((param_1 & 0x200) == 0x200) || ((param_1 & 0x100) == 0x100)) ||
        ((param_1 & 0x400) == 0x400)) && (((bVar1 & 0x10) == 0x10 || ((bVar1 & 8) == 8)))) {
      if (param_3 == '\0') {
        pcVar3 = (char *)0x009b80dc;
      }
      else {
        pcVar3 = (char *)0x009b80a8;
      }
    }
    else if ((((param_1 & 0x200) == 0x200) ||
             (((param_1 & 0x100) == 0x100 || ((param_1 & 0x400) == 0x400)))) &&
            ((bVar1 & 0x20) == 0x20)) {
      if (param_3 == '\0') {
        pcVar3 = (char *)0x009b8360;
      }
      else {
        pcVar3 = (char *)0x009b8328;
      }
    }
    else if ((((param_1 & 0x204) == 0x204) || ((param_1 & 0x104) == 0x104)) ||
            ((param_1 & 0x404) == 0x404)) {
      if (param_3 == '\0') {
        pcVar3 = (char *)0x009b8140;
      }
      else {
        pcVar3 = (char *)0x009b8110;
      }
    }
    else if ((bVar1 & 0x84) == 0x84) {
      if (param_3 == '\0') {
        pcVar3 = (char *)0x009b82fc;
      }
      else {
        pcVar3 = (char *)0x009b82d0;
      }
    }
    else {
      if ((param_1 & 0x804) != 0x804) {
        if ((bVar1 & 2) != 2) {
          if (param_1 == 0) {
            if (param_3 != '\0') {
              cm_fn_0066f4e0(param_2,(char *)0x009b81c8,&DAT_00ad9c64);
              return 0;
            }
            cm_fn_0066f4e0(param_2,(char *)0x009b81f4,&DAT_00ad9c64);
            return 0;
          }
          if (param_3 == '\0') {
            pcVar3 = (char *)0x009b819c;
          }
          else {
            pcVar3 = (char *)0x009b8170;
          }
          cm_fn_0066f4e0(param_2,pcVar3,&DAT_00ad9c64);
          if (iVar2 == 2) {
            return 2;
          }
          if (iVar2 == 5) {
            return 1;
          }
          goto LAB_00548353;
        }
        if (param_3 == '\0') {
          pcVar3 = (char *)0x009b824c;
        }
        else {
          pcVar3 = (char *)0x009b8220;
        }
        cm_fn_0066f4e0(param_2,pcVar3,&DAT_00ad9c64);
        if (iVar2 == 2) {
          return 2;
        }
        goto LAB_005485dc;
      }
      if (param_3 == '\0') {
        pcVar3 = (char *)0x009b82a4;
      }
      else {
        pcVar3 = (char *)0x009b8278;
      }
    }
    cm_fn_0066f4e0(param_2,pcVar3,&DAT_00ad9c64);
    if (iVar2 == 2) {
      return 2;
    }
  }
LAB_005485dc:
  if (iVar2 == 5) {
    return 1;
  }
LAB_00548353:
  if (iVar2 != 4) {
    return 0;
  }
  return 1;
}
