#include "cmre.h"
/* Preferred-image VA 0x00795a90, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

u32 CM_STDCALL cm_fn_00795a90(u32 destination, u32 size, s8 mode)
{
    u8 scratch[512];
    u8 *filename, *extension;
    u32 line, format;
    char *version;
    if (!destination) {
        filename = scratch + 256;
        extension = scratch;
        line = 0x2db;
    } else if ((s32)size < 50) {
        filename = scratch;
        extension = scratch + 256;
        line = 0x2e1;
    } else {
        if (mode == 0) format = 0x00a69a8c;
        else if (mode == 1) format = 0x00a69a98;
        else if (mode == 2) format = 0x00a69aa4;
        else return destination;
        cm_fn_0066f4e0(destination, format);
        return destination;
    }
    cm_fn_00944cff(0x00a69a18, 0, 0, filename, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    return 0;
}
