#include "cmre.h"
extern u32 CM_CDECL cm_fn_0066f4d0(void);
extern char *CM_CDECL cm_fn_0066f4e0(char *output, char *format, ...);

char *CM_CDECL cm_fn_0053ad00(u8 param_1, s32 param_2, s32 param_3, u8 *param_4)

{
  int iVar1;
  char *pcVar2;
  s32 selector = (s8)param_1;
  if ((u32)selector <= 0x11u) {
    volatile u32 jump_target = *(volatile u32 *)(0x0053b384u + (u32)selector * 4u);
    (void)jump_target;
  }
  
  switch(param_1) {
  case 0:
    if (param_2 != 0) {
      if (param_3 == 0) {
        pcVar2 = (char *)0x009b6b10;
      }
      else {
        pcVar2 = (char *)0x009b6b38;
      }
      break;
    }
    if (param_3 == 0) {
      cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b6af8);
    }
    else {
      cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b6b04);
    }
    goto LAB_0053b332;
  case 1:
    if (param_2 == 0) {
      if (param_3 == 0) {
        cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b6aa4);
      }
      else {
        cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b1234);
      }
      goto LAB_0053b332;
    }
    if (param_3 == 0) {
      pcVar2 = (char *)0x009b6ab0;
    }
    else {
      pcVar2 = (char *)0x009b6ad4;
    }
    break;
  case 2:
    if (param_2 == 0) {
      if (param_3 == 0) {
        cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b6a2c);
      }
      else {
        cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b6a40);
      }
    }
    else {
      if (param_3 == 0) {
        pcVar2 = (char *)0x009b6a54;
      }
      else {
        pcVar2 = (char *)0x009b6a7c;
      }
LAB_0053b00f:
      cm_fn_0066f4e0((char *)0x00ae19b4,pcVar2,(u8 *)0x00ad9c64);
    }
    goto LAB_0053b01c;
  case 3:
    if (param_2 == 0) {
      if (param_3 == 0) {
        cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b6760);
      }
      else {
        cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b6770);
      }
      goto LAB_0053b332;
    }
    if (param_3 == 0) {
      pcVar2 = (char *)0x009b6780;
    }
    else {
      pcVar2 = (char *)0x009b67a8;
    }
    break;
  case 4:
    if (param_2 == 0) {
      if (param_3 == 0) {
        cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b69ac);
      }
      else {
        cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b69c4);
      }
      goto LAB_0053b332;
    }
    if (param_3 == 0) {
      pcVar2 = (char *)0x009b69dc;
    }
    else {
      pcVar2 = (char *)0x009b6a04;
    }
    break;
  case 5:
    if (param_2 != 0) {
      if (param_3 == 0) {
        pcVar2 = (char *)0x009b6964;
      }
      else {
        pcVar2 = (char *)0x009b6988;
      }
      goto LAB_0053b00f;
    }
    if (param_3 == 0) {
      cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b695c);
    }
    else {
      cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b6440);
    }
    goto LAB_0053b01c;
  case 6:
    if (param_2 != 0) {
      if (param_3 == 0) {
        pcVar2 = (char *)0x009b690c;
      }
      else {
        pcVar2 = (char *)0x009b6934;
      }
      goto LAB_0053b00f;
    }
    if (param_3 == 0) {
      cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b68f8);
    }
    else {
      cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b642c);
    }
    goto LAB_0053b01c;
  case 7:
    if (param_2 != 0) {
      if (param_3 == 0) {
        pcVar2 = (char *)0x009b68a8;
      }
      else {
        pcVar2 = (char *)0x009b68d0;
      }
      goto LAB_0053b00f;
    }
    if (param_3 == 0) {
      cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b6878);
    }
    else {
      cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b6890);
    }
    goto LAB_0053b01c;
  case 8:
    if (param_2 == 0) {
      if (param_3 == 0) {
        cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b6828);
      }
      else {
        cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b6448);
      }
      goto LAB_0053b332;
    }
    if (param_3 == 0) {
      pcVar2 = (char *)0x009b6830;
    }
    else {
      pcVar2 = (char *)0x009b6854;
    }
    break;
  case 9:
    if (param_2 != 0) {
      if (param_3 == 0) {
        pcVar2 = (char *)0x009b67e0;
      }
      else {
        pcVar2 = (char *)0x009b6804;
      }
      goto LAB_0053b00f;
    }
    if (param_3 == 0) {
      cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b67d0);
    }
    else {
      cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b67d8);
    }
