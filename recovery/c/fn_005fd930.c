#include "cmre.h"

/* Preferred-image VA 0x005fd930, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Signed guards operate on the wrapped DWORD; record addressing wraps too. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

#define D(a) (*(volatile u32 *)(u32)(a))

u32 CM_THISCALL cm_fn_005fd930(u32 self, u32 manager)
{
    u8 scratch[512];
    u8 *filename, *extension;
    u32 index, limit, base, line;
    s32 selected;
    char *version;

    if (manager == 0) {
        selected = (s16)CM_GLOBAL(volatile u16, 0x00b77c7au);
        manager = D(0x00b74c26u + (u32)selected * 768u);
        if (manager == 0) {
            filename = scratch + 256; extension = scratch; line = 0x50d;
            goto error;
        }
    }
    index = D(manager);
    base = D(0x00ae2364u);
    index = index - base + 16u;
    if ((s32)index < 0) goto invalid_index;
    limit = D(self + 4u) - 1u;
    if ((s32)index > (s32)limit) goto invalid_index;
    base = D(self);
    return base + index * 0xa6efu + 0x9a09u;

invalid_index:
    filename = scratch; extension = scratch + 256; line = 0x519;
error:
    cm_fn_00944cff(0x009cf11cu, 0, 0, filename, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version,
                  filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0u,
                  (const char *)0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
