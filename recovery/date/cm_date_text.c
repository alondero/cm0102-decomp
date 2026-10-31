/* Instruction-derived primary 3.9.68, preferred VAs; see DATE-TEXT.md.
 * Preserve normal-return ordering, low-width arithmetic, prior stack storage
 * and original templates. No host-calendar or invented language rules.
 */
#include "cm_date_text.h"
#include "cm_date_names.h"
#include "../lang/cm_lang.h"
#include "../lang/cm_ordinal.h"

typedef void (CM_CDECL *split_fn)(const char *, char *, char *, char *, char *);
typedef int (CM_CDECL *print_fn)(char *, const char *, ...);
typedef int (CM_CDECL *error_fn)(const char *, const char *, cm_u32);
#define TEXT(va) ((const char *)(va))
#define SHORT_TIME ((char *)0x00ae24acu)
#define FULL_TIME ((char *)0x00ae2484u)
#define SHORT_WEEKDAY ((char *)0x00ae2478u)
#define FULL_WEEKDAY ((char *)0x00ae2434u)

typedef struct text_contract {
    cm_u32 capacity, null_line, size_line, plain_format, time_format;
} text_contract;
static const text_contract contracts[7] = {
    {18, 385, 392, 0x009b86f8u, 0x009b867cu},
    {18, 447, 454, 0x009b881cu, 0x009b87a0u},
    {18, 507, 514, 0x009b8900u, 0x009b8898u},
    {18, 564, 571, 0x009b89b0u, 0x009b894cu},
    {50, 624, 631, 0x009b8aa0u, 0x009b89f8u},
    {50, 687, 694, 0x009b8b84u, 0x009b8b0cu},
    {50, 749, 756, 0x009b8c4cu, 0x009b8bd8u},
};

static int signed_word(cm_u32 bits) {
    bits &= 65535u;
    return bits < 32768u ? (int)bits : (int)bits - 65536;
}

static cm_u32 diagnostic(cm_u32 line) {
    char filename[256], extension[256];
    ((split_fn)0x00944cffu)(TEXT(0x009b8478u), 0, 0, filename, extension);
    const char *version = cm_date_diagnostic_version(filename, extension, line);
    ((print_fn)0x00944c9fu)((char *)0x00ae24d0u, TEXT(0x009870e8u),
                          version, filename, extension, line);
    ((error_fn)0x005e8290u)(TEXT(0x009870e0u), TEXT(0x00ae24d0u), 0);
    *(volatile cm_u32 *)0x00b67a34u = 0;
    return 0;
}

/* Original inlined weekday arithmetic, including negative remainder behavior.
 * At this range all signed intermediate values fit int32. */
static int weekday(int year, int ordinal, cm_u32 leap) {
    year -= 1600;
    int value = year * 365 + year / 400 - year / 100 + year / 4 + ordinal;
    if (leap) --value;
    int remainder = value % 7;
    return remainder > 0 ? remainder - 1 : (remainder == 0 ? 6 : remainder);
}

static void short_weekday_name(int value) {
    static const cm_u32 formats[7] = {0x009b8890u, 0x009b888cu, 0x009b8888u,
        0x009b8884u, 0x009b8880u, 0x009b887cu, 0x009b8894u};
    if ((cm_u32)value > 6) SHORT_WEEKDAY[0] = 0;
    else cm_lang_format(SHORT_WEEKDAY, TEXT(formats[value]));
}

/* VA 0054B2F0 repeats an independent reverse scan instead of indexing with
 * the prior month byte. Its no-time path uses captured day/table; time path
 * reloads them after formatting the time string. */
static int reverse_day(int ordinal, cm_u32 table) {
    for (int i = 11; i >= 0; --i) {
        cm_u32 offset = *(const volatile cm_u16 *)(table + 2u * (cm_u32)i);
        if (signed_word(offset) < ordinal) return signed_word((cm_u32)ordinal - offset);
    }
    return -1;
}

/* Implementation support; seven original entries, no extra application count.
 * Captured full month dwords retain all three untouched high bytes. */
