/* Draft for preferred-image VA 0x005b5290, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(volatile u32, 0x00b67a34)
#define s_E__dev_CM3_cm3_00_01_cm3_code_fo_009cc3dc ((char *)0x009cc3dc)
#define s_Error_009870e0 ((char *)0x009870e0)
#define s_v_s__s__s__d_009870e8 ((char *)0x009870e8)
extern void cm_fn_005e8290(const char * self,const char * mode,int param_3);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

u32 CM_THISCALL cm_fn_005b5290(u32 self,s8 mode,u32 param_3,u32 param_4)

{
  volatile u32 *puVar1;
  u32 uVar3;
  u32 uVar5;
  u32 error_code;
  u32 format_arg1;
  u32 format_arg2;
  u8 scratch[0x200];
  u8 *local_200 = scratch;
  u8 *local_100 = scratch + 0x100;
  
  if ((mode < '\0') || ('\n' < mode)) {
    cm_fn_00944cff((u32)s_E__dev_CM3_cm3_00_01_cm3_code_fo_009cc3dc,0,0,local_200,local_100);
    format_arg1 = (u32)local_200;
    format_arg2 = (u32)local_100;
    error_code = 0x6a4;
    goto LAB_005b5780;
  }
  if (((int)param_4 < 1) && (((int)param_4 < 0 || (param_3 < 0x40001)))) {
    if ((param_3 == 0x40000) && (param_4 == 0)) {
LAB_005b56b1:
      puVar1 = (volatile u32 *)(self + 0x188b + mode * 8);
      uVar3 = *(volatile u32 *)(self + 0x188b + mode * 8) & 0x7ff3ffff;
    }
    else if ((int)param_4 < 1) {
      if ((-1 < (int)param_4) && (0x200 < param_3)) {
        if ((0 < (int)param_4) || ((-1 < (int)param_4 && (0x4000 < param_3)))) goto LAB_005b5493;
        if ((param_3 != 0x4000) || (param_4 != 0)) {
          if (((int)param_4 < 1) && (((int)param_4 < 0 || (param_3 < 0x1001)))) {
            if ((((param_3 == 0x1000) && (param_4 == 0)) || ((param_3 == 0x400 && (param_4 == 0))))
               || ((param_3 == 0x800 && (param_4 == 0)))) goto LAB_005b5462;
          }
          else if ((param_3 == 0x2000) && (param_4 == 0)) goto LAB_005b54c8;
          goto LAB_005b56e8;
        }
        goto LAB_005b54c8;
      }
      if ((param_3 == 0x200) && (param_4 == 0)) {
LAB_005b5462:
        puVar1 = (volatile u32 *)(self + 0x188b + mode * 8);
        uVar3 = *(volatile u32 *)(self + 0x188b + mode * 8) & 0xffffe1ff;
        goto LAB_005b5397;
      }
      if (((int)param_4 < 1) && (((int)param_4 < 0 || (param_3 < 0x21)))) {
        if ((param_3 != 0x20) || (param_4 != 0)) {
          if (((int)param_4 < 1) && (((int)param_4 < 0 || (param_3 < 9)))) {
            if ((param_3 == 8) && (param_4 == 0)) {
LAB_005b5383:
              puVar1 = (volatile u32 *)(self + 0x188b + mode * 8);
              uVar3 = *(volatile u32 *)(self + 0x188b + mode * 8) & 0xffffffe0;
              goto LAB_005b5397;
            }
            if ((-1 < (int)param_4) && ((0 < (int)param_4 || (param_3 != 0)))) {
              if (((int)param_4 < 0) || (((int)param_4 < 1 && (param_3 < 3)))) goto LAB_005b5383;
              if (param_3 == 4) goto joined_r0x005b537d;
            }
          }
          else if (param_3 == 0x10) {
joined_r0x005b537d:
            if (param_4 == 0) goto LAB_005b5383;
          }
          goto LAB_005b56e8;
        }
      }
      else if (((param_3 != 0x40) || (param_4 != 0)) &&
              (((param_3 != 0x80 || (param_4 != 0)) && ((param_3 != 0x100 || (param_4 != 0))))))
      goto LAB_005b56e8;
      puVar1 = (volatile u32 *)(self + 0x188b + mode * 8);
      uVar3 = *(volatile u32 *)(self + 0x188b + mode * 8) & 0xfffffe1f;
    }
    else {
LAB_005b5493:
      if ((param_3 == 0x8000) && (param_4 == 0)) {
LAB_005b54c8:
        puVar1 = (volatile u32 *)(self + 0x188b + mode * 8);
        uVar3 = *(volatile u32 *)(self + 0x188b + mode * 8) & 0xffff1fff;
      }
      else {
        if (((param_3 != 0x10000) || (param_4 != 0)) && ((param_3 != 0x20000 || (param_4 != 0))))
        goto LAB_005b56e8;
LAB_005b5689:
        puVar1 = (volatile u32 *)(self + 0x188b + mode * 8);
        uVar3 = *(volatile u32 *)(self + 0x188b + mode * 8) & 0xbffcffff;
      }
    }
  }
  else {
    if ((0 < (int)param_4) || ((-1 < (int)param_4 && (0x8000000 < param_3)))) {
      if ((int)param_4 < 2) {
        if (((int)param_4 < 1) || (param_3 == 0)) {
          if ((param_3 == 0) && (param_4 == 1)) {
LAB_005b5735:
            puVar1 = (volatile u32 *)(self + 0x188b + mode * 8);
            *puVar1 = *(volatile u32 *)(self + 0x188b + mode * 8);
            uVar3 = puVar1[1] & 0xfffffffc;
            goto LAB_005b539c;
          }
          if (((int)param_4 < 1) && (((int)param_4 < 0 || (param_3 < 0x40000001)))) {
            if ((param_3 == 0x40000000) && (param_4 == 0)) goto LAB_005b5689;
            if (((param_3 == 0x10000000) && (param_4 == 0)) ||
               ((param_3 == 0x20000000 && (param_4 == 0)))) {
              puVar1 = (volatile u32 *)(self + 0x188b + mode * 8);
              uVar3 = *(volatile u32 *)(self + 0x188b + mode * 8) & 0xcfffffff;
              goto LAB_005b5397;
            }
          }
          else if ((param_3 == 0x80000000) && (param_4 == 0)) goto LAB_005b56b1;
        }
      }
      else if (param_3 == 0) {
        if (param_4 == 2) goto LAB_005b5735;
        if ((param_4 == 4) || (param_4 == 8)) {
          puVar1 = (volatile u32 *)(self + 0x188b + mode * 8);
          *puVar1 = *(volatile u32 *)(self + 0x188b + mode * 8);
          uVar3 = puVar1[1] & 0xfffffff3;
          goto LAB_005b539c;
        }
      }
LAB_005b56e8:
      cm_fn_00944cff((u32)s_E__dev_CM3_cm3_00_01_cm3_code_fo_009cc3dc,0,0,local_100,local_200);
      format_arg1 = (u32)local_100;
      format_arg2 = (u32)local_200;
      error_code = 0x711;
LAB_005b5780:
      uVar5 = cm_fn_0090d130();
      cm_fn_00944c9f((u32)&DAT_00ae24d0,(u32)s_v_s__s__s__d_009870e8,uVar5,format_arg1,format_arg2,error_code);
      cm_fn_005e8290(s_Error_009870e0,(const char *)&DAT_00ae24d0,0);
      DAT_00b67a34 = 0;
      return 0;
    }
    if ((param_3 != 0x8000000) || (param_4 != 0)) {
      if (((int)param_4 < 1) && (((int)param_4 < 0 || (param_3 < 0x800001)))) {
        if ((param_3 != 0x800000) || (param_4 != 0)) {
          if (((int)param_4 < 1) && (((int)param_4 < 0 || (param_3 < 0x200001)))) {
            if ((param_3 != 0x200000) || (param_4 != 0)) {
              if ((param_3 == 0x80000) && (param_4 == 0)) goto LAB_005b56b1;
              if ((param_3 != 0x100000) || (param_4 != 0)) goto LAB_005b56e8;
            }
            puVar1 = (volatile u32 *)(self + 0x188b + mode * 8);
            uVar3 = *(volatile u32 *)(self + 0x188b + mode * 8) & 0xffcfffff;
            goto LAB_005b5397;
          }
          if ((param_3 != 0x400000) || (param_4 != 0)) goto LAB_005b56e8;
        }
        puVar1 = (volatile u32 *)(self + 0x188b + mode * 8);
        uVar3 = *(volatile u32 *)(self + 0x188b + mode * 8) & 0xff3fffff;
        goto LAB_005b5397;
      }
      if (((param_3 == 0x1000000) && (param_4 == 0)) || ((param_3 == 0x2000000 && (param_4 == 0))))
      {
        puVar1 = (volatile u32 *)(self + 0x188b + mode * 8);
        uVar3 = *(volatile u32 *)(self + 0x188b + mode * 8) & 0xfcffffff;
        goto LAB_005b5397;
      }
      if ((param_3 != 0x4000000) || (param_4 != 0)) goto LAB_005b56e8;
    }
    puVar1 = (volatile u32 *)(self + 0x188b + mode * 8);
    uVar3 = *(volatile u32 *)(self + 0x188b + mode * 8) & 0xf3ffffff;
  }
LAB_005b5397:
  *puVar1 = uVar3;
  uVar3 = puVar1[1];
LAB_005b539c:
  puVar1[1] = uVar3;
  *puVar1 = *puVar1 | param_3;
  puVar1[1] = puVar1[1] | param_4;
  *(volatile u32 *)(self + 0x187f) = 0;
  return 1;
}




