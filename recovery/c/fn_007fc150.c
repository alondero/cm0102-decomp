/* Preferred-image VA 007fc150, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_007fc150(s8 selector, u32 destination, s32 size)
{
    u8 work[0x200];
    u32 version;
    if (selector < 0 || selector > 9)
        goto diagnostic;
    if (destination == 0 || size < 50)
        goto diagnostic;
    if (selector == 0)
        cm_fn_0066f4e0(destination, 0x00a7dd94u);
    else if (selector == 1)
        cm_fn_0066f4e0(destination, 0x00a7dd84u);
    else if (selector == 2)
        cm_fn_0066f4e0(destination, 0x00a7dd74u);
    else
        cm_fn_0066f4e0(destination, 0x00a7dd5cu, (s32)selector - 2);
    return 1;
diagnostic:
    cm_fn_00944cff(0x00a7b094u, 0, 0, work + 0x100, work);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0xd16u);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
    return 0;
}
