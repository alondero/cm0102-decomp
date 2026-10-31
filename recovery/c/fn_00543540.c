/* WIP: plain C cannot express the listing's x87 ST0 -> 00945640 (__ftol) call boundary.
   Original call sites 0054363b and 0054364f consume values held in ST0 after the
   selector-specific FILD/FMUL/FADD/FSTP/FST sequences. Do not substitute guessed
   casts or a fake stack argument. This unvalidated Ghidra draft is retained below.
   Draft for preferred-image VA 0x00543540, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define CONCAT31(a, b) ((u32)(((u32)(u32)(a) << 8) | (u32)(u8)(b)))
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define s_E__dev_CM3_cm3_00_01_cm3_code_Da_009b5ac8 ((char *)0x009b5ac8)
#define s_Error_009870e0 ((char *)0x009870e0)
#define s_v_s__s__s__d_009870e8 ((char *)0x009870e8)
extern void cm_fn_005442f0(...);  /* NEEDS-REVIEW: callee draft says (void); these are usually arguments MSVC pushed for the next call (right-to-left evaluation, deferred cleanup) */
extern void cm_fn_00544380(...);  /* NEEDS-REVIEW: callee draft says (void); these are usually arguments MSVC pushed for the next call (right-to-left evaluation, deferred cleanup) */
extern void cm_fn_005e8290(const char * param_1,const char * param_2,int param_3);
extern char * cm_fn_0090d130(...);  /* NEEDS-REVIEW: callee draft says (void); these are usually arguments MSVC pushed for the next call (right-to-left evaluation, deferred cleanup) */
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern /* Library Function - Single Match cm_fn_00945640 Library: Visual Studio */ long long cm_fn_00945640(void);

u8 cm_fn_00543540(int param_1,int param_2,char param_3)

