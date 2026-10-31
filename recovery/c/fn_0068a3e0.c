#include "cmre.h"
extern u8 CM_THISCALL cm_fn_0068a4d0(u32 self,u32 record,u8 *value);

u8 CM_THISCALL cm_fn_0068a3e0(u32 self,u32 records,u8 mode,u8 *result)
{
  u32 selected;
  u32 table;
  u8 value;
  s32 rating;
  s16 index;
  s16 count;

  *(volatile u8 *)result = 0xff;
  if (mode == 0) selected = *(volatile u32 *)(records + 0x1c);
  else selected = *(volatile u32 *)(records + 0x20);
  rating = (s32)*(volatile s8 *)(self + 0x51) - 10;
  if (rating < 0) rating += 3;
  value = (u8)((s8)(rating >> 2) + 10);
  count = *(volatile s16 *)(self + 0x3e);
  index = 0;
  if (count <= 0) return value;
  for (;;) {
    table = *(volatile u32 *)(self + 0xb1);
    if (*(volatile u32 *)(table + (u32)(s32)index * 0x3b) == selected) break;
    index = (s16)(index + 1);
    if (index >= count) return value;
  }
  if (index == -1) return value;
  if (*(volatile s16 *)(self + 0xa9) <= 10) return value;
  if (*(volatile s16 *)(records + 0x34) < 3) return (u8)(value + 1);
  {
    table = *(volatile u32 *)(self + 0xb1);
    u32 row = table + (u32)(s32)index * 0x3b;
    if (*(volatile u8 *)(row + 0x37) == 0xff) {
      value = (u8)(value + 1);
      *(volatile u8 *)result = cm_fn_0068a4d0(self,row,&value);
    }
  }
  return value;
}
