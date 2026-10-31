#ifndef CM_DATE_NUMERIC_H
#define CM_DATE_NUMERIC_H
#include "cm_date.h"

/* Original thiscall entries, RET 12. Only low size word/time byte participate.
 * Entry shims preserve the original incoming private-stack month byte. */
cm_u32 CM_THISCALL cm_date_numeric4(cm_date *self, char *output, cm_u32 size, cm_u32 time);
cm_u32 CM_THISCALL cm_date_numeric2(cm_date *self, char *output, cm_u32 size, cm_u32 time);
#endif
