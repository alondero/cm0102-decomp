/* Preferred-image VA 0x00946e84, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
typedef u32 (CM_STDCALL *drive_type_fn)(const char *root);
u32 CM_CDECL cm_fn_00946e84(u32 drive)
{
    if (drive) {
        volatile u8 *p = (volatile u8 *)&drive;
        u32 result;
        p[3] &= 0;
        p[0] = (u8)(p[0] + 0x40u);
        p[1] = 0x3a;
        p[2] = 0x5c;
        result = ((drive_type_fn)CM_GLOBAL(volatile u32, 0x00967100u))((const char *)&drive);
        if (result == 0 || result == 1)
            return 0;
    }
    return 1;
}
