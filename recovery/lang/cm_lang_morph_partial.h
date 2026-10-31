#ifndef CM_LANG_MORPH_PARTIAL_H
#define CM_LANG_MORPH_PARTIAL_H
#include "cm_lang.h"
/* Partial parent recovery. Languages 0/7/10/8/6/2/4/3/9 and common composition are C.
 * Other languages transfer to original VA 0067C2E5 with the original frame.
 * The original 00945610 stack probe and CRT calls remain explicit dependencies.
 * Languages two/four/three/nine can erase unrecognized caller prefix/suffix bytes.
 */
char *CM_LANG_CDECL cm_lang_morph_partial(const char *word, char *prefix,
    char *suffix, cm_lang_u32 property1, cm_lang_u32 property2);
#endif
