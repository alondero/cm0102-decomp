/* Preferred-image VA 0x009478d8, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
typedef u32 (CM_STDCALL *attributes_fn)(u32 path);
typedef u32 (CM_STDCALL *last_error_fn)(void);
extern unsigned long long CM_CDECL cm_fn_0094b2a0(u32 error);
u32 CM_CDECL cm_fn_009478d8(u32 path, u32 mode)
{
    u32 attrs = ((attributes_fn)CM_GLOBAL(volatile u32, 0x0096713cu))(path);
    if (attrs == 0xffffffffu) {
        u32 error = ((last_error_fn)CM_GLOBAL(volatile u32, 0x009670e8u))();
        cm_fn_0094b2a0(error);
        return 0xffffffffu;
    }
    if ((attrs & 1u) && (mode & 2u)) {
        CM_GLOBAL(volatile u32, 0x00de2f8cu) = 13;
        CM_GLOBAL(volatile u32, 0x00de2f90u) = 5;
        return 0xffffffffu;
    }
    return 0;
}
