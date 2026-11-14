/* cm_fn_0076b100 - recovered from original instructions, preferred-image VA 0x0076b100. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif

#define G_AE2A58     CM_GLOBAL(u32, 0x00ae2a58)
#define G_009cf78c   CM_GLOBAL(int, 0x009cf78c)
#define G_009cf790   CM_GLOBAL(int, 0x009cf790)
#define G_009cf79c   CM_GLOBAL(int, 0x009cf79c)
#define G_009cf7a0   CM_GLOBAL(int, 0x009cf7a0)
#define G_009cf7a4   CM_GLOBAL(int, 0x009cf7a4)
#define G_009cf7a8   CM_GLOBAL(int, 0x009cf7a8)
#define G_009cf7ac   CM_GLOBAL(int, 0x009cf7ac)
#define G_009cf8a0   CM_GLOBAL(int, 0x009cf8a0)
#define G_009cf964   CM_GLOBAL(int, 0x009cf964)
#define LAB_0095dd08 CM_GLOBAL(u8, 0x0095dd08)

extern void CM_THISCALL cm_fn_00417c10(u32 *param_1,u32 *param_2,u16 *param_3);
extern u32 CM_CDECL cm_fn_0053b4f0(u32 nation);
extern u32 CM_CDECL cm_fn_005409b0(u32 *input);
extern short * CM_THISCALL cm_fn_00549f70(short *param_1,short param_2,char param_3,u16 param_4,u32 param_5);
extern u32 CM_THISCALL cm_fn_0054c5b0(short *param_1,u32 *param_2,int param_3);
extern void CM_THISCALL cm_fn_0054c770(short *param_1,u32 *param_2,u8 param_3);
extern u32 CM_THISCALL cm_fn_0054c8e0(short *param_1,u32 *param_2,int param_3);
extern void CM_THISCALL cm_fn_0058d460(u32 self,volatile u32 date_arg);
extern u32 CM_THISCALL cm_fn_005abf10(u32 self,volatile u32 first,volatile u32 second,volatile u32 third,volatile s8 fourth);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1f0(u8 *self,const u8 *target);
extern void CM_FASTCALL cm_fn_0076abf0(int param_1);
extern void CM_THISCALL cm_fn_0076bab0(int param_1,int *param_2);
extern void CM_THISCALL cm_fn_007771c0(u32 receiver,volatile u32 arg1,volatile u32 arg2,volatile u32 arg3);
extern void CM_THISCALL cm_fn_007774b0(u8 *self);
extern void CM_THISCALL cm_fn_007775c0(int param_1,u32 param_2);
extern u32 CM_THISCALL cm_fn_007777e0(u32 self,u32 date);
extern u32 CM_THISCALL cm_fn_007779e0(u32 self);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

u32 CM_THISCALL cm_fn_0076b100(u32 *param_1,short *param_2)

{
  u8 scratch[0x50];
  volatile struct { void *prev; void *handler; u32 state; } frame;
  u32 res1;
  u32 res2;
  u32 node;
  u32 v;
  u32 w;
  u16 flags;
  u8 matched;
  u8 counter;
  u32 r;

  frame.state = 0xffffffffu;
  frame.handler = (void *)&LAB_0095dd08;
  frame.prev = CM_FS(void *,0);
  CM_FS(void *,0) = (void *)&frame;

  cm_fn_0054c770(param_2,(u32 *)(void *)(scratch + 0x10),2);
  frame.state = 0;
  if (param_1[9] != 0) {
    cm_fn_009452ca(param_1[9]);
    param_1[9] = 0;
  }

  w = *(u32 *)(void *)param_2;
  r = (u32)cm_fn_00549f70((short *)(void *)scratch,1,0,((u16 *)(void *)param_2)[1],0xffffffffu);
  v = *(u32 *)(void *)r;
  res2 = *(u32 *)(void *)(r + 4);
  *(u32 *)(void *)(scratch + 8) = v;
  frame.state = 1;
  *(u32 *)(void *)(scratch + 0xc) = res2;
  if (v == w) {
    matched = 1;
  }
  else {
    r = (u32)cm_fn_00549f70((short *)(void *)(scratch + 0x18),0xf,3,((u16 *)(void *)param_2)[1],0xffffffffu);
    v = *(u32 *)(void *)r;
    res2 = *(u32 *)(void *)(r + 4);
    *(u32 *)(void *)(scratch + 0xc) = res2;
    *(u32 *)(void *)(scratch + 8) = v;
    if (v == w) {
      matched = 1;
    }
    else {
      r = (u32)cm_fn_00549f70((short *)(void *)(scratch + 0x20),1,7,((u16 *)(void *)param_2)[1],0xffffffffu);
      v = *(u32 *)(void *)r;
      res2 = *(u32 *)(void *)(r + 4);
      *(u32 *)(void *)(scratch + 8) = v;
      *(u32 *)(void *)(scratch + 0xc) = res2;
      matched = (v == w);
    }
  }
  frame.state = 0;
  if (matched != 0) {
    cm_fn_0076abf0((int)(u32)param_1);
  }

  *(u8 *)(void *)scratch = 0;
  do {
    u32 third;

    third = *(u32 *)(void *)scratch;
    res1 = cm_fn_005abf10(G_AE2A58,*(u32 *)(void *)(scratch + 0x12),
                          *(u32 *)(void *)(scratch + 0x10),third,0);
    res2 = cm_fn_005abf10(G_AE2A58,*(u32 *)(void *)(scratch + 0x12),
                          *(u32 *)(void *)(scratch + 0x10),third,1);
    *(u32 *)(void *)(scratch + 8) = res2;
    if (((res1 != 0 && *(u16 *)(void *)res1 != 0) ||
         (res2 != 0 && *(u16 *)(void *)res2 != 0)) && param_1[9] == 0) {
      cm_fn_007775c0((int)(u32)param_1,(u32)(void *)(scratch + 0x10));
      r = cm_fn_0054c8e0((short *)(void *)(scratch + 0x10),(u32 *)(void *)(scratch + 0x20),1);
      frame.state = 2;
      cm_fn_00417c10((u32 *)(void *)(scratch + 0x10),(u32 *)(void *)(scratch + 0x18),(u16 *)r);
      frame.state = 0;
      cm_fn_007775c0((int)(u32)param_1,(u32)(void *)(scratch + 0x10));
      r = cm_fn_0054c5b0((short *)(void *)(scratch + 0x10),(u32 *)(void *)(scratch + 0x28),2);
      frame.state = 3;
      cm_fn_00417c10((u32 *)(void *)(scratch + 0x10),(u32 *)(void *)(scratch + 0x30),(u16 *)r);
      frame.state = 0;
      cm_fn_007775c0((int)(u32)param_1,(u32)(void *)(scratch + 0x10));
      r = cm_fn_0054c8e0((short *)(void *)(scratch + 0x10),(u32 *)(void *)(scratch + 0x38),1);
      frame.state = 4;
      cm_fn_00417c10((u32 *)(void *)(scratch + 0x10),(u32 *)(void *)(scratch + 0x40),(u16 *)r);
      frame.state = 0;
    }

    if (res1 != 0) {
      *(u32 *)(void *)(res1 + 0xa) = *(u32 *)(void *)(res1 + 2);
      node = cm_fn_0068d1b0(res1);
      while (node != 0) {
        u32 field14;
        u32 key;

        cm_fn_0076bab0((int)(u32)param_1,(int *)node);
        field14 = *(u32 *)(void *)(node + 0x14);
        if (field14 != 0) {
          key = *(u32 *)(void *)field14;
          if (key == (u32)G_009cf79c) goto next1;
          if (key == (u32)G_009cf7a0) goto next1;
          if (key == (u32)G_009cf7a4) goto next1;
          if (key == (u32)G_009cf7a8) goto next1;
          if (key == (u32)G_009cf7ac) goto next1;
          if (key == (u32)G_009cf790) goto next1;
          if (key == (u32)G_009cf8a0) goto next1;
          if (key == (u32)G_009cf78c) goto next1;
          if (key == (u32)G_009cf964) goto next1;
        }
        if (cm_fn_005409b0((u32 *)(void *)*(u32 *)(void *)(node + 0x1c)) != 0) goto next1;
        if (*(u8 *)(void *)(node + 0x40) == 0) goto next1;
        cm_fn_007771c0((u32)param_1,*(u32 *)(void *)(node + 0x1c),0,node);
        cm_fn_007771c0((u32)param_1,*(u32 *)(void *)(node + 0x20),0,node);
        flags = *(u16 *)(void *)(node + 0x4d);
        if ((flags & 0x200) == 0) {
          if ((flags & 0x100) == 0) goto next1;
          cm_fn_007771c0((u32)param_1,*(u32 *)(void *)(node + 0x1c),1,node);
          cm_fn_007771c0((u32)param_1,*(u32 *)(void *)(node + 0x20),1,node);
        }
        else {
          u32 nation;

          nation = cm_fn_0053b4f0(*(u32 *)(void *)(*(u32 *)(void *)(node + 0x1c) + 0x53));
          cm_fn_007771c0((u32)param_1,nation,0,node);
          nation = cm_fn_0053b4f0(*(u32 *)(void *)(*(u32 *)(void *)(node + 0x20) + 0x53));
          cm_fn_007771c0((u32)param_1,nation,0,node);
        }
next1:
        node = cm_fn_0068d1b0(res1);
      }
    }

    res2 = *(u32 *)(void *)(scratch + 8);
    if (res2 != 0) {
      *(u32 *)(void *)(res2 + 0xa) = *(u32 *)(void *)(res2 + 2);
      node = cm_fn_0068d1b0(res2);
      while (node != 0) {
        u32 field14;
        u32 key;

        cm_fn_0076bab0((int)(u32)param_1,(int *)node);
        field14 = *(u32 *)(void *)(node + 0x14);
        if (field14 != 0) {
          key = *(u32 *)(void *)field14;
          if (key == (u32)G_009cf79c) goto next2;
          if (key == (u32)G_009cf7a0) goto next2;
          if (key == (u32)G_009cf7a4) goto next2;
          if (key == (u32)G_009cf7a8) goto next2;
          if (key == (u32)G_009cf7ac) goto next2;
          if (key == (u32)G_009cf790) goto next2;
          if (key == (u32)G_009cf8a0) goto next2;
          if (key == (u32)G_009cf78c) goto next2;
          if (key == (u32)G_009cf964) goto next2;
        }
        if (cm_fn_005409b0((u32 *)(void *)*(u32 *)(void *)(node + 0x1c)) != 0) goto next2;
        if (*(u8 *)(void *)(node + 0x40) == 0) goto next2;
        cm_fn_007771c0((u32)param_1,*(u32 *)(void *)(node + 0x1c),0,node);
        cm_fn_007771c0((u32)param_1,*(u32 *)(void *)(node + 0x20),0,node);
        flags = *(u16 *)(void *)(node + 0x4d);
        if ((flags & 0x200) == 0) {
          if ((flags & 0x100) == 0) goto next2;
          cm_fn_007771c0((u32)param_1,*(u32 *)(void *)(node + 0x1c),1,node);
          cm_fn_007771c0((u32)param_1,*(u32 *)(void *)(node + 0x20),1,node);
        }
        else {
          u32 nation;

          nation = cm_fn_0053b4f0(*(u32 *)(void *)(*(u32 *)(void *)(node + 0x1c) + 0x53));
          cm_fn_007771c0((u32)param_1,nation,0,node);
          nation = cm_fn_0053b4f0(*(u32 *)(void *)(*(u32 *)(void *)(node + 0x20) + 0x53));
          cm_fn_007771c0((u32)param_1,nation,0,node);
        }
next2:
        node = cm_fn_0068d1b0(*(u32 *)(void *)(scratch + 8));
      }
    }

    counter = (u8)(*(u8 *)(void *)scratch + 1);
    *(u8 *)(void *)scratch = counter;
  } while (counter <= 2);

  if (param_1[9] != 0) {
    cm_fn_007777e0((u32)param_1,(u32)(void *)(scratch + 0x10));
  }
  if (param_1[0] != 0) {
    u16 si;
    u32 head;

    head = param_1[0];
    v = *(u32 *)(void *)((u8 *)head + 2);
    si = *(u16 *)(void *)param_2;
    *(u32 *)(void *)((u8 *)head + 0xa) = v;
    node = cm_fn_0068d1b0(head);
    while (node != 0) {
      if (*(u16 *)(void *)(node + 0x10) == si) {
        cm_fn_0068d1f0((u8 *)(void *)param_1[0],(const u8 *)(void *)node);
      }
      node = cm_fn_0068d1b0(param_1[0]);
    }
  }
  if (param_1[7] != 0) {
    cm_fn_0058d460(param_1[7],(u32)(void *)param_2);
  }
  if ((s32)(s16)(*(u16 *)(void *)param_2) % 0x46 == 0) {
    cm_fn_007774b0((u8 *)(void *)param_1);
  }
  r = cm_fn_007779e0((u32)param_1);
  CM_FS(void *,0) = (void *)frame.prev;
  return r;
}
