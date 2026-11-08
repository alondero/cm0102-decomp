/* Preferred-image VA0079eb40, base00400000; target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original constructors and allocation helpers remain explicit boundaries. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define local_210 (*(volatile u32 *)(scratch + 4))
#define local_20c (scratch + 8)
#define local_10c (scratch + 0x108)
#define ExceptionList CM_FS(void *, 0)
#define DAT_009cf80c CM_GLOBAL(volatile u32, 0x009cf80c)
#define DAT_009cf810 CM_GLOBAL(volatile u32, 0x009cf810)
#define DAT_009cfdf8 CM_GLOBAL(volatile u32, 0x009cfdf8)
#define DAT_009cfdfc CM_GLOBAL(volatile u32, 0x009cfdfc)
#define DAT_009cfe00 CM_GLOBAL(volatile u32, 0x009cfe00)
#define DAT_009cfe04 CM_GLOBAL(volatile u32, 0x009cfe04)
#define DAT_009cfe08 CM_GLOBAL(volatile u32, 0x009cfe08)
#define DAT_009cfe0c CM_GLOBAL(volatile u32, 0x009cfe0c)
#define DAT_009cfe10 CM_GLOBAL(volatile u32, 0x009cfe10)
#define DAT_009cfe14 CM_GLOBAL(volatile u32, 0x009cfe14)
#define DAT_009cfe18 CM_GLOBAL(volatile u32, 0x009cfe18)
#define DAT_009cfe1c CM_GLOBAL(volatile u32, 0x009cfe1c)
#define DAT_009cfe20 CM_GLOBAL(volatile u32, 0x009cfe20)
#define DAT_009cfe24 CM_GLOBAL(volatile u32, 0x009cfe24)
#define DAT_009cfe28 CM_GLOBAL(volatile u32, 0x009cfe28)
#define DAT_009cfe2c CM_GLOBAL(volatile u32, 0x009cfe2c)
#define DAT_009cfefc CM_GLOBAL(volatile u32, 0x009cfefc)
#define DAT_009cff34 CM_GLOBAL(volatile u32, 0x009cff34)
#define DAT_009cff38 CM_GLOBAL(volatile u32, 0x009cff38)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00ae2c90 CM_GLOBAL(u8, 0x00ae2c90)
#define DAT_00b67a34 CM_GLOBAL(volatile u32, 0x00b67a34)
#define LAB_0095ef8e CM_GLOBAL(u8, 0x0095ef8e)
#define s_E__dev_CM3_cm3_00_01_cm3_code_aw_00a6a324 ((char *)0x00a6a324)
#define s_Error_009870e0 ((char *)0x009870e0)
#define s_v_s__s__s__d_009870e8 ((char *)0x009870e8)
extern void cm_fn_005e8290(const char * self,const char * context,int param_3);
extern u32 CM_THISCALL cm_fn_00766e10(void *self, u32 arg_04, u32 arg_08, u32 arg_0c, u32 arg_10, u32 arg_14, u32 arg_18, u32 arg_1c, u32 arg_20, u32 arg_24, u32 arg_28, u32 arg_2c, u32 arg_30);
extern u32 CM_THISCALL cm_fn_008af520(void *self, u32 arg_04, u32 arg_08, u32 arg_0c, u32 arg_10, u32 arg_14, u32 arg_18, u32 arg_1c, u32 arg_20, u32 arg_24, u32 arg_28);
extern char * cm_fn_0090d130(void);
extern u32 CM_THISCALL cm_fn_0092fe80(u32 *self, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, u32 arg7, u32 arg8, u32 arg9, u32 arg10, u32 arg11);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);

void CM_THISCALL cm_fn_0079eb40(u32 self,u32 context)

{
  void *allocation;
  u32 owner;
  u32 result;
  u32 destination;
  u32 next_id;
  u8 scratch[0x208];
  volatile struct { void *prev; void *handler; u32 state; } seh;
  u16 year;
  
  seh.state = 0xffffffff;
  seh.handler = &LAB_0095ef8e;
  seh.prev = ExceptionList;
  ExceptionList = (void *)&seh;
  year = CM_GLOBAL(volatile u16, 0x00ae2c92);
  owner = *(volatile u32 *)(self + 0x13);
  /* Only the original low word is initialized; upper bytes stay unknown. */
  *(volatile u16 *)(scratch + 4) = year;
  *(volatile u16 *)(self + 8) = 0x11;
  allocation = (void *)(u32)(cm_fn_00944e46(0x44));
  *(void *volatile *)(self + 4) = allocation;
  if (allocation == (void *)0x0) {
    cm_fn_00944cff((u32)(s_E__dev_CM3_cm3_00_01_cm3_code_aw_00a6a324),0,0,local_10c,local_20c);
    owner = (u32)(cm_fn_0090d130());
    cm_fn_00944c9f((u32)(&DAT_00ae24d0),(u32)(s_v_s__s__s__d_009870e8),owner,local_10c,local_20c,0x7e);
    cm_fn_005e8290(s_Error_009870e0,(const char *)(&DAT_00ae24d0),0);
    DAT_00b67a34 = 0;
  }
  else {
    allocation = (void *)(u32)(cm_fn_00944cf1(0x2b));
    *(volatile u32 *)(scratch + 0) = (u32)allocation;
    seh.state = 0;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      result = cm_fn_0092fe80((u32 *)allocation,owner,0,DAT_009cfdf8,3000,0x5dc,0xffffffff,1,0,0,context,1);
    }
    next_id = (u32)(allocation != (void *)0x0);
    destination = *(volatile u32 *)(self + 4);
    seh.state = 0xffffffff;
    *(volatile u32 *)(destination) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x2b));
    *(volatile u32 *)(scratch + 0) = (u32)allocation;
    seh.state = 1;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      result = cm_fn_0092fe80((u32 *)allocation,owner,next_id,DAT_009cfdfc,0x5dc,0x2ee,0xffffffff,1,0,0,context,1);
      next_id = next_id + 1;
    }
    destination = *(volatile u32 *)(self + 4);
    seh.state = 0xffffffff;
    *(volatile u32 *)(destination + 4) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x2b));
    *(volatile u32 *)(scratch + 0) = (u32)allocation;
    seh.state = 2;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      result = cm_fn_0092fe80((u32 *)allocation,owner,next_id,DAT_009cfefc,0,0,0xffffffff,1,0,0,context,1);
      next_id = next_id + 1;
    }
    destination = *(volatile u32 *)(self + 4);
    seh.state = 0xffffffff;
    *(volatile u32 *)(destination + 8) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x25d));
    *(volatile u32 *)(scratch + 0) = (u32)allocation;
    seh.state = 3;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      result = cm_fn_008af520(allocation,owner,next_id,DAT_009cfe00,1000,500,DAT_009cf80c,1,0,context,1);
      next_id = next_id + 1;
    }
    destination = *(volatile u32 *)(self + 4);
    seh.state = 0xffffffff;
    *(volatile u32 *)(destination + 0xc) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x2b));
    *(volatile u32 *)(scratch + 0) = (u32)allocation;
    seh.state = 4;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      result = cm_fn_0092fe80((u32 *)allocation,owner,next_id,DAT_009cfe04,0x5dc,0x2ee,DAT_009cf80c,1,0,0,context,1);
      next_id = next_id + 1;
    }
    destination = *(volatile u32 *)(self + 4);
    seh.state = 0xffffffff;
    *(volatile u32 *)(destination + 0x10) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x2b));
    *(volatile u32 *)(scratch + 0) = (u32)allocation;
    seh.state = 5;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      result = cm_fn_0092fe80((u32 *)allocation,owner,next_id,DAT_009cfe08,500,0xfa,DAT_009cf80c,1,0,0,context,1);
      next_id = next_id + 1;
    }
    destination = *(volatile u32 *)(self + 4);
    seh.state = 0xffffffff;
    *(volatile u32 *)(destination + 0x14) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x2b));
    *(volatile u32 *)(scratch + 0) = (u32)allocation;
    seh.state = 6;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      result = cm_fn_0092fe80((u32 *)allocation,owner,next_id,DAT_009cff34,500,0xfa,DAT_009cf80c,1,0,1,context,1);
      next_id = next_id + 1;
    }
    destination = *(volatile u32 *)(self + 4);
    seh.state = 0xffffffff;
    *(volatile u32 *)(destination + 0x18) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x25d));
    *(volatile u32 *)(scratch + 0) = (u32)allocation;
    seh.state = 7;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      result = cm_fn_008af520(allocation,owner,next_id,DAT_009cfe0c,0x5dc,0x2ee,DAT_009cf80c,1,0,context,1);
      next_id = next_id + 1;
    }
    destination = *(volatile u32 *)(self + 4);
    seh.state = 0xffffffff;
    *(volatile u32 *)(destination + 0x1c) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x2b));
    *(volatile u32 *)(scratch + 0) = (u32)allocation;
    seh.state = 8;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      result = cm_fn_0092fe80((u32 *)allocation,owner,next_id,DAT_009cfe20,0x9c4,0x4b0,DAT_009cf80c,1,0,0,context,0);
      next_id = next_id + 1;
    }
    destination = *(volatile u32 *)(self + 4);
    seh.state = 0xffffffff;
    *(volatile u32 *)(destination + 0x20) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0xa2));
    *(volatile u32 *)(scratch + 0) = (u32)allocation;
    seh.state = 9;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      result = cm_fn_00766e10(allocation,owner,next_id,DAT_009cfe24,2000,1000,DAT_009cf80c,8,local_210,3,
                           local_210 + 1,context,0);
      next_id = next_id + 1;
    }
    destination = *(volatile u32 *)(self + 4);
    seh.state = 0xffffffff;
    *(volatile u32 *)(destination + 0x24) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x25d));
    *(volatile u32 *)(scratch + 0) = (u32)allocation;
    seh.state = 10;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      result = cm_fn_008af520(allocation,owner,next_id,DAT_009cfe10,500,0xfa,DAT_009cf810,1,0,context,1);
      next_id = next_id + 1;
    }
    destination = *(volatile u32 *)(self + 4);
    seh.state = 0xffffffff;
    *(volatile u32 *)(destination + 0x28) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x2b));
    *(volatile u32 *)(scratch + 0) = (u32)allocation;
    seh.state = 0xb;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      result = cm_fn_0092fe80((u32 *)allocation,owner,next_id,DAT_009cfe14,0x2ee,400,DAT_009cf810,1,0,0,context,1);
      next_id = next_id + 1;
    }
    destination = *(volatile u32 *)(self + 4);
    seh.state = 0xffffffff;
    *(volatile u32 *)(destination + 0x2c) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x2b));
    *(volatile u32 *)(scratch + 0) = (u32)allocation;
    seh.state = 0xc;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      result = cm_fn_0092fe80((u32 *)allocation,owner,next_id,DAT_009cfe18,300,0x96,DAT_009cf810,1,0,0,context,1);
      next_id = next_id + 1;
    }
    destination = *(volatile u32 *)(self + 4);
    seh.state = 0xffffffff;
    *(volatile u32 *)(destination + 0x30) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x2b));
    *(volatile u32 *)(scratch + 0) = (u32)allocation;
    seh.state = 0xd;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      result = cm_fn_0092fe80((u32 *)allocation,owner,next_id,DAT_009cff38,500,0xfa,DAT_009cf810,1,0,1,context,1);
      next_id = next_id + 1;
    }
    destination = *(volatile u32 *)(self + 4);
    seh.state = 0xffffffff;
    *(volatile u32 *)(destination + 0x34) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x25d));
    *(volatile u32 *)(scratch + 0) = (u32)allocation;
    seh.state = 0xe;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      result = cm_fn_008af520(allocation,owner,next_id,DAT_009cfe1c,500,0xfa,DAT_009cf810,1,0,context,1);
      next_id = next_id + 1;
    }
    destination = *(volatile u32 *)(self + 4);
    seh.state = 0xffffffff;
    *(volatile u32 *)(destination + 0x38) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x2b));
    *(volatile u32 *)(scratch + 0) = (u32)allocation;
    seh.state = 0xf;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      result = cm_fn_0092fe80((u32 *)allocation,owner,next_id,DAT_009cfe28,1000,500,DAT_009cf810,1,0,0,context,0);
      next_id = next_id + 1;
    }
    destination = *(volatile u32 *)(self + 4);
    seh.state = 0xffffffff;
    *(volatile u32 *)(destination + 0x3c) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0xa2));
    *(volatile u32 *)(scratch + 0) = (u32)allocation;
    seh.state = 0x10;
    if (allocation == (void *)0x0) {
      owner = 0;
    }
    else {
      owner = cm_fn_00766e10(allocation,owner,next_id,DAT_009cfe2c,1000,500,DAT_009cf810,8,local_210,3,
                           local_210 + 1,context,0);
    }
    *(volatile u32 *)(*(volatile u32 *)(self + 4) + 0x40) = owner;
  }
  ExceptionList = seh.prev;
  return;
}
