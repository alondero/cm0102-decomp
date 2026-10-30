/* Preferred-image VA 0x0094ab27, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_009488e0(u32 source);
extern unsigned long long CM_CDECL cm_fn_0094f0f0(u32 destination, u32 source, u32 size);
unsigned long long CM_CDECL __attribute__((regparm(2))) cm_fn_0094ab27(
    u32 incoming_eax, u32 incoming_edx, u32 source, u32 offset)
{
    u32 length;
    if (!offset) return ((unsigned long long)incoming_edx << 32) | incoming_eax;
    length = cm_fn_009488e0(source);
    return cm_fn_0094f0f0(source + offset, source, length + 1u);
}
