#ifndef CM_CRT_CLASSIFY_H
#define CM_CRT_CLASSIFY_H
#include <stdint.h>
#if defined(__GNUC__) && defined(__i386__)
#define CM_CRT_CLASSIFY_CDECL __attribute__((cdecl))
#else
#error "Recovered classification frame requires 32-bit x86 GCC"
#endif
/* Primary VA0094C322. Incoming ECX supplies retained local bytes.
 * The original routine can mutate its incoming value high WORD through
 * external VA0094ED85; that dependency and locale table pointer remain explicit.
 * Normal-return validation excludes Windows locale/SEH and arbitrary faults. */
uint32_t CM_CRT_CLASSIFY_CDECL cm_crt_classify(uint32_t value, uint32_t mask);
#endif
