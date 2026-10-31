#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0

void CM_CDECL cm_fn_0053a610(const volatile s8 *param_1,const volatile s8 *param_2,volatile s16 *param_3,volatile u8 *param_4,volatile u8 *param_5,volatile u8 *param_6)

{
  volatile u8 *psVar1;
  int iVar2;
  
  iVar2 = 0x3f;
  psVar1 = (volatile u8 *)param_3 + 2;
  do {
    *psVar1 = 0;
    psVar1 += 3;
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  *param_3 = (short)*((const volatile s8 *)param_1 + 0xf);
  *(volatile s16 *)((volatile u8 *)param_3 + 3) = (short)*((const volatile s8 *)param_1 + 0x10);
  param_3[3] = (short)*((const volatile s8 *)param_1 + 0x11);
  *(volatile s16 *)((volatile u8 *)param_3 + 9) = (short)*((const volatile s8 *)param_1 + 0x12);
  param_3[6] = (short)*((const volatile s8 *)param_1 + 0x13);
  *(volatile s16 *)((volatile u8 *)param_3 + 0xf) = (short)*((const volatile s8 *)param_1 + 0x14);
  param_3[9] = (short)*((const volatile s8 *)param_1 + 0x15);
  *(volatile s16 *)((volatile u8 *)param_3 + 0x15) = (short)*((const volatile s8 *)param_1 + 0x16);
  param_3[0xc] = (short)*((const volatile s8 *)param_1 + 0x1a);
  if (param_5 != (u8 *)0x0) {
    *param_5 = 9;
  }
  *(volatile s16 *)((volatile u8 *)param_3 + 0x1b) = (short)*((const volatile s8 *)param_1 + 0x17);
  param_3[0xf] = (short)*((const volatile s8 *)param_1 + 0x18);
  *(volatile s16 *)((volatile u8 *)param_3 + 0x21) = (short)*((const volatile s8 *)param_1 + 0x19);
  if (param_4 != (u8 *)0x0) {
    *param_4 = 0xc;
  }
  param_3[0x12] = (short)*((const volatile s8 *)param_1 + 0x30);
  *(volatile s16 *)((volatile u8 *)param_3 + 0x27) = (short)*((const volatile s8 *)param_1 + 0x3b);
  param_3[0x15] = (short)*((const volatile s8 *)param_1 + 0x1e);
  *(volatile u8 *)((volatile u8 *)param_3 + 0x2f) = 2;
  *(volatile s16 *)((volatile u8 *)param_3 + 0x2d) = (short)*((const volatile s8 *)param_1 + 0x23);
  param_3[0x18] = (short)*((const volatile s8 *)param_1 + 0x24);
  *(volatile u8 *)((volatile u8 *)param_3 + 0x35) = 2;
  *(volatile s16 *)((volatile u8 *)param_3 + 0x33) = (short)*((const volatile s8 *)param_1 + 0x26);
  *(volatile u8 *)((volatile u8 *)param_3 + (0x1c * 2)) = 2;
  param_3[0x1b] = (short)*((const volatile s8 *)param_1 + 0x27);
  *(volatile u8 *)((volatile u8 *)param_3 + 0x3b) = 1;
  *(volatile s16 *)((volatile u8 *)param_3 + 0x39) = (short)*((const volatile s8 *)param_1 + 0x2a);
  *(volatile u8 *)((volatile u8 *)param_3 + (0x1f * 2)) = 2;
  param_3[0x1e] = (short)*((const volatile s8 *)param_1 + 0x2b);
  *(volatile u8 *)((volatile u8 *)param_3 + 0x41) = 2;
  *(volatile s16 *)((volatile u8 *)param_3 + 0x3f) = (short)*((const volatile s8 *)param_1 + 0x31);
  *(volatile u8 *)((volatile u8 *)param_3 + (0x22 * 2)) = 2;
  param_3[0x21] = (short)*((const volatile s8 *)param_1 + 0x32);
  *(volatile u8 *)((volatile u8 *)param_3 + 0x47) = 2;
  *(volatile s16 *)((volatile u8 *)param_3 + 0x45) = (short)*((const volatile s8 *)param_1 + 0x33);
  param_3[0x24] = (short)*((const volatile s8 *)param_1 + 0x35);
  *(volatile u8 *)((volatile u8 *)param_3 + 0x4d) = 2;
  *(volatile s16 *)((volatile u8 *)param_3 + 0x4b) = (short)*((const volatile s8 *)param_1 + 0x37);
  *(volatile u8 *)((volatile u8 *)param_3 + (0x28 * 2)) = 2;
  param_3[0x27] = (short)*((const volatile s8 *)param_1 + 0x38);
  *(volatile s16 *)((volatile u8 *)param_3 + 0x51) = (short)*((const volatile s8 *)param_1 + 0x39);
  param_3[0x2a] = (short)*((const volatile s8 *)param_1 + 0x3a);
  *(volatile u8 *)((volatile u8 *)param_3 + 0x59) = 2;
  *(volatile s16 *)((volatile u8 *)param_3 + 0x57) = (short)*((const volatile s8 *)param_1 + 0x3e);
  param_3[0x2d] = (short)*((const volatile s8 *)param_1 + 0x41);
  *(volatile u8 *)((volatile u8 *)param_3 + 0x5f) = 2;
  *(volatile s16 *)((volatile u8 *)param_3 + 0x5d) = (short)*((const volatile s8 *)param_1 + 0x43);
  if (param_6 != (u8 *)0x0) {
    *param_6 = 0x20;
  }
  *(volatile u8 *)((volatile u8 *)param_3 + (0x31 * 2)) = 2;
  param_3[0x30] = (short)*((const volatile s8 *)param_1 + 0x28);
  *(volatile u8 *)((volatile u8 *)param_3 + 0x65) = 2;
  *(volatile s16 *)((volatile u8 *)param_3 + 99) = (short)*((const volatile s8 *)param_1 + 0x22);
  param_3[0x33] = (short)*((const volatile s8 *)param_1 + 0x29);
  *(volatile u8 *)((volatile u8 *)param_3 + 0x6b) = 2;
  *(volatile s16 *)((volatile u8 *)param_3 + 0x69) = (short)*((const volatile s8 *)param_1 + 0x40);
  param_3[0x36] = (short)*((const volatile s8 *)param_1 + 0x3d);
  *(volatile s16 *)((volatile u8 *)param_3 + 0x6f) = (short)*((const volatile s8 *)param_1 + 0x3c);
  param_3[0x39] = (short)*((const volatile s8 *)param_1 + 0x36);
  *(volatile s16 *)((volatile u8 *)param_3 + 0x75) = (short)*((const volatile s8 *)param_1 + 0x2e);
  param_3[0x3c] = (short)*((const volatile s8 *)param_1 + 0x2f);
  *(volatile s16 *)((volatile u8 *)param_3 + 0x7b) = (short)*((const volatile s8 *)param_1 + 0x25);
  param_3[0x3f] = (short)*((const volatile s8 *)param_1 + 0x1f);
  *(volatile s16 *)((volatile u8 *)param_3 + 0x81) = (short)*((const volatile s8 *)param_1 + 0x20);
  param_3[0x42] = (short)*((const volatile s8 *)param_1 + 0x21);
  *(volatile u8 *)((volatile u8 *)param_3 + 0x89) = 2;
  *(volatile s16 *)((volatile u8 *)param_3 + 0x87) = (short)*((const volatile s8 *)param_1 + 0x1b);
  param_3[0x45] = (short)*((const volatile s8 *)param_1 + 0x1c);
  *(volatile s16 *)((volatile u8 *)param_3 + 0x8d) = (short)*((const volatile s8 *)param_1 + 0x1d);
  param_3[0x48] = (short)*((const volatile s8 *)param_1 + 0x2c);
  *(volatile s16 *)((volatile u8 *)param_3 + 0x93) = (short)*((const volatile s8 *)param_1 + 0x2d);
  param_3[0x4b] = (short)*((const volatile s8 *)param_1 + 0x42);
  *(volatile u8 *)((volatile u8 *)param_3 + 0x9b) = 2;
  *(volatile s16 *)((volatile u8 *)param_3 + 0x99) = (short)*((const volatile s8 *)param_1 + 0x3f);
  *(volatile u8 *)((volatile u8 *)param_3 + (0x4f * 2)) = 2;
  param_3[0x4e] = (short)*((const volatile s8 *)param_1 + 0x44);
  *(volatile s16 *)((volatile u8 *)param_3 + 0x9f) = (short)*((const volatile s8 *)param_1 + 0x34);
  param_3[0x51] = (short)*(const volatile s8 *)(param_2 + 0x58);
  return;
}
