/* Preferred-image VA 007f6cd0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_007f6cd0(u32 self, s8 kind, volatile u32 second)
{
    u8 work[0x200];
    u32 record, active, index, total, limit, base, line, version;
    void *name, *extension;
    if (kind < 0 || kind > 9) {
        name = work;
        extension = work + 0x100;
        line = 0xffu;
        goto error;
    }
    record = second;
    if (record == 0) {
        active = (u32)(s32)(s16)W(0x00b77c7au);
        record = D(0x00b74c26u + active * 768u);
        if (record == 0) {
            name = work + 0x100;
            extension = work;
            line = 0x109u;
            goto error;
        }
    }
    index = D(record);
    total = D(0x00ae2364u);
    index = (index - total + 16u) * 10u + (u32)(s32)kind;
    if ((s32)index < 0)
        goto index_error;
    limit = D(self + 8u) - 1u;
    if ((s32)index > (s32)limit)
        goto index_error;
    base = D(self);
    return base + index * 688u;
index_error:
    name = work;
    extension = work + 0x100;
    line = 0x113u;
error:
    cm_fn_00944cff(0x00a7b094u, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
