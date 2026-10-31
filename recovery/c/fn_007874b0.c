/* Preferred-image VA 007874b0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_THISCALL cm_fn_00784100(void *self, u32 person, u32 value);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_007874b0(u32 self, u32 person, u32 value)
{
    u8 work[0x200];
    u8 *name, *extension;
    u32 line, version, id, total, index, table, row, first, cursor, base, record, next;
    if (person == 0) {
        name = work + 0x100;
        extension = work;
        line = 0x2460u;
        goto diagnostic;
    }
    id = D(person);
    total = D(0x00ae2364u);
    index = id - total + 16u;
    if ((s32)index < 0) {
        name = work;
        extension = work + 0x100;
        line = 0x246bu;
        goto diagnostic;
    }
    table = D(self + 8u);
    row = table + index * 12u;
    first = D(row);
    if (first == 0xffffffffu)
        return 1;
    cursor = D(row + 4u) - 1u;
    do {
        first = D(row);
        if ((s32)cursor < (s32)first)
            cursor = first + 99u;
        base = D(self);
        record = base + cursor * 223u;
        if (D(record + 0xd6u) == 0)
            break;
        cm_fn_00784100((void *)record, person, value);
        next = D(row + 4u);
        --cursor;
        --next;
    } while (cursor != next);
    return 1;
diagnostic:
    cm_fn_00944cff(0x00a693ecu, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
