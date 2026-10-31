/* Preferred-image VA 0x0094860a, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
typedef u32 (CM_STDCALL *codepage_fn)(void);
u32 CM_CDECL cm_fn_0094860a(u32 codepage)
{
    CM_GLOBAL(volatile u32, 0x00de3004) &= 0;
    if (codepage == (u32)-2) {
        CM_GLOBAL(volatile u32, 0x00de3004) = 1;
        return ((codepage_fn)CM_GLOBAL(volatile u32, 0x00967120))();
    }
    if (codepage == (u32)-3) {
        CM_GLOBAL(volatile u32, 0x00de3004) = 1;
        return ((codepage_fn)CM_GLOBAL(volatile u32, 0x00967124))();
    }
    if (codepage == (u32)-4) {
        codepage = CM_GLOBAL(volatile u32, 0x00de3058);
        CM_GLOBAL(volatile u32, 0x00de3004) = 1;
    }
    return codepage;
}
