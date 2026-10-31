/* Instruction-derived official 3.9.68 slice. See docs/DATE.md and ledger.json.
 * Fixed original VAs deliberately retain hybrid dependencies and shared state.
 * Build only as 32-bit x86; this is not a standalone calendar library.
 */
#include "cm_date.h"

#define ADD_RESULT ((volatile cm_date *)0x00ae2420u)
#define SUB_RESULT ((volatile cm_date *)0x00ae24c0u)
#define ADD_GUARD (*(volatile unsigned char *)0x00ae2469u)
#define SUB_GUARD (*(volatile unsigned char *)0x00ae249eu)
typedef int (__attribute__((cdecl)) *register_exit)(cm_u32);
#define REGISTER_EXIT ((register_exit)0x0094627au)

/* VA 0054A3F0. C11 division/remainder truncate toward zero, like IDIV.
 * The range of year/day keeps every signed intermediate within int32.
 * Do not normalize negative remainders or recompute the stored leap flag. */
int CM_THISCALL cm_date_weekday(cm_date *self) {
    const volatile cm_date *source = self;
    int year = (int)source->year - 1600;
    int day = source->day;
    if (day >= 32768) day -= 65536;
    int value = year * 365 + year / 400 - year / 100 + year / 4 + day;
    if (source->leap) --value;
    int remainder = value % 7;
    return remainder > 0 ? remainder - 1 : (remainder == 0 ? 6 : remainder);
}

static int signed_word(cm_u16 bits) {
    return bits < 0x8000u ? (int)bits : (int)bits - 65536;
}

/* All original tests zero-extend the year before signed division. */
static cm_u32 leap_year(cm_u16 year) {
    return (year % 4u == 0u) && (year % 100u != 0u || year % 400u == 0u);
}

/* VA 0054A110, including its unusual default (day 40, year 1972). */
cm_date *CM_THISCALL cm_date_default(cm_date *self) {
    volatile cm_date *dest = self;
    dest->day = 40;
    dest->year = 1972;
    dest->leap = 1;
    return self;
}

/* VA 00417C10: assignment modifies ECX and returns a VALUE through a hidden
 * buffer, not a reference to ECX. Preserve the original access order. */
cm_date *CM_THISCALL cm_date_assign(cm_date *self, cm_date *out, const cm_date *source) {
    volatile cm_date *dest = self;
    const volatile cm_date *src = source;
    volatile cm_date *result = out;
    dest->year = src->year;
    dest->day = src->day;
    dest->leap = src->leap;
    result->day_year = dest->day_year;
    result->leap = dest->leap;
    return out;
}

/* VA 0054C5B0. Only the low word selects sign and participates in arithmetic.
 * Full-width unsigned negation models NEG ESI without C signed overflow. */
cm_date *CM_THISCALL cm_date_add(cm_date *self, cm_date *out, cm_u32 increment) {
    volatile cm_date *result = ADD_RESULT;
    const volatile cm_date *source = self;
    if (!(ADD_GUARD & 1u)) {
        unsigned char guard = ADD_GUARD;
        result->day = 40;
        ADD_GUARD = guard | 1u;
        result->year = 1972;
        result->leap = 1;
        REGISTER_EXIT(0x0054c760u);
    }
    if (signed_word((cm_u16)increment) < 0) {
        cm_date temp, unused;
        cm_date_assign((cm_date *)result, &temp, self);
        cm_date_sub((cm_date *)result, &temp, 0u - increment);
        cm_date_assign((cm_date *)result, &unused, &temp);
    } else {
        cm_u16 year = source->year;
        result->year = year;
        cm_u16 day = (cm_u16)(source->day + (cm_u16)increment);
        result->day = day;
        for (;;) {
            cm_u16 length = (cm_u16)(365u + leap_year(year));
            if (signed_word(day) < (int)length) break;
            day = (cm_u16)(day - length);
            year = (cm_u16)(year + 1u);
        }
        result->year = year;
        result->day = day;
        result->leap = leap_year(year);
    }
    ((volatile cm_date *)out)->day_year = result->day_year;
    ((volatile cm_date *)out)->leap = result->leap;
    return out;
}

