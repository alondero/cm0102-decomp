/* Tier-2 C for preferred-image VA 0x004ccbf0 (image base 0x00400000).
   Thiscall: ECX = this; one stack argument (param_2, a record pointer); ret 4.
   This routine returns EAX at its ret. Exits leave EAX as 0, or as the record key
   with its low byte replaced by zero (`mov al, byte ...` read a zero byte). */
#include "cmre.h"

extern u32 CM_FASTCALL cm_fn_0068d1b0(int param_1);
extern u32 CM_THISCALL cm_fn_0068d1f0(u8 *self, const u8 *target);
extern void CM_THISCALL cm_fn_008c5430(int param_1,int *param_2);

u32 CM_THISCALL cm_fn_004ccbf0(u32 param_1, u32 param_2)
{
  u32 iter;
  u32 idx;

  if (*(u32 *)(param_1 + 0x34) == 0) {
    return 0;
  }
  {
    u32 table = CM_GLOBAL(volatile u32, 0x00ae2d0c);
    idx = *(volatile u32 *)param_2;
    if (*(volatile u8 *)(table + idx * 79u + 8u) == 0) {
      return idx & 0xffffff00u;
    }
  }
  cm_fn_008c5430((int)CM_GLOBAL(u32, 0x00de1f54), (int *)param_2);
  {
    u32 obj = *(u32 *)(param_1 + 0x34);
    *(u32 *)(obj + 0xa) = *(u32 *)(obj + 2);
  }
  iter = cm_fn_0068d1b0(*(u32 *)(param_1 + 0x34));
  if (iter == 0) {
    return 0;
  }
  for (;;) {
    u32 found = *(volatile u32 *)iter;
    if (found == *(volatile u32 *)param_2) {
      cm_fn_0068d1f0((u8 *)*(u32 *)(param_1 + 0x34), (const u8 *)iter);
      idx = *(u32 *)param_2;
      {
        u32 cell = CM_GLOBAL(u32, 0x00ae2d0c) + idx * 79u + 8u;
        *(volatile u8 *)cell = (u8)(*(volatile u8 *)cell - 1);
      }
      idx = *(u32 *)param_2;
      if (*(volatile u8 *)(CM_GLOBAL(u32, 0x00ae2d0c) + idx * 79u + 8u) == 0) {
        return idx & 0xffffff00u;
      }
    }
    iter = cm_fn_0068d1b0(*(u32 *)(param_1 + 0x34));
    if (iter == 0) {
      return 0;
    }
  }
}
