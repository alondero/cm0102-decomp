#include "cmre.h"

/* Preferred-image VA 0x005230e0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Allocation sizes sign-extend the supplied WORDs and then wrap as DWORDs.
   The original does not store the supplied dimensions into the object. */
extern u32 CM_FASTCALL cm_fn_00518640(u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))

u32 CM_THISCALL cm_fn_005230e0(u32 self, s16 count, s16 rounds, u32 sizes)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x204];
    u8 *filename, *extension;
    u32 allocation, base, index = 0, length, line;
    s32 limit;
    char *version;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095785bu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_00518640(self);
    D(self) = 0x00969468u;
    frame.state = 0;
    allocation = (u32)cm_fn_00944e46((u32)(s32)count * 6u);
    D(self + 0xa7u) = allocation;
    if (allocation == 0) {
        filename = scratch + 0x104; extension = scratch + 4; line = 0x7e;
        goto error;
    }
    limit = rounds;
    allocation = (u32)cm_fn_00944e46((u32)limit * 104u);
    D(self + 0xa3u) = allocation;
    if (allocation == 0) {
        filename = scratch + 4; extension = scratch + 0x104; line = 0x85;
        goto error;
    }
    allocation = (u32)cm_fn_00944e46((u32)limit * 4u);
    D(self + 0xadu) = allocation;
    if (allocation == 0) {
        filename = scratch + 4; extension = scratch + 0x104; line = 0x8d;
        goto error;
    }
    if (limit <= 0) goto done;
    do {
        length = (u32)(s32)(s16)W(sizes);
        allocation = (u32)cm_fn_00944e46(length * 53u);
        base = D(self + 0xadu);
        D(base + index * 4u) = allocation;
        base = D(self + 0xadu);
        if (D(base + index * 4u) == 0) {
            filename = scratch + 4; extension = scratch + 0x104; line = 0x97;
            goto error;
        }
        ++index;
        sizes += 2u;
    } while ((s32)index < limit);
    goto done;
error:
    cm_fn_00944cff(0x009b5944u, 0, 0, filename, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version,
                  filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0u,
                  (const char *)0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return self;
}
