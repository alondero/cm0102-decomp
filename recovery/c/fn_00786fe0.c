/* Preferred-image VA 00786fe0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_THISCALL cm_fn_00785670(u32 self);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_00786fe0(u32 self, u32 record, volatile u8 kind)
{
    u8 work[0x204];
    u32 index, total, table, row, value, i, start, base, address, result, version, line;
    void *name, *extension;
    if (record == 0) {
        name = work + 0x104;
        extension = work + 4;
        line = 0x230eu;
        goto error;
    }
    index = D(record);
    total = D(0x00ae2364u);
    index = index - total + 16u;
    if ((s32)index < 0) {
        name = work + 4;
        extension = work + 0x104;
        line = 0x2319u;
        goto error;
    }
    table = D(self + 8u);
    row = table + index * 12u;
    value = D(row + 8u);
    if ((s32)value < 0)
        D(row + 8u) = 0;
    if (kind == 0)
        return D(row + 8u);
    if (D(row) == 0xffffffffu)
        return 0;
    D((u32)work) = 0;
    for (i = 0; i < 100u; ++i) {
        start = D(row);
        base = D(self);
        address = base + (start + i) * 223u;
        value = D(address + 0xd6u);
        if (value != 0) {
            result = (u32)cm_fn_00785670(address);
            if ((u8)result == kind && D(address + 0xdau) == 0) {
                value = D((u32)work);
                D((u32)work) = value + 1u;
            }
        }
    }
    return D((u32)work);
error:
    cm_fn_00944cff(0x00a693ecu, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
