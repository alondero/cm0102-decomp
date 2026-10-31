#ifndef CM_DATE_H
#define CM_DATE_H

/* Original 32-bit layout. Values are bits, not host signed-short conversions. */
typedef unsigned short cm_u16;
typedef unsigned int cm_u32;
typedef struct cm_date {
    union {
        struct { cm_u16 day, year; };
        cm_u32 day_year;
    };
    cm_u32 leap;
} cm_date;
_Static_assert(sizeof(cm_u16) == 2 && sizeof(cm_u32) == 4, "integer widths");
_Static_assert(sizeof(cm_date) == 8, "original Date layout");

#define CM_THISCALL __attribute__((thiscall))
#define CM_CDECL __attribute__((cdecl))
cm_date *CM_THISCALL cm_date_default(cm_date *self);
cm_date *CM_THISCALL cm_date_assign(cm_date *self, cm_date *out, const cm_date *source);
cm_date *CM_THISCALL cm_date_add(cm_date *self, cm_date *out, cm_u32 increment);
cm_date *CM_THISCALL cm_date_sub(cm_date *self, cm_date *out, cm_u32 decrement);
int CM_THISCALL cm_date_weekday(cm_date *self);
/* Side-effect operation: original EAX is target on equality, otherwise a dead
 * private-stack temporary. Neither is exposed by this bounded interface. */
void CM_THISCALL cm_date_adjust_weekday(cm_date *self, cm_u32 target, cm_date *dest);
/* Constructor side effects only; original EAX is not a this-pointer return. */
void CM_THISCALL cm_date_ymd(cm_date *self, cm_u32 day, cm_u32 month,
                           cm_u32 year, cm_u32 target);
cm_date *CM_THISCALL cm_date_construct_ymd(cm_date *self, cm_u32 day, cm_u32 month,
                                        cm_u32 year, cm_u32 target);
cm_date *CM_THISCALL cm_date_add_weeks(cm_date *self, cm_date *out, cm_u32 weeks);
cm_date *CM_THISCALL cm_date_sub_weeks(cm_date *self, cm_date *out, cm_u32 weeks);
/* Only AX is the day-of-month result; upper EAX contains incidental bits. */
cm_u16 CM_THISCALL cm_date_day_of_month(cm_date *self);
int CM_THISCALL cm_date_month(cm_date *self);
cm_u32 CM_CDECL cm_date_is_leap(cm_u32 year);
/* The second operand is passed by VALUE, two dwords on the original stack. */
cm_date *CM_THISCALL cm_date_elapsed_components(cm_date *self, cm_date *out, cm_date other);
int CM_THISCALL cm_date_days_between(cm_date *self, cm_date other);
const char *CM_CDECL cm_date_diagnostic_version(const char *file, const char *extension, cm_u32 line);
#endif