/* VA 0054C8E0. Negative word 0x8000 intentionally retains mutual recursion;
 * replacing it with a mathematical +32768 would change original behavior. */
cm_date *CM_THISCALL cm_date_sub(cm_date *self, cm_date *out, cm_u32 decrement) {
    volatile cm_date *result = SUB_RESULT;
    const volatile cm_date *source = self;
    if (!(SUB_GUARD & 1u)) {
        unsigned char guard = SUB_GUARD;
        result->day = 40;
        SUB_GUARD = guard | 1u;
        result->year = 1972;
        result->leap = 1;
        REGISTER_EXIT(0x0054caa0u);
    }
    if (signed_word((cm_u16)decrement) < 0) {
        cm_date temp, unused;
        cm_date_assign((cm_date *)result, &temp, self);
        cm_date_add((cm_date *)result, &temp, 0u - decrement);
        cm_date_assign((cm_date *)result, &unused, &temp);
    } else {
        cm_u16 year = source->year;
        result->year = year;
        cm_u16 day = (cm_u16)(source->day - (cm_u16)decrement);
        result->day = day;
        while (signed_word(day) < 0) {
            year = (cm_u16)(year - 1u);
            day = (cm_u16)(day + 365u + leap_year(year));
        }
        result->year = year;
        result->day = day;
        result->leap = leap_year(year);
    }
    ((volatile cm_date *)out)->day_year = result->day_year;
    ((volatile cm_date *)out)->leap = result->leap;
    return out;
}

/* VA 0054A2D0, normal-return semantics. Original installs a temporary MSVC
 * SEH frame; exception dispatch/unwinding is not recovered here. The caller's
 * destination is copied even when no adjustment is required. Both original
 * branches call subtraction (which may dispatch to addition). */
void CM_THISCALL cm_date_adjust_weekday(cm_date *self, cm_u32 target, cm_date *dest) {
    cm_u32 current = (cm_u32)cm_date_weekday(self);
    cm_date temp, unused;
    cm_date_assign(dest, &unused, self);
    if ((cm_u16)current != (cm_u16)target) {
        cm_u32 delta = current - target;
        int low = signed_word((cm_u16)delta);
        if (low < 0) {
            if (low < -3) delta += 7u;
        } else {
            if (low > 3) delta -= 7u;
        }
        cm_date_sub(dest, &temp, delta);
        cm_date_assign(dest, &unused, &temp);
    }
}

/* VA 0090D130: arguments are ignored; returns the original version string. */
const char *CM_CDECL cm_date_diagnostic_version(const char *file, const char *extension, cm_u32 line) {
    (void)file; (void)extension; (void)line;
    return (const char *)0x00ad4394u;
}

/* Constructor diagnostics retain explicit original library/UI dependencies.
 * Tests hook these calls, not their effects on a real Windows process. */
