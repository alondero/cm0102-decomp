#ifndef CM_DATE_TEXT_H
#define CM_DATE_TEXT_H
#include "cm_date.h"

/* Original thiscall interfaces, full EAX 0/1, RET 12. Low size word/time byte.
 * Entry shims capture the original prior-frame month byte/dword. */
cm_u32 CM_THISCALL cm_date_weekday_numeric(cm_date *, char *, cm_u32, cm_u32);
cm_u32 CM_THISCALL cm_date_short_weekday_year(cm_date *, char *, cm_u32, cm_u32);
cm_u32 CM_THISCALL cm_date_short_month_year(cm_date *, char *, cm_u32, cm_u32);
cm_u32 CM_THISCALL cm_date_short_weekday(cm_date *, char *, cm_u32, cm_u32);
cm_u32 CM_THISCALL cm_date_full_weekday_year(cm_date *, char *, cm_u32, cm_u32);
cm_u32 CM_THISCALL cm_date_full_weekday(cm_date *, char *, cm_u32, cm_u32);
cm_u32 CM_THISCALL cm_date_full_month_year(cm_date *, char *, cm_u32, cm_u32);
#endif
