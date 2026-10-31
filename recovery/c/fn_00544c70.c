#include "cmre.h"
/* Preferred-image VA 0x00544c70, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_CDECL cm_fn_00540a50(u32, u32, u32);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);

static u32 collect(u32 club, u32 output, s8 limit, u8 *count)
{
    u32 value, i;
    static const u32 offsets[] = {0xbf, 0xc3, 0xc7, 0xcb, 0xcf, 0xd3,
        0x19f, 0x1a3, 0x1a7, 0x1ab, 0x1af,
        0x1b3, 0x1b7, 0x1bb, 0x1bf, 0x1c3, 0x1c7, 0x1cb,
        0x1cf, 0x1d3, 0x1d7};
    for (i = 0; i != 21; ++i) {
        value = *(volatile u32 *)(club + offsets[i]);
        if (value != 0 && (s8)*count < limit) {
            u32 index = (u32)(s32)(s8)*count;
            ++*count;
            *(volatile u32 *)(output + index * 4u) = value;
        }
    }
    return value;
}

u32 CM_CDECL cm_fn_00544c70(u32 club, u32 output, s8 limit)
{
    u8 scratch[512], count = 0;
    u32 other, last, result;
    char *version;
    if (club == 0) {
        cm_fn_00944cff(0x009b5ac8, 0, 0, scratch + 256, scratch);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8,
                      version, scratch + 256, scratch, 0x4d49);
        result = cm_fn_005e8290((const char *)0x009870e0,
                               (const char *)0x00ae24d0, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        return result & 0xffffff00u;
    }
    collect(club, output, limit, &count);
    other = cm_fn_00540a50(club, 0, 1);
    if (other == 0)
        return count;
    last = collect(other, output, limit, &count);
    /* EAX holds the last slot read, then only AL is replaced by the count. */
    return (last & 0xffffff00u) | count;
}
