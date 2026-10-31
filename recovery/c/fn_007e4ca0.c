/* Preferred-image VA 007e4ca0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_007e4ca0(s8 kind, u32 destination, s32 capacity)
{
    u8 work[0x200];
    u8 *name, *extension;
    u32 line, version, format;
    if (destination == 0 || capacity < 5) {
        name = work;
        extension = work + 0x100;
        line = 0x12cu;
        goto error;
    }
    B(destination) = 0;
    if (kind == 1)
        format = 0x0098f880u;
    else if (kind == 2)
        format = 0x0098f868u;
    else if (kind == 3)
        format = 0x00990b98u;
    else {
        name = work + 0x100;
        extension = work;
        line = 0x14du;
        goto error;
    }
    cm_fn_0066f4e0(destination, format, 0x00ad9c64u);
    return 1;
error:
    cm_fn_00944cff(0x00a7a088u, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
