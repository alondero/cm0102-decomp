#include "cmre.h"

/* Preferred-image VA 0x005b3c70, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Grid coordinates occupy the signed low and high words of one DWORD. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

u32 CM_THISCALL cm_fn_005b3c70(u32 self, s8 index, s8 alternate,
                             u32 grid, u32 output)
{
    u8 scratch[512];
    u8 *filename, *extension;
    u32 line, slot, value;
    s16 x, y;
    char *version;

    if (index < 0 || index > 10) {
        filename = scratch; extension = scratch + 256; line = 0x356;
        goto error;
    }
    if (alternate < 0 || alternate > 1) {
        filename = scratch; extension = scratch + 256; line = 0x35e;
        goto error;
    }
    x = (s16)grid;
    if (x < 0 || x > 2) goto invalid_grid;
    y = (s16)(grid >> 16);
    if (y < 0 || y > 3) goto invalid_grid;
    slot = ((u32)alternate + (u32)index * 2u) * 3u + (u32)x;
    slot = (u32)y + slot * 4u;
    value = *(volatile u32 *)(self + slot * 4u + 0x164u);
    *(volatile u32 *)output = value;
    return 1;

invalid_grid:
    filename = scratch + 256; extension = scratch; line = 0x367;
error:
    cm_fn_00944cff(0x009cc3dcu, 0, 0, filename, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version,
                  filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0u,
                  (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
    return 0;
}
