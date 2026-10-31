#include "cmre.h"
/* Preferred-image VA 0x00540520, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
#define G(a) CM_GLOBAL(volatile u32, a)

u32 CM_CDECL cm_fn_00540520(u32 object)
{
    u8 scratch[512];
    u32 cached = G(0x009cf1e4);
    u32 id = *(volatile u32 *)object;
    u32 low, result;
    char *version;
    if (id == cached || id == G(0x009cf1e8)) low = 10;
    else if (id == G(0x009cf178) || id == G(0x009cf1d0)) low = 1;
    else if (id == G(0x009cf17c)) low = 2;
    else if (id == G(0x009cf180) || id == G(0x009cf184)
             || id == G(0x009cf188)) low = 3;
    else if (id == G(0x009cf18c) || id == G(0x009cf190)
             || id == G(0x009cf194) || id == G(0x009cf198)) low = 8;
    else if (id == G(0x009cf19c) || id == G(0x009cf1a0)) low = 9;
    else if (id == G(0x009cf1f0) || id == G(0x009cf1a4)
             || id == G(0x009cf1a8) || id == G(0x009cf1fc)) low = 4;
    else if (id == G(0x009cf1ac) || id == G(0x009cf1b0)
             || id == G(0x009cf1b4) || id == G(0x009cf1b8)
             || id == G(0x009cf1bc)) low = 5;
    else if (id == G(0x009cf1c0) || id == G(0x009cf1c4)
             || id == G(0x009cf1c8) || id == G(0x009cf1cc)
             || id == G(0x009cf1f4) || id == G(0x009cf1f8)) low = 6;
    else if (id == G(0x009cf1d4) || id == G(0x009cf1d8)
             || id == G(0x009cf1dc) || id == G(0x009cf1e0)
             || id == G(0x009cf1ec)) low = 7;
    else {
        cm_fn_00944cff(0x009b5ac8, 0, 0, scratch + 256, scratch);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8,
                      version, scratch + 256, scratch, 0x42c5);
        result = cm_fn_005e8290((const char *)0x009870e0,
                               (const char *)0x00ae24d0, 0);
        G(0x00b67a34) = 0;
        return (result & 0xffffff00u) | 2u;
    }
    return (id & 0xffffff00u) | low;
}
