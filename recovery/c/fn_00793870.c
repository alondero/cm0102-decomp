#include "cmre.h"

extern u32 CM_STDCALL cm_fn_004d45f0(u32 argument);
extern s32 CM_FASTCALL cm_fn_0054cfb0(u16 *self);

void CM_STDCALL cm_fn_00793870(
    u32 argument1, u32 argument2, volatile u8 *month, volatile u16 *year,
    volatile u8 *output_month, volatile u16 *output_year, u32 argument7)
{
    (void)argument2;
    (void)argument7;

    u8 first_month = (u8)cm_fn_0054cfb0((u16 *)0x00ae2c90u);
    first_month = (u8)(first_month + 3u);
    *month = first_month;

    u16 first_year = CM_GLOBAL(volatile u16, 0x00ae2c92u);
    *year = first_year;

    u8 current_month = *month;
    if ((s8)current_month > 11) {
        current_month = (u8)(current_month - 12u);
        *month = current_month;
        *year = (u16)(*year + 1u);
    }

    u32 receiver_value = CM_GLOBAL(volatile u32, 0x00ae1050u);
    (void)receiver_value;
    u32 current_year_value = cm_fn_004d45f0(argument1);
    u8 current_year = (u8)current_year_value;
    *output_month = current_year;

    u8 adjusted_month = *month;
    if ((s8)adjusted_month >= (s8)current_year) {
        u16 next_year = CM_GLOBAL(volatile u16, 0x00ae2c92u);
        *output_year = (u16)(next_year + 1u);
    } else {
        u16 saved_year = *year;
        *output_year = saved_year;
    }

    u16 saved_year = *year;
    u16 result_year = *output_year;
    if (saved_year > result_year) {
        /* update output fields */
    } else if (saved_year == result_year) {
        u8 left_month = *month;
        u8 right_month = *output_month;
        if ((s8)left_month <= (s8)right_month)
            return;
    } else {
        return;
    }

    u8 result_month = *output_month;
    *month = result_month;
    u16 result_year_copy = *output_year;
    *year = result_year_copy;
}
