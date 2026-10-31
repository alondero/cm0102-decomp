#include "cmre.h"
/* Preferred-image VA 0x0053cb80, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_CDECL cm_fn_00540a50(u32, u32, u32);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);

u32 CM_CDECL cm_fn_0053cb80(u32 club)
{
    u8 scratch[512];
    u32 result, other;
    char *version;
    if (club == 0) {
        cm_fn_00944cff(0x009b5ac8, 0, 0, scratch + 256, scratch);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8,
                      version, scratch + 256, scratch, 0x3973);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        return 0;
    }
    result = *(volatile u32 *)(club + 0xd3u);
    if (result != 0)
        return result;
    other = cm_fn_00540a50(club, 0, 1);
    if (other == 0)
        return 0;
    return *(volatile u32 *)(other + 0xd3u);
}
