/* fn_00642790: VA 0x00642790, image base 0x00400000, 235 bytes, region end 0x00642a70.
   Entry takes no arguments and ends with `ret` (0 bytes). Its only direct caller is
   0x00641735. At exit EAX is the number of collected entries (the bl count), or 0.
   Frame: 216-byte scratch array at [esp+0x14] holds the collected object pointers;
   [esp+0x10] is the loop counter, kept here as a local. */
#include "cmre.h"

#define G_9CF580 CM_GLOBAL(volatile int, 0x009cf580)
#define G_9CF584 CM_GLOBAL(volatile int, 0x009cf584)
#define G_9CF588 CM_GLOBAL(volatile int, 0x009cf588)
#define G_AE23D0 CM_GLOBAL(volatile int, 0x00ae23d0)
#define G_AE23B4 CM_GLOBAL(volatile int, 0x00ae23b4)
#define G_AE235C CM_GLOBAL(volatile int, 0x00ae235c)

extern void cm_fn_00945333(void *param_1, u32 param_2, u32 param_3, void *param_4);

u32 cm_fn_00642790(void)
{
  u32 arr[54];
  u32 v1;
  u32 v2;
  u32 v3;
  u32 base;
  u32 k1;
  u32 k2;
  u32 k3;
  u32 p;
  u32 field;
  u32 counter;
  u32 n;
  u32 i;
  u32 eax_out;
  s8 bl;
  s16 dx;

  v1 = (u32)G_9CF580;
  v2 = (u32)G_9CF584;
  base = (u32)G_AE23D0;
  k1 = v1 * 0x6bu + base;
  k2 = v2 * 0x6bu + base;
  v3 = (u32)G_9CF588;
  k3 = v3 * 0x6bu + base;
  p = (u32)G_AE23B4;
  n = (u32)G_AE235C;

  bl = 0;
  counter = 0;
  if ((s32)n > 0) {
    for (;;) {
      if ((s8)bl >= 0x36) {
        break;
      }
      field = *(volatile u32 *)(p + 0x57);
      if (field == k1 || field == k2 || field == k3) {
        i = (u32)(s32)(s8)bl;
        bl = (s8)(bl + 1);
        arr[i] = p;
      }
      n = (u32)G_AE235C;
      counter = counter + 1;
      p = p + 0x245;
      dx = (s16)counter;
      if (!((s32)dx < (s32)n)) {
        break;
      }
    }
  }

  cm_fn_00945333((void *)arr, (u32)(s32)(s8)bl, 4, (void *)&CM_GLOBAL(u8, 0x0053fd40));

  dx = (s16)(s8)bl;
  eax_out = 0;
  if (dx > 0) {
    i = 0;
    do {
      u32 obj;
      obj = arr[i];
      if ((s16)eax_out < 0x12) {
        *(volatile u32 *)(obj + 0x57) = k3;
      } else if ((s16)eax_out < 0x24) {
        *(volatile u32 *)(obj + 0x57) = k2;
      } else {
        *(volatile u32 *)(obj + 0x57) = k1;
      }
      eax_out = eax_out + 1;
      i = i + 1;
    } while ((s16)eax_out < dx);
  }

  return eax_out;
}