{
  double dVar1;
  double dVar2;
  u8 uVar3;
  u32 uVar4;
  u8 *puVar5;
  u8 *puVar6;
  u32 uVar7;
  u8 local_200 [256];
  u8 local_100 [256];
  
  if (((param_1 == 0) || (param_3 < '\x01')) || ('\x1f' < param_3)) {
    cm_fn_00944cff(s_E__dev_CM3_cm3_00_01_cm3_code_Da_009b5ac8,0,0,local_200,local_100);
    puVar6 = local_100;
    puVar5 = local_200;
    uVar7 = 0x48fc;
LAB_00543a56:
    uVar7 = cm_fn_0090d130(puVar5,puVar6,uVar7);
    cm_fn_00944c9f(&DAT_00ae24d0,s_v_s__s__s__d_009870e8,uVar7);
    cm_fn_005e8290(s_Error_009870e0,&DAT_00ae24d0,0);
    DAT_00b67a34 = 0;
    return 0xff;
  }
  if (param_2 == 0) {
    cm_fn_00944cff(s_E__dev_CM3_cm3_00_01_cm3_code_Da_009b5ac8,0,0,local_100,local_200);
    puVar6 = local_200;
    puVar5 = local_100;
    uVar7 = 0x4903;
    goto LAB_00543a56;
  }
  uVar4 = (u32)((u32)(param_3 + -1) >> 8);
  switch(param_3 + -1) {
  case 0:
    return *(u8 *)(param_2 + 0x1b);
  case 1:
    return *(u8 *)(param_2 + 0x1c);
  case 2:
    return *(u8 *)(param_2 + 0x1d);
  case 3:
    dVar1 = (double)(int)*(char *)(param_2 + 0x1e) + (double)(int)*(char *)(param_2 + 0x1e);
    dVar2 = (double)(int)*(short *)(param_2 + 5);
    break;
  case 4:
    return *(u8 *)(param_2 + 0x1f);
  case 5:
    return *(u8 *)(param_2 + 0x20);
  case 6:
    if (*(char *)(param_2 + 0xf) == '\x14') {
      dVar1 = (double)(int)*(short *)(param_2 + 5) * 0.1;
      dVar2 = (double)(int)*(char *)(param_2 + 0x43) + (double)(int)*(char *)(param_2 + 0x43);
    }
    else {
      dVar1 = (double)(int)*(char *)(param_2 + 0x43) + (double)(int)*(char *)(param_2 + 0x43);
      dVar2 = (double)(int)*(short *)(param_2 + 5);
    }
    break;
  case 7:
    if (*(char *)(param_2 + 0xf) == '\x14') {
      dVar1 = (double)(int)*(short *)(param_2 + 5) * 0.1;
      dVar2 = (double)(int)*(char *)(param_2 + 0x23) + (double)(int)*(char *)(param_2 + 0x23);
    }
    else {
      dVar1 = (double)(int)*(char *)(param_2 + 0x23) + (double)(int)*(char *)(param_2 + 0x23);
      dVar2 = (double)(int)*(short *)(param_2 + 5);
    }
    break;
  case 8:
    dVar1 = ((double)(int)*(short *)(param_2 + 5) +
            (double)(int)*(char *)(param_2 + 0x24) + (double)(int)*(char *)(param_2 + 0x24)) * 0.05
            + 10.0;
    dVar1 = dVar1 + dVar1 * dVar1 * 0.1;
    goto joined_r0x00543615;
  case 9:
    return *(u8 *)(param_1 + 0x58);
  case 10:
    if (*(char *)(param_2 + 0xf) == '\x14') {
      dVar1 = (double)(int)*(short *)(param_2 + 5) * 0.1;
      dVar2 = (double)(int)*(char *)(param_2 + 0x26) + (double)(int)*(char *)(param_2 + 0x26);
    }
    else {
      dVar1 = (double)(int)*(char *)(param_2 + 0x26) + (double)(int)*(char *)(param_2 + 0x26);
      dVar2 = (double)(int)*(short *)(param_2 + 5);
    }
    break;
  case 0xb:
    if (*(char *)(param_2 + 0xf) == '\x14') {
      uVar3 = cm_fn_005442f0(param_2,*(u8 *)(param_2 + 0x27));
      return uVar3;
    }
    uVar3 = cm_fn_00544380(param_2,CONCAT31(uVar4,*(u8 *)(param_2 + 0x27)));
    return uVar3;
  case 0xc:
    return *(u8 *)(param_2 + 0x28);
  case 0xd:
    if (*(char *)(param_2 + 0xf) < '\x0f') {
      dVar1 = (double)(int)*(short *)(param_2 + 5) * 0.1;
      dVar2 = (double)(int)*(char *)(param_2 + 0x2a) + (double)(int)*(char *)(param_2 + 0x2a);
    }
    else {
      dVar1 = (double)(int)*(char *)(param_2 + 0x2a) + (double)(int)*(char *)(param_2 + 0x2a);
      dVar2 = (double)(int)*(short *)(param_2 + 5);
    }
    break;
  case 0xe:
    dVar1 = (double)(int)*(char *)(param_2 + 0x2b) + (double)(int)*(char *)(param_2 + 0x2b);
    dVar2 = (double)(int)*(short *)(param_2 + 5);
    break;
  case 0xf:
    return *(u8 *)(param_2 + 0x2f);
  case 0x10:
    return *(u8 *)(param_2 + 0x2e);
  case 0x11:
    uVar3 = cm_fn_00544380(param_2,CONCAT31(uVar4,*(u8 *)(param_2 + 0x31)));
    return uVar3;
  case 0x12:
    if (*(char *)(param_2 + 0xf) == '\x14') {
      dVar1 = (double)(int)*(short *)(param_2 + 5) * 0.1;
      dVar2 = (double)(int)*(char *)(param_2 + 0x32) + (double)(int)*(char *)(param_2 + 0x32);
    }
    else {
      dVar1 = (double)(int)*(char *)(param_2 + 0x32) + (double)(int)*(char *)(param_2 + 0x32);
      dVar2 = (double)(int)*(short *)(param_2 + 5);
    }
    break;
  case 0x13:
    if (*(char *)(param_2 + 0xf) == '\x14') {
      dVar1 = (double)(int)*(short *)(param_2 + 5) * 0.1;
      dVar2 = (double)(int)*(char *)(param_2 + 0x33) + (double)(int)*(char *)(param_2 + 0x33);
    }
    else {
      dVar1 = (double)(int)*(char *)(param_2 + 0x33) + (double)(int)*(char *)(param_2 + 0x33);
      dVar2 = (double)(int)*(short *)(param_2 + 5);
    }
    break;
  case 0x14:
    return *(u8 *)(param_2 + 0x36);
  case 0x15:
    dVar1 = (double)(int)*(char *)(param_2 + 0x37) + (double)(int)*(char *)(param_2 + 0x37);
    dVar2 = (double)(int)*(short *)(param_2 + 5);
    break;
  case 0x16:
    dVar1 = (double)(int)*(char *)(param_2 + 0x39) + (double)(int)*(char *)(param_2 + 0x39);
    dVar2 = (double)(int)*(short *)(param_2 + 5);
    break;
  case 0x17:
    if ('\x0e' < *(char *)(param_2 + 0xf)) {
      uVar3 = cm_fn_00544380(param_2,CONCAT31(uVar4,*(u8 *)(param_2 + 0x3a)));
      return uVar3;
    }
    dVar1 = (double)(int)*(short *)(param_2 + 5) * 0.1;
    dVar2 = (double)(int)*(char *)(param_2 + 0x3a) + (double)(int)*(char *)(param_2 + 0x3a);
    break;
  case 0x18:
    return *(u8 *)(param_2 + 0x29);
  case 0x19:
    return *(u8 *)(param_2 + 0x3c);
  case 0x1a:
    return *(u8 *)(param_2 + 0x3d);
  case 0x1b:
    uVar3 = cm_fn_00544380(param_2,*(u8 *)(param_2 + 0x3e));
    return uVar3;
  case 0x1c:
    return *(u8 *)(param_2 + 0x3f);
  case 0x1d:
    return *(u8 *)(param_2 + 0x40);
  case 0x1e:
    return *(u8 *)(param_2 + 0x44);
  default:
    cm_fn_00944cff(s_E__dev_CM3_cm3_00_01_cm3_code_Da_009b5ac8,0,0,local_200,local_100);
    puVar6 = local_100;
    puVar5 = local_200;
    uVar7 = 0x4981;
    goto LAB_00543a56;
  }
  dVar1 = (dVar2 + dVar1) * 0.05 + 10.0;
  dVar1 = dVar1 + dVar1 * dVar1 * 0.1;
joined_r0x00543615:
  dVar1 = dVar1 * 0.3333333333333333 + 0.5;
  if (((dVar1 <= 1.0) || (dVar1 < 20.0)) && (dVar1 <= 1.0)) {
    uVar3 = cm_fn_00945640();
    return uVar3;
  }
  uVar3 = cm_fn_00945640();
  return uVar3;
}
