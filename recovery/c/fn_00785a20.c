#include "cmre.h"
/* Preferred-image VA 0x00785a20, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_CDECL cm_fn_009466d0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

u32 CM_THISCALL cm_fn_00785a20(u32 self, s8 index, u32 destination, u32 count)
{
    u8 scratch[512];
    u8 *filename, *extension;
    u32 line;
    char *version;
    if (index < 0 || index >= 50) {
        filename = scratch;
        extension = scratch + 256;
        line = 0x1ebf;
    } else {
        if (destination && (s32)count >= 1) {
            cm_fn_009466d0(destination, self + (u32)(s32)index * 4u + 5u, count);
            *(volatile u8 *)(destination + count - 1u) = 0;
            return 1;
        }
        filename = scratch + 256;
        extension = scratch;
        line = 0x1ec7;
    }
    cm_fn_00944cff(0x00a693ec, 0, 0, filename, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    return 0;
}
