#ifndef CM_LANG_MORPH_H
#define CM_LANG_MORPH_H
#include "cm_lang.h"
/* Primary VA 006701F0. Five cdecl arguments; shared static output.
 * Caller qualifiers are mutable and may be erased by recognized language rules.
 * Original 00945610 stack probe, fixed-address data and CRT calls are explicit.
 * Valid terminated byte strings, bounded buffers and normal returns are the
 * differential contract; this file does not establish live integration. */
char *CM_LANG_CDECL cm_lang_morph(const char *word, char *prefix,
    char *suffix, cm_lang_u32 property1, cm_lang_u32 property2);
#endif
