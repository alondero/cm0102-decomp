/* Preferred-image VA 0x0090c510, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern unsigned long long CM_CDECL __attribute__((regparm(3))) cm_fn_00947299(u32 eax, u32 edx, u32 ecx);
unsigned long long CM_CDECL __attribute__((regparm(3))) cm_fn_0090c510(u32 eax, u32 edx, u32 ecx)
{
    return cm_fn_00947299(eax, edx, ecx);
}
