/* WIP: recurring memory page mismatch at 0x7ffdf000, cases 2-4. */
/* Draft for preferred-image VA 0x005896d0, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
extern u32 CM_THISCALL cm_fn_0077bbe0(void *self,u32 arg1,u32 arg2);
extern u32 CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern u32 CM_THISCALL cm_fn_007861e0(void *self,u32 arg1,u32 arg2);

void cm_fn_005896d0(u32 *param_1,u32 *param_2,u32 *param_3,u32 param_4)

{

  u32 *puVar1;
  u8 local_ec [224];
  volatile struct { u32 prev, handler, state; } frame;
  frame.prev=CM_FS(u32,0); frame.handler=0x958c3b; frame.state=0xffffffff;
  int valid = param_1 != 0;
  if (valid) valid = *(volatile u32 *)((u8 *)param_1 + 0x61) != 0;
  if (valid) valid = *(volatile u32 *)((u8 *)param_1 + 0x1a) != 0;
  if (valid) valid = param_2 != 0;
  if (valid) valid = *(volatile u32 *)((u8 *)param_2 + 0x53) != 0;
  u32 nested = 0;
  if (valid) nested = *(volatile u32 *)((u8 *)param_2 + 0x69);
  if (valid) valid = nested != 0;
  if (valid) valid = *(volatile u32 *)((u8 *)nested + 0x38) != 0;
  if (valid) valid = *(volatile u32 *)((u8 *)param_2 + 0xcf) != 0;
  if (valid) valid = param_3 != 0;
  if (valid) {

    CM_FS(u32,0)=(u32)&frame;
    cm_fn_0077bbe0(local_ec,0xbe1,0);
    frame.state = 0;
    cm_fn_00785790(local_ec,0,*param_1);
    cm_fn_00785790(local_ec,1,*(u32 *)(param_1[1] + 0x33));
    cm_fn_00785790(local_ec,2,*(u32 *)(param_1[2] + 0x33));
    cm_fn_00785790(local_ec,3,*(u32 *)(param_1[3] + 0x33));
    cm_fn_00785790(local_ec,4,*param_2);
    cm_fn_00785790(local_ec,5,(0x1c52 < *(short *)(*(int *)((int)param_1 + 0x61) + 0xb)) + '\x01');
    puVar1 = *(u32 **)((int)param_2 + 0xcf);
    cm_fn_00785790(local_ec,6,*puVar1);
    cm_fn_00785790(local_ec,7,*(u32 *)(puVar1[1] + 0x33));
    cm_fn_00785790(local_ec,8,*(u32 *)(puVar1[2] + 0x33));
    cm_fn_00785790(local_ec,9,*(u32 *)(puVar1[3] + 0x33));
    cm_fn_00785790(local_ec,10,*param_3);
    cm_fn_00785790(local_ec,0xb,param_4);
    cm_fn_007861e0((void*)CM_GLOBAL(u32 volatile,0xb7431c),(u32)local_ec,(u32)param_2);
  }
  CM_FS(u32,0)=frame.prev;
  return;
}






