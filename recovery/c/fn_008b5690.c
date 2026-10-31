/* Preferred-image VA 008b5690, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_008b5690(u8 kind, u32 destination, s32 capacity)
{
    u8 work[0x200];
    u8 *name, *extension;
    u32 version, line, format;
    if (capacity < 20) {
        name = work + 0x100;
        extension = work;
        line = 0x294u;
        goto error;
    }
    if (kind == 0)
        format = 0x00a8d060u;
    else if (kind == 10)
        format = 0x00a8d034u;
    else if (kind == 25)
        format = 0x00a8d004u;
    else if (kind == 50)
        format = 0x00a8cfd4u;
    else {
        name = work;
        extension = work + 0x100;
        line = 0x2afu;
        goto error;
    }
    cm_fn_0066f4e0(destination, format, 0x00ad9c64u);
    return 1;
error:
    cm_fn_00944cff(0x00a8cfa0u, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