cm_u32 CM_CDECL cm_date_text_worker(cm_u32 variant, cm_date *self, char *output,
                                  cm_u32 size, cm_u32 time, cm_u32 seed) {
    const text_contract *contract = &contracts[variant];
    if (!output) return diagnostic(contract->null_line);
    if (signed_word(size) < (int)contract->capacity) return diagnostic(contract->size_line);
    const volatile cm_date *source = self;
    cm_u32 leap = source->leap;
    cm_u32 table = leap ? 0x009b8460u : 0x009b8448u;
    int ordinal = signed_word(source->day);
    for (cm_u32 i = 0; i < 12; ++i)
        if (signed_word(*(const volatile cm_u16 *)(table + 2u * i)) < ordinal)
            seed = (seed & 0xffffff00u) | i;
    cm_u32 low_month = seed & 255u;
    int month = low_month < 128u ? (int)low_month : (int)low_month - 256;
    int day = -1;
    if (variant != 2) {
        cm_u32 offset = *(const volatile cm_u16 *)(table + 2u * (cm_u32)month);
        day = signed_word((cm_u32)ordinal - offset);
    }
    time &= 255u;
    cm_u32 with_time = time <= 2;
    cm_u32 full = variant >= 4;
    char *time_text = full ? FULL_TIME : SHORT_TIME;
    if (with_time) {
        static const cm_u32 short_formats[3] = {0x009b8508u, 0x009b8524u, 0x009b8544u};
        static const cm_u32 full_formats[3] = {0x009b8a84u, 0x009b8a8cu, 0x009b8a98u};
        if (full) cm_lang_format(time_text, TEXT(full_formats[time]));
        else cm_lang_format(time_text, TEXT(short_formats[time]), TEXT(0x00ad9c64u));
    }
    char *week_text = (variant == 0 || full) ? FULL_WEEKDAY : SHORT_WEEKDAY;
    if (variant != 2 && variant != 6) {
        int year = source->year;
        int weekday_ordinal = with_time ? signed_word(source->day) : ordinal;
        cm_u32 weekday_leap = with_time ? source->leap : leap;
        int value = weekday(year, weekday_ordinal, weekday_leap);
        if (variant == 0 || full) cm_date_weekday_name((cm_u32)value);
        else short_weekday_name(value);
    }
    cm_u32 format = with_time ? contract->time_format : contract->plain_format;
    if (variant == 0) {
        cm_u32 year = source->year % 100u;
        if (with_time) cm_lang_format(output, TEXT(format), 100u, week_text, day, month + 1, year, time_text);
        else cm_lang_format(output, TEXT(format), 100u, week_text, day, month + 1, year);
        return 1;
    }
    if (variant == 2) {
        if (with_time) {
            cm_u32 current_table = source->leap ? 0x009b8460u : 0x009b8448u;
            day = reverse_day(signed_word(source->day), current_table);
        } else day = reverse_day(ordinal, table);
    }
    /* Original pushes year before the month-name call and retains it on the
     * argument stack through month/ordinal calls. Do not reread it afterward. */
    cm_u32 has_year = variant == 1 || variant == 2 || variant == 4 || variant == 6;
    cm_u32 year = has_year ? source->year : 0;
    char *month_text = full ? cm_date_full_month_name(seed) : cm_date_short_month_name(seed);
    char *day_text = cm_ordinal((cm_u32)day, 1);
    if (variant == 2 || variant == 6) {
        if (with_time) cm_lang_format(output, TEXT(format), day_text, month_text, year, time_text);
        else cm_lang_format(output, TEXT(format), day_text, month_text, year);
    } else if (full) {
        if (has_year) {
            if (with_time) cm_lang_format(output, TEXT(format), 100u, week_text, day_text, month_text, year, time_text);
            else cm_lang_format(output, TEXT(format), 100u, week_text, day_text, month_text, year);
        } else {
            if (with_time) cm_lang_format(output, TEXT(format), 100u, week_text, day_text, month_text, time_text);
            else cm_lang_format(output, TEXT(format), 100u, week_text, day_text, month_text);
        }
    } else if (has_year) {
        if (with_time) cm_lang_format(output, TEXT(format), week_text, day_text, month_text, year, time_text);
        else cm_lang_format(output, TEXT(format), week_text, day_text, month_text, year);
    } else {
        if (with_time) cm_lang_format(output, TEXT(format), week_text, day_text, month_text, time_text);
        else cm_lang_format(output, TEXT(format), week_text, day_text, month_text);
    }
    return 1;
}
