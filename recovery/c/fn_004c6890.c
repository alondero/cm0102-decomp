/* Preferred-image VA 0x004C6890, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: stdcall, one pointer argument, ret 4. A null argument returns it
   (EAX = 0). Otherwise the dword it points to is compared, in this order, with
   the globals at 0x009D00AC, 00D0, 00B4, 00D8, 00B0, 026C, 0270, 0274, 0278,
   0358, 00E0, 0354, 0280, 0284, 0288, 0298, 028C, 0290, 0294 and 027C (all
   0x009Dxxxx); the first match returns 1, no match returns 0.
   Field and global meanings remain unknown. */
#include "cmre.h"

u32 CM_STDCALL cm_fn_004c6890(u32 *arg)
{
    u32 value;

    if (arg == 0) {
        return 0;
    }
    value = *arg;
    if (value == CM_GLOBAL(u32, 0x009d00ac) || value == CM_GLOBAL(u32, 0x009d00d0)
        || value == CM_GLOBAL(u32, 0x009d00b4) || value == CM_GLOBAL(u32, 0x009d00d8)
        || value == CM_GLOBAL(u32, 0x009d00b0) || value == CM_GLOBAL(u32, 0x009d026c)
        || value == CM_GLOBAL(u32, 0x009d0270) || value == CM_GLOBAL(u32, 0x009d0274)
        || value == CM_GLOBAL(u32, 0x009d0278) || value == CM_GLOBAL(u32, 0x009d0358)
        || value == CM_GLOBAL(u32, 0x009d00e0) || value == CM_GLOBAL(u32, 0x009d0354)
        || value == CM_GLOBAL(u32, 0x009d0280) || value == CM_GLOBAL(u32, 0x009d0284)
        || value == CM_GLOBAL(u32, 0x009d0288) || value == CM_GLOBAL(u32, 0x009d0298)
        || value == CM_GLOBAL(u32, 0x009d028c) || value == CM_GLOBAL(u32, 0x009d0290)
        || value == CM_GLOBAL(u32, 0x009d0294) || value == CM_GLOBAL(u32, 0x009d027c)) {
        return 1;
    }
    return 0;
}
