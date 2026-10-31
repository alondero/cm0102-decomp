/* Preferred-image VA 0x00945bd2, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
typedef u32 (CM_STDCALL *create_directory_945bd2)(u32, u32);
typedef u32 (CM_STDCALL *last_error_945bd2)(void);
extern unsigned long long CM_CDECL cm_fn_0094b2a0(u32 error);
u32 CM_CDECL cm_fn_00945bd2(u32 path)
{
    u32 error;
    if (((create_directory_945bd2)CM_GLOBAL(volatile u32, 0x00967174u))(path, 0)) return 0;
    error = ((last_error_945bd2)CM_GLOBAL(volatile u32, 0x009670e8u))();
    if (!error) return 0;
    cm_fn_0094b2a0(error);
    return 0xffffffffu;
}
