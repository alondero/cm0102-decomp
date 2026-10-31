#include "cmre.h"

/* Preferred-image VA 0x007c7240, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Both direct callers overwrite EAX immediately. Negative indices retain
   their original signed guard and wrapping address arithmetic. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

#define D(a) (*(volatile u32 *)(u32)(a))

void CM_THISCALL cm_fn_007c7240(u32 self, s32 index, u32 first, u32 second)
{
    u8 scratch[512];
    u8 *filename, *extension;
    u32 row, offset, count, capacity, data, line;
    char *version;

    capacity = D(self + 0x18u);
    if (index >= (s32)capacity) {
        filename = scratch + 256;
        extension = scratch;
        line = 0x75;
        goto error;
    }
    row = D(self);
    offset = (u32)index * 18u;
    row += offset;
    count = D(row + 8u);
    capacity = D(row + 4u);
    if ((s32)count >= (s32)capacity) {
        filename = scratch;
        extension = scratch + 256;
        line = 0x7b;
        goto error;
    }
    data = D(row);
    D(data + count * 8u) = first;
    row = D(self) + offset;
    count = D(row + 8u);
    data = D(row);
    D(data + count * 8u + 4u) = second;
    row = D(self) + offset + 8u;
    D(row) = D(row) + 1u;
    return;

error:
    cm_fn_00944cff(0x00a7853cu, 0, 0, filename, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version,
                  filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0u,
                  (const char *)0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
}