typedef void (CM_CDECL *split_path)(const char *, char *, char *, char *, char *);
typedef int (CM_CDECL *format_message)(char *, const char *, ...);
typedef int (CM_CDECL *show_error)(const char *, const char *, cm_u32);
static void constructor_diagnostic(cm_u32 line) {
    char file[256], extension[256];
    ((split_path)0x00944cffu)((const char *)0x009b8478u, 0, 0, file, extension);
    const char *version = cm_date_diagnostic_version(file, extension, line);
    ((format_message)0x00944c9fu)((char *)0x00ae24d0u, (const char *)0x009870e8u,
                                version, file, extension, line);
    ((show_error)0x005e8290u)((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    *(volatile cm_u32 *)0x00b67a34u = 0;
}

static const cm_u16 month_offsets[2][12] = {
    {65535, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333},
    {65535, 30, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334}
};

/* VA 0054A130. Validation is on a signed WORD/CHAR, not the full arguments.
 * There is no month-specific day check (31 February remains constructible).
 * The target's low-word FFFF sentinel suppresses weekday adjustment. */
static void date_set_ymd(cm_date *self, cm_u32 day, cm_u32 month,
                         cm_u32 year, cm_u32 target, cm_u32 day_line, cm_u32 month_line) {
    if (signed_word((cm_u16)day) < 1 || signed_word((cm_u16)day) > 31) {
        constructor_diagnostic(day_line);
        day = 1;
    }
    unsigned int m = month & 255u;
    if (m > 11u) {
        constructor_diagnostic(month_line);
        m = 0;
    }
    volatile cm_date *dest = self;
    dest->year = (cm_u16)year;
    dest->leap = leap_year((cm_u16)year);
    dest->day = (cm_u16)(month_offsets[dest->leap][m] + (cm_u16)day);
    if ((cm_u16)target != 65535u) cm_date_adjust_weekday(self, target, self);
}

void CM_THISCALL cm_date_ymd(cm_date *self, cm_u32 day, cm_u32 month,
                           cm_u32 year, cm_u32 target) {
    date_set_ymd(self, day, month, year, target, 114, 121);
}

/* VA 00549F70 is the constructor variant: diagnostic source lines 52/59,
 * and MOV EAX,ESI restores a this-pointer result even after adjustment.
 * Unlike 0054A130, its full normal-return EAX contract is preserved. */
cm_date *CM_THISCALL cm_date_construct_ymd(cm_date *self, cm_u32 day, cm_u32 month,
                                        cm_u32 year, cm_u32 target) {
    date_set_ymd(self, day, month, year, target, 52, 59);
    return self;
}

/* VAs 0054CF70 / 0054CFB0. Stored leap flag chooses the table, independent
 * of year. Signed comparisons preserve invalid negative-ordinal behavior. */
int CM_THISCALL cm_date_month(cm_date *self) {
    const volatile cm_date *source = self;
    const cm_u16 *table = month_offsets[source->leap != 0];
    int day = signed_word(source->day);
    for (int month = 11; month >= 0; --month)
        if (signed_word(table[month]) < day) return month;
    return -1;
}

cm_u16 CM_THISCALL cm_date_day_of_month(cm_date *self) {
    const volatile cm_date *source = self;
    const cm_u16 *table = month_offsets[source->leap != 0];
    cm_u16 day = source->day;
    for (int month = 11; month >= 0; --month)
        if (signed_word(table[month]) < signed_word(day)) return (cm_u16)(day - table[month]);
    return 65535u;
}

/* VA 0054CFE0: cdecl, zero-extend only the low WORD of the argument. */
cm_u32 CM_CDECL cm_date_is_leap(cm_u32 year) { return leap_year((cm_u16)year); }

/* VAs 0054C770 / 0054CAB0. Independent scratches and unsigned low BYTE
 * week counts; do not route through the signed-word day operators. */
static cm_date *weeks_result(cm_date *self, cm_date *out, cm_u32 weeks, int subtract) {
    volatile cm_date *result = (volatile cm_date *)(subtract ? 0x00ae24c8u : 0x00ae2410u);
    volatile unsigned char *guard = (volatile unsigned char *)(subtract ? 0x00ae24aau : 0x00ae2430u);
    if (!(*guard & 1u)) {
        unsigned char bits = *guard;
        result->day = 40;
        *guard = bits | 1u;
        result->year = 1972;
        result->leap = 1;
        REGISTER_EXIT(subtract ? 0x0054cc20u : 0x0054c8d0u);
    }
    const volatile cm_date *source = self;
    cm_u16 year = source->year;
    result->year = year;
    cm_u16 count = (cm_u16)((weeks & 255u) * 7u);
    cm_u16 day = subtract ? (cm_u16)(source->day - count) : (cm_u16)(source->day + count);
    result->day = day;
    if (subtract) {
        while (signed_word(day) < 0) {
            year = (cm_u16)(year - 1u);
            day = (cm_u16)(day + 365u + leap_year(year));
        }
    } else {
        for (;;) {
            cm_u16 length = (cm_u16)(365u + leap_year(year));
            if (signed_word(day) < (int)length) break;
            day = (cm_u16)(day - length);
            year = (cm_u16)(year + 1u);
        }
    }
    result->year = year;
    result->day = day;
    result->leap = leap_year(year);
    ((volatile cm_date *)out)->day_year = result->day_year;
    ((volatile cm_date *)out)->leap = result->leap;
    return out;
}
cm_date *CM_THISCALL cm_date_add_weeks(cm_date *self, cm_date *out, cm_u32 weeks) {
    return weeks_result(self, out, weeks, 0);
}
cm_date *CM_THISCALL cm_date_sub_weeks(cm_date *self, cm_date *out, cm_u32 weeks) {
    return weeks_result(self, out, weeks, 1);
}

/* VA 0054CC30, normal-return semantics (temporary SEH omitted).
 * This yields an absolute pair of year/day components, not elapsed days.
 * A day borrow uses SELF's year length, and scratch leap remains untouched. */
cm_date *CM_THISCALL cm_date_elapsed_components(cm_date *self, cm_date *out, cm_date other) {
    volatile cm_date *result = (volatile cm_date *)0x00ae2428u;
    volatile unsigned char *guard = (volatile unsigned char *)0x00ae249du;
    if (!(*guard & 1u)) {
        unsigned char bits = *guard;
        result->day = 40;
        *guard = bits | 1u;
        result->year = 1972;
        result->leap = 1;
        REGISTER_EXIT(0x0054cda0u);
    }
    const volatile cm_date *source = self;
    cm_u16 year = source->year;
    if (other.year > year || (other.year == year && signed_word(other.day) > signed_word(source->day))) {
        cm_date copy;
        copy.leap = source->leap;
        copy.day_year = source->day_year;
        cm_date_elapsed_components(&other, out, copy);
        return out;
    }
    int delta = signed_word(source->day) - signed_word(other.day);
    if (delta >= 0) {
        result->day = (cm_u16)delta;
        result->year = (cm_u16)(source->year - other.year);
    } else {
        result->day = (cm_u16)(delta + 365u + leap_year(year));
        result->year = (cm_u16)(source->year - other.year - 1u);
    }
    ((volatile cm_date *)out)->day_year = result->day_year;
    ((volatile cm_date *)out)->leap = result->leap;
    return out;
}

/* VA 0054CDB0. Keep the asymmetric ORIGINAL forward-year rule: when self
 * precedes other, the year loop counts year-1, not year. */
int CM_THISCALL cm_date_days_between(cm_date *self, cm_date other) {
    volatile cm_date *result = (volatile cm_date *)0x00ae2470u;
    volatile unsigned char *guard = (volatile unsigned char *)0x00ae2418u;
    if (!(*guard & 1u)) {
        unsigned char bits = *guard;
        result->day = 40;
        *guard = bits | 1u;
        result->year = 1972;
        result->leap = 1;
        REGISTER_EXIT(0x0054cf60u);
    }
    const volatile cm_date *source = self;
    cm_u16 year = source->year;
    int total = 0;
    if (year != other.year) {
        cm_date unused;
        cm_date_assign((cm_date *)result, &unused, self);
        year = result->year;
        if (year < other.year) {
            while (year < other.year) {
                total -= (int)(365u + leap_year((cm_u16)(year - 1u)));
                year = (cm_u16)(year + 1u);
            }
        } else {
            while (year > other.year) {
                year = (cm_u16)(year - 1u);
                total += (int)(365u + leap_year(year));
            }
        }
        result->year = year;
    }
    return total + signed_word(source->day) - signed_word(other.day);
}
