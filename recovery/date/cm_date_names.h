#ifndef CM_DATE_NAMES_H
#define CM_DATE_NAMES_H
#include "cm_date.h"
char *CM_CDECL cm_date_time_name(cm_u32 time, cm_u32 full);
/* Original result is AL; preserve dynamic getter reads, upper EAX excluded. */
unsigned char CM_CDECL cm_date_format_order(cm_u32 variant);
char *CM_CDECL cm_date_weekday_name(cm_u32 weekday);
char *CM_CDECL cm_date_short_month_name(cm_u32 month);
char *CM_CDECL cm_date_full_month_name(cm_u32 month);
#endif
