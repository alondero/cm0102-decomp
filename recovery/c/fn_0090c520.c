/* Draft for preferred-image VA 0x0090c520, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_0098806c CM_GLOBAL(u8, 0x0098806c)
#define DAT_00ad4288 CM_GLOBAL(u8, 0x00ad4288)
#define DAT_00ad428c CM_GLOBAL(u8, 0x00ad428c)
#define DAT_00ad42a4 CM_GLOBAL(u8, 0x00ad42a4)
#define DAT_00ad42a8 CM_GLOBAL(u8, 0x00ad42a8)
#define DAT_00ad42b0 CM_GLOBAL(u8, 0x00ad42b0)
#define DAT_00ad42c0 CM_GLOBAL(u8, 0x00ad42c0)
#define DAT_00ad42c4 CM_GLOBAL(u8, 0x00ad42c4)
#define DAT_00ad42c8 CM_GLOBAL(u8, 0x00ad42c8)
#define DAT_00ad4334 CM_GLOBAL(u8, 0x00ad4334)
#define DAT_00ad4338 CM_GLOBAL(u8, 0x00ad4338)
#define DAT_00ad4354 CM_GLOBAL(u8, 0x00ad4354)
#define DAT_00ad435c CM_GLOBAL(u8, 0x00ad435c)
#define DAT_00ad436c CM_GLOBAL(u8, 0x00ad436c)
#define DAT_00ad4370 CM_GLOBAL(u8, 0x00ad4370)
#define DAT_00ad4374 CM_GLOBAL(u8, 0x00ad4374)
#define DAT_00ae24d0 (*(volatile u8 *)0x00ae24d0)
#define DAT_00b67a34 (*(volatile u32 *)0x00b67a34)
#define PTR_LAB_00786972_1_00ad4350 CM_GLOBAL(void *, 0x00ad4350)
#define s_E__dev_CM3_cm3_00_01_si_code_uti_00ad4134 ((char *)0x00ad4134)
#define s_Eight_00ad4294 ((char *)0x00ad4294)
#define s_Eighteen_00ad4238 ((char *)0x00ad4238)
#define s_Eleven_00ad4280 ((char *)0x00ad4280)
#define s_Error_009870e0 ((char *)0x009870e0)
#define s_Fifteen_00ad4258 ((char *)0x00ad4258)
#define s_Fourteen_00ad4260 ((char *)0x00ad4260)
#define s_Nineteen_00ad422c ((char *)0x00ad422c)
#define s_Seven_00ad429c ((char *)0x00ad429c)
#define s_Seventeen_00ad4244 ((char *)0x00ad4244)
#define s_Sixteen_00ad4250 ((char *)0x00ad4250)
#define s_Thirteen_00ad426c ((char *)0x00ad426c)
#define s_Three_00ad42b8 ((char *)0x00ad42b8)
#define s_Twelve_00ad4278 ((char *)0x00ad4278)
#define s_Twenty_00ad4224 ((char *)0x00ad4224)
#define s_eight_00ad4340 ((char *)0x00ad4340)
#define s_eighteen_00ad42e4 ((char *)0x00ad42e4)
#define s_eleven_00ad432c ((char *)0x00ad432c)
#define s_fifteen_00ad4304 ((char *)0x00ad4304)
#define s_fourteen_00ad430c ((char *)0x00ad430c)
#define s_nineteen_00ad42d8 ((char *)0x00ad42d8)
#define s_seven_00ad4348 ((char *)0x00ad4348)
#define s_seventeen_00ad42f0 ((char *)0x00ad42f0)
#define s_sixteen_00ad42fc ((char *)0x00ad42fc)
#define s_thirteen_00ad4318 ((char *)0x00ad4318)
#define s_three_00ad4364 ((char *)0x00ad4364)
#define s_twelve_00ad4324 ((char *)0x00ad4324)
#define s_twenty_00ad42d0 ((char *)0x00ad42d0)
#define s_v_s__s__s__d_009870e8 ((char *)0x009870e8)
extern void cm_fn_005e8290(const char * param_1,const char * param_2,int param_3);
extern /* WARNING: Globals starting with '_' overlap smaller symbols at the same address */ char * cm_fn_0066f4e0(int param_1,char *param_2);
extern char * cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

u32 cm_fn_0090c520(u32 param_1,int param_2,short param_3,int param_4)

