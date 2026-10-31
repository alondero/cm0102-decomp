/* Preferred-image VA 005b0010, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_00932c30(void *destination, u32 size, u32 count, u32 stream);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_005b0010(u32 self, u32 stream)
{
    u8 work[0x204];
    u32 index, total, base, end, destination, result, version, line;
    void *name, *extension;
    if (cm_fn_00932c30(work, 4, 1, stream) != 1) {
        name = work + 0x104;
        extension = work + 4;
        line = 0x230u;
        goto error;
    }
    index = D((u32)work);
    if ((s32)index < 0)
        goto index_error;
    total = D(0x00ae2364u);
    if ((s32)index > (s32)total)
        goto index_error;
    base = D(0x00ae23bcu);
    end = base + index * 110u;
    destination = D(self);
    D(self + 4u) = end;
    total = D(0x00ae2364u);
    result = cm_fn_00932c30((void *)destination, 9, total, stream);
    total = D(0x00ae2364u);
    if (result == total)
        return 1;
    name = work + 4;
    extension = work + 0x104;
    line = 0x23eu;
    goto error;
index_error:
    name = work + 4;
    extension = work + 0x104;
    line = 0x235u;
error:
    cm_fn_00944cff(0x009cc27cu, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
