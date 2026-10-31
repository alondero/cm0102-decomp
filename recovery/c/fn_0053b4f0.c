#include "cmre.h"
/* Preferred-image VA 0x0053b4f0, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
#define G(a) CM_GLOBAL(volatile u32, a)

u32 CM_CDECL cm_fn_0053b4f0(u32 nation)
{
    u8 scratch[512];
    u8 *first, *second;
    u32 base, id, n, line;
    char *version;
    if (nation == 0) {
        first = scratch + 256; second = scratch; line = 0x3368;
        goto error;
    }
    base = G(0x00ae23b4);
    if (base == 0) {
        first = scratch; second = scratch + 256; line = 0x336f;
        goto error;
    }
    if (G(0x00ae23a8) == 0) {
        first = scratch; second = scratch + 256; line = 0x3376;
        goto error;
    }
    id = *(volatile u32 *)nation;
    n = G(0x00ae2350);
    id -= n;
    n = G(0x00ae235c);
    id += n;
    return base + id * 581u;
error:
    cm_fn_00944cff(0x009b5ac8, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first, second, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    G(0x00b67a34) = 0;
    return 0;
}
