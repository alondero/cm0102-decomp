/* Preferred-image VA 0x005788c0, base 0x00400000.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Packed award table initializer. Original callees remain explicit boundaries.
   Year scratch receives only its low word; upper bytes remain uninitialized. */
#include "cmre.h"
#define DAT_009cf5bc CM_GLOBAL(volatile u32, 0x009cf5bc)
#define DAT_009cf5c0 CM_GLOBAL(volatile u32, 0x009cf5c0)
#define DAT_009cf5c4 CM_GLOBAL(volatile u32, 0x009cf5c4)
#define DAT_009cf5c8 CM_GLOBAL(volatile u32, 0x009cf5c8)
#define DAT_009cf69c CM_GLOBAL(volatile u32, 0x009cf69c)
#define DAT_009cfa34 CM_GLOBAL(volatile u32, 0x009cfa34)
#define DAT_009cfa38 CM_GLOBAL(volatile u32, 0x009cfa38)
#define DAT_009cfa3c CM_GLOBAL(volatile u32, 0x009cfa3c)
#define DAT_009cfa40 CM_GLOBAL(volatile u32, 0x009cfa40)
#define DAT_009cfa44 CM_GLOBAL(volatile u32, 0x009cfa44)
#define DAT_009cfa48 CM_GLOBAL(volatile u32, 0x009cfa48)
#define DAT_009cfa4c CM_GLOBAL(volatile u32, 0x009cfa4c)
#define DAT_009cfa50 CM_GLOBAL(volatile u32, 0x009cfa50)
#define DAT_009cfa54 CM_GLOBAL(volatile u32, 0x009cfa54)
#define DAT_009cfa58 CM_GLOBAL(volatile u32, 0x009cfa58)
#define DAT_009cfa5c CM_GLOBAL(volatile u32, 0x009cfa5c)
#define DAT_009cfa60 CM_GLOBAL(volatile u32, 0x009cfa60)
#define DAT_009cfa64 CM_GLOBAL(volatile u32, 0x009cfa64)
#define DAT_009cfa68 CM_GLOBAL(volatile u32, 0x009cfa68)
#define DAT_009cfa6c CM_GLOBAL(volatile u32, 0x009cfa6c)
#define DAT_009cfa70 CM_GLOBAL(volatile u32, 0x009cfa70)
#define DAT_009cfa74 CM_GLOBAL(volatile u32, 0x009cfa74)
#define DAT_009cfa78 CM_GLOBAL(volatile u32, 0x009cfa78)
#define DAT_009cfbd4 CM_GLOBAL(volatile u32, 0x009cfbd4)
#define DAT_009cfbd8 CM_GLOBAL(volatile u32, 0x009cfbd8)
#define DAT_009cfbdc CM_GLOBAL(volatile u32, 0x009cfbdc)
#define DAT_009cfbe0 CM_GLOBAL(volatile u32, 0x009cfbe0)
#define DAT_009cfbe4 CM_GLOBAL(volatile u32, 0x009cfbe4)
#define DAT_009cfbe8 CM_GLOBAL(volatile u32, 0x009cfbe8)
#define DAT_009cfbec CM_GLOBAL(volatile u32, 0x009cfbec)
#define DAT_009cfbf0 CM_GLOBAL(volatile u32, 0x009cfbf0)
#define DAT_009cfc84 CM_GLOBAL(volatile u32, 0x009cfc84)
#define DAT_009cfc88 CM_GLOBAL(volatile u32, 0x009cfc88)
#define DAT_009cfc8c CM_GLOBAL(volatile u32, 0x009cfc8c)
#define DAT_009cfc90 CM_GLOBAL(volatile u32, 0x009cfc90)
#define DAT_009cfc94 CM_GLOBAL(volatile u32, 0x009cfc94)
#define DAT_009cfc98 CM_GLOBAL(volatile u32, 0x009cfc98)
#define DAT_00ae24d0 CM_GLOBAL(volatile u8, 0x00ae24d0)
#define DAT_00ae2c90 CM_GLOBAL(volatile u8, 0x00ae2c90)
#define DAT_00b67a34 CM_GLOBAL(volatile u32, 0x00b67a34)
extern u32 CM_THISCALL cm_fn_00766e10(void *self, u32 arg_04, u32 arg_08, u32 arg_0c, u32 arg_10, u32 arg_14, u32 arg_18, u32 arg_1c, u32 arg_20, u32 arg_24, u32 arg_28, u32 arg_2c, u32 arg_30);
extern u32 CM_THISCALL cm_fn_008af520(void *self, u32 arg_04, u32 arg_08, u32 arg_0c, u32 arg_10, u32 arg_14, u32 arg_18, u32 arg_1c, u32 arg_20, u32 arg_24, u32 arg_28);
extern u32 * CM_THISCALL cm_fn_0092fe80(u32 *self, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, u32 arg7, u32 arg8, u32 arg9, u32 arg10, u32 arg11);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_005e8290(const char *,const char *,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);

