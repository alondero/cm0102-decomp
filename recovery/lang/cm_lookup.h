#ifndef CM_LOOKUP_H
#define CM_LOOKUP_H
#include "cm_lang.h"

typedef struct cm_lang_record {
    char *key, *format, *text;
    cm_lang_u32 order[10];
} cm_lang_record;
_Static_assert(sizeof(cm_lang_record) == 52, "original language record");

typedef int (CM_LANG_CDECL *cm_lang_comparator)(const void *, const void *);
int CM_LANG_CDECL cm_lang_key_compare(const void *left, const void *right);
/* Instruction-derived CRT search support, accounted separately from game code. */
void *CM_LANG_CDECL cm_lang_bsearch(const void *key, const void *table,
                                  cm_lang_u32 count, cm_lang_u32 stride,
                                  cm_lang_comparator compare);
cm_lang_u32 CM_LANG_CDECL cm_lang_lookup(cm_lang_record *out, const char *source,
                                       cm_lang_u32 *index);
#endif
