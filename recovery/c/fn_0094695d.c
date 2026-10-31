/* Preferred-image VA 0x0094695d, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
typedef u32 (CM_STDCALL *find_close_fn)(u32 handle);
u32 CM_CDECL cm_fn_0094695d(u32 handle)
{
    u32 result = ((find_close_fn)CM_GLOBAL(volatile u32, 0x00967184u))(handle);
    if (!result) {
        CM_GLOBAL(volatile u32, 0x00de2f8cu) = 0x16u;
        return 0xffffffffu;
    }
    return 0;
}