LAB_0053b01c:
    if ((param_4 != (u8 *)0x0) && (iVar1 = cm_fn_0066f4d0(), iVar1 == 4)) {
      *param_4 = 4;
      return (char *)0x00ae19b4;
    }
    goto LAB_0053b377;
  case 10:
    if (param_2 == 0) {
      if (param_3 == 0) {
        cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b6708);
      }
      else {
        cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b6710);
      }
      goto LAB_0053b332;
    }
    if (param_3 == 0) {
      pcVar2 = (char *)0x009b6718;
    }
    else {
      pcVar2 = (char *)0x009b673c;
    }
    break;
  case 0xb:
    if (param_2 == 0) {
      if (param_3 == 0) {
        pcVar2 = (char *)0x009b6678;
      }
      else {
        pcVar2 = (char *)0x009b6698;
      }
    }
    else if (param_3 == 0) {
      pcVar2 = (char *)0x009b66b8;
    }
    else {
      pcVar2 = (char *)0x009b66e0;
    }
    break;
  case 0xc:
    if (param_2 == 0) {
      if (param_3 == 0) {
        cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b6608);
      }
      else {
        cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b6618);
      }
      goto LAB_0053b332;
    }
    if (param_3 == 0) {
      pcVar2 = (char *)0x009b6628;
    }
    else {
      pcVar2 = (char *)0x009b6650;
    }
    break;
  case 0xd:
    if (param_2 == 0) {
      if (param_3 == 0) {
        cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b6578);
      }
      else {
        cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b6594);
      }
      goto LAB_0053b332;
    }
    if (param_3 == 0) {
      pcVar2 = (char *)0x009b65b0;
    }
    else {
      pcVar2 = (char *)0x009b65dc;
    }
    break;
  default:
    return (char *)0x0;
  case 0xf:
    if (param_2 == 0) {
      if (param_3 == 0) {
        pcVar2 = (char *)0x009b6510;
      }
      else {
        pcVar2 = (char *)0x009b6520;
      }
      cm_fn_0066f4e0((char *)0x00ae19b4,pcVar2);
      if ((param_4 != (u8 *)0x0) && (iVar1 = cm_fn_0066f4d0(), iVar1 == 4)) {
        *param_4 = 1;
        return (char *)0x00ae19b4;
      }
      goto LAB_0053b377;
    }
    if (param_3 == 0) {
      pcVar2 = (char *)0x009b6530;
    }
    else {
      pcVar2 = (char *)0x009b6554;
    }
    break;
  case 0x10:
    if (param_2 == 0) {
      if (param_3 != 0) {
        cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b64b8);
        return (char *)0x00ae19b4;
      }
      cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b64a8);
      return (char *)0x00ae19b4;
    }
    if (param_3 != 0) {
      cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b64ec,(u8 *)0x00ad9c64);
      return (char *)0x00ae19b4;
    }
    cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b64c8,(u8 *)0x00ad9c64);
    return (char *)0x00ae19b4;
  case 0x11:
    if (param_2 == 0) {
      cm_fn_0066f4e0((char *)0x00ae19b4,(char *)0x009b6450);
      if ((param_4 != (u8 *)0x0) && (iVar1 = cm_fn_0066f4d0(), iVar1 == 4)) {
        *param_4 = 1;
      }
      goto LAB_0053b377;
    }
    if (param_3 == 0) {
      pcVar2 = (char *)0x009b6460;
    }
    else {
      pcVar2 = (char *)0x009b6484;
    }
  }
  cm_fn_0066f4e0((char *)0x00ae19b4,pcVar2,(u8 *)0x00ad9c64);
LAB_0053b332:
  if ((param_4 != (u8 *)0x0) && (iVar1 = cm_fn_0066f4d0(), iVar1 == 4)) {
    *param_4 = 1;
    return (char *)0x00ae19b4;
  }
LAB_0053b377:
  return (char *)0x00ae19b4;
}
