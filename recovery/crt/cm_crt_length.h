#ifndef CM_CRT_LENGTH_H
#define CM_CRT_LENGTH_H
#include <stdint.h>
#if defined(__GNUC__) && defined(__i386__)
#define CM_CRT_LENGTH_CDECL __attribute__((cdecl))
#else
#error "Recovered length ABI requires 32-bit x86 GCC"
#endif
/* Primary preferred-image VA 0x009519D3, official 3.9.68.
 * Reads pointer[limit] when no earlier NUL is found, including limit zero.
 * A NUL encountered by the loop is read again. The extra read may fault.
 * No external code/data dependencies; Windows SEH remains unvalidated. */
uint32_t CM_CRT_LENGTH_CDECL cm_crt_length(const char *pointer, uint32_t limit);
#endif
