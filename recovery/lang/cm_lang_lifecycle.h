#ifndef CM_LANG_LIFECYCLE_H
#define CM_LANG_LIFECYCLE_H
#include "cm_lookup.h"

#define CM_LANG_THISCALL __attribute__((thiscall))
/* Full normal-return EAX, including incidental destructor/free result bits. */
cm_lang_record *CM_LANG_THISCALL cm_lang_row_construct(cm_lang_record *);
cm_lang_u32 CM_LANG_THISCALL cm_lang_row_destroy(cm_lang_record *);
cm_lang_u32 CM_LANG_CDECL cm_lang_teardown(void);
cm_lang_u32 CM_LANG_CDECL cm_lang_available(cm_lang_u32 language);
cm_lang_u32 CM_LANG_CDECL cm_lang_setup(const char *filename, cm_lang_u32 memory_mode);
#endif
