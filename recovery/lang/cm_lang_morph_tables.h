#ifndef CM_LANG_MORPH_TABLES_H
#define CM_LANG_MORPH_TABLES_H
#include "cm_lang.h"
/* Primary 3.9.68 cdecl; low-byte property, full incidental EAX.
 * The four-pointer tables remain explicit original-image data dependencies.
 */
cm_lang_u32 CM_LANG_CDECL cm_lang_morph_table_a(char *word, cm_lang_u32 property);
cm_lang_u32 CM_LANG_CDECL cm_lang_morph_table_b(char *word, cm_lang_u32 property);
#endif
