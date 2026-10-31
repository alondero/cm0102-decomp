/* Tier-2 C for preferred-image VA 0x004cca70 (image base 0x00400000).
   Thiscall: ECX = this; two stack arguments (param_2, param_3: record pointers); ret 8.
   EAX at ret: 0 on the search-miss and null exits, or the address of the
   decremented byte on the success exit (the last `mov eax` before ret). */
#include "cmre.h"

extern u32 CM_FASTCALL cm_fn_0068d1b0(int param_1);
extern u32 CM_THISCALL cm_fn_0068d1f0(u8 *self, const u8 *target);
extern void CM_THISCALL cm_fn_007a81a0(int param_1,u32 param_2,u32 param_3,u32 param_4);
extern void CM_THISCALL cm_fn_008c5430(int param_1,int *param_2);

u32 CM_THISCALL cm_fn_004cca70(u32 param_1, u32 param_2, u32 param_3)
{
  u32 key_y;
  u32 key_x;
  u32 obj;
  u32 iter;
  u32 idx;
  u32 addr;

  /* Keys are read once at entry; the listing keeps them in EBX/EBP. */
  key_y = *(u32 *)param_3;
  key_x = *(u32 *)param_2;
  obj = *(u32 *)(param_1 + 0x34);
  if (obj == 0) {
    return 0;
  }
  *(u32 *)(obj + 0xa) = *(u32 *)(obj + 2);
  iter = cm_fn_0068d1b0(*(u32 *)(param_1 + 0x34));
  if (iter == 0) {
    return 0;
  }
  while (!(*(u32 *)iter == key_x && *(u32 *)(iter + 4) == key_y)) {
    iter = cm_fn_0068d1b0(*(u32 *)(param_1 + 0x34));
    if (iter == 0) {
      return 0;
    }
  }
  if (iter == 0) {
    return 0;
  }
  {
    u32 a63 = *(u32 *)(iter + 0x63);
    if (a63 != 0xffffffffu) {
      u32 c6b = *(u32 *)(iter + 0x6b);
      u32 b67 = *(u32 *)(iter + 0x67);
      cm_fn_007a81a0((int)CM_GLOBAL(u32, 0x00b7434c), a63, b67, c6b);
    }
  }
  if (*(volatile u8 *)(iter + 0x50) == 2) {
    cm_fn_008c5430((int)CM_GLOBAL(u32, 0x00de1f54), (int *)param_2);
  }
  cm_fn_0068d1f0((u8 *)*(u32 *)(param_1 + 0x34), (const u8 *)iter);
  idx = *(u32 *)param_2;
  addr = CM_GLOBAL(u32, 0x00ae2d0c) + idx * 79u + 8u;
  *(volatile u8 *)addr = (u8)(*(volatile u8 *)addr - 1);
  return addr;
}
