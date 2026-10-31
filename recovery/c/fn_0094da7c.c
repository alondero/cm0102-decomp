/* Preferred-image VA 0x0094da7c, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern unsigned long long CM_CDECL __attribute__((regparm(2))) cm_fn_0094e3c4(u32 eax, u32 edx, u32 c, u32 stream);
unsigned long long CM_CDECL __attribute__((regparm(2))) cm_fn_0094da7c(
    u32 incoming_eax, u32 incoming_edx, u32 c, u32 stream)
{
    if (c == 0xffffffffu) return ((unsigned long long)incoming_edx << 32) | incoming_eax;
    return cm_fn_0094e3c4(incoming_eax, incoming_edx, c, stream);
}
