/* Draft for preferred-image VA 0x0056f320, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_00ae24d0 CM_GLOBAL(volatile u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(volatile u32, 0x00b67a34)
#define s_E__dev_CM3_cm3_00_01_cm3_code_co_009c42cc ((char *)0x009c42cc)
#define s_Error_009870e0 ((char *)0x009870e0)
#define s_v_s__s__s__d_009870e8 ((char *)0x009870e8)
extern void CM_CDECL cm_fn_00521e60(u32 object,s16 param_2,u32 param_3,u32 param_4,int param_5,u32 param_6,int param_7);
extern void CM_CDECL cm_fn_00521eb0(u32 object,s16 param_2,u32 param_3,u32 param_4,int param_5,u32 param_6,u8 param_7,int param_8,u32 param_9);
extern void cm_fn_005e8290(const char * self,const char * param_2,int param_3);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);

void * CM_THISCALL
cm_fn_0056f320(u32 self,s8 param_2,volatile u16 *param_3,volatile u16 *param_4,
               volatile u32 *param_5)

{
  char cVar1;
  u32 pvVar2;
  u32 uVar3;
  u8 scratch[0x200];
  u8 *local_200 = scratch;
  u8 *local_100 = scratch + 0x100;
  
  if (param_2 == -1) {
    if (param_5 != (u32 *)0x0) {
      *param_5 = 0;
    }
    *param_3 = 9;
    *param_4 = 0;
    pvVar2 = (u32)cm_fn_00944e46(0x3a8);
    if (pvVar2 != 0) {
      cm_fn_00521e60(pvVar2,0,9,9,0,5,(int)*(volatile s16 *)(self + 0x40));
      cm_fn_00521eb0(pvVar2,0,0x10,9,0,5,1,(int)*(volatile s16 *)(self + 0x40),0);
      *(volatile u16 *)((u32)pvVar2 + 7) = 0xdc;
      *(volatile u16 *)((u32)pvVar2 + 9) = 0;
      *(volatile u16 *)((u32)pvVar2 + 0xb) = 0;
      *(volatile u16 *)((u32)pvVar2 + 0xd) = 0;
      *(volatile u8 *)((u32)pvVar2 + 0x17) = 4;
      *(volatile u16 *)((u32)pvVar2 + 0x18) = 0x38;
      *(volatile u16 *)((u32)pvVar2 + 0x1a) = 0x1c;
      *(volatile u16 *)((u32)pvVar2 + 0x1c) = 0x38;
      *(volatile u16 *)((u32)pvVar2 + 0xf) = 3;
      *(volatile u32 *)((u32)pvVar2 + 0x13) = 0;
      *(volatile u16 *)((u32)pvVar2 + 0x1e) = 0;
      *(volatile u8 *)((u32)pvVar2 + 0x20) = 1;
      *(volatile u8 *)((u32)pvVar2 + 0x21) = 0;
      *(volatile u8 *)((u32)pvVar2 + 0x22) = 10;
      *(volatile u32 *)((u32)pvVar2 + 0x5c) = 0;
      *(volatile u32 *)((u32)pvVar2 + 0x60) = 10000;
      *(volatile u32 *)((u32)pvVar2 + 100) = 0;
      cm_fn_00521e60(pvVar2,1,0x11,9,0,6,(int)*(volatile s16 *)(self + 0x40));
      cm_fn_00521eb0(pvVar2,1,0x12,10,0,5,1,(int)*(volatile s16 *)(self + 0x40),0);
      *(volatile u16 *)((u32)pvVar2 + 0x6f) = 10;
      *(volatile u16 *)((u32)pvVar2 + 0x71) = 1;
      *(volatile u16 *)((u32)pvVar2 + 0x73) = 0;
      *(volatile u16 *)((u32)pvVar2 + 0x75) = 0;
      *(volatile u8 *)((u32)pvVar2 + 0x7f) = 4;
      *(volatile u16 *)((u32)pvVar2 + 0x80) = 0x50;
      *(volatile u16 *)((u32)pvVar2 + 0x82) = 0x28;
      *(volatile u16 *)((u32)pvVar2 + 0x84) = 0x34;
      *(volatile u16 *)((u32)pvVar2 + 0x77) = 3;
      *(volatile u32 *)((u32)pvVar2 + 0x7b) = 0;
      *(volatile u16 *)((u32)pvVar2 + 0x86) = 0x38;
      *(volatile u8 *)((u32)pvVar2 + 0x88) = 1;
      *(volatile u8 *)((u32)pvVar2 + 0x89) = 1;
      *(volatile u8 *)((u32)pvVar2 + 0x8a) = 10;
      *(volatile u32 *)((u32)pvVar2 + 0xc4) = 0;
      *(volatile u32 *)((u32)pvVar2 + 200) = 20000;
      *(volatile u32 *)((u32)pvVar2 + 0xcc) = 0;
      cm_fn_00521e60(pvVar2,2,0x13,10,0,6,(int)*(volatile s16 *)(self + 0x40));
      cm_fn_00521eb0(pvVar2,2,9,0xb,0,5,1,(int)*(volatile s16 *)(self + 0x40),0);
      *(volatile u16 *)((u32)pvVar2 + 0xd7) = 0x14;
      *(volatile u16 *)((u32)pvVar2 + 0xd9) = 2;
      *(volatile u16 *)((u32)pvVar2 + 0xdb) = 0;
      *(volatile u16 *)((u32)pvVar2 + 0xdd) = 0;
      *(volatile u16 *)((u32)pvVar2 + 0xdf) = 3;
      *(volatile u32 *)((u32)pvVar2 + 0xe3) = 0;
      *(volatile u8 *)((u32)pvVar2 + 0xe7) = 4;
      if (*(volatile s8 *)(self + 0xb4) == '\x01') {
        *(volatile u16 *)((u32)pvVar2 + 0xe8) = 0x28;
        *(volatile u16 *)((u32)pvVar2 + 0xea) = 0x13;
        *(volatile u16 *)((u32)pvVar2 + 0xec) = 0;
        *(volatile u16 *)((u32)pvVar2 + 0xee) = 0;
      }
      else {
        *(volatile s16 *)((u32)pvVar2 + 0xe8) = *(volatile s8 *)(self + 0xb3) * 6 + 0x28;
        *(volatile s16 *)((u32)pvVar2 + 0xea) = *(volatile s8 *)(self + 0xb3) * 3 + 0x14;
        *(volatile s16 *)((u32)pvVar2 + 0xec) = *(volatile s8 *)(self + 0xb3) * 6;
        *(volatile u16 *)((u32)pvVar2 + 0xee) = 0x6c;
      }
      *(volatile u8 *)((u32)pvVar2 + 0xf0) = 1;
      *(volatile u8 *)((u32)pvVar2 + 0xf1) = 1;
      *(volatile u8 *)((u32)pvVar2 + 0xf2) = 10;
      *(volatile u32 *)((u32)pvVar2 + 300) = 0;
      *(volatile u32 *)((u32)pvVar2 + 0x130) = 20000;
      *(volatile u32 *)((u32)pvVar2 + 0x134) = 0;
      cm_fn_00521e60(pvVar2,3,10,0xb,0,6,(int)*(volatile s16 *)(self + 0x40));
      cm_fn_00521eb0(pvVar2,3,6,0,1,5,1,(int)*(volatile s16 *)(self + 0x40),0);
      *(volatile u16 *)((u32)pvVar2 + 0x13f) = 0x1e;
      *(volatile u16 *)((u32)pvVar2 + 0x141) = 3;
      *(volatile u16 *)((u32)pvVar2 + 0x143) = 0;
      *(volatile u16 *)((u32)pvVar2 + 0x145) = 0;
      *(volatile u16 *)((u32)pvVar2 + 0x147) = 3;
      *(volatile u32 *)((u32)pvVar2 + 0x14b) = 0;
      *(volatile u8 *)((u32)pvVar2 + 0x14f) = 4;
      if (*(volatile s8 *)(self + 0xb4) == '\x01') {
        *(volatile u16 *)((u32)pvVar2 + 0x150) = 0x40;
        *(volatile u16 *)((u32)pvVar2 + 0x152) = 0x20;
        *(volatile u16 *)((u32)pvVar2 + 0x154) = 0x2b;
        *(volatile u16 *)((u32)pvVar2 + 0x156) = 0x6c;
      }
      else {
        *(volatile s16 *)((u32)pvVar2 + 0x150) = (0x10 - *(volatile s8 *)(self + 0xb3)) * 4;
        *(volatile s16 *)((u32)pvVar2 + 0x152) = (0x10 - *(volatile s8 *)(self + 0xb3)) * 2;
        *(volatile s16 *)((u32)pvVar2 + 0x154) = *(volatile s8 *)(self + 0xb3) * -7 + 0x2c;
        *(volatile s16 *)((u32)pvVar2 + 0x156) = (*(volatile s8 *)(self + 0xb3) + 0x12) * 6;
      }
      *(volatile u8 *)((u32)pvVar2 + 0x158) = 1;
      *(volatile u8 *)((u32)pvVar2 + 0x159) = 1;
      *(volatile u8 *)((u32)pvVar2 + 0x15a) = 10;
      *(volatile u32 *)((u32)pvVar2 + 0x194) = 0;
      *(volatile u32 *)((u32)pvVar2 + 0x198) = 30000;
      *(volatile u32 *)((u32)pvVar2 + 0x19c) = 0;
      cm_fn_00521e60(pvVar2,4,7,0,1,6,(int)*(volatile s16 *)(self + 0x40));
      cm_fn_00521eb0(pvVar2,4,0x1b,0,1,5,1,(int)*(volatile s16 *)(self + 0x40),0);
      *(volatile u16 *)((u32)pvVar2 + 0x1a7) = 0x28;
      *(volatile u16 *)((u32)pvVar2 + 0x1a9) = 4;
      *(volatile u16 *)((u32)pvVar2 + 0x1ab) = 0;
      *(volatile u16 *)((u32)pvVar2 + 0x1ad) = 0;
      *(volatile u8 *)((u32)pvVar2 + 0x1b7) = 4;
      *(volatile s16 *)((u32)pvVar2 + 0x1b8) = (0x10 - *(volatile s8 *)(self + 0xb3)) * 2;
      cVar1 = *(volatile s8 *)(self + 0xb3);
      *(volatile u16 *)((u32)pvVar2 + 0x1bc) = 0;
      *(volatile s16 *)((u32)pvVar2 + 0x1ba) = 0x10 - cVar1;
      *(volatile u16 *)((u32)pvVar2 + 0x1af) = 3;
      *(volatile u32 *)((u32)pvVar2 + 0x1b3) = 0;
      *(volatile u16 *)((u32)pvVar2 + 0x1be) = 0;
      *(volatile u8 *)((u32)pvVar2 + 0x1c0) = 1;
      *(volatile u8 *)((u32)pvVar2 + 0x1c1) = 1;
      *(volatile u8 *)((u32)pvVar2 + 0x1c2) = 10;
      *(volatile u32 *)((u32)pvVar2 + 0x1fc) = 0;
      *(volatile u32 *)((u32)pvVar2 + 0x200) = 50000;
      *(volatile u32 *)((u32)pvVar2 + 0x204) = 0;
      cm_fn_00521e60(pvVar2,5,0x1c,0,1,6,(int)*(volatile s16 *)(self + 0x40));
      cm_fn_00521eb0(pvVar2,5,0x11,1,1,5,1,(int)*(volatile s16 *)(self + 0x40),0);
      *(volatile u16 *)((u32)pvVar2 + 0x20f) = 0x32;
      *(volatile u16 *)((u32)pvVar2 + 0x211) = 5;
      *(volatile u16 *)((u32)pvVar2 + 0x213) = 0;
      *(volatile u16 *)((u32)pvVar2 + 0x215) = 0;
      *(volatile u8 *)((u32)pvVar2 + 0x21f) = 4;
      *(volatile u16 *)((u32)pvVar2 + 0x220) = 0x10;
      *(volatile u16 *)((u32)pvVar2 + 0x222) = 8;
      *(volatile s16 *)((u32)pvVar2 + 0x224) = (short)*(volatile s8 *)(self + 0xb3);
      *(volatile u16 *)((u32)pvVar2 + 0x217) = 3;
      *(volatile u32 *)((u32)pvVar2 + 0x21b) = 0;
      {
        s8 rating = *(volatile s8 *)(self + 0xb3);
        s16 output_base = *(volatile s16 *)(self + 0x3e);
        *(volatile s16 *)((u32)pvVar2 + 0x226) = (s16)(output_base - (s16)rating);
      }
      *(volatile u8 *)((u32)pvVar2 + 0x228) = 1;
      *(volatile u8 *)((u32)pvVar2 + 0x229) = 1;
      *(volatile u8 *)((u32)pvVar2 + 0x22a) = 10;
      *(volatile u32 *)((u32)pvVar2 + 0x264) = 0;
      *(volatile u32 *)((u32)pvVar2 + 0x268) = 75000;
      *(volatile u32 *)((u32)pvVar2 + 0x26c) = 0;
      cm_fn_00521e60(pvVar2,6,0x12,1,1,6,(int)*(volatile s16 *)(self + 0x40));
      cm_fn_00521eb0(pvVar2,6,10,2,1,5,1,(int)*(volatile s16 *)(self + 0x40),0);
      *(volatile u16 *)((u32)pvVar2 + 0x277) = 0x78;
      *(volatile u16 *)((u32)pvVar2 + 0x279) = 6;
      *(volatile u16 *)((u32)pvVar2 + 0x27b) = 0;
      *(volatile u16 *)((u32)pvVar2 + 0x27d) = 0;
      *(volatile u8 *)((u32)pvVar2 + 0x287) = 6;
      *(volatile u16 *)((u32)pvVar2 + 0x288) = 8;
      *(volatile u16 *)((u32)pvVar2 + 0x28a) = 4;
      *(volatile u16 *)((u32)pvVar2 + 0x28c) = 0;
      *(volatile u16 *)((u32)pvVar2 + 0x27f) = 3;
      *(volatile u32 *)((u32)pvVar2 + 0x283) = 0;
      *(volatile u16 *)((u32)pvVar2 + 0x28e) = 0;
      *(volatile u8 *)((u32)pvVar2 + 0x290) = 1;
      *(volatile u8 *)((u32)pvVar2 + 0x291) = 1;
      *(volatile u8 *)((u32)pvVar2 + 0x292) = 10;
      *(volatile u32 *)((u32)pvVar2 + 0x2cc) = 0;
      *(volatile u32 *)((u32)pvVar2 + 0x2d0) = 150000;
      *(volatile u32 *)((u32)pvVar2 + 0x2d4) = 0;
      cm_fn_00521e60(pvVar2,7,0xb,2,1,6,(int)*(volatile s16 *)(self + 0x40));
      cm_fn_00521eb0(pvVar2,7,8,3,1,6,1,(int)*(volatile s16 *)(self + 0x40),0x2a);
      *(volatile u16 *)((u32)pvVar2 + 0x2df) = 0x82;
      *(volatile u16 *)((u32)pvVar2 + 0x2e1) = 7;
      *(volatile u16 *)((u32)pvVar2 + 0x2e3) = 0;
      *(volatile u16 *)((u32)pvVar2 + 0x2e5) = 3;
      *(volatile u8 *)((u32)pvVar2 + 0x2ef) = 6;
      *(volatile u16 *)((u32)pvVar2 + 0x2f0) = 4;
      *(volatile u16 *)((u32)pvVar2 + 0x2f2) = 2;
      *(volatile u16 *)((u32)pvVar2 + 0x2f4) = 0;
      *(volatile u16 *)((u32)pvVar2 + 0x2e7) = 0;
      *(volatile u16 *)((u32)pvVar2 + 0x2f6) = 0;
      *(volatile u8 *)((u32)pvVar2 + 0x2f8) = 0;
      *(volatile u8 *)((u32)pvVar2 + 0x2f9) = 1;
      *(volatile u8 *)((u32)pvVar2 + 0x2fa) = 10;
      *(volatile u32 *)((u32)pvVar2 + 0x334) = 0;
      *(volatile u32 *)((u32)pvVar2 + 0x338) = 400000;
      *(volatile u32 *)((u32)pvVar2 + 0x33c) = 0;
      cm_fn_00521e60(pvVar2,8,9,3,1,0,(int)*(volatile s16 *)(self + 0x40));
      cm_fn_00521eb0(pvVar2,8,0xc,4,1,5,1,(int)*(volatile s16 *)(self + 0x40),4);
      *(volatile u16 *)((u32)pvVar2 + 0x347) = 0x96;
      *(volatile u16 *)((u32)pvVar2 + 0x349) = 8;
      *(volatile u16 *)((u32)pvVar2 + 0x34b) = 0;
      *(volatile u16 *)((u32)pvVar2 + 0x34d) = 3;
      *(volatile u8 *)((u32)pvVar2 + 0x357) = 6;
      *(volatile u16 *)((u32)pvVar2 + 0x358) = 2;
      *(volatile u16 *)((u32)pvVar2 + 0x35a) = 1;
      *(volatile u16 *)((u32)pvVar2 + 0x35c) = 0;
      *(volatile u16 *)((u32)pvVar2 + 0x34f) = 0;
      *(volatile u16 *)((u32)pvVar2 + 0x35e) = 0;
      *(volatile u8 *)((u32)pvVar2 + 0x360) = 0;
      *(volatile u8 *)((u32)pvVar2 + 0x361) = 1;
      *(volatile u8 *)((u32)pvVar2 + 0x362) = 0;
      *(volatile u32 *)((u32)pvVar2 + 0x39c) = 0;
      *(volatile u32 *)((u32)pvVar2 + 0x3a0) = 2000000;
      *(volatile u32 *)((u32)pvVar2 + 0x3a4) = 1000000;
      return (void *)pvVar2;
    }
    cm_fn_00944cff((u32)s_E__dev_CM3_cm3_00_01_cm3_code_co_009c42cc,0,0,local_100,local_200);
    uVar3 = (u32)cm_fn_0090d130();
    cm_fn_00944c9f((u32)&DAT_00ae24d0,(u32)s_v_s__s__s__d_009870e8,uVar3,
                   (u32)local_100,(u32)local_200,0xb6);
    cm_fn_005e8290(s_Error_009870e0,(const char *)&DAT_00ae24d0,0);
    DAT_00b67a34 = 0;
  }
  return (void *)0x0;
}
