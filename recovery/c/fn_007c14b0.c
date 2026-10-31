#include "cmre.h"
/* Preferred-image VA 0x007c14b0, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);

u32 CM_STDCALL cm_fn_007c14b0(s8 kind, u32 destination, s32 capacity)
{
    u8 scratch[0x200];
    u8 *first, *second;
    u32 target, line;
    char *version;
    if (destination == 0 || capacity < 51) {
        first = scratch; second = scratch + 0x100; line = 0x3ce;
        goto error;
    }
    *(volatile u8 *)destination = 0;
    if ((u32)((s32)kind - 1) > 5u) goto invalid;
    target = CM_GLOBAL(volatile u32, 0x007c1644u + ((u32)(s32)kind - 1u) * 4u);
    switch (target) {
    case 0x007c14ec: cm_fn_0066f4e0(destination, 0x00a783dc, 0x00ad9c64); break;
    case 0x007c150d: cm_fn_0066f4e0(destination, 0x00990f4c); break;
    case 0x007c1529: cm_fn_0066f4e0(destination, 0x00990f48); break;
    case 0x007c1545: cm_fn_0066f4e0(destination, 0x00a783a4, 0x00ad9c64); break;
    case 0x007c1566: cm_fn_0066f4e0(destination, 0x00a78368, 0x00ad9c64); break;
    case 0x007c1587: cm_fn_0066f4e0(destination, 0x00a7835c); break;
    default: goto invalid;
    }
    return 1;
invalid:
    first = scratch + 0x100; second = scratch; line = 0x404;
error:
    cm_fn_00944cff(0x00a78310, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first, second, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    return 0;
}
