/* fn_00419710: thiscall (ECX = object, read before written) with one stack argument (ret 4).
   Preferred-image VA 0x00419710, image base 0x00400000.
   Record size used by the table walk is 0x6e (110) bytes at global 0x00ae23bc. */
#include "cmre.h"

extern void CM_STDCALL cm_fn_00419ad0(volatile u8 *object, volatile u8 *other, s8 arg3, s16 arg4);
extern int CM_THISCALL cm_fn_004cf5a0(int *param_1, int *param_2);
extern u32 CM_THISCALL cm_fn_0068d1f0(u8 *self, const u8 *target);

void CM_THISCALL cm_fn_00419710(u8 *param_1, int *param_2)
{
  u32 idx;
  u32 base;
  u32 rec;
  u32 eax;
  u32 value;
  u8 bVar1;

  if (param_2 == 0) {
    return;
  }
  idx = *(u32 *)param_2;
  base = *(u32 *)(param_1 + 0xc);
  if (*(u8 *)(base + idx) == 0) {
    return;
  }

  rec = CM_GLOBAL(u32, 0x00ae23bc) + idx * 0x6eu;
  eax = (u32)cm_fn_004cf5a0((int *)CM_GLOBAL(u32, 0x00ae1050), (int *)rec);
  CM_GLOBAL(u32, 0x00ae1080) = eax;
  if (eax != 0u) {
    if (CM_GLOBAL(u32, 0x00ae23b4) + *(u32 *)(eax + 4u) * 0x245u != 0u) {
      idx = *(u32 *)param_2;
      rec = CM_GLOBAL(u32, 0x00ae23bc) + idx * 0x6eu;
      value = *(u32 *)(CM_GLOBAL(u32, 0x00ae23bc) + 0x39u + idx * 0x6eu);
      /* ECX (this) is also set to param_1 before this call; the stdcall
         record does not take it. */
      cm_fn_00419ad0((volatile u8 *)rec, (volatile u8 *)value, 0, -1);
    }
  }

  base = *(u32 *)(param_1 + 0xc);
  idx = *(u32 *)param_2;
  bVar1 = *(u8 *)(base + idx);
  /* 'or dl, 1' is never zero, so the store always runs. */
  *(u8 *)(base + idx) = (u8)(bVar1 ^ 1);

  cm_fn_0068d1f0(*(u8 **)param_1, (const u8 *)param_2);
}
