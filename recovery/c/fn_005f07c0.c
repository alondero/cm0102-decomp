/* The error formatter receives the deferred buffer and line pushes, not d130. */
/* Draft for preferred-image VA 0x005f07c0, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_00ad9c64 CM_GLOBAL(volatile u8, 0x00ad9c64)
#define DAT_00ae24d0 CM_GLOBAL(volatile u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(volatile u32, 0x00b67a34)
#define _s___COMMENT_009ce894 ((u32)0x009ce894)
#define _s___COMMENT___h_009ce7e0 ((u32)0x009ce7e0)
#define _s___COMMENT___hal_009ce5b4 ((u32)0x009ce5b4)
#define _s___COMMENT___hall_o_009ce534 ((u32)0x009ce534)
#define _s___COMMENT___hall_of_009ceb3c ((u32)0x009ceb3c)
#define _s___COMMENT___hall_of_009ceb7c ((u32)0x009ceb7c)
#define _s___COMMENT___hall_of_f_009ce6a4 ((u32)0x009ce6a4)
#define _s___COMMENT___hall_of_f_009ce720 ((u32)0x009ce720)
#define _s___COMMENT___hall_of_f_009ce8dc ((u32)0x009ce8dc)
#define _s___COMMENT___hall_of_f_009ce954 ((u32)0x009ce954)
#define _s___COMMENT___hall_of_f_009ce9cc ((u32)0x009ce9cc)
#define _s___COMMENT___hall_of_f_009cea08 ((u32)0x009cea08)
#define _s___COMMENT___hall_of_f_009cea44 ((u32)0x009cea44)
#define _s___COMMENT___hall_of_f_009cea80 ((u32)0x009cea80)
#define _s___COMMENT___hall_of_f_009ceafc ((u32)0x009ceafc)
#define _s___COMMENT___hall_of_fa_009ce5f4 ((u32)0x009ce5f4)
#define _s___COMMENT___hall_of_fa_009ce630 ((u32)0x009ce630)
#define _s___COMMENT___hall_of_fa_009ce6e0 ((u32)0x009ce6e0)
#define _s___COMMENT___hall_of_fa_009ce75c ((u32)0x009ce75c)
#define _s___COMMENT___hall_of_fa_009ce79c ((u32)0x009ce79c)
#define _s___COMMENT___hall_of_fa_009ce918 ((u32)0x009ce918)
#define _s___COMMENT___hall_of_fa_009ce990 ((u32)0x009ce990)
#define _s___COMMENT___hall_of_fa_009ceac0 ((u32)0x009ceac0)
#define _s___COMMENT___hall_of_fam_009ce574 ((u32)0x009ce574)
#define _s___COMMENT___hall_of_fam_009ce66c ((u32)0x009ce66c)
#define _s___COMMENT___hall_of_fam_009ce824 ((u32)0x009ce824)
#define _s___COMMENT___hall_of_fam_009ce85c ((u32)0x009ce85c)
#define s_Argentinian_League_009ceb68 ((u32)0x009ceb68)
#define s_Australian_League_009ceb28 ((u32)0x009ceb28)
#define s_Belgian_League_009ceaec ((u32)0x009ceaec)
#define s_Brazilian_League_009ceaac ((u32)0x009ceaac)
#define s_Croatian_League_009cea70 ((u32)0x009cea70)
#define s_Danish_League_009cea34 ((u32)0x009cea34)
#define s_Dutch_League_009ce8cc ((u32)0x009ce8cc)
#define s_E__dev_CM3_cm3_00_01_cm3_code_ha_009ce408 ((u32)0x009ce408)
#define s_English_League_009ce9f8 ((u32)0x009ce9f8)
#define s_Error_009870e0 ((char *)0x009870e0)
#define s_Finnish_League_009ce9bc ((u32)0x009ce9bc)
#define s_French_League_009ce980 ((u32)0x009ce980)
#define s_German_League_009ce944 ((u32)0x009ce944)
#define s_Greek_League_009ce908 ((u32)0x009ce908)
#define s_Irish_League_009ce884 ((u32)0x009ce884)
#define s_Italian_League_009ce84c ((u32)0x009ce84c)
#define s_Jan___Dec_009cebbc ((u32)0x009cebbc)
#define s_January___December_009ceba8 ((u32)0x009ceba8)
#define s_Japanese_League_009ce814 ((u32)0x009ce814)
#define s_Korean_League_009ce524 ((u32)0x009ce524)
#define s_Northern_Irish_League_009ce7c8 ((u32)0x009ce7c8)
#define s_Norwegian_League_009ce788 ((u32)0x009ce788)
#define s_Polish_League_009ce74c ((u32)0x009ce74c)
#define s_Portuguese_League_009ce70c ((u32)0x009ce70c)
#define s_Russian_League_009ce6d0 ((u32)0x009ce6d0)
#define s_Scottish_League_009ce694 ((u32)0x009ce694)
#define s_Spanish_League_009ce65c ((u32)0x009ce65c)
#define s_Swedish_League_009ce620 ((u32)0x009ce620)
#define s_Turkish_League_009ce5e4 ((u32)0x009ce5e4)
#define s_United_States_League_009ce59c ((u32)0x009ce59c)
#define s_Welsh_League_009ce564 ((u32)0x009ce564)
#define s_v_s__s__s__d_009870e8 ((u32)0x009870e8)
extern void cm_fn_005e8290(const char * param_1,const char * param_2,int param_3);
extern char * CM_CDECL cm_fn_0066f4e0(u32 destination,u32 format,...);
extern char * cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

u32 CM_CDECL cm_fn_005f07c0(s8 param_1,int param_2,int param_3,int param_4)

{
  u8 *puVar1;
  u8 *puVar2;
  u32 uVar3;
  u8 scratch[512];
  u8 *local_200 = scratch;
  u8 *local_100 = scratch + 0x100;
  
  if ((((param_1 < '\0') || ('\x1b' < param_1)) || (param_2 == 0)) || (param_3 < 0x32)) {
    cm_fn_00944cff((u32)0x009ce408,0,0,local_200,local_100);
    puVar1 = local_200;
    puVar2 = local_100;
    uVar3 = 0x3e6;
  }
  else {
    (void)*(volatile u32 *)(u32)(0x005f1014 + (u32)(u8)param_1 * 4);
    switch(param_1) {
    case '\0':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_January___December_009ceba8);
        return 1;
      }
      cm_fn_0066f4e0(param_2,s_Jan___Dec_009cebbc);
      return 1;
    case '\x01':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Argentinian_League_009ceb68);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_009ceb7c,&DAT_00ad9c64);
      return 1;
    case '\x02':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Australian_League_009ceb28);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_009ceb3c,&DAT_00ad9c64);
      return 1;
    case '\x03':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Belgian_League_009ceaec);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_f_009ceafc,&DAT_00ad9c64);
      return 1;
    case '\x04':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Brazilian_League_009ceaac);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_fa_009ceac0,&DAT_00ad9c64);
      return 1;
    case '\x05':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Croatian_League_009cea70);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_f_009cea80,&DAT_00ad9c64);
      return 1;
    case '\x06':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Danish_League_009cea34);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_f_009cea44,&DAT_00ad9c64);
      return 1;
    case '\a':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_English_League_009ce9f8);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_f_009cea08,&DAT_00ad9c64);
      return 1;
    case '\b':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Finnish_League_009ce9bc);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_f_009ce9cc,&DAT_00ad9c64);
      return 1;
    case '\t':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_French_League_009ce980);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_fa_009ce990,&DAT_00ad9c64);
      return 1;
    case '\n':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_German_League_009ce944);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_f_009ce954,&DAT_00ad9c64);
      return 1;
    case '\v':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Greek_League_009ce908);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_fa_009ce918,&DAT_00ad9c64);
      return 1;
    case '\f':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Dutch_League_009ce8cc);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_f_009ce8dc,&DAT_00ad9c64);
      return 1;
    case '\r':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Irish_League_009ce884);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT_009ce894,&DAT_00ad9c64);
      return 1;
    case '\x0e':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Italian_League_009ce84c);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_fam_009ce85c,&DAT_00ad9c64);
      return 1;
    case '\x0f':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Japanese_League_009ce814);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_fam_009ce824,&DAT_00ad9c64);
      return 1;
    case '\x10':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Northern_Irish_League_009ce7c8);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___h_009ce7e0,&DAT_00ad9c64);
      return 1;
    case '\x11':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Norwegian_League_009ce788);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_fa_009ce79c,&DAT_00ad9c64);
      return 1;
    case '\x12':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Polish_League_009ce74c);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_fa_009ce75c,&DAT_00ad9c64);
      return 1;
    case '\x13':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Portuguese_League_009ce70c);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_f_009ce720,&DAT_00ad9c64);
      return 1;
    case '\x14':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Russian_League_009ce6d0);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_fa_009ce6e0,&DAT_00ad9c64);
      return 1;
    case '\x15':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Scottish_League_009ce694);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_f_009ce6a4,&DAT_00ad9c64);
      return 1;
    case '\x16':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Spanish_League_009ce65c);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_fam_009ce66c,&DAT_00ad9c64);
      return 1;
    case '\x17':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Swedish_League_009ce620);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_fa_009ce630,&DAT_00ad9c64);
      return 1;
    case '\x18':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Turkish_League_009ce5e4);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_fa_009ce5f4,&DAT_00ad9c64);
      return 1;
    case '\x19':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_United_States_League_009ce59c);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hal_009ce5b4,&DAT_00ad9c64);
      return 1;
    case '\x1a':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Welsh_League_009ce564);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_of_fam_009ce574,&DAT_00ad9c64);
      return 1;
    case '\x1b':
      if (param_4 == 0) {
        cm_fn_0066f4e0(param_2,s_Korean_League_009ce524);
        return 1;
      }
      cm_fn_0066f4e0(param_2,_s___COMMENT___hall_o_009ce534,&DAT_00ad9c64);
      return 1;
    default:
      cm_fn_00944cff((u32)0x009ce408,0,0,local_100,local_200);
      puVar2 = local_200;
      puVar1 = local_100;
      uVar3 = 0x496;
    }
  }
  cm_fn_00944c9f((u32)&DAT_00ae24d0,(u32)0x009870e8,
                 cm_fn_0090d130(),puVar1,puVar2,uVar3);
  cm_fn_005e8290(s_Error_009870e0,(const char *)(u32)&DAT_00ae24d0,0);
  DAT_00b67a34 = 0;
  return 0;
}


