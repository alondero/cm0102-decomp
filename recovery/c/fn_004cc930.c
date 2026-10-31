/* Tier-2 C for preferred-image VA 0x004cc930 (image base 0x00400000).
   Thiscall: ECX = this; one stack argument (param_2, a record pointer); ret 4.
   EAX at ret is the 0/1 result of `setg cl; mov eax, ecx`. */
#include "cmre.h"

extern u32 CM_FASTCALL cm_fn_0068d1b0(int param_1);
extern void CM_THISCALL cm_fn_004cd230(int *param_1,int param_2,int param_3);

u32 CM_THISCALL cm_fn_004cc930(u32 param_1, u32 param_2)
{
  u32 iter;
  u32 key_a;
  u32 key_b;
  u32 table;
  u32 node;
  s32 hi;
  s32 q;
  u32 rec_b;
  u32 g1;
  s16 v;

  {
    u32 obj = *(u32 *)(param_1 + 0x34);
    *(u32 *)(obj + 0xa) = *(u32 *)(obj + 2);
  }
  iter = cm_fn_0068d1b0(*(u32 *)(param_1 + 0x34));
  while (iter != 0) {
    u32 found;
    if ((found = *(volatile u32 *)iter, found == *(volatile u32 *)(param_2 + 0xc)) &&
        (found = *(volatile u32 *)(iter + 4), found == *(volatile u32 *)(param_2 + 8)) &&
        *(volatile u8 *)(iter + 0x62) == 0) {
      *(volatile u8 *)(iter + 0x62) = 1;
      key_b = *(u32 *)(param_2 + 0x20);
      key_a = *(u32 *)(param_2 + 0xc);
      cm_fn_004cd230((int *)param_1, (int)key_a, (int)key_b);
      break;
    }
    iter = cm_fn_0068d1b0(*(u32 *)(param_1 + 0x34));
  }

  /* 004cc988: result from the record's fields, read again after the calls. */
  table = CM_GLOBAL(u32, 0x00ae23bc);
  node = *(u32 *)(table + *(u32 *)(param_2 + 0xc) * 110u + 0x61u);
  {
    long long prod = (long long)(s32)*(s16 *)(node + 0xb) * (long long)0x10624dd3;
    hi = (s32)(prod >> 32);
  }
  q = (hi >> 5) + (s32)((u32)hi >> 31);
  rec_b = *(u32 *)(param_2 + 8);
  g1 = CM_GLOBAL(u32, 0x00ae23b4);
  v = *(s16 *)(g1 + rec_b * 581u + 0x80u);
  return (q > (s32)v) ? 1u : 0u;
}
