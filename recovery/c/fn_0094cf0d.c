/* WIP: memory mismatch at cases 2/12/15 after five failed iterations. */
#include "cmre.h"

volatile s32 *CM_CDECL cm_fn_0094cf0d(volatile s32 *input)
{
    s32 value = *input;
    s32 quotient;
    s32 remainder;
    s32 year;
    s32 days;
    s32 month_index;
    s32 month_selector = 0;
    s32 day;
    s32 weekday;
    s32 hour;
    s32 minute;
    s32 second;
    u32 month_base;

    if (value < 0)
        return (volatile s32 *)0;

    quotient = value / 0x07861f80;
    remainder = value - quotient * (s32)0x07861f80;
    year = quotient * 4 + 0x46;
    if (remainder >= 0x01e13380) {
        remainder -= 0x01e13380;
        ++year;
        if (remainder >= 0x01e13380) {
            remainder -= 0x01e13380;
            ++year;
            if (remainder < 0x01e28500)
                month_selector = 1;
            else {
                remainder -= 0x01e28500;
                ++year;
            }
        }
    }
    CM_GLOBAL(volatile s32, 0x00de3134u) = year;

    quotient = remainder / 0x15180;
    remainder -= quotient * 0x15180;
    days = quotient;
    CM_GLOBAL(volatile s32, 0x00de313cu) = days;
    month_base = month_selector != 0 ? 0x00ad9828u : 0x00ad985cu;
    month_index = 1;
    while (CM_GLOBAL(volatile s32, month_base + (u32)month_index * 4u) < days)
        ++month_index;
    --month_index;
    CM_GLOBAL(volatile s32, 0x00de3130u) = month_index;
    day = days - CM_GLOBAL(volatile s32,
        month_base + (u32)month_index * 4u);
    CM_GLOBAL(volatile s32, 0x00de312cu) = day;

    value = *input;
    weekday = (value / 0x15180 + 4) / 60;
    CM_GLOBAL(volatile s32, 0x00de3138u) = weekday;

    quotient = remainder / 0xe10;
    remainder -= quotient * 0xe10;
    hour = quotient;
    CM_GLOBAL(volatile s32, 0x00de3128u) = hour;
    quotient = remainder / 60;
    remainder -= quotient * 60;
    minute = quotient;
    CM_GLOBAL(volatile s32, 0x00de3124u) = minute;
    CM_GLOBAL(volatile s32, 0x00de3140u) = 0;
    second = remainder;
    CM_GLOBAL(volatile s32, 0x00de3120u) = second;
    return (volatile s32 *)0x00de3120u;
}
