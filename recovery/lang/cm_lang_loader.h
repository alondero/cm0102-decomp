#ifndef CM_LANG_LOADER_H
#define CM_LANG_LOADER_H
#include "cm_lang_lifecycle.h"
/* Primary 3.9.68 cdecl normal return, including named prior-frame read slots.
 * Allocator, array support, qsort, file/marker, formatter and UI are dependencies.
 */
cm_lang_u32 CM_LANG_CDECL cm_lang_load(cm_lang_u32 language, cm_lang_u32 progress);
/* Writes fread bytes into its incoming stack argument, as the original does. */
cm_lang_u32 CM_LANG_CDECL cm_lang_byte_order(void *file);
#endif
