/* WIP: plain C cannot express the listing's x87 ST0 -> 00945640 (__ftol) call boundary.
   Original call sites: 00597ea1, 00597f19, 00598002, 0059808d, 00598114,
   0059813c, 00598178, 005981c7. Their preceding FILD/FDIV/FMUL/FADD/FSTP
   instruction sequences are observable; do not replace them with guessed casts.
   The declaration below is an unvalidated Ghidra draft retained for reference.
   Draft for preferred-image VA 0x00597db0, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_009cf244 CM_GLOBAL(int, 0x009cf244)
#define DAT_009cf2c0 CM_GLOBAL(int, 0x009cf2c0)
#define DAT_009cf2e4 CM_GLOBAL(int, 0x009cf2e4)
#define DAT_009cf300 CM_GLOBAL(int, 0x009cf300)
#define DAT_009cf314 CM_GLOBAL(int, 0x009cf314)
#define DAT_009cf338 CM_GLOBAL(int, 0x009cf338)
#define DAT_009cf360 CM_GLOBAL(int, 0x009cf360)
#define DAT_009cf36c CM_GLOBAL(int, 0x009cf36c)
#define DAT_009cf434 CM_GLOBAL(int, 0x009cf434)
#define DAT_009cf45c CM_GLOBAL(int, 0x009cf45c)
#define DAT_009cf488 CM_GLOBAL(int, 0x009cf488)
#define DAT_009cf4a8 CM_GLOBAL(int, 0x009cf4a8)
#define DAT_009cf4f8 CM_GLOBAL(int, 0x009cf4f8)
#define DAT_00ae23b4 CM_GLOBAL(int, 0x00ae23b4)
extern u8 cm_fn_0053b900(int *param_1);
extern u32 CM_FASTCALL cm_fn_005986b0(u32 *param_1);
extern /* Library Function - Single Match cm_fn_00945640 Library: Visual Studio */ long long cm_fn_00945640(void);

int CM_THISCALL cm_fn_00597db0(int param_1,char param_2)

{
  int iVar1;
  int *piVar2;
  u8 uVar3;
  int iVar4;
  char cVar5;
  int iVar6;
  
  iVar6 = *(int *)(param_1 + 8);
  iVar1 = DAT_00ae23b4 + iVar6 + iVar6 * 0x244;
  if (*(char *)(DAT_00ae23b4 + iVar6 + 100 + iVar6 * 0x244) == '\x03') {
    return 0;
  }
  switch(param_2 + -5) {
  case 0:
  case 1:
  case 2:
    iVar6 = 35000;
    break;
  case 3:
    iVar6 = 20000;
    break;
  case 4:
    iVar6 = 0x5dc;
    break;
  case 5:
    iVar6 = 1000;
    break;
  default:
    iVar6 = 0x2ee;
  }
  iVar4 = cm_fn_0053b900(iVar1);
  if (iVar4 != 0) {
    return iVar6;
  }
  piVar2 = *(int **)(iVar1 + 0x53);
  if (piVar2 == (int *)0x0) {
LAB_00598178:
    iVar6 = cm_fn_00945640();
  }
  else {
    cVar5 = *(char *)((int)piVar2 + 0x7e);
    if (cVar5 < '\x01') {
      cVar5 = '\x01';
    }
    else if ('\x03' < cVar5) {
      cVar5 = '\x03';
    }
    iVar6 = cm_fn_00945640();
    if (cVar5 == '\x01') {
      iVar4 = *piVar2;
      if (((iVar4 == DAT_009cf2e4) || (iVar4 == DAT_009cf360)) || (iVar4 == DAT_009cf488)) {
        if (0x1c52 < *(short *)(iVar1 + 0x80)) {
          iVar6 = cm_fn_00945640();
        }
      }
      else if (iVar4 != DAT_009cf36c) {
        if (iVar4 == DAT_009cf4f8) {
          if (0x6d6 < iVar6) {
            iVar6 = cm_fn_00945640();
          }
        }
        else if (((iVar4 == DAT_009cf314) || (iVar4 == DAT_009cf300)) ||
                ((iVar4 == DAT_009cf45c || (iVar4 == DAT_009cf338)))) {
          if (0x9c4 < iVar6) {
            iVar6 = cm_fn_00945640();
          }
        }
        else if (((iVar4 == DAT_009cf4a8) || (iVar4 == DAT_009cf2c0)) ||
                ((iVar4 == DAT_009cf244 || (iVar4 == DAT_009cf434)))) {
          if (0x4e2 < iVar6) {
            iVar6 = cm_fn_00945640();
          }
        }
        else if (0x2ee < iVar6) {
          iVar6 = cm_fn_00945640();
        }
      }
    }
    if (*(int *)(iVar1 + 0x57) != 0) goto LAB_00598178;
  }
  uVar3 = cm_fn_005986b0();
  switch(uVar3) {
  default:
    goto switchD_0059819a_caseD_0;
  case 1:
    break;
  case 2:
    iVar6 = iVar6 * 2;
    goto switchD_0059819a_caseD_0;
  case 0xfe:
    break;
  case 0xff:
  }
  iVar6 = cm_fn_00945640();
switchD_0059819a_caseD_0:
  if (*(int *)(param_1 + 0x14) < iVar6) {
    iVar6 = *(int *)(param_1 + 0x14);
  }
  switch(param_2 + -5) {
  case 0:
    if (35000 < iVar6) {
      return 35000;
    }
    if (iVar6 < 0x6d6) {
      return 0x6d6;
    }
    goto LAB_005982d6;
  case 1:
  case 2:
    if (20000 < iVar6) {
      return 20000;
    }
    if (iVar6 < 500) {
      return 500;
    }
    goto LAB_005982d6;
  case 3:
    if (0x1d4c < iVar6) {
      return 0x1d4c;
    }
    if (iVar6 < 0xfa) {
      return 0xfa;
    }
    goto LAB_005982d6;
  case 4:
    if (2000 < iVar6) {
      return 2000;
    }
    break;
  case 5:
    if (0x5dc < iVar6) {
      return 0x5dc;
    }
    break;
  default:
    if (1000 < iVar6) {
      return 1000;
    }
  }
  if (iVar6 < 0x7d) {
    return 0x7d;
  }
LAB_005982d6:
  if (250000 < iVar6) {
    iVar6 = 250000;
  }
  return iVar6;
}
