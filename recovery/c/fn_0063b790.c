/* Preferred-image VA 0063b790, base 00400000; target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Thiscall, RET4. Sole indexed direct caller overwrites EAX with saved self.
   Original allocator and constructor bodies remain explicit boundaries. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define ExceptionList CM_FS(void *, 0)
#define DAT_009cf80c CM_GLOBAL(u32, 0x009cf80c)
#define DAT_009cf810 CM_GLOBAL(u32, 0x009cf810)
#define DAT_009cfdf8 CM_GLOBAL(u32, 0x009cfdf8)
#define DAT_009cfdfc CM_GLOBAL(u32, 0x009cfdfc)
#define DAT_009cfe00 CM_GLOBAL(u32, 0x009cfe00)
#define DAT_009cfe04 CM_GLOBAL(u32, 0x009cfe04)
#define DAT_009cfe08 CM_GLOBAL(u32, 0x009cfe08)
#define DAT_009cfe0c CM_GLOBAL(u32, 0x009cfe0c)
#define DAT_009cfe10 CM_GLOBAL(u32, 0x009cfe10)
#define DAT_009cfe14 CM_GLOBAL(u32, 0x009cfe14)
#define DAT_009cfe18 CM_GLOBAL(u32, 0x009cfe18)
#define DAT_009cfe1c CM_GLOBAL(u32, 0x009cfe1c)
#define DAT_009cfe20 CM_GLOBAL(u32, 0x009cfe20)
#define DAT_009cfe24 CM_GLOBAL(u32, 0x009cfe24)
#define DAT_009cfe28 CM_GLOBAL(u32, 0x009cfe28)
#define DAT_009cfe2c CM_GLOBAL(u32, 0x009cfe2c)
#define DAT_009cfefc CM_GLOBAL(u32, 0x009cfefc)
#define DAT_009cff34 CM_GLOBAL(u32, 0x009cff34)
#define DAT_009cff38 CM_GLOBAL(u32, 0x009cff38)
#define DAT_009cff70 CM_GLOBAL(u32, 0x009cff70)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00ae2c90 CM_GLOBAL(u8, 0x00ae2c90)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define LAB_0095b78c CM_GLOBAL(u8, 0x0095b78c)
#define s_E__dev_CM3_cm3_00_01_cm3_code_aw_009ea0f4 ((char *)0x009ea0f4)
#define s_Error_009870e0 ((char *)0x009870e0)
#define s_v_s__s__s__d_009870e8 ((char *)0x009870e8)
extern void cm_fn_005e8290(const char * param_1,const char * param_2,int param_3);
extern u32 CM_THISCALL cm_fn_00766e10(void *self, u32 arg_04, u32 arg_08, u32 arg_0c, u32 arg_10, u32 arg_14, u32 arg_18, u32 arg_1c, u32 arg_20, u32 arg_24, u32 arg_28, u32 arg_2c, u32 arg_30);
extern u32 CM_THISCALL cm_fn_008af520(void *self, u32 arg_04, u32 arg_08, u32 arg_0c, u32 arg_10, u32 arg_14, u32 arg_18, u32 arg_1c, u32 arg_20, u32 arg_24, u32 arg_28);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 * CM_THISCALL cm_fn_0092fe80(u32 *self, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, u32 arg7, u32 arg8, u32 arg9, u32 arg10, u32 arg11);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);

void CM_THISCALL cm_fn_0063b790(u32 param_1,u32 param_2)

{
  void *pvVar1;
  u32 uVar2;
  u32 uVar3;
  u32 uVar4;
  u32 award_table;
  u32 final_year;
  /* Original L+0 initializes a WORD only. Remaining bytes stay unknown.
     Error buffers retain adjacency at L+4 and L+0x104. */
  volatile struct { union { u32 year; u16 low; }; u8 first[256]; u8 second[256]; } scratch;
  volatile struct { void *prev; void *handler; u32 state; } frame;
  u16 year_word;
  frame.state = 0xffffffffu;
  frame.handler = (void *)0x0095b78c;
  frame.prev = ExceptionList;
  ExceptionList = (void *)&frame;
  year_word = CM_GLOBAL(volatile u16, 0x00ae2c92);
  uVar2 = *(volatile u32 *)(param_1 + 0x13);
  scratch.low = year_word;
  *(volatile u16 *)(param_1 + 8) = 0x12;
  pvVar1 = (void *)(u32)(cm_fn_00944e46(0x48));
  *(void *volatile *)(param_1 + 4) = pvVar1;
  if (pvVar1 == (void *)0x0) {
    cm_fn_00944cff((u32)(s_E__dev_CM3_cm3_00_01_cm3_code_aw_009ea0f4),0,0,((void *)scratch.second),((void *)scratch.first));
    uVar2 = (u32)(cm_fn_0090d130());
    cm_fn_00944c9f((u32)(&DAT_00ae24d0),(u32)(s_v_s__s__s__d_009870e8),uVar2, (void *)scratch.second, (void *)scratch.first, 0x7f);
    cm_fn_005e8290(s_Error_009870e0,(const char *)(&DAT_00ae24d0),0);
    DAT_00b67a34 = 0;
  }
  else {
    pvVar1 = (void *)(u32)(cm_fn_00944cf1(0x2b));
    frame.state = 0;
    if (pvVar1 == (void *)0x0) {
      uVar3 = 0;
    }
    else {
      uVar3 = (u32)cm_fn_0092fe80((u32 *)pvVar1,uVar2,0,DAT_009cfdf8,3000,0x5dc,0xffffffff,1,0,0,param_2,1);
    }
    uVar4 = (u32)(pvVar1 != (void *)0x0);
    award_table = *(volatile u32 *)(param_1 + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(award_table) = uVar3;
    pvVar1 = (void *)(u32)(cm_fn_00944cf1(0x2b));
    frame.state = 1;
    if (pvVar1 == (void *)0x0) {
      uVar3 = 0;
    }
    else {
      uVar3 = (u32)cm_fn_0092fe80((u32 *)pvVar1,uVar2,uVar4,DAT_009cfdfc,0x5dc,0x2ee,0xffffffff,1,0,0,param_2,1);
      uVar4 = uVar4 + 1;
    }
    award_table = *(volatile u32 *)(param_1 + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(award_table + 4) = uVar3;
    pvVar1 = (void *)(u32)(cm_fn_00944cf1(0x2b));
    frame.state = 2;
    if (pvVar1 == (void *)0x0) {
      uVar3 = 0;
    }
    else {
      uVar3 = (u32)cm_fn_0092fe80((u32 *)pvVar1,uVar2,uVar4,DAT_009cfefc,0,0,0xffffffff,1,0,0,param_2,1);
      uVar4 = uVar4 + 1;
    }
    award_table = *(volatile u32 *)(param_1 + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(award_table + 8) = uVar3;
    pvVar1 = (void *)(u32)(cm_fn_00944cf1(0xa2));
    frame.state = 3;
    if (pvVar1 == (void *)0x0) {
      uVar3 = 0;
    }
    else {
      uVar3 = cm_fn_00766e10(pvVar1,uVar2,uVar4,DAT_009cff70,3000,0x5dc,0xffffffff,8,scratch.year,3,
                           scratch.year + 1,param_2,1);
      uVar4 = uVar4 + 1;
    }
    award_table = *(volatile u32 *)(param_1 + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(award_table + 0xc) = uVar3;
    pvVar1 = (void *)(u32)(cm_fn_00944cf1(0x25d));
    frame.state = 4;
    if (pvVar1 == (void *)0x0) {
      uVar3 = 0;
    }
    else {
      uVar3 = cm_fn_008af520(pvVar1,uVar2,uVar4,DAT_009cfe00,1000,500,DAT_009cf80c,1,0,param_2,1);
      uVar4 = uVar4 + 1;
    }
    award_table = *(volatile u32 *)(param_1 + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(award_table + 0x10) = uVar3;
    pvVar1 = (void *)(u32)(cm_fn_00944cf1(0x2b));
    frame.state = 5;
    if (pvVar1 == (void *)0x0) {
      uVar3 = 0;
    }
    else {
      uVar3 = (u32)cm_fn_0092fe80((u32 *)pvVar1,uVar2,uVar4,DAT_009cfe04,0x5dc,0x2ee,DAT_009cf80c,1,0,0,param_2,1);
      uVar4 = uVar4 + 1;
    }
    award_table = *(volatile u32 *)(param_1 + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(award_table + 0x14) = uVar3;
    pvVar1 = (void *)(u32)(cm_fn_00944cf1(0x2b));
    frame.state = 6;
    if (pvVar1 == (void *)0x0) {
      uVar3 = 0;
    }
    else {
      uVar3 = (u32)cm_fn_0092fe80((u32 *)pvVar1,uVar2,uVar4,DAT_009cfe08,500,0xfa,DAT_009cf80c,1,0,0,param_2,1);
      uVar4 = uVar4 + 1;
    }
    award_table = *(volatile u32 *)(param_1 + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(award_table + 0x18) = uVar3;
    pvVar1 = (void *)(u32)(cm_fn_00944cf1(0x2b));
    frame.state = 7;
    if (pvVar1 == (void *)0x0) {
      uVar3 = 0;
    }
    else {
      uVar3 = (u32)cm_fn_0092fe80((u32 *)pvVar1,uVar2,uVar4,DAT_009cff34,500,0xfa,DAT_009cf80c,1,0,1,param_2,1);
      uVar4 = uVar4 + 1;
    }
    award_table = *(volatile u32 *)(param_1 + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(award_table + 0x1c) = uVar3;
    pvVar1 = (void *)(u32)(cm_fn_00944cf1(0x25d));
    frame.state = 8;
    if (pvVar1 == (void *)0x0) {
      uVar3 = 0;
    }
    else {
      uVar3 = cm_fn_008af520(pvVar1,uVar2,uVar4,DAT_009cfe0c,0x5dc,0x2ee,DAT_009cf80c,1,0,param_2,1);
      uVar4 = uVar4 + 1;
    }
    award_table = *(volatile u32 *)(param_1 + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(award_table + 0x20) = uVar3;
    pvVar1 = (void *)(u32)(cm_fn_00944cf1(0x2b));
    frame.state = 9;
    if (pvVar1 == (void *)0x0) {
      uVar3 = 0;
    }
    else {
      uVar3 = (u32)cm_fn_0092fe80((u32 *)pvVar1,uVar2,uVar4,DAT_009cfe20,0x9c4,0x4b0,DAT_009cf80c,1,0,0,param_2,0);
      uVar4 = uVar4 + 1;
    }
    award_table = *(volatile u32 *)(param_1 + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(award_table + 0x24) = uVar3;
    pvVar1 = (void *)(u32)(cm_fn_00944cf1(0xa2));
    frame.state = 10;
    if (pvVar1 == (void *)0x0) {
      uVar3 = 0;
    }
    else {
      uVar3 = cm_fn_00766e10(pvVar1,uVar2,uVar4,DAT_009cfe24,2000,1000,DAT_009cf80c,8,scratch.year,3,
                           scratch.year + 1,param_2,0);
      uVar4 = uVar4 + 1;
    }
    award_table = *(volatile u32 *)(param_1 + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(award_table + 0x28) = uVar3;
    pvVar1 = (void *)(u32)(cm_fn_00944cf1(0x25d));
    frame.state = 0xb;
    if (pvVar1 == (void *)0x0) {
      uVar3 = 0;
    }
    else {
      uVar3 = cm_fn_008af520(pvVar1,uVar2,uVar4,DAT_009cfe10,500,0xfa,DAT_009cf810,1,0,param_2,1);
      uVar4 = uVar4 + 1;
    }
    award_table = *(volatile u32 *)(param_1 + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(award_table + 0x2c) = uVar3;
    pvVar1 = (void *)(u32)(cm_fn_00944cf1(0x2b));
    frame.state = 0xc;
    if (pvVar1 == (void *)0x0) {
      uVar3 = 0;
    }
    else {
      uVar3 = (u32)cm_fn_0092fe80((u32 *)pvVar1,uVar2,uVar4,DAT_009cfe14,0x2ee,400,DAT_009cf810,1,0,0,param_2,1);
      uVar4 = uVar4 + 1;
    }
    award_table = *(volatile u32 *)(param_1 + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(award_table + 0x30) = uVar3;
    pvVar1 = (void *)(u32)(cm_fn_00944cf1(0x2b));
    frame.state = 0xd;
    if (pvVar1 == (void *)0x0) {
      uVar3 = 0;
    }
    else {
      uVar3 = (u32)cm_fn_0092fe80((u32 *)pvVar1,uVar2,uVar4,DAT_009cfe18,300,0x96,DAT_009cf810,1,0,0,param_2,1);
      uVar4 = uVar4 + 1;
    }
    award_table = *(volatile u32 *)(param_1 + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(award_table + 0x34) = uVar3;
    pvVar1 = (void *)(u32)(cm_fn_00944cf1(0x2b));
    frame.state = 0xe;
    if (pvVar1 == (void *)0x0) {
      uVar3 = 0;
    }
    else {
      uVar3 = (u32)cm_fn_0092fe80((u32 *)pvVar1,uVar2,uVar4,DAT_009cff38,500,0xfa,DAT_009cf810,1,0,1,param_2,1);
      uVar4 = uVar4 + 1;
    }
    award_table = *(volatile u32 *)(param_1 + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(award_table + 0x38) = uVar3;
    pvVar1 = (void *)(u32)(cm_fn_00944cf1(0x25d));
    frame.state = 0xf;
    if (pvVar1 == (void *)0x0) {
      uVar3 = 0;
    }
    else {
      uVar3 = cm_fn_008af520(pvVar1,uVar2,uVar4,DAT_009cfe1c,500,0xfa,DAT_009cf810,1,0,param_2,1);
      uVar4 = uVar4 + 1;
    }
    award_table = *(volatile u32 *)(param_1 + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(award_table + 0x3c) = uVar3;
    pvVar1 = (void *)(u32)(cm_fn_00944cf1(0x2b));
    frame.state = 0x10;
    if (pvVar1 == (void *)0x0) {
      uVar3 = 0;
    }
    else {
      uVar3 = (u32)cm_fn_0092fe80((u32 *)pvVar1,uVar2,uVar4,DAT_009cfe28,1000,500,DAT_009cf810,1,0,0,param_2,0);
      uVar4 = uVar4 + 1;
    }
    award_table = *(volatile u32 *)(param_1 + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(award_table + 0x40) = uVar3;
    pvVar1 = (void *)(u32)(cm_fn_00944cf1(0xa2));
    frame.state = 0x11;
    if (pvVar1 == (void *)0x0) {
      uVar2 = 0;
    }
    else {
      final_year = scratch.year;
      uVar2 = cm_fn_00766e10(pvVar1,uVar2,uVar4,DAT_009cfe2c,1000,500,DAT_009cf810,8,final_year,3,
                           final_year + 1,param_2,0);
    }
    *(volatile u32 *)(*(volatile u32 *)(param_1 + 4) + 0x44) = uVar2;
  }
  ExceptionList = frame.prev;
  return;
}
