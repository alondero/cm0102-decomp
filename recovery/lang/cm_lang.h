#ifndef CM_LANG_H
#define CM_LANG_H

#define CM_LANG_CDECL __attribute__((cdecl))
typedef unsigned int cm_lang_u32;
_Static_assert(sizeof(cm_lang_u32) == 4 && sizeof(void *) == 4, "original x86 widths");

cm_lang_u32 CM_LANG_CDECL cm_lang_current(void);
/* Normal-return wrapper contract. Lookup, morphology and CRT remain explicit
 * original dependencies. This is not a standalone localization library. */
char *CM_LANG_CDECL cm_lang_format(char *output, const char *format, ...);

#endif