u32 CM_THISCALL cm_fn_005788c0(u32 root,u32 arg04)

{
  void *allocation;
  u32 context;
  u32 result;
  u32 ordinal;
  struct {
    volatile union { u32 full; u16 half[2]; } year;
    u8 buffers[512];
  } scratch;
  u32 output;
  u32 text;
  u32 country;
  u32 next_year;
  u32 raw_year;
  volatile struct { u32 prev, handler, state; } frame;
  frame.state = 0xffffffffu;
  frame.handler = 0x009586d0u;
  frame.prev = CM_FS(u32, 0);
  CM_FS(u32, 0) = (u32)&frame;
  scratch.year.half[0] = CM_GLOBAL(volatile u16,0x00ae2c92);
  u32 nested = *(volatile u32 *)(root + 0xe);
  context = *(volatile u32 *)(root + 0x13);
  if ((*(volatile u8 *)(nested + 0x11c) & 4) == 0) {
    *(volatile u16 *)(root + 8) = 0x1a;
  }
  else {
    *(volatile u16 *)(root + 8) = 0x20;
  }
  allocation = (void *)(u32)(cm_fn_00944e46((u32)(s32)*(volatile s16 *)(root + 8) * 4u));
  *(volatile u32 *)(root + 4) = (u32)allocation;
  if (allocation == (void *)0x0) {
    cm_fn_00944cff((u32)(((char *)0x009c4460)),0,0,(scratch.buffers + 256),scratch.buffers);
    context = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,context,(scratch.buffers + 256),scratch.buffers,0x95);
    result = cm_fn_005e8290(((char *)0x009870e0),(const char *)(&DAT_00ae24d0),0);
    DAT_00b67a34 = 0;
  }
  else {
    allocation = (void *)(u32)(cm_fn_00944cf1(0x2b));
    frame.state = 0;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      /* Constructor row 0, original CALL VA 0x005789cb. */
      result = (u32)cm_fn_0092fe80((u32 *)allocation,context,0,DAT_009cfa34,8000,5000,0xffffffff,1,0,0,arg04,1);
    }
    ordinal = (u32)(allocation != (void *)0x0);
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)output = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x2b));
    frame.state = 1;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      /* Constructor row 1, original CALL VA 0x00578a24. */
      result = (u32)cm_fn_0092fe80((u32 *)allocation,context,ordinal,DAT_009cfa38,7000,0xdac,0xffffffff,1,0,0,arg04,1);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 4) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x25d));
    frame.state = 2;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      country = DAT_009cf5bc;
      text = DAT_009cfa6c;
      /* Constructor row 2, original CALL VA 0x00578a84. */
      result = cm_fn_008af520(allocation,context,ordinal,text,4000,500,country,1,0,arg04,1);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 8) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0xa2));
    frame.state = 3;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      next_year = scratch.year.full + 1u;
      raw_year = scratch.year.full;
      country = DAT_009cf5bc;
      text = DAT_009cfbd8;
      /* Constructor row 3, original CALL VA 0x00578aef. */
      result = cm_fn_00766e10(allocation,context,ordinal,text,5000,0x5dc,country,7,raw_year,4,
                           next_year,arg04,0);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0xc) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0xa2));
    frame.state = 4;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      next_year = scratch.year.full + 1u;
      raw_year = scratch.year.full;
      country = DAT_009cf5bc;
      text = DAT_009cfa4c;
      /* Constructor row 4, original CALL VA 0x00578b5a. */
      result = cm_fn_00766e10(allocation,context,ordinal,text,5000,0x5dc,country,7,raw_year,4,
                           next_year,arg04,1);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x10) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0xa2));
    frame.state = 5;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      next_year = scratch.year.full + 1u;
      raw_year = scratch.year.full;
      country = DAT_009cf5bc;
      text = DAT_009cfa5c;
      /* Constructor row 5, original CALL VA 0x00578bc5. */
      result = cm_fn_00766e10(allocation,context,ordinal,text,0x1194,1000,country,7,raw_year,4,
                           next_year,arg04,1);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x14) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x2b));
    frame.state = 6;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      country = DAT_009cf5bc;
      text = DAT_009cfbd4;
      /* Constructor row 6, original CALL VA 0x00578c24. */
      result = (u32)cm_fn_0092fe80((u32 *)allocation,context,ordinal,text,0x1d4c,0xdac,country,1,0,0,arg04,0);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x18) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x25d));
    frame.state = 7;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      country = DAT_009cf5bc;
      text = DAT_009cfa3c;
      /* Constructor row 7, original CALL VA 0x00578c84. */
      result = cm_fn_008af520(allocation,context,ordinal,text,0x1964,0x9c4,country,1,0,arg04,1);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x1c) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x25d));
    frame.state = 8;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      country = DAT_009cf5c0;
      text = DAT_009cfa70;
      /* Constructor row 8, original CALL VA 0x00578ce4. */
      result = cm_fn_008af520(allocation,context,ordinal,text,3000,500,country,1,0,arg04,1);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x20) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0xa2));
    frame.state = 9;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      next_year = scratch.year.full + 1u;
      raw_year = scratch.year.full;
      country = DAT_009cf5c0;
      text = DAT_009cfbe0;
      /* Constructor row 9, original CALL VA 0x00578d4f. */
      result = cm_fn_00766e10(allocation,context,ordinal,text,3000,500,country,7,raw_year,4,
                           next_year,arg04,0);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x24) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0xa2));
    frame.state = 10;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      next_year = scratch.year.full + 1u;
      raw_year = scratch.year.full;
      country = DAT_009cf5c0;
      text = DAT_009cfa50;
      /* Constructor row 10, original CALL VA 0x00578dba. */
      result = cm_fn_00766e10(allocation,context,ordinal,text,0xdac,500,country,7,raw_year,4,
                           next_year,arg04,1);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x28) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0xa2));
    frame.state = 0xb;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      next_year = scratch.year.full + 1u;
      raw_year = scratch.year.full;
      country = DAT_009cf5c0;
      text = DAT_009cfa60;
      /* Constructor row 11, original CALL VA 0x00578e25. */
      result = cm_fn_00766e10(allocation,context,ordinal,text,3000,500,country,7,raw_year,4,
                           next_year,arg04,1);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x2c) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x2b));
    frame.state = 0xc;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      country = DAT_009cf5c0;
      text = DAT_009cfbdc;
      /* Constructor row 12, original CALL VA 0x00578e84. */
      result = (u32)cm_fn_0092fe80((u32 *)allocation,context,ordinal,text,5000,500,country,1,0,0,arg04,0);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x30) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x25d));
    frame.state = 0xd;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      country = DAT_009cf5c0;
      text = DAT_009cfa40;
      /* Constructor row 13, original CALL VA 0x00578ee4. */
      result = cm_fn_008af520(allocation,context,ordinal,text,0x1194,500,country,1,0,arg04,1);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x34) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x25d));
    frame.state = 0xe;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      country = DAT_009cf5c4;
      text = DAT_009cfa74;
      /* Constructor row 14, original CALL VA 0x00578f41. */
      result = cm_fn_008af520(allocation,context,ordinal,text,2000,0,country,1,0,arg04,1);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x38) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0xa2));
    frame.state = 0xf;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      next_year = scratch.year.full + 1u;
      raw_year = scratch.year.full;
      country = DAT_009cf5c4;
      text = DAT_009cfbe8;
      /* Constructor row 15, original CALL VA 0x00578fa9. */
      result = cm_fn_00766e10(allocation,context,ordinal,text,2000,0,country,7,raw_year,4,next_year,
                           arg04,0);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x3c) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0xa2));
    frame.state = 0x10;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      next_year = scratch.year.full + 1u;
      raw_year = scratch.year.full;
      country = DAT_009cf5c4;
      text = DAT_009cfa54;
      /* Constructor row 16, original CALL VA 0x00579011. */
      result = cm_fn_00766e10(allocation,context,ordinal,text,0x9c4,0,country,7,raw_year,4,next_year
                           ,arg04,1);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x40) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0xa2));
    frame.state = 0x11;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      next_year = scratch.year.full + 1u;
      raw_year = scratch.year.full;
      country = DAT_009cf5c4;
      text = DAT_009cfa64;
      /* Constructor row 17, original CALL VA 0x00579079. */
      result = cm_fn_00766e10(allocation,context,ordinal,text,2000,0,country,7,raw_year,4,next_year,
                           arg04,1);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x44) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x2b));
    frame.state = 0x12;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      country = DAT_009cf5c4;
      text = DAT_009cfbe4;
      /* Constructor row 18, original CALL VA 0x005790d5. */
      result = (u32)cm_fn_0092fe80((u32 *)allocation,context,ordinal,text,3000,0,country,1,0,0,arg04,0);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x48) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x25d));
    frame.state = 0x13;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      country = DAT_009cf5c4;
      text = DAT_009cfa44;
      /* Constructor row 19, original CALL VA 0x00579132. */
      result = cm_fn_008af520(allocation,context,ordinal,text,3000,0,country,1,0,arg04,1);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x4c) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x25d));
    frame.state = 0x14;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      country = DAT_009cf5c8;
      text = DAT_009cfa78;
      /* Constructor row 20, original CALL VA 0x0057918f. */
      result = cm_fn_008af520(allocation,context,ordinal,text,0x5dc,0,country,1,0,arg04,1);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x50) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0xa2));
    frame.state = 0x15;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      next_year = scratch.year.full + 1u;
      raw_year = scratch.year.full;
      country = DAT_009cf5c8;
      text = DAT_009cfbf0;
      /* Constructor row 21, original CALL VA 0x005791f7. */
      result = cm_fn_00766e10(allocation,context,ordinal,text,1000,0,country,7,raw_year,4,next_year,
                           arg04,0);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x54) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0xa2));
    frame.state = 0x16;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      next_year = scratch.year.full + 1u;
      raw_year = scratch.year.full;
      country = DAT_009cf5c8;
      text = DAT_009cfa58;
      /* Constructor row 22, original CALL VA 0x0057925f. */
      result = cm_fn_00766e10(allocation,context,ordinal,text,0x6d6,0,country,7,raw_year,4,next_year
                           ,arg04,1);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x58) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0xa2));
    frame.state = 0x17;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      next_year = scratch.year.full + 1u;
      raw_year = scratch.year.full;
      country = DAT_009cf5c8;
      text = DAT_009cfa68;
      /* Constructor row 23, original CALL VA 0x005792c7. */
      result = cm_fn_00766e10(allocation,context,ordinal,text,0x5dc,0,country,7,raw_year,4,next_year
                           ,arg04,1);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x5c) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x2b));
    frame.state = 0x18;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      country = DAT_009cf5c8;
      text = DAT_009cfbec;
      /* Constructor row 24, original CALL VA 0x00579323. */
      result = (u32)cm_fn_0092fe80((u32 *)allocation,context,ordinal,text,2000,0,country,1,0,0,arg04,0);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x60) = result;
    allocation = (void *)(u32)(cm_fn_00944cf1(0x25d));
    frame.state = 0x19;
    if (allocation == (void *)0x0) {
      result = 0;
    }
    else {
      country = DAT_009cf5c8;
      text = DAT_009cfa48;
      /* Constructor row 25, original CALL VA 0x00579380. */
      result = cm_fn_008af520(allocation,context,ordinal,text,2000,0,country,1,0,arg04,1);
      ordinal = ordinal + 1;
    }
    output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 100) = result;
    if ((*(volatile u8 *)(*(volatile u32 *)(root + 0xe) + 0x11c) & 4) != 0) {
      allocation = (void *)(u32)(cm_fn_00944cf1(0x25d));
      frame.state = 0x1a;
      if (allocation == (void *)0x0) {
        result = 0;
      }
      else {
        country = DAT_009cf69c;
      text = DAT_009cfc94;
      /* Constructor row 26, original CALL VA 0x005793ed. */
      result = cm_fn_008af520(allocation,context,ordinal,text,0x5dc,0,country,1,0,arg04,1);
        ordinal = ordinal + 1;
      }
      output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x68) = result;
      allocation = (void *)(u32)(cm_fn_00944cf1(0xa2));
      frame.state = 0x1b;
      if (allocation == (void *)0x0) {
        result = 0;
      }
      else {
        next_year = scratch.year.full + 1u;
        raw_year = scratch.year.full;
        country = DAT_009cf69c;
      text = DAT_009cfc84;
      /* Constructor row 27, original CALL VA 0x00579455. */
      result = cm_fn_00766e10(allocation,context,ordinal,text,1000,0,country,7,raw_year,4,
                             next_year,arg04,0);
        ordinal = ordinal + 1;
      }
      output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x6c) = result;
      allocation = (void *)(u32)(cm_fn_00944cf1(0xa2));
      frame.state = 0x1c;
      if (allocation == (void *)0x0) {
        result = 0;
      }
      else {
        next_year = scratch.year.full + 1u;
        raw_year = scratch.year.full;
        country = DAT_009cf69c;
      text = DAT_009cfc90;
      /* Constructor row 28, original CALL VA 0x005794bd. */
      result = cm_fn_00766e10(allocation,context,ordinal,text,0x6d6,0,country,7,raw_year,4,
                             next_year,arg04,1);
        ordinal = ordinal + 1;
      }
      output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x70) = result;
      allocation = (void *)(u32)(cm_fn_00944cf1(0xa2));
      frame.state = 0x1d;
      if (allocation == (void *)0x0) {
        result = 0;
      }
      else {
        next_year = scratch.year.full + 1u;
        raw_year = scratch.year.full;
        country = DAT_009cf69c;
      text = DAT_009cfc98;
      /* Constructor row 29, original CALL VA 0x00579525. */
      result = cm_fn_00766e10(allocation,context,ordinal,text,0x5dc,0,country,7,raw_year,4,
                             next_year,arg04,1);
        ordinal = ordinal + 1;
      }
      output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x74) = result;
      allocation = (void *)(u32)(cm_fn_00944cf1(0x2b));
      frame.state = 0x1e;
      if (allocation == (void *)0x0) {
        result = 0;
      }
      else {
        country = DAT_009cf69c;
      text = DAT_009cfc8c;
      /* Constructor row 30, original CALL VA 0x00579581. */
      result = (u32)cm_fn_0092fe80((u32 *)allocation,context,ordinal,text,2000,0,country,1,0,0,arg04,0);
        ordinal = ordinal + 1;
      }
      output = *(volatile u32 *)(root + 4);
    frame.state = 0xffffffff;
    *(volatile u32 *)(output + 0x78) = result;
      allocation = (void *)(u32)(cm_fn_00944cf1(0x25d));
      frame.state = 0x1f;
      if (allocation == (void *)0x0) {
        context = 0;
      }
      else {
        country = DAT_009cf69c;
      text = DAT_009cfc88;
      /* Constructor row 31, original CALL VA 0x005795db. */
      context = cm_fn_008af520(allocation,context,ordinal,text,2000,0,country,1,0,arg04,1);
      }
      output = *(volatile u32 *)(root + 4);
      *(volatile u32 *)(output + 0x7c) = context;
      result = context;
    }
  }
  CM_FS(u32,0) = frame.prev;
  return result;
}