{
  u32 uVar3;
  u8 scratch[512];
  u8 *local_200 = scratch;
  u8 *local_100 = scratch + 256;
  
  if (param_2 == 0) {
    cm_fn_00944cff((u32)s_E__dev_CM3_cm3_00_01_si_code_uti_00ad4134,0,0,local_100,local_200);
    uVar3 = (u32)cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,(u32)s_v_s__s__s__d_009870e8,uVar3,local_100,local_200,0x45d);
    cm_fn_005e8290(s_Error_009870e0,(const char *)0x00ae24d0,0);
    DAT_00b67a34 = 0;
    return 0;
  }
  else {
    if (0x31 < param_3) {
      if (param_4 == 0) {
        if (param_1 <= 0x14U) {
          (void)*(volatile u32 *)(0x0090cac0U + param_1 * 4U);
        }
        switch(param_1) {
        case 0:
          cm_fn_0066f4e0(param_2,(char *)&DAT_00ad42c8);
          return 1;
        case 1:
          cm_fn_0066f4e0(param_2,(char *)&DAT_00ad42c4);
          return 1;
        case 2:
          cm_fn_0066f4e0(param_2,(char *)&DAT_00ad42c0);
          return 1;
        case 3:
          cm_fn_0066f4e0(param_2,s_Three_00ad42b8);
          return 1;
        case 4:
          cm_fn_0066f4e0(param_2,(char *)&DAT_00ad42b0);
          return 1;
        case 5:
          cm_fn_0066f4e0(param_2,(char *)&DAT_00ad42a8);
          return 1;
        case 6:
          cm_fn_0066f4e0(param_2,(char *)&DAT_00ad42a4);
          return 1;
        case 7:
          cm_fn_0066f4e0(param_2,s_Seven_00ad429c);
          return 1;
        case 8:
          cm_fn_0066f4e0(param_2,s_Eight_00ad4294);
          return 1;
        case 9:
          cm_fn_0066f4e0(param_2,(char *)&DAT_00ad428c);
          return 1;
        case 10:
          cm_fn_0066f4e0(param_2,(char *)&DAT_00ad4288);
          return 1;
        case 0xb:
          cm_fn_0066f4e0(param_2,s_Eleven_00ad4280);
          return 1;
        case 0xc:
          cm_fn_0066f4e0(param_2,s_Twelve_00ad4278);
          return 1;
        case 0xd:
          cm_fn_0066f4e0(param_2,s_Thirteen_00ad426c);
          return 1;
        case 0xe:
          cm_fn_0066f4e0(param_2,s_Fourteen_00ad4260);
          return 1;
        case 0xf:
          cm_fn_0066f4e0(param_2,s_Fifteen_00ad4258);
          return 1;
        case 0x10:
          cm_fn_0066f4e0(param_2,s_Sixteen_00ad4250);
          return 1;
        case 0x11:
          cm_fn_0066f4e0(param_2,s_Seventeen_00ad4244);
          return 1;
        case 0x12:
          cm_fn_0066f4e0(param_2,s_Eighteen_00ad4238);
          return 1;
        case 0x13:
          cm_fn_0066f4e0(param_2,s_Nineteen_00ad422c);
          return 1;
        case 0x14:
          cm_fn_0066f4e0(param_2,s_Twenty_00ad4224);
          return 1;
        }
      }
      else {
        if (param_1 <= 0x14U) {
          (void)*(volatile u32 *)(0x0090ca6cU + param_1 * 4U);
        }
        switch(param_1) {
        case 0:
          cm_fn_0066f4e0(param_2,(char *)&DAT_00ad4374);
          return 1;
        case 1:
          cm_fn_0066f4e0(param_2,(char *)&DAT_00ad4370);
          return 1;
        case 2:
          cm_fn_0066f4e0(param_2,(char *)&DAT_00ad436c);
          return 1;
        case 3:
          cm_fn_0066f4e0(param_2,s_three_00ad4364);
          return 1;
        case 4:
          cm_fn_0066f4e0(param_2,(char *)&DAT_00ad435c);
          return 1;
        case 5:
          cm_fn_0066f4e0(param_2,(char *)&DAT_00ad4354);
          return 1;
        case 6:
          cm_fn_0066f4e0(param_2,(char *)&PTR_LAB_00786972_1_00ad4350);
          return 1;
        case 7:
          cm_fn_0066f4e0(param_2,s_seven_00ad4348);
          return 1;
        case 8:
          cm_fn_0066f4e0(param_2,s_eight_00ad4340);
          return 1;
        case 9:
          cm_fn_0066f4e0(param_2,(char *)&DAT_00ad4338);
          return 1;
        case 10:
          cm_fn_0066f4e0(param_2,(char *)&DAT_00ad4334);
          return 1;
        case 0xb:
          cm_fn_0066f4e0(param_2,s_eleven_00ad432c);
          return 1;
        case 0xc:
          cm_fn_0066f4e0(param_2,s_twelve_00ad4324);
          return 1;
        case 0xd:
          cm_fn_0066f4e0(param_2,s_thirteen_00ad4318);
          return 1;
        case 0xe:
          cm_fn_0066f4e0(param_2,s_fourteen_00ad430c);
          return 1;
        case 0xf:
          cm_fn_0066f4e0(param_2,s_fifteen_00ad4304);
          return 1;
        case 0x10:
          cm_fn_0066f4e0(param_2,s_sixteen_00ad42fc);
          return 1;
        case 0x11:
          cm_fn_0066f4e0(param_2,s_seventeen_00ad42f0);
          return 1;
        case 0x12:
          cm_fn_0066f4e0(param_2,s_eighteen_00ad42e4);
          return 1;
        case 0x13:
          cm_fn_0066f4e0(param_2,s_nineteen_00ad42d8);
          return 1;
        case 0x14:
          cm_fn_0066f4e0(param_2,s_twenty_00ad42d0);
          return 1;
        }
      }
      cm_fn_00944c9f((u32)param_2,0x0098806cU,param_1);
      return 1;
    }
    cm_fn_00944cff((u32)s_E__dev_CM3_cm3_00_01_si_code_uti_00ad4134,0,0,local_200,local_100);
    uVar3 = (u32)cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,(u32)s_v_s__s__s__d_009870e8,uVar3,local_200,local_100,0x464);
    cm_fn_005e8290(s_Error_009870e0,(const char *)0x00ae24d0,0);
    DAT_00b67a34 = 0;
    return 0;
  }
  return 0;
}
