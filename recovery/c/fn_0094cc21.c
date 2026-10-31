#include "cmre.h"

extern void CM_CDECL cm_fn_0094cdcd(s32 mode, s32 calendar_mode, u32 year,
                                     s32 month, s32 week, s32 weekday,
                                     s32 day_offset, s32 hour, s32 minute,
                                     s32 second, s32 millisecond);

u32 CM_CDECL cm_fn_0094cc21(const volatile u8 *record)
{
    u32 enabled = CM_GLOBAL(volatile u32, 0x00ad91fc);
    if (enabled == 0) return 0;

    s32 cached_year = *(const volatile s32 *)(record + 0x14);
    s32 first_year = CM_GLOBAL(volatile s32, 0x00ad9290);
    if (cached_year == first_year) {
        s32 second_year = CM_GLOBAL(volatile s32, 0x00ad92a0);
        if (cached_year == second_year) goto year_checks_done;
    }

    if (CM_GLOBAL(volatile s32, 0x00de3060) == 0) {
        cm_fn_0094cdcd(1, 1, (u32)cached_year, 4, 1, 0, 0, 2, 0, 0, 0);
        s32 reloaded_year = *(const volatile s32 *)(record + 0x14);
        cm_fn_0094cdcd(0, 1, (u32)reloaded_year, 10, 5, 0, 0, 2, 0, 0, 0);
    } else {
        u32 millisecond = CM_GLOBAL(volatile u16, 0x00de310e);
        u32 calendar_word = CM_GLOBAL(volatile u16, 0x00de3100);
        u32 second = CM_GLOBAL(volatile u16, 0x00de310c);
        u32 minute = CM_GLOBAL(volatile u16, 0x00de310a);
        u32 hour = CM_GLOBAL(volatile u16, 0x00de3108);
        if (calendar_word == 0) {
            u32 weekday = CM_GLOBAL(volatile u16, 0x00de3104);
            u32 week = CM_GLOBAL(volatile u16, 0x00de3106);
            u32 month = CM_GLOBAL(volatile u16, 0x00de3102);
            cm_fn_0094cdcd(1, 1, (u32)cached_year, (s32)month, (s32)week,
                           (s32)weekday, 0, (s32)hour, (s32)minute,
                           (s32)second, (s32)millisecond);
        } else {
            u32 week = CM_GLOBAL(volatile u16, 0x00de3106);
            u32 month = CM_GLOBAL(volatile u16, 0x00de3102);
            cm_fn_0094cdcd(1, 0, (u32)cached_year, (s32)month, 0, 0,
                           (s32)week, (s32)hour, (s32)minute, (s32)second,
                           (s32)millisecond);
        }

        u32 ms2 = CM_GLOBAL(volatile u16, 0x00de30ba);
        u32 calendar_word2 = CM_GLOBAL(volatile s16, 0x00de30ac);
        u32 second2 = CM_GLOBAL(volatile u16, 0x00de30b8);
        u32 minute2 = CM_GLOBAL(volatile u16, 0x00de30b6);
        u32 hour2 = CM_GLOBAL(volatile u16, 0x00de30b4);
        if ((s16)calendar_word2 == 0) {
            u32 weekday2 = CM_GLOBAL(volatile u16, 0x00de30b0);
            u32 week2 = CM_GLOBAL(volatile u16, 0x00de30b2);
            u32 month2 = CM_GLOBAL(volatile u16, 0x00de30ae);
            s32 reloaded_year = *(const volatile s32 *)(record + 0x14);
            cm_fn_0094cdcd(0, 1, (u32)reloaded_year, (s32)month2,
                           (s32)week2, (s32)weekday2, 0, (s32)hour2,
                           (s32)minute2, (s32)second2, (s32)ms2);
        } else {
            u32 week2 = CM_GLOBAL(volatile u16, 0x00de30b2);
            u32 month2 = CM_GLOBAL(volatile u16, 0x00de30ae);
            s32 reloaded_year = *(const volatile s32 *)(record + 0x14);
            cm_fn_0094cdcd(0, 0, (u32)reloaded_year, (s32)month2, 0, 0,
                           (s32)week2, (s32)hour2, (s32)minute2,
                           (s32)second2, (s32)ms2);
        }
    }

year_checks_done:
    s32 start = CM_GLOBAL(volatile s32, 0x00ad9294);
    s32 end = CM_GLOBAL(volatile s32, 0x00ad92a4);
    s32 value = *(const volatile s32 *)(record + 0x1c);

    if (start < end) {
        if (value < start || value > end) return 0;
        if (value > start && value < end) return 1;
    } else {
        if (value < end || value > start) return 1;
        if (value > end && value < start) {
            return 0;
        }
    }

    u32 hours = (u32)*(const volatile s32 *)(record + 8);
    hours = hours * 0x3cu + (u32)*(const volatile s32 *)(record + 4);
    hours = hours * 0x3cu + (u32)*(const volatile s32 *)(record + 0);
    u32 milliseconds = hours * 1000u;
    if (value == start) {
        s32 lower = CM_GLOBAL(volatile s32, 0x00ad9298);
        return (s32)milliseconds >= lower;
    }
    s32 upper = CM_GLOBAL(volatile s32, 0x00ad92a8);
    return (s32)milliseconds < upper;
}
