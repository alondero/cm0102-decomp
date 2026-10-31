#include "cmre.h"

#define DAY_TABLE_A CM_GLOBAL(volatile u32, 0x00ad9824)
#define DAY_TABLE_B CM_GLOBAL(volatile u32, 0x00ad9858)
#define DAY5_TABLE_A CM_GLOBAL(volatile u32, 0x00ad9828)
#define DAY5_TABLE_B CM_GLOBAL(volatile u32, 0x00ad985c)
#define DAT_00ad9200 CM_GLOBAL(volatile s32, 0x00ad9200)
#define DAT_00ad9290 CM_GLOBAL(volatile u32, 0x00ad9290)
#define DAT_00ad9294 CM_GLOBAL(volatile s32, 0x00ad9294)
#define DAT_00ad9298 CM_GLOBAL(volatile s32, 0x00ad9298)
#define DAT_00ad92a0 CM_GLOBAL(volatile u32, 0x00ad92a0)
#define DAT_00ad92a4 CM_GLOBAL(volatile s32, 0x00ad92a4)
#define DAT_00ad92a8 CM_GLOBAL(volatile s32, 0x00ad92a8)

void CM_CDECL cm_fn_0094cdcd(s32 mode,s32 calendar_mode,u32 year,s32 month,s32 week,
                              s32 weekday,s32 day_offset,s32 hour,s32 minute,s32 second,s32 ms)
{
  u32 month_index = year & 3u;
  u32 table_offset = (u32)month * 4u;
  u32 table_value;
  u32 day_value;
  u32 time_value;
  s32 remainder;

  if (calendar_mode == 1) {
    if (month_index == 0) table_value = *(volatile u32 *)((u32)&DAY_TABLE_A + table_offset);
    else table_value = *(volatile u32 *)((u32)&DAY_TABLE_B + table_offset);
    {
      u32 ecx = table_value + 1u;
      u32 edx = year * 0x16du;
      s32 correction = (s32)(year - 1u) >> 2;
      s32 dividend = (s32)(edx + ecx + (u32)correction - 0x63dbu);
      remainder = dividend % 7;
      {
        u32 day_term = (u32)week * 7u - (u32)remainder + (u32)weekday;
        day_value = ecx;
        if (remainder > weekday) day_value += day_term;
        else day_value += day_term - 7u;
      }
    }
    if (week == 5) {
      if (month_index == 0) table_value = *(volatile u32 *)((u32)&DAY5_TABLE_A + table_offset);
      else table_value = *(volatile u32 *)((u32)&DAY5_TABLE_B + table_offset);
      if ((s32)table_value < (s32)day_value) day_value -= 7u;
    }
  }
  else {
    if (month_index == 0) table_value = *(volatile u32 *)((u32)&DAY_TABLE_A + table_offset);
    else table_value = *(volatile u32 *)((u32)&DAY_TABLE_B + table_offset);
    day_value = table_value + (u32)day_offset;
  }

  if (mode == 1) {
    DAT_00ad9294 = (s32)day_value;
    time_value = (u32)hour * 0x3cu;
    time_value += (u32)minute;
    time_value *= 0x3cu;
    time_value += (u32)second;
    time_value *= 1000u;
    time_value += (u32)ms;
    DAT_00ad9290 = year;
    DAT_00ad9298 = (s32)time_value;
  }
  else {
    DAT_00ad92a4 = (s32)day_value;
    time_value = (u32)hour * 0x3cu;
    time_value += (u32)minute;
    time_value *= 0x3cu;
    time_value += (u32)DAT_00ad9200;
    time_value += (u32)second;
    time_value *= 1000u;
    time_value += (u32)ms;
    DAT_00ad92a8 = (s32)time_value;
    if ((s32)time_value < 0) {
      time_value += 0x5265c00u;
      day_value -= 1u;
      DAT_00ad92a8 = (s32)time_value;
      DAT_00ad92a4 = (s32)day_value;
    }
    else if (time_value >= 0x5265c00u) {
      time_value -= 0x5265c00u;
      day_value += 1u;
      DAT_00ad92a8 = (s32)time_value;
      DAT_00ad92a4 = (s32)day_value;
    }
    DAT_00ad92a0 = year;
  }
}
