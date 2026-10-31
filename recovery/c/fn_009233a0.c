/* Draft for preferred-image VA 0x009233a0, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
/* WIP: REP string instructions consume incoming DF; plain C has no faithful DF transport. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_00ad4e94 CM_GLOBAL(u8, 0x00ad4e94)
#define DAT_00ad4ed8 CM_GLOBAL(u8, 0x00ad4ed8)
#define DAT_00ad4f38 CM_GLOBAL(u8, 0x00ad4f38)
#define DAT_00ad9c64 CM_GLOBAL(u8, 0x00ad9c64)
#define PTR_LAB_00746556_1_00ad4f1c CM_GLOBAL(void *, 0x00ad4f1c)
#define PTR_LAB_00ad4f20 CM_GLOBAL(void *, 0x00ad4f20)
#define s_Breezy_00ad4f54 ((char *)0x00ad4f54)
#define s_Downpour_00ad4f08 ((char *)0x00ad4f08)
#define s_Drizzle_00ad4f14 ((char *)0x00ad4f14)
#define s_Gusty_00ad4f4c ((char *)0x00ad4f4c)
#define s_Heavy_Snow_00ad4ee8 ((char *)0x00ad4ee8)
#define s_Invalid_WeatherSeasonPrecipitati_00ad4eb0 ((char *)0x00ad4eb0)
#define s_Invalid_Wind___d_00ad4f24 ((char *)0x00ad4f24)
#define s_Light_Snow_00ad4ef4 ((char *)0x00ad4ef4)
#define s_Shower_00ad4f00 ((char *)0x00ad4f00)
#define s_Sleet_00ad4ee0 ((char *)0x00ad4ee0)
#define s_Strong_Wind_00ad4f40 ((char *)0x00ad4f40)
#define s__s___s_0098f6ac ((char *)0x0098f6ac)
#define s__s___s___s_00ad4e88 ((char *)0x00ad4e88)
extern /* WARNING: Globals starting with '_' overlap smaller symbols at the same address */ char * cm_fn_0066f4e0(int param_1,char *param_2);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);

void cm_fn_009233a0(u32 param_1,char *param_2)

{
  char cVar1;
  u32 uVar2;
  u32 uVar3;
  char *pcVar4;
  char *pcVar5;
  char local_5dc [500];
  char local_3e8 [500];
  u8 local_1f4 [500];
  
  switch((int)*param_2) {
  case 0:
    uVar2 = 0xffffffff;
    pcVar5 = &DAT_00ad9c64;
    do {
      pcVar4 = pcVar5;
      if (uVar2 == 0) break;
      uVar2 = uVar2 - 1;
      pcVar4 = pcVar5 + 1;
      cVar1 = *pcVar5;
      pcVar5 = pcVar4;
    } while (cVar1 != '\0');
    uVar2 = ~uVar2;
    pcVar5 = pcVar4 + -uVar2;
    pcVar4 = local_3e8;
    for (uVar3 = uVar2 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
      *(u32 *)pcVar4 = *(u32 *)pcVar5;
      pcVar5 = pcVar5 + 4;
      pcVar4 = pcVar4 + 4;
    }
    for (uVar2 = uVar2 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
      *pcVar4 = *pcVar5;
      pcVar5 = pcVar5 + 1;
      pcVar4 = pcVar4 + 1;
    }
    break;
  case 1:
    pcVar5 = s_Breezy_00ad4f54;
    goto LAB_00923424;
  case 2:
    cm_fn_0066f4e0(local_3e8,s_Gusty_00ad4f4c);
    break;
  case 3:
    cm_fn_0066f4e0(local_3e8,s_Strong_Wind_00ad4f40);
    break;
  case 4:
    pcVar5 = &DAT_00ad4f38;
LAB_00923424:
    cm_fn_0066f4e0(local_3e8,pcVar5);
    break;
  default:
    cm_fn_00944c9f(local_1f4,s_Invalid_Wind___d_00ad4f24,(int)*param_2);
  }
  switch((int)param_2[1]) {
  case 0:
    if (local_3e8[0] != '\0') {
      uVar2 = 0xffffffff;
      pcVar5 = &DAT_00ad9c64;
      do {
        pcVar4 = pcVar5;
        if (uVar2 == 0) break;
        uVar2 = uVar2 - 1;
        pcVar4 = pcVar5 + 1;
        cVar1 = *pcVar5;
        pcVar5 = pcVar4;
      } while (cVar1 != '\0');
      uVar2 = ~uVar2;
      pcVar5 = pcVar4 + -uVar2;
      pcVar4 = local_5dc;
      for (uVar3 = uVar2 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
        *(u32 *)pcVar4 = *(u32 *)pcVar5;
        pcVar5 = pcVar5 + 4;
        pcVar4 = pcVar4 + 4;
      }
      for (uVar2 = uVar2 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
        *pcVar4 = *pcVar5;
        pcVar5 = pcVar5 + 1;
        pcVar4 = pcVar4 + 1;
      }
      break;
    }
    pcVar5 = (char *)&PTR_LAB_00ad4f20;
    goto LAB_00923470;
  case 1:
    cm_fn_0066f4e0(local_5dc,&PTR_LAB_00746556_1_00ad4f1c);
    break;
  case 2:
    cm_fn_0066f4e0(local_5dc,s_Drizzle_00ad4f14);
    break;
  case 3:
    cm_fn_0066f4e0(local_5dc,s_Shower_00ad4f00);
    break;
  default:
    cm_fn_00944c9f(local_5dc,s_Invalid_WeatherSeasonPrecipitati_00ad4eb0,(int)param_2[1]);
    break;
  case 5:
    pcVar5 = s_Downpour_00ad4f08;
    goto LAB_00923470;
  case 6:
    cm_fn_0066f4e0(local_5dc,s_Sleet_00ad4ee0);
    break;
  case 7:
    cm_fn_0066f4e0(local_5dc,&DAT_00ad4ed8);
    break;
  case 8:
    cm_fn_0066f4e0(local_5dc,s_Light_Snow_00ad4ef4);
    break;
  case 9:
    pcVar5 = s_Heavy_Snow_00ad4ee8;
LAB_00923470:
    cm_fn_0066f4e0(local_5dc,pcVar5);
  }
  cm_fn_0066f4e0(local_1f4,&DAT_00ad4e94,(int)param_2[2]);
  pcVar5 = local_5dc;
  if (local_3e8[0] != '\0') {
    if (local_5dc[0] != '\0') {
      cm_fn_00944c9f(param_1,s__s___s___s_00ad4e88,local_3e8,local_5dc,local_1f4);
      return;
    }
    pcVar5 = local_3e8;
  }
  cm_fn_00944c9f(param_1,s__s___s_0098f6ac,pcVar5,local_1f4);
  return;
}
