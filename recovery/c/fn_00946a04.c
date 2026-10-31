/* Preferred-image VA 0x00946a04, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u32 CM_CDECL cm_fn_00946a04(void)
{
    u32 state = CM_GLOBAL(volatile u32, 0x00ad6c30u);
    state = state * 0x343fdu + 0x269ec3u;
    CM_GLOBAL(volatile u32, 0x00ad6c30u) = state;
    return (state >> 16) & 0x7fffu;
}
