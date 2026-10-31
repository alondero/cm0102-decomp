#include "cmre.h"
#define DAT_00ae237c CM_GLOBAL(volatile s32, 0x00ae237c)
#define DAT_00ae23d4 CM_GLOBAL(volatile u32, 0x00ae23d4)
#define DAT_00ae2c92 CM_GLOBAL(volatile u16, 0x00ae2c92)
extern u32 CM_CDECL cm_fn_00527f60(s32 amount);

void CM_CDECL cm_fn_004afce0(u32 param_1,u32 param_2,u32 param_3,u32 param_4,u32 param_5)
{
  s32 count = DAT_00ae237c;
  u32 base;
  u32 destination = 0;
  s32 last;
  s32 index;
  u32 candidate;
  u32 value;

  base = DAT_00ae23d4;
  if (count > 0) {
    u32 scaled = (u32)count * 0x1a;
    destination = base + scaled - 0x1a;
  }
  last = (s32)((u32)count - 1u);
  if (last >= 0) {
    u32 probe = destination + 4;
    index = last;
    for (;;) {
      if (*(volatile u32 *)probe != 0) {
        if (index < last) {
          candidate = base + (u32)index * 0x1a + 0x1a;
          if (candidate != 0) {
            destination = candidate;
            goto store_fields;
          }
        }
        break;
      }
      --index;
      probe -= 0x1a;
      if (index < 0) break;
    }
  }
  value = cm_fn_00527f60(0x14);
  if (value == 0) return;
  count = DAT_00ae237c;
  {
    u32 offset = (u32)count * 0x1a - 0x208u;
    base = DAT_00ae23d4;
    destination = base + offset;
  }

store_fields:
  value = *(volatile u32 *)(param_1 + 4);
  *(volatile u32 *)(destination + 4) = value;
  value = DAT_00ae2c92;
  *(volatile u16 *)(destination + 8) = (u16)value;
  value = param_2;
  *(volatile u32 *)(destination + 0xa) = value;
  value = param_3;
  *(volatile u32 *)(destination + 0xe) = value;
  value = param_4;
  *(volatile u32 *)(destination + 0x12) = value;
  value = param_5;
  *(volatile u32 *)(destination + 0x16) = value;
}
