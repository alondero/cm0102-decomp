#ifndef CM_CRT_SEARCH_H
#define CM_CRT_SEARCH_H
#include <stdint.h>
#if defined(__GNUC__) && defined(__i386__)
#define CM_CRT_CDECL __attribute__((cdecl))
#else
#error "Recovered search ABI requires 32-bit x86 GCC"
#endif
/* Primary 00945A60/00946540, cdecl. Shared source uses the original
 * alignment/DWORD reads and faults. Only the character argument low byte
 * matters. No allocation or original executable/data dependency.
 * Finite CPU evidence does not establish Windows exception integration. */
char *CM_CRT_CDECL cm_crt_strstr(const char *haystack, const char *needle);
char *CM_CRT_CDECL cm_crt_strchr(const char *haystack, uint32_t character);
#endif
