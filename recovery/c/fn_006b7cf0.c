#include "cmre.h"
/* Preferred-image VA 0x006b7cf0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Comparisons write AL only; retain the pointer or reporter above that byte. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile s8 *)(u32)(a))

u32 CM_CDECL cm_fn_006b7cf0(u32 report, u32 club)
{
    u8 scratch[512];
    u8 *filename, *extension;
    u32 eax, line, first;
    s8 left, right;
    char *version;
    if (!report || !club) {
        filename = scratch;
        extension = scratch + 256;
        line = 0xced;
        goto error;
    }
    first = D(report + 0x1c);
    if (first != club) {
        if (D(report + 0x20) != club) {
            filename = scratch + 256;
            extension = scratch;
            line = 0xcfb;
            goto error;
        }
    }
    left = B(report + 0x49);
    if (left < 0) {
        left = B(report + 0x47);
        if (left < 0) {
            left = B(report + 0x43);
            if (left < 0) return report & 0xffffff00u;
            right = B(report + 0x44);
        } else right = B(report + 0x48);
    } else right = B(report + 0x4a);
    return (report & 0xffffff00u) | (u32)(first == club ? left > right : left < right);
error:
    cm_fn_00944cff(0x00a15708, 0, 0, filename, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, filename, extension, line);
    eax = cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    return eax & 0xffffff00u;
}
