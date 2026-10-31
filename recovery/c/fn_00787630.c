/* Preferred-image VA 00787630, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_007874b0(u32 self, u32 person, u32 value);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_00787630(u32 self, u32 record)
{
    u8 work[0x200];
    u32 index, total, table, row, value, start, base, i, version, result, line;
    void *name, *extension;
    if (record == 0) {
        name = work + 0x100;
        extension = work;
        line = 0x24c1u;
        goto error;
    }
    index = D(record);
    total = D(0x00ae2364u);
    index = index - total + 16u;
    if ((s32)index < 0) {
        name = work;
        extension = work + 0x100;
        line = 0x24ccu;
        goto error;
    }
    table = D(self + 8u);
    row = table + index * 12u;
    value = D(record + 0x39u);
    if (value != 0)
        cm_fn_007874b0(self, record, value);
    value = D(record + 0x24u);
    if (value != 0)
        cm_fn_007874b0(self, record, value);
    start = D(row);
    D(row + 8u) = 0;
    D(row + 4u) = start;
    result = start;
    if (start != 0xffffffffu) {
        for (i = 0; i < 100u; ++i) {
            start = D(row);
            base = D(self);
            D(base + (start + i) * 223u + 0xd6u) = 0;
            start = D(row);
            base = D(self);
            D(base + (start + i) * 223u + 0xdau) = 1;
            result = base;
        }
    }
    return result;
error:
    cm_fn_00944cff(0x00a693ecu, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return result;
}
