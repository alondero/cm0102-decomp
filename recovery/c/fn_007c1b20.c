#include "cmre.h"
/* Preferred-image VA 0x007c1b20, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);

u32 CM_STDCALL cm_fn_007c1b20(s8 kind, u32 destination, s32 capacity)
{
    u8 scratch[0x200];
    u8 *first, *second;
    u32 target, format, line;
    char *version;
    if (destination == 0 || capacity < 51) {
        first = scratch; second = scratch + 0x100; line = 0x5bc;
        goto error;
    }
    *(volatile u8 *)destination = 0;
    if ((u32)((s32)kind - 1) > 16u) goto invalid;
    target = CM_GLOBAL(volatile u32, 0x007c1de4u + ((u32)(s32)kind - 1u) * 4u);
    switch (target) {
    case 0x007c1b5c: format = 0x00a78494; break;
    case 0x007c1b78: format = 0x00a78490; break;
    case 0x007c1b94:
        cm_fn_0066f4e0(destination, 0x00a78464, 0x00ad9c64);
        return 1;
    case 0x007c1bb5: format = 0x00990e40; break;
    case 0x007c1bd1: format = 0x00a7845c; break;
    case 0x007c1bed: format = 0x00a78454; break;
    case 0x007c1c09: format = 0x00a7844c; break;
    case 0x007c1c25:
        cm_fn_0066f4e0(destination, 0x00990b1c, 0x00ad9c64);
        return 1;
    case 0x007c1c46: format = 0x00a78444; break;
    case 0x007c1c62: format = 0x0098e978; break;
    case 0x007c1c7e: format = 0x00a78440; break;
    case 0x007c1c9a: format = 0x00a7843c; break;
    case 0x007c1cb6: format = 0x00a78434; break;
    case 0x007c1cd2: format = 0x00a7842c; break;
    case 0x007c1cee: format = 0x00a161c8; break;
    case 0x007c1d0a: format = 0x00a78428; break;
    case 0x007c1d26: format = 0x00a78420; break;
    default: goto invalid;
    }
    cm_fn_0066f4e0(destination, format);
    return 1;
invalid:
    first = scratch + 0x100; second = scratch; line = 0x63f;
error:
    cm_fn_00944cff(0x00a78310, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first, second, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    return 0;
}
