/* Draft for preferred-image VA 0x008fc670, image base 0x00400000, converted from Ghidra 12.1.4
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
#define s_E__dev_CM3_cm3_00_01_cm3_code_co_00aa180c ((char *)0x00aa180c)
#define s_Error_009870e0 ((char *)0x009870e0)
#define s_v_s__s__s__d_009870e8 ((char *)0x009870e8)
extern void CM_CDECL cm_fn_00521e60(void *object,short mode,u32 param_3,u32 param_4,int param_5, u32 param_6,int param_7);
extern void CM_CDECL cm_fn_00521eb0(void *object,short mode,u32 param_3,u32 param_4,int param_5, u32 param_6,u8 param_7,int param_8,u32 param_9);
extern void cm_fn_005e8290(const char * self,const char * mode,int param_3);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);

void * CM_THISCALL
cm_fn_008fc670(u32 self,s8 mode,u16 *param_3,u16 *param_4,u32 *param_5)

{
  void *pvVar1;
  u32 uVar2;
  u8 scratch[0x200];
  u8 *local_200 = scratch;
  u8 *local_100 = scratch + 0x100;
  
  if (mode == -1) {
    if (param_5 != (u32 *)0x0) {
      *(volatile u32 *)param_5 = 0;
    }
    *(volatile u16 *)param_3 = 7;
    *(volatile u16 *)param_4 = 0;
    pvVar1 = (void *)(u32)cm_fn_00944e46(0x2d8);
    if (pvVar1 != (void *)0x0) {
      cm_fn_00521e60(pvVar1,0,0x11,9,0,1,(int)*(volatile s16 *)(self + 0x40));
      cm_fn_00521eb0(pvVar1,0,0x19,9,0,2,2,(int)*(volatile s16 *)(self + 0x40),0);
      *(volatile u16 *)((u32)pvVar1 + 7) = 10;
      *(volatile u16 *)((u32)pvVar1 + 9) = 0;
      *(volatile u16 *)((u32)pvVar1 + 0xb) = 0;
      *(volatile u16 *)((u32)pvVar1 + 0xd) = 0x83;
      *(volatile u8 *)((u32)pvVar1 + 0x17) = 4;
      *(volatile u16 *)((u32)pvVar1 + 0x18) = 0x24;
      *(volatile u16 *)((u32)pvVar1 + 0x1a) = 0x12;
      *(volatile u16 *)((u32)pvVar1 + 0x1c) = 0x24;
      *(volatile u16 *)((u32)pvVar1 + 0xf) = 0;
      *(volatile u16 *)((u32)pvVar1 + 0x1e) = 0;
      *(volatile u8 *)((u32)pvVar1 + 0x20) = 0;
      *(volatile u8 *)((u32)pvVar1 + 0x21) = 1;
      *(volatile u8 *)((u32)pvVar1 + 0x22) = 4;
      *(volatile u32 *)((u32)pvVar1 + 0x5c) = 0;
      *(volatile u32 *)((u32)pvVar1 + 0x60) = 0xffffffff;
      *(volatile u32 *)((u32)pvVar1 + 100) = 0;
      cm_fn_00521e60(pvVar1,1,0x1f,9,0,1,(int)*(volatile s16 *)(self + 0x40));
      cm_fn_00521eb0(pvVar1,1,8,10,0,2,2,(int)*(volatile s16 *)(self + 0x40),0);
      *(volatile u16 *)((u32)pvVar1 + 0x6f) = 0x14;
      *(volatile u16 *)((u32)pvVar1 + 0x71) = 1;
      *(volatile u16 *)((u32)pvVar1 + 0x73) = 0;
      *(volatile u16 *)((u32)pvVar1 + 0x75) = 0x83;
      *(volatile u8 *)((u32)pvVar1 + 0x7f) = 4;
      *(volatile u16 *)((u32)pvVar1 + 0x80) = 0x1c;
      *(volatile u16 *)((u32)pvVar1 + 0x82) = 0xe;
      *(volatile u16 *)((u32)pvVar1 + 0x84) = 10;
      *(volatile u16 *)((u32)pvVar1 + 0x77) = 0;
      *(volatile u16 *)((u32)pvVar1 + 0x86) = 0x24;
      *(volatile u8 *)((u32)pvVar1 + 0x88) = 0;
      *(volatile u8 *)((u32)pvVar1 + 0x89) = 1;
      *(volatile u8 *)((u32)pvVar1 + 0x8a) = 0;
      *(volatile u32 *)((u32)pvVar1 + 0xc4) = 0;
      *(volatile u32 *)((u32)pvVar1 + 200) = 0xffffffff;
      *(volatile u32 *)((u32)pvVar1 + 0xcc) = 0;
      cm_fn_00521e60(pvVar1,2,5,0xb,0,1,(int)*(volatile s16 *)(self + 0x40));
      cm_fn_00521eb0(pvVar1,2,0xd,0xb,0,2,2,(int)*(volatile s16 *)(self + 0x40),0);
      *(volatile u16 *)((u32)pvVar1 + 0xd7) = 0x1e;
      *(volatile u16 *)((u32)pvVar1 + 0xd9) = 2;
      *(volatile u16 *)((u32)pvVar1 + 0xdb) = 0;
      *(volatile u16 *)((u32)pvVar1 + 0xdd) = 0x83;
      *(volatile u16 *)((u32)pvVar1 + 0xdf) = 0;
      *(volatile u8 *)((u32)pvVar1 + 0xe7) = 4;
      *(volatile u16 *)((u32)pvVar1 + 0xe8) = 0x20;
      *(volatile u16 *)((u32)pvVar1 + 0xea) = 0x10;
      *(volatile u16 *)((u32)pvVar1 + 0xec) = 0x12;
      *(volatile u16 *)((u32)pvVar1 + 0xee) = 0x2e;
      *(volatile u8 *)((u32)pvVar1 + 0xf0) = 0;
      *(volatile u8 *)((u32)pvVar1 + 0xf1) = 1;
      *(volatile u8 *)((u32)pvVar1 + 0xf2) = 0;
      *(volatile u32 *)((u32)pvVar1 + 300) = 0;
      *(volatile u32 *)((u32)pvVar1 + 0x130) = 0xffffffff;
      *(volatile u32 *)((u32)pvVar1 + 0x134) = 0;
      cm_fn_00521e60(pvVar1,3,9,0,1,1,(int)*(volatile s16 *)(self + 0x40));
      cm_fn_00521eb0(pvVar1,3,0x11,0,1,2,2,(int)*(volatile s16 *)(self + 0x40),0);
      *(volatile u16 *)((u32)pvVar1 + 0x13f) = 0x28;
      *(volatile u16 *)((u32)pvVar1 + 0x141) = 3;
      *(volatile u16 *)((u32)pvVar1 + 0x143) = 0;
      *(volatile u16 *)((u32)pvVar1 + 0x145) = 0x83;
      *(volatile u16 *)((u32)pvVar1 + 0x147) = 0;
      *(volatile u8 *)((u32)pvVar1 + 0x14f) = 4;
      *(volatile u16 *)((u32)pvVar1 + 0x150) = 0x10;
      *(volatile u16 *)((u32)pvVar1 + 0x152) = 8;
      *(volatile u16 *)((u32)pvVar1 + 0x154) = 0;
      *(volatile u16 *)((u32)pvVar1 + 0x156) = 0;
      *(volatile u8 *)((u32)pvVar1 + 0x158) = 0;
      *(volatile u8 *)((u32)pvVar1 + 0x159) = 1;
      *(volatile u8 *)((u32)pvVar1 + 0x15a) = 0;
      *(volatile u32 *)((u32)pvVar1 + 0x194) = 0;
      *(volatile u32 *)((u32)pvVar1 + 0x198) = 0;
      *(volatile u32 *)((u32)pvVar1 + 0x19c) = 3000;
      cm_fn_00521e60(pvVar1,4,10,0,1,2,(int)*(volatile s16 *)(self + 0x40));
      cm_fn_00521eb0(pvVar1,4,0x1f,0,1,2,2,(int)*(volatile s16 *)(self + 0x40),0);
      *(volatile u16 *)((u32)pvVar1 + 0x1a7) = 0x78;
      *(volatile u16 *)((u32)pvVar1 + 0x1a9) = 4;
      *(volatile u16 *)((u32)pvVar1 + 0x1ab) = 0;
      *(volatile u16 *)((u32)pvVar1 + 0x1ad) = 0x83;
      *(volatile u16 *)((u32)pvVar1 + 0x1af) = 0;
      *(volatile u8 *)((u32)pvVar1 + 0x1b7) = 6;
      *(volatile u16 *)((u32)pvVar1 + 0x1b8) = 8;
      *(volatile u16 *)((u32)pvVar1 + 0x1ba) = 4;
      *(volatile u16 *)((u32)pvVar1 + 0x1bc) = 0;
      *(volatile u16 *)((u32)pvVar1 + 0x1be) = 0;
      *(volatile u8 *)((u32)pvVar1 + 0x1c0) = 0;
      *(volatile u8 *)((u32)pvVar1 + 0x1c1) = 1;
      *(volatile u8 *)((u32)pvVar1 + 0x1c2) = 0;
      *(volatile u32 *)((u32)pvVar1 + 0x1fc) = 0;
      *(volatile u32 *)((u32)pvVar1 + 0x200) = 0xffffffff;
      *(volatile u32 *)((u32)pvVar1 + 0x204) = 0;
      cm_fn_00521e60(pvVar1,5,6,1,1,1,(int)*(volatile s16 *)(self + 0x40));
      cm_fn_00521eb0(pvVar1,5,0xe,1,1,2,2,(int)*(volatile s16 *)(self + 0x40),0);
      *(volatile u16 *)((u32)pvVar1 + 0x20f) = 0x82;
      *(volatile u16 *)((u32)pvVar1 + 0x211) = 5;
      *(volatile u16 *)((u32)pvVar1 + 0x213) = 0;
      *(volatile u16 *)((u32)pvVar1 + 0x215) = 0x83;
      *(volatile u16 *)((u32)pvVar1 + 0x217) = 0;
      *(volatile u8 *)((u32)pvVar1 + 0x21f) = 6;
      *(volatile u16 *)((u32)pvVar1 + 0x220) = 4;
      *(volatile u16 *)((u32)pvVar1 + 0x222) = 2;
      *(volatile u16 *)((u32)pvVar1 + 0x224) = 0;
      *(volatile u16 *)((u32)pvVar1 + 0x226) = 0;
      *(volatile u8 *)((u32)pvVar1 + 0x228) = 0;
      *(volatile u8 *)((u32)pvVar1 + 0x229) = 1;
      *(volatile u8 *)((u32)pvVar1 + 0x22a) = 0;
      *(volatile u32 *)((u32)pvVar1 + 0x264) = 0;
      *(volatile u32 *)((u32)pvVar1 + 0x268) = 0xffffffff;
      *(volatile u32 *)((u32)pvVar1 + 0x26c) = 0;
      cm_fn_00521e60(pvVar1,6,0xf,1,1,3,(int)*(volatile s16 *)(self + 0x40));
      cm_fn_00521eb0(pvVar1,6,2,4,1,2,2,(int)*(volatile s16 *)(self + 0x40),4);
      *(volatile u16 *)((u32)pvVar1 + 0x277) = 0x96;
      *(volatile u16 *)((u32)pvVar1 + 0x279) = 6;
      *(volatile u16 *)((u32)pvVar1 + 0x27b) = 0;
      *(volatile u16 *)((u32)pvVar1 + 0x27d) = 0x83;
      *(volatile u16 *)((u32)pvVar1 + 0x27f) = 0;
      *(volatile u8 *)((u32)pvVar1 + 0x287) = 6;
      *(volatile u16 *)((u32)pvVar1 + 0x288) = 2;
      *(volatile u16 *)((u32)pvVar1 + 0x28a) = 1;
      *(volatile u16 *)((u32)pvVar1 + 0x28c) = 0;
      *(volatile u16 *)((u32)pvVar1 + 0x28e) = 0;
      *(volatile u8 *)((u32)pvVar1 + 0x290) = 0;
      *(volatile u8 *)((u32)pvVar1 + 0x291) = 1;
      *(volatile u8 *)((u32)pvVar1 + 0x292) = 0;
      *(volatile u32 *)((u32)pvVar1 + 0x2cc) = 0;
      *(volatile u32 *)((u32)pvVar1 + 0x2d0) = 0xffffffff;
      *(volatile u32 *)((u32)pvVar1 + 0x2d4) = 0;
      return pvVar1;
    }
    cm_fn_00944cff((u32)s_E__dev_CM3_cm3_00_01_cm3_code_co_00aa180c,0,0,local_100,local_200);
    uVar2 = cm_fn_0090d130();
    cm_fn_00944c9f((u32)&DAT_00ae24d0,(u32)s_v_s__s__s__d_009870e8,
                   uVar2,(u32)local_100,(u32)local_200,0x9d);
    cm_fn_005e8290(s_Error_009870e0,(const char *)&DAT_00ae24d0,0);
    DAT_00b67a34 = 0;
  }
  return (void *)0x0;
}





