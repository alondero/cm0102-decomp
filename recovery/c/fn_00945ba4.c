/* Preferred-image VA 0x00945ba4, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
typedef u32 (CM_STDCALL *move_file_fn)(u32 from, u32 to);
typedef u32 (CM_STDCALL *last_error_fn)(void);
extern unsigned long long CM_CDECL cm_fn_0094b2a0(u32 error);
u32 CM_CDECL cm_fn_00945ba4(u32 from, u32 to)
{
    u32 result = ((move_file_fn)CM_GLOBAL(volatile u32, 0x00967170u))(from, to);
    u32 error = result ? 0 : ((last_error_fn)CM_GLOBAL(volatile u32, 0x009670e8u))();
    if (error) {
        cm_fn_0094b2a0(error);
        return 0xffffffffu;
    }
    return 0;
}
