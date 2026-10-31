#include "cmre.h"
/* Preferred-image VA 0x00540340, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);

u32 CM_CDECL cm_fn_00540340(u32 first_date, u32 second_date, u32 output, s32 size)
{
    u8 scratch[512];
    u8 *first, *second;
    u32 line, difference;
    s16 day;
    char *version;
    if (output == 0 || size < 5) {
        first = scratch; second = scratch + 256; line = 0x423e;
        goto error;
    }
    *(volatile u8 *)output = 0;
    if (first_date == 0) {
        first = scratch + 256; second = scratch; line = 0x4249;
        goto error;
    }
    if (second_date == 0) {
        first = scratch; second = scratch + 256; line = 0x4251;
        goto error;
    }
    difference = *(volatile u16 *)(first_date + 2u);
    difference -= *(volatile u16 *)(second_date + 2u);
    day = *(volatile s16 *)second_date;
    if (day > *(volatile s16 *)first_date)
        --difference;
    cm_fn_00944c9f(output, 0x0098806c, difference & 0xffffu);
    return 1;
error:
    cm_fn_00944cff(0x009b5ac8, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first, second, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    return 0;
}
